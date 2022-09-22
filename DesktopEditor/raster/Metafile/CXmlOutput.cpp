#include "CXmlOutput.h"
#include "../../common/Base64.h"

#ifdef _DEBUG
#include <iostream>
#endif

namespace MetaFile
{
    static int GetInteger (const std::wstring& string)
    {
        if (string.empty()) return 0;

        try
        {
			return std::stoi(string);
        }
        catch(...)
        {
        }

        try
        {
			return static_cast<int>(std::stoll(string));
        }
        catch(...)
        {
            return 0;
        }
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
            RELEASEOBJECT(m_pXmlWriter);
            RELEASEOBJECT(m_pXmlLiteReader);
    }

    bool CXmlOutput::IsWriter() const
    {
        return NULL != m_pXmlWriter;
    }

    bool CXmlOutput::IsReader() const
    {
        return NULL != m_pXmlLiteReader;
    }

    bool CXmlOutput::IsValid() const
    {
        return (IsReader()) ? m_pXmlLiteReader->IsValid() : m_pXmlLiteReader->IsValid();
    }

    bool CXmlOutput::IsEmptyNode() const
    {
        return (IsReader() ? m_pXmlLiteReader->IsEmptyNode() : false);
    }

    bool CXmlOutput::MoveToStart()
    {
        return (IsReader() ? m_pXmlLiteReader->MoveToStart() : false);
    }

    void CXmlOutput::WriteString(const std::wstring &wsValue)
    {
        m_pXmlWriter->WriteString(wsValue);
    }

    void CXmlOutput::SaveToFile(const std::wstring &wsPathToFile)
    {
        m_pXmlWriter->SaveToFile(wsPathToFile);
    }

    void CXmlOutput::WriteNodeBegin(const std::wstring &wsNameNode)
    {        
        if (wsNameNode.empty())
            return;

        m_pXmlWriter->WriteNodeBegin(wsNameNode);
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
        WriteNodeBegin(wsNameNode);
        WriteString(wsValueNode);
        WriteNodeEnd(wsNameNode);
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

        if (!wsValue.empty())
            WriteNode(wsNameNode, wsValue);

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

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode)
    {
        m_pXmlWriter->WriteNodeBegin(wsNameNode, true);
        m_pXmlWriter->WriteNodeEnd(wsNameNode, true, true);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfRectL &oTEmfRectL)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfRectL(oTEmfRectL);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfPlusRect &oNodeValue)
    {
            WriteNodeBegin(wsNameNode);
            WriteTEmfPlusRect(oNodeValue);
            WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfPlusRectF &oNodeValue)
    {
            WriteNodeBegin(wsNameNode);
            WriteTEmfPlusRectF(oNodeValue);
            WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfSizeL &oTEmfSizeL)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfSizeL(oTEmfSizeL);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfAlphaBlend &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfAlphaBlend(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TXForm &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteTXForm(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfColor &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfColor(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfPlusARGB &oNodeValue)
    {
            WriteNodeBegin(wsNameNode);
            WriteARGB(oNodeValue);
            WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfStretchDIBITS &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfStretchDIBITS(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfBitBlt &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfBitBlt(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfSetDiBitsToDevice &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfSetDiBitsToDevice(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfStretchBLT &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfStretchBLT(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfDibPatternBrush &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfDibPatternBrush(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfExtTextoutA &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfExtTextoutA(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfPointL &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfPointL(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfPointS &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfPointS(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfPlusPointR &oNodeValue)
    {
            WriteNodeBegin(wsNameNode);
            WriteTEmfPlusPointR(oNodeValue);
            WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfPlusPointF &oNodeValue)
    {
            WriteNodeBegin(wsNameNode);
            WriteTEmfPlusPointF(oNodeValue);
            WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfEmrText &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfEmrText(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfExtTextoutW &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfExtTextoutW(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfSmallTextout &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteTEmfSmallTextout(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const CEmfLogBrushEx &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteEmfLogBrushEx(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, CEmfLogFont &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteEmfLogFont(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const CEmfLogPen &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteEmfLogPen(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const CEmfLogPalette &oNodeValue)
    {
        WriteNodeBegin(wsNameNode);
        WriteEmfLogPalette(oNodeValue);
        WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const std::vector<TEmfPlusPointF>& arValues)
    {
            WriteNodeBegin(wsNameNode);
            for (unsigned int unIndex = 0; unIndex < arValues.size(); ++unIndex)
                    WriteNode(L"Point" + std::to_wstring(unIndex + 1), arValues[unIndex]);
            WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
    {
            WriteNodeBegin(wsNameNode);
                WriteTRegionDataHeader(oRegionDataHeader);
                for (unsigned int unIndex = 0; unIndex < arRects.size(); ++unIndex)
                        WriteNode(L"Rect" + std::to_wstring(unIndex + 1), arRects[unIndex]);
            WriteNodeEnd(wsNameNode);
    }

    void CXmlOutput::WriteTEmfRectL(const TEmfRectL &oTEmfRectL)
    {
        WriteNode(L"Left",     oTEmfRectL.lLeft);
        WriteNode(L"Top",      oTEmfRectL.lTop);
        WriteNode(L"Right",    oTEmfRectL.lRight);
        WriteNode(L"Bottom",   oTEmfRectL.lBottom);
    }

    void CXmlOutput::WriteTEmfPlusRect(const TEmfPlusRect &oTEmfPlusRect)
    {
            WriteNode(L"X",      oTEmfPlusRect.shX);
            WriteNode(L"Y",      oTEmfPlusRect.shY);
            WriteNode(L"Width",  oTEmfPlusRect.shWidth);
            WriteNode(L"Height", oTEmfPlusRect.shHeight);
    }

    void CXmlOutput::WriteTEmfPlusRectF(const TEmfPlusRectF &oTEmfPlusRectF)
    {
            WriteNode(L"X",      oTEmfPlusRectF.dX);
            WriteNode(L"Y",      oTEmfPlusRectF.dY);
            WriteNode(L"Width",  oTEmfPlusRectF.dWidth);
            WriteNode(L"Height", oTEmfPlusRectF.dHeight);
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

    void CXmlOutput::WriteARGB(const TEmfPlusARGB &oTEmfPlusARGB)
    {
            WriteNode(L"Blue",  oTEmfPlusARGB.chBlue);
            WriteNode(L"Green", oTEmfPlusARGB.chGreen);
            WriteNode(L"Red",   oTEmfPlusARGB.chRed);
            WriteNode(L"Alpha", oTEmfPlusARGB.chAlpha);
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
        WriteNode(L"offBmi",  32/*oTEmfDibPatternBrush.offBmi*/);
        WriteNode(L"cbBmi",   oTEmfDibPatternBrush.cbBmi);
        WriteNode(L"offBits", 32 + oTEmfDibPatternBrush.cbBmi/*oTEmfDibPatternBrush.offBits*/);
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
        WriteNode(L"offString", 76/*oTEmfEmrText.offString*/);
        WriteNode(L"Options",   oTEmfEmrText.Options);
        WriteNode(L"Rectangle", oTEmfEmrText.Rectangle);
        WriteNode(L"offDx",     76 + oTEmfEmrText.Chars * 2 + (oTEmfEmrText.Chars % 2) * 2/*oTEmfEmrText.offDx*/);

        if (oTEmfEmrText.Chars == 0)
                return;

        std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfEmrText.OutputString, oTEmfEmrText.Chars);

        WriteNode(L"Text",      StringNormalization(wsText));
        WriteNode(L"Dx",        oTEmfEmrText.OutputDx, oTEmfEmrText.Chars);
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

    void CXmlOutput::WriteTEmfPlusPointR(const TEmfPlusPointR &oTEmfPlusPointR)
    {
            //TODO: реализовать
    }

    void CXmlOutput::WriteTEmfPlusPointF(const TEmfPlusPointF &oTEmfPlusPointF)
    {
            WriteNode(L"X", oTEmfPlusPointF.X);
            WriteNode(L"Y", oTEmfPlusPointF.Y);
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

    void CXmlOutput::WriteTRegionDataHeader(const TRegionDataHeader &oTRegionDataHeader)
    {
            WriteNode(L"CountRects", oTRegionDataHeader.unCountRects);
            WriteNode(L"Bounds ",    oTRegionDataHeader.oBounds);
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
//		WriteNode(L"NumAxes",    oEmfLogFont.DesignVector.NumAxes);

		for (unsigned int i = 0; i <oEmfLogFont.DesignVector.NumAxes; ++i)
		    WriteNode(L"Value" + std::to_wstring(i + 1),	oEmfLogFont.DesignVector.Values[i]);

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

    void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const unsigned int *pUnValues, const unsigned int &unSize)
    {
        if (NULL == pUnValues || unSize == 0)
            return;

        WriteNodeBegin(wsNameNode);
            for (unsigned int i = 1; i < unSize + 1; ++i)
                WriteNode(L"Dx" + std::to_wstring(i), pUnValues[i - 1]);
            WriteNodeEnd(wsNameNode);
    }

    bool CXmlOutput::ReadFromFile(const std::wstring &wsPathToFile)
    {
        return m_pXmlLiteReader->FromFile(wsPathToFile);
    }

    void CXmlOutput::ReadArguments(unsigned int &unType, unsigned int &unSize)
    {
        while(!IsRecord())
            m_pXmlLiteReader->ReadNextNode();

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

    bool CXmlOutput::ReadNextRecord()
    {
        return m_pXmlLiteReader->ReadNextSiblingNode(0);
    }

    bool CXmlOutput::ReadNextNode()
    {
        return m_pXmlLiteReader->ReadNextNode();
    }

    bool CXmlOutput::IsRecord() const
    {
        return (IsReader() && m_pXmlLiteReader->GetDepth() == 1) ? true : false;
    }

    std::wstring CXmlOutput::GetName() const
    {
        return (IsReader()) ? m_pXmlLiteReader->GetName() : L"";
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

    void CXmlOutput::ReadPoint(TEmfPointL &oPoint)
    {
            *this >> oPoint.x;
            *this >> oPoint.y;
    }

    void CXmlOutput::ReadPoint(TEmfPointS &oPoint)
    {
        *this >> oPoint.x;
        *this >> oPoint.y;
    }

    void CXmlOutput::ReadDx(unsigned int arunValue[], const unsigned int &unCount)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        for (unsigned int  i = 0; i < unCount; ++i)
            *this >> arunValue[i];
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
    }

    void CXmlOutput::operator>>(TEmfStretchDIBITS &oTEmfStretchDIBITS)
    {
        *this >> oTEmfStretchDIBITS.Bounds;
        *this >> oTEmfStretchDIBITS.xDest;
        *this >> oTEmfStretchDIBITS.yDest;
        *this >> oTEmfStretchDIBITS.xSrc;
        *this >> oTEmfStretchDIBITS.ySrc;
        *this >> oTEmfStretchDIBITS.cxSrc;
        *this >> oTEmfStretchDIBITS.cySrc;
        *this >> oTEmfStretchDIBITS.offBmiSrc;
        *this >> oTEmfStretchDIBITS.cbBmiSrc;
        *this >> oTEmfStretchDIBITS.offBitsSrc;
        *this >> oTEmfStretchDIBITS.cbBitsSrc;
        *this >> oTEmfStretchDIBITS.UsageSrc;
        *this >> oTEmfStretchDIBITS.BitBltRasterOperation;
        *this >> oTEmfStretchDIBITS.cxDest;
        *this >> oTEmfStretchDIBITS.cyDest;
    }

    void CXmlOutput::operator>>(TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice)
    {
        *this >> oTEmfSetDiBitsToDevice.Bounds;
        *this >> oTEmfSetDiBitsToDevice.xDest;
        *this >> oTEmfSetDiBitsToDevice.yDest;
        *this >> oTEmfSetDiBitsToDevice.xSrc;
        *this >> oTEmfSetDiBitsToDevice.ySrc;
        *this >> oTEmfSetDiBitsToDevice.cxSrc;
        *this >> oTEmfSetDiBitsToDevice.cySrc;
        *this >> oTEmfSetDiBitsToDevice.offBmiSrc;
        *this >> oTEmfSetDiBitsToDevice.cbBmiSrc;
        *this >> oTEmfSetDiBitsToDevice.offBitsSrc;
        *this >> oTEmfSetDiBitsToDevice.cbBitsSrc;
        *this >> oTEmfSetDiBitsToDevice.UsageSrc;
        *this >> oTEmfSetDiBitsToDevice.iStartScan;
        *this >> oTEmfSetDiBitsToDevice.cScans;
    }

    void CXmlOutput::operator>>(TEmfStretchBLT &oTEmfStretchBLT)
    {
        *this >> oTEmfStretchBLT.Bounds;
        *this >> oTEmfStretchBLT.xDest;
        *this >> oTEmfStretchBLT.yDest;
        *this >> oTEmfStretchBLT.cxDest;
        *this >> oTEmfStretchBLT.cyDest;
        *this >> oTEmfStretchBLT.BitBltRasterOperation;
        *this >> oTEmfStretchBLT.xSrc;
        *this >> oTEmfStretchBLT.ySrc;
        *this >> oTEmfStretchBLT.XformSrc;
        *this >> oTEmfStretchBLT.BkColorSrc;
        *this >> oTEmfStretchBLT.UsageSrc;
        *this >> oTEmfStretchBLT.offBmiSrc;
        *this >> oTEmfStretchBLT.cbBmiSrc;
        *this >> oTEmfStretchBLT.offBitsSrc;
        *this >> oTEmfStretchBLT.cbBitsSrc;
        *this >> oTEmfStretchBLT.cxSrc;
        *this >> oTEmfStretchBLT.cySrc;
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
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

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
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        *this >> oTEmfDesignVector.Signature;
//        *this >> oTEmfDesignVector.NumAxes;

        unsigned int unDepth = m_pXmlLiteReader->GetDepth();
        std::vector<int> arValues;

        while (m_pXmlLiteReader->ReadNextSiblingNode(unDepth - 1))
                arValues.push_back(std::stoi(m_pXmlLiteReader->GetText2()));

        oTEmfDesignVector.NumAxes = arValues.size();

        if (!arValues.empty())
        {
                unsigned int unSize = arValues.size();
                oTEmfDesignVector.Values = new int[unSize];

                for (unsigned int unIndex = 0; unIndex < unSize; ++unIndex)
                        oTEmfDesignVector.Values[unIndex] = arValues[unIndex];
        }

//        if (oTEmfDesignVector.NumAxes > 0)
//        {
//            oTEmfDesignVector.Values = new int[oTEmfDesignVector.NumAxes];
//            int nValue;
//            for (unsigned int i = 0; i < oTEmfDesignVector.NumAxes; ++i)
//            {
//                *this >> nValue;
//                oTEmfDesignVector.Values[i] = nValue;
//            }
//        }
    }

    void CXmlOutput::operator>>(TEmfLogPaletteEntry &oTEmfLogPaletteEntry)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        *this >> oTEmfLogPaletteEntry.Reserved;
        *this >> oTEmfLogPaletteEntry.Blue;
        *this >> oTEmfLogPaletteEntry.Green;
        *this >> oTEmfLogPaletteEntry.Red;
    }

    void CXmlOutput::operator>>(TEmfExtTextoutW &oTEmfExtTextoutW)
    {
        *this >> oTEmfExtTextoutW.Bounds;
        *this >> oTEmfExtTextoutW.iGraphicsMode;
        *this >> oTEmfExtTextoutW.exScale;
        *this >> oTEmfExtTextoutW.eyScale;
        *this >> oTEmfExtTextoutW.wEmrText;
    }

    void CXmlOutput::operator>>(TEmfExtTextoutA &oTEmfExtTextoutA)
    {
        *this >> oTEmfExtTextoutA.Bounds;
        *this >> oTEmfExtTextoutA.iGraphicsMode;
        *this >> oTEmfExtTextoutA.exScale;
        *this >> oTEmfExtTextoutA.eyScale;
        *this >> oTEmfExtTextoutA.aEmrText;
    }

    void CXmlOutput::operator>>(TEmfSmallTextout &oTEmfSmallTextout)
    {
        *this >> oTEmfSmallTextout.x;
        *this >> oTEmfSmallTextout.y;
        *this >> oTEmfSmallTextout.cChars;
        *this >> oTEmfSmallTextout.fuOptions;
        *this >> oTEmfSmallTextout.iGraphicsMode;
        *this >> oTEmfSmallTextout.exScale;
        *this >> oTEmfSmallTextout.eyScale;
        *this >> oTEmfSmallTextout.Bounds;

        oTEmfSmallTextout.TextString = new unsigned short[oTEmfSmallTextout.cChars];
        *this >> (unsigned short*)oTEmfSmallTextout.TextString;
    }

    void CXmlOutput::operator>>(TEmfEmrText &oTEmfEmrText)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        *this >> oTEmfEmrText.Reference;
        *this >> oTEmfEmrText.Chars;
        *this >> oTEmfEmrText.offString;
        *this >> oTEmfEmrText.Options;
        *this >> oTEmfEmrText.Rectangle;
        *this >> oTEmfEmrText.offDx;

        if (oTEmfEmrText.Chars == 0)
                return;

        oTEmfEmrText.OutputString   = new unsigned short[oTEmfEmrText.Chars];
        oTEmfEmrText.OutputDx       = new unsigned int[oTEmfEmrText.Chars];

        ((unsigned short*)oTEmfEmrText.OutputString)[oTEmfEmrText.Chars - 1] = 0x00;
        *this >> (unsigned short*)oTEmfEmrText.OutputString;

        ReadDx(oTEmfEmrText.OutputDx, oTEmfEmrText.Chars);
    }

    void CXmlOutput::operator>>(TEmfDibPatternBrush &oTEmfDibPatternBrush)
    {
        *this >> oTEmfDibPatternBrush.Usage;
        *this >> oTEmfDibPatternBrush.offBmi;
        *this >> oTEmfDibPatternBrush.cbBmi;
        *this >> oTEmfDibPatternBrush.offBits;
        *this >> oTEmfDibPatternBrush.cbBits;
    }

    void CXmlOutput::operator>>(CEmfLogPalette &oCEmfLogPalette)
    {
        unsigned int unVersion;
        *this >> unVersion;
        *this >> oCEmfLogPalette.NumberOfEntries;
        if (oCEmfLogPalette.NumberOfEntries > 0)
        {
            oCEmfLogPalette.PaletteEntries = new TEmfLogPaletteEntry[oCEmfLogPalette.NumberOfEntries];

            for (unsigned int i = 0; i < oCEmfLogPalette.NumberOfEntries; ++i)
                *this >> oCEmfLogPalette.PaletteEntries[i];
        }
    }

    void CXmlOutput::operator>>(CDataStream &oCDataStream)
    {
        std::string sBuffer;
        *this >> sBuffer;

        if (sBuffer.empty())
            return;

        BYTE* pBuffer;
        int unSize = NSBase64::Base64DecodeGetRequiredLength(sBuffer.length());

        pBuffer = new BYTE[unSize];
        NSBase64::Base64Decode(sBuffer.c_str(), sBuffer.length(), pBuffer, &unSize);
        oCDataStream.SetStream(pBuffer, unSize);
    }

    void CXmlOutput::operator>>(TXForm &oTXForm)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        *this >> oTXForm.M11;
        *this >> oTXForm.M12;
        *this >> oTXForm.M21;
        *this >> oTXForm.M22;
        *this >> oTXForm.Dx;
        *this >> oTXForm.Dy;
    }

    void CXmlOutput::operator>>(TEmfColor &oTEmfColor)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        *this >> oTEmfColor.r;
        *this >> oTEmfColor.g;
        *this >> oTEmfColor.b;
        *this >> oTEmfColor.a;
    }


    void CXmlOutput::operator>>(CEmfLogBrushEx &oCEmfLogBrushEx)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        *this >> oCEmfLogBrushEx.BrushStyle;
        *this >> oCEmfLogBrushEx.Color;
        *this >> oCEmfLogBrushEx.BrushHatch;
    }

    void CXmlOutput::operator>>(TEmfRectL &oTEmfRectL)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        *this >> oTEmfRectL.lLeft;
        *this >> oTEmfRectL.lTop;
        *this >> oTEmfRectL.lRight;
        *this >> oTEmfRectL.lBottom;
    }

    void CXmlOutput::operator>>(std::vector<TEmfPointL> &arPoints)
    {
            if (!m_pXmlLiteReader->ReadNextNode())
                return;

            unsigned int unDepth = m_pXmlLiteReader->GetDepth();

            do
            {
                TEmfPointL oPoint;
                ReadPoint(oPoint);
                arPoints.push_back(oPoint);
            }
            while (m_pXmlLiteReader->ReadNextSiblingNode(unDepth - 1));
    }

    void CXmlOutput::operator>>(std::vector<TEmfPointS> &arPoints)
    {
            if (!m_pXmlLiteReader->ReadNextNode())
                return;

            unsigned int unDepth = m_pXmlLiteReader->GetDepth();

            do
            {
                TEmfPointS oPoint;
                ReadPoint(oPoint);
                arPoints.push_back(oPoint);
            }
            while (m_pXmlLiteReader->ReadNextSiblingNode(unDepth - 1));
    }

    void CXmlOutput::operator>>(std::vector<TEmfRectL> &arRects)
    {
            if (!m_pXmlLiteReader->ReadNextNode())
                return;

            unsigned int unDepth = m_pXmlLiteReader->GetDepth();

            do
            {
                TEmfRectL oRect;
                *this >> oRect;
                arRects.push_back(oRect);
            }
            while (m_pXmlLiteReader->ReadNextSiblingNode(unDepth - 1));
    }

    void CXmlOutput::operator>>(std::vector<std::vector<TEmfPointL>> &arPoints)
    {
            if (!m_pXmlLiteReader->ReadNextNode())
                return;

            unsigned int unDepth = m_pXmlLiteReader->GetDepth();

            do
            {
                std::vector<TEmfPointL> arTempPoints;
                *this >> arTempPoints;
                arPoints.push_back(arTempPoints);
            }
            while (m_pXmlLiteReader->ReadNextSiblingNode(unDepth - 1));
    }

    void CXmlOutput::operator>>(std::vector<std::vector<TEmfPointS>> &arPoints)
    {
            if (!m_pXmlLiteReader->ReadNextNode())
                return;

            unsigned int unDepth = m_pXmlLiteReader->GetDepth();

            do
            {
                std::vector<TEmfPointS> arTempPoints;
                *this >> arTempPoints;
                arPoints.push_back(arTempPoints);
            }
            while (m_pXmlLiteReader->ReadNextSiblingNode(unDepth - 1));
    }

    void CXmlOutput::operator>>(TEmfSizeL &oTEmfSizeL)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        *this >> oTEmfSizeL.cx;
        *this >> oTEmfSizeL.cy;
    }

    void CXmlOutput::operator>>(TEmfPointL &oTEmfPointL)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        ReadPoint(oTEmfPointL);
    }

    void CXmlOutput::operator>>(TEmfPointS &oTEmfPointS)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        ReadPoint(oTEmfPointS);
    }

    void CXmlOutput::operator>>(TRect &oTRect)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        *this >> oTRect.nLeft;
        *this >> oTRect.nTop;
        *this >> oTRect.nRight;
        *this >> oTRect.nBottom;
    }

    void CXmlOutput::operator>>(int &nValue)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        const std::wstring wsValue = m_pXmlLiteReader->GetText2();
        nValue = (wsValue.empty()) ? 0 : GetInteger(wsValue);
    }

    void CXmlOutput::operator>>(double &dValue)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        const std::wstring wsValue = m_pXmlLiteReader->GetText2();
        dValue = (wsValue.empty()) ? 0.0 : GetInteger(wsValue);
    }


    void CXmlOutput::operator>>(unsigned int &unValue)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        const std::wstring wsValue = m_pXmlLiteReader->GetText2();
        unValue = (wsValue.empty()) ? 0 : (unsigned int)GetInteger(wsValue);
    }

    void CXmlOutput::operator>>(short &shValue)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        const std::wstring wsValue = m_pXmlLiteReader->GetText2();
        shValue = (wsValue.empty()) ? 0 : (short)GetInteger(wsValue);
    }

    void CXmlOutput::operator>>(unsigned short &ushValue)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        const std::wstring wsValue = m_pXmlLiteReader->GetText2();
        ushValue = (wsValue.empty()) ? 0 : (unsigned short)GetInteger(wsValue);
    }

    void CXmlOutput::operator>>(unsigned char &ucValue)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        const std::wstring wsValue = m_pXmlLiteReader->GetText2();
        ucValue = (wsValue.empty()) ? 0 : (unsigned char)GetInteger(wsValue);
    }

    void CXmlOutput::operator>>(unsigned short arushValue[])
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        const std::wstring wsValue = m_pXmlLiteReader->GetText2();

        unsigned int unSize = 0;
        unsigned short *pTemp = NSStringExt::CConverter::GetUtf16FromUnicode(wsValue, unSize);

        for (unsigned int i = 0; i < unSize; ++i)
            arushValue[i] = pTemp[i];
    }

    void CXmlOutput::operator>>(std::wstring& wsValue)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        wsValue = m_pXmlLiteReader->GetText2();
    }

    void CXmlOutput::operator>>(std::string& sValue)
    {
        if (!m_pXmlLiteReader->ReadNextNode())
            return;

        const std::wstring wsValue = m_pXmlLiteReader->GetText2();
        sValue = U_TO_UTF8(wsValue);
    }
}
