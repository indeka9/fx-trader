#include "FxTrader.h"


#include <limits> // For std::numeric_limits<float>::quiet_NaN()

std::deque<float> FxTrader::calculateEMA(const std::deque<Candlestick>& candles, int period) {
    std::deque<float> ema;

    // Check for invalid input
    if (candles.size() < period || period <= 0) {
        return ema; // Return empty if there are not enough candles or invalid period
    }

    // Pad the beginning of the series with NaN
    for (int i = 0; i < period - 1; ++i) {
        ema.push_back(std::numeric_limits<float>::quiet_NaN());
    }

    // Calculate the smoothing factor
    float smoothingFactor = 2.0f / (period + 1);

    // Calculate the initial SMA as the first EMA value
    float sum = 0.0f;
    for (int i = 0; i < period; ++i) {
        sum += candles[i].close;
    }
    float initialEMA = sum / period;
    ema.push_back(initialEMA);

    // Calculate EMA for the remaining candles
    for (size_t i = period; i < candles.size(); ++i) {
        float currentEMA = (candles[i].close - ema.back()) * smoothingFactor + ema.back();
        ema.push_back(currentEMA);
    }

    return ema;
}


std::deque<float> FxTrader::calculateSMA(const std::deque<Candlestick>& candles, int period) {
    std::deque<float> sma;

    // Check for invalid input
    if (candles.size() < period || period <= 0) {
        return sma; // Return empty if there are not enough candles or invalid period
    }

    // Pad the beginning of the series with NaN
    for (int i = 0; i < period - 1; ++i) {
        sma.push_back(std::numeric_limits<float>::quiet_NaN());
    }

    // Calculate the initial SMA for the first 'period' candles
    float sum = 0.0f;
    for (int i = 0; i < period; ++i) {
        sum += candles[i].close;
    }
    sma.push_back(sum / period);

    // Calculate SMA for the remaining candles using a sliding window
    for (size_t i = period; i < candles.size(); ++i) {
        sum += candles[i].close - candles[i - period].close;
        sma.push_back(sum / period);
    }

    return sma;
}


std::deque<float> FxTrader::calculateRSI(const std::deque<Candlestick>& candles, int period) {
    std::deque<float> rsi;

    // Check for invalid input
    if (candles.size() < period + 1 || period <= 0) {
        return rsi; // Return empty if there are not enough candles or invalid period
    }

    // Pad the beginning of the series with NaN
    for (int i = 0; i < period; ++i) {
        rsi.push_back(std::numeric_limits<float>::quiet_NaN());
    }

    // Calculate initial gains and losses
    std::deque<float> gains;
    std::deque<float> losses;
    for (size_t i = 1; i < candles.size(); ++i) {
        float change = candles[i].close - candles[i - 1].close;
        if (change > 0) {
            gains.push_back(change);
            losses.push_back(0.0f);
        }
        else {
            gains.push_back(0.0f);
            losses.push_back(-change);
        }
    }

    float avgGain = 0.0f;
    float avgLoss = 0.0f;
    for (int i = 0; i < period; ++i) {
        avgGain += gains[i];
        avgLoss += losses[i];
    }
    avgGain /= period;
    avgLoss /= period;

    // Calculate the initial RSI
    if (avgLoss == 0.0f) {
        rsi.push_back(100.0f); // Avoid division by zero
    }
    else {
        float rs = avgGain / avgLoss;
        rsi.push_back(100.0f - (100.0f / (1.0f + rs)));
    }

    // Calculate RSI for the remaining candles using a sliding window
    for (size_t i = period; i < gains.size(); ++i) {
        avgGain = ((avgGain * (period - 1)) + gains[i]) / period;
        avgLoss = ((avgLoss * (period - 1)) + losses[i]) / period;

        if (avgLoss == 0.0f) {
            rsi.push_back(100.0f); // Avoid division by zero
        }
        else {
            float rs = avgGain / avgLoss;
            rsi.push_back(100.0f - (100.0f / (1.0f + rs)));
        }
    }

    return rsi;
}