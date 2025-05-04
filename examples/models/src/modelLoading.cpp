#include "modelLoading.hpp"
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <ostream>
#include <vector>

std::vector<ivpt> parse_model_into_ivpts(std::string str)
{
	Assimp::Importer importer;
	const aiScene *scene =
	importer.ReadFile(str, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error: Assimp - " << importer.GetErrorString() << std::endl;
    }
	return rec_process_nodes(scene->mRootNode, scene);
}

std::vector<ivpt> rec_process_nodes(aiNode *node, const aiScene *scene)
{
	std::vector<ivpt> ivpts;
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        unsigned int mesh_index = node->mMeshes[i];
        aiMesh *mesh = scene->mMeshes[mesh_index];
        ivpts.push_back(process_mesh_ivpts(mesh, scene));
    }    
	for (unsigned int i = 0; i < node->mNumChildren; i++) 
	{
        	std::vector<ivpt> x = rec_process_nodes(node->mChildren[i], scene);
		for (auto y : x)
		{
			ivpts.push_back(y);
		}
    	}
	return ivpts;
}

ivpt process_mesh_ivpts(aiMesh *mesh, const aiScene *scene) {
    std::vector<glm::vec3> vertices = process_mesh_vertex_positions(mesh);
    std::vector<unsigned int> indices = process_mesh_indices(mesh);
    std::vector<glm::vec2> texture_coordinates = process_mesh_texture_coordinates(mesh);
    return {indices, vertices, texture_coordinates};
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

std::vector<glm::vec2> process_mesh_texture_coordinates(aiMesh *mesh) {
    std::vector<glm::vec2> texture_coordinates;
    // there is a better way to do this at some point
    bool mesh_has_texture_coordinates = mesh->mTextureCoords[0] != nullptr;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

        glm::vec2 texture_coordinate;
        if (mesh_has_texture_coordinates) {
            texture_coordinate = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            /*spdlog::get(Systems::asset_loading)->warn("This mesh doesn't have texture coordinates!");*/
            texture_coordinate = glm::vec2(0.0f, 0.0f);
        }
        texture_coordinates.push_back(texture_coordinate);
    }
    return texture_coordinates;
}