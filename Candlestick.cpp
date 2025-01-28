#include "Candlestick.h"


// Static methods for finding max and min values
float Candlestick::findMaxValue(const std::deque<Candlestick>& candles) {
    return std::max_element(candles.begin(), candles.end(),
        [](const Candlestick& a, const Candlestick& b) {
            return a.high < b.high;
        })->high;
}

float Candlestick::findMinValue(const std::deque<Candlestick>& candles) {
    return std::min_element(candles.begin(), candles.end(),
        [](const Candlestick& a, const Candlestick& b) { return a.low < b.low; })->low;
}

// Method to generate realistic candlesticks
std::deque<Candlestick> Candlestick::generateRealisticCandlesticks(int n) {
    std::deque<Candlestick> candlesticks(n);
    std::random_device rd;
    std::mt19937 gen(rd());

    // Randomize the starting price within a reasonable range (e.g., 1.0000 to 1.2000 for EUR/USD)
    std::uniform_real_distribution<> startPriceDist(1.0000, 1.2000);
    float startPrice = startPriceDist(gen);

    // Increase volatility for more significant price movements
    float volatility = 0.0050f; // Higher volatility for larger price swings

    // Simulate a trend (e.g., slight upward or downward drift)
    std::uniform_real_distribution<> trendDist(-0.0001, 0.0001); // Small daily trend
    float trend = trendDist(gen);

    // Randomize the time interval between candlesticks
    std::uniform_int_distribution<> timeIntervalDist(50, 70); // Random interval between 50 and 70 seconds

    std::time_t currentTime = std::time(nullptr); // Current time

    for (int i = 0; i < n; ++i) {
        if (i == 0) {
            candlesticks[i].open = startPrice;
        } else {
            candlesticks[i].open = candlesticks[i - 1].close;
        }

        // Simulate price movements with increased randomness
        std::normal_distribution<> priceChangeDist(0, volatility); // Normal distribution for price changes
        float change = priceChangeDist(gen) + trend; // Add trend to the price change
        candlesticks[i].close = candlesticks[i].open + change;

        // Simulate high and low prices with more randomness
        std::normal_distribution<> highLowDist(0, volatility * 2); // Wider range for high/low
        candlesticks[i].high = std::max(candlesticks[i].open, candlesticks[i].close) + std::abs(highLowDist(gen));
        candlesticks[i].low = std::min(candlesticks[i].open, candlesticks[i].close) - std::abs(highLowDist(gen));

        // Simulate volume with more randomness
        std::uniform_real_distribution<> volumeDist(500, 5000); // Random volume between 500 and 5000
        candlesticks[i].volume = volumeDist(gen);

        // Set timestamp with random intervals
        currentTime += timeIntervalDist(gen); // Add random time interval
        candlesticks[i].timestamp = currentTime;
    }

    return candlesticks;
}


// Method to generate realistic candlesticks
std::deque<Candlestick> Candlestick::generateRealisticCandlesticks(std::deque<Candlestick> candlesticks,int n) {
    
    std::random_device rd;
    std::mt19937 gen(rd());

    
    // Randomize the starting price within a reasonable range (e.g., 1.0000 to 1.2000 for EUR/USD)
    std::uniform_real_distribution<> startPriceDist(1.0000, 1.2000);
    float startPrice = startPriceDist(gen);

    // Increase volatility for more significant price movements
    float volatility = 0.0050f; // Higher volatility for larger price swings

    // Simulate a trend (e.g., slight upward or downward drift)
    std::uniform_real_distribution<> trendDist(-0.0001, 0.0001); // Small daily trend
    float trend = trendDist(gen);

    // Randomize the time interval between candlesticks
    std::uniform_int_distribution<> timeIntervalDist(50, 70); // Random interval between 50 and 70 seconds

    std::time_t currentTime = std::time(nullptr); // Current time

    int offset = candlesticks.size();
    candlesticks.resize(offset + n);

    for (int i = offset; i < n+offset; ++i) {

        if (i == 0) {
            candlesticks[i].open = startPrice;
        }
        else {
            candlesticks[i].open = candlesticks[i - 1].close;
        }

        // Simulate price movements with increased randomness
        std::normal_distribution<> priceChangeDist(0, volatility); // Normal distribution for price changes
        float change = priceChangeDist(gen) + trend; // Add trend to the price change
        candlesticks[i].close = candlesticks[i].open + change;

        // Simulate high and low prices with more randomness
        std::normal_distribution<> highLowDist(0, volatility * 2); // Wider range for high/low
        candlesticks[i].high = std::max(candlesticks[i].open, candlesticks[i].close) + std::abs(highLowDist(gen));
        candlesticks[i].low = std::min(candlesticks[i].open, candlesticks[i].close) - std::abs(highLowDist(gen));

        // Simulate volume with more randomness
        std::uniform_real_distribution<> volumeDist(500, 5000); // Random volume between 500 and 5000
        candlesticks[i].volume = volumeDist(gen);

        // Set timestamp with random intervals
        currentTime += timeIntervalDist(gen); // Add random time interval
        candlesticks[i].timestamp = currentTime;
    }

    return candlesticks;
}

// Static helper method to convert Period enum to seconds
int Candlestick::periodToSeconds(Candlestick::Period period) {
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

bool Candlestick::onTimer(Candlestick::Period timerperiod) {

    static std::chrono::steady_clock::time_point lastUpdateTime = std::chrono::steady_clock::now();
    bool _updateFlag = false;
    auto currentTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastUpdateTime).count();
    int periodInSeconds = periodToSeconds(timerperiod);

    if (duration >= periodInSeconds) {
        _updateFlag = !_updateFlag; // Toggle the update flag
        lastUpdateTime = currentTime;
    }

    return _updateFlag;
}
