#include "tgaimage.h"
#include <iostream>

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool xToy = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        xToy = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    for (int i = 0; i <= x1 - x0; i++) {
        int x = x0 + i;
        int y = y0 + (i * 1.0) / (x1 - x0) * (y1 - y0);
        if (xToy)
            image.set(y, x, color);
        else
            image.set(x, y, color);
    }
}

int main() {
    TGAImage image(512, 512, TGAImage::RGB);
// 绘制红色正方形
    line(100, 100, 100, 400, image, {255, 0, 0, 255});
    line(100, 400, 400, 400, image, {255, 0, 0, 255});
    line(400, 400, 400, 100, image, {255, 0, 0, 255});
    line(400, 100, 100, 100, image, {255, 0, 0, 255});
// 绘制蓝色三角形
    line(200, 200, 300, 200, image, {0, 0, 255, 255});
    line(300, 200, 250, 300, image, {0, 0, 255, 255});
    line(250, 300, 200, 200, image, {0, 0, 255, 255});
// 绘制绿色菱形
    line(250, 150, 300, 200, image, {0, 255, 0, 255});
    line(300, 200, 250, 250, image, {0, 255, 0, 255});
    line(250, 250, 200, 200, image, {0, 255, 0, 255});
    line(200, 200, 250, 150, image, {0, 255, 0, 255});
    image.write_tga_file("result.tga");
    return 0;
}