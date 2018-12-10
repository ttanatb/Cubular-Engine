#include "stdafx.h"
#include "Camera.h"
#include "IEntity.h"
#include "Material.h"
#include "Mesh.h"
#include <algorithm>

IEntity::IEntity( const char* scriptName )
{
    children = std::vector<IEntity*>();

    script.open_libraries( sol::lib::base );
    script.script_file( std::string( C_SCRIPT_DIR ) + std::string( scriptName ) );

    startFunc = script[ "start" ];
    updateFunc = script[ "update" ];
}

IEntity::~IEntity() 
{
    if ( children.empty() )
        return;

    size_t size = children.size();
    for ( size_t i = 0; i < size; ++i )
    {
        delete children[ i ];
    }
}

void IEntity::Update()
{
    updateFunc();
    if ( children.empty() )
        return;

    size_t size = children.size();
    for (size_t i = 0; i < size; ++i )
    {
        children[ i ]->Update();
    }
}

void IEntity::Render( Camera* camera )
{
    if ( children.empty() )
        return;

    size_t size = children.size();
    for ( size_t i = 0; i < size; ++i )
    {
        children[ i ]->Render(camera);
    }
}

void IEntity::AddChild( IEntity * newChild )
{
    //already child of this entity
    if ( std::find( children.begin(), children.end(), newChild ) != children.end() )
    {
        DEBUG_PRINT( "Entity (%s) already contains child (%s)", name.c_str(), newChild->name.c_str() );
        return;
    }

    newChild->AttachToParent( this );
}

void IEntity::AttachToParent( IEntity * newParent )
{
    //remove itself from the previous parent's list
    if ( parent )
        parent->children.erase( std::find( parent->children.begin(), parent->children.end(), this ) );

    //assign new parent and add itself as child
    parent = newParent;
    parent->children.push_back( this );
}
