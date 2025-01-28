#include <iostream>
#include "CandlestickChart.h"
#include "FxTrader.h"
#include <thread>
#include <chrono>

#include <functional>

// Static helper method to convert Period enum to seconds
int periodToSeconds(Candlestick::Period period) {
	switch (period) {
	case Candlestick::Period::ONE_MINUTE: return 5;
	case Candlestick::Period::FIVE_MINUTES: return 300;
	case Candlestick::Period::FIFTEEN_MINUTES: return 900;
	case Candlestick::Period::THIRTY_MINUTES: return 1800;
	case Candlestick::Period::ONE_HOUR: return 3600;
	case Candlestick::Period::FOUR_HOURS: return 14400;
	case Candlestick::Period::ONE_DAY: return 86400;
	case Candlestick::Period::ONE_WEEK: return 604800;
	case Candlestick::Period::ONE_MONTH: return 2592000; // Approximate, assuming 30 days
	default: return 60; // Default to 1 minute
	}
}

int main(int argc, char** argv) {

	const int WIDTH = 800;
	const int HEIGHT = 600;

	std::deque<Candlestick> candlesticks = Candlestick::generateRealisticCandlesticks(100);


	// Calculate SMA, EMA, and RSI
	int period = 14; // Example period for SMA, EMA, and RSI
	std::deque<float> sma = FxTrader::calculateSMA(candlesticks, period);
	std::deque<float> ema = FxTrader::calculateEMA(candlesticks, period);
	std::deque<float> rsi = FxTrader::calculateRSI(candlesticks, period);


	CandlestickChart chart(WIDTH, HEIGHT) ;
	GLFWwindow* window =  chart.getWindow();

	if (!window) return -1;
	glfwSetWindowUserPointer(window, &chart);



	// Set candlesticks and indicators
	chart.setCandlesticks(candlesticks);

	chart.setSMA(sma);
	chart.setEMA(ema);
	chart.setRSI(rsi);


	chart.sma_on = true;
	chart.ema_on = true;
	chart.rsi_on = true;


	Candlestick::Period timerperiod = Candlestick::Period::ONE_MINUTE; // Set the period


	bool _updateFlag=false;

	//chart.renderLoop();
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render here
		

		static std::chrono::steady_clock::time_point lastUpdateTime = std::chrono::steady_clock::now();
		auto currentTime = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastUpdateTime).count();
		int periodInSeconds = periodToSeconds(timerperiod);

		if (duration >= periodInSeconds) {
			_updateFlag = !_updateFlag; // Toggle the update flag
			lastUpdateTime = currentTime;
		}

		if (_updateFlag) {
			candlesticks = Candlestick::generateRealisticCandlesticks(candlesticks, 1);

			sma = FxTrader::calculateSMA(candlesticks, period);
			ema = FxTrader::calculateEMA(candlesticks, period);
			rsi = FxTrader::calculateRSI(candlesticks, period);

			chart.setCandlesticks(candlesticks);
			chart.setSMA(sma);
			chart.setEMA(ema);
			chart.setRSI(rsi);

			chart.move_left(); // Move the chart to the left when a new candle is added

			_updateFlag = !_updateFlag;
		}

		chart.draw();
		

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
		
	}

	
	

	return 0;

}


//// Print SMA and RSI values (for testing)
//std::cout << "SMA Values:\n";
//for (float value : sma) {
//	std::cout << value << " ";
//}
//std::cout << "\n\nRSI Values:\n";
//for (float value : rsi) {
//	std::cout << value << " ";
//}