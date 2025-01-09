#pragma once
#ifndef RSIWINDOW_H
#define RSIWINDOW_H

#include <GLFW/glfw3.h>
#include <deque>
#include <algorithm>
#include <cmath>

class RSIWindow {
public:
    RSIWindow(int width, int height, const std::deque<float>& rsiValues);
    ~RSIWindow();

    GLFWwindow* getWindow() const; // Add this method
    void drawRSI() const;

private:
    int width;
    int height;
    std::deque<float> rsiValues;
    GLFWwindow* window;

    void initGL();
};

#endif // RSIWINDOW_H