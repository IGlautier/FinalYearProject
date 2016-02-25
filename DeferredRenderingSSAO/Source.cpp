// Includes
#define GLEW_STATIC
#include <glew.h> // Include GLEW
#include <glfw3.h> // Include GLFW
#include <iostream> // Include io
#include <fstream> // Include file io
#include <streambuf>
#include "Shader.h" // Include shader class
#include "Camera.h" // Include camera class
#include "Model.h" // Includes for model loading (This is a third party class courtesy of LearnOpenGL.com)
#include <glm\glm.hpp> // Include GLM
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <random> // Include random function
// End includes

// Function declarations
void keyPress(GLFWwindow* window, int key, int scanCode, int action, int mode); // User input
void mouseMove(GLFWwindow* window, double x, double y);
glm::vec3 genKernel(GLuint i); // SSAO sample generator
glm::vec3 genNoise(); // SSAO noise texture generator
// End function declarations

// Global variables
bool activeKeys[1024]; // Log key presses
GLfloat delta = 0.0f; // Frame control
GLfloat lastFrame = 0.0f;
GLfloat lastX = 400.0f; // Mouse input
GLfloat lastY = 300.0f;
GLfloat pitch = 0.0f;
GLfloat yaw = 0.0f;
std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // SSAO random functions
std::default_random_engine generator;
GLboolean ssaoEnable = true; // SSAO ON/OFF
GLboolean blurEnable = false;
GLint ssaoLevel = 1;
GLfloat ssaoRadius = 1.0f;
// End global variables

int main() {

	// Dimensions 
	const GLuint WIDTH = 1080;
	const GLuint HEIGHT = 720;
	// End dimensions

	// GLFW setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Only use OpenGL version 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Lock window size
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MEng Project SSAO", nullptr, nullptr); // Create window
	if (window == nullptr) { // Error Check
		std::cout << "ERROR: Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Bind mouse to window
	glfwSetKeyCallback(window, keyPress); // Add input handling functions
	glfwSetCursorPosCallback(window, mouseMove);
	// End GLFW setup

	// GLEW setup
	glewExperimental = GL_TRUE; // Ensures access to up to date opengl functions
	if (glewInit() != GLEW_OK) { // Error check
		std::cout << "ERROR: Failed to initialise GLEW" << std::endl;
		return -1;
	}
	// End GLEW setup

	// OpenGL setup
	glViewport(0, 0, WIDTH, HEIGHT); // Pass window dimensions 
	glEnable(GL_DEPTH_TEST); // Required for proper depth testing
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black back drop
	// End OpenGL setup

	// Loading and object creation
	Camera camera = Camera(); // Create camera
	Shader gShader("geometry.vs", "geometry.frag"); // Shader loading
	Shader lightShader("light.vs", "light.frag");
	Shader blurShader("blur.vs", "blur.frag");
	Shader ssaoShader("ssao.vs", "ssao.frag");
	Model suit("nanosuit/nanosuit.obj"); // Model loading
	Model bunny("bunny/bunny.obj");
	Model armadillo("armadillo/armadillo.obj");
	// End loading and object creation	

	// Geometry input
	GLfloat verticesFloor[] = { // Geometry for floor object (Just a cube that we flatten out)
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	GLfloat verticesQuad[] = { // Quad that we render to for screen space techniques
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f
	};
	glm::vec3 lightPosition = glm::vec3(10.0f, 10.0f, 10.0f); // Initial position of point light
	glm::mat4 bunnyModel1, bunnyModel2, suitModel, armaModel, floorModel; // Sets up uniforms for location of models in world
	bunnyModel1 = glm::translate(bunnyModel1, glm::vec3(0.0f, 0.15f, 0.0f));
	bunnyModel2 = glm::translate(bunnyModel2, glm::vec3(5.0f, 0.15f, 0.0f));
	suitModel = glm::translate(suitModel, glm::vec3(15.0f, 0.0f, 0.0f));
	armaModel = glm::translate(armaModel, glm::vec3(5.0f, 0.5f, 5.0f));
	floorModel = glm::scale(floorModel, glm::vec3(100.0f, 0.1f, 100.0f));
	// End geometry input

	// Shader texture uniforms
	lightShader.Use();
	glUniform1i(glGetUniformLocation(lightShader.pID, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(lightShader.pID, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(lightShader.pID, "gColour"), 2);
	glUniform1i(glGetUniformLocation(lightShader.pID, "ssao"), 3);
	ssaoShader.Use();
	glUniform1i(glGetUniformLocation(ssaoShader.pID, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(ssaoShader.pID, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(ssaoShader.pID, "noiseTex"), 2);
	blurShader.Use();
	glUniform1i(glGetUniformLocation(blurShader.pID, "ssao"), 0);
	// End shader texture uniforms

	// gBuffer for deferred rendering
	GLuint gBuffer, gPosition, gNormal, gColour, gDepth;
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glGenTextures(1, &gPosition); // Set up position data storage
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	glGenTextures(1, &gNormal); // Set up normal data storage
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	glGenTextures(1, &gColour); // Set up colour data storage
	glBindTexture(GL_TEXTURE_2D, gColour);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColour, 0);
	GLuint attach[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attach);
	glGenRenderbuffers(1, &gDepth); // Set up depth
	glBindRenderbuffer(GL_RENDERBUFFER, gDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR: Geometry framebuffer not complete" << std::endl; // Error check
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// End GBuffer

	// Quad buffer object
	GLuint qVBO, qVAO;
	glGenVertexArrays(1, &qVAO);
	glGenBuffers(1, &qVBO);
	glBindVertexArray(qVAO);
	glBindBuffer(GL_ARRAY_BUFFER, qVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesQuad), verticesQuad, GL_STATIC_DRAW); // Pass in quad geometry
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); // Set stepping for vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Set stepping for texture coordinates
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// End quad buffer object

	// Light buffer object
	GLuint lVBO, lVAO;
	glGenVertexArrays(1, &lVAO);
	glGenBuffers(1, &lVBO);
	glBindVertexArray(lVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFloor), verticesFloor, GL_STATIC_DRAW); // Pass in floor geometry
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Be careful setting stride and size here (3 values) position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));  // Textures
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// End light buffer object

	// SSAO frame buffer object
	GLuint ssaoFBO, ssaoColour;
	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glGenTextures(1, &ssaoColour);
	glBindTexture(GL_TEXTURE_2D, ssaoColour);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColour, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR: SSAO framebuffer not complete" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// End SSAO frame buffer object

	// Blur frame buffer object
	GLuint blurFBO, blurColour;
	glGenFramebuffers(1, &blurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
	glGenTextures(1, &blurColour);
	glBindTexture(GL_TEXTURE_2D, blurColour);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColour, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR: Blur framebuffer not complete" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// End Blur frame buffer object

	// Setup sample kernel
	std::vector<glm::vec3> kernel;
	for (GLuint i = 0; i < 64; ++i) kernel.push_back(genKernel(i));
	// End setup sample kernel

	// Setup noise texture
	std::vector<glm::vec3> noise;
	for (GLuint i = 0; i < 16; ++i) noise.push_back(genNoise());
	GLuint noiseTex;
	glGenTextures(1, &noiseTex);
	glBindTexture(GL_TEXTURE_2D, noiseTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &noise[0]); // 4x4 texture that will be tiled over screen
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set to wrap and repeat across whole screen
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// End setup noise texture


	while (!glfwWindowShouldClose(window)) { // The "game loop"

		// Handle user input
		glfwPollEvents();
		GLfloat curFrame = glfwGetTime();
		delta = curFrame - lastFrame; // Frame smoothing
		lastFrame = curFrame;
		camera.setSpeed(delta * 10.0f);
		camera.update(activeKeys, pitch, yaw);
		// End user input

		// Light update
		//lightPosition.x = sin(glfwGetTime())*20.0f; // Uncomment to enable rotation of light around scene
		//lightPosition.z = cos(glfwGetTime())*20.0f;
		// End light update

		// View and projection matrix setup
		glm::mat4 view, projection;
		view = glm::lookAt(camera.getPos(), camera.getPos() + camera.getForward(), camera.getUp()); // the sum on the middle argument ensures the camera remains focussed on where we are looking
		projection = glm::perspective(glm::radians(45.0f), GLfloat(WIDTH) / GLfloat(HEIGHT), 0.1f, 100.0f);

		// End view and projection matrix setup

		// Geometry pass	
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer); // Bind gBuffer to fill with geometry data from the scene
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(gShader.pID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(gShader.pID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(lVAO);
		glUniform3f(glGetUniformLocation(gShader.pID, "colour"), 0.5f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(gShader.pID, "specular"), 0.1f);
		glUniformMatrix4fv(glGetUniformLocation(gShader.pID, "model"), 1, GL_FALSE, glm::value_ptr(floorModel)); // Render floor
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glUniform3f(glGetUniformLocation(gShader.pID, "colour"), 0.1f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(gShader.pID, "specular"), 0.5f);
		glUniformMatrix4fv(glGetUniformLocation(gShader.pID, "model"), 1, GL_FALSE, glm::value_ptr(bunnyModel1)); // Render rabbit 1
		bunny.Draw(gShader);
		glUniform3f(glGetUniformLocation(gShader.pID, "colour"), 0.1f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(gShader.pID, "specular"), 0.7f);
		glUniformMatrix4fv(glGetUniformLocation(gShader.pID, "model"), 1, GL_FALSE, glm::value_ptr(bunnyModel2)); // Render rabbit 2
		bunny.Draw(gShader);
		glUniform3f(glGetUniformLocation(gShader.pID, "colour"), 0.9f, 0.1f, 0.1f);
		glUniform1f(glGetUniformLocation(gShader.pID, "specular"), 0.3f);
		glUniformMatrix4fv(glGetUniformLocation(gShader.pID, "model"), 1, GL_FALSE, glm::value_ptr(armaModel)); // Render armadillo
		armadillo.Draw(gShader);
		glUniform3f(glGetUniformLocation(gShader.pID, "colour"), 0.1f, 0.9f, 0.1f);
		glUniform1f(glGetUniformLocation(gShader.pID, "specular"), 0.2f);
		glUniformMatrix4fv(glGetUniformLocation(gShader.pID, "model"), 1, GL_FALSE, glm::value_ptr(suitModel)); // Render suit
		suit.Draw(gShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// End geometry pass

		// SSAO pass
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		ssaoShader.Use();
		glActiveTexture(GL_TEXTURE0); // Bind textures from gBuffer to retrieve required information
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, noiseTex);
		for (GLuint i = 0; i < 64; ++i) glUniform3fv(glGetUniformLocation(ssaoShader.pID, ("kernel[" + std::to_string(i) + "]").c_str()), 1, &kernel[i][0]);
		glUniformMatrix4fv(glGetUniformLocation(ssaoShader.pID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform1i(glGetUniformLocation(ssaoShader.pID, "ssaoLevel"), ssaoLevel);
		glUniform1f(glGetUniformLocation(ssaoShader.pID, "ssaoRadius"), ssaoRadius);
		glBindVertexArray(qVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// End ssao pass

		// Blur pass
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		blurShader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ssaoColour);
		glUniform1i(glGetUniformLocation(blurShader.pID, "blurEnable"), blurEnable);
		glBindVertexArray(qVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// End blur pass

		// Lighting pass
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		lightShader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gColour);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, blurColour);
		glUniform1i(glGetUniformLocation(lightShader.pID, "ssaoEnable"), ssaoEnable);
		glUniform3f(glGetUniformLocation(lightShader.pID, "camPosition"), camera.getPos().x, camera.getPos().y, camera.getPos().z);
		glUniform3f(glGetUniformLocation(lightShader.pID, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);
		glBindVertexArray(qVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		// End lighting pass

		glfwSwapBuffers(window); // Push new data to screen (this uses double buffering to avoid flickering)

	}

	// Clean up
	glDeleteVertexArrays(1, &lVAO);
	glDeleteBuffers(1, &lVBO);
	glDeleteVertexArrays(1, &qVAO);
	glDeleteBuffers(1, &qVBO);
	glDeleteFramebuffers(1, &ssaoFBO);
	glfwTerminate();
	// End clean up

	return 0;
}


void keyPress(GLFWwindow* window, int key, int scanCode, int action, int mode) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE); // Escape exits programme

	if (action == GLFW_PRESS) // Capture key presses for movement
		activeKeys[key] = true;
	else if (action == GLFW_RELEASE)
		activeKeys[key] = false;

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) ssaoEnable = !ssaoEnable; // For comparisons

	if (key == GLFW_KEY_B && action == GLFW_PRESS) blurEnable = !blurEnable;

	if (key == GLFW_KEY_1 && action == GLFW_PRESS) ssaoLevel = 1;

	if (key == GLFW_KEY_2 && action == GLFW_PRESS) ssaoLevel = 2;

	if (key == GLFW_KEY_3 && action == GLFW_PRESS) ssaoLevel = 4;

	if (key == GLFW_KEY_4 && action == GLFW_PRESS) ssaoLevel = 1.0f;

	if (key == GLFW_KEY_5 && action == GLFW_PRESS) ssaoLevel = 5.0f;

	if (key == GLFW_KEY_6 && action == GLFW_PRESS) ssaoLevel = 10.0f;
}

void mouseMove(GLFWwindow* window, double x, double y) {
	GLfloat xChange = x - lastX;
	GLfloat yChange = y - lastY;
	lastX = x;
	lastY = y;
	xChange *= 0.05f; // Sensitivity scaler for look
	yChange *= 0.05f;
	yaw += xChange;
	pitch -= yChange;
	if (pitch > 89.0f) pitch = 89.0f; // Bound camera look angle
	if (pitch < -89.0f) pitch = -89.0f;
}

glm::vec3 genKernel(GLuint i) {
	/* Generates vectors in normalised device coordinates (screen space). When used iteratively will bias towards vectors of shorter length */
	glm::vec3 sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));
	sample = glm::normalize(sample);
	sample *= randomFloats(generator);
	GLfloat minScale = 0.1f; // Minimum and maximum scaling factors to apply to vectors
	GLfloat maxScale = 1.0f;
	GLfloat scale = minScale + (GLfloat(i) / (64.0f / ssaoLevel)) * (GLfloat(i) / (64.0f / ssaoLevel)) * (maxScale - minScale);
	sample *= scale;
	return sample;
}

glm::vec3 genNoise() {
	return glm::vec3(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, 0.0f);
}