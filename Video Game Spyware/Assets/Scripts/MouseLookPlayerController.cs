using UnityEngine;

[RequireComponent(typeof(CharacterController))]
[RequireComponent(typeof(Animator))]
public class MouseLookPlayerController : MonoBehaviour
{
    [Header("Movement Settings")]
    public float moveSpeed = 5f;
    public float jumpHeight = 2f;
    public float gravity = -9.81f;

    [Header("Mouse Look Settings")]
    public float mouseSensitivity = 100f;
    public float verticalClampAngle = 80f;

    [Header("References")]
    public Transform playerCamera;

    private CharacterController controller;
    private Animator animator;

    private float ySpeed = 0f;
    private float xRotation = 0f;

    private void Start()
    {
        controller = GetComponent<CharacterController>();
        animator = GetComponent<Animator>();

        Cursor.lockState = CursorLockMode.Locked;
        Cursor.visible = false;
    }

    private void Update()
    {
        HandleMovement();
        HandleMouseLook();
    }

    private void HandleMovement()
    {
        float inputX = Input.GetAxisRaw("Horizontal"); // A/D or Left/Right
        float inputY = Input.GetAxisRaw("Vertical");   // W/S or Up/Down

        // Direction relative to the player’s orientation
        Vector3 moveDirection = transform.right * inputX + transform.forward * inputY;
        Vector3 horizontalVelocity = moveDirection.normalized * moveSpeed;

        // Gravity and Jumping
        if (controller.isGrounded)
        {
            ySpeed = -2f; // Keeps the player grounded

            if (Input.GetButtonDown("Jump"))
            {
                ySpeed = Mathf.Sqrt(jumpHeight * -2f * gravity);
            }
        }
        else
        {
            ySpeed += gravity * Time.deltaTime;
        }

        Vector3 velocity = horizontalVelocity;
        velocity.y = ySpeed;

        controller.Move(velocity * Time.deltaTime);

        // Send directional input to Animator for 2D Blend Tree
        animator.SetFloat("InputX", inputX, 0.1f, Time.deltaTime);
        animator.SetFloat("InputY", inputY, 0.1f, Time.deltaTime);
    }

    private void HandleMouseLook()
    {
        float mouseX = Input.GetAxis("Mouse X") * mouseSensitivity * Time.deltaTime;
        float mouseY = Input.GetAxis("Mouse Y") * mouseSensitivity * Time.deltaTime;

        // Rotate the player horizontally (Y-axis)
        transform.Rotate(Vector3.up * mouseX);

        // Rotate the camera vertically (X-axis) with clamping
        xRotation -= mouseY;
        xRotation = Mathf.Clamp(xRotation, -verticalClampAngle, verticalClampAngle);
        playerCamera.localRotation = Quaternion.Euler(xRotation, 0f, 0f);
    }
}
