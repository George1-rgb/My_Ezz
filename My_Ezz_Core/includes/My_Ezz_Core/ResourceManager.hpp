#include <string>
#include <memory>
#include <map>
#include <vector>

namespace My_Ezz
{
	class AudioBase;
	class ShaderProgram;
}

class ResourceManager
{
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

private:
	static std::string getFileString(const std::string& ralativeFilePath);

	typedef std::map<const std::string, std::shared_ptr<My_Ezz::AudioBase>> SoundsMap;
	static SoundsMap  m_sounds;

	typedef std::map<const std::string, std::shared_ptr<My_Ezz::ShaderProgram>> ShaderProgramsMap;
	static ShaderProgramsMap  m_shadersPrograms;

	static std::string path;
};