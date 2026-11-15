#include "model_loader.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

bool ModelLoader::inspect(const std::string& path)
{
    // Load FBX File with assimp
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    if (scene == nullptr) {
        std::cerr << "[ERROR] Model load failed: " << importer.GetErrorString() << '\n';
        return false;
    }

    // Print file data
    std::cout << "[INFO] Mesh count: " << scene->mNumMeshes << '\n';

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        const aiMesh* mesh = scene->mMeshes[i];

        std::cout << "Mesh " << i
                  << " | Vertices: " << mesh->mNumVertices
                  << " | Faces: " << mesh->mNumFaces
                  << " | Normals: " << mesh->HasNormals()
                  << " | UV0: " << mesh->HasTextureCoords(0)
                  << '\n';
    }

    return true;
}