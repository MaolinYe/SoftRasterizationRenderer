#include "tgaimage.h"
#include <iostream>
#include "model.h"
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
    int width=512,height=512;
    TGAImage image(width, height, TGAImage::RGB);
    Model model(R"(C:\Users\v_maolinye\Desktop\SoftRasterizationRenderer\obj\body.obj)");
    for(auto&triangle:model.triangles){
        for(int i=0;i<3;i++){
            auto v0=model.vertexes[triangle[i]];
            auto v1=model.vertexes[triangle[(i+1)%3]];
            int x0=(v0.x+1)*width/2;
            int y0=(v0.y+1)*height/2;
            int x1=(v1.x+1)*width/2;
            int y1=(v1.y+1)*height/2;
            line(x0,y0,x1,y1,image,{255,255,255,255});
        }
    }
    image.write_tga_file("result.tga");
    return 0;
}