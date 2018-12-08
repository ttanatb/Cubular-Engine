#include "stdafx.h"
#include "Engine.h"

//program entry
int main()
{
    //configure the program to print out memory leaks
#if defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#endif

    //added stack to make sure everything stack allocated is gone
    {
        Engine engine = Engine();
        if ( engine.Init() != 0 )
        {
            DEBUG_PRINT( "Engine Initialization failed" );
        }
        else
        {
            engine.Run();
        }
    }

    return 0;
}