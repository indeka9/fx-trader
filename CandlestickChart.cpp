#include "CandlestickChart.h"


CandlestickChart::CandlestickChart(int w, int h) : Canvas(w, h), maxVisibleCandles(80){
  
}

CandlestickChart::~CandlestickChart() {

}


void CandlestickChart::draw() const{
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(panX, 10.0 / zoomLevel + panX, panY, 10.0 / zoomLevel + panY, -1.0, 1.0);
    glOrtho(0, 10.0 / zoomLevel , 0, 10.0 / zoomLevel , -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawBackground();
    drawGrid();
    drawAxes();
    drawCandlesticks();
    //drawVolumeBars();

    if (sma_on) drawSMA();  // Draw SMA line
    if (ema_on) drawEMA();  // Draw EMA line
    if (rsi_on) drawRSI();  // Draw RSI line
    drawLegend();
}

void CandlestickChart::move_left() {
    // Move the chart to the left by one candle width
    float candleWidth = 9.6f / maxVisibleCandles;
    panX += candleWidth;

    // Clamp panX to ensure it stays within valid bounds
    clampPanX();
}

void CandlestickChart::setCandlesticks(const std::deque<Candlestick>& candles) {

    candlesticks = candles;
    updatePanBounds();
}



void CandlestickChart::drawBackground()  const {
    glBegin(GL_QUADS);
    glColor3f(0.1f, 0.1f, 0.1f); // Dark gray at the bottom
    glVertex2f(0.0f, 0.0f);
    glVertex2f(10.0f, 0.0f);
    glColor3f(0.3f, 0.3f, 0.3f); // Lighter gray at the top
    glVertex2f(10.0f, 10.0f);
    glVertex2f(0.0f, 10.0f);
    glEnd();
}

// Function to draw grid lines
void CandlestickChart::drawGrid() const {

    glColor3f(0.3f, 0.3f, 0.3f); // Dark gray color for grid lines
    glBegin(GL_LINES);
    for (float i = 0.2f; i <= 10.0f; i += 0.2f) {
        // Vertical grid lines
        glVertex2f(i, 0.2f);
        glVertex2f(i, 10.0f);
        // Horizontal grid lines
        glVertex2f(0.2f, i);
        glVertex2f(10.0f, i);
    }
    glEnd();
}


void CandlestickChart::drawAxes()const {

    float maxCandleValue = Candlestick::findMaxValue(candlesticks);
    float minCandleValue = Candlestick::findMinValue(candlesticks);
    float scaleX = 9.6f / candlesticks.size();
    float scaleY = 9.6f / (maxCandleValue - minCandleValue);
    float offsetX = 0.2f;
    
    
    // Draw Y axis
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for Y axis
    glBegin(GL_LINES);
    glVertex2f(0.2f, 0.2f);
    glVertex2f(0.2f, 10.0f);
    glEnd();

    // Draw X axis
    glColor3f(0.0f, 1.0f, 1.0f); // Cyan color for X axis
    glBegin(GL_LINES);
    glVertex2f(10.0f, 0.2f);
    glVertex2f(0.2f, 0.2f);
    glEnd();

    // Label the axes
    glColor3f(1.0f, 1.0f, 1.0f); // White color for labels
    glRasterPos2f(9.5f, .5f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'X');
    glRasterPos2f(0.5f, 9.5f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Y');

    // Draw value labels along the Y axis
    for (float i = 0.2f; i <= 10.0f; i += 1.0f) {
        glRasterPos2f(0.1f, i);
        float value = (i - 0.2f) / scaleY + findMinValue(candlesticks);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(5) << value;
        std::string label = oss.str();
        for (char c : label) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }


    // Draw value labels along the X axis
    size_t step = candlesticks.size() / 10;
    for (size_t i = 0; i < candlesticks.size(); i += step) {
        float x = offsetX + (i * scaleX);
        glRasterPos2f(x, 0.1f);
        std::time_t timestamp = candlesticks[i].timestamp;
        std::tm tm;
        localtime_s(&tm, &timestamp);
        char buffer[6];
        std::strftime(buffer, sizeof(buffer), "%H:%M", &tm);
        std::string label(buffer);
        for (char c : label) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }
}


// In CandlestickChart.cpp
void CandlestickChart::drawLegend() const {

    
    if ((sma_on || ema_on || rsi_on) ==false) return ;

    float legendX = 1.5f;
    float legendY = 9.7f;
    float legendWidth = 1.5f;
    float legendHeight = 0.7f;
    float padding = 0.2f;

    // Draw the background rectangle for the legend
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f); // Semi-transparent black background
    glBegin(GL_QUADS);
    glVertex2f(legendX, legendY);
    glVertex2f(legendX + legendWidth, legendY);
    glVertex2f(legendX + legendWidth, legendY - legendHeight);
    glVertex2f(legendX, legendY - legendHeight);
    glEnd();

    // Draw the border
    glColor3f(1.0f, 1.0f, 1.0f); // White border
    glBegin(GL_LINE_LOOP);
    glVertex2f(legendX, legendY);
    glVertex2f(legendX + legendWidth, legendY);
    glVertex2f(legendX + legendWidth, legendY - legendHeight);
    glVertex2f(legendX, legendY - legendHeight);
    glEnd();

    float itemY = legendY - padding;

    
    
    // Draw the legend items
    if (sma_on)
    {
        glColor3f(0.0f, 0.0f, 1.0f); // Blue color for SMA
        glBegin(GL_LINES);
        glVertex2f(legendX + padding, itemY);
        glVertex2f(legendX + padding + 0.5f, itemY);
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f); // White text
        glRasterPos2f(legendX + padding + 0.6f, itemY - 0.05f);
        std::string smaText = "SMA";
        for (char c : smaText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }

    if (ema_on)
    {
        itemY -= 0.2f;
        glColor3f(1.0f, 0.0f, 1.0f); // Magenta color for EMA
        glBegin(GL_LINES);
        glVertex2f(legendX + padding, itemY);
        glVertex2f(legendX + padding + 0.5f, itemY);
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f); // White text
        glRasterPos2f(legendX + padding + 0.6f, itemY - 0.05f);
        std::string emaText = "EMA";
        for (char c : emaText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }

    if (rsi_on)
    {

        itemY -= 0.2f;
        glColor3f(0.5f, 0.7f, 1.0f); // Blue color for SMA line
        glBegin(GL_LINES);
        glVertex2f(legendX + padding, itemY);
        glVertex2f(legendX + padding + 0.5f, itemY);
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f); // White text
        glRasterPos2f(legendX + padding + 0.6f, itemY - 0.05f);
        std::string rsiText = "RSI";
        for (char c : rsiText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }
}

void CandlestickChart::drawCandlesticks() const {
    // Clamp panX to ensure it stays within valid bounds
    const_cast<CandlestickChart*>(this)->clampPanX();

    float maxCandleValue = Candlestick::findMaxValue(candlesticks);
    float minCandleValue = Candlestick::findMinValue(candlesticks);
    float scaleX = 9.6f / maxVisibleCandles; // Scale based on visible candles
    float scaleY = 9.6f / (maxCandleValue - minCandleValue);
    float offsetX = 0.2f;
    float offsetY = 0.2f - (minCandleValue * scaleY);

    double mouseX, mouseY;
    glfwGetCursorPos(glfwGetCurrentContext(), &mouseX, &mouseY);

    // Convert mouse position to world coordinates
    //float worldMouseX = panX + (mouseX / width) * (10.0f / zoomLevel);
    //float worldMouseY = panY + ((height - mouseY) / height) * (10.0f / zoomLevel);
    float worldMouseX = (mouseX / width) * (10.0f / zoomLevel);
    float worldMouseY = ((height - mouseY) / height) * (10.0f / zoomLevel);


    // Calculate the start and end indices for visible candles
    int startIndex = static_cast<int>(panX / scaleX);
    int endIndex = std::min(startIndex + maxVisibleCandles, static_cast<int>(candlesticks.size()));

    // Draw visible candlesticks
    for (int i = startIndex; i < endIndex; ++i) {
        bool isHovered = isMouseHovering(worldMouseX, worldMouseY, static_cast<float>(i - startIndex), candlesticks[i], scaleX, scaleY, offsetX, offsetY);
        drawCandlestick(static_cast<float>(i - startIndex), candlesticks[i], scaleX, scaleY, offsetX, offsetY, isHovered);
    }

    // Draw tooltips for visible candlesticks
    for (int i = startIndex; i < endIndex; ++i) {
        bool isHovered = isMouseHovering(worldMouseX, worldMouseY, static_cast<float>(i - startIndex), candlesticks[i], scaleX, scaleY, offsetX, offsetY);
        if (isHovered) {
            drawTooltip(i, mouseX, mouseY, candlesticks[i]);
        }
    }
}





void CandlestickChart::drawCandlestick(float x, const Candlestick& cs, float scaleX, float scaleY, float offsetX, float offsetY, bool isHovered) const {
    float padding = 0.2f; // Add padding to the x-axis
    float scaledOpen = (cs.open * scaleY) + offsetY;
    float scaledClose = (cs.close * scaleY) + offsetY;
    float scaledHigh = (cs.high * scaleY) + offsetY;
    float scaledLow = (cs.low * scaleY) + offsetY;

    // Determine the color based on whether the close price is higher or lower than the open price
    float colorTop[3], colorBottom[3];
    if (cs.close > cs.open) {
        colorTop[0] = 0.0f; colorTop[1] = 0.5f; colorTop[2] = 0.0f; // Darker green for top
        colorBottom[0] = 0.0f; colorBottom[1] = 1.0f; colorBottom[2] = 0.0f; // Lighter green for bottom
    }
    else {
        colorTop[0] = 0.5f; colorTop[1] = 0.0f; colorTop[2] = 0.0f; // Darker red for top
        colorBottom[0] = 1.0f; colorBottom[1] = 0.0f; colorBottom[2] = 0.0f; // Lighter red for bottom
    }

    // Draw the candlestick body with gradient
    glBegin(GL_QUADS);
    glColor3f(colorBottom[0], colorBottom[1], colorBottom[2]);
    glVertex2f(offsetX + padding + (x * scaleX) - (scaleX / 4), scaledOpen);
    glVertex2f(offsetX + padding + (x * scaleX) + (scaleX / 4), scaledOpen);
    glColor3f(colorTop[0], colorTop[1], colorTop[2]);
    glVertex2f(offsetX + padding + (x * scaleX) + (scaleX / 4), scaledClose);
    glVertex2f(offsetX + padding + (x * scaleX) - (scaleX / 4), scaledClose);
    glEnd();

    // Draw the candlestick wick
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    glVertex2f(offsetX + padding + (x * scaleX), scaledHigh);
    glVertex2f(offsetX + padding + (x * scaleX), scaledLow);
    glEnd();

    // Highlight the candlestick if hovered
    if (isHovered) {
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow border for highlight
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(offsetX + padding + (x * scaleX) - (scaleX / 4), scaledOpen);
        glVertex2f(offsetX + padding + (x * scaleX) + (scaleX / 4), scaledOpen);
        glVertex2f(offsetX + padding + (x * scaleX) + (scaleX / 4), scaledClose);
        glVertex2f(offsetX + padding + (x * scaleX) - (scaleX / 4), scaledClose);
        glEnd();
        glLineWidth(1.0f);
    }
}


void CandlestickChart::drawVolumeBars(const std::deque<Candlestick>& candles, float scaleX, float scaleY, float offsetX, float offsetY) {
    float padding = 0.2f; // Add padding to the x-axis
    for (size_t i = 0; i < candles.size(); ++i) {
        float volumeHeight = (candles[i].volume * scaleY) / 10.0f; // Scale volume height
        if (candles[i].close > candles[i].open) {
            glColor4f(0.0f, 1.0f, 0.0f, 0.5f); // Semi-transparent green for up volume bars
        }
        else {
            glColor4f(1.0f, 0.0f, 0.0f, 0.5f); // Semi-transparent red for down volume bars
        }
        glBegin(GL_QUADS);
        glVertex2f(offsetX + padding + (i * scaleX) - (scaleX / 4), offsetY);
        glVertex2f(offsetX + padding + (i * scaleX) + (scaleX / 4), offsetY);
        glVertex2f(offsetX + padding + (i * scaleX) + (scaleX / 4), offsetY + volumeHeight);
        glVertex2f(offsetX + padding + (i * scaleX) - (scaleX / 4), offsetY + volumeHeight);
        glEnd();
    }
}



// In CandlestickChart.cpp
bool CandlestickChart::isMouseHovering(float mouseX, float mouseY, float x, const Candlestick& cs, float scaleX, float scaleY, float offsetX, float offsetY) const {
    float padding = 0.2f;
    float scaledOpen = (cs.open * scaleY) + offsetY;
    float scaledClose = (cs.close * scaleY) + offsetY;
    float scaledHigh = (cs.high * scaleY) + offsetY;
    float scaledLow = (cs.low * scaleY) + offsetY;

    float candlestickX = offsetX + padding + (x * scaleX);
    float candlestickWidth = scaleX / 2;

    return mouseX >= candlestickX - candlestickWidth && mouseX <= candlestickX + candlestickWidth &&
        mouseY >= scaledLow && mouseY <= scaledHigh;
}




void CandlestickChart::drawRoundedRect(float x, float y, float width, float height, float radius) const {
//    int numSegments = 16; // Number of segments to approximate the rounded corners
//    float theta = 2.0f * 3.1415926f / float(numSegments);
//    float tangetialFactor = tanf(theta);
//    float radialFactor = cosf(theta);
//
//    // Draw the four corners
//    float cx = radius;
//    float cy = 0.0f;
//
//    // Top-right corner
//    glBegin(GL_TRIANGLE_FAN);
//    glVertex2f(x + width - radius, y + height - radius);
//    for (int i = 0; i <= numSegments; i++) {
//        glVertex2f(x + width - radius + cx, y + height - radius + cy);
//        float tx = -cy;
//        float ty = cx;
//        cx += tx * tangetialFactor;
//        cy += ty * tangetialFactor;
//        cx *= radialFactor;
//        cy *= radialFactor;
//    }
//    glEnd();
//
//    // Top-left corner
//    cx = radius;
//    cy = 0.0f;
//    glBegin(GL_TRIANGLE_FAN);
//    glVertex2f(x + radius, y + height - radius);
//    for (int i = 0; i <= numSegments; i++) {
//        glVertex2f(x + radius - cx, y + height - radius + cy);
//        float tx = -cy;
//        float ty = cx;
//        cx += tx * tangetialFactor;
//        cy += ty * tangetialFactor;
//        cx *= radialFactor;
//        cy *= radialFactor;
//    }
//    glEnd();
//
//    // Bottom-left corner
//    cx = radius;
//    cy = 0.0f;
//    glBegin(GL_TRIANGLE_FAN);
//    glVertex2f(x + radius, y + radius);
//    for (int i = 0; i <= numSegments; i++) {
//        glVertex2f(x + radius - cx, y + radius - cy);
//        float tx = -cy;
//        float ty = cx;
//        cx += tx * tangetialFactor;
//        cy += ty * tangetialFactor;
//        cx *= radialFactor;
//        cy *= radialFactor;
//    }
//    glEnd();
//
//    // Bottom-right corner
//    cx = radius;
//    cy = 0.0f;
//    glBegin(GL_TRIANGLE_FAN);
//    glVertex2f(x + width - radius, y + radius);
//    for (int i = 0; i <= numSegments; i++) {
//        glVertex2f(x + width - radius + cx, y + radius - cy);
//        float tx = -cy;
//        float ty = cx;
//        cx += tx * tangetialFactor;
//        cy += ty * tangetialFactor;
//        cx *= radialFactor;
//        cy *= radialFactor;
//    }
//    glEnd();
//
//    // Draw the four edges
//    glBegin(GL_QUADS);
//    // Top edge
//    glVertex2f(x + radius, y + height);
//    glVertex2f(x + width - radius, y + height);
//    glVertex2f(x + width - radius, y + height - radius);
//    glVertex2f(x + radius, y + height - radius);
//
//    // Bottom edge
//    glVertex2f(x + radius, y);
//    glVertex2f(x + width - radius, y);
//    glVertex2f(x + width - radius, y + radius);
//    glVertex2f(x + radius, y + radius);
//
//    // Left edge
//    glVertex2f(x, y + radius);
//    glVertex2f(x + radius, y + radius);
//    glVertex2f(x + radius, y + height - radius);
//    glVertex2f(x, y + height - radius);
//
//    // Right edge
//    glVertex2f(x + width, y + radius);
//    glVertex2f(x + width - radius, y + radius);
//    glVertex2f(x + width - radius, y + height - radius);
//    glVertex2f(x + width, y + height - radius);
//    glEnd();
}



void CandlestickChart::drawTooltip(int index,float mouseX, float mouseY, const Candlestick& cs) const {
    // Convert mouse coordinates to world coordinates
    float worldMouseX = panX + (mouseX / width) * (10.0f / zoomLevel);
    float worldMouseY = panY + ((height - mouseY) / height) * (10.0f / zoomLevel);

    // Create the tooltip text
    std::stringstream ss;

    
    if (sma_on && index < smaValues.size() && !std::isnan(smaValues[index])) {
        ss << "\nSMA:   " << std::fixed << std::setprecision(5) << smaValues[index];
    }
    if (ema_on && index < emaValues.size() && !std::isnan(emaValues[index])) {
        ss << "\nEMA:   " << std::fixed << std::setprecision(5) << emaValues[index];
    }
    if (rsi_on && index < rsiValues.size() && !std::isnan(rsiValues[index])) {
        ss << "\nRSI:  " << std::fixed << std::setprecision(5) << rsiValues[index];
    }
    ss << "\n\n";
    ss << "Open:  " << std::fixed << std::setprecision(5) << cs.open << "\n"
        << "High:  " << std::fixed << std::setprecision(5) << cs.high << "\n"
        << "Low:   " << std::fixed << std::setprecision(5) << cs.low << "\n"
        << "Close: " << std::fixed << std::setprecision(5) << cs.close;

    // Add SMA, EMA, and RSI values if the corresponding flags are enabled
    


    std::string tooltipText = ss.str();

    // Calculate the width and height of the tooltip based on the text
    int maxLineLength = 0;
    int numLines = 0;
    for (char c : tooltipText) {
        if (c == '\n') {
            numLines++;
            maxLineLength = 0;
        }
        else {
            maxLineLength++;
        }
    }
    numLines++; // Account for the last line

    // Adjust tooltip dimensions based on zoom level
    float textWidth = maxLineLength * 0.1f / zoomLevel; // Scale width with zoom
    float textHeight = numLines * 0.2f / zoomLevel;     // Scale height with zoom
    float padding = 0.2f / zoomLevel;                  // Scale padding with zoom
    float radius = 0.2f / zoomLevel;                   // Scale corner radius with zoom

    // Calculate the tooltip position to center it relative to the mouse cursor
    float tooltipX = worldMouseX - (textWidth / 2); // Center horizontally
    float tooltipY = worldMouseY + (textHeight / 2); // Center vertically

    // Clamp the tooltip position to stay within the chart bounds
    tooltipX = std::max(0.2f, std::min(tooltipX, 10.0f - (textWidth + 2 * padding)));
    tooltipY = std::max(textHeight + 2 * padding, std::min(tooltipY, 10.0f - (textHeight + 2 * padding)));

    //// Draw the shadow (slightly offset from the tooltip)
    //glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // Semi-transparent black shadow
    //drawRoundedRect(tooltipX + 0.05f / zoomLevel, tooltipY - 0.05f / zoomLevel, textWidth + 2 * padding, textHeight + 2 * padding, radius);

    //// Draw the tooltip background with rounded corners
    //glColor4f(0.2f, 0.2f, 0.2f, 0.9f); // Semi-transparent dark gray background
    //drawRoundedRect(tooltipX, tooltipY, textWidth + 2 * padding, textHeight + 2 * padding, radius);

    //// Draw the tooltip border
    //glColor3f(1.0f, 1.0f, 1.0f); // White border
    //glBegin(GL_LINE_LOOP);
    //glVertex2f(tooltipX + radius, tooltipY);
    //glVertex2f(tooltipX + textWidth + 2 * padding - radius, tooltipY);
    //glVertex2f(tooltipX + textWidth + 2 * padding, tooltipY + radius);
    //glVertex2f(tooltipX + textWidth + 2 * padding, tooltipY + textHeight + 2 * padding - radius);
    //glVertex2f(tooltipX + textWidth + 2 * padding - radius, tooltipY + textHeight + 2 * padding);
    //glVertex2f(tooltipX + radius, tooltipY + textHeight + 2 * padding);
    //glVertex2f(tooltipX, tooltipY + textHeight + 2 * padding - radius);
    //glVertex2f(tooltipX, tooltipY + radius);
    //glEnd();

    // Draw the tooltip text (centered)
    glColor3f(1.0f, 1.0f, 1.0f); // White text
    float lineY = tooltipY + padding + 0.1f / zoomLevel;
    std::istringstream iss(tooltipText);
    std::string line;
    while (std::getline(iss, line)) {
        // Calculate the width of the current line
        float lineWidth = line.length() * 0.1f / zoomLevel;

        // Calculate the starting X position to center the line
        float lineX = tooltipX + (textWidth + 2 * padding - lineWidth) / 2;

        // Draw the line
        glRasterPos2f(lineX, lineY);
        for (char c : line) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
        lineY += 0.2f / zoomLevel; // Move to the next line
    }
}




void CandlestickChart::drawRSI() const {
    if (rsiValues.empty()) return;

    // RSI is typically plotted on a scale of 0 to 100
    const float RSI_MIN = 0.0f;
    const float RSI_MAX = 100.0f;

    float scaleX = 9.6f / maxVisibleCandles; // Scale based on visible candles
    float scaleY = 9.6f / (RSI_MAX - RSI_MIN); // Scale for the y-axis (RSI values)
    float offsetX = 0.2f; // X-axis offset (left margin)
    float offsetY = 0.2f - (RSI_MIN * scaleY); // Y-axis offset (bottom margin)

    // Calculate the start and end indices for visible candles
    int startIndex = static_cast<int>(panX / scaleX);
    int endIndex = std::min(startIndex + maxVisibleCandles, static_cast<int>(candlesticks.size()));

    glColor3f(0.0f, 1.0f, 1.0f); // Cyan color for RSI
    glBegin(GL_LINE_STRIP);
    for (int i = startIndex; i < endIndex; ++i) {
        if (std::isnan(rsiValues[i])) continue;
        float x = offsetX + ((i - startIndex) * scaleX);
        float y = offsetY + (rsiValues[i] * scaleY);
        glVertex2f(x, y);
    }
    glEnd();
}

void CandlestickChart::drawSMA() const {
    if (smaValues.empty()) return;

    float maxCandleValue = Candlestick::findMaxValue(candlesticks);
    float minCandleValue = Candlestick::findMinValue(candlesticks);
    float scaleX = 9.6f / maxVisibleCandles; // Scale based on visible candles
    float scaleY = 9.6f / (maxCandleValue - minCandleValue);
    float offsetX = 0.2f;
    float offsetY = 0.2f - (minCandleValue * scaleY);

    // Calculate the start and end indices for visible candles
    int startIndex = static_cast<int>(panX / scaleX);
    int endIndex = std::min(startIndex + maxVisibleCandles, static_cast<int>(candlesticks.size()));

    glColor3f(0.0f, 0.0f, 1.0f); // Blue color for SMA
    glBegin(GL_LINE_STRIP);
    for (int i = startIndex; i < endIndex; ++i) {
        if (std::isnan(smaValues[i])) continue;
        float x = offsetX + ((i - startIndex) * scaleX);
        float y = (smaValues[i] * scaleY) + offsetY;
        glVertex2f(x, y);
    }
    glEnd();
}


void CandlestickChart::drawEMA() const {
    if (emaValues.empty()) return;

    float maxCandleValue = Candlestick::findMaxValue(candlesticks);
    float minCandleValue = Candlestick::findMinValue(candlesticks);
    float scaleX = 9.6f / maxVisibleCandles; // Scale based on visible candles
    float scaleY = 9.6f / (maxCandleValue - minCandleValue);
    float offsetX = 0.2f;
    float offsetY = 0.2f - (minCandleValue * scaleY);

    // Calculate the start and end indices for visible candles
    int startIndex = static_cast<int>(panX / scaleX);
    int endIndex = std::min(startIndex + maxVisibleCandles, static_cast<int>(candlesticks.size()));

    glColor3f(1.0f, 0.0f, 1.0f); // Magenta color for EMA
    glBegin(GL_LINE_STRIP);
    for (int i = startIndex; i < endIndex; ++i) {
        if (std::isnan(emaValues[i])) continue;
        float x = offsetX + ((i - startIndex) * scaleX);
        float y = (emaValues[i] * scaleY) + offsetY;
        glVertex2f(x, y);
    }
    glEnd();
}


float CandlestickChart::findMaxValue(const std::deque<Candlestick>& candles) {
    return std::max_element(candles.begin(), candles.end(),
        [](const Candlestick& a, const Candlestick& b) {
            return a.high < b.high;
        })->high;
}

float CandlestickChart::findMinValue(const std::deque<Candlestick>& candles) {
    return std::min_element(candles.begin(), candles.end(),
        [](const Candlestick& a, const Candlestick& b) { return a.low < b.low; })->low;
}

// In CandlestickChart.cpp
void CandlestickChart::setSMA(const std::deque<float>& sma) {
    smaValues = sma;
}

void CandlestickChart::setEMA(const std::deque<float>& ema) {
    emaValues = ema;
}

void CandlestickChart::setRSI(const std::deque<float>& rsi) {
    rsiValues = rsi;
}


// CandlestickChart.cpp
void CandlestickChart::handleKeyPress(int key, int action) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_R) {
            zoomLevel = 1.0f;
            panX = 0.0f;
            panY = 0.0f;
        }
        // Handle left and right arrow keys for horizontal scrolling
        else if (key == GLFW_KEY_LEFT) {
            handleHorizontalScroll(-1.0f); // Scroll left
        }
        else if (key == GLFW_KEY_RIGHT) {
            handleHorizontalScroll(1.0f); // Scroll right
        }

        else if (key == GLFW_KEY_S) {
            // Toggle SMA display
            sma_on = !sma_on;
        }

        else if (key == GLFW_KEY_E) {
            // Toggle EMA display
            ema_on = !ema_on;
        }
        else if (key == GLFW_KEY_I) {
            // Toggle RSI display
            rsi_on = !rsi_on;
        }
        // Add more key handling as needed
    }
}



void CandlestickChart::updatePanBounds() {
    // Calculate the total width of the candlestick data in world coordinates
    float totalWidth = candlesticks.size() * (9.6f / maxVisibleCandles);

    // Calculate the minimum and maximum panX values
    minPanX = 0.0f; // Start at the first candle
    maxPanX = totalWidth - 10.0f; // End at the last candle, adjusted for the chart width

    // Ensure maxPanX is not negative (if there are fewer candles than the visible area)
    maxPanX = std::max(0.0f, maxPanX);

    // Clamp the current panX to the new bounds
    clampPanX();
}