#include "CEmfInterpretatorXml.h"
#include <typeinfo>

namespace MetaFile
{
	CEmfInterpretatorXml::CEmfInterpretatorXml(const wchar_t *wsFilePath) :
		m_pOutputXml(new CXmlOutput(TypeXmlOutput::IsWriter)),
		m_wsXmlFilePath(wsFilePath){}

		CEmfInterpretatorXml::CEmfInterpretatorXml(const CEmfInterpretatorXml &oInterpretator)
				: m_pOutputXml(NULL),
				m_wsXmlFilePath(oInterpretator.m_wsXmlFilePath)
		{
				if (NULL == oInterpretator.m_pOutputXml)
						return;

				if (oInterpretator.m_pOutputXml->IsWriter())
						m_pOutputXml = new CXmlOutput(TypeXmlOutput::IsWriter);
				else
						m_pOutputXml = new CXmlOutput(TypeXmlOutput::IsReader);
		}

	CEmfInterpretatorXml::~CEmfInterpretatorXml()
	{
		if (NULL != m_pOutputXml)
			delete m_pOutputXml;
	}

	InterpretatorType CEmfInterpretatorXml::GetType() const
	{
		return InterpretatorType::XML;
	}

	void CEmfInterpretatorXml::HANDLE_EMR_HEADER(const TEmfHeader &oTEmfHeader)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_HEADER");

		m_pOutputXml->WriteNode(L"Bounds",		oTEmfHeader.oBounds);
		m_pOutputXml->WriteNode(L"Frame",		oTEmfHeader.oFrame);
		m_pOutputXml->WriteNode(L"Signature",		oTEmfHeader.ulSignature);
		m_pOutputXml->WriteNode(L"Version",		oTEmfHeader.ulVersion);
		m_pOutputXml->WriteNode(L"Size",		oTEmfHeader.ulSize);
		m_pOutputXml->WriteNode(L"Records",		oTEmfHeader.ulRecords);
		m_pOutputXml->WriteNode(L"CountObjects",	oTEmfHeader.ushObjects);
		m_pOutputXml->WriteNode(L"Reserved",		oTEmfHeader.ushReserved);
		m_pOutputXml->WriteNode(L"SizeDescription",	0/*oTEmfHeader.ulSizeDescription*/);
		m_pOutputXml->WriteNode(L"OffsetDescription",	0/*oTEmfHeader.ulOffsetDescription*/);
		m_pOutputXml->WriteNode(L"PalletEntries",	oTEmfHeader.ulPalEntries);
		m_pOutputXml->WriteNode(L"Device",		oTEmfHeader.oDevice);
		m_pOutputXml->WriteNode(L"Millimeters",		oTEmfHeader.oMillimeters);

		m_pOutputXml->WriteNodeEnd(L"EMR_HEADER");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_ALPHABLEND(const TEmfAlphaBlend &oTEmfAlphaBlend, CDataStream &oDataStream)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_ALPHABLEND");
		m_pOutputXml->WriteNode(L"", oTEmfAlphaBlend);

		unsigned int unSize = oTEmfAlphaBlend.cbBitsSrc + oTEmfAlphaBlend.cbBmiSrc;

		if (unSize > 0)
			m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

		m_pOutputXml->WriteNodeEnd(L"EMR_ALPHABLEND");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_STRETCHDIBITS(const TEmfStretchDIBITS &oTEmfStretchDIBITS, CDataStream &oDataStream)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_STRETCHDIBITS");
		m_pOutputXml->WriteNode(L"", oTEmfStretchDIBITS);

		unsigned int unSize = oTEmfStretchDIBITS.cbBitsSrc + oTEmfStretchDIBITS.cbBmiSrc;

		if (unSize > 0)
			m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

		m_pOutputXml->WriteNodeEnd(L"EMR_STRETCHDIBITS");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_BITBLT(const TEmfBitBlt &oTEmfBitBlt, CDataStream &oDataStream)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_BITBLT");
		m_pOutputXml->WriteNode(L"", oTEmfBitBlt);

		unsigned int unSize = oTEmfBitBlt.cbBitsSrc + oTEmfBitBlt.cbBmiSrc;

		if (unSize > 0)
			m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

		m_pOutputXml->WriteNodeEnd(L"EMR_BITBLT");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice, CDataStream &oDataStream)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETDIBITSTODEVICE");
		m_pOutputXml->WriteNode(L"", oTEmfSetDiBitsToDevice);

		unsigned int unSize = oTEmfSetDiBitsToDevice.cbBitsSrc + oTEmfSetDiBitsToDevice.cbBmiSrc;

		if (unSize > 0)
			m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

		m_pOutputXml->WriteNodeEnd(L"EMR_SETDIBITSTODEVICE");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_STRETCHBLT(const TEmfStretchBLT &oTEmfStretchBLT, CDataStream &oDataStream)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_STRETCHBLT");
		m_pOutputXml->WriteNode(L"", oTEmfStretchBLT);

		unsigned int unSize = oTEmfStretchBLT.cbBitsSrc + oTEmfStretchBLT.cbBmiSrc;

		if (unSize > 0)
			m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

		m_pOutputXml->WriteNodeEnd(L"EMR_STRETCHBLT");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_EOF()
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_EOF");
		m_pOutputXml->WriteNode(L"nPalEntries",	    0);
		m_pOutputXml->WriteNode(L"offPalEntries",   0);
		m_pOutputXml->WriteNode(L"SizeLast",	    20);
		m_pOutputXml->WriteNodeEnd(L"EMR_EOF");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SAVEDC()
	{
		m_pOutputXml->WriteNode(L"EMR_SAVEDC");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_RESTOREDC(const int &nIndexDC)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_RESTOREDC");
		m_pOutputXml->WriteNode(L"SavedDC", nIndexDC);
		m_pOutputXml->WriteNodeEnd(L"EMR_RESTOREDC");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_MODIFYWORLDTRANSFORM(const TXForm &oXForm, const unsigned int &unMode)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_MODIFYWORLDTRANSFORM");
		m_pOutputXml->WriteNode(L"XForm",			oXForm);
		m_pOutputXml->WriteNode(L"ModifyWorldTransformMode",	unMode);
		m_pOutputXml->WriteNodeEnd(L"EMR_MODIFYWORLDTRANSFORM");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETWORLDTRANSFORM(const TXForm &oXForm)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETWORLDTRANSFORM");
		m_pOutputXml->WriteNode(L"XForm", oXForm);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETWORLDTRANSFORM");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_CREATEBRUSHINDIRECT(const unsigned int &unBrushIndex, const CEmfLogBrushEx *pBrush)
	{
		if (NULL == pBrush)
			return;

		m_pOutputXml->WriteNodeBegin(L"EMR_CREATEBRUSHINDIRECT");
		m_pOutputXml->WriteNode(L"ihBrush",	unBrushIndex);
		m_pOutputXml->WriteNode(L"LogBrush ",   *pBrush);
		m_pOutputXml->WriteNodeEnd(L"EMR_CREATEBRUSHINDIRECT");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETTEXTCOLOR(const TEmfColor &oColor)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETTEXTCOLOR");
		m_pOutputXml->WriteNode(L"Color", oColor);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETTEXTCOLOR");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SELECTOBJECT(const unsigned int &unObjectIndex)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SELECTOBJECT");
		m_pOutputXml->WriteNode(L"Index", unObjectIndex);
		m_pOutputXml->WriteNodeEnd(L"EMR_SELECTOBJECT");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_EXTCREATEFONTINDIRECTW(const unsigned int &unIndex, CEmfLogFont *oLogFont)
	{
		if (NULL == oLogFont)
			return;

		m_pOutputXml->WriteNodeBegin(L"EMR_EXTCREATEFONTINDIRECTW");
		m_pOutputXml->WriteNode(L"ihFonts", unIndex);
		m_pOutputXml->WriteNode(L"",	    *oLogFont);
		m_pOutputXml->WriteNodeEnd(L"EMR_EXTCREATEFONTINDIRECTW");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETTEXTALIGN(const unsigned int &unAlign)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETTEXTALIGN");
		m_pOutputXml->WriteNode(L"TextAlign", unAlign);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETTEXTALIGN");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETBKMODE(const unsigned int &unBgMode)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETBKMODE");
		m_pOutputXml->WriteNode(L"BgMode", unBgMode);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETBKMODE");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_DELETEOBJECT(const unsigned int &unObjectIndex)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_DELETEOBJECT");
		m_pOutputXml->WriteNode(L"Index", unObjectIndex);
		m_pOutputXml->WriteNodeEnd(L"EMR_DELETEOBJECT");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETMITERLIMIT(const unsigned int &unMeterLimit)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETMITERLIMIT");
		m_pOutputXml->WriteNode(L"MiterLimit", unMeterLimit);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETMITERLIMIT");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_EXTCREATEPEN(const unsigned int &unPenIndex, CEmfLogPen *pPen, const std::vector<unsigned int>& arUnused)
	{
		if (NULL == pPen || arUnused.size() < 2)
			return;

		m_pOutputXml->WriteNodeBegin(L"EMR_EXTCREATEPEN");
		m_pOutputXml->WriteNode(L"ihPen",   unPenIndex);
		m_pOutputXml->WriteNodeBegin(L"LogPenEx");
		m_pOutputXml->WriteNode(L"PenStyle",            pPen->PenStyle);
		m_pOutputXml->WriteNode(L"Width",               pPen->Width);
		m_pOutputXml->WriteNode(L"BrushStyle",          arUnused[0]);
		m_pOutputXml->WriteNode(L"ColorRef",            pPen->Color);
		m_pOutputXml->WriteNode(L"BrushHatch",          arUnused[1]);
		m_pOutputXml->WriteNode(L"NumStyleEntries",     pPen->NumStyleEntries);

		for (unsigned int ulIndex = 0; ulIndex < pPen->NumStyleEntries; ulIndex++)
			m_pOutputXml->WriteNode(L"StyleEntry" + std::to_wstring(ulIndex + 1),  pPen->StyleEntry[ulIndex]);

		m_pOutputXml->WriteNodeEnd(L"LogPenEx");
		m_pOutputXml->WriteNodeEnd(L"EMR_EXTCREATEPEN");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_CREATEPEN(const unsigned int &unPenIndex, const unsigned int &unWidthX,
													const CEmfLogPen *pPen)
	{
		if (NULL == pPen)
			return;

		m_pOutputXml->WriteNodeBegin(L"EMR_CREATEPEN");
		m_pOutputXml->WriteNode(L"ihPen", unPenIndex);
		m_pOutputXml->WriteNodeBegin(L"LogPen");
		m_pOutputXml->WriteNode(L"PenStyle", pPen->PenStyle);
		m_pOutputXml->WriteNode(L"Width", unWidthX);
		m_pOutputXml->WriteNodeEnd(L"LogPen");
		m_pOutputXml->WriteNode(L"COLORREF", pPen->Color);
		m_pOutputXml->WriteNodeEnd(L"EMR_CREATEPEN");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETPOLYFILLMODE(const unsigned int &unFillMode)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETPOLYFILLMODE");
		m_pOutputXml->WriteNode(L"PolygonFillMode", unFillMode);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETPOLYFILLMODE");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_BEGINPATH()
	{
		m_pOutputXml->WriteNode(L"EMR_BEGINPATH");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_ENDPATH()
	{
		m_pOutputXml->WriteNode(L"EMR_ENDPATH");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_CLOSEFIGURE()
	{
		m_pOutputXml->WriteNode(L"EMR_CLOSEFIGURE");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_FLATTENPATH()
	{
		m_pOutputXml->WriteNode(L"EMR_FLATTENPATH");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_WIDENPATH()
	{
		m_pOutputXml->WriteNode(L"EMR_WIDENPATH");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_ABORTPATH()
	{
		m_pOutputXml->WriteNode(L"EMR_ABORTPATH");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_MOVETOEX(const TEmfPointL &oPoint)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_MOVETOEX");
		m_pOutputXml->WriteNode(L"Offset", oPoint);
		m_pOutputXml->WriteNodeEnd(L"EMR_MOVETOEX");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETARCDIRECTION(const unsigned int &unDirection)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETARCDIRECTION");
		m_pOutputXml->WriteNode(L"arcDirection", unDirection);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETARCDIRECTION");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_FILLPATH(const TEmfRectL &oBounds)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_FILLPATH");
		m_pOutputXml->WriteNode(L"Bounds", oBounds);
		m_pOutputXml->WriteNodeEnd(L"EMR_FILLPATH");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETMAPMODE(const unsigned int &unMapMode)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETMAPMODE");
		m_pOutputXml->WriteNode(L"MapMode", unMapMode);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETMAPMODE");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETWINDOWORGEX(const TEmfPointL &oOrigin)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETWINDOWORGEX");
		m_pOutputXml->WriteNode(L"Origin", oOrigin);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETWINDOWORGEX");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETWINDOWEXTEX(const TEmfSizeL &oExtent)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETWINDOWEXTEX");
		m_pOutputXml->WriteNode(L"Extent", oExtent);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETWINDOWEXTEX");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SCALEWINDOWEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SCALEWINDOWEXTEX");
		m_pOutputXml->WriteNode(L"xNum",    nXNum);
		m_pOutputXml->WriteNode(L"nXDenom", nXDenom);
		m_pOutputXml->WriteNode(L"nYNum",   nYNum);
		m_pOutputXml->WriteNode(L"nYDenom", nYDenom);
		m_pOutputXml->WriteNodeEnd(L"EMR_SCALEWINDOWEXTEX");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETVIEWPORTORGEX(const TEmfPointL &oOrigin)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETVIEWPORTORGEX");
		m_pOutputXml->WriteNode(L"Origin", oOrigin);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETVIEWPORTORGEX");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETVIEWPORTEXTEX(const TEmfSizeL &oExtent)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETVIEWPORTEXTEX");
		m_pOutputXml->WriteNode(L"Extent", oExtent);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETVIEWPORTEXTEX");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SCALEVIEWPORTEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SCALEVIEWPORTEXTEX");
		m_pOutputXml->WriteNode(L"xNum",    nXNum);
		m_pOutputXml->WriteNode(L"nXDenom", nXDenom);
		m_pOutputXml->WriteNode(L"nYNum",   nYNum);
		m_pOutputXml->WriteNode(L"nYDenom", nYDenom);
		m_pOutputXml->WriteNodeEnd(L"EMR_SCALEVIEWPORTEXTEX");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETSTRETCHBLTMODE(const unsigned int &unStretchMode)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETSTRETCHBLTMODE");
		m_pOutputXml->WriteNode(L"StretchMode", unStretchMode);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETSTRETCHBLTMODE");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETICMMODE(const unsigned int &unICMMode)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETICMMODE");
		m_pOutputXml->WriteNode(L"ICMMode", unICMMode);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETICMMODE");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_CREATEMONOBRUSH(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush, CDataStream &oDataStream)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_CREATEMONOBRUSH");
		m_pOutputXml->WriteNode(L"ihBrush", unBrushIndex);
		m_pOutputXml->WriteNode(L"", oDibBrush);

		unsigned int unSize = oDibBrush.cbBmi + oDibBrush.cbBits;
		unsigned int unSkip = oDibBrush.offBmi - 32;

		oDataStream.Skip(unSkip);

		if (unSize > 0)
			m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

		oDataStream.SeekBack(unSkip);

		m_pOutputXml->WriteNodeEnd(L"EMR_CREATEMONOBRUSH");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush,
																  CDataStream &oDataStream)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_CREATEDIBPATTERNBRUSHPT");
		m_pOutputXml->WriteNode(L"ihBrush", unBrushIndex);
		m_pOutputXml->WriteNode(L"", oDibBrush);

		unsigned int unSize = oDibBrush.cbBmi + oDibBrush.cbBits;
		unsigned int unSkip = oDibBrush.offBmi - 32;

		oDataStream.Skip(unSkip);

		if (unSize > 0)
			m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

		oDataStream.SeekBack(unSkip);

		m_pOutputXml->WriteNodeEnd(L"EMR_CREATEDIBPATTERNBRUSHPT");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SELECTCLIPPATH(const unsigned int &unRegionMode)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SELECTCLIPPATH");
		m_pOutputXml->WriteNode(L"RegionMode", unRegionMode);
		m_pOutputXml->WriteNodeEnd(L"EMR_SELECTCLIPPATH");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETBKCOLOR(const TEmfColor &oColor)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETBKCOLOR");
		m_pOutputXml->WriteNode(L"Color", oColor);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETBKCOLOR");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_EXCLUDECLIPRECT(const TEmfRectL &oClip)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_EXCLUDECLIPRECT");
		m_pOutputXml->WriteNode(L"Clip", oClip);
		m_pOutputXml->WriteNodeEnd(L"EMR_EXCLUDECLIPRECT");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode,
														   CDataStream &oDataStream)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_EXTSELECTCLIPRGN");
		m_pOutputXml->WriteNode(L"RgnDataSize", unRgnDataSize);
		m_pOutputXml->WriteNode(L"RegionMode",  unRegionMode);

		unsigned int unSize = unRgnDataSize;

		if (unSize > 0)
			m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

		m_pOutputXml->WriteNodeEnd(L"EMR_EXTSELECTCLIPRGN");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETMETARGN()
	{
		m_pOutputXml->WriteNode(L"EMR_SETMETARGN");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETROP2(const unsigned int &unRop2Mode)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETROP2");
		m_pOutputXml->WriteNode(L"Rop2Mode", unRop2Mode);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETROP2");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_CREATEPALETTE(const unsigned int &unPaletteIndex, const CEmfLogPalette *oEmfLogPalette)
	{
		if (NULL == oEmfLogPalette)
			return;

		m_pOutputXml->WriteNodeBegin(L"EMR_CREATEPALETTE");
		m_pOutputXml->WriteNode(L"ihPal", unPaletteIndex);
		m_pOutputXml->WriteNode(L"LogPalette", *oEmfLogPalette);
		m_pOutputXml->WriteNodeEnd(L"EMR_CREATEPALETTE");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SELECTPALETTE(const unsigned int &unPaletteIndex)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SELECTPALETTE");
		m_pOutputXml->WriteNode(L"ihPal", unPaletteIndex);
		m_pOutputXml->WriteNodeEnd(L"EMR_SELECTPALETTE");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_REALIZEPALETTE()
	{
		m_pOutputXml->WriteNode(L"EMR_REALIZEPALETTE");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_INTERSECTCLIPRECT(const TEmfRectL &oClip)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_INTERSECTCLIPRECT");
		m_pOutputXml->WriteNode(L"Clip", oClip);
		m_pOutputXml->WriteNodeEnd(L"EMR_INTERSECTCLIPRECT");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETLAYOUT(const unsigned int &unLayoutMode)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETLAYOUT");
		m_pOutputXml->WriteNode(L"LayoutMode", unLayoutMode);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETLAYOUT");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETBRUSHORGEX(const TEmfPointL &oOrigin)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETBRUSHORGEX");
		m_pOutputXml->WriteNode(L"Origin", oOrigin);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETBRUSHORGEX");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_ANGLEARC(const TEmfPointL &oCenter, const unsigned int &unRadius,
												   const double &dStartAngle, const double &dSweepAngle)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_ANGLEARC");
		m_pOutputXml->WriteNode(L"Center",     oCenter);
		m_pOutputXml->WriteNode(L"Radius",     unRadius);
		m_pOutputXml->WriteNode(L"StartAngle", dStartAngle);
		m_pOutputXml->WriteNode(L"SweepAngle", dSweepAngle);
		m_pOutputXml->WriteNodeEnd(L"EMR_ANGLEARC");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_ARC(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_ARC");
		m_pOutputXml->WriteNode(L"Rectangle", oBox);
		m_pOutputXml->WriteNode(L"StartPoint", oStart);
		m_pOutputXml->WriteNode(L"EndPoint", oEnd);
		m_pOutputXml->WriteNodeEnd(L"EMR_ARC");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_ARCTO(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_ARCTO");
		m_pOutputXml->WriteNode(L"Rectangle", oBox);
		m_pOutputXml->WriteNode(L"StartPoint", oStart);
		m_pOutputXml->WriteNode(L"EndPoint", oEnd);
		m_pOutputXml->WriteNodeEnd(L"EMR_ARCTO");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_CHORD(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_CHORD");
		m_pOutputXml->WriteNode(L"Rectangle", oBox);
		m_pOutputXml->WriteNode(L"StartPoint", oStart);
		m_pOutputXml->WriteNode(L"EndPoint", oEnd);
		m_pOutputXml->WriteNodeEnd(L"EMR_CHORD");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_ELLIPSE(const TEmfRectL &oBox)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_ELLIPSE");
		m_pOutputXml->WriteNode(L"Rectangle", oBox);
		m_pOutputXml->WriteNodeEnd(L"EMR_ELLIPSE");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_EXTTEXTOUTA(const TEmfExtTextoutA &oTEmfExtTextoutA)
	{
		m_pOutputXml->WriteNode(L"EMR_EXTTEXTOUTA", oTEmfExtTextoutA);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_EXTTEXTOUTW(const TEmfExtTextoutW &oTEmfExtTextoutW)
	{
		m_pOutputXml->WriteNode(L"EMR_EXTTEXTOUTW", oTEmfExtTextoutW);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_LINETO(const TEmfPointL &oPoint)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_LINETO");
		m_pOutputXml->WriteNode(L"Point", oPoint);
		m_pOutputXml->WriteNodeEnd(L"EMR_LINETO");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_PIE(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_PIE");
		m_pOutputXml->WriteNode(L"Rectangle",	oBox);
		m_pOutputXml->WriteNode(L"StartPoint",	oStart);
		m_pOutputXml->WriteNode(L"EndPoint",	oEnd);
		m_pOutputXml->WriteNodeEnd(L"EMR_PIE");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointL>& arPoints)
	{
		Save_EMR_POLY_BASE<TEmfPointL>(RecordData(L"EMR_POLYBEZIER", EMR_POLYBEZIER, oBounds), arPoints);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointS>& arPoints)
	{
		Save_EMR_POLY_BASE<TEmfPointS>(RecordData(L"EMR_POLYBEZIER16", EMR_POLYBEZIER16, oBounds), arPoints);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointL>& arPoints)
	{
		Save_EMR_POLY_BASE<TEmfPointL>(RecordData(L"EMR_POLYBEZIERTO", EMR_POLYBEZIERTO, oBounds), arPoints);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointS>& arPoints)
	{
		Save_EMR_POLY_BASE<TEmfPointS>(RecordData(L"EMR_POLYBEZIERTO16", EMR_POLYBEZIERTO16, oBounds), arPoints);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointL *arPoints,
												   const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		Save_EMR_POLYDRAW_BASE<TEmfPointL>(oBounds, arPoints, unCount, pAbTypes);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointS *arPoints,
												   const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		Save_EMR_POLYDRAW_BASE<TEmfPointS>(oBounds, arPoints, unCount, pAbTypes);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		Save_EMR_POLY_BASE<TEmfPointL>(RecordData(L"EMR_POLYGON", EMR_POLYGON, oBounds), arPoints);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		Save_EMR_POLY_BASE<TEmfPointS>(RecordData(L"EMR_POLYGON16", EMR_POLYGON16, oBounds), arPoints);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		Save_EMR_POLY_BASE<TEmfPointL>(RecordData(L"EMR_POLYLINE", EMR_POLYLINE, oBounds), arPoints);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		Save_EMR_POLY_BASE<TEmfPointS>(RecordData(L"EMR_POLYLINE16", EMR_POLYLINE16, oBounds), arPoints);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		Save_EMR_POLY_BASE<TEmfPointL>(RecordData(L"EMR_POLYLINETO", EMR_POLYLINETO, oBounds), arPoints);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		Save_EMR_POLY_BASE<TEmfPointS>(RecordData(L"EMR_POLYLINETO16", EMR_POLYLINETO16, oBounds), arPoints);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYPOLYGON(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointL>> &arPoints)
	{
		Save_EMR_POLYPOLY_BASE<TEmfPointL>(RecordData(L"EMR_POLYPOLYGON", EMR_POLYPOLYGON, oBounds), arPoints);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYPOLYGON(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointS>> &arPoints)
	{
		Save_EMR_POLYPOLY_BASE<TEmfPointS>(RecordData(L"EMR_POLYPOLYGON16", EMR_POLYPOLYGON16, oBounds), arPoints);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYPOLYLINE(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointL>> &arPoints)
	{
		Save_EMR_POLYPOLY_BASE<TEmfPointL>(RecordData(L"EMR_POLYPOLYLINE", EMR_POLYPOLYLINE, oBounds), arPoints);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_POLYPOLYLINE(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointS>> &arPoints)
	{
		Save_EMR_POLYPOLY_BASE<TEmfPointS>(RecordData(L"EMR_POLYPOLYLINE16", EMR_POLYPOLYLINE16, oBounds), arPoints);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_RECTANGLE(const TEmfRectL& oBox)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_RECTANGLE");
		m_pOutputXml->WriteNode(L"Rectangle", oBox);
		m_pOutputXml->WriteNodeEnd(L"EMR_RECTANGLE");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_ROUNDRECT(const TEmfRectL &oBox, const TEmfSizeL &oCorner)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_ROUNDRECT");
		m_pOutputXml->WriteNode(L"Rectangle",   oBox);
		m_pOutputXml->WriteNode(L"Corner",      oCorner);
		m_pOutputXml->WriteNodeEnd(L"EMR_ROUNDRECT");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SETPIXELV(const TEmfPointL &oPoint, const TEmfColor &oColor)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_SETPIXELV");
		m_pOutputXml->WriteNode(L"Pixel", oPoint);
		m_pOutputXml->WriteNode(L"Color", oColor);
		m_pOutputXml->WriteNodeEnd(L"EMR_SETPIXELV");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_SMALLTEXTOUT(const TEmfSmallTextout &oText)
	{
		m_pOutputXml->WriteNode(L"EMR_SMALLTEXTOUT", oText);
	}

	void CEmfInterpretatorXml::HANDLE_EMR_STROKEANDFILLPATH(const TEmfRectL &oBounds)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_STROKEANDFILLPATH");
		m_pOutputXml->WriteNode(L"Bounds", oBounds);
		m_pOutputXml->WriteNodeEnd(L"EMR_STROKEANDFILLPATH");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_STROKEPATH(const TEmfRectL &oBounds)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_STROKEPATH");
		m_pOutputXml->WriteNode(L"Bounds", oBounds);
		m_pOutputXml->WriteNodeEnd(L"EMR_STROKEPATH");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_UNKNOWN(CDataStream &oDataStream)
	{
		unsigned int unRecordSize;

		oDataStream.Skip(4);
		oDataStream >> unRecordSize;

		oDataStream.SeekBack(8);

		m_pOutputXml->WriteNodeBegin(L"EMR_UNKNOWN");
		m_pOutputXml->WriteNode(L"Buffer", oDataStream, unRecordSize);
		m_pOutputXml->WriteNodeEnd(L"EMR_UNKNOWN");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_FILLRGN(const TEmfRectL &oBounds, unsigned int unIhBrush, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_FILLRGN");
		m_pOutputXml->WriteNode(L"Bounds",  oBounds);
		m_pOutputXml->WriteNode(L"ihBrush", unIhBrush);
		m_pOutputXml->WriteNode(L"RgnData", oRegionDataHeader, arRects);
		m_pOutputXml->WriteNodeEnd(L"EMR_FILLRGN");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_PAINTRGN(const TEmfRectL &oBounds, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_PAINTRGN");
		m_pOutputXml->WriteNode(L"Bounds",  oBounds);
		m_pOutputXml->WriteNode(L"RgnData", oRegionDataHeader, arRects);
		m_pOutputXml->WriteNodeEnd(L"EMR_PAINTRGN");
	}

	void CEmfInterpretatorXml::HANDLE_EMR_FRAMERGN(const TEmfRectL &oBounds, unsigned int unIhBrush, int nWidth, int nHeight, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		m_pOutputXml->WriteNodeBegin(L"EMR_FRAMERGN");
		m_pOutputXml->WriteNode(L"Bounds",  oBounds);
		m_pOutputXml->WriteNode(L"ihBrush", unIhBrush);
		m_pOutputXml->WriteNode(L"Width",   nWidth);
		m_pOutputXml->WriteNode(L"Height",  nHeight);
		m_pOutputXml->WriteNode(L"RgnData", oRegionDataHeader, arRects);
		m_pOutputXml->WriteNodeEnd(L"EMR_FRAMERGN");
	}

	void CEmfInterpretatorXml::Begin()
	{
		if (NULL == m_pOutputXml)
			return;

		m_pOutputXml->WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
		m_pOutputXml->WriteNodeBegin(L"EMF");
	}

	void CEmfInterpretatorXml::End()
	{
		if (NULL == m_pOutputXml)
			return;

		m_pOutputXml->WriteNodeEnd(L"EMF");

		m_pOutputXml->SaveToFile(m_wsXmlFilePath.empty() ?
									 NSFile::GetProcessDirectory() + L"/test.xml" :
									 m_wsXmlFilePath);
	}

	template<typename T>
	void CEmfInterpretatorXml::Save_EMR_POLY_BASE(const RecordData &oRecordData, const std::vector<T> &arPoints)
	{
		if (arPoints.empty())
			return;

		m_pOutputXml->WriteNodeBegin(oRecordData.m_wsName);
		m_pOutputXml->WriteNode(L"Bounds",          oRecordData.m_oBounds);

		for (unsigned int unIndex = 0; unIndex < arPoints.size(); ++unIndex)
			m_pOutputXml->WriteNode(L"Point" + std::to_wstring(unIndex + 1), arPoints[unIndex]);

		m_pOutputXml->WriteNodeEnd(oRecordData.m_wsName);
	}

	template<typename T>
	void CEmfInterpretatorXml::Save_EMR_POLYPOLY_BASE(const RecordData &oRecordData, const std::vector<std::vector<T>> &arPoints)
	{
		if (arPoints.empty())
			return;

		m_pOutputXml->WriteNodeBegin(oRecordData.m_wsName);
		m_pOutputXml->WriteNode(L"Bounds",              oRecordData.m_oBounds);

		for (unsigned int unPolygonIndex = 0; unPolygonIndex < arPoints.size(); ++unPolygonIndex)
		{
			std::wstring wsNameNode = L"PolygonPoint" + std::to_wstring(unPolygonIndex + 1);
			m_pOutputXml->WriteNodeBegin(wsNameNode);
			for (unsigned int unPointIndex = 0; unPointIndex < arPoints[unPolygonIndex].size(); ++unPointIndex)
				m_pOutputXml->WriteNode(L"Point" + std::to_wstring(unPointIndex + 1), arPoints[unPolygonIndex][unPointIndex]);
			m_pOutputXml->WriteNodeEnd(wsNameNode);
		}

		m_pOutputXml->WriteNodeEnd(oRecordData.m_wsName);

	}

	template<typename T>
	void CEmfInterpretatorXml::Save_EMR_POLYDRAW_BASE(const TEmfRectL &oBounds, T *arPoints,
													  const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		if (NULL == arPoints || NULL == pAbTypes)
			return;

		std::wstring wsRecordName;
		unsigned int unRecordId;

		if (typeid (T).name() == "TEmfPointL")
		{
			wsRecordName = L"EMR_POLYDRAW";
			unRecordId = EMR_POLYDRAW;
		}
		else if(typeid (T).name() == "TEmfPointS")
		{
			wsRecordName = L"EMR_POLYDRAW16";
			unRecordId = EMR_POLYDRAW16;
		}

		m_pOutputXml->WriteNodeBegin(wsRecordName);
		m_pOutputXml->WriteNode(L"Bounds",	    oBounds);
		m_pOutputXml->WriteNode(L"NumberPoints",    unCount);

		for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
			m_pOutputXml->WriteNode(L"Point" + std::to_wstring(unIndex + 1), arPoints[unIndex]);

		for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
			m_pOutputXml->WriteNode(L"abTypes" + std::to_wstring(unIndex + 1), pAbTypes[unIndex]);

		m_pOutputXml->WriteNodeEnd(wsRecordName);
	}
}
