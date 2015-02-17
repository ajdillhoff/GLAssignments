// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <iostream>

const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout ( location = 0 ) in vec2 position;\n"
    "void main() {\n"
    "gl_Position = vec4( position.x, position.y, 0.0, 1.0 );\n"
    "}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
    "out vec4 color;\n"
    "void main() {\n"
    "color = vec4( 1.0f, 0.5f, 0.2f, 1.0f );\n"
    "}\0";

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

    // Compile the vertex shader
    GLuint vertexShader;
    vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShader );

    // Compile fragment shader
    GLuint fragmentShader;
    fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
    glCompileShader( fragmentShader );

    // Compile fragment shader 2
    GLuint fragmentShader2;
    fragmentShader2 = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader2, 1, &fragmentShader2Source, NULL );
    glCompileShader( fragmentShader2 );

    // Build shader program 2
    GLuint shaderProgram2;
    shaderProgram2 = glCreateProgram();
    glAttachShader( shaderProgram2, vertexShader );
    glAttachShader( shaderProgram2, fragmentShader2 );
    glLinkProgram( shaderProgram2 );

    // Build shader program
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glLinkProgram( shaderProgram );

    // Copy vertices array in a buffer for OpenGL to use
    GLuint VBO, VAO, VBO2, VAO2, EBO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );

    // Second triangle
    glGenVertexArrays( 1, &VAO2 );
    glGenBuffers( 1, &VBO2 );
    
    // Bind VAO first, then bind and set buffers and pointers
    glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    // Set vertex attribute pointers
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_TRUE, 0 * sizeof( GLfloat ), (GLvoid*)0 );
    glEnableVertexAttribArray( 0 );

    // second triangle
    glBindVertexArray( VAO2 );
    glBindBuffer( GL_ARRAY_BUFFER, VBO2 );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices2 ), vertices2, GL_STATIC_DRAW );
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
        glUseProgram( shaderProgram );
        glDrawArrays( GL_TRIANGLES, 0, 3 );
        //glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
        
        glBindVertexArray( VAO2 );

        glUseProgram( shaderProgram2 );
        glDrawArrays( GL_TRIANGLES, 0, 3 );

	// Show the scene
	glfwSwapBuffers( window );
    }

    // Delete shaders
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    glfwTerminate();

    return 0;
}
