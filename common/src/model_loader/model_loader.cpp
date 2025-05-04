#include "model_loader/model_loader.hpp"
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <ostream>
#include <vector>

std::vector<ivp> parse_model_into_ivps(std::string str)
{
	Assimp::Importer importer;
	const aiScene *scene =
	importer.ReadFile(str, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error: Assimp - " << importer.GetErrorString() << std::endl;
    }
	return rec_process_nodes(scene->mRootNode, scene);
}

std::vector<ivp> rec_process_nodes(aiNode *node, const aiScene *scene)
{
	std::vector<ivp> ivps;
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        unsigned int mesh_index = node->mMeshes[i];
        aiMesh *mesh = scene->mMeshes[mesh_index];
        ivps.push_back(process_mesh_ivps(mesh, scene));
    }    
	for (unsigned int i = 0; i < node->mNumChildren; i++) 
	{
        	std::vector<ivp> x = rec_process_nodes(node->mChildren[i], scene);
		for (auto y : x)
		{
			ivps.push_back(y);
		}
    	}
	return ivps;
}

ivp process_mesh_ivps(aiMesh *mesh, const aiScene *scene) {
    std::vector<glm::vec3> vertices = process_mesh_vertex_positions(mesh);
    std::vector<unsigned int> indices = process_mesh_indices(mesh);
    return {indices, vertices};
};
glm::vec3 assimp_to_glm_3d_vector(const aiVector3D& vec) {
    return glm::vec3(vec.x, vec.y, vec.z);
}
std::vector<glm::vec3> process_mesh_vertex_positions(aiMesh *mesh) {
    std::vector<glm::vec3> vertices;
    /*spdlog::get(Systems::asset_loading)->info("This mesh has {} vertex_positions", mesh->mNumVertices);*/
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 vertex = {assimp_to_glm_3d_vector(mesh->mVertices[i])};
        vertices.push_back(vertex);
    }
    return vertices;
}

std::vector<unsigned int> process_mesh_indices(aiMesh *mesh) {
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        assert(face.mNumIndices == 3); // if this is false we are not working with triangles
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    return indices;
}
