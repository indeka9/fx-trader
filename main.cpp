#include <iostream>
#include "CandlestickChart.h"
#include "FxTrader.h"
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime> // Include ctime for localtime_s

std::deque<Candlestick> generateCandlestickDataFile(int n, const std::string& start_date) {
	std::ofstream file;
	std::string filename = "forex-" + start_date + ".csv";

	Candlestick::Period period = Candlestick::Period::ONE_MINUTE; // Example period
	std::deque<Candlestick> candlesticks = Candlestick::generateRealisticCandlesticks(n, period);

	try {
		file.open(filename);
		if (!file.is_open()) {
			throw std::ios_base::failure("Failed to open file: " + filename);
		}


		for (int i = 0; i < n; ++i) {
			Candlestick candle = candlesticks[i];

			// Write to file
			file << candle.open << "," << candle.high << "," << candle.low << "," << candle.close << "," << candle.volume << "," << candle.timestamp << "\n";

		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	finally:
	if (file.is_open()) {
		file.close();
	}

	return candlesticks;
}

int main(int argc, char** argv) {

	const int WIDTH = 800;
	const int HEIGHT = 600;

	//std::deque<Candlestick> candlesticks = Candlestick::generateRealisticCandlesticks(100);
	generateCandlestickDataFile(100, "2021-01-01");

	CandlestickChart chart(WIDTH, HEIGHT);
	GLFWwindow* window = chart.getWindow();

	if (!window) return -1;
	glfwSetWindowUserPointer(window, &chart);

	// Set candlesticks and indicators
	chart.loadDataFromCSV("forex-2021-01-01.csv");
//	chart.setCandlesticks(candlesticks);
	chart.curFirstCandleIndex = std::max(0, static_cast<int>(chart.getCandlesticks().size() - chart.maxVisibleCandles));

	// Calculate SMA, EMA, and RSI
	int period = 14; // Example period for SMA, EMA, and RSI
	std::deque<float> sma = FxTrader::calculateSMA(chart.getCandlesticks(), period);
	std::deque<float> ema = FxTrader::calculateEMA(chart.getCandlesticks(), period);
	std::deque<float> rsi = FxTrader::calculateRSI(chart.getCandlesticks(), period);

	chart.maxVisibleCandles = 70;
	chart.setSMA(sma);
	chart.setEMA(ema);
	chart.setRSI(rsi);

	chart.sma_on = true;
	chart.ema_on = true;
	chart.rsi_on = true;

	const Candlestick::Period timerperiod = Candlestick::Period::ONE_MINUTE; // Set the period

	while (!glfwWindowShouldClose(window)) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (Candlestick::onTimer(timerperiod)) {
			generateCandlestickDataFile(1, "2021-01-01");
			chart.loadDataFromCSV("forex-2021-01-01.csv");
			sma = FxTrader::calculateSMA(chart.getCandlesticks(), period);
			ema = FxTrader::calculateEMA(chart.getCandlesticks(), period);
			rsi = FxTrader::calculateRSI(chart.getCandlesticks(), period);

			//chart.setCandlesticks(candlesticks);
			chart.setSMA(sma);
			chart.setEMA(ema);
			chart.setRSI(rsi);

		}

		chart.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}




