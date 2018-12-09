#include "stdafx.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager * ResourceManager::GetInstance()
{
    if ( instance == nullptr )
        instance = new ResourceManager();
    return instance;
}

void ResourceManager::ReleaseInstance()
{
    delete instance;
    instance = nullptr;
}

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
    for ( auto& mesh : meshes )
        delete mesh.second;

    for ( auto& mat : materials )
        delete mat.second;
}
