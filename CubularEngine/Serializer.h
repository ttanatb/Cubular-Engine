#pragma once

namespace Networking
{
    class Serializer
    {
    public:
        static void Write( char * buffer, const void * inData, size_t inByteCount ) {
            memcpy( reinterpret_cast<void *>(buffer), inData, inByteCount );
        }

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

