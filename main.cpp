#include "tgaimage.h"
#include <iostream>
#include <tuple>
#include "model.h"
int width,height;
Vector3f light_direction(0,0,-1);
Vector3f camera_direction(0,0,-1);
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
void triangle(Vector3f*v,float*depth_buffer,TGAImage&image,Vector3f *normals,Vector2f*texture_uv,TGAImage&texture_image){
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
                    Vector2f uv=texture_uv[0]*alpha+texture_uv[1]*beta+texture_uv[2]*gamma;
                    TGAColor color=texture_image.get(uv.x,uv.y);
                    Vector3f normal=normals[0]*alpha+normals[1]*beta+normals[2]*gamma;
                    float l_i=normal*light_direction;
                    Vector3f half=(light_direction+camera_direction).normalize(); // 半程向量
                    float nh=normal*half;
                    float ka=10,kd=1,ks=1;
                    std::uint8_t ambient=ka*1;
                    TGAColor diffuse=color*std::max(l_i,-l_i)*kd;
                    std::uint8_t specular=255*ks*std::pow(std::max(nh,-nh),50);
                    image.set(x,y,diffuse+specular+ambient);
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
    width=1024,height=1024;
    TGAImage image(width, height, TGAImage::RGB);
    Model model(R"(C:\Users\v_maolinye\Desktop\SoftRasterizationRenderer\obj\diablo3_pose.obj)");
    model.load_texture(R"(C:\Users\v_maolinye\Desktop\SoftRasterizationRenderer\texture\diablo3_pose_diffuse.tga)");
    auto*depth_buffer=new float[width*height];
    for(int i=0;i<width*height;i++){
        depth_buffer[i]=-std::numeric_limits<float>::max();
    }
    for(int j=0;j<model.triangles.size();j++){
        Vector3f vertex[3],mvp_v[3],normal[3];
        Vector2f uv[3];
        for(int i=0;i<3;i++){
            vertex[i]=model.vertexes[model.triangles[j][i]];
            mvp_v[i]= mvp(vertex[i]);
            uv[i]=model.textures[model.triangles_textures[j][i]];
            normal[i]=model.normals[model.triangles_normals[j][i]];
        }
        triangle(mvp_v, depth_buffer, image, normal, uv, model.texture_image);
    }
    image.write_tga_file("result.tga");
    return 0;
}