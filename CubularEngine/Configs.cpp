#include "stdafx.h"
#include "Configs.h"

#define SCRIPT_CONFIG_NAME "assets/scripts/configs.lua"
#define SCRIPT_CONFIG_TABLE "EngineConfigs"
#define SCRIPT_CONFIG_WINDOW_WIDTH "windowWidth"
#define SCRIPT_CONFIG_WINDOW_HEIGTH "windowHeight"
#define SCRIPT_CONFIG_WINDOW_TITLE "windowTitle"

Configs* Configs::instance = nullptr;

Configs* Configs::GetInstance()
{
    if ( instance == nullptr )
        instance = new Configs();
    return instance;
}

void Configs::Release()
{
    delete( instance );
    instance = nullptr;
}

Configs::Configs() {
    sol::state lua;
    lua.open_libraries( sol::lib::base );
    lua.script_file( SCRIPT_CONFIG_NAME );

    sol::table configs = lua[ SCRIPT_CONFIG_TABLE ];
    windowWidth = configs.get<int>( SCRIPT_CONFIG_WINDOW_WIDTH );
    windowHeight = configs.get<int>( SCRIPT_CONFIG_WINDOW_HEIGTH );
    windowTitle = configs.get<const char*>( SCRIPT_CONFIG_WINDOW_TITLE );
    lua.end();
}
