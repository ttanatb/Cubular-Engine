#include "stdafx.h"
#include "UIEntity.h"

UIEntity::UIEntity(
    const char* scriptName )
    :IEntity( scriptName )
{
    script.new_usertype<UIEntity>( "test_entity",
        "getParent", &UIEntity::GetParent,
        "getChildren", &UIEntity::GetChildren,
        "getName", &UIEntity::GetName,
        "setName", &UIEntity::SetName,
        "setSize", &UIEntity::SetSize,
        "setPos", &UIEntity::SetPos
    );

    script[ "entity" ] = this;

    startFunc();
}

UIEntity::~UIEntity()
{
}

void UIEntity::Update()
{
    ImGui::Begin( name.c_str() );
    ImGui::SetWindowPos( { pos.x, pos.y } );
    ImGui::SetWindowSize( { size.x, size.y } );
    ImGui::Text( "Test" );
    ImGui::End();
    IEntity::Update();
}

void UIEntity::Render( Camera * camera )
{
    IEntity::Render( camera );
}

void UIEntity::SetPos( float x, float y )
{
    pos = glm::vec2( x, y );
}

void UIEntity::SetSize( float x, float y )
{
    size = glm::vec2( x, y );
}
