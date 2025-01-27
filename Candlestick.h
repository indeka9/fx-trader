#pragma once
#ifndef CANDLESTICK_H
#define CANDLESTICK_H

#include <ctime>
#include <deque>
#include <random>
#include <algorithm>
#include <chrono>
#include <thread>
#include <iostream>
#include <functional> // For std::function

class Candlestick {
public:
    enum class Period {
        ONE_MINUTE,
        FIVE_MINUTES,
        FIFTEEN_MINUTES,
        THIRTY_MINUTES,
        ONE_HOUR,
        FOUR_HOURS,
        ONE_DAY,
        ONE_WEEK,
        ONE_MONTH
    };

    
    // Static methods for finding max and min values
    static float findMaxValue(const std::deque<Candlestick>& candles);

    static float findMinValue(const std::deque<Candlestick>& candles);

    // Method to generate realistic candlesticks
    static std::deque<Candlestick> generateRealisticCandlesticks(int n);
    static std::deque<Candlestick> generateRealisticCandlesticks(std::deque<Candlestick> candlesticks, int n);

    // Member variables
    double open;
    double high;
    double low;
    double close;
    double volume;
    std::time_t timestamp; // Timestamp for the candlestick


};

#endif // CANDLESTICK_H