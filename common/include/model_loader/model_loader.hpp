#pragma once

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>  // Specifically for vec3
#include <vector>

struct ivpt {
	std::vector<unsigned int> indices;

	std::vector<glm::vec3> vert;

	std::vector<glm::vec2> tex_cords;
};
std::vector<ivpt> parse_model_into_ivpts(std::string str);


std::vector<ivpt> rec_process_nodes(aiNode *node, const aiScene *scene);


ivpt process_mesh_ivpts(aiMesh *mesh, const aiScene *scene);

std::vector<glm::vec3> process_mesh_vertex_positions(aiMesh *mesh); 

std::vector<unsigned int> process_mesh_indices(aiMesh *mesh);

std::vector<glm::vec2> process_mesh_texture_coordinates(aiMesh *mesh);