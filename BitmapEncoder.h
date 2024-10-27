#include <stdio.h>
typedef int color;

struct Bitmap {
    int width;
    int height;
    color* pixels;
    Bitmap(int w, int h) : width(w), height(h) {
        pixels = new color[width * height];
    }
    ~Bitmap() {
        delete[] pixels;
    }
};

// Colors
const color GREEN = 0x001A4000;  // RGB(0, 64, 26)
const color WHITE = 0x00FFFFFF;  // RGB(255, 255, 255)

int writeFileHeader(FILE*& file, const Bitmap& bitmap) {
    int pixelDataSize = (bitmap.width * 3 + (4 - (bitmap.width * 3) % 4) % 4) * bitmap.height;
    int fileSize = 14 + 40 + pixelDataSize;
    short reserved = 0;
    int offset = 54;
    fputs("BM", file);
    fwrite(&fileSize, sizeof(int), 1, file);
    fwrite(&reserved, sizeof(short), 1, file);
    fwrite(&reserved, sizeof(short), 1, file);
    fwrite(&offset, sizeof(int), 1, file);
    return 0;
}

int writeInfoHeader(FILE*& file, const Bitmap& bitmap) {
    int size = 40;
    short biPlanes = 1;
    short bitCountPerPixel = 24;
    int compression = 0;
    int pixelDataSize = (bitmap.width * 3 + (4 - (bitmap.width * 3) % 4) % 4) * bitmap.height;
    int zero = 0;
    fwrite(&size, sizeof(int), 1, file);
    fwrite(&bitmap.width, sizeof(int), 1, file);
    fwrite(&bitmap.height, sizeof(int), 1, file);
    fwrite(&biPlanes, sizeof(short), 1, file);
    fwrite(&bitCountPerPixel, sizeof(short), 1, file);
    fwrite(&compression, sizeof(int), 1, file);
    fwrite(&pixelDataSize, sizeof(int), 1, file);
    fwrite(&zero, sizeof(int), 1, file);
    fwrite(&zero, sizeof(int), 1, file);
    fwrite(&zero, sizeof(int), 1, file);
    fwrite(&zero, sizeof(int), 1, file);
    return 0;
}

void writeBitmapData(FILE* file, const Bitmap& bitmap) {
    int padding = (4 - (bitmap.width * 3) % 4) % 4;
    unsigned char paddingData[3] = { 0, 0, 0 };

    for (int y = bitmap.height - 1; y >= 0; --y) {
        for (int x = 0; x < bitmap.width; ++x) {
            color pixel = bitmap.pixels[y * bitmap.width + x];
            unsigned char color[3] = {
                static_cast<unsigned char>((pixel >> 16) & 0xFF),
                static_cast<unsigned char>((pixel >> 8) & 0xFF),
                static_cast<unsigned char>(pixel & 0xFF)
            };
            fwrite(color, sizeof(unsigned char), 3, file);
        }
        fwrite(paddingData, sizeof(unsigned char), padding, file);
    }
}

void write(const Bitmap& bitmap, const char* filename) {
    FILE* file = fopen(filename,"wb");
    if ( !file ) {
        return;
    }
    writeFileHeader(file, bitmap);
    writeInfoHeader(file, bitmap);
    writeBitmapData(file, bitmap);
    fclose(file);
}

