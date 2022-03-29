#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


    glewExperimental = GL_TRUE;


	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    //-----------------------------------------

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Массив 3 векторов, которые являются вершинами треугольника
    static const GLfloat triangle_1[] = {
       -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.0f,  0.5f, 0.0f,
       -0.25f, -0.5f, 0.0f,
       0.75f, -0.5f, 0.0f,
       0.5f,  0.5f, 0.0f,
    };

    static const GLfloat pyramide[] = {
        -0.7f, -0.8f, -0.3f,
        -0.3f, -0.8f, -0.3f,
        -0.5f, -0.4f, -0.5f,

        -0.7f, -0.8f, -0.3f,
        -0.5f, -0.8f, -0.7f,
        -0.5f, -0.4f, -0.5f,

        -0.5f, -0.8f, -0.7f,
        -0.3f, -0.8f, -0.3f,
        -0.5f, -0.4f, -0.5f,

        -0.7f, -0.8f, -0.3f,
        -0.5f, -0.8f, -0.7f,
        -0.3f, -0.8f, -0.3f,
    };
    static const GLfloat pyramide_color[] = {
        0.7f, 0.0f, 0.0f,
        0.3f, 0.0f, 0.0f,
        0.1f, 0.0f, 0.0f,

        0.5f, 0.8f, 0.0f,
        0.5f, 0.6f, 0.0f,
        0.5f, 0.4f, 0.0f,

        0.0f, 0.6f, 1.0f,
        0.0f, 0.4f, 1.0f,
        0.0f, 0.2f, 1.0f,

        0.3f, 0.3f, 0.3f,
        0.2f, 0.2f, 0.2f,
        0.1f, 0.1f, 0.1f,
    };

    // Это будет идентификатором нашего буфера вершин
    GLuint vertexbuffer;
    // Создадим 1 буфер и поместим в переменную vertexbuffer его идентификатор
    glGenBuffers(1, &vertexbuffer);
    // Сделаем только что созданный буфер текущим
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Передадим информацию о вершинах в OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_1), triangle_1, GL_STATIC_DRAW);


    GLuint pyramidebuffer;
    glGenBuffers(1, &pyramidebuffer);
    glBindBuffer(GL_ARRAY_BUFFER, pyramidebuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramide), pyramide, GL_STATIC_DRAW);
    
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramide_color), pyramide_color, GL_STATIC_DRAW);

    // Создать и откомпилировать нашу шейдерную программу
    GLuint programID_1 = LoadShaders("vertexshader.vertexshader", "triangle.fragmentshader");
    GLuint programID_2 = LoadShaders("vertexshader.vertexshader", "triangle2.fragmentshader");
    GLuint programID_3 = LoadShaders("vertexshader.vertexshader", "pyramide.fragmentshader");

    // Get a handle for our "MVP" uniform
    GLuint MatrixID_1 = glGetUniformLocation(programID_1, "MVP");
    GLuint MatrixID_2 = glGetUniformLocation(programID_2, "MVP");
    GLuint MatrixID_3 = glGetUniformLocation(programID_3, "MVP");
    
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(0, -2, 3), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 2, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        View = glm::rotate(View, glm::radians(0.5f), glm::vec3(0, 1, 0));
        glm::mat4 MVP = Projection * View * Model;

        // Устанавливаем наш шейдер текущим
        glUseProgram(programID_1);

        // Send our transformation to the currently bound shader, 
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID_1, 1, GL_FALSE, &MVP[0][0]);

        
        // Указываем, что первым буфером атрибутов будут вершины
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,                  // Атрибут 0. Подробнее об этом будет рассказано в части, посвященной шейдерам.
            3,                  // Размер
            GL_FLOAT,           // Тип
            GL_FALSE,           // Указывает, что значения не нормализованы
            0,                  // Шаг
            (void*)0            // Смещение массива в буфере
        );

        // Вывести треугольник!
        glDrawArrays(GL_TRIANGLES, 0, 3); // Начиная с вершины 0, всего 3 вершины -> один треугольник




        glUseProgram(programID_2);
        glUniformMatrix4fv(MatrixID_2, 1, GL_FALSE, &MVP[0][0]);
        glDrawArrays(GL_TRIANGLES, 3, 3);

 
        //Pyramide
        glUseProgram(programID_3);
        glUniformMatrix4fv(MatrixID_3, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, pyramidebuffer);
        glVertexAttribPointer(
            0,                  // Атрибут 0. Подробнее об этом будет рассказано в части, посвященной шейдерам.
            3,                  // Размер
            GL_FLOAT,           // Тип
            GL_FALSE,           // Указывает, что значения не нормализованы
            0,                  // Шаг
            (void*)0            // Смещение массива в буфере
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );
        glDrawArrays(GL_TRIANGLES, 0, 12);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteProgram(programID_3);
    glDeleteProgram(programID_2);
    glDeleteProgram(programID_1);
    glDeleteBuffers(1, &pyramidebuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteBuffers(1, &colorbuffer);

    //----------------------------------------------------------
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
    
	return 0;
}

