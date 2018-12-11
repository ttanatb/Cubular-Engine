#pragma once
#include "IEntity.h"


enum UIType
{
    UIButton,
    UIText
};

struct UIElement
{
    UIType uiType;
    std::string text;
    sol::function callBack;

    UIElement( UIType type, std::string uiText, sol::function bttnCallback ) {
        uiType = type;
        text = uiText;
        callBack = bttnCallback;
    }

    UIElement( UIType type, std::string uiText ) {
        uiType = type;
        text = uiText;
    }
};

class UIEntity : public IEntity
{
private:
    //2d transforms
    glm::vec2 pos;
    glm::vec2 size;

    //used for identifying
    std::string id;
    static int count;
    static int totalCount;

    //elements on the window
    std::vector<UIElement> uiElements;
    bool shouldResizeToFitContents;

public:
    //const & destr
    UIEntity( const char* scriptName, bool isFromLua );
    ~UIEntity();

    void Update();
    void Render( Camera* camera ) override;

    void SetPos( float x, float y );
    void SetSize( float x, float y );

    float GetPosX() { return pos.x; }
    float GetPosY() { return pos.y; }

    int AddButton( std::string text, std::string functionCallbackName );
    int AddText( std::string text );

    std::string GetUIElementText( int index ) { return uiElements[ index ].text; }
    void SetUIElementText( int index, std::string newText ) { uiElements[ index ].text = newText; }

    void SetShouldResizeToFitContents( bool newSetting ) { shouldResizeToFitContents = newSetting; }

    int GetCount() { return count; }
    void Destroy() override;

};
