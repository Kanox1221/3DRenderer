#include "model_loader.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <utility>


void printNodeInfo(const aiNode* node, unsigned int depth = 0)
{
    if (node == nullptr) {
        return;
    }

    const std::string indent(depth * 2, ' ');
    const aiMatrix4x4& m = node->mTransformation;

    std::cout << indent << "Node: " << node->mName.C_Str()
              << " | Meshes: " << node->mNumMeshes
              << " | Children: " << node->mNumChildren << '\n';

    std::cout << indent << "Mesh indices:";
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        std::cout << ' ' << node->mMeshes[i];
    }
    std::cout << '\n';

    std::cout << indent << m.a1 << ' ' << m.a2 << ' '
              << m.a3 << ' ' << m.a4 << '\n';
    std::cout << indent << m.b1 << ' ' << m.b2 << ' '
              << m.b3 << ' ' << m.b4 << '\n';
    std::cout << indent << m.c1 << ' ' << m.c2 << ' '
              << m.c3 << ' ' << m.c4 << '\n';
    std::cout << indent << m.d1 << ' ' << m.d2 << ' '
              << m.d3 << ' ' << m.d4 << "\n\n";

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        printNodeInfo(node->mChildren[i], depth + 1);
    }
}

void processNode(const aiNode* node, const aiScene* scene, const aiMatrix4x4& parentTransform, std::vector<MeshData>& outMeshes)
{
    // Accumulate Transformation
    const aiMatrix4x4 transform = parentTransform * node->mTransformation;

    aiMatrix3x3 normalTransform(transform);
    normalTransform.Inverse().Transpose();

    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        // Mesh Reading
        const unsigned int meshIndex = node->mMeshes[i];
        const aiMesh* mesh = scene->mMeshes[meshIndex];

        MeshData meshData;
        meshData.vertices.reserve(mesh->mNumVertices);

        // Bounding Box
        glm::vec3 minPos(0.0f);
        glm::vec3 maxPos(0.0f);

        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            // Vertex Reading
            VertexData vertex{};
            vertex.normal = glm::vec3(0.0f);
            vertex.uv = glm::vec2(0.0f);

            // Apply Accumulated Transformation
            const aiVector3D position = transform * mesh->mVertices[j];
            vertex.position = glm::vec3(position.x, position.y, position.z);

            if (j == 0) {
                // Bounding Box Initialization
                minPos = vertex.position;
                maxPos = vertex.position;
            } else {
                // Bounding Box Comparison
                minPos = glm::min(minPos, vertex.position);
                maxPos = glm::max(maxPos, vertex.position);
            }

            if (mesh->HasNormals()) {
                aiVector3D normal = normalTransform * mesh->mNormals[j];

                if (normal.SquareLength() > 0.0f) {
                    normal.Normalize();
                }

                vertex.normal = glm::vec3(normal.x, normal.y, normal.z);
            }

            if (mesh->HasTextureCoords(0)) {
                const aiVector3D& uv = mesh->mTextureCoords[0][j];
                vertex.uv = glm::vec2(uv.x, uv.y);
            }

            meshData.vertices.push_back(vertex);
        }

        for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            // Index Reading
            const aiFace& face = mesh->mFaces[j];

            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                meshData.indices.push_back(face.mIndices[k]);
            }
        }

        // Print Bounding Box
        if (!meshData.vertices.empty()) {
            const glm::vec3 size = maxPos - minPos;
            const glm::vec3 center = (minPos + maxPos) * 0.5f;

            std::cout << "Node: " << node->mName.C_Str()
                << " | Mesh " << meshIndex
                << " | Min: (" << minPos.x << ", " << minPos.y << ", " << minPos.z << ")"
                << " | Max: (" << maxPos.x << ", " << maxPos.y << ", " << maxPos.z << ")"
                << " | Size: (" << size.x << ", " << size.y << ", " << size.z << ")"
                << " | Center: (" << center.x << ", " << center.y << ", " << center.z << ")"
                << '\n';
        }

        outMeshes.push_back(std::move(meshData));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, transform, outMeshes);
    }
}

bool ModelLoader::loadMeshData(const std::string& path, std::vector<MeshData>& outMeshes)
{
    // Load FBX File with assimp
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    if (scene == nullptr) {
        std::cerr << "[ERROR] Model load failed: " << importer.GetErrorString() << '\n';
        return false;
    }

    if (scene->mRootNode == nullptr) {
        std::cerr << "[ERROR] Model has no root node\n";
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
    outMeshes.clear();

    const aiMatrix4x4 identity;
    processNode(scene->mRootNode, scene, identity, outMeshes);

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

    // Print Node Transform Data
    std::cout << "[INFO] =========== Node Transform Data ===========" << "\n";

    printNodeInfo(scene->mRootNode);

    return true;
}