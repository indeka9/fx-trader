
#include "CandlestickChart.h"
#include "FxTrader.h"


int main(int argc, char** argv) {

	const int WIDTH = 800;
	const int HEIGHT = 600;

	std::deque<Candlestick> candlesticks = Candlestick::generateRealisticCandlesticks(100);

	// Calculate SMA, EMA, and RSI
	int period = 14; // Example period for SMA, EMA, and RSI
	std::deque<float> sma = FxTrader::calculateSMA(candlesticks, period);
	std::deque<float> ema = FxTrader::calculateEMA(candlesticks, period);
	std::deque<float> rsi = FxTrader::calculateRSI(candlesticks, period);
	

	CandlestickChart chart(WIDTH, HEIGHT);
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

	chart.renderLoop();

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