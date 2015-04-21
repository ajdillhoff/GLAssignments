// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// SOIL
#include "include/SOIL.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "Camera.hpp"
#include <iostream>

const GLuint WIDTH = 2560, HEIGHT = 1440;

// Define camera position and vectors

bool keys[1024];

// Define mouse position offset
GLfloat lastX = WIDTH / 2, lastY = HEIGHT / 2;
GLfloat yaw = -90.0f, pitch = 0.0f;
// Aspect ratio for zoom
GLfloat aspect = 45.0f;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mode ) {
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
        glfwSetWindowShouldClose( window, GL_TRUE );
    } 

    if( key >= 0 && key < 1024 ) {
        if( action == GLFW_PRESS ) {
            keys[key] = true;
        } else if( action == GLFW_RELEASE ) {
            keys[key] = false;
        }
    }
}

void mouseCallback( GLFWwindow *window, double xpos, double ypos ) {
    if( firstMouse ) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // y-coords range from bottom to top
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.15f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if( pitch > 89.0f ) {
        pitch = 89.0f;
    }
    if( pitch < -89.0f ) {
        pitch = -89.0f;
    }

    glm::vec3 front;
    front.x = cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
    front.y = sin( glm::radians( pitch ) );
    front.z = sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
    cameraFront = glm::normalize( front );
}

void scrollCallback( GLFWwindow *window, double xoffset, double yoffset ) {
    if( aspect >= 1.0f && aspect <= 45.0f ) {
        aspect -= yoffset;
    }
    if( aspect <= 1.0f ) {
        aspect = 1.0f;
    }
    if( aspect >= 45.0f ) {
        aspect = 45.0f;
    }
}

void movementAction() {
    // Camera controls
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if( keys[GLFW_KEY_W] ) {
        cameraPos += cameraSpeed * cameraFront;       
    }
    if( keys[GLFW_KEY_S] ) {
        cameraPos -= cameraSpeed * cameraFront;       
    }
    if( keys[GLFW_KEY_A] ) {
        cameraPos -= glm::normalize( glm::cross( cameraFront, cameraUp ) ) * cameraSpeed;       
    }
    if( keys[GLFW_KEY_D] ) {
        cameraPos += glm::normalize( glm::cross( cameraFront, cameraUp ) ) * cameraSpeed;       
    }
    if( keys[GLFW_KEY_SPACE] ) {
        cameraPos += cameraSpeed * cameraUp;
    }
}

int main( int argc, char **argv ) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // For OSX
#if defined( __APPLE__ )
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow( WIDTH / 2, HEIGHT / 2, "LearnOpenGL", nullptr, nullptr );
    glfwMakeContextCurrent( window );
    if ( window == NULL ) {
	std::cout << "Failed to create GLFW window" << std::endl;
	glfwTerminate();
	return -1;
    }

    // GLFW options
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );

    // Set input callbacks
    glfwSetKeyCallback( window, keyCallback );
    glfwSetCursorPosCallback( window, mouseCallback );
    glfwSetScrollCallback( window, scrollCallback );

    glewExperimental = GL_TRUE;
    if ( glewInit() != GLEW_OK ) {
	std::cout << "Failed to initialize GLEW" << std::endl;
	return -1;
    }
    
    glViewport( 0, 0, WIDTH, HEIGHT );

    // Define our object
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // World space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(  0.0f,  0.0f,   0.0f ),
        glm::vec3(  2.0f,  5.0f, -15.0f ),
        glm::vec3( -1.5f, -2.2f,  -2.5f ),
        glm::vec3( -3.8f, -2.0f, -12.3f ),
        glm::vec3(  2.4f, -0.4f,  -3.5f ),
        glm::vec3( -1.7f,  3.0f,  -7.5f ),
        glm::vec3(  1.3f, -2.0f,  -2.5f ),
        glm::vec3(  1.5f,  2.0f,  -2.5f ),
        glm::vec3(  1.5f,  0.2f,  -1.5f ),
        glm::vec3( -1.3f,  1.0f,  -1.5f )
    };
    
    // DEBUG
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    // Shader stuff
    Shader ourShader( "shaders/shader.vs", "shaders/shader.frag" );

    // Copy vertices array in a buffer for OpenGL to use
    GLuint VBO, VAO, EBO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    //glGenBuffers( 1, &EBO );

    // Bind VAO first, then bind and set buffers and pointers
    glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    // Create the buffer element object
    //glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    //glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    // Position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), (GLvoid*)0 );
    glEnableVertexAttribArray( 0 );
    
    // TexCoord attribute
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 2 );

    // Unbind buffer
    glBindVertexArray( 0 );

    // Load and create a texture
    GLuint texture1;
    GLuint texture2;

    // Texture1
    glGenTextures( 1, &texture1 );
    glBindTexture( GL_TEXTURE_2D, texture1 );

    // Set our texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    // Set texture filtering
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // load, create texture and generate mipmaps
    int width, height;
    unsigned char *image = SOIL_load_image( "images/container.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    // Texture2
    glGenTextures( 1, &texture2 );
    glBindTexture( GL_TEXTURE_2D, texture2 );

    // Set our texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    // Set texture filtering
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // load, create texture and generate mipmaps
    image = SOIL_load_image( "images/sonic.png", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 );

    // enable depth test
    glEnable( GL_DEPTH_TEST );

    while( !glfwWindowShouldClose( window ) ) {
        // Check and call events
	glfwPollEvents();
        movementAction();

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Start rendering
	glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // Bind textures using texture units
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, texture1 );
        glUniform1i( glGetUniformLocation( ourShader.Program, "ourTexture1" ), 0 );
        glActiveTexture( GL_TEXTURE1 );
        glBindTexture( GL_TEXTURE_2D, texture2 );
        glUniform1i( glGetUniformLocation( ourShader.Program, "ourTexture2" ), 1 );

        // Pick our shader program
        ourShader.Use();

        // Create Transformations
        // view matrix
        glm::mat4 view;
        view = glm::lookAt( cameraPos, cameraPos + cameraFront, cameraUp );

        // projection matrix
        glm::mat4 projection;
        projection = glm::perspective( glm::radians( aspect ), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f );
        
        // Get their uniform location
        GLint modelLoc = glGetUniformLocation( ourShader.Program, "model" );
        GLint viewLoc = glGetUniformLocation( ourShader.Program, "view" );
        GLint projLoc = glGetUniformLocation( ourShader.Program, "projection" );

        // Pass them to the shaders
        glUniformMatrix4fv( viewLoc, 1 , GL_FALSE, glm::value_ptr( view ) );
        glUniformMatrix4fv( projLoc, 1 , GL_FALSE, glm::value_ptr( projection ) );

        // Draw
        glBindVertexArray( VAO );
        for( GLuint i = 0; i < 10; i++ ) {
            // model matrix
            glm::mat4 model;
            model = glm::translate( model, cubePositions[i] );
            GLfloat angle = glfwGetTime() * 1.0f * (i + 1);
            model = glm::rotate( model, angle, glm::vec3( 1.0f, 0.3f, 0.5f ) );
            glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );

            glDrawArrays( GL_TRIANGLES, 0, 36 );
        }
        glBindVertexArray( 0 );
        
	// Show the scene
	glfwSwapBuffers( window );
    }

    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &EBO );

    glfwTerminate();

    return 0;
}
