#include "stdafx.h"
#include "UIEntity.h"
#include "SceneGraph.h"
#include "GameEntity.h"

int UIEntity::count = 0;
int UIEntity::totalCount = 0;

UIEntity::UIEntity(
    const char* scriptName,
    bool isFromLua )
    :IEntity( scriptName, isFromLua )
{
    //lua binding
    script.new_usertype<UIEntity>( scriptName,
        sol::constructors<UIEntity( const char*, bool )>(),
        "getParent", &UIEntity::GetParent,
        "destroy", &UIEntity::Destroy,
        "addChild", &UIEntity::AddChild,
        "getChildren", &UIEntity::GetChildren,
        "getName", &UIEntity::GetName,
        "setName", &UIEntity::SetName,
        "setSize", &UIEntity::SetSize,
        "setPos", &UIEntity::SetPos,
        "getPosX", &UIEntity::GetPosX,
        "getPosY", &UIEntity::GetPosY,
        "addButton", &UIEntity::AddButton,
        "addText", &UIEntity::AddText,
        "getUIElementText", &UIEntity::GetUIElementText,
        "setUIElementText", &UIEntity::SetUIElementText,
        "getCount", &UIEntity::GetCount,
        "setShouldResizeToFitContents", &UIEntity::SetShouldResizeToFitContents
        );

    script.new_usertype<GameEntity>( "horizontalBox", sol::constructors<GameEntity( const char * scriptName, bool )>() );
    script.new_usertype<GameEntity>( "verticalBox", sol::constructors<GameEntity( const char * scriptName, bool )>() );

    script[ "entity" ] = this;

    id = std::to_string( totalCount );
    ++count;
    ++totalCount;

    startFunc();
    SceneGraph::GetInstance()->AddRootEntity( this );

}

UIEntity::~UIEntity()
{
}

void UIEntity::Update()
{
    const ImVec2 sizeIm = { size.x, size.y };
    bool open = true;
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar;
    if ( shouldResizeToFitContents )
        flags |= ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::Begin( ( name + " [" + id + "]" ).c_str(), &open, sizeIm, 0.8f, flags );
    if ( count == 1 )
        ImGui::SetWindowPos( { 20.0f, 660.0f } );

    for ( size_t i = 0; i < uiElements.size(); ++i )
    {
        UIElement uiElement = uiElements[ i ];
        switch ( uiElement.uiType )
        {
            case UIType::UIButton:
                if ( ImGui::Button( uiElement.text.c_str() ) )
                    uiElement.callBack();
                break;
            case UIType::UIText:
                ImGui::Text( uiElement.text.c_str() );
                break;
        }

        if ( i < uiElements.size() - 2 )
        {
            if ( uiElement.uiType == UIType::UIButton && uiElements[ i + 1 ].uiType == UIType::UIButton )
                ImGui::SameLine();
            else ImGui::NewLine();
        }
    }


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

int UIEntity::AddButton( std::string text, std::string functionCallbackName )
{
    UIElement e = UIElement( UIType::UIButton, text, script[ functionCallbackName ] );
    uiElements.push_back( e );
    return uiElements.size() - 1;
}

int UIEntity::AddText( std::string text )
{
    UIElement e = UIElement( UIType::UIText, text );
    uiElements.push_back( e );
    return uiElements.size() - 1;
}

void UIEntity::Destroy()
{
    --count;
    IEntity::Destroy();
}
