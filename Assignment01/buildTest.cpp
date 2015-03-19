// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// SOIL
#include "include/SOIL.h"

#include "shader.hpp"
#include <iostream>

const GLchar* fragmentShader2Source = "#version 330 core\n"
    "out vec4 color;\n"
    "void main() {\n"
    "color = vec4( 1.0f, 1.0f, 0.0f, 1.0f );\n"
    "}\0";

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

    GLFWwindow *window = glfwCreateWindow( 800, 600, "LearnOpenGL", nullptr, nullptr );
    glfwMakeContextCurrent( window );
    if ( window == NULL ) {
	std::cout << "Failed to create GLFW window" << std::endl;
	glfwTerminate();
	return -1;
    }

    glewExperimental = GL_TRUE;
    if ( glewInit() != GLEW_OK ) {
	std::cout << "Failed to initialize GLEW" << std::endl;
	return -1;
    }
    
    glViewport( 0, 0, 1600, 1200 );

    // Define our object
    GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // Top Right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom Left
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Top Left
    };
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    
    // DEBUG
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    // Shader stuff
    Shader ourShader( "shaders/shader.vs", "shaders/shader.frag" );

    // Copy vertices array in a buffer for OpenGL to use
    GLuint VBO, VAO, EBO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glGenBuffers( 1, &EBO );

    // Bind VAO first, then bind and set buffers and pointers
    glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    // Create the buffer element object
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    // Position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (GLvoid*)0 );
    glEnableVertexAttribArray( 0 );

    // Color attribute
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 1 );
    
    // TexCoord attribute
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 6 * sizeof( GLfloat ) ) );
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
    image = SOIL_load_image( "images/awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 );

    while( !glfwWindowShouldClose( window ) ) {
        // Check and call events
	glfwPollEvents();

        // Start rendering
	glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );

        // Bind textures using texture units
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, texture1 );
        glUniform1i( glGetUniformLocation( ourShader.Program, "ourTexture1" ), 0 );
        glActiveTexture( GL_TEXTURE1 );
        glBindTexture( GL_TEXTURE_2D, texture2 );
        glUniform1i( glGetUniformLocation( ourShader.Program, "ourTexture2" ), 1 );

        // Pick our shader program
        ourShader.Use();

        // Draw
        glBindVertexArray( VAO );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
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
