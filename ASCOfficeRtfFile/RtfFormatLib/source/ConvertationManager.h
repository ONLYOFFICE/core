#pragma once

#include <string>

class OOXWriter;
class OOXReader;
class RtfWriter;
class RtfReader;

class RtfConvertationManager
{
public:
    std::wstring m_sTempFolder;

    RtfConvertationManager( )
    {
        m_poOOXWriter = NULL;
        m_poOOXReader = NULL;

        m_poRtfWriter = NULL;
        m_poRtfReader = NULL;
    }

    long ConvertRtfToOOX( std::wstring sSrcFileName, std::wstring sDstPath);

    long ConvertOOXToRtf( std::wstring sDstFileName, std::wstring sSrcPath);

    void OnCompleteItemRtf();
    void OnCompleteItemOOX();
private:
    OOXWriter* m_poOOXWriter;
    OOXReader* m_poOOXReader;

    RtfWriter* m_poRtfWriter;
    RtfReader* m_poRtfReader;

    bool m_bParseFirstItem;


};
