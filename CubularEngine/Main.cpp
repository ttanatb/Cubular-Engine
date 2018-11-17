#include "stdafx.h"
#include "Engine.h"

int main()
{
#if defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#endif
    {
        Engine engine = Engine();
        if ( engine.Init() == 0 )
        {
            engine.Run();
        }
    }
#if defined(_DEBUG)
    _CrtDumpMemoryLeaks();
#endif
    return 0;
}