#include "CXmlOutput.h"
#include "../../common/Base64.h"

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

    CXmlOutput::CXmlOutput()
    {}

    MetaFile::CXmlOutput::~CXmlOutput()
    {}

    void CXmlOutput::WriteString(const std::wstring &wsValue)
    {
        m_oXmlWriter.WriteString(wsValue);
    }

    void CXmlOutput::SaveToFile(const std::wstring &wsPathToFile)
    {
        m_oXmlWriter.SaveToFile(wsPathToFile);
    }

    void CXmlOutput::WriteNodeBegin(const std::wstring &wsNameNode, std::vector<XmlArgument> arArguments)
    {        
        if (wsNameNode.empty())
            return;

        m_oXmlWriter.WriteNodeBegin(wsNameNode, !arArguments.empty());

        if (!arArguments.empty())
        {
            for (unsigned int i = 0; i < arArguments.size(); ++i)
                WriteAttribute(arArguments[i].wsName, arArguments[i].wsValue);

            m_oXmlWriter.WriteNodeEnd(wsNameNode, true, false);
        }
    }

    void CXmlOutput::WriteAttribute(const std::wstring &wsNameAttribute, const std::wstring &wsValue)
    {
        m_oXmlWriter.WriteAttribute(wsNameAttribute, wsValue);
    }

    void CXmlOutput::WriteAttribute(const std::wstring &wsNameAttribute, unsigned int unValue)
    {
        m_oXmlWriter.WriteAttribute(wsNameAttribute, std::to_wstring(unValue));
    }

    void CXmlOutput::WriteAttribute(const std::wstring &wsNameAttribute, int nValue)
    {
        m_oXmlWriter.WriteAttribute(wsNameAttribute, nValue);
    }

    void CXmlOutput::WriteNodeEnd(const std::wstring &wsNameAttribute, bool bEmptyNode, bool bEndNode)
    {
        if (wsNameAttribute.empty())
            return;

        m_oXmlWriter.WriteNodeEnd(wsNameAttribute, bEmptyNode, bEndNode);
    }

    void CXmlOutput::WriteNode(const std::wstring& wsNameNode, const std::wstring& wsValueNode)
    {
        m_oXmlWriter.WriteNode(wsNameNode, wsValueNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, unsigned short usValueNode)
    {
        m_oXmlWriter.WriteNode(wsNameNode, std::to_wstring(usValueNode));
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, unsigned char ucValueNode)
    {
        WriteNode(wsNameNode, (unsigned int)ucValueNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, unsigned int unValueNode)
    {
        m_oXmlWriter.WriteNode(wsNameNode, std::to_wstring(unValueNode));
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const BYTE *pValueNode, unsigned int unSizeValue)
    {
        int nSize = NSBase64::Base64EncodeGetRequiredLength(unSizeValue);
        unsigned char* ucValue = new unsigned char[nSize];

        NSBase64::Base64Encode(pValueNode, unSizeValue, ucValue, &nSize);
        std::wstring wsValue(ucValue, ucValue + nSize);
        m_oXmlWriter.WriteNode(wsNameNode, wsValue);

        delete[] ucValue;
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, double dValueNode)
    {
        m_oXmlWriter.WriteNode(wsNameNode, dValueNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, short shValueNode)
    {
        m_oXmlWriter.WriteNode(wsNameNode, std::to_wstring(shValueNode));
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, int nValueNode)
    {
        m_oXmlWriter.WriteNode(wsNameNode, nValueNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, std::vector<XmlArgument> arArguments)
    {
        m_oXmlWriter.WriteNodeBegin(wsNameNode, true);

        if (!arArguments.empty())
        {
            for (unsigned int i = 0; i < arArguments.size(); ++i)
                WriteAttribute(arArguments[i].wsName, arArguments[i].wsValue);
        }

        m_oXmlWriter.WriteNodeEnd(wsNameNode, true, true);

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

        std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromSingleByteString((unsigned char*)oTEmfEmrText.OutputString, oTEmfEmrText.Chars);

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

}
