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
            int vIndex,tIndex,nIndex; // 顶点坐标索引、纹理坐标索引、面法线坐标索引
            while(iss>>vIndex>>trash>>tIndex>>trash>>nIndex){ // 忽略字符/
                vIndex--;
                triangle.emplace_back(vIndex);
            }
            triangles.push_back(triangle);
        }
    }
}

Model::~Model() = default;
