#pragma once
#include "IEntity.h"

class UIEntity : public IEntity 
{
public:
    UIEntity(const char* scriptName);
    ~UIEntity();

    void Update();
    void Render( Camera* camera ) override;

    void SetPos( float x, float y );
    void SetSize( float x, float y );

private:
    glm::vec2 pos;
    glm::vec2 size;
};
