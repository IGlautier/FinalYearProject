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

// End function declarations

// Global variables
bool activeKeys[1024]; // Log key presses
GLfloat delta = 0.0f; // Frame control
GLfloat lastFrame = 0.0f; 
GLfloat lastX = 400.0f; // Mouse input
GLfloat lastY = 300.0f; 
GLfloat pitch = 0.0f; 
GLfloat yaw = 0.0f; 

// End global variables


int main() {

	// Dimensions 
	const GLuint WIDTH = 1080;
	const GLuint HEIGHT = 720;
	const GLuint SWIDTH = 1024; // Shadow map
	const GLuint SHEIGHT = 1024;
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
	Shader shadowShader("shadow.vs", "shadow.frag", "shadow.gs");
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
	gShader.Use();
	glUniform1i(glGetUniformLocation(gShader.pID, "shadowMap"), 0);
	lightShader.Use();
	glUniform1i(glGetUniformLocation(lightShader.pID, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(lightShader.pID, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(lightShader.pID, "gColour"), 2);
	// End shader texture uniforms


	// Shadow mapping setup
	GLuint shadowMap, shadowBuffer;
	glGenFramebuffers(1, &shadowBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
	for (GLuint i = 0; i < 6; ++i) glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SWIDTH, SHEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR: Shadow buffer not complete" << std::endl; // Error check
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// End shadow mapping setup


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
		lightPosition.x = sin(glfwGetTime())*20.0f; // Uncomment to enable rotation of light around scene
		lightPosition.z = cos(glfwGetTime())*20.0f;
		// End light update

		// View and projection matrix setup
		glm::mat4 view, projection;
		view = glm::lookAt(camera.getPos(), camera.getPos() + camera.getForward(), camera.getUp()); // the sum on the middle argument ensures the camera remains focussed on where we are looking
		projection = glm::perspective(glm::radians(45.0f), GLfloat(WIDTH)/ GLfloat(HEIGHT), 0.1f, 100.0f);
		
		// End view and projection matrix setup


		// Shadow pass
		glm::mat4 shadowProjection = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 100.0f);
		std::vector<glm::mat4> shadowViews;
		shadowViews.push_back(shadowProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowViews.push_back(shadowProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowViews.push_back(shadowProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowViews.push_back(shadowProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowViews.push_back(shadowProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowViews.push_back(shadowProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		glViewport(0, 0, SWIDTH, SHEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer); // Bind gBuffer to fill with geometry data from the scene
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowShader.Use();
		glUniform3f(glGetUniformLocation(shadowShader.pID, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);
		for (GLuint i = 0; i < 6; ++i) glUniformMatrix4fv(glGetUniformLocation(shadowShader.pID, ("shadowViews[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(shadowViews[i]));
		glBindVertexArray(lVAO);
		glUniformMatrix4fv(glGetUniformLocation(shadowShader.pID, "model"), 1, GL_FALSE, glm::value_ptr(floorModel)); // Render floor
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glUniformMatrix4fv(glGetUniformLocation(shadowShader.pID, "model"), 1, GL_FALSE, glm::value_ptr(bunnyModel1)); // Render rabbit 1
		bunny.Draw(shadowShader);
		glUniformMatrix4fv(glGetUniformLocation(shadowShader.pID, "model"), 1, GL_FALSE, glm::value_ptr(bunnyModel2)); // Render rabbit 2
		bunny.Draw(shadowShader);
		glUniformMatrix4fv(glGetUniformLocation(shadowShader.pID, "model"), 1, GL_FALSE, glm::value_ptr(armaModel)); // Render armadillo
		armadillo.Draw(shadowShader);
		glUniformMatrix4fv(glGetUniformLocation(shadowShader.pID, "model"), 1, GL_FALSE, glm::value_ptr(suitModel)); // Render suit
		suit.Draw(gShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, WIDTH, HEIGHT); // Back to normal viewport
		// End shadow pass



		// Geometry pass	
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer); // Bind gBuffer to fill with geometry data from the scene
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gShader.Use();
		glActiveTexture(GL_TEXTURE0); // Shadow map for shadow calculation
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
		glUniformMatrix4fv(glGetUniformLocation(gShader.pID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(gShader.pID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(glGetUniformLocation(gShader.pID, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);
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

		

		// Lighting pass
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		lightShader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gColour);
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
