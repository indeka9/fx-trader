#pragma once
#ifndef FXTRADER_H
#define FXTRADER_H

#include "Candlestick.h"
#include <deque>


class FxTrader
{
public:
	static std::deque<float> calculateSMA(const std::deque<Candlestick>& candles, int period);
	static std::deque<float> calculateEMA(const std::deque<Candlestick>& candles, int period);
	static std::deque<float> calculateRSI(const std::deque<Candlestick>& candles, int period);

};

#endif // FXTRADER_H