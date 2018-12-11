#include "stdafx.h"
#include "SceneGraph.h"
#include "GameEntity.h"
#include "UIEntity.h"
#include <algorithm>
#include "Configs.h"

SceneGraph* SceneGraph::instance = nullptr;

void SceneGraph::AddRootEntity( IEntity * entity )
{
    //entity is created from C++, make sure to dealloc
    if ( !entity->GetIsFromLua() )
        toDealloc.push_back( entity );

    //exceeded entity count
    if ( count > Configs::GetInstance()->GetMaxEntityCount() )
        return;
    
    rootEntities.push_back( entity );
    ++count;
}

void SceneGraph::RemoveEntity( IEntity * entity )
{
    std::vector<IEntity*>::iterator it = std::find( rootEntities.begin(), rootEntities.end(), entity );
    rootEntities.erase( it );
    --count;
}

SceneGraph::SceneGraph()
{
    count = 0;
    rootEntities = std::vector<IEntity *>();
}

SceneGraph::~SceneGraph()
{
    for ( size_t i = 0; i < toDealloc.size(); ++i )
        delete toDealloc[ i ];
}

SceneGraph * SceneGraph::GetInstance()
{
    if ( instance == nullptr )
        instance = new SceneGraph();
    return instance;
}

void SceneGraph::ReleaseInstance()
{
    delete instance;
    instance = nullptr;
}

void SceneGraph::InitFromConfig()
{
    //load lua script
    sol::state lua;
    lua.open_libraries( sol::lib::base, sol::lib::table );
    lua.script_file(std::string(C_SCRIPT_DIR) + std::string(C_SCRIPT_SCENE_1) + std::string(C_SCRIPT_EXT));

    //loop through the scene file and create entities
    sol::table scene = lua[ C_SCRIPT_SCENE_TABLE ];
    for ( int i = 0; ; ++i )
    {
        sol::optional<std::string> scriptName = scene[ std::string( C_SCRIPT_SCENE_OBJ_PREFIX ) + std::to_string( i ) ];
        if ( !scriptName )  //no more elements
            break;

        std::string fileName = scriptName.value();
        if ( fileName.length() > 1 && fileName[ 0 ] == 'u' && fileName[ 1 ] == 'i' )
            new UIEntity( fileName.c_str(), false );
        else new GameEntity( fileName.c_str(), false );
    }
}

void SceneGraph::Update()
{
    for ( size_t i = 0; i < count; ++i )
        rootEntities[ i ]->Update();
}

void SceneGraph::Render( Camera * camera )
{
    for ( size_t i = 0; i < count; ++i )
        rootEntities[ i ]->Render( camera );
}
