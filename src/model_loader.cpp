#include "model_loader.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

bool ModelLoader::loadMeshData(const std::string& path, std::vector<MeshData>& outMeshes)
{
    // Load FBX File with assimp
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    if (scene == nullptr) {
        std::cerr << "[ERROR] Model load failed: " << importer.GetErrorString() << '\n';
        return false;
    }

    // Print file data
    std::cout << "[INFO] =========== File Data ===========\n" <<
         "Mesh count: " << scene->mNumMeshes << '\n';

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        const aiMesh* mesh = scene->mMeshes[i];

        std::cout << "Mesh " << i
                  << " | Vertices: " << mesh->mNumVertices
                  << " | Faces: " << mesh->mNumFaces
                  << " | Normals: " << mesh->HasNormals()
                  << " | UV0: " << mesh->HasTextureCoords(0)
                  << '\n';
    }

    // Load Mesh Data
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i){
        const aiMesh* mesh = scene->mMeshes[i];

        MeshData mesh_data;
        
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j){
            const aiVector3D& position = mesh->mVertices[j];
            const aiVector3D& normal = mesh->mNormals[j];
            const aiVector3D& uv = mesh->mTextureCoords[0][j];

            VertexData v_data;
            v_data.position = glm::vec3(position.x, position.y, position.z);
            v_data.normal = glm::vec3(normal.x, normal.y, normal.z);
            v_data.uv = glm::vec2(uv.x, uv.y);

            mesh_data.vertices.push_back(v_data);
        }

        for (unsigned int j = 0; j < mesh->mNumFaces; ++j){
            const aiFace& face = mesh->mFaces[j];

            for (unsigned int k = 0; k < face.mNumIndices; ++k){
                mesh_data.indices.push_back(face.mIndices[k]);
            }
        }

        outMeshes.push_back(mesh_data);
    }

    // Print Load Data
    std::cout << "[INFO] =========== Load Data ===========\n" <<
         "Mesh count: " << outMeshes.size() << '\n';

    for (unsigned int i = 0; i < outMeshes.size(); ++i) {
        const MeshData& mesh = outMeshes.at(i);
        std::cout << "Mesh " << i
                  << " | Vertices: " << mesh.vertices.size()
                  << " | Indices: " << mesh.indices.size()
                  << '\n';
    }

    return true;
}