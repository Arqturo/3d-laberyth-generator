#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    glm::vec3 back_Position;
    glm::vec3 back_Front;
    glm::vec3 back_Up;
    glm::vec3 back_Right;
    glm::vec3 back_WorldUp;

    // euler Angles
    float Yaw;
    float back_Yaw;
    float Pitch;
    float back_Pitch;
    bool locked = true; 
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    bool isTurning = false;       // Flag indicating whether the camera is currently turning
    float targetYaw = 0.0f;      // Target yaw angle when turning
    float rotationSpeed = 90.0f; // Rotation speed in degrees per secon
    bool isMoving = false;       // Flag indicating whether the camera is currently moving
    glm::vec3 targetPosition;    // Target position when moving
   
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

  
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    
void load_backup() {
        Position = back_Position;
        WorldUp = back_Up;
        Yaw = back_Yaw;
        Pitch = back_Pitch;
        updateCameraVectors();
}

void unlock() {
        save_backup();
        locked = false;
}


void save_backup() {
    if (locked) {
        back_Position = Position;
        back_Up = WorldUp;
        back_Yaw = Yaw;
        back_Pitch = Pitch;
        updateCameraVectors();
        }
}

void lock() {
        locked = true;
        load_backup();
}

void toggle_lock() {
    if (locked == false) {
        lock();
    } else {
        unlock();
    }
}


    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    if (locked == true) {
    float velocity = MovementSpeed * deltaTime;

    // Process input only if the camera is not currently turning or moving
    if (!isTurning && !isMoving)
    {
        if (direction == FORWARD)
        {
      
            isMoving = true;
            targetPosition = Position + Front * 2.0f; 
        }
        else if (direction == BACKWARD)
        {
            
            isMoving = true;
            targetPosition = Position - Front * 2.0f; 
        }
        else if (direction == LEFT)
        {
            isTurning = true;            
            targetYaw = Yaw - 90.0f;    
        }
        else if (direction == RIGHT)
        {
            isTurning = true;            
            targetYaw = Yaw + 90.0f;     
        }
    }

    // If the camera is moving, continuously update the position until it reaches the target
    if (isMoving)
    {
        // Calculate the new position based on the movement speed
        glm::vec3 movementDirection = glm::normalize(targetPosition - Position);
        float movementDistance = glm::length(targetPosition - Position);
        float movementSpeed = 2.0f; // Movement speed in units per second

        if (movementDistance > movementSpeed * deltaTime)
        {
            if (direction == FORWARD) { 
                float faster = movementSpeed * 1.5;
                Position += movementDirection * faster * deltaTime;
             } else {
                Position += movementDirection * movementSpeed * deltaTime;
             }
        }
        else
        {
            Position = targetPosition; 
            isMoving = false; // Exit moving state
        }
    }

   
    if (isTurning)
    {
        // Calculate the new yaw angle based on the rotation speed
        float yawIncrement = rotationSpeed * deltaTime;
        if (targetYaw < Yaw)
        {
            Yaw -= yawIncrement; // Rotate left
            if (Yaw < targetYaw)
            {
                Yaw = targetYaw; 
                isTurning = false; // Exit turning state
            }
        }
        else if (targetYaw > Yaw)
        {
            Yaw += yawIncrement; // Rotate right
            if (Yaw > targetYaw)
            {
                Yaw = targetYaw; // Ensure exact alignment with the target
                isTurning = false; 
            }
        }
    }
    }

    // Update camera vectors after modifying position and yaw
    updateCameraVectors();
}


    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }




    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        
        Right = glm::normalize(glm::cross(Front, WorldUp)); 
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif