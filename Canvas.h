#pragma once
#ifndef CANVAS_H
#define CANVAS_H
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <iostream>


class Canvas {
public:
    Canvas(int w, int h);
    ~Canvas();
    GLFWwindow* getWindow() const;
    virtual void draw() const =0;
    virtual void renderLoop() const;
   

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
    void handleMouseMove(double xpos, double ypos);
    void handleKeyPress(int key, int action);
    void handleFramebufferSizeCallback( int width, int height);
};

#endif // CANVAS_H

