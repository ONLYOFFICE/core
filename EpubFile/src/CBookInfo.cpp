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

        if (sName == L"metadata")
            ReadMetadata();
        else if (sName == L"manifest")
            ReadManifest();
        else
            ReadNodes();
    }
}

void CBookInfo::ReadMetadata()
{
    int nParentDepth = _reader->GetDepth();
    while(_reader->ReadNextSiblingNode(nParentDepth))
    {
        std::wstring sName = _reader->GetName();
        if (_reader->GetNamespacePrefix() == L"dc")
            _bookInfo[sName.substr(3, sName.length())] = _reader->GetText2();
    }
}

void CBookInfo::ReadManifest()
{
    int nParentDepth = _reader->GetDepth();
    while(_reader->ReadNextSiblingNode(nParentDepth))
    {
        if (_reader->GetAttributesCount() > 0 &&
            _reader->MoveToFirstAttribute())
        {
            std::wstring sAttributeName = _reader->GetName();
            while (!sAttributeName.empty())
            {
                std::wstring sAttributeValue = _reader->GetText();
                if (sAttributeName == L"href")
                {

                    if ((sAttributeValue.find(L"/") != std::wstring::npos))
                        sAttributeValue = sAttributeValue.substr(sAttributeValue.find_last_of(L"/") + 1, sAttributeValue.length());
                    if (sAttributeValue.substr(sAttributeValue.find_last_of(L".") + 1, sAttributeValue.length()) == L"xhtml")
                        _hrefs.push_back(sAttributeValue);
                }
                if (!_reader->MoveToNextAttribute())
                    break;
                sAttributeName = _reader->GetName();
            }
        }
    }
}

std::map <std::wstring, std::wstring> CBookInfo::GetBookInfo()
{
    return _bookInfo;
}

void CBookInfo::ShowInfo()
{
    std::wcout << "-----DATA-----" << std::endl;
    for (auto item = _bookInfo.begin(); item != _bookInfo.end(); item++)
        std::wcout << item->first << " - " << item->second << std::endl;
    std::wcout << "-----HREFS-----" << std::endl;
    for (int i = 0; i < (int)_hrefs.size(); i++)
        std::wcout << _hrefs[i] << std::endl;
}
