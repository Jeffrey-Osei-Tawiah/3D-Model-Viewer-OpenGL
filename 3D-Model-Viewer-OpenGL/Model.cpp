#include "Model.h"
#include <GL/glew.h>
#include <stb_image.h>

std::vector<Texture>loadedTextures;

unsigned int LoadTextureFromFile(std::string, std::string);
Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetupMesh();
}

void Mesh::DestroyMesh()
{
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::Draw(Shader& sh)
{
	int diffuseNr = 1;
	int specularNr = 1;
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string name = textures[i].type;
		std::string number;
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		// Set Sampler2D
		sh.SetInt(name + number, i);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

	glBindVertexArray(0);
}

void Model::Draw(Shader& sh)
{
	for (Mesh& mesh : meshes)
	{
		mesh.Draw(sh);
	}
}

void Model::LoadModel(std::string& filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_FlipUVs | aiProcess_Triangulate);

	if ((!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && !scene->mRootNode)
	{
		SDL_Log("Error Loading Model: %s\n", importer.GetErrorString());
		return;
	}

	directory = filePath.substr(0, filePath.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(const aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// positions
		glm::vec3 pos;
		pos.x = mesh->mVertices[i].x;
		pos.y = mesh->mVertices[i].y;
		pos.z = mesh->mVertices[i].z;
		vertex.position = pos;

		// Normal
		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		vertex.Normal = normal;

		//Texture Coords
		glm::vec2 texCoord;
		texCoord.x = mesh->mTextureCoords[0][i].x;
		texCoord.y = mesh->mTextureCoords[0][i].y;
		vertex.TexCoord = texCoord;

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	std::vector<Texture> diffuseTextures = LoadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, "texture_diffuse");
	std::vector<Texture> specularTextures = LoadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR, "texture_specular");

	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
	textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());

	this->meshes.push_back(Mesh(vertices, indices, textures));
}

std::vector<Texture> Model::LoadMaterialTextures(const aiMaterial* mat, const aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		Texture tex;
		aiString path;

		mat->GetTexture(type, i, &path);
		bool skip = false;
		for (Texture loadedTex : loadedTextures)
		{
			if (loadedTex.path == std::string(path.C_Str()))
			{
				skip = true;
				textures.push_back(loadedTex);
				break;
			}
		}

		if (!skip)
		{
			tex.id = LoadTextureFromFile(std::string(path.C_Str()), directory);
			tex.type = typeName;
			tex.path = std::string(path.C_Str());

			loadedTextures.push_back(tex);
			textures.push_back(tex);
		}
	}

	return textures;
}

unsigned int LoadTextureFromFile(std::string path, std::string directory)
{
	std::string sFilePath = (directory + '/' + path);
	const char* filePath = sFilePath.c_str();


	unsigned int tex = 0;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	int w, h, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filePath, &w, &h, &channels, 3);

	if (data)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	else
	{
		SDL_Log("Unable to load texture from file: %s", filePath);
	}

	return tex;
}