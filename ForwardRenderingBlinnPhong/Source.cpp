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
	// End dimensions

	// GLFW setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Only use OpenGL version 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Lock window size
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MEng Project Forward Rendering", nullptr, nullptr); // Create window
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
	Shader shader("vertex.vs", "fragment.frag"); // Shader loading

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

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(shader.pID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.pID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(glGetUniformLocation(shader.pID, "camPosition"), camera.getPos().x, camera.getPos().y, camera.getPos().z);
		glUniform3f(glGetUniformLocation(shader.pID, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);
		glBindVertexArray(lVAO);
		glUniform3f(glGetUniformLocation(shader.pID, "iColour"), 0.5f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(shader.pID, "specular"), 0.1f);
		glUniformMatrix4fv(glGetUniformLocation(shader.pID, "model"), 1, GL_FALSE, glm::value_ptr(floorModel)); // Render floor
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glUniform3f(glGetUniformLocation(shader.pID, "iColour"), 0.1f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(shader.pID, "specular"), 0.5f);
		glUniformMatrix4fv(glGetUniformLocation(shader.pID, "model"), 1, GL_FALSE, glm::value_ptr(bunnyModel1)); // Render rabbit 1
		bunny.Draw(shader);
		glUniform3f(glGetUniformLocation(shader.pID, "iColour"), 0.1f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(shader.pID, "specular"), 0.7f);
		glUniformMatrix4fv(glGetUniformLocation(shader.pID, "model"), 1, GL_FALSE, glm::value_ptr(bunnyModel2)); // Render rabbit 2
		bunny.Draw(shader);
		glUniform3f(glGetUniformLocation(shader.pID, "iColour"), 0.9f, 0.1f, 0.1f);
		glUniform1f(glGetUniformLocation(shader.pID, "specular"), 0.3f);
		glUniformMatrix4fv(glGetUniformLocation(shader.pID, "model"), 1, GL_FALSE, glm::value_ptr(armaModel)); // Render armadillo
		armadillo.Draw(shader);
		glUniform3f(glGetUniformLocation(shader.pID, "iColour"), 0.1f, 0.9f, 0.1f);
		glUniform1f(glGetUniformLocation(shader.pID, "specular"), 0.2f);
		glUniformMatrix4fv(glGetUniformLocation(shader.pID, "model"), 1, GL_FALSE, glm::value_ptr(suitModel)); // Render suit
		suit.Draw(shader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glfwSwapBuffers(window); // Push new data to screen (this uses double buffering to avoid flickering)

	}

	// Clean up
	glDeleteVertexArrays(1, &lVAO);
	glDeleteBuffers(1, &lVBO);
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
