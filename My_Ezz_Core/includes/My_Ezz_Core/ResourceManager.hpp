#include <string>
#include <memory>
#include <map>
#include <vector>

namespace My_Ezz
{
	class AudioBase;
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

	
	//static bool loadJSONResurces(const std::string& JSONPath);


	static std::shared_ptr<My_Ezz::AudioBase> loadSound(const std::string& soundName, const std::string& soundPath);
	static std::shared_ptr<My_Ezz::AudioBase> getSound(const std::string& soundName);

private:
	static std::string getFileString(const std::string& ralativeFilePath);

	typedef std::map<const std::string, std::shared_ptr<My_Ezz::AudioBase>> SoundsMap;
	static SoundsMap  m_sounds;

	static std::string path;
};