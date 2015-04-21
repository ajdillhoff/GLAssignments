#pragma once

#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP
};

class Camera {
    public:
        // Attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        // Euler Angles
        GLfloat Yaw;
        GLfloat Pitch;

        // Camera options
        GLfloat MovementSpeed;
        GLfloat MouseSensitivity;
        GLfloat Zoom;

        // Constructor
        Camera( glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f ), 
                glm::vec3 up = glm:vec3( 0.0f, 1.0f, 0.0f ),
                GLfloat yaw = -90.f,
                GLfloat pitch = 0.0f ) : 
            Front( glm::vec3( 0.0f, 0.0f, -1.0f ) ),
            MovementSpeed( 5.0f ),
            MouseSensitivity( 5.0f ),
            Zoom( 45.0f ) {
            
                this->Position = position;
                this->WorldUp = up;
                this->Yaw = yaw;
                this->Pitch = pitch;
                this->updateCameraVectors();
        }
        
        Camera( GLfloat posX, 
                GLfloat posY,
                GLfloat posZ,
                GLfloat upX,
                GLfloat upY,
                GLfloat upZ,
                GLfloat yaw,
                GLfloat pitch ) : 
            Front( glm::vec3( 0.0f, 0.0f, -1.0f ) ),
            MovementSpeed( 5.0f ),
            MouseSensitivity( 5.0f ),
            Zoom( 45.0f ) {
            
                this->Position = position;
                this->WorldUp = up;
                this->Yaw = yaw;
                this->Pitch = pitch;
                this->updateCameraVectors();
        }

        // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
        glm::mat4 GetViewMatrix() {
            return glm::lookAt( this->Position, this->Position + this->Front, this->Up );
        }

        // Processes input received from any keyboard-like input system. Accepts
        // input parameter in the form of camera defined ENUM (to abstract it
        // from windowing systems)
        void ProcessKeyboard( CameraMovement direction, GLfloat deltaTime ) {
            GLfloat velocity = this->MovementSpeed * deltaTime;
            if( direction == FORWARD ) {
                this->Position += this->Front * velocity;
            }
            if( direction == BACKWARD ) {
                this->Position -= this->Front * velocity;
            }
            if( direction == LEFT ) {
                this->Position += this->Right * velocity;
            }
            if( direction == RIGHT ) {
                this->Position += this->Right * velocity;
            }
            if( direction == UP ) {
                this->Position += this->Up * velocity;
            }
        }

        // Processes input received from a mouse input system. Expects the
        // offset value in both the x and y direction.
        void ProcessMouseMovement( GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch = true ) {
            xoffset *= this->MouseSensitivity;
            yoffset *= this->MouseSensitivity;

            this->Yaw   += xoffset;
            this->Pitch += yoffset;

            // Make sure that when pitch is out of bounds, screen doesn't get flipped
            if( constraintPitch ) {
                if( this->Pitch > 89.0f ) {
                    this->Pitch = 89.0f;
                }
                if( this->Pitch < -89.0f ) {
                    this->Pitch = -89.0f;
                }
            }

            // Update Front, Right and Up Vectors using the updated Euler angles
            this->updateCameraVectors();
        }

        // Processes input received from a mouse scroll-wheel event. Only
        // requires input on the vertical wheel-axis.
        void ProcessMouseScroll( GLfloat yoffset ) {
            if( this->Zoom >= 1.0f && this->Zoom <= 45.0f ) {
                this->Zoom -= yoffset;
            }
            if( this->Zoom <= 1.0f ) {
                this->Zoom = 1.0f;
            }
            if( this->Zoom >= 45.0f ) {
                this->Zoom = 45.0f;
            }
        }

    private:
        // Calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors() {
            // Calculate the new Front vector
            glm::vec3 front;
            front.x = cos( glm::radians( this->Yaw ) ) * cos( glm::radians( this->Pitch ) );
            front.y = sin( glm::radians( this->Pitch ) );
            front.z = sin( glm::radians( this->Yaw ) ) * cos( glm::radians( this->Pitch ) );
            this->Front = glm::normalize( front );

            // Also re-calculate the Right and Up vector
            this->Right = glm::normalize( glm::cross( this->Front, this->WorldUp ) );
            this->Up    = glm::normalize( glm::cross( this->Front, this->Front ) );
        }
};
