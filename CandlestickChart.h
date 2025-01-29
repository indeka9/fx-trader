#pragma once
#ifndef CANDLESTICKCHART_H
#define CANDLESTICKCHART_H

#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <iomanip>
#include "Canvas.h"
#include "Candlestick.h"
#include <deque>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
#include "Chart.h"



class CandlestickChart : public Canvas, Chart {
public:
	
	CandlestickChart(int w, int h);
	~CandlestickChart();
	
	int maxVisibleCandles = 70;
	int curFirstCandleIndex = 0;


	void handleKeyPress(int key, int action) override; // Override the base class method
	void handleMouseMove(double xpos, double ypos) override; // Override the base class method
	void draw() const ;
	void drawAxes() const;
	void drawGrid() const;
	void drawBackground() const;
	void drawCandlestick(float x, const Candlestick& cs, float scaleX, float scaleY, float offsetX, float offsetY, bool isHovered) const;
	void drawCandlesticks() const;

	void setCandlesticks(const std::deque<Candlestick>& candles) ;
	bool isMouseHovering(float mouseX, float mouseY, float x, const Candlestick& cs, float scaleX, float scaleY, float offsetX, float offsetY) const;
	void drawTooltip(int index, float mouseX, float mouseY, const Candlestick& cs) const;
	void drawLegend() const;
	void drawEMA() const;
	void drawSMA() const;
	void drawRSI() const;
	void setSMA(const std::deque<float>& sma);
	void setEMA(const std::deque<float>& ema);
	void setRSI(const std::deque<float>& rsi);
	bool sma_on = false;
	bool ema_on = false;
	bool rsi_on = false;


private:
	
	std::deque<Candlestick> candlesticks;
	std::deque<float> smaValues; // Store SMA values
	std::deque<float> emaValues; // Store EMA values
	std::deque<float> rsiValues; // Store EMA values

	static float findMinValue(const std::deque<Candlestick>& candles);
	static float findMaxValue(const std::deque<Candlestick>& candles);
	bool fl = false;
};


#endif // CANDLESTICKCHART_H