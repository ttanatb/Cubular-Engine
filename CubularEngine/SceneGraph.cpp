#include "stdafx.h"
#include "SceneGraph.h"
#include "GameEntity.h"
#include "UIEntity.h"
#include <algorithm>

SceneGraph* SceneGraph::instance = nullptr;

void SceneGraph::AddRootEntity( IEntity * entity )
{
    //entity is created from C++, make sure to dealloc
    if ( !entity->GetIsFromLua() )
        toDealloc.push_back( entity );

    //exceeded entity count
    if ( count > C_ENTITY_MAX_COUNT )
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
    //C++ side of entity creation
    new GameEntity( "verticalBox", false );
    new GameEntity( "horizontalBox", false );
    new UIEntity( "speechBox", false );
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
