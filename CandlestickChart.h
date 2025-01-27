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


class CandlestickChart : public Canvas {
public:
	
	CandlestickChart(int w, int h);
	~CandlestickChart();
	
	void draw() const ;
	void move_left();
	void setCandlesticks(const std::deque<Candlestick>& candles) ;
	
	void drawCandlestick(float x, const Candlestick& cs, float scaleX, float scaleY, float offsetX, float offsetY, bool isHovered) const;
	void drawCandlesticks() const;
	void drawVolumeBars(const std::deque<Candlestick>& candles, float scaleX, float scaleY, float offsetX, float offsetY);
	void drawAxes() const;
	void drawGrid() const;
	void drawTooltip(int index, float mouseX, float mouseY, const Candlestick& cs) const;

	void setSMA(const std::deque<float>& sma);
	void setEMA(const std::deque<float>& ema);
	void setRSI(const std::deque<float>& rsi);
	bool sma_on =false;
	bool ema_on = false;
	bool rsi_on = false;

	void drawEMA() const;
	void drawSMA() const;
	void drawRSI() const;
	void drawBackground() const;
	void drawRoundedRect(float x, float y, float width, float height, float radius) const;
	void drawLegend() const;
	static float findMinValue(const std::deque<Candlestick>& candles);
	static float findMaxValue(const std::deque<Candlestick>& candles);
	bool isMouseHovering(float mouseX, float mouseY, float x, const Candlestick& cs, float scaleX, float scaleY, float offsetX, float offsetY) const;

private:
	
	std::deque<Candlestick> candlesticks;
	std::deque<float> smaValues; // Store SMA values
	std::deque<float> emaValues; // Store EMA values
	std::deque<float> rsiValues; // Store EMA values
	int maxVisibleCandles;
	
	void updatePanBounds();
	float minPanX;
	float maxPanX;
	
};


#endif // CANDLESTICKCHART_H