#include "stdafx.h"
#include "SceneGraph.h"
#include "GameEntity.h"
#include "UIEntity.h"
#include "ResourceManager.h"

SceneGraph::SceneGraph()
{
    ResourceManager* r = ResourceManager::GetInstance();
    rootEntities.push_back( new GameEntity( "tank.lua" ) );
    rootEntities.push_back( new UIEntity( "speechBox.lua" ) );
}

SceneGraph::~SceneGraph()
{
    for ( size_t i = 0; i < rootEntities.size(); ++i )
        delete rootEntities[ i ];
}

void SceneGraph::InitFromConfig()
{
}

void SceneGraph::Update()
{
    for ( size_t i = 0; i < rootEntities.size(); ++i )
        rootEntities[ i ]->Update();
}

void SceneGraph::Render( Camera * camera )
{
    for ( size_t i = 0; i < rootEntities.size(); ++i )
        rootEntities[ i ]->Render( camera );
}
