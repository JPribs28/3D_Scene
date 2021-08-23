//standard library 
#include <iostream>        

// GLAD and GLFW headers
#include <glad/glad.h>		// GLAD library
#include <GLFW/glfw3.h>		// GLFW library

// GLM headers 
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// project headers 
#include <shader.h>
#include <cylinder.h>
#include <plane.h>
#include <camera.h>

// stb_image headers for image textures
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*
user-defined functions to create a GLFW window,
resize the window, and processing user input
*/
bool initializeWindow(GLFWwindow** window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

/*
* user-defined functions for using mouse and scroll wheel for camera movement
*/
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// unnamed namespace
namespace
{
	//width and height settings for GLFW window
	const int WIDTH = 800;
	const int HEIGHT = 600;

	// GLFW window
	GLFWwindow* window = nullptr;	
}


/*
* CAMERA SETUP
*/
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 2.0f, 0.0f);

Camera camera(glm::vec3(0.0f, 3.0f, 8.0f));
bool firstMouse = true;
float lastX = (float)WIDTH / 2.0;
float lastY = (float)HEIGHT / 2.0;

// timing
float deltaTime = 0.0f;		// time between frames
float lastFrame = 0.0f;		// for calculating delta in loop

/*
* LIGHT  POSITIONS
*/
glm::vec3 lightPos = glm::vec3(-5.0f, 2.0f, 0.0f);


/*
* MAIN PROGRAM
*/
int main()
{
	// try to create GLFW window
	if (!initializeWindow(&window))
	{
		return EXIT_FAILURE;
	}


	/*
	* CREATE SHADER PROGRAMS
	*/
	Shader ourShader("shader.vert", "shader.frag");			


	/*
	* CREATE OBJECTS
	*/
	Plane plane(1.0f, 1.0f);
	Cylinder cornMiddle(1.0f, 1.0f, 100, 3.0f, true, true);
	Cylinder cornEnd(0.6f, 1.0f, 100, 2.0f, true, true);
	Cylinder skilletMain(1.0f, 0.8f, 100, 1.0f, false, true);
	Cylinder skilletHandle(0.5f, 0.5f, 50, 0.5f, true, true);
	Cylinder burger(1.0f, 1.0f, 50, 0.25f, true, true);
	Cylinder plate(1.0f, 0.8f, 50, 0.1f, false, true);
	Cylinder sausage(0.5f, 0.5f, 50, 3.0f, true, true);


	/*
	* LOAD TEXTURES
	*/
	// corn texture
	unsigned int cornTexture;
	glGenTextures(1, &cornTexture);
	glBindTexture(GL_TEXTURE_2D, cornTexture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);							// tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("corn.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// table texture
	unsigned int tableTexture;
	glGenTextures(1, &tableTexture);
	glBindTexture(GL_TEXTURE_2D, tableTexture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);							// tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load("table.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// skillet texture
	unsigned int skilletTexture;
	glGenTextures(1, &skilletTexture);
	glBindTexture(GL_TEXTURE_2D, skilletTexture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);							// tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load("skillet.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// burger texture
	unsigned int burgerTexture;
	glGenTextures(1, &burgerTexture);
	glBindTexture(GL_TEXTURE_2D, burgerTexture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);							// tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load("burger.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// plate texture
	unsigned int plateTexture;
	glGenTextures(1, &plateTexture);
	glBindTexture(GL_TEXTURE_2D, plateTexture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);							// tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load("plate.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	/*
	* RENDER LOOP
	*/
	while (!glfwWindowShouldClose(window))
	{
		// calculate time between frames
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);				// process and apply user input

		// enable z-depth and set background color
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*
		* SET LIGHT ATTRIBUTES
		*/
		ourShader.use();
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setVec3("lightPos", lightPos);
		ourShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 0.95f));


		/*
		* PROJECTION AND CAMERA
		*/

		// set projection vector and pass to shader
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);
		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);
		 

		/*
		* PLANE TRANSFORMATIONS
		*/

		// set ashpalt texture for plane
		glBindTexture(GL_TEXTURE_2D, tableTexture);
		ourShader.setInt("ourTexture", 0);

		glm::mat4 scale = glm::scale(glm::vec3(5.0f, 5.0f, 3.5f));
		glm::mat4 rotation = glm::rotate(glm::radians(0.0f), glm::vec3(0.0, 1.0f, 0.0f));
		glm::mat4 translate = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 model = translate * rotation * scale;
		// model uniform data
		ourShader.setMat4("model", model);
		plane.render();


		/*
		* SKILLET
		*/
		// set metal texture for skillet
		glBindTexture(GL_TEXTURE_2D, skilletTexture);
		ourShader.setInt("ourTexture", 0);

		scale = glm::scale(glm::vec3(1.5f, 0.5f, 1.5f));
		rotation = glm::rotate(glm::radians(0.0f), glm::vec3(0.0, 0.0f, 1.0f));
		translate = glm::translate(glm::vec3(-2.0f, 0.26f, 0.0f));
		model = translate * rotation * scale;
		// model uniform data 
		ourShader.setMat4("model", model);
		skilletMain.render();

		scale = glm::scale(glm::vec3(0.3f, 2.75f, 0.1f));
		rotation = glm::rotate(glm::radians(90.0f), glm::vec3(1.0, 0.0f, 0.0f));
		translate = glm::translate(glm::vec3(-2.0f, 0.4f, 2.12f));
		model = translate * rotation * scale;
		// model uniform data 
		ourShader.setMat4("model", model);
		skilletHandle.render();


		/*
		* PLATE
		*/
		glBindTexture(GL_TEXTURE_2D, plateTexture);
		ourShader.setInt("ourTexture", 0);

		scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
		rotation = glm::rotate(glm::radians(0.0f), glm::vec3(1.0, 0.0f, 0.0f));
		translate = glm::translate(glm::vec3(2.0f, 0.06f, 0.0f));
		model = translate * rotation * scale;
		// model uniform data 
		ourShader.setMat4("model", model);
		plate.render();


		/*
		* BURGERS
		*/
		glBindTexture(GL_TEXTURE_2D, burgerTexture);
		ourShader.setInt("ourTexture", 0);

		scale = glm::scale(glm::vec3(0.4f));
		rotation = glm::rotate(glm::radians(0.0f), glm::vec3(1.0, 0.0f, 0.0f));
		translate = glm::translate(glm::vec3(-2.3f, 0.1f, -0.3f));
		model = translate * rotation * scale;
		// model uniform data 
		ourShader.setMat4("model", model);
		burger.render();

		scale = glm::scale(glm::vec3(0.4f));
		rotation = glm::rotate(glm::radians(0.0f), glm::vec3(1.0, 0.0f, 0.0f));
		translate = glm::translate(glm::vec3(-1.7f, 0.1f, 0.5f));
		model = translate * rotation * scale;
		// model uniform data 
		ourShader.setMat4("model", model);
		burger.render();


		/*
		* SAUSAGE TRANSFORMATIONS
		*/
		scale = glm::scale(glm::vec3(0.2f, 0.35f, 0.25f));
		rotation = glm::rotate(glm::radians(90.0f), glm::vec3(0.2, 0.0f, 1.0f));
		translate = glm::translate(glm::vec3(2.0f, 0.1f, -0.4f));
		model = translate * rotation * scale;
		// model uniform data 
		ourShader.setMat4("model", model);
		sausage.render();

	
		/*
		* CORN TRANSFORMATIONS
		*/
		// set corn texture for corn pieces
		glBindTexture(GL_TEXTURE_2D, cornTexture);
		ourShader.setInt("ourTexture", 0);

		scale = glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
		rotation = glm::rotate(glm::radians(-90.0f), glm::vec3(0.0, 0.0f, 1.0f));
		translate = glm::translate(glm::vec3(2.5f, 0.2f, 0.4f));
		model = translate * rotation * scale;
		// model uniform data 
		ourShader.setMat4("model", model);
		cornEnd.render();

		scale = glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
		rotation = glm::rotate(glm::radians(90.0f), glm::vec3(0.0, 0.0f, 1.0f));
		translate = glm::translate(glm::vec3(1.5f, 0.2f, 0.4f));
		model = translate * rotation * scale;
		// model uniform data 
		ourShader.setMat4("model", model);
		cornEnd.render();

		scale = glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
		rotation = glm::rotate(glm::radians(90.0f), glm::vec3(0.0, 0.0f, 1.0f));
		translate = glm::translate(glm::vec3(2.0f, 0.2f, 0.4f));
		model = translate * rotation * scale;
		// model uniform data 
		ourShader.setMat4("model", model);
		cornMiddle.render();

		glfwSwapBuffers(window);	// swap buffers every frame
		glfwPollEvents();			// retrieve user input
	}

	// destroy meshes and shader program
	cornEnd.deleteVBO();
	cornMiddle.deleteVBO();
	glDeleteProgram(ourShader.ID);

	glfwTerminate();				// terminate to clear all GLFW resourses
	return 0;
}


/*
initializes GLFW, sets OpenGL version, loads GLAD and creates
a GLFW Window
*/
bool initializeWindow(GLFWwindow** window)
{
	// initialize and configure for OpenGL version
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// run this piece of code if operating system is macOS
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// create GLFW window with ability to resize and check for errors during creation
	*window = glfwCreateWindow(WIDTH, HEIGHT, "3D Scene", NULL, NULL);		
	if (*window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;				
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(*window);

	// callback functions
	glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);			// for window resizing
	glfwSetCursorPosCallback(*window, mouse_callback);				// for mouse view control
	glfwSetScrollCallback(*window, scroll_callback);				// for mouse scroll wheel input

	// tell GLFW to capture our mouse
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// load OpenGL function pointers from GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;		// error message if unable to load GLAD pointers
		return false;
	}
	

	return true;
}


/*
sets GLFW window viewport to size of window if changed after starting program
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


/* 
processes the user input from keyboard while running
*/
void processInput(GLFWwindow* window)
{
	// close window when 'ESC' key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// switch to wireframe mode when '1' is pressed
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// switch to fill mode when '2' is pressed
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// move camera forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);

	// move camera back
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);

	// move camera left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);

	// move camera right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	// move camera up
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);

	// move camera down
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

/*
* processes the user input from mouse cursor
*/
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

/*
* processes the user input from mouse scroll wheel
*/
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}
