#include <math.h>
#include "BitmapEncoder.h"

bool isInsideCircle(int x, int y, int centerX, int centerY, int radius) {
    int dx = x - centerX;
    int dy = y - centerY;
    return dx * dx + dy * dy <= radius * radius;
}

bool isInsideCrescent(int x, int y, int centerX, int centerY, int outerRadius, int innerRadius, int offset) {
    return isInsideCircle(x, y, centerX, centerY, outerRadius) &&
        !isInsideCircle(x, y, centerX + offset, centerY, innerRadius);
}

bool isInsideStar(int x, int y, int centerX, int centerY, int size) {
    const int points = 5;
    const double PI = 3.14159265358979323846;
    double px_prev, py_prev;
    bool firstPoint = true;

    for (int i = 0; i < points * 2; ++i) {
        double angle = i * PI / points - PI / 2;
        double radius = (i % 2 == 0) ? size : size / 2.0;
        double px = centerX + radius * cos(angle);
        double py = centerY + radius * sin(angle);

        if (!firstPoint) {
            double vx1 = px - px_prev;
            double vy1 = py - py_prev;
            double vx2 = x - px_prev;
            double vy2 = y - py_prev;
            if (vx1 * vy2 - vy1 * vx2 < 0) {
                return false;
            }
        }

        px_prev = px;
        py_prev = py;
        firstPoint = false;
    }
    return true;
}

void drawPakistaniFlag(Bitmap& bitmap) {
    int flagWidth = bitmap.width;
    int flagHeight = bitmap.height;
    int stripeWidth = flagWidth / 4;

    // Draw green background
    for (int y = 0; y < flagHeight; ++y) {
        for (int x = 0; x < flagWidth; ++x) {
            bitmap.pixels[y * flagWidth + x] = GREEN;
        }
    }

    // Draw white stripe
    for (int y = 0; y < flagHeight; ++y) {
        for (int x = 0; x < stripeWidth; ++x) {
            bitmap.pixels[y * flagWidth + x] = WHITE;
        }
    }

    // Draw crescent
    int centerX = flagWidth * 5 / 8;
    int centerY = flagHeight / 2;
    int outerRadius = flagHeight * 2 / 5;
    int innerRadius = flagHeight * 2.9 / 8;
    int offset = flagHeight / 16;

    // Draw star
    int starCenterX = centerX + outerRadius / 2;
    int starCenterY = centerY;
    int starSize = flagHeight / 6;

    for (int y = 0; y < flagHeight; ++y) {
        for (int x = stripeWidth; x < flagWidth; ++x) {
            if (isInsideCrescent(x, y, centerX, centerY, outerRadius, innerRadius, offset) ||
                isInsideStar(x, y, starCenterX, starCenterY, starSize)) {
                bitmap.pixels[y * flagWidth + x] = WHITE;
            }
        }
    }
}

int main() {
    int width=0,height = 0;
    printf("Enter width of Pakistani Flag: ");
    scanf("%d", &width);
    height = (float) width * 2 / 3; // for 3:2 ratio of green and white
    Bitmap bitmap(width, height);
    drawPakistaniFlag(bitmap);
    const char* filename = "PakistanFlag.bmp";
    write(bitmap, filename);

    return 0;
}
