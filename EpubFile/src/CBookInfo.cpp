#include "CBookInfo.h"
#include <iostream>

CBookInfo::CBookInfo()
{
    _reader = new XmlUtils::CXmlLiteReader();
}

CBookInfo::CBookInfo(const XmlUtils::CXmlLiteReader &reader)
{
    _reader = new XmlUtils::CXmlLiteReader();
    *_reader = reader;
}
CBookInfo::~CBookInfo()
{
    delete _reader;
}

bool CBookInfo::ReadInfo()
{
    if (_reader == NULL || _reader->IsEmptyNode())
        return false;

    ReadNodes();

    return true;
}

void CBookInfo::ReadNodes()
{
    if (_reader->IsEmptyNode())
        return;

    int nParentDepth = _reader->GetDepth();
    while(_reader->ReadNextSiblingNode(nParentDepth))
    {
        std::wstring sName = _reader->GetName();

        if (_reader->GetNamespacePrefix() == L"dc")
            _bookInfo[sName.substr(3, sName.length())] = _reader->GetText2();

        if (sName == L"metadata")
            ReadNodes();
    }
}

std::map <std::wstring, std::wstring> CBookInfo::GetBookInfo()
{
    return _bookInfo;
}
