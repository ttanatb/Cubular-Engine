#include "stdafx.h"
#include "Camera.h"
#include "IEntity.h"
#include "Material.h"
#include "Mesh.h"
#include <algorithm>

IEntity::IEntity() 
{
    children = std::vector<IEntity*>();
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
        DEBUG_PRINT( "Entity (%s) already contains child (%s)", name, newChild->name );
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

void IEntity::SetName( const char * newName )
{
    assert( strlen( newName ) < NAME_LENGTH );

    //copy over the name
    memcpy( name, newName, ( NAME_LENGTH - static_cast<size_t>( 1 ) ) * sizeof( char ) );
}
