using UnityEngine;

public class BoatFloat : MonoBehaviour
{
    [Header("Floating Settings")]
    public float floatStrength = 0.5f;     // Height of bobbing
    public float floatSpeed = 1f;          // Speed of bobbing
    public float rotationAmount = 1f;      // Rotation tilt amount
    public float rotationSpeed = 0.5f;     // Speed of tilt changes

    private Vector3 startPos;
    private Quaternion startRot;

    void Start()
    {
        startPos = transform.position;
        startRot = transform.rotation;
    }

    void Update()
    {
        // Vertical bobbing (Y-axis)
        float newY = startPos.y + Mathf.Sin(Time.time * floatSpeed) * floatStrength;

        // Slight rotation on X/Z axes
        float rotX = Mathf.Sin(Time.time * rotationSpeed) * rotationAmount;
        float rotZ = Mathf.Cos(Time.time * rotationSpeed) * rotationAmount;

        // Apply position and rotation
        transform.position = new Vector3(startPos.x, newY, startPos.z);
        transform.rotation = startRot * Quaternion.Euler(rotX, 0f, rotZ);
    }
}
