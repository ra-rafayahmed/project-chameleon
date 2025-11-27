using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;
using UnityEngine.Rendering.RendererUtils; // For RTHandle

public class Water_Volume : ScriptableRendererFeature
{
    class CustomRenderPass : ScriptableRenderPass
    {
        private Material _material;

        // Use RTHandle instead of RenderTargetIdentifier
        private RTHandle cameraColorTargetHandle;

        public CustomRenderPass(Material mat)
        {
            _material = mat;
        }

        public void Setup(RTHandle cameraColorTargetHandle)
        {
            this.cameraColorTargetHandle = cameraColorTargetHandle;
        }

        public override void Execute(ScriptableRenderContext context, ref RenderingData renderingData)
        {
            if (renderingData.cameraData.cameraType == CameraType.Reflection)
                return;

            CommandBuffer cmd = CommandBufferPool.Get("Water Volume Pass");

            var desc = renderingData.cameraData.cameraTargetDescriptor;

            // Allocate temporary RTHandle
            int tempRTId = Shader.PropertyToID("_TemporaryColourTexture");
            cmd.GetTemporaryRT(tempRTId, desc, FilterMode.Bilinear);
            RenderTargetIdentifier tempRT = new RenderTargetIdentifier(tempRTId);

            // Blit from camera color target to tempRT using material pass 0
            cmd.Blit(cameraColorTargetHandle, tempRT, _material, 0);

            // Blit back from tempRT to camera color target (copy)
            cmd.Blit(tempRT, cameraColorTargetHandle);

            // Release temp RT
            cmd.ReleaseTemporaryRT(tempRTId);

            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
    }

    [System.Serializable]
    public class _Settings
    {
        public Material material = null;
        public RenderPassEvent renderPass = RenderPassEvent.AfterRenderingSkybox;
    }

    public _Settings settings = new _Settings();

    CustomRenderPass m_ScriptablePass;

    public override void Create()
    {
        if (settings.material == null)
        {
            settings.material = Resources.Load<Material>("Water_Volume");
        }

        m_ScriptablePass = new CustomRenderPass(settings.material);
        m_ScriptablePass.renderPassEvent = settings.renderPass;
    }

    public override void AddRenderPasses(ScriptableRenderer renderer, ref RenderingData renderingData)
    {
        // Pass RTHandle instead of RenderTargetIdentifier
        m_ScriptablePass.Setup(renderer.cameraColorTargetHandle);
        renderer.EnqueuePass(m_ScriptablePass);
    }
}
