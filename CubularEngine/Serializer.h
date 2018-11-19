#pragma once
#include "stdafx.h"

namespace Networking
{
    class Serializer
    {
    public:
        template<typename T>
        static T& Serialize( char * ioData ) {
            return reinterpret_cast<T>( ioData );
        }

        template<typename T>
        static char* Deserialize( T ioData ) {
            return reinterpret_cast<char *>( ioData );
        }
    };
}

