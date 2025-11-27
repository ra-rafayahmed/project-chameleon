Shader "Custom/HyperRealisticCinematicWaterURP"
{
    Properties
    {
        _Color ("Water Color", Color) = (0, 0.5, 0.7, 0.8)
        _DepthColor ("Deep Water Color", Color) = (0, 0.2, 0.4, 1)
        _WaveA ("Wave A (dir x,y, steepness, wavelength)", Vector) = (1,0,0.5,10)
        _WaveB ("Wave B (dir x,y, steepness, wavelength)", Vector) = (0,1,0.25,20)
        _WaveC ("Wave C (dir x,y, steepness, wavelength)", Vector) = (1,1,0.15,10)
        _NormalMap1 ("Normal Map 1", 2D) = "bump" {}
        _NormalMap2 ("Normal Map 2", 2D) = "bump" {}
        _NormalStrength ("Normal Strength", Float) = 1.0
        _FoamTex ("Foam Texture", 2D) = "white" {}
        _FoamAmount ("Foam Amount", Float) = 0.5
        _FoamDistance ("Foam Distance", Float) = 0.1
        _FoamCrestThreshold ("Foam Crest Threshold", Float) = 0.5
        _CausticsTex ("Caustics Texture", 2D) = "white" {}
        _CausticsStrength ("Caustics Strength", Float) = 0.5
        _CausticsSpeed ("Caustics Speed", Float) = 0.5
        _Smoothness ("Smoothness", Range(0, 1)) = 0.9
        _SpecularStrength ("Specular Strength", Float) = 0.5
        _RefractionStrength ("Refraction Strength", Float) = 0.02
        _DepthFade ("Depth Fade", Float) = 1.0
        _FresnelPower ("Fresnel Power (Bloom-like Rim)", Float) = 5.0
        _FresnelStrength ("Fresnel Strength", Float) = 1.0
        _TessellationUniform ("Tessellation Uniform Factor", Range(1, 64)) = 10
        _TessellationMinDist ("Tessellation Min Distance", Float) = 10
        _TessellationMaxDist ("Tessellation Max Distance", Float) = 50
    }
    SubShader
    {
        Tags
        {
            "RenderType" = "Transparent"
            "Queue" = "Transparent"
            "RenderPipeline" = "UniversalPipeline"
            "IgnoreProjector" = "True"
        }
        LOD 300

        Pass
        {
            Name "ForwardLit"
            Tags { "LightMode" = "UniversalForward" }

            Blend SrcAlpha OneMinusSrcAlpha
            ZWrite Off
            Cull Back

            HLSLPROGRAM
            #pragma target 4.6  // Required for tessellation
            #pragma vertex TessVertex
            #pragma hull hull
            #pragma domain domain
            #pragma fragment frag
            #pragma multi_compile_fog
            #pragma multi_compile _ _MAIN_LIGHT_SHADOWS _MAIN_LIGHT_SHADOWS_CASCADE
            #pragma multi_compile _ _SHADOWS_SOFT
            #pragma multi_compile _ _ADDITIONAL_LIGHTS

            #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Core.hlsl"
            #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"
            #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/DeclareDepthTexture.hlsl"

            // Define UNITY_PI manually
            #define PI 3.14159265359

            // Declare textures and properties
            TEXTURE2D(_NormalMap1);
            SAMPLER(sampler_NormalMap1);
            TEXTURE2D(_NormalMap2);
            SAMPLER(sampler_NormalMap2);
            TEXTURE2D(_FoamTex);
            SAMPLER(sampler_FoamTex);
            TEXTURE2D(_CausticsTex);
            SAMPLER(sampler_CausticsTex);
            TEXTURE2D(_CameraOpaqueTexture);
            SAMPLER(sampler_CameraOpaqueTexture);

            CBUFFER_START(UnityPerMaterial)
                float4 _Color;
                float4 _DepthColor;
                float4 _WaveA;
                float4 _WaveB;
                float4 _WaveC;
                float _NormalStrength;
                float _FoamAmount;
                float _FoamDistance;
                float _FoamCrestThreshold;
                float _CausticsStrength;
                float _CausticsSpeed;
                float _Smoothness;
                float _SpecularStrength;
                float _RefractionStrength;
                float _DepthFade;
                float _FresnelPower;
                float _FresnelStrength;
                float _TessellationUniform;
                float _TessellationMinDist;
                float _TessellationMaxDist;
                float4 _NormalMap1_ST;
                float4 _NormalMap2_ST;
                float4 _FoamTex_ST;
                float4 _CausticsTex_ST;
            CBUFFER_END

            struct Attributes
            {
                float4 positionOS : POSITION;
                float2 uv : TEXCOORD0;
                float3 normalOS : NORMAL;
            };

            struct Varyings
            {
                float4 positionCS : SV_POSITION;
                float2 uv : TEXCOORD0;
                float2 foamUV : TEXCOORD1;
                float4 screenPos : TEXCOORD2;
                float3 worldPos : TEXCOORD3;
                float3 normalWS : TEXCOORD4;
                float3 viewDirWS : TEXCOORD5;
                float fogFactor : TEXCOORD6;
                float waveHeight : TEXCOORD7;
            };

            struct TessControlPoint
            {
                float4 positionOS : INTERNALTESSPOS;
                float2 uv : TEXCOORD0;
                float3 normalOS : NORMAL;
            };

            // Gerstner Wave function
            float3 GerstnerWave(float4 wave, float3 p, inout float3 tangent, inout float3 binormal)
            {
                float steepness = wave.z;
                float wavelength = wave.w;
                float k = 2 * PI / wavelength;
                float c = sqrt(9.8 / k);
                float2 d = normalize(wave.xy);
                float f = k * (dot(d, p.xz) - c * _Time.y);
                float a = steepness / k;

                tangent += float3(
                    -d.x * d.x * (steepness * sin(f)),
                    steepness * cos(f) * d.x,
                    -d.x * d.y * (steepness * sin(f))
                );
                binormal += float3(
                    -d.x * d.y * (steepness * sin(f)),
                    steepness * cos(f) * d.y,
                    -d.y * d.y * (steepness * sin(f))
                );
                return float3(
                    d.x * (a * cos(f)),
                    a * sin(f),
                    d.y * (a * cos(f))
                );
            }

            // Tessellation vertex (pre-tessellation)
            TessControlPoint TessVertex(Attributes input)
            {
                TessControlPoint output;
                output.positionOS = input.positionOS;
                output.uv = input.uv;
                output.normalOS = input.normalOS;
                return output;
            }

            // Patch constant function for tessellation factors
            [patchconstantfunc("PatchConstantFunc")]
            struct PatchTess
            {
                float edgeTess[3] : SV_TessFactor;
                float insideTess : SV_InsideTessFactor;
            };

            PatchTess PatchConstantFunc(InputPatch<TessControlPoint, 3> patch)
            {
                PatchTess pt;
                float3 p0 = mul(UNITY_MATRIX_M, patch[0].positionOS).xyz;
                float3 p1 = mul(UNITY_MATRIX_M, patch[1].positionOS).xyz;
                float3 p2 = mul(UNITY_MATRIX_M, patch[2].positionOS).xyz;

                float dist = distance(GetCameraPositionWS(), (p0 + p1 + p2) / 3.0);
                float tessFactor = lerp(_TessellationUniform, 1.0, saturate((dist - _TessellationMinDist) / (_TessellationMaxDist - _TessellationMinDist)));

                pt.edgeTess[0] = tessFactor;
                pt.edgeTess[1] = tessFactor;
                pt.edgeTess[2] = tessFactor;
                pt.insideTess = tessFactor;
                return pt;
            }

            // Hull shader
            [domain("tri")]
            [partitioning("fractional_odd")]
            [outputtopology("triangle_cw")]
            [outputcontrolpoints(3)]
            [patchconstantfunc("PatchConstantFunc")]
            TessControlPoint hull(InputPatch<TessControlPoint, 3> patch, uint id : SV_OutputControlPointID)
            {
                return patch[id];
            }

            // Domain shader (applies displacement after tessellation)
            [domain("tri")]
            Varyings domain(PatchTess patchTess, float3 bary : SV_DomainLocation, const OutputPatch<TessControlPoint, 3> patch)
            {
                Varyings output;

                // Barycentric interpolation
                float3 positionOS = bary.x * patch[0].positionOS.xyz + bary.y * patch[1].positionOS.xyz + bary.z * patch[2].positionOS.xyz;
                float2 uv = bary.x * patch[0].uv + bary.y * patch[1].uv + bary.z * patch[2].uv;
                float3 normalOS = bary.x * patch[0].normalOS + bary.y * patch[1].normalOS + bary.z * patch[2].normalOS;

                // Apply Gerstner waves displacement
                float3 tangent = float3(1, 0, 0);
                float3 binormal = float3(0, 0, 1);
                float3 p = positionOS;
                p += GerstnerWave(_WaveA, positionOS, tangent, binormal);
                p += GerstnerWave(_WaveB, positionOS, tangent, binormal);
                p += GerstnerWave(_WaveC, positionOS, tangent, binormal);
                float3 normal = normalize(cross(binormal, tangent));

                // Update position and normal
                VertexPositionInputs vertexInput = GetVertexPositionInputs(p);
                output.positionCS = vertexInput.positionCS;
                output.worldPos = vertexInput.positionWS;
                output.uv = uv;
                output.foamUV = uv;
                output.screenPos = ComputeScreenPos(vertexInput.positionCS);
                output.normalWS = TransformObjectToWorldNormal(normal);
                output.viewDirWS = GetWorldSpaceViewDir(vertexInput.positionWS);
                output.fogFactor = ComputeFogFactor(vertexInput.positionCS.z);
                output.waveHeight = p.y;

                return output;
            }

            // Fragment shader
            half4 frag(Varyings input) : SV_Target
            {
                // Wave animation for detail normals
                float time = _Time.y;
                float2 waveOffset1 = float2(time, time * 0.7) * 0.1;
                float2 waveOffset2 = float2(-time * 0.8, time * 0.5) * 0.15;
                float2 uv1 = input.uv + waveOffset1;
                float2 uv2 = input.uv + waveOffset2;

                // Combine detail normals
                half3 normal1 = UnpackNormal(SAMPLE_TEXTURE2D(_NormalMap1, sampler_NormalMap1, uv1));
                half3 normal2 = UnpackNormal(SAMPLE_TEXTURE2D(_NormalMap2, sampler_NormalMap2, uv2));
                half3 detailNormal = normalize(lerp(normal1, normal2, 0.5));
                detailNormal.z *= _NormalStrength;

                // Combine with Gerstner normal
                half3 normalWS = normalize(input.normalWS + detailNormal);

                // Refraction
                float2 screenUV = input.screenPos.xy / input.screenPos.w;
                float2 refractUV = screenUV + detailNormal.xy * _RefractionStrength;
                half3 grabColor = SAMPLE_TEXTURE2D(_CameraOpaqueTexture, sampler_CameraOpaqueTexture, refractUV).rgb;

                // Depth-based color and intersection foam
                float sceneDepth = LinearEyeDepth(SAMPLE_TEXTURE2D(_CameraDepthTexture, sampler_CameraDepthTexture, screenUV).r, _ZBufferParams);
                float waterDepth = LinearEyeDepth(input.positionCS.z / input.positionCS.w, _ZBufferParams);
                float depthDiff = sceneDepth - waterDepth;
                float depth = saturate(depthDiff / _DepthFade);
                half4 waterColor = lerp(_Color, _DepthColor, depth);

                // Intersection foam
                half foamIntersection = saturate(1.0 - depthDiff / _FoamDistance) * _FoamAmount;

                // Crest foam based on wave height
                half foamCrest = step(_FoamCrestThreshold, input.waveHeight) * _FoamAmount;

                // Combine foams
                half foam = SAMPLE_TEXTURE2D(_FoamTex, sampler_FoamTex, input.foamUV + waveOffset1).r * (foamIntersection + foamCrest);

                // Caustics
                float2 causticsUV = input.uv + waveOffset1 * _CausticsSpeed;
                half caustics = SAMPLE_TEXTURE2D(_CausticsTex, sampler_CausticsTex, causticsUV).r * _CausticsStrength * (1.0 - depth);

                // Combine colors
                half3 albedo = lerp(grabColor, waterColor.rgb, waterColor.a) + foam + caustics;
                half alpha = waterColor.a + foam;

                // Lighting with specular
                Light mainLight = GetMainLight();
                half3 lightDir = mainLight.direction;
                half3 viewDir = normalize(input.viewDirWS);
                half3 halfDir = normalize(lightDir + viewDir);
                half specular = pow(max(0, dot(normalWS, halfDir)), _Smoothness * 100) * _SpecularStrength;
                half3 lighting = mainLight.color * (max(0, dot(normalWS, lightDir)) + specular);

                // Additional lights
                #ifdef _ADDITIONAL_LIGHTS
                    uint pixelLightCount = GetAdditionalLightsCount();
                    for (uint lightIndex = 0; lightIndex < pixelLightCount; ++lightIndex)
                    {
                        Light light = GetAdditionalLight(lightIndex, input.worldPos);
                        half3 lightHalfDir = normalize(light.direction + viewDir);
                        half lightSpecular = pow(max(0, dot(normalWS, lightHalfDir)), _Smoothness * 100) * _SpecularStrength;
                        lighting += light.color * (max(0, dot(normalWS, light.direction)) + lightSpecular) * light.distanceAttenuation * light.shadowAttenuation;
                    }
                #endif

                // Fresnel for bloomy rim effect
                half fresnel = pow(1.0 - saturate(dot(normalWS, viewDir)), _FresnelPower) * _FresnelStrength;

                // Apply lighting, fresnel, and fog
                half3 finalColor = albedo * lighting + fresnel;
                finalColor = MixFog(finalColor, input.fogFactor);

                return half4(finalColor, alpha);
            }
            ENDHLSL
        }
    }
    FallBack "Universal Render Pipeline/Unlit"
}