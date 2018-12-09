#pragma once
#include <unordered_map>

class Material;
class Mesh;

class ResourceManager
{
public:
    static ResourceManager* GetInstance();
    static void ReleaseInstance();

    void AddMaterial( std::string name, Material *m ) { materials.insert( { name, m } ); }
    void AddMesh( std::string name, Mesh *m ) { meshes.insert( { name, m } ); }

    Material* GetMaterial( std::string name ) { return materials[ name ]; }
    Mesh* GetMesh( std::string name ) { return meshes[ name ]; }

private:
    static ResourceManager* instance;
    ResourceManager();
    ~ResourceManager();

    std::unordered_map<std::string, Material*> materials;
    std::unordered_map<std::string, Mesh*> meshes;
};

