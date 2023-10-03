#include "main.h"

#define WINDOW_TITLE "POK1"
unsigned int window_width = 800;
unsigned int window_height = 600;

int main()
{
	// Initialize GLFW and setup some config options
	initGlfw(3, 3, true);

	// Create the GLFW window
	GLFWwindow * window;
	if(createWindow(&window)) // Window creation failed
	{
		return -1;
	}

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Tell OpenGL what part of the window it should use
	glViewport(0, 0, window_width, window_height);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);	// Dark gray background color

	ShaderProgram shaderProgram("assets/shaders/mvp_matrices_cubemap.vert", "assets/shaders/cubemap.frag");
	shaderProgram.use();

	// A cube
	GLfloat vertices[] = {
		// Coordinates        // Color           // Texture coordinates
		 0.5f,  0.5f,  0.5f,  //1.0f, 1.0f, 1.0f,  1.0f, 1.0f,	// Top left front relative to the cube, facing towards the camera
		-0.5f,  0.5f,  0.5f,  //0.0f, 1.0f, 1.0f,  0.0f, 1.0f,	// Top right front
		-0.5f, -0.5f,  0.5f,  //0.0f, 0.0f, 1.0f,  0.0f, 0.0f,	// Bottom right front
		 0.5f, -0.5f,  0.5f,  //1.0f, 0.0f, 1.0f,  1.0f, 0.0f,	// Bottom left front
		 0.5f,  0.5f, -0.5f,  //1.0f, 1.0f, 0.0f,  1.0f, 1.0f,	// Top left back
		-0.5f,  0.5f, -0.5f,  //0.0f, 1.0f, 0.0f,  0.0f, 1.0f,	// Top right back
		-0.5f, -0.5f, -0.5f,  //0.0f, 0.0f, 0.0f,  0.0f, 0.0f,	// Bottom right back
		 0.5f, -0.5f, -0.5f,  //1.0f, 0.0f, 0.0f,  1.0f, 0.0f,	// Bottom left back
	};

	// Draw 12 triangles for the entire cube, 2 per face
	GLuint indices[] = {
		// Front face: 0, 1, 2, 3 counterclockwise
		0, 1, 2,
		0, 2, 3,
		// Back face: 4, 7, 6, 5 counterclockwise
		4, 7, 6,
		4, 6, 5,
		// Left face: 0, 3, 7, 4 counterclockwise
		0, 3, 7,
		0, 7, 4,
		// Right face: 1, 5, 6, 2 counterclockwise
		1, 5, 6,
		1, 6, 2,
		// Top face: 0, 4, 5, 1 counterclockwise
		0, 4, 5,
		0, 5, 1,
		// Bottom face: 2, 6, 7, 3 counterclockwise
		2, 6, 7,
		2, 7, 3,
	};

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); // Vertices coordinates
	glGenBuffers(1, &EBO); // Vertices indices

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Specify the layout of the vertices' data in memory, required for the vertex shader
	// Coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
//	// Color
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
//	glEnableVertexAttribArray(1);
//	// Texture coordinates
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
//	glEnableVertexAttribArray(2);

	GLuint texture_grass;
	glGenTextures(1, &texture_grass);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_grass);

	//Repeat texture when the texture coordinate is outside the [0, 1] range
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);

	// Nearest neighbour upscalind, linear downscaling, linear interpolation between mipmaps
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *data_dirt = stbi_load("assets/textures/dirt.png", &width, &height, &nrChannels, 0);
	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_dirt);
	}
	stbi_image_free(data_dirt);

	unsigned char *data_grass = stbi_load("assets/textures/grass.png", &width, &height, &nrChannels, 0);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_grass);

	stbi_image_free(data_grass);

//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Draw a wireframe
	glEnable(GL_DEPTH_TEST); // Enable depth testing to avoir drawing far elements over close elements
	glEnable(GL_CULL_FACE); // Enable back face culling

	// Render loop
	while(!glfwWindowShouldClose(window))
	{
		processInput(window, shaderProgram);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture_grass);

		shaderProgram.use();

		// Model matrix: transformations applied to the model
		glm::mat4x4 model = glm::mat4x4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate horizontally over time

		// View matrix: transformation applied to the entire scene to move the camera
		glm::mat4x4 view = glm::mat4x4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		view = glm::rotate(view, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		// Projection matrix: final projection into OpenGL internal coordinates, perspective
		glm::mat4x4 projection = glm::perspective(glm::radians(45.0f), (float)window_width/(float)window_height, 0.1f, 100.0f);

		// Pass these matrices to the vertex shader
		shaderProgram.setUniformMat4x4("model", model);
		shaderProgram.setUniformMat4x4("view", view);
		shaderProgram.setUniformMat4x4("projection", projection);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void initGlfw(int gl_major, int gl_minor, bool core_profile)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
	if (core_profile)
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	else
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
}

int createWindow(GLFWwindow ** window)
{
	*window = glfwCreateWindow(window_width, window_height, WINDOW_TITLE, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(*window);

	// Allow proper window resizing
	glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	window_width = width;
	window_height = height;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, ShaderProgram &shaderProgram)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)	// Close the window when escape is pressed
		glfwSetWindowShouldClose(window, true);

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}

	if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
	}
}
