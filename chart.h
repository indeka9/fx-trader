#pragma once
#ifndef CHART_H
#define CHART_H

class Chart {
public:
    virtual ~Chart() = default; // Virtual destructor for proper cleanup
    virtual void handleKeyPress(int key, int action) = 0; // Pure virtual method
};



#endif


