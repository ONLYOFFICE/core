#pragma once


#include <sstream>
#include <fstream>
#include <memory>
#include "../../DesktopEditor/common/Types.h"

namespace CFCPP
{

class IStream
{
public:
    virtual std::streamsize tell() = 0;
    virtual std::streamsize seek(std::streamsize offset, std::ios_base::seekdir mode = std::ios::beg) = 0;
    virtual std::streamsize read(char* buffer, std::streamsize len) = 0;
    virtual void write (const char* buffer, std::streamsize len) = 0;
    virtual void flush() = 0;
    virtual void close() = 0;
};

using Stream = std::shared_ptr<IStream>;

class FStreamWrapper : public IStream, public std::fstream
{
public:
    FStreamWrapper(std::string filename, std::ios_base::openmode openmode) :
        std::fstream(filename, openmode) {}

    inline std::streamsize tell() override { return std::fstream::tellg(); }
    inline std::streamsize seek(std::streamsize offset, std::ios_base::seekdir mode = std::ios::beg) override
            { std::fstream::seekp(offset, mode); std::fstream::seekg(offset, mode); return tell();}
    inline std::streamsize read(char* buffer, std::streamsize len) override { std::fstream::read(buffer, len); return tell(); }
    inline void write (const char* buffer, std::streamsize len) override { std::fstream::write(buffer, len); }
    inline void flush() override { std::fstream::flush(); }
    inline void close() override { std::fstream::close(); }
};

std::string CorrectUnixPath(const std::string original);

Stream OpenFileStream(std::wstring filename, bool bRewrite = false, bool trunc = false);
Stream OpenFileStream(std::string filename, bool bRewrite = false, bool trunc = false);

bool IsOpen(const Stream& st);
std::streamsize Length(const Stream& st);
int FileLenght(std::wstring filename);

ULONG64 FileSimpleHash(std::wstring filename, int len = -1, int offset = 0);

}
