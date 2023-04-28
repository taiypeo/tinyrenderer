#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model
{
private:
    std::vector<Vec3f> verts_, texture_coords_;
    std::vector<std::vector<int>> faces_, textures_;

public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int nfaces();
    int ntexture_coords();
    Vec3f vert(int i);
    Vec3f texture_coords(int i);
    std::vector<int> face(int idx);
    std::vector<int> texture(int idx);
};

#endif //__MODEL_H__
