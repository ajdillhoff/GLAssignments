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
    
    glViewport( 0, 0, 800, 600 );

    // Define our object
    GLfloat vertices[] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.0f, 0.5f
    };

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

    // Build shader program
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glLinkProgram( shaderProgram );
    glUseProgram( shaderProgram );
    
    while( !glfwWindowShouldClose( window ) ) {
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	glfwPollEvents();

	// Copy vertices array in a buffer for OpenGL to use
	GLuint VBO;
	glGenBuffers( 1, &VBO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	// Set vertex attribute pointers
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0 );
	glEnableVertexAttribArray( 0 );

	// Render the scene
	
	// Unbind buffer
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	// Show the scene
	glfwSwapBuffers( window );
    }

    // Delete shaders
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    glfwTerminate();

    return 0;
}
