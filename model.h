#ifndef SOFTRASTERIZATIONRENDERER_MODEL_H
#define SOFTRASTERIZATIONRENDERER_MODEL_H
#include <vector>
#include "geometry.h"
class Model {
public:
    std::vector<Vector3f> vertexes; // 存储顶点坐标
    std::vector<std::vector<int> > triangles; // 存储三角形的顶点索引
    explicit Model(const char *filename);
    ~Model();
};
#endif //SOFTRASTERIZATIONRENDERER_MODEL_H
