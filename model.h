#ifndef SOFTRASTERIZATIONRENDERER_MODEL_H
#define SOFTRASTERIZATIONRENDERER_MODEL_H
#include <vector>
#include "geometry.h"
#include "tgaimage.h"
class Model {
public:
    std::vector<Vector3f> vertexes; // 存储顶点坐标
    std::vector<std::vector<int> > triangles; // 存储三角形的顶点索引
    std::vector<Vector2f> textures; // 存储纹理坐标
    std::vector<std::vector<int> >triangles_textures; // 存储三角形的顶点纹理坐标索引
    TGAImage texture_image;
    void load_texture(const char * filename);
    explicit Model(const char *filename);
    ~Model();
};
#endif //SOFTRASTERIZATIONRENDERER_MODEL_H
