#include "model.h"
#include <iostream>
#include <fstream>
#include <sstream>

Model::Model(const char *filename) {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) {
    std::cerr<<"fail to load model"<<std::endl;
    exit(1);
    }
    std::string line;
    while(!in.eof()){
        std::getline(in,line);
        std::istringstream iss(line);
        char trash;
        if(!line.compare(0,2,"v ")){
            iss>>trash; // 忽略字符v
            Vector3f vertex;
            iss>>vertex.x>>vertex.y>>vertex.z;
            vertexes.push_back(vertex);
        }else if(!line.compare(0,2,"f ")){
            iss>>trash; // 忽略字符f
            std::vector<int>triangle;
            std::vector<int>triangle_textures;
            std::vector<int>triangle_normals;
            int vIndex,tIndex,nIndex; // 顶点坐标索引、纹理坐标索引、点法线坐标索引
            while(iss>>vIndex>>trash>>tIndex>>trash>>nIndex){ // 忽略字符/
                vIndex--;
                triangle.emplace_back(vIndex);
                tIndex--;
                triangle_textures.emplace_back(tIndex);
                nIndex--;
                triangle_normals.emplace_back(nIndex);
            }
            triangles.push_back(triangle);
            triangles_textures.push_back(triangle_textures);
            triangles_normals.push_back(triangle_normals);
        }else if(!line.compare(0,3,"vt ")){
            iss>>trash>>trash;
            Vector2f texture;
            iss>>texture.x>>texture.y;
            textures.push_back(texture);
        }else if(!line.compare(0,3,"vn ")){
            iss>>trash>>trash;
            Vector3f  normal;
            iss>>normal.x>>normal.y>>normal.z;
            normals.push_back(normal);
        }
    }
}

void Model::load_texture(const char *filename) {
    texture_image.read_tga_file(filename);
    texture_image.flip_vertically();
    int width=texture_image.width(),height=texture_image.height();
    for(auto&one:textures){
        one.x=one.x*width;
        one.y=one.y*height;
    }
}

Model::~Model() = default;
