#include "ResourcesManager.hpp"

namespace Resources
{
	ResourceManager::ResourceManager()
		: resources()
	{
	}

	void ResourceManager::Delete(const std::string p_name)
	{
		resources.erase(p_name);
		Core::Debug::Log::Print("Delete element " + p_name + " from resources\n", Core::Debug::LogLevel::Notification);
	}
}