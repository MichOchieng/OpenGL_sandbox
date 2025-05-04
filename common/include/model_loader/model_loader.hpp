#pragma once

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>  // Specifically for vec3
#include <vector>

struct ivp {
	std::vector<unsigned int> indices;

	std::vector<glm::vec3> vert;
};
std::vector<ivp> parse_model_into_ivps(std::string str);


std::vector<ivp> rec_process_nodes(aiNode *node, const aiScene *scene);


ivp process_mesh_ivps(aiMesh *mesh, const aiScene *scene);

std::vector<glm::vec3> process_mesh_vertex_positions(aiMesh *mesh); 

std::vector<unsigned int> process_mesh_indices(aiMesh *mesh);
