#include "main.h"

#define WINDOW_TITLE "POK1"
#define WORLD_DIMS_X 500
#define WORLD_DIMS_Y 500

unsigned int window_width = 800;
unsigned int window_height = 600;

int main()
{
	// Initialize GLFW and setup some config options
	initGlfw(4, 6, true);

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

	glClearColor(0.6f, 0.7f, 1.0f, 1.0f);	// Light blue background color

	ShaderProgram shaderProgram("assets/shaders/model_view_proj_matrices.vert", "assets/shaders/light_tex_array.frag");
	shaderProgram.use();

	std::vector<GLfloat> vertices;

	std::vector<GLuint> indices;

	std::random_device rd;
	unsigned int seed = rd();

	World world = World(seed, 4, 0.01, 1000.0, 0.4);

	std::array<std::array<int, WORLD_DIMS_Y>, WORLD_DIMS_X> heights;

	for (int i = -WORLD_DIMS_X/2; i < WORLD_DIMS_X/2 + WORLD_DIMS_X % 2; i++)
	{
		for (int j = -WORLD_DIMS_Y/2; j < WORLD_DIMS_Y/2 + WORLD_DIMS_Y % 2; j++)
		{
			heights[i+WORLD_DIMS_X/2][j+WORLD_DIMS_Y/2] = world.getHeightAt(i, j);
		}
	}

	for (int i = -WORLD_DIMS_X/2; i < WORLD_DIMS_X/2 + WORLD_DIMS_X % 2; i++)
	{
		for (int j = -WORLD_DIMS_Y/2; j < WORLD_DIMS_Y/2 + WORLD_DIMS_Y % 2; j++)
		{
			int h = heights[i+WORLD_DIMS_X/2][j+WORLD_DIMS_Y/2];
			add_top_face(i, j, h, vertices, indices);
			if (-WORLD_DIMS_X/2 < i)
			{
				for (int h_w = heights[i+WORLD_DIMS_X/2 - 1][j+WORLD_DIMS_Y/2]; h_w < h; h_w++)
					add_west_face(i, j, h_w+1, vertices, indices);
			}

			if (i < WORLD_DIMS_X/2 + (WORLD_DIMS_X % 2) - 1)
			{
				for (int h_e = heights[i+WORLD_DIMS_X/2 + 1][j+WORLD_DIMS_Y/2]; h_e < h; h_e++)
					add_east_face(i, j, h_e+1, vertices, indices);
			}

			if (-WORLD_DIMS_Y/2 < j)
			{
				for (int h_n = heights[i+WORLD_DIMS_X/2][j+WORLD_DIMS_Y/2 - 1]; h_n < h; h_n++)
					add_north_face(i, j, h_n+1, vertices, indices);
			}

			if (j < WORLD_DIMS_Y/2 + WORLD_DIMS_Y % 2 - 1)
			{
				for (int h_s = heights[i+WORLD_DIMS_X/2][j+WORLD_DIMS_Y/2 + 1]; h_s < h; h_s++)
					add_south_face(i, j, h_s+1, vertices, indices);
			}
		}
	}

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); // Vertices coordinates
	glGenBuffers(1, &EBO); // Vertices indices

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// Specify the layout of the vertices' data in memory, required for the vertex shader
	// Coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Normal vector
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture coordinates
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6* sizeof(float)));
	glEnableVertexAttribArray(2);

	GLuint textures;
	glGenTextures(1, &textures);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textures);

	int width, height, nrChannels;
	unsigned char *data_grass = stbi_load("assets/textures/grass.png", &width, &height, &nrChannels, 0);
	unsigned char *data_dirt = stbi_load("assets/textures/dirt.png", &width, &height, &nrChannels, 0);
	unsigned char *data_stone = stbi_load("assets/textures/stone.png", &width, &height, &nrChannels, 0);


	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 5, GL_RGB8, width, height, 3);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, data_grass);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 1, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, data_dirt);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 2, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, data_stone);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	//Repeat texture when the texture coordinate is outside the [0, 1] range
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Nearest neighbour upscaling, linear downscaling, linear interpolation between mipmaps
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_image_free(data_grass);
	stbi_image_free(data_dirt);
	stbi_image_free(data_stone);

	glEnable(GL_DEPTH_TEST); // Enable depth testing to avoir drawing far elements over close elements
	glEnable(GL_CULL_FACE); // Enable back face culling: don't draw a triangle when its vertices are in a clockwise order

	Camera camera(glm::vec3(0.0f, 100.0f, 0.0f), 0.0f, -90.0f);

	// Render loop
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D_ARRAY, textures);

		shaderProgram.use();

		// Model matrix: transformations applied to the model, unneeded here
		glm::mat4x4 model = glm::mat4x4(1.0f);

		// View matrix: transformation applied to the entire scene to move the camera
		glm::mat4x4 view = camera.viewMatrix();

		// Projection matrix: final projection into OpenGL internal coordinates
		glm::mat4x4 projection = glm::perspective(glm::radians(45.0f), (float)window_width/(float)window_height, 0.1f, 1000.0f);

		// Pass these matrices to the vertex shader
		shaderProgram.setUniformMat4x4("model", model);
		shaderProgram.setUniformMat4x4("view", view);
		shaderProgram.setUniformMat4x4("projection", projection);

		shaderProgram.setUniform3f("LightDirection", glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));
		shaderProgram.setUniform1f("ambient", 0.6f);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

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

void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)	// Close the window when escape is pressed
		glfwSetWindowShouldClose(window, true);
}

void add_top_face(int x, int z, int y, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices)
{
	unsigned int first_index = vertices.size() / 9; // Each vertex is represented by 9 elements of vertices
	//					 // Spatial coordinates : y up, (x, z) horizontal       // Normal		    // Texture
	add_vertex(vertices, (GLfloat)x,       (GLfloat)y,       (GLfloat)z,         0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f);
	add_vertex(vertices, (GLfloat)x,       (GLfloat)y,       (GLfloat)(z + 1),   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f);
	add_vertex(vertices, (GLfloat)(x + 1), (GLfloat)y,       (GLfloat)(z + 1),   0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f);
	add_vertex(vertices, (GLfloat)(x + 1), (GLfloat)y,       (GLfloat)z,         0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f);

	indices.push_back(first_index);
	indices.push_back(first_index + 1);
	indices.push_back(first_index + 2);
	indices.push_back(first_index);
	indices.push_back(first_index + 2);
	indices.push_back(first_index + 3);
}

void add_east_face(int x, int z, int y, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices)
{
	unsigned int first_index = vertices.size() / 9; // Each vertex is represented by 9 elements of vertices
	//					 // Spatial coordinates							         // Normal		     // Texture
	add_vertex(vertices, (GLfloat)(x + 1), (GLfloat)y,       (GLfloat)(z + 1),   1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f);
	add_vertex(vertices, (GLfloat)(x + 1), (GLfloat)(y - 1), (GLfloat)(z + 1),   1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 1.0f);
	add_vertex(vertices, (GLfloat)(x + 1), (GLfloat)(y - 1), (GLfloat)z,         1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f);
	add_vertex(vertices, (GLfloat)(x + 1), (GLfloat)y,       (GLfloat)z,         1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f);

	indices.push_back(first_index);
	indices.push_back(first_index + 1);
	indices.push_back(first_index + 2);
	indices.push_back(first_index);
	indices.push_back(first_index + 2);
	indices.push_back(first_index + 3);
}

void add_west_face(int x, int z, int y, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices)
{
	unsigned int first_index = vertices.size() / 9; // Each vertex is represented by 9 elements of vertices
	//					 // Spatial coordinates							         // Normal		     // Texture
	add_vertex(vertices, (GLfloat)x,      (GLfloat)y,       (GLfloat)z,         -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f);
	add_vertex(vertices, (GLfloat)x,      (GLfloat)(y - 1), (GLfloat)z,         -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 1.0f);
	add_vertex(vertices, (GLfloat)x,      (GLfloat)(y - 1), (GLfloat)(z + 1),   -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f);
	add_vertex(vertices, (GLfloat)x,      (GLfloat)y,       (GLfloat)(z + 1),   -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f);

	indices.push_back(first_index);
	indices.push_back(first_index + 1);
	indices.push_back(first_index + 2);
	indices.push_back(first_index);
	indices.push_back(first_index + 2);
	indices.push_back(first_index + 3);
}

void add_south_face(int x, int z, int y, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices)
{
	unsigned int first_index = vertices.size() / 9; // Each vertex is represented by 9 elements of vertices
	//					 // Spatial coordinates							          // Normal		    // Texture
	add_vertex(vertices, (GLfloat)x,       (GLfloat)y,       (GLfloat)(z + 1),   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f);
	add_vertex(vertices, (GLfloat)x,       (GLfloat)(y - 1), (GLfloat)(z + 1),   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f);
	add_vertex(vertices, (GLfloat)(x + 1), (GLfloat)(y - 1), (GLfloat)(z + 1),   0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f);
	add_vertex(vertices, (GLfloat)(x + 1), (GLfloat)y,       (GLfloat)(z + 1),   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 1.0f);

	indices.push_back(first_index);
	indices.push_back(first_index + 1);
	indices.push_back(first_index + 2);
	indices.push_back(first_index);
	indices.push_back(first_index + 2);
	indices.push_back(first_index + 3);
}

void add_north_face(int x, int z, int y, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices)
{
	unsigned int first_index = vertices.size() / 9; // Each vertex is represented by 9 elements of vertices
	//					 // Spatial coordinates							        // Normal		    // Texture
	add_vertex(vertices, (GLfloat)(x + 1), (GLfloat)y,       (GLfloat)z,         0.0f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f);
	add_vertex(vertices, (GLfloat)(x + 1), (GLfloat)(y - 1), (GLfloat)z,         0.0f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f);
	add_vertex(vertices, (GLfloat)x,       (GLfloat)(y - 1), (GLfloat)z,         0.0f, 0.0f, -1.0f,  1.0f, 1.0f, 1.0f);
	add_vertex(vertices, (GLfloat)x,       (GLfloat)y,       (GLfloat)z,         0.0f, 0.0f, -1.0f,  1.0f, 0.0f, 1.0f);

	indices.push_back(first_index);
	indices.push_back(first_index + 1);
	indices.push_back(first_index + 2);
	indices.push_back(first_index);
	indices.push_back(first_index + 2);
	indices.push_back(first_index + 3);
}

void add_bottom_face(int x, int z, int y, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices)
{
	unsigned int first_index = vertices.size() / 9; // Each vertex is represented by 9 elements of vertices
	//					 // Spatial coordinates							         // Normal		   // Texture
	add_vertex(vertices, (GLfloat)(x + 1), (GLfloat)(y - 1), (GLfloat)z,         0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f);
	add_vertex(vertices, (GLfloat)(x + 1), (GLfloat)(y - 1), (GLfloat)(z + 1),   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f);
	add_vertex(vertices, (GLfloat)x,       (GLfloat)(y - 1), (GLfloat)(z + 1),   0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f);
	add_vertex(vertices, (GLfloat)x,       (GLfloat)(y - 1), (GLfloat)z,         0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 1.0f);

	indices.push_back(first_index);
	indices.push_back(first_index + 1);
	indices.push_back(first_index + 2);
	indices.push_back(first_index);
	indices.push_back(first_index + 2);
	indices.push_back(first_index + 3);
}

void add_vertex(std::vector<GLfloat> &vertices, GLfloat x, GLfloat y, GLfloat z, GLfloat n_x, GLfloat n_y, GLfloat n_z, GLfloat t_x, GLfloat t_y, GLfloat t_z)
{
	// spacial coordinates
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);

	// Normal vector
	vertices.push_back(n_x);
	vertices.push_back(n_y);
	vertices.push_back(n_z);

	// Texture coordinates
	vertices.push_back(t_x);
	vertices.push_back(t_y);
	vertices.push_back(t_z);
}
