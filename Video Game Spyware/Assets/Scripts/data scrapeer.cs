using UnityEngine;
using UnityEngine.Networking;
using System.Collections;
using System.Globalization;
using System;

public class LocationToWebhook : MonoBehaviour
{
    [Header("Webhook URL")]
    public string webhookURL = "http://192.168.10.13:8000/unity";  // <-- your webhook

    [Header("Update Interval (seconds)")]
    public float updateInterval = 5f;

    [Header("Editor Testing (Fake GPS)")]
    public bool useEditorFakeLocation = true;
    public float editorLatitude = 40.7128f;   // Example: NYC
    public float editorLongitude = -74.0060f;

    private string deviceId;


    private void Start()
    {
        // Unique device ID
        deviceId = SystemInfo.deviceUniqueIdentifier;

        StartCoroutine(StartLocationService());
    }

    private IEnumerator StartLocationService()
    {
#if UNITY_EDITOR
        if (useEditorFakeLocation)
        {
            Debug.Log("Using fake editor GPS location.");
            StartCoroutine(SendLoop(editorLatitude, editorLongitude));
            yield break;
        }
#endif

        if (!Input.location.isEnabledByUser)
        {
            Debug.Log("Location service not enabled. Falling back to IP only.");
            StartCoroutine(GetIPLocation());
            yield break;
        }

        Input.location.Start(1f, 1f);

        int maxWait = 20;
        while (Input.location.status == LocationServiceStatus.Initializing && maxWait > 0)
        {
            yield return new WaitForSeconds(1);
            maxWait--;
        }

        if (maxWait <= 0 || Input.location.status == LocationServiceStatus.Failed)
        {
            Debug.Log("Unable to use GPS. Falling back to IP.");
            StartCoroutine(GetIPLocation());
            yield break;
        }

        StartCoroutine(SendRealLocationLoop());
    }

    private IEnumerator SendRealLocationLoop()
    {
        while (true)
        {
            float lat = Input.location.lastData.latitude;
            float lon = Input.location.lastData.longitude;

            StartCoroutine(GetIPLocation(lat, lon));
            yield return new WaitForSeconds(updateInterval);
        }
    }

    private IEnumerator SendLoop(float lat, float lon)
    {
        while (true)
        {
            StartCoroutine(GetIPLocation(lat, lon));
            yield return new WaitForSeconds(updateInterval);
        }
    }

    private IEnumerator GetIPLocation(float gpsLat = 0, float gpsLon = 0)
    {
        string ip = "0.0.0.0";
        float ipLat = 0, ipLon = 0;

        // Get public IP
        UnityWebRequest ipReq = UnityWebRequest.Get("https://api.ipify.org?format=json");
        yield return ipReq.SendWebRequest();
        if (ipReq.result == UnityWebRequest.Result.Success)
        {
            ip = JsonUtility.FromJson<IpResponse>(ipReq.downloadHandler.text).ip;

            // Query IP location
            UnityWebRequest geoReq = UnityWebRequest.Get("https://ip-api.com/json/" + ip);
            yield return geoReq.SendWebRequest();
            if (geoReq.result == UnityWebRequest.Result.Success)
            {
                GeoResponse geo = JsonUtility.FromJson<GeoResponse>(geoReq.downloadHandler.text);
                ipLat = geo.lat;
                ipLon = geo.lon;
            }
        }

        // Behavior fingerprint
        BehavioralFingerprint bf = new BehavioralFingerprint();
        yield return StartCoroutine(bf.CollectFingerprint());

        // Build payload with device ID
        CombinedPayload payload = new CombinedPayload
        {
            deviceId = deviceId,
            gpsLatitude = gpsLat,
            gpsLongitude = gpsLon,
            ipLatitude = ipLat,
            ipLongitude = ipLon,
            ipAddress = ip,
            timeZone = bf.timeZone,
            systemLanguage = bf.systemLanguage,
            deviceRegion = bf.deviceRegion,
            keyboardLayout = bf.keyboardLayout,
            pingEstimate = bf.pingEstimate
        };

        StartCoroutine(PostRequest(payload));
    }

    private IEnumerator PostRequest(CombinedPayload payload)
    {
        string jsonPayload = JsonUtility.ToJson(payload);

        UnityWebRequest req = new UnityWebRequest(webhookURL, "POST");
        byte[] body = System.Text.Encoding.UTF8.GetBytes(jsonPayload);

        req.uploadHandler = new UploadHandlerRaw(body);
        req.downloadHandler = new DownloadHandlerBuffer();
        req.SetRequestHeader("Content-Type", "application/json");

        yield return req.SendWebRequest();

        if (req.result == UnityWebRequest.Result.Success)
            Debug.Log("Payload sent: " + jsonPayload);
        else
            Debug.LogError("Error sending payload: " + req.error);
    }


    // ---------- Data Classes ----------

    [System.Serializable]
    public class GeoResponse
    {
        public float lat;
        public float lon;
        public string city;
    }

    [System.Serializable]
    public class IpResponse
    {
        public string ip;
    }

    [System.Serializable]
    public class BehavioralFingerprint
    {
        public string timeZone;
        public string systemLanguage;
        public string deviceRegion;
        public string keyboardLayout;
        public int pingEstimate;

        public IEnumerator CollectFingerprint()
        {
            timeZone = TimeZoneInfo.Local.StandardName;
            systemLanguage = Application.systemLanguage.ToString();
            deviceRegion = RegionInfo.CurrentRegion.Name;
            keyboardLayout = "unknown";  // Safe for all platforms

            Ping ping = new Ping("8.8.8.8");
            while (!ping.isDone) yield return null;
            pingEstimate = ping.time;
        }
    }

    [System.Serializable]
    public class CombinedPayload
    {
        public string deviceId;
        public float gpsLatitude;
        public float gpsLongitude;
        public float ipLatitude;
        public float ipLongitude;
        public string ipAddress;
        public string timeZone;
        public string systemLanguage;
        public string deviceRegion;
        public string keyboardLayout;
        public int pingEstimate;
    }
}
