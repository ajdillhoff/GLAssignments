// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

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
        -1.0f, -1.0f,
        -0.5f, 0.5f,
        0.0f, -1.0f
    };

    GLfloat vertices2[] = {
        0.0f, -1.0f,
        0.5f, 0.5f,
        1.0f, -1.0f
    };

    //GLfloat vertices[] = {
        //0.5f, 0.5f, 0.0f,   // Top Right
        //0.5f, -0.5f, 0.0f,   // Bottom Right
        //-0.5f, -0.5f, 0.0f,   // Bottom Left
        //-0.5f, 0.5f, 0.0f   // Top Left
    //};
    //GLuint indices[] = {
        //0, 1, 3,
        //1, 2, 3
    //};
    
    // DEBUG
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    // Shader stuff
    Shader ourShader( "shaders/shader.vs", "shaders/shader.frag" );

    // Copy vertices array in a buffer for OpenGL to use
    GLuint VBO, VAO, EBO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );

    // Bind VAO first, then bind and set buffers and pointers
    glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    // Set vertex attribute pointers
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_TRUE, 0 * sizeof( GLfloat ), (GLvoid*)0 );
    glEnableVertexAttribArray( 0 );

    // Create the buffer element object
    //glGenBuffers( 1, &EBO );
    //glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    //glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    // Unbind buffer
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
    
    while( !glfwWindowShouldClose( window ) ) {
        // Check and call events
	glfwPollEvents();

	glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );

        // Pick our shader program
        glBindVertexArray( VAO );

        // Draw
        ourShader.Use();
        glDrawArrays( GL_TRIANGLES, 0, 3 );
        //glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
        
	// Show the scene
	glfwSwapBuffers( window );
    }

    glfwTerminate();

    return 0;
}
