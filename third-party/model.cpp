#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_()
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail())
        return;
    std::string line;
    while (!in.eof())
    {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v "))
        {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++)
                iss >> v.raw[i];
            verts_.push_back(v);
        }
        else if (!line.compare(0, 4, "vt  "))
        {
            iss >> trash >> trash;
            Vec3f t;
            for (int i = 0; i < 3; i++)
                iss >> t.raw[i];
            texture_coords_.push_back(t);
        }
        else if (!line.compare(0, 2, "f "))
        {
            std::vector<int> f, t;
            int itrash, idx, t_idx;
            iss >> trash;
            while (iss >> idx >> trash >> t_idx >> trash >> itrash)
            {
                idx--; // in wavefront obj all indices start at 1, not zero
                t_idx--;
                f.push_back(idx);
                t.push_back(t_idx);
            }
            faces_.push_back(f);
            textures_.push_back(t);
        }
    }
    std::cerr << "# v# " << verts_.size() << " tc# " << texture_coords_.size() << " f# " << faces_.size() << " t# " << textures_.size() << std::endl;
}

Model::~Model()
{
}

int Model::nverts()
{
    return (int)verts_.size();
}

int Model::nfaces()
{
    return (int)faces_.size();
}

int Model::ntexture_coords()
{
    return (int)texture_coords_.size();
}

std::vector<int> Model::face(int idx)
{
    return faces_[idx];
}

Vec3f Model::vert(int i)
{
    return verts_[i];
}

Vec3f Model::texture_coords(int i)
{
    return texture_coords_[i];
}

std::vector<int> Model::texture(int idx)
{
    return textures_[idx];
}
