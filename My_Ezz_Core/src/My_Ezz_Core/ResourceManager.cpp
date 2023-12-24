#include "My_Ezz_Core/ResourceManager.hpp"
#include "My_Ezz_Multimedia/AudioBase.hpp"
#include "My_Ezz_Logger/Log.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/ShaderProgram.hpp"
#include "My_Ezz_Core/Objects/Object.hpp"
#include "My_Ezz_Core/Objects/LightBase.hpp"
#include "My_Ezz_Core/Objects/Mesh/BaseMesh.hpp"
#include "My_Ezz_Core/Objects/Mesh/LightMesh.hpp"


#include "My_Ezz_Core/Rendering/OpenGL/VertexBuffer.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexArray.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/IndexBuffer.hpp"
#include "My_Ezz_Core/Objects/Material.hpp"
#include "My_Ezz_Core/Objects/MaterialLib.hpp"
//#include <rapidjson/document.h>
//#include <rapidjson/error/en.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <regex>
#include <iterator>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "My_Ezz_Core/Objects/Image/stb_image.h"


std::string ResourceManager::path;
ResourceManager::SoundsMap  ResourceManager::m_sounds;
ResourceManager::ShaderProgramsMap  ResourceManager::m_shadersPrograms;
ResourceManager::ObjectsMap  ResourceManager::m_objects;
ResourceManager::TexturesMap  ResourceManager::m_textures;

void ResourceManager::unloadAllResources()
{
	m_sounds.clear();
	path.clear();
}

void ResourceManager::setExecutablePath(const std::string& execitablePath)
{
	size_t found = execitablePath.find_last_of("/\\");
	path = execitablePath.substr(0, found);
}


std::string ResourceManager::getFileString(const std::string& ralativeFilePath)
{
	std::ifstream file;
	file.open(path + "/" + ralativeFilePath.c_str(), std::ios::in, std::ios::binary);
	if (!file.is_open())
	{
		//cerr << "Failed to open file: " + ralativeFilePath << std::endl;
		return std::string{};
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

std::vector<std::string> ResourceManager::split(const std::string& strInput, const std::string& strRegex)
{
	std::regex re(strRegex);
	std::sregex_token_iterator
		first{ strInput.begin(), strInput.end(), re, -1 },
		last;
	return { first, last };
}

std::int32_t convert_to_int(char* buffer, std::size_t len)
{
	std::int32_t a = 0;
	if (std::endian::native == std::endian::little)
		std::memcpy(&a, buffer, len);
	else
		for (std::size_t i = 0; i < len; ++i)
			reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
	return a;
}

bool load_wav_file_header(std::ifstream& file,
	std::uint8_t& channels,
	std::int32_t& sampleRate,
	std::uint8_t& bitsPerSample,
	int& size)
{
	char buffer[4];
	if (!file.is_open())
		return false;

	// the RIFF
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read RIFF" << std::endl;
		return false;
	}
	if (std::strncmp(buffer, "RIFF", 4) != 0)
	{
		std::cerr << "ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)" << std::endl;
		return false;
	}

	// the size of the file
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read size of file" << std::endl;
		return false;
	}

	// the WAVE
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read WAVE" << std::endl;
		return false;
	}
	if (std::strncmp(buffer, "WAVE", 4) != 0)
	{
		std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
		return false;
	}

	// "fmt/0"
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read fmt/0" << std::endl;
		return false;
	}

	// this is always 16, the size of the fmt data chunk
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read the 16" << std::endl;
		return false;
	}

	// PCM should be 1?
	if (!file.read(buffer, 2))
	{
		std::cerr << "ERROR: could not read PCM" << std::endl;
		return false;
	}

	// the number of channels
	if (!file.read(buffer, 2))
	{
		std::cerr << "ERROR: could not read number of channels" << std::endl;
		return false;
	}
	channels = convert_to_int(buffer, 2);

	// sample rate
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read sample rate" << std::endl;
		return false;
	}
	sampleRate = convert_to_int(buffer, 4);

	// (sampleRate * bitsPerSample * channels) / 8
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
		return false;
	}

	// ?? dafaq
	if (!file.read(buffer, 2))
	{
		std::cerr << "ERROR: could not read dafaq" << std::endl;
		return false;
	}

	// bitsPerSample
	if (!file.read(buffer, 2))
	{
		std::cerr << "ERROR: could not read bits per sample" << std::endl;
		return false;
	}
	bitsPerSample = convert_to_int(buffer, 2);

	// data chunk header "data"
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read data chunk header" << std::endl;
		return false;
	}
	if (std::strncmp(buffer, "data", 4) != 0)
	{
		std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
		return false;
	}

	// size of data
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read data size" << std::endl;
		return false;
	}
	size = convert_to_int(buffer, 4);

	/* cannot be at the end of file */
	if (file.eof())
	{
		std::cerr << "ERROR: reached EOF on the file" << std::endl;
		return false;
	}
	if (file.fail())
	{
		std::cerr << "ERROR: fail state set on the file" << std::endl;
		return false;
	}

	return true;
}

char* load_wav(const std::string& filename,
	std::uint8_t& channels,
	std::int32_t& sampleRate,
	std::uint8_t& bitsPerSample,
	int& size)
{
	std::ifstream in(filename, std::ios::binary);
	if (!in.is_open())
	{
		LOG_CRITICAL("Невозможно открыть файл: {0}", filename);
		return nullptr;
	}
	if (!load_wav_file_header(in, channels, sampleRate, bitsPerSample, size))
	{
		LOG_CRITICAL("Невозможно открыть загрузить wav заголовок: {0}", filename);
		return nullptr;
	}

	char* data = new char[size];

	in.read(data, size);

	return data;
}



void ResourceManager::UnloadAllResources()
{
	m_shadersPrograms.clear();
	m_sounds.clear();
	path.clear();
}

std::shared_ptr<My_Ezz::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty())
	{
		LOG_CRITICAL("No vertex shader");
		return nullptr;
	}
	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		LOG_CRITICAL("No fragment shader");
		return nullptr;
	}

	std::shared_ptr<My_Ezz::ShaderProgram>& newShader =
		m_shadersPrograms.emplace(shaderName, std::make_shared<My_Ezz::ShaderProgram>(vertexString.c_str(), fragmentString.c_str())).first->second;
	if (newShader->isCompiled())
		return newShader;
	else
	{
		LOG_CRITICAL("Can't load shader program:  Vertex: {0}, Fragment: {1}", vertexPath, fragmentPath);
		return nullptr;
	}
}

std::shared_ptr<My_Ezz::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shadersPrograms.find(shaderName);
	if (it != m_shadersPrograms.end())
	{
		return it->second;
	}
	else
	{
		LOG_CRITICAL("Can't find the shader program: {0}", shaderName);
		return nullptr;
	}
}

std::shared_ptr<My_Ezz::AudioBase> ResourceManager::loadSound(const std::string& soundName, const std::string& soundPath)
{
	std::uint8_t channels;
	std::int32_t sampleRate;
	std::uint8_t bitsPerSample;
	char* soundData;
	std::int32_t size;
	soundData = load_wav(path + "/" + soundPath, channels, sampleRate, bitsPerSample, size);
	std::shared_ptr<My_Ezz::AudioBase>& newSound = m_sounds.emplace(soundName,
		std::make_shared<My_Ezz::AudioBase>(soundData, channels, sampleRate, bitsPerSample, size)).first->second;
	return newSound;
}

std::shared_ptr<My_Ezz::AudioBase> ResourceManager::getSound(const std::string& soundName)
{
	SoundsMap::const_iterator it = m_sounds.find(soundName);
	if (it != m_sounds.end())
	{
		return it->second;
	}
	else
	{
		//cerr << "Can't find the sprite: " << soundName << endl;
		return nullptr;
	}
}

std::shared_ptr<My_Ezz::Object> ResourceManager::loadObject(const std::string& strObjectName, const std::string& strObjectPath, EObjectType eType)
{
	struct Helper
	{
		static void CalcTBN(std::list<GLfloat>& vertexBuff)
		{
			double x, y, z;
			for (int i = 0; i < vertexBuff.size();)
			{
				std::list<GLfloat>::iterator Iter = vertexBuff.begin();
				std::advance(Iter, i);
				//point 1
				x = *Iter;
				y = *(++Iter);
				z = *(++Iter);
				glm::vec3 v1 = glm::vec3(x, y, z);
				std::advance(Iter, 3);
				x = *(++Iter);
				y = *(++Iter);
				glm::vec2 uv1 = glm::vec2(x, y);
				//point 2
				x = *(++Iter);
				y = *(++Iter);
				z = *(++Iter);
				glm::vec3 v2 = glm::vec3(x, y, z);
				std::advance(Iter, 3);
				x = *(++Iter);
				y = *(++Iter);
				glm::vec2 uv2 = glm::vec2(x, y);
				//point 3
				x = *(++Iter);
				y = *(++Iter);
				z = *(++Iter);
				glm::vec3 v3 = glm::vec3(x, y, z);
				std::advance(Iter, 3);
				x = *(++Iter);
				y = *(++Iter);
				glm::vec2 uv3 = glm::vec2(x, y);

				glm::vec3 deltaPos1 = v2 - v1;
				glm::vec3 deltaPos2 = v3 - v1;

				glm::vec2 deltaUV1 = uv2 - uv1;
				glm::vec2 deltaUV2 = uv3 - uv1;

				float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

				glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
				glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
				//point 1
				Iter = vertexBuff.begin();
				std::advance(Iter, i+8);
				vertexBuff.emplace(Iter, tangent.x);
				vertexBuff.emplace(Iter, tangent.y);
				vertexBuff.emplace(Iter, tangent.z);
				vertexBuff.emplace(Iter, bitangent.x);
				vertexBuff.emplace(Iter, bitangent.y);
				vertexBuff.emplace(Iter, bitangent.z);
				//point 2
				std::advance(Iter, 8);
				vertexBuff.emplace(Iter, tangent.x);
				vertexBuff.emplace(Iter, tangent.y);
				vertexBuff.emplace(Iter, tangent.z);
				vertexBuff.emplace(Iter, bitangent.x);
				vertexBuff.emplace(Iter, bitangent.y);
				vertexBuff.emplace(Iter, bitangent.z);
				//point 3
				std::advance(Iter, 8);
				vertexBuff.emplace(Iter, tangent.x);
				vertexBuff.emplace(Iter, tangent.y);
				vertexBuff.emplace(Iter, tangent.z);
				vertexBuff.emplace(Iter, bitangent.x);
				vertexBuff.emplace(Iter, bitangent.y);
				vertexBuff.emplace(Iter, bitangent.z);
				i += 42;
			}
		}
	};


	std::ifstream file;
	file.open(path + "/" + strObjectPath.c_str(), std::ios::in, std::ios::binary);
	if (!file.is_open())
	{
		return nullptr;
	}

	std::vector<glm::vec3> coords;
	std::vector<glm::vec2> textCoords;
	std::vector<glm::vec3> normals;

	std::shared_ptr<My_Ezz::VertexBuffer> vertexes;
	std::shared_ptr<My_Ezz::IndexBuffer> indexes;

	std::shared_ptr<My_Ezz::Object> object;
	if (eType == EObjectType::kBase)
		object = std::make_shared<My_Ezz::Object>();
	else if (eType == EObjectType::kLight)
		object = std::make_shared<My_Ezz::LightBase>();

	std::shared_ptr<My_Ezz::BaseMesh> pMesh;
	std::string strMtlName;

	std::string line;

	std::list<GLfloat> vertBuffer;
	std::list<GLint> indexBuffer;

	My_Ezz::BufferLayout bufferLayout_2vec3_vec2
	{
		My_Ezz::ShaderDataType::Float3,
		My_Ezz::ShaderDataType::Float3,
		My_Ezz::ShaderDataType::Float2,
		My_Ezz::ShaderDataType::Float3,
		My_Ezz::ShaderDataType::Float3
	};

	while (std::getline(file, line))
	{
		std::vector<std::string> list = split(line, " ");

		if (list[0] == "#")
		{
			continue;
		}
		else if (list[0] == "mtllib")
		{
			std::vector<std::string> localPath = split(strObjectPath, "/");
			loadMaterial(localPath[0] + "/" + localPath[1] + "/" + list[1], object->GetMtlLib());
			continue;
		}
		else if (list[0] == "v")
		{
			coords.push_back(glm::vec3(std::stod(list[1]), std::stod(list[2]), std::stod(list[3])));
			continue;
		}
		else if (list[0] == "vt")
		{
			textCoords.push_back(glm::vec2(std::stod(list[1]), std::stod(list[2])));
			continue;
		}
		else if (list[0] == "vn")
		{
			normals.push_back(glm::vec3(std::stod(list[1]), std::stod(list[2]), std::stod(list[3])));
			continue;
		}
		else if (list[0] == "f")
		{
			for (int i = 1; i <= 3; i++)
			{
				std::vector<std::string> vert = split(list[i], "/");

				glm::vec3 tmpCoords = coords[std::stol(vert[0]) - 1];
				vertBuffer.push_back(tmpCoords.x);
				vertBuffer.push_back(tmpCoords.y);
				vertBuffer.push_back(tmpCoords.z);

				glm::vec3 tmpNormals = normals[std::stol(vert[2]) - 1];
				vertBuffer.push_back(tmpNormals.x);
				vertBuffer.push_back(tmpNormals.y);
				vertBuffer.push_back(tmpNormals.z);

				glm::vec2 tmpTextures = textCoords[std::stol(vert[1]) - 1];
				vertBuffer.push_back(tmpTextures.x);
				vertBuffer.push_back(tmpTextures.y);

				indexBuffer.push_back(indexBuffer.size());
			}
			continue;
		}
		else if (list[0] == "usemtl")
		{

			if (pMesh)
			{
				Helper::CalcTBN(vertBuffer);
				std::vector<GLfloat> tmpVertBuff{ std::make_move_iterator(std::begin(vertBuffer)), std::make_move_iterator(std::end(vertBuffer))};
				std::vector<GLint> tmpIndexBuff{ std::make_move_iterator(std::begin(indexBuffer)), std::make_move_iterator(std::end(indexBuffer))};
				vertexes = std::make_shared<My_Ezz::VertexBuffer>(tmpVertBuff.data(), tmpVertBuff.size() * sizeof(GLfloat), bufferLayout_2vec3_vec2);
				indexes = std::make_shared<My_Ezz::IndexBuffer>(tmpIndexBuff.data(), tmpIndexBuff.size());
				pMesh->LoadMesh(vertexes, indexes);
				pMesh->SetMaterial(object->GetMtlLib()->GetMaterial(strMtlName));
			}
			strMtlName = list[1];
			object->AddMesh(pMesh);
			if (eType == EObjectType::kBase)
				pMesh = std::make_shared<My_Ezz::BaseMesh>();
			else if (eType == EObjectType::kLight)
				pMesh = std::make_shared<My_Ezz::LightMesh>();
			vertBuffer.clear();
			indexBuffer.clear();
		}
	}
	file.close();

	if (pMesh)
	{

		Helper::CalcTBN(vertBuffer);
		std::vector<GLfloat> tmpVertBuff{ std::make_move_iterator(std::begin(vertBuffer)), std::make_move_iterator(std::end(vertBuffer)) };
		std::vector<GLint> tmpIndexBuff{ std::make_move_iterator(std::begin(indexBuffer)), std::make_move_iterator(std::end(indexBuffer)) };
		vertexes = std::make_shared<My_Ezz::VertexBuffer>(tmpVertBuff.data(), tmpVertBuff.size() * sizeof(GLfloat), bufferLayout_2vec3_vec2);
		indexes = std::make_shared<My_Ezz::IndexBuffer>(tmpIndexBuff.data(), tmpIndexBuff.size());
		pMesh->LoadMesh(vertexes, indexes);
		pMesh->SetMaterial(object->GetMtlLib()->GetMaterial(strMtlName));
	}
	object->AddMesh(pMesh);
	m_objects.emplace(strObjectName, object);

	return object;
}

std::shared_ptr<My_Ezz::Object> ResourceManager::getObject(const std::string& strObjectName)
{
	ObjectsMap::const_iterator it = m_objects.find(strObjectName);
	if (it != m_objects.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

static unsigned long long getStreamSize(std::fstream& is)
{
	std::streampos savePos = is.tellg();
	is.seekg(0, std::ios::end);
	std::streampos endPos = is.tellg();
	is.seekg(savePos);
	return (std::streampos)(endPos - savePos);
}

std::shared_ptr<My_Ezz::Texture2D> ResourceManager::loadTexture(const std::string& strTextureName, const std::string& strTexturePath, My_Ezz::TextureType textureType)
{
	
	std::shared_ptr<My_Ezz::Texture2D> texure;
	int channels = 0;
	int width = 0;
	int height = 0;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load((path + "/" + strTexturePath).c_str(), &width, &height, &channels, 0);
	if (!pixels)
	{
		LOG_CRITICAL("Don't load texture {0}", (path + "/" + strTexturePath).c_str());
		return nullptr;
	}
	texure = std::make_shared<My_Ezz::Texture2D>(pixels, width, height, textureType);
	m_textures.emplace(strTextureName, texure);

	return texure;
}



std::shared_ptr<My_Ezz::Texture2D> ResourceManager::getTexture(const std::string& strTextureName)
{
	TexturesMap::const_iterator it = m_textures.find(strTextureName);
	if (it != m_textures.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

std::shared_ptr<My_Ezz::Material> ResourceManager::loadMaterial(const std::string& strMaterialPath, std::shared_ptr<My_Ezz::MaterialLibrary> pMaterialLib)
{
	std::ifstream file;
	file.open(path + "/" + strMaterialPath.c_str(), std::ios::in, std::ios::binary);
	if (!file.is_open())
	{
		return nullptr;
	}

	std::string line;
	std::shared_ptr<My_Ezz::Material> pNewMaterial;
	std::vector<std::string> localPath = split(strMaterialPath, "/");
	while (std::getline(file, line))
	{
		std::vector<std::string> list = split(line, " ");

		if (list[0] == "#")
		{
			continue;
		}
		else if (list[0] == "newmtl")
		{
			pNewMaterial = std::make_shared<My_Ezz::Material>();
			pNewMaterial->SetName(list[1]);
			pMaterialLib->AddMaterial(pNewMaterial);

		}
		else if (list[0] == "Ns")
		{
			pNewMaterial->SetShines(std::stod(list[1]));
		}
		else if (list[0] == "Ka")
		{
			pNewMaterial->SetAmbientColor(glm::vec3(std::stod(list[1]), std::stod(list[2]), std::stod(list[3])));
		}
		else if (list[0] == "Kd")
		{
			pNewMaterial->SetDiffuseColor(glm::vec3(std::stod(list[1]), std::stod(list[2]), std::stod(list[3])));
		}
		else if (list[0] == "Ks")
		{
			pNewMaterial->SetSpecularColor(glm::vec3(std::stod(list[1]), std::stod(list[2]), std::stod(list[3])));
		}
		else if (list[0] == "map_Kd")
		{
			std::shared_ptr<My_Ezz::Texture2D> pDiffMap = loadTexture(list[1], localPath[0] + "/" + localPath[1] + "/" + list[1].c_str());
			if (pDiffMap)
				pNewMaterial->SetDiffuseMap(pDiffMap);
		}
		else if (list[0] == "map_Bump")
		{
			std::shared_ptr<My_Ezz::Texture2D> pNormMap = loadTexture(list[1], localPath[0] + "/" + localPath[1] + "/" + list[1].c_str(), My_Ezz::TextureType::kNormal);
			if (pNormMap)
				pNewMaterial->SetNormalMap(pNormMap);
		}
	}
	file.close();
	pMaterialLib->AddMaterial(pNewMaterial);

	return pNewMaterial;
}

//bool ResourceManager::loadJSONResurces(const string& JSONPath)
//{
//	const string JSONString = getFileString(JSONPath);
//	if (JSONString.empty())
//	{
//		cerr << "No JSON file" << endl;
//		return false;
//	}
//	rapidjson::Document document;
//	rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());
//	if (!parseResult)
//	{
//		cerr << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << endl;
//		cerr << "In JSON flie: " << JSONPath << endl;
//		return false;
//	}
//
//	auto shadersIt = document.FindMember("shaders");
//	if (shadersIt != document.MemberEnd())
//	{
//		for (const auto& currentShader : shadersIt->value.GetArray())
//		{
//			const string name = currentShader["name"].GetString();
//			const string filePath_v = currentShader["filePath_v"].GetString();
//			const string filePath_f = currentShader["filePath_f"].GetString();
//			loadShaders(name, filePath_v, filePath_f);
//		}
//	}
//
//	auto textureAtlasesIt = document.FindMember("textureAtlases");
//	if (textureAtlasesIt != document.MemberEnd())
//	{
//		for (const auto& currentTextureAtlas : textureAtlasesIt->value.GetArray())
//		{
//			const string name = currentTextureAtlas["name"].GetString();
//			const string filePath = currentTextureAtlas["filePath"].GetString();
//			const unsigned int subTextureWidth = currentTextureAtlas["subTextureWidth"].GetInt();
//			const unsigned int subTextureHeight = currentTextureAtlas["subTextureHeight"].GetInt();
//			const auto subTexturesArray = currentTextureAtlas["subTextures"].GetArray();
//			vector<string> subTextures;
//			subTextures.reserve(subTexturesArray.Size());
//			for (const auto& currentSubTexture : subTexturesArray)
//			{
//				subTextures.emplace_back(currentSubTexture.GetString());
//			}
//			loadTexureAtlas(name, filePath, subTextures, move(subTextureWidth), move(subTextureHeight));
//		}
//	}
//
//	auto spritesIt = document.FindMember("sprites");
//	if (spritesIt != document.MemberEnd())
//	{
//		for (const auto& currentSprite : spritesIt->value.GetArray())
//		{
//			const string name = currentSprite["name"].GetString();
//			const string textureAtlas = currentSprite["textureAtlas"].GetString();
//			const string shader = currentSprite["shader"].GetString();
//			const string subTexture = currentSprite["initialSubTexture"].GetString();
//
//			auto sprite = loadSprite(name, textureAtlas, shader, subTexture);
//			if (!sprite)
//			{
//				continue;
//			}
//
//			auto framesIt = currentSprite.FindMember("frames");
//			if (framesIt != currentSprite.MemberEnd())
//			{
//				const auto framesArray = framesIt->value.GetArray();
//				vector<Render::Sprite::FrameDescription> framesDescriptions;
//				framesDescriptions.reserve(framesArray.Size());
//				for (const auto& currentFrame : framesArray)
//				{
//					const string subTexture = currentFrame["subTexture"].GetString();
//					const double duration = currentFrame["duration"].GetDouble();
//					const auto TextureAtlas = getTexture2D(textureAtlas);
//					const auto SubTexture = TextureAtlas->getSubTexture(subTexture);
//					framesDescriptions.emplace_back(SubTexture.leftBottomUV, SubTexture.rightTopUV, duration);
//				}
//				sprite->insertFrames(framesDescriptions);
//			}
//		}
//	}
//
//	auto startLevelIt = document.FindMember("start_screen");
//	if (startLevelIt != document.MemberEnd())
//	{
//		const auto descriptionArray = startLevelIt->value.GetArray();
//		m_startLevel.reserve(descriptionArray.Size());
//		size_t maxLength = 0;
//		for (const auto& currentRow : descriptionArray)
//		{
//			m_startLevel.emplace_back(currentRow.GetString());
//			if (maxLength < m_startLevel.back().length())
//			{
//				maxLength = m_startLevel.back().length();
//			}
//		}
//
//		for (auto& currentRow : m_startLevel)
//		{
//			while (currentRow.length() < maxLength)
//			{
//				currentRow.append("F");
//			}
//		}
//	}
//
//	auto levelsIt = document.FindMember("levels");
//	if (levelsIt != document.MemberEnd())
//	{
//		for (const auto& currentLevel : levelsIt->value.GetArray())
//		{
//			const auto description = currentLevel["description"].GetArray();
//			vector<string> levelRows;
//			levelRows.reserve(description.Size());
//			size_t max_lenght = 0;
//			for (const auto& currentRow : description)
//			{
//				levelRows.emplace_back(currentRow.GetString());
//				if (max_lenght < levelRows.back().length())
//				{
//					max_lenght = levelRows.back().length();
//				}
//			}
//			for (auto& currentRow : levelRows)
//			{
//				while (currentRow.length() < max_lenght)
//				{
//					currentRow.append("D");
//				}
//			}
//			m_levels.emplace_back(move(levelRows));
//		}
//	}
//
//	auto soundsIt = document.FindMember("sounds");
//	if (soundsIt != document.MemberEnd())
//	{
//		for (const auto& currentSound : soundsIt->value.GetArray())
//		{
//			const std::string soundName = currentSound["soundName"].GetString();
//			const std::string soundPath = currentSound["soundPath"].GetString();
//			loadSound(soundName, soundPath);
//		}
//	}
//
//	return true;
//}