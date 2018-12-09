#include "stdafx.h"
#include "UIEntity.h"

UIEntity::UIEntity( 
    glm::vec2 startingPos, 
    glm::vec2 startingSize, 
    const char* startScriptName,
    const char* updateScriptName )
    :IEntity(startScriptName, updateScriptName)
{
    pos = startingPos;
    size = startingSize;

}

UIEntity::~UIEntity()
{
}

void UIEntity::Update()
{
    ImGui::Begin( name );
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
