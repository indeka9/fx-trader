#include "Canvas.h"


// Canvas.cpp
Canvas::Canvas(int w, int h)
	: width(w), height(h), isPanning(false), zoomLevel(1.0f), panX(0), panY(0), lastMouseX(0), lastMouseY(0) {
	initGL();
}

Canvas::~Canvas() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow* Canvas::getWindow() const {
	return window;
}


void Canvas::initGL() {

	int argc = 1;
	char* argv[1] = { (char*)"Something" };
	glutInit(&argc, argv);

	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;

	}

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(width, height, "EUR/USD Forex Chart", NULL, NULL);
	if (!window) {
		glfwTerminate();
		std::cerr << "Failed to create GLFW window" << std::endl;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);


	// Enable anti-aliasing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // Objects with smaller z-values are drawn first

	// Enable blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set the mouse button callback
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		auto canvas = static_cast<Canvas*>(glfwGetWindowUserPointer(window));
		canvas->handleFramebufferSizeCallback(width, height);
		});

	// Set the mouse button callback
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
		auto canvas = static_cast<Canvas*>(glfwGetWindowUserPointer(window));
		canvas->handleMouseButton(button, action, mods);
		});

	// Set the cursor position callback
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
		auto canvas = static_cast<Canvas*>(glfwGetWindowUserPointer(window));
		canvas->handleMouseMove(xpos, ypos);
		});


	glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
		auto canvas = static_cast<Canvas*>(glfwGetWindowUserPointer(window));
		canvas->handleScroll(xoffset, yoffset);
		});

	// Set the key callback
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto canvas = static_cast<Canvas*>(glfwGetWindowUserPointer(window));
		canvas->handleKeyPress(key, action);
		});



}



void Canvas::handleScroll(double xoffset, double yoffset) {
	// Adjust zoom level based on scroll direction
	float zoomFactor = (yoffset > 0) ? 1.1f : 0.9f; // Zoom in (1.1x) or out (0.9x)
	float newZoomLevel = zoomLevel * zoomFactor;

	// Clamp the zoom level to a reasonable range
	newZoomLevel = std::max(0.1f, std::min(newZoomLevel, 10.0f));

	// Convert mouse position to world coordinates
	float worldMouseX = panX + (xoffset / width) * (10.0f / zoomLevel);
	float worldMouseY = panY + ((height - yoffset) / height) * (10.0f / zoomLevel);

	// Adjust panX and panY to keep the mouse position fixed during zoom
	panX = worldMouseX - (xoffset / width) * (10.0f / newZoomLevel);
	panY = worldMouseY - ((height - yoffset) / height) * (10.0f / newZoomLevel);

	// Update the zoom level
	zoomLevel = newZoomLevel;

	
	lastMouseX = xoffset;
	lastMouseY = yoffset;

}


void Canvas::handleMouseButton(int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			isPanning = true;
			glfwGetCursorPos(glfwGetCurrentContext(), &lastMouseX, &lastMouseY);
		}
		else if (action == GLFW_RELEASE) {
			isPanning = false;
		}
	}
}





void Canvas::handleFramebufferSizeCallback(int w, int h) {

	width = w;
	height = h;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(panX, 10.0 / zoomLevel + panX, panY, 10.0 / zoomLevel + panY, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void Canvas::handleHorizontalScroll(float offset) {
	// Adjust panX based on the offset
	panX += offset;

	// Clamp panX to prevent scrolling too far left or right
	clampPanX();
}


void Canvas::clampPanX() {
	panX = std::max(-10.0f, std::min(panX, 10.0f));
}

