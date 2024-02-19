# SoftRasterizationRenderer
不依赖第三方库的软件的光栅化渲染器
## ① 画线
对于给定两个点，我们需要画出两点形成的线段，像素是离散的，这就需要在两个点之间画上足够的点，取Δx和Δy中的较大者为采样率
```c++
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool xToy = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        xToy= true;
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
```
绘制一些简单的几何图形
```c++
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
```
![img.png](outputPictures/line.png)
## ② 读取模型文件
二维向量的运算
```c++
template<class T>
struct Vec2 {
    T x, y;
    Vec2() : x(0), y(0) {}
    Vec2(T x, T y) : x(x), y(y) {}
    Vec2<T> operator+(const Vec2<T> V) const {
        return Vec2<T>(x + V.x, y + V.y);
    }
    Vec2<T> operator-(const Vec2<T> V) const {
        return Vec2<T>(x - V.x, y - V.y);
    }
    Vec2<T> operator*(float k) const {
        return Vec2<T>(x * k, y * k);
    }
};
typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
```
三维向量的运算
```c++
template<class T>
struct Vec3 {
    T x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    Vec3<T> operator^(const Vec3<T> &v) const {
        return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    Vec3<T> operator+(const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
    Vec3<T> operator-(const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
    Vec3<T> operator*(float f) const { return Vec3<T>(x * f, y * f, z * f); }
    T operator*(const Vec3<T> &v) const { return x * v.x + y * v.y + z * v.z; }
    float norm() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3<T> &normalize(T l = 1) {
        *this = (*this) * (l / norm());
        return *this;
    }
};
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
```
### 读取模型文件
这里先只读取模型的三角形坐标，纹理和法线后面再用
```c++
class Model {
public:
    std::vector<Vec3f> vertexes; // 存储顶点坐标
    std::vector<std::vector<int> > triangles; // 存储三角形的顶点索引
    explicit Model(const char *filename);
    ~Model();
};
```
记录顶点坐标和三角形的顶点索引，模型文件中的索引从1开始
```c++
    std::string line;
    while(!in.eof()){
        std::getline(in,line);
        std::istringstream iss(line);
        char trash;
        if(!line.compare(0,2,"v ")){
            iss>>trash; // 忽略字符v
            Vec3f vertex;
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
```
### 世界坐标系转换到屏幕坐标系
模型文件空间为[-1,1]³，屏幕坐标为二维的width×height，这里先直接忽略z坐标，即固定看向-z方向，x和y先+1转正后除以2乘以宽高
```c++
    int width=512,height=512;
    TGAImage image(width, height, TGAImage::RGB);
    Model model("C:\\Users\\v_maolinye\\Desktop\\SoftRasterizationRenderer\\obj\\african_head.obj");
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
```
![img.png](outputPictures/modelLine.png)
![img.png](outputPictures/bodyLine.png)