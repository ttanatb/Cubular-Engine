#pragma once
#include "IEntity.h"

class UIEntity : public IEntity 
{
public:
    UIEntity(
        glm::vec2 startingPos,
        glm::vec2 startingSize
    );
    ~UIEntity();

    void Update();
    void Render( Camera* camera ) override;

private:
    glm::vec2 pos;
    glm::vec2 size;
};