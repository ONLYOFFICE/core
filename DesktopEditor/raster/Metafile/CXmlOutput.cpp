#include "CXmlOutput.h"
#include "../../common/Base64.h"

#ifdef _DEBUG
    #include <iostream>
#endif

namespace MetaFile
{
    std::wstring StringNormalization(std::wstring wsString)
    {
        std::wstring wsText;
        for (wchar_t wChar : wsString)
            if (wChar == L'<')
                   wsText += L"&lt;";
            else if (wChar == L'>')
                   wsText += L"&gt;";
            else if (wChar == L'&')
                   wsText += L"&amp;";
            else if (wChar == L'\'')
                   wsText += L"&apos;";
            else if (wChar == L'"')
                   wsText += L"&quot;";
            else if (wChar == 0x00)
                return wsText;
            else wsText += wChar;
        return wsText;
    }

    CXmlOutput::CXmlOutput(TypeXmlOutput oTypeXmlOutput) :
        m_pXmlWriter    (NULL),
        m_pXmlLiteReader(NULL)
    {
        if (oTypeXmlOutput == TypeXmlOutput::IsWriter)
            m_pXmlWriter = new XmlUtils::CXmlWriter;
        else if (oTypeXmlOutput == TypeXmlOutput::IsReader)
            m_pXmlLiteReader = new XmlUtils::CXmlLiteReader;
    }

    MetaFile::CXmlOutput::~CXmlOutput()
    {
        Clear();
    }

    void CXmlOutput::Clear()
    {
        if (NULL != m_pXmlWriter)
            delete m_pXmlWriter;
        else if (NULL != m_pXmlLiteReader)
            delete m_pXmlLiteReader;
    }

    bool CXmlOutput::IsWriter()
    {
        return NULL != m_pXmlWriter;
    }

    bool CXmlOutput::IsReader()
    {
        return NULL != m_pXmlLiteReader;
    }

    void CXmlOutput::WriteString(const std::wstring &wsValue)
    {
        m_pXmlWriter->WriteString(wsValue);
    }

    void CXmlOutput::SaveToFile(const std::wstring &wsPathToFile)
    {
        m_pXmlWriter->SaveToFile(wsPathToFile);
    }

    void CXmlOutput::WriteNodeBegin(const std::wstring &wsNameNode, std::vector<XmlArgument> arArguments)
    {        
        if (wsNameNode.empty())
            return;

        m_pXmlWriter->WriteNodeBegin(wsNameNode, !arArguments.empty());

        if (!arArguments.empty())
        {
            for (unsigned int i = 0; i < arArguments.size(); ++i)
                WriteAttribute(arArguments[i].wsName, arArguments[i].wsValue);

            m_pXmlWriter->WriteNodeEnd(wsNameNode, true, false);
        }
    }

    void CXmlOutput::WriteAttribute(const std::wstring &wsNameAttribute, const std::wstring &wsValue)
    {
        m_pXmlWriter->WriteAttribute(wsNameAttribute, wsValue);
    }

    void CXmlOutput::WriteAttribute(const std::wstring &wsNameAttribute, unsigned int unValue)
    {
        m_pXmlWriter->WriteAttribute(wsNameAttribute, std::to_wstring(unValue));
    }

    void CXmlOutput::WriteAttribute(const std::wstring &wsNameAttribute, int nValue)
    {
        m_pXmlWriter->WriteAttribute(wsNameAttribute, nValue);
    }

    void CXmlOutput::WriteNodeEnd(const std::wstring &wsNameAttribute, bool bEmptyNode, bool bEndNode)
    {
        if (wsNameAttribute.empty())
            return;

        m_pXmlWriter->WriteNodeEnd(wsNameAttribute, bEmptyNode, bEndNode);
    }

    void CXmlOutput::WriteNode(const std::wstring& wsNameNode, const std::wstring& wsValueNode)
    {
        m_pXmlWriter->WriteNode(wsNameNode, wsValueNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, unsigned short usValueNode)
    {
        m_pXmlWriter->WriteNode(wsNameNode, std::to_wstring(usValueNode));
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, unsigned char ucValueNode)
    {
        WriteNode(wsNameNode, (unsigned int)ucValueNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, unsigned int unValueNode)
    {
        m_pXmlWriter->WriteNode(wsNameNode, std::to_wstring(unValueNode));
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, CDataStream& oDataStream, unsigned int unSizeValue)
    {
        BYTE *pData = new BYTE[unSizeValue];
        oDataStream.ReadBytes(pData, unSizeValue);
        oDataStream.SeekBack(unSizeValue);

        int nSize = NSBase64::Base64EncodeGetRequiredLength(unSizeValue);
        unsigned char* ucValue = new unsigned char[nSize];

        NSBase64::Base64Encode(pData, unSizeValue, ucValue, &nSize);
        std::wstring wsValue(ucValue, ucValue + nSize);
        m_pXmlWriter->WriteNode(wsNameNode, wsValue);

        delete[] pData;
        delete[] ucValue;
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, double dValueNode)
    {
        m_pXmlWriter->WriteNode(wsNameNode, dValueNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, short shValueNode)
    {
        m_pXmlWriter->WriteNode(wsNameNode, std::to_wstring(shValueNode));
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, int nValueNode)
    {
        m_pXmlWriter->WriteNode(wsNameNode, nValueNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, std::vector<XmlArgument> arArguments)
    {
        m_pXmlWriter->WriteNodeBegin(wsNameNode, true);

        if (!arArguments.empty())
        {
            for (unsigned int i = 0; i < arArguments.size(); ++i)
                WriteAttribute(arArguments[i].wsName, arArguments[i].wsValue);
        }

        m_pXmlWriter->WriteNodeEnd(wsNameNode, true, true);

    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfRectL &oTEmfRectL, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfRectL(oTEmfRectL);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfSizeL &oTEmfSizeL, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfSizeL(oTEmfSizeL);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfAlphaBlend &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfAlphaBlend(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TXForm &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTXForm(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfColor &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfColor(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfStretchDIBITS &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfStretchDIBITS(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfBitBlt &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfBitBlt(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfSetDiBitsToDevice &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfSetDiBitsToDevice(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfStretchBLT &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfStretchBLT(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfDibPatternBrush &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfDibPatternBrush(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfExtTextoutA &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfExtTextoutA(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfPointL &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfPointL(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfPointS &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfPointS(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfEmrText &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfEmrText(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfExtTextoutW &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfExtTextoutW(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfSmallTextout &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteTEmfSmallTextout(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const CEmfLogBrushEx &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteEmfLogBrushEx(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, CEmfLogFont &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteEmfLogFont(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const CEmfLogPen &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteEmfLogPen(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const CEmfLogPalette &oNodeValue, std::vector<XmlArgument> arArguments)
    {
        WriteNodeBegin(wsNameNode, arArguments);
        WriteEmfLogPalette(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteTEmfRectL(const TEmfRectL &oTEmfRectL)
    {
        WriteNode(L"Left",     oTEmfRectL.lLeft);
        WriteNode(L"Top",      oTEmfRectL.lTop);
        WriteNode(L"Right",    oTEmfRectL.lRight);
        WriteNode(L"Bottom",   oTEmfRectL.lBottom);
    }

    void CXmlOutput::WriteTEmfSizeL(const TEmfSizeL &oTEmfSizeL)
    {
        WriteNode(L"cX", oTEmfSizeL.cx);
        WriteNode(L"cY", oTEmfSizeL.cy);
    }

    void CXmlOutput::WriteTEmfAlphaBlend(const TEmfAlphaBlend &oTEmfAlphaBlend)
    {
        WriteNode(L"Bounds",    oTEmfAlphaBlend.Bounds);
        WriteNode(L"xDest",     oTEmfAlphaBlend.xDest);
        WriteNode(L"yDest",     oTEmfAlphaBlend.xDest);
        WriteNode(L"cxDest",    oTEmfAlphaBlend.cxDest);
        WriteNode(L"cyDest",    oTEmfAlphaBlend.cxDest);

        WriteNodeBegin(L"BLENDFUNCTION");
            WriteNode(L"BlendOperation",    oTEmfAlphaBlend.BlendOperation);
            WriteNode(L"BlendFlags",        oTEmfAlphaBlend.BlendFlags);
            WriteNode(L"SrcConstantAlpha",  oTEmfAlphaBlend.SrcConstantAlpha);
            WriteNode(L"AlphaFormat",       oTEmfAlphaBlend.AlphaFormat);
            WriteNodeEnd(L"BLENDFUNCTION");

        WriteNode(L"cxSrc",    oTEmfAlphaBlend.cxSrc);
        WriteNode(L"cySrc",    oTEmfAlphaBlend.cySrc);

        WriteNode(L"XFormSrc",      oTEmfAlphaBlend.XformSrc);
        WriteNode(L"BkColorSrc",    oTEmfAlphaBlend.BkColor);
        WriteNode(L"UsageSrc",      oTEmfAlphaBlend.UsageSrc);
        WriteNode(L"offBmiSrc",     oTEmfAlphaBlend.offBmiSrc);
        WriteNode(L"cbBmiSrc",      oTEmfAlphaBlend.cbBmiSrc);
        WriteNode(L"offBitsSrc",    oTEmfAlphaBlend.offBitsSrc);
        WriteNode(L"cbBitsSrc",     oTEmfAlphaBlend.cbBitsSrc);
        WriteNode(L"cxSrc",         oTEmfAlphaBlend.cxSrc);
        WriteNode(L"cySrc",         oTEmfAlphaBlend.cySrc);
    }

    void CXmlOutput::WriteTXForm(const TXForm &oTXForm)
    {
        WriteNode(L"M11",   oTXForm.M11);
        WriteNode(L"M12",   oTXForm.M12);
        WriteNode(L"M21",   oTXForm.M21);
        WriteNode(L"M22",   oTXForm.M22);
        WriteNode(L"Dx",    oTXForm.Dx);
        WriteNode(L"Dy",    oTXForm.Dy);
    }

    void CXmlOutput::WriteTEmfColor(const TEmfColor &oTEmfColor)
    {
        WriteNode(L"Red",       oTEmfColor.r);
        WriteNode(L"Green",     oTEmfColor.g);
        WriteNode(L"Blue",      oTEmfColor.b);
        WriteNode(L"Reserved",  oTEmfColor.a);
    }

    void CXmlOutput::WriteTEmfStretchDIBITS(const TEmfStretchDIBITS &oTEmfStretchDIBITS)
    {
        WriteNode(L"Bounds",                oTEmfStretchDIBITS.Bounds);
        WriteNode(L"xDest",                 oTEmfStretchDIBITS.xDest);
        WriteNode(L"yDest",                 oTEmfStretchDIBITS.yDest);
        WriteNode(L"xSrc",                  oTEmfStretchDIBITS.xSrc);
        WriteNode(L"ySrc",                  oTEmfStretchDIBITS.ySrc);
        WriteNode(L"cxSrc",                 oTEmfStretchDIBITS.cxSrc);
        WriteNode(L"cySrc",                 oTEmfStretchDIBITS.cySrc);
        WriteNode(L"offBmiSrc",             oTEmfStretchDIBITS.offBmiSrc);
        WriteNode(L"cbBmiSrc",              oTEmfStretchDIBITS.cbBmiSrc);
        WriteNode(L"offBitsSrc",            oTEmfStretchDIBITS.offBitsSrc);
        WriteNode(L"cbBitsSrc",             oTEmfStretchDIBITS.cbBitsSrc);
        WriteNode(L"UsageSrc",              oTEmfStretchDIBITS.UsageSrc);
        WriteNode(L"BitBltRasterOperation", oTEmfStretchDIBITS.BitBltRasterOperation);
        WriteNode(L"cxDest",                oTEmfStretchDIBITS.cxDest);
        WriteNode(L"cyDest",                oTEmfStretchDIBITS.cyDest);
    }

    void CXmlOutput::WriteTEmfBitBlt(const TEmfBitBlt &oTEmfBitBlt)
    {
        WriteNode(L"Bounds",                oTEmfBitBlt.Bounds);
        WriteNode(L"xDest",                 oTEmfBitBlt.xDest);
        WriteNode(L"yDest",                 oTEmfBitBlt.yDest);
        WriteNode(L"cxDest",                oTEmfBitBlt.cxDest);
        WriteNode(L"cyDest",                oTEmfBitBlt.cyDest);
        WriteNode(L"BitBltRasterOperation", oTEmfBitBlt.BitBltRasterOperation);
        WriteNode(L"xSrc",                  oTEmfBitBlt.xSrc);
        WriteNode(L"ySrc",                  oTEmfBitBlt.ySrc);
        WriteNode(L"XFormSrc",              oTEmfBitBlt.XfromSrc);
        WriteNode(L"BkColorSrc",            oTEmfBitBlt.BkColorSrc);
        WriteNode(L"UsageSrc",              oTEmfBitBlt.UsageSrc);
        WriteNode(L"offBmiSrc",             oTEmfBitBlt.offBmiSrc);
        WriteNode(L"cbBmiSrc",              oTEmfBitBlt.cbBmiSrc);
        WriteNode(L"offBitsSrc",            oTEmfBitBlt.offBitsSrc);
        WriteNode(L"cbBitsSrc",             oTEmfBitBlt.cbBitsSrc);
    }

    void CXmlOutput::WriteTEmfSetDiBitsToDevice(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice)
    {
        WriteNode(L"Bounds",        oTEmfSetDiBitsToDevice.Bounds);
        WriteNode(L"xDest",         oTEmfSetDiBitsToDevice.xDest);
        WriteNode(L"yDest",         oTEmfSetDiBitsToDevice.yDest);
        WriteNode(L"xSrc",          oTEmfSetDiBitsToDevice.xSrc);
        WriteNode(L"ySrc",          oTEmfSetDiBitsToDevice.ySrc);
        WriteNode(L"cxSrc",         oTEmfSetDiBitsToDevice.cxSrc);
        WriteNode(L"cySrc",         oTEmfSetDiBitsToDevice.cySrc);
        WriteNode(L"offBmiSrc",     oTEmfSetDiBitsToDevice.offBmiSrc);
        WriteNode(L"cbBmiSrc",      oTEmfSetDiBitsToDevice.cbBmiSrc);
        WriteNode(L"offBitsSrc",    oTEmfSetDiBitsToDevice.offBitsSrc);
        WriteNode(L"cbBitsSrc",     oTEmfSetDiBitsToDevice.cbBitsSrc);
        WriteNode(L"UsageSrc",      oTEmfSetDiBitsToDevice.UsageSrc);
        WriteNode(L"iStartScan",    oTEmfSetDiBitsToDevice.iStartScan);
        WriteNode(L"cScans",        oTEmfSetDiBitsToDevice.cScans);
    }

    void CXmlOutput::WriteTEmfStretchBLT(const TEmfStretchBLT &oTEmfStretchBLT)
    {
        WriteNode(L"Bounds",                oTEmfStretchBLT.Bounds);
        WriteNode(L"xDest",                 oTEmfStretchBLT.xDest);
        WriteNode(L"yDest",                 oTEmfStretchBLT.yDest);
        WriteNode(L"cxDest",                oTEmfStretchBLT.cxDest);
        WriteNode(L"cyDest",                oTEmfStretchBLT.cyDest);
        WriteNode(L"BitBltRasterOperation", oTEmfStretchBLT.BitBltRasterOperation);
        WriteNode(L"xSrc",                  oTEmfStretchBLT.xSrc);
        WriteNode(L"ySrc",                  oTEmfStretchBLT.ySrc);
        WriteNode(L"XFormSrc",              oTEmfStretchBLT.XformSrc);
        WriteNode(L"BkColorSrc",            oTEmfStretchBLT.BkColorSrc);
        WriteNode(L"UsageSrc",              oTEmfStretchBLT.UsageSrc);
        WriteNode(L"offBmiSrc",             oTEmfStretchBLT.offBmiSrc);
        WriteNode(L"cbBmiSrc",              oTEmfStretchBLT.cbBmiSrc);
        WriteNode(L"offBitsSrc",            oTEmfStretchBLT.offBitsSrc);
        WriteNode(L"cbBitsSrc",             oTEmfStretchBLT.cbBitsSrc);
        WriteNode(L"cxSrc",                 oTEmfStretchBLT.cxSrc);
        WriteNode(L"cySrc",                 oTEmfStretchBLT.cySrc);
    }

    void CXmlOutput::WriteTEmfDibPatternBrush(const TEmfDibPatternBrush &oTEmfDibPatternBrush)
    {
        WriteNode(L"Usage",   oTEmfDibPatternBrush.Usage);
        WriteNode(L"offBmi",  oTEmfDibPatternBrush.offBmi);
        WriteNode(L"cbBmi",   oTEmfDibPatternBrush.cbBmi);
        WriteNode(L"offBits", oTEmfDibPatternBrush.offBits);
        WriteNode(L"cbBits",  oTEmfDibPatternBrush.cbBits);
    }

    void CXmlOutput::WriteTEmfExtTextoutA(const TEmfExtTextoutA &oTEmfExtTextoutA)
    {
        WriteNode(L"Bounds",        oTEmfExtTextoutA.Bounds);
        WriteNode(L"iGraphicsMode", oTEmfExtTextoutA.iGraphicsMode);
        WriteNode(L"exScale",       oTEmfExtTextoutA.exScale);
        WriteNode(L"eyScale",       oTEmfExtTextoutA.eyScale);
        WriteNode(L"EmrText",       oTEmfExtTextoutA.aEmrText);
    }

    void CXmlOutput::WriteTEmfEmrText(const TEmfEmrText &oTEmfEmrText)
    {
        WriteNode(L"Reference", oTEmfEmrText.Reference);
        WriteNode(L"Chars",     oTEmfEmrText.Chars);
        WriteNode(L"offString", oTEmfEmrText.offString);
        WriteNode(L"Options",   oTEmfEmrText.Options);
        WriteNode(L"Rectangle", oTEmfEmrText.Rectangle);
        WriteNode(L"offDx",     oTEmfEmrText.offDx);

        std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfEmrText.OutputString, oTEmfEmrText.Chars);

        WriteNode(L"Text",      StringNormalization(wsText));
    }

    void CXmlOutput::WriteTEmfPointL(const TEmfPointL &oTEmfPointL)
    {
        WriteNode(L"X", oTEmfPointL.x);
        WriteNode(L"Y", oTEmfPointL.y);
    }

    void CXmlOutput::WriteTEmfPointS(const TEmfPointS &oTEmfPointS)
    {
        WriteNode(L"X", oTEmfPointS.x);
        WriteNode(L"Y", oTEmfPointS.y);
    }

    void CXmlOutput::WriteTEmfExtTextoutW(const TEmfExtTextoutW &oTEmfExtTextoutW)
    {
        WriteNode(L"Bounds",        oTEmfExtTextoutW.Bounds);
        WriteNode(L"iGraphicsMode", oTEmfExtTextoutW.iGraphicsMode);
        WriteNode(L"exScale",       oTEmfExtTextoutW.exScale);
        WriteNode(L"eyScale",       oTEmfExtTextoutW.eyScale);
        WriteNode(L"EmrText",       oTEmfExtTextoutW.wEmrText);
    }

    void CXmlOutput::WriteTEmfSmallTextout(const TEmfSmallTextout &oTEmfSmallTextout)
    {
        WriteNode(L"x",             oTEmfSmallTextout.x);
        WriteNode(L"y",             oTEmfSmallTextout.y);
        WriteNode(L"cChars",        oTEmfSmallTextout.cChars);
        WriteNode(L"fuOptions",     oTEmfSmallTextout.fuOptions);
        WriteNode(L"iGraphicsMode", oTEmfSmallTextout.iGraphicsMode);
        WriteNode(L"exScale",       oTEmfSmallTextout.exScale);
        WriteNode(L"eyScale",       oTEmfSmallTextout.eyScale);
        WriteNode(L"Bounds",        oTEmfSmallTextout.Bounds);

        std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfSmallTextout.TextString, oTEmfSmallTextout.cChars);

        WriteNode(L"Text",          StringNormalization(wsText));
    }

    void CXmlOutput::WriteEmfLogBrushEx(const CEmfLogBrushEx &oEmfLogBrushEx)
    {
        WriteNode(L"BrushStyle",    oEmfLogBrushEx.BrushStyle);
        WriteNode(L"Color",         oEmfLogBrushEx.Color);
        WriteNode(L"BrushHatch",    oEmfLogBrushEx.BrushHatch);
    }

    void CXmlOutput::WriteEmfLogFont(CEmfLogFont &oEmfLogFont)
    {
        if (!oEmfLogFont.IsFixedLength())
            WriteNodeBegin(L"LogFontExDv");

        WriteNodeBegin(L"LogFont");
            WriteNode(L"Height",            oEmfLogFont.LogFontEx.LogFont.Height);
            WriteNode(L"Width",             oEmfLogFont.LogFontEx.LogFont.Width);
            WriteNode(L"Escapement",        oEmfLogFont.LogFontEx.LogFont.Escapement);
            WriteNode(L"Orientation",       oEmfLogFont.LogFontEx.LogFont.Orientation);
            WriteNode(L"Weight",            oEmfLogFont.LogFontEx.LogFont.Weight);
            WriteNode(L"Italic",            oEmfLogFont.LogFontEx.LogFont.Italic);
            WriteNode(L"Underline",         oEmfLogFont.LogFontEx.LogFont.Underline);
            WriteNode(L"StrikeOut",         oEmfLogFont.LogFontEx.LogFont.StrikeOut);
            WriteNode(L"CharSet",           oEmfLogFont.LogFontEx.LogFont.CharSet);
            WriteNode(L"OutPrecision",      oEmfLogFont.LogFontEx.LogFont.OutPrecision);
            WriteNode(L"ClipPrecision",     oEmfLogFont.LogFontEx.LogFont.ClipPrecision);
            WriteNode(L"Quality",           oEmfLogFont.LogFontEx.LogFont.Quality);
            WriteNode(L"PitchAndFamily",    oEmfLogFont.LogFontEx.LogFont.PitchAndFamily);
            WriteNode(L"FaceName",          StringNormalization(NSStringExt::CConverter::GetUnicodeFromUTF16(oEmfLogFont.LogFontEx.LogFont.FaceName, 32)));
            WriteNodeEnd(L"LogFont");

	WriteNode(L"FullName",	StringNormalization(NSStringExt::CConverter::GetUnicodeFromUTF16(oEmfLogFont.LogFontEx.FullName, 64)));
	WriteNode(L"Style",	StringNormalization(NSStringExt::CConverter::GetUnicodeFromUTF16(oEmfLogFont.LogFontEx.Style,	32)));
	WriteNode(L"Script",	StringNormalization(NSStringExt::CConverter::GetUnicodeFromUTF16(oEmfLogFont.LogFontEx.Script,	32)));

	if (!oEmfLogFont.IsFixedLength())
	{
	    WriteNodeBegin(L"DesignVector");
		WriteNode(L"Signature",  oEmfLogFont.DesignVector.Signature);
		WriteNode(L"NumAxes",    oEmfLogFont.DesignVector.NumAxes);

		for (unsigned int i = 0; i <oEmfLogFont.DesignVector.NumAxes; ++i)
		    WriteNode(L"NumAxes" + std::to_wstring(i + 1),	oEmfLogFont.DesignVector.Values[i]);

		WriteNodeEnd(L"DesignVector");
		WriteNodeEnd(L"LogFontExDv");
	}

    }

    void CXmlOutput::WriteEmfLogPen(const CEmfLogPen &oEmfLogPen)
    {

    }

    void CXmlOutput::WriteEmfLogPalette(const CEmfLogPalette &oEmfLogPalette)
    {
        WriteNode(L"Version",           768);
        WriteNode(L"NumberOfEntries",   oEmfLogPalette.NumberOfEntries);

	for(unsigned int i = 0; i < oEmfLogPalette.NumberOfEntries; ++i)
	{
	    WriteNodeBegin(L"LogPaletteEntry" + std::to_wstring(i + 1));
		WriteNode(L"Reserved",	oEmfLogPalette.PaletteEntries[i].Reserved);
		WriteNode(L"Blue",	oEmfLogPalette.PaletteEntries[i].Blue);
		WriteNode(L"Green",	oEmfLogPalette.PaletteEntries[i].Green);
		WriteNode(L"Red",	oEmfLogPalette.PaletteEntries[i].Red);
		WriteNodeEnd(L"LogPaletteEntry" + std::to_wstring(i + 1));
	    }
    }

    bool CXmlOutput::ReadFromFile(const std::wstring wsPathToFile)
    {
        if (m_pXmlLiteReader->FromFile(wsPathToFile))
        {
            m_pXmlLiteReader->ReadNextNode();

            if (m_pXmlLiteReader->GetName() != L"EMF")
                return false;

            m_pXmlLiteReader->ReadNextSiblingNode(0);
            return true;
        }
        return false;
    }

    void CXmlOutput::ReadArguments(unsigned int &unType, unsigned int &unSize)
    {
        if (m_pXmlLiteReader->GetAttributesCount() == 2)
        {
            m_pXmlLiteReader->MoveToFirstAttribute();
            if (m_pXmlLiteReader->GetName() == L"Id")
                unType = stoi(m_pXmlLiteReader->GetAttributeTextWithHHHH());

            m_pXmlLiteReader->MoveToNextAttribute();
            if (m_pXmlLiteReader->GetName() == L"Size")
                unSize = stoi(m_pXmlLiteReader->GetAttributeTextWithHHHH());
            m_pXmlLiteReader->MoveToElement();
        }
    }

    void CXmlOutput::ReadNextRecord()
    {
        m_pXmlLiteReader->ReadNextSiblingNode(0);
    }

    void CXmlOutput::operator>>(TEmfHeader &oTEmfHeader)
    {
        *this >> oTEmfHeader.oBounds;
        *this >> oTEmfHeader.oFrame;
        *this >> oTEmfHeader.ulSignature;
        *this >> oTEmfHeader.ulVersion;
        *this >> oTEmfHeader.ulSize;
        *this >> oTEmfHeader.ulRecords;
        *this >> oTEmfHeader.ushObjects;
        *this >> oTEmfHeader.ushReserved;
        *this >> oTEmfHeader.ulSizeDescription;
        *this >> oTEmfHeader.ulOffsetDescription;
        *this >> oTEmfHeader.ulPalEntries;
        *this >> oTEmfHeader.oDevice;
        *this >> oTEmfHeader.oMillimeters;
//        *this >> oTEmfHeader.oFrameToBounds;
        //        *this >> oTEmfHeader.oFramePx;
    }

    void CXmlOutput::operator>>(TEmfAlphaBlend &oTEmfAlphaBlend)
    {
        *this >> oTEmfAlphaBlend.Bounds;
        *this >> oTEmfAlphaBlend.xDest;
        *this >> oTEmfAlphaBlend.yDest;
        *this >> oTEmfAlphaBlend.cxDest;
        *this >> oTEmfAlphaBlend.cyDest;
        //TODO: закончить реализацию при встрече примера

    }

    void CXmlOutput::operator>>(TEmfBitBlt &oTEmfBitBlt)
    {
        *this >> oTEmfBitBlt.Bounds;
        *this >> oTEmfBitBlt.xDest;
        *this >> oTEmfBitBlt.yDest;
        *this >> oTEmfBitBlt.cxDest;
        *this >> oTEmfBitBlt.cyDest;
        *this >> oTEmfBitBlt.BitBltRasterOperation;
        *this >> oTEmfBitBlt.xSrc;
        *this >> oTEmfBitBlt.ySrc;
        *this >> oTEmfBitBlt.XfromSrc;
        *this >> oTEmfBitBlt.BkColorSrc;
        *this >> oTEmfBitBlt.UsageSrc;
        *this >> oTEmfBitBlt.offBmiSrc;
        *this >> oTEmfBitBlt.cbBmiSrc;
        *this >> oTEmfBitBlt.offBitsSrc;
        *this >> oTEmfBitBlt.cbBitsSrc;

        if (oTEmfBitBlt.cbBitsSrc != 0)
        {
            //TODO: закончить реализацию при встрече примера
        }
    }

    void CXmlOutput::operator>>(CEmfLogFont &oCEmfLogFont)
    {
        if (!oCEmfLogFont.IsFixedLength())
            m_pXmlLiteReader->ReadNextNode();

        *this >> oCEmfLogFont.LogFontEx;

        if (!oCEmfLogFont.IsFixedLength())
            *this >> oCEmfLogFont.DesignVector;
    }

    void CXmlOutput::operator>>(TEmfLogFontEx&  oTEmfLogFontEx)
    {
        m_pXmlLiteReader->ReadNextNode();

        *this >> oTEmfLogFontEx.LogFont;
        *this >> oTEmfLogFontEx.FullName;
        *this >> oTEmfLogFontEx.Style;
        *this >> oTEmfLogFontEx.Script;
    }

    void CXmlOutput::operator>>(TEmfLogFont&  oTEmfLogFont)
    {
        *this >> oTEmfLogFont.Height;
        *this >> oTEmfLogFont.Width;
        *this >> oTEmfLogFont.Escapement;
        *this >> oTEmfLogFont.Orientation;
        *this >> oTEmfLogFont.Weight;
        *this >> oTEmfLogFont.Italic;
        *this >> oTEmfLogFont.Underline;
        *this >> oTEmfLogFont.StrikeOut;
        *this >> oTEmfLogFont.CharSet;
        *this >> oTEmfLogFont.OutPrecision;
        *this >> oTEmfLogFont.ClipPrecision;
        *this >> oTEmfLogFont.Quality;
        *this >> oTEmfLogFont.PitchAndFamily;
        *this >> oTEmfLogFont.FaceName;
    }

    void CXmlOutput::operator>>(TEmfDesignVector&  oTEmfDesignVector)
    {
        m_pXmlLiteReader->ReadNextNode();

        *this >> oTEmfDesignVector.Signature;
        *this >> oTEmfDesignVector.NumAxes;

        if (oTEmfDesignVector.NumAxes > 0)
        {
            oTEmfDesignVector.Values = new int[oTEmfDesignVector.NumAxes];
            int nValue;
            for (unsigned int i = 0; i < oTEmfDesignVector.NumAxes; ++i)
            {
                *this >> nValue;
                oTEmfDesignVector.Values[i] = nValue;
            }
        }
    }

    void CXmlOutput::operator>>(TEmfExtTextoutW &oTEmfExtTextoutW)
    {
        *this >> oTEmfExtTextoutW.Bounds;
        *this >> oTEmfExtTextoutW.iGraphicsMode;
        *this >> oTEmfExtTextoutW.exScale;
        *this >> oTEmfExtTextoutW.eyScale;
        *this >> oTEmfExtTextoutW.wEmrText;
    }

    void CXmlOutput::operator>>(TEmfEmrText &oTEmfEmrText)
    {
        m_pXmlLiteReader->ReadNextNode();

        *this >> oTEmfEmrText.Reference;
        *this >> oTEmfEmrText.Chars;
        *this >> oTEmfEmrText.offString;
        *this >> oTEmfEmrText.Options;
        *this >> oTEmfEmrText.Rectangle;
        *this >> oTEmfEmrText.offDx;
        oTEmfEmrText.OutputString = new unsigned short[oTEmfEmrText.Chars];
        *this >> (unsigned short*)oTEmfEmrText.OutputString;
    }

    void CXmlOutput::operator>>(TXForm &oTXForm)
    {
        m_pXmlLiteReader->ReadNextNode();

        *this >> oTXForm.M11;
        *this >> oTXForm.M12;
        *this >> oTXForm.M21;
        *this >> oTXForm.M22;
        *this >> oTXForm.Dx;
        *this >> oTXForm.Dy;
    }

    void CXmlOutput::operator>>(TEmfColor &oTEmfColor)
    {
        m_pXmlLiteReader->ReadNextNode();

        *this >> oTEmfColor.r;
        *this >> oTEmfColor.g;
        *this >> oTEmfColor.b;
        *this >> oTEmfColor.a;
    }


    void CXmlOutput::operator>>(CEmfLogBrushEx &oCEmfLogBrushEx)
    {
        m_pXmlLiteReader->ReadNextNode();

        *this >> oCEmfLogBrushEx.BrushStyle;
        *this >> oCEmfLogBrushEx.Color;
        *this >> oCEmfLogBrushEx.BrushHatch;
    }

    void CXmlOutput::operator>>(TEmfRectL &oTEmfRectL)
    {
        m_pXmlLiteReader->ReadNextNode();
        *this >> oTEmfRectL.lLeft;
        *this >> oTEmfRectL.lTop;
        *this >> oTEmfRectL.lRight;
        *this >> oTEmfRectL.lBottom;
    }

    void CXmlOutput::operator>>(TEmfSizeL &oTEmfSizeL)
    {
        m_pXmlLiteReader->ReadNextNode();
        *this >> oTEmfSizeL.cx;
        *this >> oTEmfSizeL.cy;
    }

    void CXmlOutput::operator>>(TEmfPointL &oTEmfPointL)
    {
        m_pXmlLiteReader->ReadNextNode();
        *this >> oTEmfPointL.x;
        *this >> oTEmfPointL.y;
    }

    void CXmlOutput::operator>>(TRect &oTRect)
    {
        m_pXmlLiteReader->ReadNextNode();
        *this >> oTRect.nLeft;
        *this >> oTRect.nTop;
        *this >> oTRect.nRight;
        *this >> oTRect.nBottom;
    }

    void CXmlOutput::operator>>(int &nValue)
    {
        m_pXmlLiteReader->ReadNextNode();
        const std::wstring wsValue = m_pXmlLiteReader->GetText2();
        nValue = (wsValue.empty()) ? 0 : stoi(wsValue);
    }

    void CXmlOutput::operator>>(double &dValue)
    {
        m_pXmlLiteReader->ReadNextNode();
        const std::wstring wsValue = m_pXmlLiteReader->GetText2();
        dValue = (wsValue.empty()) ? 0.0 : stod(wsValue);
    }


    void CXmlOutput::operator>>(unsigned int &unValue)
    {
        m_pXmlLiteReader->ReadNextNode();
        const std::wstring wsValue = m_pXmlLiteReader->GetText2();
        unValue = (wsValue.empty()) ? 0 : (unsigned int)stoul(wsValue);
    }

    void CXmlOutput::operator>>(unsigned short &ushValue)
    {
        m_pXmlLiteReader->ReadNextNode();
        const std::wstring wsValue = m_pXmlLiteReader->GetText2();
        ushValue = (wsValue.empty()) ? 0 : (unsigned short)stoul(wsValue);
    }

    void CXmlOutput::operator>>(unsigned char &ucValue)
    {
        m_pXmlLiteReader->ReadNextNode();
        const std::wstring wsValue = m_pXmlLiteReader->GetText2();
        ucValue = (wsValue.empty()) ? 0 : (unsigned char)stoul(wsValue);
    }

    void CXmlOutput::operator>>(unsigned short arushValue[])
    {
        m_pXmlLiteReader->ReadNextNode();
        const std::wstring wsValue = m_pXmlLiteReader->GetText2();

        unsigned int unSize;
        unsigned short *pTemp = NSStringExt::CConverter::GetUtf16FromUnicode(wsValue, unSize);

        for (unsigned int i = 0; i < unSize; ++i)
            arushValue[i] = pTemp[i];
    }
}
