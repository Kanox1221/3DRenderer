#pragma once

#include "mesh_data.hpp"
#include <string>

class ModelLoader
{
public:
    bool loadMeshData(const std::string& path, std::vector<MeshData>& outMeshes);
};