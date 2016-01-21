#ifndef STRINGS_HACK_PRINTF_H
#define STRINGS_HACK_PRINTF_H

#include <string>
#include <string.h>
#include <wchar.h>
#include <stdarg.h>

#ifndef STD_BUF_SIZE
#define STD_BUF_SIZE 1024
#endif

static int strings_hack_printf_internal(wchar_t* _buffer, size_t _size_alloc, wchar_t* _format, va_list va)
{
    size_t write_size = 0;

    wchar_t* tmp_format = _format;
    wchar_t* tmp_buffer = _buffer;
    do
    {
        wchar_t* buffer_cur = tmp_format;
        int nSkip = 0;
        while (*buffer_cur)
        {
            if (buffer_cur[0] == '%')
            {
                if (buffer_cur[1] == 's') // not crash (last symbol - 0)
                {
                    nSkip = 2;
                    break;
                }
                else if (buffer_cur[1] == 'l' && buffer_cur[2] == 's') // not crash (last symbol - 0)
                {
                    nSkip = 3;
                    break;
                }
            }
            ++buffer_cur;
        }

        if (nSkip > 0)
        {
            buffer_cur[0] = '\0';
        }

        if (write_size >= _size_alloc)
            return -1;

        if (*tmp_format)
        {
            int fmt_size = vswprintf(tmp_buffer, _size_alloc - write_size, tmp_format, va);
            if (fmt_size < 0)
                return -1;

            tmp_buffer += fmt_size;
            write_size += fmt_size;
        }

        if (nSkip)
        {
            wchar_t* _string_param = va_arg(va, wchar_t*);
            size_t _len = wcslen(_string_param);

            if (_size_alloc > (_len + write_size))
                memcpy(tmp_buffer, _string_param, _len * sizeof(wchar_t));

            write_size += _len;
            tmp_buffer += _len;
        }

        tmp_format = buffer_cur + nSkip;
    } while (*tmp_format);

    if (write_size >= _size_alloc)
        return -1;

    tmp_buffer[0] = '\0';
    return (int)write_size;
}

static int strings_hack_printf_internal_a(char* _buffer, size_t _size_alloc, char* _format, va_list va)
{
    size_t write_size = 0;

    char* tmp_format = _format;
    char* tmp_buffer = _buffer;
    do
    {
        char* buffer_cur = tmp_format;
        int nSkip = 0;
        while (*buffer_cur)
        {
            if (buffer_cur[0] == '%')
            {
                if (buffer_cur[1] == 's') // not crash (last symbol - 0)
                {
                    nSkip = 2;
                    break;
                }
                else if (buffer_cur[1] == 'l' && buffer_cur[2] == 's') // not crash (last symbol - 0)
                {
                    nSkip = 3;
                    break;
                }
            }
            ++buffer_cur;
        }

        if (nSkip > 0)
        {
            buffer_cur[0] = '\0';
        }

        if (write_size >= _size_alloc)
            return -1;

        if (*tmp_format)
        {
            int fmt_size = vsnprintf(tmp_buffer, _size_alloc - write_size, tmp_format, va);
            if (fmt_size < 0)
                return -1;

            tmp_buffer += fmt_size;
            write_size += fmt_size;
        }

        if (nSkip)
        {
            char* _string_param = va_arg(va, char*);
            size_t _len = strlen(_string_param);

            if (_size_alloc > (_len + write_size))
                memcpy(tmp_buffer, _string_param, _len * sizeof(char));

            write_size += _len;
            tmp_buffer += _len;
        }

        tmp_format = buffer_cur + nSkip;
    } while (*tmp_format);

    if (write_size >= _size_alloc)
        return -1;

    tmp_buffer[0] = '\0';
    return (int)write_size;
}

template <typename T>
static int strings_hack_printf(T& str, const wchar_t *format, va_list argptr)
{
    int buf_size = STD_BUF_SIZE;

    size_t nFormatLen = wcslen(format);
    if (0 == nFormatLen)
        return -1;

    wchar_t* tmp_format = new wchar_t[nFormatLen + 1];
    memcpy(tmp_format, format, nFormatLen * sizeof(wchar_t));
    tmp_format[nFormatLen] = '\0';

    while (buf_size < STD_BUF_SIZE * STD_BUF_SIZE)
    {
        va_list args;
        va_copy(args, argptr);
        wchar_t buffer[buf_size];

        int fmt_size = strings_hack_printf_internal(buffer, sizeof(buffer)/sizeof(buffer[0]), tmp_format, args);
        if (fmt_size >= 0)
        {
            str = T(buffer);
            return fmt_size;
        }

        buf_size *= 2;
    }

    delete [] tmp_format;

    return -1;
}
template <typename T>
static int strings_hack_printf(T& str, const char* format, va_list argptr)
{
    int buf_size = STD_BUF_SIZE;

    size_t nFormatLen = strlen(format);
    if (0 == nFormatLen)
        return -1;

    char* tmp_format = new char[nFormatLen + 1];
    memcpy(tmp_format, format, nFormatLen * sizeof(char));
    tmp_format[nFormatLen] = '\0';

    while (buf_size < STD_BUF_SIZE * STD_BUF_SIZE)
    {
        va_list args;
        va_copy(args, argptr);
        char buffer[buf_size];

        int fmt_size = strings_hack_printf_internal_a(buffer, sizeof(buffer)/sizeof(buffer[0]), tmp_format, args);
        if (fmt_size >= 0)
        {
            str = T(buffer);
            return fmt_size;
        }

        buf_size *= 2;
    }

    delete [] tmp_format;

    return -1;
}

template<typename T, typename CHAR_TYPE>
static void strings_hack_printf_exec(T& sRes, const CHAR_TYPE* szFmt, ...)
{
    va_list argList;

    va_start(argList, szFmt);
    strings_hack_printf(sRes, szFmt, argList);
    va_end(argList);
}

#endif // STRINGS_HACK_PRINTF_H
