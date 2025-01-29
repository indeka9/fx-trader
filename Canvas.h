#pragma once
#ifndef CANVAS_H
#define CANVAS_H
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <iostream>
#include <algorithm>
#include "Chart.h"

class Canvas {
public:
    Canvas(int w, int h); // Accept a pointer to the base Chart class
    ~Canvas();
    GLFWwindow* getWindow() const;
    virtual void draw() const =0;
       
    
protected:

    
    int width;
    int height;
    float zoomLevel;
    float panX;
    float panY;
    bool isPanning;
    double lastMouseX;
    double lastMouseY;
    GLFWwindow* window;
    

    void initGL();
    void handleScroll(double xoffset, double yoffset) ;
    void handleMouseButton(int button, int action, int mods);
    virtual void handleMouseMove(double xpos, double ypos) = 0;

    void handleFramebufferSizeCallback( int width, int height);
    void handleHorizontalScroll(float offset);
    void clampPanX();
    virtual void handleKeyPress(int key, int action) = 0;
};

#endif // CANVAS_H

