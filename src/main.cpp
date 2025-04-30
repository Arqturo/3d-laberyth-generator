#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <math.h>
#include <algorithm>


#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"



// Vertices coordinates
GLfloat vertices[] =
{ //               COORDINATES                  /     COLORS           //
	-0.25f, -0.25f, 0.0f,     0.0f, 0.0f,  1.0f, // Lower left corner
	 0.25f, -0.25f, 0.0f,     0.0f, 0.0f,  1.0f, // Lower right corner
	 0.0f,  0.25f, 0.0f,     0.0f, 0.0f,  1.0f, // Upper corner
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2 // Lower left triangle

};

GLfloat vertices2[] =
{ //               COORDINATES                  /     COLORS           //
	-0.25f, -0.25f, 0.0f,     1.0f, 0.0f,  0.0f, // Lower left corner
	 0.25f, -0.25f, 0.0f,     1.0f, 0.0f,  0.0f, // Lower right corner
	 0.25f,  0.25f, 0.0f,     1.0f, 0.0f,  0.0f, // Upper right corner
	-0.25f,  0.25f, 0.0f,     1.0f, 0.0f,  0.0f, // Upper left corner
};

// Indices to form two triangles for a square
GLuint indices2[] = {
	0, 1, 2, // First triangle (lower-left to upper-right to lower-right)
	0, 2, 3  // Second triangle (lower-left to upper-right to upper-left)
};

GLfloat vertices3[] =
{ //               COORDINATES                  /     COLORS           //
    -0.25f, -0.4f, 0.0f,     0.0f, 1.0f,  0.0f, // Vertex 0 (Bottom left)
     0.25f, -0.4f, 0.0f,     0.0f, 1.0f,  0.0f, // Vertex 1 (Bottom right)
     0.5f,   0.1f, 0.0f,     0.0f, 1.0f,  0.0f, // Vertex 2 (Right middle)
     0.0f,   0.5f, 0.0f,     0.0f, 1.0f,  0.0f, // Vertex 3 (Top center)
    -0.5f,   0.1f, 0.0f,     0.0f, 1.0f,  0.0f, // Vertex 4 (Left middle)
};

// Indices to form one triangle fan for a pentagon
GLuint indices3[] = {
    0, 1, 2, // Triangle 1 (bottom-left to bottom-right to right-middle)
    0, 2, 3, // Triangle 2 (bottom-left to right-middle to top-center)
    0, 3, 4, // Triangle 3 (bottom-left to top-center to left-middle)
};

void setClearColor(const float clearColor[4]) {
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
}


	   float clearColorArray[][4] = {
        {0.07f, 0.13f, 0.17f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
        {0.2f, 0.4f, 0.6f, 1.0f}
    };


const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void bindFirstVAO(VAO* vaos, int numVAOs) {
	setClearColor(clearColorArray[0]);
    if (numVAOs > 0) {
        vaos[0].Bind();
    } else {
        std::cout << "No VAOs in the array\n";
    }
}

void unbindAndMoveToEnd(VAO* vaos, int numVAOs) {
    if (numVAOs > 0) {
        // Unbind the first element
        vaos[0].Unbind();
		setClearColor(clearColorArray[0]);
		

        // Move the first element to the end
        std::rotate(vaos, vaos + 1, vaos + numVAOs);
        std::rotate(&clearColorArray[0][0], &clearColorArray[1][0], &clearColorArray[numVAOs][0]);

    } else {
        std::cout << "No VAOs in the array\n";
    }
}

void unbindAndMoveToStart(VAO* vaos, int numVAOs) {
    if (numVAOs > 0) {
        // Unbind the first element
        vaos[0].Unbind();
        setClearColor(clearColorArray[0]);

        // Move the last element to the beginning
        std::rotate(vaos, vaos + numVAOs - 1, vaos + numVAOs);
        std::rotate(&clearColorArray[0][0], &clearColorArray[numVAOs - 1][0], &clearColorArray[numVAOs][0]);
    } else {
        std::cout << "No VAOs in the array\n";
    }
}

void processInput(GLFWwindow* window, VAO* vaos, int numVAOs, double inputDelay) {
    static double lastInputTime = 0.0;
    double currentTime = glfwGetTime();

    if ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) ||
        ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) &&
         (currentTime - lastInputTime) >= inputDelay)) {
        lastInputTime = currentTime;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            // Call your custom function when the right arrow key is pressed
            unbindAndMoveToEnd(vaos, numVAOs);
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            // Call your custom function when the left arrow key is pressed
            unbindAndMoveToStart(vaos, numVAOs);
        }
    }
}


int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800



	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");



	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	VAO VAO2;
	VAO2.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO2(vertices2, sizeof(vertices2));
	// Generates Element Buffer Object and links it to indices
	EBO EBO2(indices2, sizeof(indices2));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO2.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();

	VAO VAO3;
	VAO3.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO3(vertices3, sizeof(vertices3));
	// Generates Element Buffer Object and links it to indices
	EBO EBO3(indices3, sizeof(indices3));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO3.LinkAttrib(VBO3, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO3.LinkAttrib(VBO3, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO3.Unbind();
	VBO3.Unbind();
	EBO3.Unbind();

	const int numVAOs = 3;
    const float circleRadius = 10.0f;

    VAO vaos[numVAOs] = {VAO1, VAO2, VAO3};
	setClearColor(clearColorArray[0]);

	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		        processInput(window,vaos,3,0.5);

		// Specify the color of the background
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
		glUniform1f(uniID, 0.5f);
		// Bind the VAO so OpenGL knows to use it
		bindFirstVAO(vaos, 3);
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	/*
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	*/
	VAO2.Delete();
	VBO2.Delete();
	EBO2.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}