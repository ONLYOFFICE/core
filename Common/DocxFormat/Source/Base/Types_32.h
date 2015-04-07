#pragma once

#if defined(_WIN32) || defined (_WIN64)
        typedef __int16			_INT16;
        typedef __int32			_INT32;
        typedef __int64			_INT64;
	typedef unsigned __int16	_UINT16;
	typedef unsigned __int32	_UINT32;
	typedef unsigned __int64	_UINT64;
#elif __linux__
        typedef int16_t             _INT16;
        typedef int32_t             _INT32;
        typedef int64_t             _INT64;
        typedef uint16_t            _UINT16;
        typedef uint32_t            _UINT32;
        typedef uint64_t            _UINT64;
#elif __APPLE__
        typedef int16_t             _INT16;
        typedef int32_t             _INT32;
        typedef int64_t             _INT64;
        typedef uint16_t            _UINT16;
        typedef uint32_t            _UINT32;
        typedef uint64_t            _UINT64;
#endif
