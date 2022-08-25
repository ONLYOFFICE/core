#pragma once

#include "stream.h"
#include "cfitem.h"
#include <vector>

namespace CFCPP
{
class CFStream : public CFItem
{
public:
    CFStream(CompoundFile* compFile, std::weak_ptr<IDirectoryEntry> dirEntry);

    void SetData(const std::vector<BYTE>& data);
    void Write(const std::vector<BYTE>& data, std::streamsize position);
    void Write(const std::vector<BYTE>& data, std::streamsize position, int offset, int count);
    void Append(const std::vector<BYTE>& data);
    std::vector<BYTE> getData() const;
    int Read(std::vector<BYTE>& buffer, std::streamsize position, int count);
    int Read(std::vector<BYTE>& buffer, std::streamsize position, int offset, int count);
    void CopyFrom(const Stream& input);
    void Resize(std::streamsize length);
    std::streamsize size() const {return CFItem::size();}
};
}
