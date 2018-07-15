#include "AssetManager.h"
#include "tinyobjloader/tiny_obj_loader.h"
#include <vector>
#include <string>

static std::string GetBaseDir(const std::string& filepath) {
	if (filepath.find_last_of("/\\") != std::string::npos)
		return filepath.substr(0, filepath.find_last_of("/\\"));
	return "";
}

AssetManager::AssetManager()
{
}

void AssetManager::LoadObject(std::string filename)
{	
	tinyobj::attrib_t att;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;	

	bool worked = tinyobj::LoadObj(&att, &shapes, &materials, &err, filename.c_str(), "res/objects/" );
}
