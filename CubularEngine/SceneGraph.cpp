#include "stdafx.h"
#include "SceneGraph.h"
#include "GameEntity.h"
#include "UIEntity.h"
#include "ResourceManager.h"

SceneGraph::SceneGraph()
{
    ResourceManager* r = ResourceManager::GetInstance();
    root = new GameEntity(
        r->GetMesh( std::string( C_MESH_TANK ) ),
        r->GetMaterial( std::string( C_MAT_TANK ) )
    );

    root->AddChild( new UIEntity( glm::vec2( 50.f, 100.f ), glm::vec2( 200.f, 200.f ) ) );
    root->GetChildren()[ 0 ]->SetName( "Test UI" );
}

SceneGraph::~SceneGraph()
{
    delete root;
}

void SceneGraph::InitFromConfig()
{
}

void SceneGraph::Update()
{
    root->Update();
}

void SceneGraph::Render( Camera * camera )
{
    root->Render( camera );
}
