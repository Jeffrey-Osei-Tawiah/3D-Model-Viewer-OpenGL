#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <SDL3/SDL_log.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Shader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

struct Texture
{
	unsigned int id;
	// can be texture_diffuse or texture_specular
	// for now
	std::string type;
	std::string path;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex>&, std::vector<unsigned int>&, std::vector<Texture>&);
	void DestroyMesh(); // unused. TODO: use!
	void Draw(Shader&);

private:
	void SetupMesh();
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	unsigned VAO, VBO, EBO;
};

class Model
{
public:
	Model(std::string filePath)
	{
		SDL_Log("Loading Model: %s\n", filePath.c_str());
		LoadModel(filePath);
		SDL_Log("Done Loading Model: %s\n", filePath.c_str());
	}
	void Draw(Shader&);

private:
	std::string directory;
	std::vector <Mesh> meshes;
	void LoadModel(std::string& filePath);

	void ProcessNode(const aiNode* node, const aiScene* scene);
	void ProcessMesh(const aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(const aiMaterial* mat, const aiTextureType type, std::string typeName);
};