#include "stdafx.h"
#include "Input.h"

//for singleton
Input* Input::instance = nullptr;

Input::Input()
{
    //empty
}

Input::~Input()
{
    //empty
}

Input* Input::GetInstance()
{
    //basic singleton stuff
    if ( instance == nullptr )
    {
        instance = new Input();
    }
    return instance;
}

void Input::Release()
{
    if ( instance != nullptr )
    {
        delete instance;
        instance = nullptr;
    }
}

void Input::Init( GLFWwindow * window )
{
    this->window = window;
}

bool Input::IsKeyDown( int key )
{
    //TODO - maybe we should assert if window is nullptr
    return glfwGetKey( window, key ) == GLFW_PRESS;
}
