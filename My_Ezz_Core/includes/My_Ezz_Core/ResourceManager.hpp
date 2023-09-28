#include <string>
#include <memory>
#include <map>
#include <vector>

namespace My_Ezz
{
	class AudioBase;
	class ShaderProgram;
	class Object;
	class Texture2D;
}

class ResourceManager
{
public:
	enum class EObjectType
	{
		kBase,
		kLight,
		kNONE = -1
	};
public:
	static void setExecutablePath(const std::string& executablePath);
	static void unloadAllResources();

	~ResourceManager() = delete;
	ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	static void UnloadAllResources();
	
	//static bool loadJSONResurces(const std::string& JSONPath);
	static std::shared_ptr<My_Ezz::ShaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	static std::shared_ptr<My_Ezz::ShaderProgram> getShaderProgram(const std::string& shaderName);

	static std::shared_ptr<My_Ezz::AudioBase> loadSound(const std::string& soundName, const std::string& soundPath);
	static std::shared_ptr<My_Ezz::AudioBase> getSound(const std::string& soundName);

	static std::shared_ptr<My_Ezz::Object> loadObject(const std::string& strObjectName, const std::string& strObjectPath, EObjectType eType = EObjectType::kBase);
	static std::shared_ptr<My_Ezz::Object> getObject(const std::string& strObjectName);

	static std::shared_ptr<My_Ezz::Texture2D> loadTexture(const std::string& strTextureName, const std::string& strTexturePath);
	static std::shared_ptr<My_Ezz::Texture2D> getTexture(const std::string& strTextureName);

private:
	static std::string getFileString(const std::string& ralativeFilePath);

	static std::vector<std::string> split(const std::string& strInput, const std::string& strRegex);

	typedef std::map<const std::string, std::shared_ptr<My_Ezz::AudioBase>> SoundsMap;
	static SoundsMap  m_sounds;

	typedef std::map<const std::string, std::shared_ptr<My_Ezz::ShaderProgram>> ShaderProgramsMap;
	static ShaderProgramsMap  m_shadersPrograms;

	typedef std::map<const std::string, std::shared_ptr<My_Ezz::Object>> ObjectsMap;
	static ObjectsMap m_objects;

	typedef std::map<const std::string, std::shared_ptr<My_Ezz::Texture2D>> TexturesMap;
	static TexturesMap m_textures;

	static std::string path;
};