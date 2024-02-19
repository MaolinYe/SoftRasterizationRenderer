#include "tgaimage.h"
#include <iostream>
#include <tuple>
#include "model.h"
int width,height;
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
std::tuple<float, float, float> computeBarycentric2D(float x, float y, Vector3f *v){
    float c1 = (x*(v[1].y - v[2].y) + (v[2].x - v[1].x)*y + v[1].x*v[2].y - v[2].x*v[1].y) / (v[0].x*(v[1].y - v[2].y) + (v[2].x - v[1].x)*v[0].y + v[1].x*v[2].y - v[2].x*v[1].y);
    float c2 = (x*(v[2].y - v[0].y) + (v[0].x - v[2].x)*y + v[2].x*v[0].y - v[0].x*v[2].y) / (v[1].x*(v[2].y - v[0].y) + (v[0].x - v[2].x)*v[1].y + v[2].x*v[0].y - v[0].x*v[2].y);
    float c3 = (x*(v[0].y - v[1].y) + (v[1].x - v[0].x)*y + v[0].x*v[1].y - v[1].x*v[0].y) / (v[2].x*(v[0].y - v[1].y) + (v[1].x - v[0].x)*v[2].y + v[0].x*v[1].y - v[1].x*v[0].y);
    return {c1,c2,c3};
}
void triangle(Vector3f*v,float*depth_buffer,TGAImage&image,TGAColor color){
    int minx=std::min(v[0].x,std::min(v[1].x,v[2].x));
    int miny=std::min(v[0].y,std::min(v[1].y,v[2].y));
    int maxX=std::max(v[0].x,std::max(v[1].x,v[2].x));
    int maxY=std::max(v[0].y,std::max(v[1].y,v[2].y));
    for(int x=minx;x<=maxX;x++){
        for(int y=miny;y<=maxY;y++){
            auto[alpha, beta, gamma] = computeBarycentric2D(x,y,v);
            if(alpha>=0&&beta>=0&&gamma>=0){
                float depth=alpha*v[0].z+beta*v[1].z+gamma*v[2].z;
                if(depth>depth_buffer[x*width+y]){
                    depth_buffer[x*width+y]=depth;
                    image.set(x,y,color);
                }
            }
        }
    }
}
Vector3f mvp(Vector3f v){
    return {(v.x+1)/2*width,(v.y+1)/2*height,v.z};
}
Vector3f crossProduct(Vector3f v1,Vector3f v2){
    return {v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x};
}
int main() {
    width=512,height=512;
    TGAImage image(width, height, TGAImage::RGB);
    Model model(R"(C:\Users\v_maolinye\Desktop\SoftRasterizationRenderer\obj\african_head.obj)");
    auto*depth_buffer=new float[width*height];
    for(int i=0;i<width*height;i++){
        depth_buffer[i]=-std::numeric_limits<float>::max();
    }
    Vector3f light_direction(0,0,-1);
    for(auto&t:model.triangles){
        Vector3f v[3],mvp_v[3];
        for(int i=0;i<3;i++){
            v[i]=model.vertexes[t[i]];
            mvp_v[i]= mvp(v[i]);
        }
        Vector3f normal= crossProduct(v[2]-v[0],v[1]-v[0]);
        float light_intensity=normal.normalize()*light_direction;
        if(light_intensity<0)
            light_intensity=-light_intensity;
        std::uint8_t color=light_intensity * 255;
        triangle(mvp_v,depth_buffer,image,{color,color,color, 255});
    }
    image.write_tga_file("result.tga");
    return 0;
}