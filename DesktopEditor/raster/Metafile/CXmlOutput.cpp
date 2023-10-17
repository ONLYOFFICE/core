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

	void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TRectL &oTRectL)
	{
		WriteNodeBegin(wsNameNode);
		WriteTRectL(oTRectL);
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

	void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TSizeL &oTSizeL)
	{
		WriteNodeBegin(wsNameNode);
		WriteTSizeL(oTSizeL);
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

	void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TRGBA &oNodeValue)
	{
		WriteNodeBegin(wsNameNode);
		WriteTRGBA(oNodeValue);
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

//	void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TPointL &oNodeValue)
//	{
//		WriteNodeBegin(wsNameNode);
//		WriteTPointL(oNodeValue);
//		WriteNodeEnd(wsNameNode);
//	}

	void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TPointS &oNodeValue)
	{
		WriteNodeBegin(wsNameNode);
		WriteTPointS(oNodeValue);
		WriteNodeEnd(wsNameNode);
	}

	void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmfPlusPointF &oNodeValue)
	{
			WriteNodeBegin(wsNameNode);
			WriteTEmfPlusPointF(oNodeValue);
			WriteNodeEnd(wsNameNode);
	}

	void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmrTextW &oNodeValue)
	{
		WriteNodeBegin(wsNameNode);
		WriteTEmrText(oNodeValue);
		WriteNodeEnd(wsNameNode);
	}
	
	void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TEmrTextA &oNodeValue)
	{
		WriteNodeBegin(wsNameNode);
		WriteTEmrText(oNodeValue);
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

	void CXmlOutput::WriteNode(const std::wstring &wsNameNode, const TRegionDataHeader &oRegionDataHeader, const std::vector<TRectL> &arRects)
	{
			WriteNodeBegin(wsNameNode);
				WriteTRegionDataHeader(oRegionDataHeader);
				for (unsigned int unIndex = 0; unIndex < arRects.size(); ++unIndex)
						WriteNode(L"Rect" + std::to_wstring(unIndex + 1), arRects[unIndex]);
			WriteNodeEnd(wsNameNode);
	}

	void CXmlOutput::WriteTRectL(const TRectL &oTRectL)
	{
		WriteNode(L"Left",     oTRectL.Left);
		WriteNode(L"Top",      oTRectL.Top);
		WriteNode(L"Right",    oTRectL.Right);
		WriteNode(L"Bottom",   oTRectL.Bottom);
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

	void CXmlOutput::WriteTSizeL(const TSizeL &oTSizeL)
	{
		WriteNode(L"cX", oTSizeL.X);
		WriteNode(L"cY", oTSizeL.Y);
	}

	void CXmlOutput::WriteTEmfAlphaBlend(const TEmfAlphaBlend &oTEmfAlphaBlend)
	{
		WriteNode(L"Bounds",    oTEmfAlphaBlend.oBounds);
		WriteNode(L"xDest",     oTEmfAlphaBlend.nXDest);
		WriteNode(L"yDest",     oTEmfAlphaBlend.nYDest);
		WriteNode(L"cxDest",    oTEmfAlphaBlend.nCxDest);
		WriteNode(L"cyDest",    oTEmfAlphaBlend.nCyDest);

			WriteNodeBegin(L"BLENDFUNCTION");
			WriteNode(L"BlendOperation",    oTEmfAlphaBlend.uchBlendOperation);
			WriteNode(L"BlendFlags",        oTEmfAlphaBlend.uchBlendFlags);
			WriteNode(L"SrcConstantAlpha",  oTEmfAlphaBlend.uchSrcConstantAlpha);
			WriteNode(L"AlphaFormat",       oTEmfAlphaBlend.uchAlphaFormat);
			WriteNodeEnd(L"BLENDFUNCTION");

		WriteNode(L"cxSrc",    oTEmfAlphaBlend.nCxSrc);
		WriteNode(L"cySrc",    oTEmfAlphaBlend.nCySrc);

		WriteNode(L"XFormSrc",      oTEmfAlphaBlend.oXformSrc);
		WriteNode(L"BkColorSrc",    oTEmfAlphaBlend.oBkColor);
		WriteNode(L"UsageSrc",      oTEmfAlphaBlend.unUsageSrc);
		WriteNode(L"offBmiSrc",     oTEmfAlphaBlend.unOffBmiSrc);
		WriteNode(L"cbBmiSrc",      oTEmfAlphaBlend.unCbBmiSrc);
		WriteNode(L"offBitsSrc",    oTEmfAlphaBlend.unOffBitsSrc);
		WriteNode(L"cbBitsSrc",     oTEmfAlphaBlend.unCbBitsSrc);
		WriteNode(L"cxSrc",         oTEmfAlphaBlend.nCxSrc);
		WriteNode(L"cySrc",         oTEmfAlphaBlend.nCySrc);
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

	void CXmlOutput::WriteTRGBA(const TRGBA &oTRGBA)
	{
		WriteNode(L"Red",       oTRGBA.r);
		WriteNode(L"Green",     oTRGBA.g);
		WriteNode(L"Blue",      oTRGBA.b);
		WriteNode(L"Reserved",  oTRGBA.a);
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
		WriteNode(L"Bounds",                oTEmfStretchDIBITS.oBounds);
		WriteNode(L"xDest",                 oTEmfStretchDIBITS.nXDest);
		WriteNode(L"yDest",                 oTEmfStretchDIBITS.nYDest);
		WriteNode(L"xSrc",                  oTEmfStretchDIBITS.nXSrc);
		WriteNode(L"ySrc",                  oTEmfStretchDIBITS.nYSrc);
		WriteNode(L"cxSrc",                 oTEmfStretchDIBITS.nCxSrc);
		WriteNode(L"cySrc",                 oTEmfStretchDIBITS.nCySrc);
		WriteNode(L"offBmiSrc",             oTEmfStretchDIBITS.unOffBmiSrc);
		WriteNode(L"cbBmiSrc",              oTEmfStretchDIBITS.unCbBmiSrc);
		WriteNode(L"offBitsSrc",            oTEmfStretchDIBITS.unOffBitsSrc);
		WriteNode(L"cbBitsSrc",             oTEmfStretchDIBITS.unCbBitsSrc);
		WriteNode(L"UsageSrc",              oTEmfStretchDIBITS.unUsageSrc);
		WriteNode(L"BitBltRasterOperation", oTEmfStretchDIBITS.unBitBltRasterOperation);
		WriteNode(L"cxDest",                oTEmfStretchDIBITS.nCxDest);
		WriteNode(L"cyDest",                oTEmfStretchDIBITS.nCyDest);
	}

	void CXmlOutput::WriteTEmfBitBlt(const TEmfBitBlt &oTEmfBitBlt)
	{
		WriteNode(L"Bounds",                oTEmfBitBlt.oBounds);
		WriteNode(L"xDest",                 oTEmfBitBlt.nXDest);
		WriteNode(L"yDest",                 oTEmfBitBlt.nYDest);
		WriteNode(L"cxDest",                oTEmfBitBlt.nCxDest);
		WriteNode(L"cyDest",                oTEmfBitBlt.nCyDest);
		WriteNode(L"BitBltRasterOperation", oTEmfBitBlt.unBitBltRasterOperation);
		WriteNode(L"xSrc",                  oTEmfBitBlt.nXSrc);
		WriteNode(L"ySrc",                  oTEmfBitBlt.nYSrc);
		WriteNode(L"XFormSrc",              oTEmfBitBlt.oXfromSrc);
		WriteNode(L"BkColorSrc",            oTEmfBitBlt.oBkColorSrc);
		WriteNode(L"UsageSrc",              oTEmfBitBlt.unUsageSrc);
		WriteNode(L"offBmiSrc",             oTEmfBitBlt.unOffBmiSrc);
		WriteNode(L"cbBmiSrc",              oTEmfBitBlt.unCbBmiSrc);
		WriteNode(L"offBitsSrc",            oTEmfBitBlt.unOffBitsSrc);
		WriteNode(L"cbBitsSrc",             oTEmfBitBlt.unCbBitsSrc);
	}

	void CXmlOutput::WriteTEmfSetDiBitsToDevice(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice)
	{
		WriteNode(L"Bounds",        oTEmfSetDiBitsToDevice.oBounds);
		WriteNode(L"xDest",         oTEmfSetDiBitsToDevice.nXDest);
		WriteNode(L"yDest",         oTEmfSetDiBitsToDevice.nYDest);
		WriteNode(L"xSrc",          oTEmfSetDiBitsToDevice.nXSrc);
		WriteNode(L"ySrc",          oTEmfSetDiBitsToDevice.nYSrc);
		WriteNode(L"cxSrc",         oTEmfSetDiBitsToDevice.nCxSrc);
		WriteNode(L"cySrc",         oTEmfSetDiBitsToDevice.nCySrc);
		WriteNode(L"offBmiSrc",     oTEmfSetDiBitsToDevice.unOffBmiSrc);
		WriteNode(L"cbBmiSrc",      oTEmfSetDiBitsToDevice.unCbBmiSrc);
		WriteNode(L"offBitsSrc",    oTEmfSetDiBitsToDevice.unOffBitsSrc);
		WriteNode(L"cbBitsSrc",     oTEmfSetDiBitsToDevice.unCbBitsSrc);
		WriteNode(L"UsageSrc",      oTEmfSetDiBitsToDevice.unUsageSrc);
		WriteNode(L"iStartScan",    oTEmfSetDiBitsToDevice.unIStartScan);
		WriteNode(L"cScans",        oTEmfSetDiBitsToDevice.unCScans);
	}

	void CXmlOutput::WriteTEmfStretchBLT(const TEmfStretchBLT &oTEmfStretchBLT)
	{
		WriteNode(L"Bounds",                oTEmfStretchBLT.oBounds);
		WriteNode(L"xDest",                 oTEmfStretchBLT.nXDest);
		WriteNode(L"yDest",                 oTEmfStretchBLT.nYDest);
		WriteNode(L"cxDest",                oTEmfStretchBLT.nCxDest);
		WriteNode(L"cyDest",                oTEmfStretchBLT.nCyDest);
		WriteNode(L"BitBltRasterOperation", oTEmfStretchBLT.unBitBltRasterOperation);
		WriteNode(L"xSrc",                  oTEmfStretchBLT.nXSrc);
		WriteNode(L"ySrc",                  oTEmfStretchBLT.nYSrc);
		WriteNode(L"XFormSrc",              oTEmfStretchBLT.oXformSrc);
		WriteNode(L"BkColorSrc",            oTEmfStretchBLT.oBkColorSrc);
		WriteNode(L"UsageSrc",              oTEmfStretchBLT.unUsageSrc);
		WriteNode(L"offBmiSrc",             oTEmfStretchBLT.unOffBmiSrc);
		WriteNode(L"cbBmiSrc",              oTEmfStretchBLT.unCbBmiSrc);
		WriteNode(L"offBitsSrc",            oTEmfStretchBLT.unOffBitsSrc);
		WriteNode(L"cbBitsSrc",             oTEmfStretchBLT.unCbBitsSrc);
		WriteNode(L"cxSrc",                 oTEmfStretchBLT.nCxSrc);
		WriteNode(L"cySrc",                 oTEmfStretchBLT.nCySrc);
	}

	void CXmlOutput::WriteTEmfDibPatternBrush(const TEmfDibPatternBrush &oTEmfDibPatternBrush)
	{
		WriteNode(L"Usage",   oTEmfDibPatternBrush.unUsage);
		WriteNode(L"offBmi",  32/*oTEmfDibPatternBrush.offBmi*/);
		WriteNode(L"cbBmi",   oTEmfDibPatternBrush.unCbBmi);
		WriteNode(L"offBits", 32 + oTEmfDibPatternBrush.unCbBmi/*oTEmfDibPatternBrush.offBits*/);
		WriteNode(L"cbBits",  oTEmfDibPatternBrush.unCbBits);
	}

	void CXmlOutput::WriteTEmfExtTextoutA(const TEmfExtTextoutA &oTEmfExtTextoutA)
	{
		WriteNode(L"Bounds",        oTEmfExtTextoutA.oBounds);
		WriteNode(L"iGraphicsMode", oTEmfExtTextoutA.unIGraphicsMode);
		WriteNode(L"exScale",       oTEmfExtTextoutA.dExScale);
		WriteNode(L"eyScale",       oTEmfExtTextoutA.dEyScale);
		WriteNode(L"EmrText",       oTEmfExtTextoutA.oEmrText);
	}

	void CXmlOutput::WriteTEmrText(const TEmrTextW &oTEmfEmrText)
	{
		WriteNode(L"Reference", oTEmfEmrText.oReference);
		WriteNode(L"Chars",     oTEmfEmrText.unChars);
		WriteNode(L"offString", 76/*oTEmfEmrText.offString*/);
		WriteNode(L"Options",   oTEmfEmrText.unOptions);
		WriteNode(L"Rectangle", oTEmfEmrText.oRectangle);
		WriteNode(L"offDx",     76 + oTEmfEmrText.unChars * 2 + (oTEmfEmrText.unChars % 2) * 2/*oTEmfEmrText.offDx*/);

		if (oTEmfEmrText.unChars == 0)
				return;

		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfEmrText.pOutputString, oTEmfEmrText.unChars);

		WriteNode(L"Text",      StringNormalization(wsText));
		WriteNode(L"Dx",        oTEmfEmrText.pOutputDx, oTEmfEmrText.unChars);
	}

	void CXmlOutput::WriteTEmrText(const TEmrTextA &oTEmfEmrText)
	{
		WriteNode(L"Reference", oTEmfEmrText.oReference);
		WriteNode(L"Chars",     oTEmfEmrText.unChars);
		WriteNode(L"offString", 76/*oTEmfEmrText.offString*/);
		WriteNode(L"Options",   oTEmfEmrText.unOptions);
		WriteNode(L"Rectangle", oTEmfEmrText.oRectangle);
		WriteNode(L"offDx",     76 + oTEmfEmrText.unChars * 2 + (oTEmfEmrText.unChars % 2) * 2/*oTEmfEmrText.offDx*/);

		if (oTEmfEmrText.unChars == 0)
				return;

		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromSingleByteString((unsigned char*)oTEmfEmrText.pOutputString, oTEmfEmrText.unChars);

		WriteNode(L"Text",      StringNormalization(wsText));
		WriteNode(L"Dx",        oTEmfEmrText.pOutputDx, oTEmfEmrText.unChars);
	}

	void CXmlOutput::WriteTPointL(const TPointL &oTPointL)
	{
		WriteNode(L"X", oTPointL.X);
		WriteNode(L"Y", oTPointL.Y);
	}

	void CXmlOutput::WriteTPointS(const TPointS &oTPointS)
	{
		WriteNode(L"X", oTPointS.X);
		WriteNode(L"Y", oTPointS.Y);
	}

	void CXmlOutput::WriteTEmfPlusPointF(const TEmfPlusPointF &oTEmfPlusPointF)
	{
			WriteNode(L"X", oTEmfPlusPointF.X);
			WriteNode(L"Y", oTEmfPlusPointF.Y);
	}

	void CXmlOutput::WriteTEmfExtTextoutW(const TEmfExtTextoutW &oTEmfExtTextoutW)
	{
		WriteNode(L"Bounds",        oTEmfExtTextoutW.oBounds);
		WriteNode(L"iGraphicsMode", oTEmfExtTextoutW.unIGraphicsMode);
		WriteNode(L"exScale",       oTEmfExtTextoutW.dExScale);
		WriteNode(L"eyScale",       oTEmfExtTextoutW.dEyScale);
		WriteNode(L"EmrText",       oTEmfExtTextoutW.oEmrText);
	}

	void CXmlOutput::WriteTEmfSmallTextout(const TEmfSmallTextout &oTEmfSmallTextout)
	{
		WriteNode(L"x",             oTEmfSmallTextout.nX);
		WriteNode(L"y",             oTEmfSmallTextout.nY);
		WriteNode(L"cChars",        oTEmfSmallTextout.unCChars);
		WriteNode(L"fuOptions",     oTEmfSmallTextout.unFuOptions);
		WriteNode(L"iGraphicsMode", oTEmfSmallTextout.unIGraphicsMode);
		WriteNode(L"exScale",       oTEmfSmallTextout.dExScale);
		WriteNode(L"eyScale",       oTEmfSmallTextout.dEyScale);
		WriteNode(L"Bounds",        oTEmfSmallTextout.oBounds);

		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfSmallTextout.pTextString, oTEmfSmallTextout.unCChars);

		WriteNode(L"Text",          StringNormalization(wsText));
	}

	void CXmlOutput::WriteTRegionDataHeader(const TRegionDataHeader &oTRegionDataHeader)
	{
			WriteNode(L"CountRects", oTRegionDataHeader.unCountRects);
			WriteNode(L"Bounds ",    oTRegionDataHeader.oBounds);
	}

	void CXmlOutput::WriteEmfLogBrushEx(const CEmfLogBrushEx &oEmfLogBrushEx)
	{
		WriteNode(L"BrushStyle",    oEmfLogBrushEx.unBrushStyle);
		WriteNode(L"Color",         oEmfLogBrushEx.oColor);
		WriteNode(L"BrushHatch",    oEmfLogBrushEx.unBrushHatch);
	}

	void CXmlOutput::WriteEmfLogFont(CEmfLogFont &oEmfLogFont)
	{
		if (!oEmfLogFont.IsFixedLength())
			WriteNodeBegin(L"LogFontExDv");

		WriteNodeBegin(L"LogFont");
		WriteNode(L"Height",            oEmfLogFont.oLogFontEx.oLogFont.nHeight);
		WriteNode(L"Width",             oEmfLogFont.oLogFontEx.oLogFont.nWidth);
		WriteNode(L"Escapement",        oEmfLogFont.oLogFontEx.oLogFont.nEscapement);
		WriteNode(L"Orientation",       oEmfLogFont.oLogFontEx.oLogFont.nOrientation);
		WriteNode(L"Weight",            oEmfLogFont.oLogFontEx.oLogFont.nWeight);
		WriteNode(L"Italic",            oEmfLogFont.oLogFontEx.oLogFont.uchItalic);
		WriteNode(L"Underline",         oEmfLogFont.oLogFontEx.oLogFont.uchUnderline);
		WriteNode(L"StrikeOut",         oEmfLogFont.oLogFontEx.oLogFont.uchStrikeOut);
		WriteNode(L"CharSet",           oEmfLogFont.oLogFontEx.oLogFont.uchCharSet);
		WriteNode(L"OutPrecision",      oEmfLogFont.oLogFontEx.oLogFont.uchOutPrecision);
		WriteNode(L"ClipPrecision",     oEmfLogFont.oLogFontEx.oLogFont.uchClipPrecision);
		WriteNode(L"Quality",           oEmfLogFont.oLogFontEx.oLogFont.uchQuality);
		WriteNode(L"PitchAndFamily",    oEmfLogFont.oLogFontEx.oLogFont.uchPitchAndFamily);
		WriteNode(L"FaceName",          StringNormalization(NSStringExt::CConverter::GetUnicodeFromUTF16(oEmfLogFont.oLogFontEx.oLogFont.ushFaceName, 32)));
		WriteNodeEnd(L"LogFont");

		WriteNode(L"FullName",	StringNormalization(NSStringExt::CConverter::GetUnicodeFromUTF16(oEmfLogFont.oLogFontEx.ushFullName, 64)));
		WriteNode(L"Style",	StringNormalization(NSStringExt::CConverter::GetUnicodeFromUTF16(oEmfLogFont.oLogFontEx.ushStyle,        32)));
		WriteNode(L"Script",	StringNormalization(NSStringExt::CConverter::GetUnicodeFromUTF16(oEmfLogFont.oLogFontEx.ushScript,   32)));

		if (!oEmfLogFont.IsFixedLength())
		{
			WriteNodeBegin(L"DesignVector");
			WriteNode(L"Signature",  oEmfLogFont.oDesignVector.unSignature);
	//		WriteNode(L"NumAxes",    oEmfLogFont.DesignVector.NumAxes);
	
			for (unsigned int i = 0; i <oEmfLogFont.oDesignVector.unNumAxes; ++i)
				WriteNode(L"Value" + std::to_wstring(i + 1),	oEmfLogFont.oDesignVector.pValues[i]);
	
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
		WriteNode(L"NumberOfEntries",   oEmfLogPalette.ushNumberOfEntries);

		for(unsigned int i = 0; i < oEmfLogPalette.ushNumberOfEntries; ++i)
		{
			WriteNodeBegin(L"LogPaletteEntry" + std::to_wstring(i + 1));
			WriteNode(L"Reserved",	oEmfLogPalette.pPaletteEntries[i].uchReserved);
			WriteNode(L"Blue",	oEmfLogPalette.pPaletteEntries[i].uchBlue);
			WriteNode(L"Green",	oEmfLogPalette.pPaletteEntries[i].uchGreen);
			WriteNode(L"Red",	oEmfLogPalette.pPaletteEntries[i].uchRed);
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

	void CXmlOutput::operator>>(TEmfHeader &oTHeader)
	{
		*this >> oTHeader.oBounds;
		*this >> oTHeader.oFrame;
		*this >> oTHeader.ulSignature;
		*this >> oTHeader.ulVersion;
		*this >> oTHeader.ulSize;
		*this >> oTHeader.ulRecords;
		*this >> oTHeader.ushObjects;
		*this >> oTHeader.ushReserved;
		*this >> oTHeader.ulSizeDescription;
		*this >> oTHeader.ulOffsetDescription;
		*this >> oTHeader.ulPalEntries;
		*this >> oTHeader.oDevice;
		*this >> oTHeader.oMillimeters;
//        *this >> oTEmfHeader.oFrameToBounds;
			//        *this >> oTEmfHeader.oFramePx;
	}

	void CXmlOutput::ReadPoint(TPointL &oPoint)
	{
		*this >> oPoint.X;
		*this >> oPoint.Y;
	}

	void CXmlOutput::ReadPoint(TPointS &oPoint)
	{
		*this >> oPoint.X;
		*this >> oPoint.Y;
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
		*this >> oTEmfAlphaBlend.oBounds;
		*this >> oTEmfAlphaBlend.nXDest;
		*this >> oTEmfAlphaBlend.nYDest;
		*this >> oTEmfAlphaBlend.nCxDest;
		*this >> oTEmfAlphaBlend.nCyDest;
		//TODO: закончить реализацию при встрече примера

	}

	void CXmlOutput::operator>>(TEmfBitBlt &oTEmfBitBlt)
	{
		*this >> oTEmfBitBlt.oBounds;
		*this >> oTEmfBitBlt.nXDest;
		*this >> oTEmfBitBlt.nYDest;
		*this >> oTEmfBitBlt.nCxDest;
		*this >> oTEmfBitBlt.nCyDest;
		*this >> oTEmfBitBlt.unBitBltRasterOperation;
		*this >> oTEmfBitBlt.nXSrc;
		*this >> oTEmfBitBlt.nYSrc;
		*this >> oTEmfBitBlt.oXfromSrc;
		*this >> oTEmfBitBlt.oBkColorSrc;
		*this >> oTEmfBitBlt.unUsageSrc;
		*this >> oTEmfBitBlt.unOffBmiSrc;
		*this >> oTEmfBitBlt.unCbBmiSrc;
		*this >> oTEmfBitBlt.unOffBitsSrc;
		*this >> oTEmfBitBlt.unCbBitsSrc;
	}

	void CXmlOutput::operator>>(TEmfStretchDIBITS &oTEmfStretchDIBITS)
	{
		*this >> oTEmfStretchDIBITS.oBounds;
		*this >> oTEmfStretchDIBITS.nXDest;
		*this >> oTEmfStretchDIBITS.nYDest;
		*this >> oTEmfStretchDIBITS.nXSrc;
		*this >> oTEmfStretchDIBITS.nYSrc;
		*this >> oTEmfStretchDIBITS.nCxSrc;
		*this >> oTEmfStretchDIBITS.nCySrc;
		*this >> oTEmfStretchDIBITS.unOffBmiSrc;
		*this >> oTEmfStretchDIBITS.unCbBmiSrc;
		*this >> oTEmfStretchDIBITS.unOffBitsSrc;
		*this >> oTEmfStretchDIBITS.unCbBitsSrc;
		*this >> oTEmfStretchDIBITS.unUsageSrc;
		*this >> oTEmfStretchDIBITS.unBitBltRasterOperation;
		*this >> oTEmfStretchDIBITS.nCxDest;
		*this >> oTEmfStretchDIBITS.nCyDest;
	}

	void CXmlOutput::operator>>(TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice)
	{
		*this >> oTEmfSetDiBitsToDevice.oBounds;
		*this >> oTEmfSetDiBitsToDevice.nXDest;
		*this >> oTEmfSetDiBitsToDevice.nYDest;
		*this >> oTEmfSetDiBitsToDevice.nXSrc;
		*this >> oTEmfSetDiBitsToDevice.nYSrc;
		*this >> oTEmfSetDiBitsToDevice.nCxSrc;
		*this >> oTEmfSetDiBitsToDevice.nCySrc;
		*this >> oTEmfSetDiBitsToDevice.unOffBmiSrc;
		*this >> oTEmfSetDiBitsToDevice.unCbBmiSrc;
		*this >> oTEmfSetDiBitsToDevice.unOffBitsSrc;
		*this >> oTEmfSetDiBitsToDevice.unCbBitsSrc;
		*this >> oTEmfSetDiBitsToDevice.unUsageSrc;
		*this >> oTEmfSetDiBitsToDevice.unIStartScan;
		*this >> oTEmfSetDiBitsToDevice.unCScans;
	}

	void CXmlOutput::operator>>(TEmfStretchBLT &oTEmfStretchBLT)
	{
		*this >> oTEmfStretchBLT.oBounds;
		*this >> oTEmfStretchBLT.nXDest;
		*this >> oTEmfStretchBLT.nYDest;
		*this >> oTEmfStretchBLT.nCxDest;
		*this >> oTEmfStretchBLT.nCyDest;
		*this >> oTEmfStretchBLT.unBitBltRasterOperation;
		*this >> oTEmfStretchBLT.nXSrc;
		*this >> oTEmfStretchBLT.nYSrc;
		*this >> oTEmfStretchBLT.oXformSrc;
		*this >> oTEmfStretchBLT.oBkColorSrc;
		*this >> oTEmfStretchBLT.unUsageSrc;
		*this >> oTEmfStretchBLT.unOffBmiSrc;
		*this >> oTEmfStretchBLT.unCbBmiSrc;
		*this >> oTEmfStretchBLT.unOffBitsSrc;
		*this >> oTEmfStretchBLT.unCbBitsSrc;
		*this >> oTEmfStretchBLT.nCxSrc;
		*this >> oTEmfStretchBLT.nCySrc;
	}

	void CXmlOutput::operator>>(CEmfLogFont &oCEmfLogFont)
	{
		if (!oCEmfLogFont.IsFixedLength())
			m_pXmlLiteReader->ReadNextNode();

		*this >> oCEmfLogFont.oLogFontEx;

		if (!oCEmfLogFont.IsFixedLength())
			*this >> oCEmfLogFont.oDesignVector;
	}

	void CXmlOutput::operator>>(TEmfLogFontEx&  oTEmfLogFontEx)
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		*this >> oTEmfLogFontEx.oLogFont;
		*this >> oTEmfLogFontEx.ushFullName;
		*this >> oTEmfLogFontEx.ushStyle;
		*this >> oTEmfLogFontEx.ushScript;
	}

	void CXmlOutput::operator>>(TEmfLogFont&  oTEmfLogFont)
	{
		*this >> oTEmfLogFont.nHeight;
		*this >> oTEmfLogFont.nWidth;
		*this >> oTEmfLogFont.nEscapement;
		*this >> oTEmfLogFont.nOrientation;
		*this >> oTEmfLogFont.nWeight;
		*this >> oTEmfLogFont.uchItalic;
		*this >> oTEmfLogFont.uchUnderline;
		*this >> oTEmfLogFont.uchStrikeOut;
		*this >> oTEmfLogFont.uchCharSet;
		*this >> oTEmfLogFont.uchOutPrecision;
		*this >> oTEmfLogFont.uchClipPrecision;
		*this >> oTEmfLogFont.uchQuality;
		*this >> oTEmfLogFont.uchPitchAndFamily;
		*this >> oTEmfLogFont.ushFaceName;
	}

	void CXmlOutput::operator>>(TEmfDesignVector&  oTEmfDesignVector)
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		*this >> oTEmfDesignVector.unSignature;
//        *this >> oTEmfDesignVector.NumAxes;

		unsigned int unDepth = m_pXmlLiteReader->GetDepth();
		std::vector<int> arValues;

		while (m_pXmlLiteReader->ReadNextSiblingNode(unDepth - 1))
				arValues.push_back(std::stoi(m_pXmlLiteReader->GetText2()));

		oTEmfDesignVector.unNumAxes = arValues.size();

		if (!arValues.empty())
		{
				unsigned int unSize = arValues.size();
				oTEmfDesignVector.pValues = new int[unSize];

				for (unsigned int unIndex = 0; unIndex < unSize; ++unIndex)
					oTEmfDesignVector.pValues[unIndex] = arValues[unIndex];
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

		*this >> oTEmfLogPaletteEntry.uchReserved;
		*this >> oTEmfLogPaletteEntry.uchBlue;
		*this >> oTEmfLogPaletteEntry.uchGreen;
		*this >> oTEmfLogPaletteEntry.uchRed;
	}

	void CXmlOutput::operator>>(TEmfExtTextoutW &oTEmfExtTextoutW)
	{
		*this >> oTEmfExtTextoutW.oBounds;
		*this >> oTEmfExtTextoutW.unIGraphicsMode;
		*this >> oTEmfExtTextoutW.dExScale;
		*this >> oTEmfExtTextoutW.dEyScale;
		*this >> oTEmfExtTextoutW.oEmrText;
	}

	void CXmlOutput::operator>>(TEmfExtTextoutA &oTEmfExtTextoutA)
	{
		*this >> oTEmfExtTextoutA.oBounds;
		*this >> oTEmfExtTextoutA.unIGraphicsMode;
		*this >> oTEmfExtTextoutA.dExScale;
		*this >> oTEmfExtTextoutA.dEyScale;
		*this >> oTEmfExtTextoutA.oEmrText;
	}

	void CXmlOutput::operator>>(TEmfSmallTextout &oTEmfSmallTextout)
	{
		*this >> oTEmfSmallTextout.nX;
		*this >> oTEmfSmallTextout.nY;
		*this >> oTEmfSmallTextout.unCChars;
		*this >> oTEmfSmallTextout.unFuOptions;
		*this >> oTEmfSmallTextout.unIGraphicsMode;
		*this >> oTEmfSmallTextout.dExScale;
		*this >> oTEmfSmallTextout.dEyScale;
		*this >> oTEmfSmallTextout.oBounds;

		oTEmfSmallTextout.pTextString = new unsigned short[oTEmfSmallTextout.unCChars];
		*this >> (unsigned short*)oTEmfSmallTextout.pTextString;
	}

	void CXmlOutput::operator>>(TEmrTextW &oTEmfEmrText)
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		*this >> oTEmfEmrText.oReference;
		*this >> oTEmfEmrText.unChars;
		*this >> oTEmfEmrText.unOffString;
		*this >> oTEmfEmrText.unOptions;
		*this >> oTEmfEmrText.oRectangle;
		*this >> oTEmfEmrText.unOffDx;

		if (oTEmfEmrText.unChars == 0)
				return;

		oTEmfEmrText.pOutputString   = new unsigned short[oTEmfEmrText.unChars];
		oTEmfEmrText.pOutputDx       = new unsigned int[oTEmfEmrText.unChars];

		((unsigned short*)oTEmfEmrText.pOutputString)[oTEmfEmrText.unChars - 1] = 0x00;
		*this >> (unsigned short*)oTEmfEmrText.pOutputString;

		ReadDx(oTEmfEmrText.pOutputDx, oTEmfEmrText.unChars);
	}
	
	void CXmlOutput::operator>>(TEmrTextA &oTEmrText)
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		*this >> oTEmrText.oReference;
		*this >> oTEmrText.unChars;
		*this >> oTEmrText.unOffString;
		*this >> oTEmrText.unOptions;
		*this >> oTEmrText.oRectangle;
		*this >> oTEmrText.unOffDx;

		if (oTEmrText.unChars == 0)
			return;

		oTEmrText.pOutputString   = new unsigned char[oTEmrText.unChars];
		oTEmrText.pOutputDx       = new unsigned int[oTEmrText.unChars];

		((unsigned char*)oTEmrText.pOutputString)[oTEmrText.unChars - 1] = 0x00;
		*this >> (unsigned char*)oTEmrText.pOutputString;

		ReadDx(oTEmrText.pOutputDx, oTEmrText.unChars);
	}

	void CXmlOutput::operator>>(TEmfDibPatternBrush &oTEmfDibPatternBrush)
	{
		*this >> oTEmfDibPatternBrush.unUsage;
		*this >> oTEmfDibPatternBrush.unOffBmi;
		*this >> oTEmfDibPatternBrush.unCbBmi;
		*this >> oTEmfDibPatternBrush.unOffBits;
		*this >> oTEmfDibPatternBrush.unCbBits;
	}

	void CXmlOutput::operator>>(CEmfLogPalette &oCEmfLogPalette)
	{
		unsigned int unVersion;
		*this >> unVersion;
		*this >> oCEmfLogPalette.ushNumberOfEntries;
		if (oCEmfLogPalette.ushNumberOfEntries > 0)
		{
			oCEmfLogPalette.pPaletteEntries = new TEmfLogPaletteEntry[oCEmfLogPalette.ushNumberOfEntries];

			for (unsigned int i = 0; i < oCEmfLogPalette.ushNumberOfEntries; ++i)
				*this >> oCEmfLogPalette.pPaletteEntries[i];
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

	void CXmlOutput::operator>>(TRGBA &oTRGBA)
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		*this >> oTRGBA.r;
		*this >> oTRGBA.g;
		*this >> oTRGBA.b;
		*this >> oTRGBA.a;
	}


	void CXmlOutput::operator>>(CEmfLogBrushEx &oCEmfLogBrushEx)
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		*this >> oCEmfLogBrushEx.unBrushStyle;
		*this >> oCEmfLogBrushEx.oColor;
		*this >> oCEmfLogBrushEx.unBrushHatch;
	}

	void CXmlOutput::operator>>(TRectL &oTRectL)
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		*this >> oTRectL.Left;
		*this >> oTRectL.Top;
		*this >> oTRectL.Right;
		*this >> oTRectL.Bottom;
	}

	void CXmlOutput::operator>>(std::vector<TPointL> &arPoints)
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		unsigned int unDepth = m_pXmlLiteReader->GetDepth();

		do
		{
			TPointL oPoint;
			ReadPoint(oPoint);
			arPoints.push_back(oPoint);
		}
		while (m_pXmlLiteReader->ReadNextSiblingNode(unDepth - 1));
	}

	void CXmlOutput::operator>>(std::vector<TPointS> &arPoints)
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		unsigned int unDepth = m_pXmlLiteReader->GetDepth();

		do
		{
			TPointS oPoint;
			ReadPoint(oPoint);
			arPoints.push_back(oPoint);
		}
		while (m_pXmlLiteReader->ReadNextSiblingNode(unDepth - 1));
	}

	void CXmlOutput::operator>>(std::vector<TRectL> &arRects)
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		unsigned int unDepth = m_pXmlLiteReader->GetDepth();

		do
		{
			TRectL oRect;
			*this >> oRect;
			arRects.push_back(oRect);
		}
		while (m_pXmlLiteReader->ReadNextSiblingNode(unDepth - 1));
	}

	void CXmlOutput::operator>>(std::vector<std::vector<TPointL>> &arPoints)
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		unsigned int unDepth = m_pXmlLiteReader->GetDepth();

		do
		{
			std::vector<TPointL> arTempPoints;
			*this >> arTempPoints;
			arPoints.push_back(arTempPoints);
		}
		while (m_pXmlLiteReader->ReadNextSiblingNode(unDepth - 1));
	}

	void CXmlOutput::operator>>(std::vector<std::vector<TPointS>> &arPoints)
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		unsigned int unDepth = m_pXmlLiteReader->GetDepth();

		do
		{
			std::vector<TPointS> arTempPoints;
			*this >> arTempPoints;
			arPoints.push_back(arTempPoints);
		}
		while (m_pXmlLiteReader->ReadNextSiblingNode(unDepth - 1));
	}

//	void CXmlOutput::operator>>(TSizeL &oTSizeL)
//	{
//		if (!m_pXmlLiteReader->ReadNextNode())
//			return;

//		*this >> oTSizeL.X;
//		*this >> oTSizeL.Y;
//	}

	void CXmlOutput::operator>>(TPointL &oTPointL)
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		ReadPoint(oTPointL);
	}

	void CXmlOutput::operator>>(TPointS &oTPointS)
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		ReadPoint(oTPointS);
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

	void CXmlOutput::operator>>(unsigned char aruchValue[])
	{
		if (!m_pXmlLiteReader->ReadNextNode())
			return;

		const std::wstring wsValue = m_pXmlLiteReader->GetText2();

		for (unsigned int i = 0; i < wsValue.length(); ++i)
			aruchValue[i] = wsValue[i];
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
