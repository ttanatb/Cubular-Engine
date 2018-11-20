#include "stdafx.h"
#include "Camera.h"

Camera::Camera( float screenWidth, float screenHeight )
{
    position = glm::vec3( 0.f , 0.f, 50.f);
    forward = glm::vec3( 0.f, 0.f, -1.f );
    up = glm::vec3( 0.f, 1.f, 0.f );

    fov = 60.f * glm::pi<float>() / 180.0f;
    width = screenWidth;
    height = screenHeight;
    nearZ = 0.01f;
    farZ = 100.f;
}

Camera::~Camera()
{
}

void Camera::Update()
{
    //view matrix
    //we use forward here instead of a lookTarget because it's easier to track
    //and conceptualize.
    viewMatrix = glm::lookAtLH(
        position,               //where the camera is
        position + forward,     //where the camera is looking at 
        up                      //what is 'up' for the camera
    );

    //create the projection matrix
    projectionMatrix = glm::perspectiveFovLH<GLfloat>(
        fov,
        width,                  //the width of the window in float
        height,                 //the height of the window in float
        nearZ,                  //the near Z-plane
        farZ                    //the far Z-plane
    );
}
