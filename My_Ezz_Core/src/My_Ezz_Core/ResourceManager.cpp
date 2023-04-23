#include "My_Ezz_Core/ResourceManager.hpp"
#include "My_Ezz_Multimedia/AudioBase.hpp"
#include "My_Ezz_Logger/Log.hpp"
//#include <rapidjson/document.h>
//#include <rapidjson/error/en.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
//#define STB_IMAGE_IMPLEMENTATION
//#define STBI_ONLY_PNG
//#include "stb_image.h"


std::string ResourceManager::path;
ResourceManager::SoundsMap  ResourceManager::m_sounds;

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



std::shared_ptr<My_Ezz::AudioBase> ResourceManager::loadSound(const std::string& soundName, const std::string& soundPath)
{
	std::uint8_t channels;
	std::int32_t sampleRate;
	std::uint8_t bitsPerSample;
	char* soundData;
	std::int32_t size;
	soundData = load_wav(path + "/" + soundPath, channels, sampleRate, bitsPerSample, size);
	std::shared_ptr<My_Ezz::AudioBase> newSound = m_sounds.emplace(soundName,
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