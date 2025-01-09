#include "RSIWindow.h"
#include <iostream>

// Constructor implementation
RSIWindow::RSIWindow(int width, int height, const std::deque<float>& rsiValues)
    : width(width), height(height), rsiValues(rsiValues), window(nullptr) {
    initGL();
}

// Destructor implementation
RSIWindow::~RSIWindow() {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

// Get the GLFW window pointer
GLFWwindow* RSIWindow::getWindow() const {
    return window;
}

// Initialize OpenGL and GLFW
void RSIWindow::initGL() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    // Create a window for the RSI
    window = glfwCreateWindow(width, height, "RSI Chart", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create RSI window" << std::endl;
        return;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize OpenGL settings
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 10.0, 0, 100.0, -1.0, 1.0); // RSI typically ranges from 0 to 100
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Enable anti-aliasing
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

// Draw the RSI line
void RSIWindow::drawRSI() const {
    if (rsiValues.empty()) return;

    // Find the maximum and minimum values in the RSI values for scaling
    float maxRSIValue = *std::max_element(rsiValues.begin(), rsiValues.end());
    float minRSIValue = *std::min_element(rsiValues.begin(), rsiValues.end());

    // If all RSI values are NaN (e.g., insufficient data), skip drawing
    if (std::isnan(maxRSIValue) || std::isnan(minRSIValue)) return;

    // Calculate the scaling factors for the x and y axes
    float scaleX = 9.6f / rsiValues.size();  // Scale for the x-axis (time)
    float scaleY = 9.6f / (maxRSIValue - minRSIValue);  // Scale for the y-axis (RSI values)

    // Calculate the offset for the y-axis to align with the chart's coordinate system
    float offsetX = 0.2f;  // X-axis offset (left margin)
    float offsetY = 0.2f - (minRSIValue * scaleY);  // Y-axis offset (bottom margin)

    // Set the color for the RSI line (cyan)
    glColor3f(0.0f, 1.0f, 1.0f);  // Cyan color for RSI

    // Begin drawing the RSI line
    glBegin(GL_LINE_STRIP);
    for (size_t i = 0; i < rsiValues.size(); ++i) {
        // Skip NaN values (e.g., at the beginning of the RSI series)
        if (std::isnan(rsiValues[i])) continue;

        // Calculate the x and y positions for the RSI point
        float x = offsetX + (i * scaleX);  // X position based on time
        float y = offsetY + (rsiValues[i] * scaleY);  // Y position based on RSI value

        // Add the point to the RSI line
        glVertex2f(x, y);
    }
    glEnd();
}