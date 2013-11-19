#pragma once

#ifdef __cplusplus

#ifdef ENABLE_LOG

#include "Log.h"

#define LOG_ERROR( m ) Log::error( m )
#define LOG_WARNING( m ) Log::warning( m )
#define LOG_MESSAGE( m ) Log::message( m )
#define LOG_EVENT( m ) Log::event( m )

#else

#define LOG_ERROR( m )
#define LOG_WARNING( m )
#define LOG_MESSAGE( m )
#define LOG_EVENT( m )

#endif

extern "C" {

#else

#include "stdio.h"

#define LOG_ERROR( m ) { \
        FILE * __log_file = fopen( "converter.log", "a+" ); \
        if ( __log_file ) \
        {   \
            fprintf( __log_file, "Error: %s", m ); \
            fclose( __log_file ); \
            __log_file = NULL; \
        } \
    }

#define LOG_WARNING( m ) { \
        FILE * __log_file = fopen( "converter.log", "a+" ); \
        if ( __log_file ) \
        {   \
            fprintf( __log_file, "Warning: %s", m ); \
            fclose( __log_file ); \
            __log_file = NULL; \
        } \
    }

#define LOG_EVENT( m ) { \
        FILE * __log_file = fopen( "converter.log", "a+" ); \
        if ( __log_file ) \
        {   \
            fprintf( __log_file, "Event: %s", m ); \
            fclose( __log_file ); \
            __log_file = NULL; \
        } \
    }

#define LOG_MESSAGE( m ) { \
        FILE * __log_file = fopen( "converter.log", "a+" ); \
        if ( __log_file ) \
        {   \
            fprintf( __log_file, "Message: %s", m ); \
            fclose( __log_file ); \
            __log_file = NULL; \
        } \
    }

#endif

void str2wstr( wchar_t * dest, const char * src );
void wstr2str( char * dest, const wchar_t * src );


#ifdef __cplusplus
}
#endif
