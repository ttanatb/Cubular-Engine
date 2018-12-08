#pragma once

class Configs
{
public:
    const int GetWindowWidth()   { return windowWidth; } 
    const int GetWindowHeight()  { return windowHeight; } 
    const char* GetWindowTitle() { return windowTitle; } 
    
    static Configs* GetInstance();
    static void Release();

private:
    Configs();

    static Configs* instance;

    int windowWidth;
    int windowHeight;
    const char* windowTitle;
};
