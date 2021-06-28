#include "CEmfInterpretatorXml.h"

namespace MetaFile
{
    CEmfInterpretatorXml::CEmfInterpretatorXml() :
        m_pOutputXml(new CXmlOutput(TypeXmlOutput::IsWriter))
    {
        m_pOutputXml->WriteString(L"<?xml version=\"1.0\"?>\n");
        m_pOutputXml->WriteNodeBegin(L"EMF");
    }

    CEmfInterpretatorXml::~CEmfInterpretatorXml()
    {
        if (NULL != m_pOutputXml)
        {
            m_pOutputXml->WriteNodeEnd(L"EMF");
            m_pOutputXml->SaveToFile(NSFile::GetProcessDirectory() + L"/test.xml");
            delete m_pOutputXml;
        }
    }

    void CEmfInterpretatorXml::Save_EMR_HEADER(const TEmfHeader &oTEmfHeader, CDataStream &oDataStream,
                                               const unsigned int &unRecordSize)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_HEADER", {XmlArgument(L"Id", EMR_HEADER),
                                                     XmlArgument(L"Size", unRecordSize + 8)});

	    m_pOutputXml->WriteNode(L"Bounds",		oTEmfHeader.oBounds);
	    m_pOutputXml->WriteNode(L"Frame",		oTEmfHeader.oFrame);
	    m_pOutputXml->WriteNode(L"Signature",		oTEmfHeader.ulSignature);
	    m_pOutputXml->WriteNode(L"Version",		oTEmfHeader.ulVersion);
	    m_pOutputXml->WriteNode(L"Size",		oTEmfHeader.ulSize);
	    m_pOutputXml->WriteNode(L"Records",		oTEmfHeader.ulRecords);
	    m_pOutputXml->WriteNode(L"CountObjects",	oTEmfHeader.ushObjects);
	    m_pOutputXml->WriteNode(L"Reserved",		oTEmfHeader.ushReserved);
	    m_pOutputXml->WriteNode(L"SizeDescription",	oTEmfHeader.ulSizeDescription);
	    m_pOutputXml->WriteNode(L"OffsetDescription",	oTEmfHeader.ulOffsetDescription);
	    m_pOutputXml->WriteNode(L"PalletEntries",	oTEmfHeader.ulPalEntries);
	    m_pOutputXml->WriteNode(L"Device",		oTEmfHeader.oDevice);
	    m_pOutputXml->WriteNode(L"Millimeters",		oTEmfHeader.oMillimeters);

	    unsigned int unSize = unRecordSize - 80;
	    if (unSize > 0)
		    m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

            m_pOutputXml->WriteNodeEnd(L"EMR_HEADER");
    }

    void CEmfInterpretatorXml::Save_EMR_ALPHABLEND(const TEmfAlphaBlend &oTEmfAlphaBlend, CDataStream &oDataStream,
                                                   const unsigned int &unRecordSize)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_ALPHABLEND", {XmlArgument(L"Id", EMR_ALPHABLEND),
                                                         XmlArgument(L"Size", unRecordSize + 8)});
            m_pOutputXml->WriteNode(L"", oTEmfAlphaBlend);

	    unsigned int unSize = unRecordSize - sizeof (TEmfAlphaBlend);
	    if (unSize > 0)
		    m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

            m_pOutputXml->WriteNodeEnd(L"EMR_ALPHABLEND");
    }

    void CEmfInterpretatorXml::Save_EMR_STRETCHDIBITS(const TEmfStretchDIBITS &oTEmfStretchDIBITS, CDataStream &oDataStream,
                                                      const unsigned int &unRecordSize)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_STRETCHDIBITS", {XmlArgument(L"Id", EMR_STRETCHDIBITS),
                                                            XmlArgument(L"Size", unRecordSize + 8)});
            m_pOutputXml->WriteNode(L"", oTEmfStretchDIBITS);

	    unsigned int unSize = unRecordSize - sizeof (TEmfStretchDIBITS);
	    if (unSize > 0)
		    m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

            m_pOutputXml->WriteNodeEnd(L"EMR_STRETCHDIBITS");
    }

    void CEmfInterpretatorXml::Save_EMR_BITBLT(const TEmfBitBlt &oTEmfBitBlt, CDataStream &oDataStream,
                                               const unsigned int &unRecordSize)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_BITBLT", {XmlArgument(L"Id", EMR_BITBLT),
                                                     XmlArgument(L"Size", unRecordSize + 8)});
            m_pOutputXml->WriteNode(L"", oTEmfBitBlt);

	    unsigned int unSize = unRecordSize - 92;
	    if (unSize > 0)
		    m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

            m_pOutputXml->WriteNodeEnd(L"EMR_BITBLT");
    }

    void CEmfInterpretatorXml::Save_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice, CDataStream &oDataStream,
                                                          const unsigned int &unRecordSize)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETDIBITSTODEVICE", {XmlArgument(L"Id", EMR_SETDIBITSTODEVICE),
                                                                XmlArgument(L"Size", unRecordSize + 8)});
            m_pOutputXml->WriteNode(L"", oTEmfSetDiBitsToDevice);

	    unsigned int unSize = unRecordSize - sizeof (TEmfSetDiBitsToDevice);
	    if (unSize > 0)
		    m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

            m_pOutputXml->WriteNodeEnd(L"EMR_SETDIBITSTODEVICE");
    }

    void CEmfInterpretatorXml::Save_EMR_STRETCHBLT(const TEmfStretchBLT &oTEmfStretchBLT, CDataStream &oDataStream,
                                                   const unsigned int &unRecordSize)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_STRETCHBLT", {XmlArgument(L"Id", EMR_STRETCHBLT),
                                                         XmlArgument(L"Size", unRecordSize + 8)});
            m_pOutputXml->WriteNode(L"", oTEmfStretchBLT);

	    unsigned int unSize = unRecordSize - 100;
	    if (unSize > 0)
		    m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

            m_pOutputXml->WriteNodeEnd(L"EMR_STRETCHBLT");
    }

    void CEmfInterpretatorXml::Save_EMR_EOF(const unsigned int &unCount,    const unsigned int &unOffset,
                                            const unsigned int &unSizeLast)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_EOF", {XmlArgument(L"Id", EMR_EOF),
                                                  XmlArgument(L"Size", 20)});
            m_pOutputXml->WriteNode(L"nPalEntries",	    unCount);
            m_pOutputXml->WriteNode(L"offPalEntries",   unOffset);
            m_pOutputXml->WriteNode(L"SizeLast",	    unSizeLast);
            m_pOutputXml->WriteNodeEnd(L"EMR_EOF");
    }

    void CEmfInterpretatorXml::Save_EMR_SAVEDC()
    {
        m_pOutputXml->WriteNode(L"EMR_SAVEDC", {XmlArgument(L"Id", EMR_SAVEDC),
                                                XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretatorXml::Save_EMR_RESTOREDC(const int &nIndexDC)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_RESTOREDC", {XmlArgument(L"Id", EMR_RESTOREDC),
                                                        XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"SavedDC", nIndexDC);
            m_pOutputXml->WriteNodeEnd(L"EMR_RESTOREDC");
    }

    void CEmfInterpretatorXml::Save_EMR_MODIFYWORLDTRANSFORM(const TXForm &oXForm, const unsigned int &unMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_MODIFYWORLDTRANSFORM", {XmlArgument(L"Id", EMR_MODIFYWORLDTRANSFORM),
                                                                   XmlArgument(L"Size", 36)});
            m_pOutputXml->WriteNode(L"XForm",			oXForm);
            m_pOutputXml->WriteNode(L"ModifyWorldTransformMode",	unMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_MODIFYWORLDTRANSFORM");
    }

    void CEmfInterpretatorXml::Save_EMR_SETWORLDTRANSFORM(const TXForm &oXForm)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETWORLDTRANSFORM", {XmlArgument(L"Id", EMR_SETWORLDTRANSFORM),
                                                                XmlArgument(L"Size", 32)});
            m_pOutputXml->WriteNode(L"XForm", oXForm);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETWORLDTRANSFORM");
    }

    void CEmfInterpretatorXml::Save_EMR_CREATEBRUSHINDIRECT(const unsigned int &unBrushIndex, const CEmfLogBrushEx *pBrush)
    {
        if (NULL == pBrush)
            return;

        m_pOutputXml->WriteNodeBegin(L"EMR_CREATEBRUSHINDIRECT", {XmlArgument(L"Id", EMR_CREATEBRUSHINDIRECT),
                                                                  XmlArgument(L"Size", 24)});
            m_pOutputXml->WriteNode(L"ihBrush",	unBrushIndex);
            m_pOutputXml->WriteNode(L"LogBrush ",   *pBrush);
            m_pOutputXml->WriteNodeEnd(L"EMR_CREATEBRUSHINDIRECT");
    }

    void CEmfInterpretatorXml::Save_EMR_SETTEXTCOLOR(const TEmfColor &oColor)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETTEXTCOLOR", {XmlArgument(L"Id", EMR_SETTEXTCOLOR),
                                                           XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"Color", oColor);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETTEXTCOLOR");
    }

    void CEmfInterpretatorXml::Save_EMR_SELECTOBJECT(const unsigned int &unObjectIndex)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SELECTOBJECT", {XmlArgument(L"Id", EMR_SELECTOBJECT),
                                                           XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"Index", unObjectIndex);
            m_pOutputXml->WriteNodeEnd(L"EMR_SELECTOBJECT");
    }

    void CEmfInterpretatorXml::Save_EMR_EXTCREATEFONTINDIRECTW(const unsigned int &unIndex, CEmfLogFont *oLogFont,
                                                               const unsigned int unRecordSize)
    {
        if (NULL == oLogFont)
            return;

        m_pOutputXml->WriteNodeBegin(L"EMR_EXTCREATEFONTINDIRECTW", {XmlArgument(L"Id", EMR_EXTCREATEFONTINDIRECTW),
                                                                     XmlArgument(L"Size", unRecordSize + 8)});
            m_pOutputXml->WriteNode(L"ihFonts", unIndex);
            m_pOutputXml->WriteNode(L"",	    *oLogFont);
            m_pOutputXml->WriteNodeEnd(L"EMR_EXTCREATEFONTINDIRECTW");
    }

    void CEmfInterpretatorXml::Save_EMR_SETTEXTALIGN(const unsigned int &unAlign)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETTEXTALIGN", {XmlArgument(L"Id", EMR_SETTEXTALIGN),
                                                           XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"TextAlign", unAlign);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETTEXTALIGN");
    }

    void CEmfInterpretatorXml::Save_EMR_SETBKMODE(const unsigned int &unBgMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETBKMODE", {XmlArgument(L"Id", EMR_SETBKMODE),
                                                        XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"BgMode", unBgMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETBKMODE");
    }

    void CEmfInterpretatorXml::Save_EMR_DELETEOBJECT(const unsigned int &unObjectIndex)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_DELETEOBJECT", {XmlArgument(L"Id", EMR_DELETEOBJECT),
                                                           XmlArgument(L"Size",12)});
            m_pOutputXml->WriteNode(L"Index", unObjectIndex);
            m_pOutputXml->WriteNodeEnd(L"EMR_DELETEOBJECT");
    }

    void CEmfInterpretatorXml::Save_EMR_SETMITERLIMIT(const unsigned int &unMeterLimit)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETMITERLIMIT", {XmlArgument(L"Id", EMR_SETMITERLIMIT),
                                                            XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"MiterLimit", unMeterLimit);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETMITERLIMIT");
    }

    void CEmfInterpretatorXml::Save_EMR_EXTCREATEPEN(const unsigned int &unPenIndex, CEmfLogPen *pPen,
                                                     const unsigned int &unRecordSize)
    {
        if (NULL != pPen)
            return;

        m_pOutputXml->WriteNodeBegin(L"EMR_EXTCREATEPEN", {XmlArgument(L"Id", EMR_EXTCREATEPEN),
                                                           XmlArgument(L"Size", unRecordSize + 8)});
            m_pOutputXml->WriteNode(L"ihPen",   unPenIndex);
            m_pOutputXml->WriteNodeBegin(L"LogPenEx");
                m_pOutputXml->WriteNode(L"PenStyle",	pPen->PenStyle);
                m_pOutputXml->WriteNode(L"Width",	pPen->Width);
                m_pOutputXml->WriteNode(L"ColorRef",    pPen->Color);
                m_pOutputXml->WriteNode(L"NumStyleEntries", pPen->NumStyleEntries);

                for (unsigned int ulIndex = 0; ulIndex < pPen->NumStyleEntries; ulIndex++)
                    m_pOutputXml->WriteNode(L"StyleEntry" + std::to_wstring(ulIndex + 1),  pPen->StyleEntry[ulIndex]);

		m_pOutputXml->WriteNodeEnd(L"LogPenEx");
	m_pOutputXml->WriteNodeEnd(L"EMR_EXTCREATEPEN");

    }

    void CEmfInterpretatorXml::Save_EMR_CREATEPEN(const unsigned int &unPenIndex, const unsigned int &unWidthX, const CEmfLogPen *pPen)
    {
        if (NULL == pPen)
            return;

        m_pOutputXml->WriteNodeBegin(L"EMR_CREATEPEN", {XmlArgument(L"Id", EMR_CREATEPEN),
                                                        XmlArgument(L"Size", 28)});
            m_pOutputXml->WriteNode(L"ihPen", unPenIndex);
            m_pOutputXml->WriteNodeBegin(L"LogPen");
                m_pOutputXml->WriteNode(L"PenStyle", pPen->PenStyle);
                m_pOutputXml->WriteNode(L"Width", unWidthX);
                m_pOutputXml->WriteNode(L"Height", unWidthX);
                m_pOutputXml->WriteNodeEnd(L"LogPen");
            m_pOutputXml->WriteNode(L"COLORREF", pPen->Color);
            m_pOutputXml->WriteNodeEnd(L"EMR_CREATEPEN");
    }

    void CEmfInterpretatorXml::Save_EMR_SETPOLYFILLMODE(const unsigned int &unFillMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETPOLYFILLMODE", {XmlArgument(L"Id", EMR_SETPOLYFILLMODE),
                                                              XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"PolygonFillMode", unFillMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETPOLYFILLMODE");
    }

    void CEmfInterpretatorXml::Save_EMR_BEGINPATH()
    {
        m_pOutputXml->WriteNode(L"EMR_BEGINPATH", {XmlArgument(L"Id", EMR_BEGINPATH),
                                                   XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretatorXml::Save_EMR_ENDPATH()
    {
        m_pOutputXml->WriteNode(L"EMR_ENDPATH", {XmlArgument(L"Id", EMR_ENDPATH),
                                                 XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretatorXml::Save_EMR_CLOSEFIGURE()
    {
        m_pOutputXml->WriteNode(L"EMR_CLOSEFIGURE", {XmlArgument(L"Id", EMR_CLOSEFIGURE),
                                                     XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretatorXml::Save_EMR_FLATTENPATH()
    {
        m_pOutputXml->WriteNode(L"EMR_FLATTENPATH", {XmlArgument(L"Id", EMR_FLATTENPATH),
                                                     XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretatorXml::Save_EMR_WIDENPATH()
    {
        m_pOutputXml->WriteNode(L"EMR_WIDENPATH", {XmlArgument(L"Id", EMR_WIDENPATH),
                                                   XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretatorXml::Save_EMR_ABORTPATH()
    {
        m_pOutputXml->WriteNode(L"EMR_ABORTPATH", {XmlArgument(L"Id", EMR_ABORTPATH),
                                                   XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretatorXml::Save_EMR_MOVETOEX(const TEmfPointL &oPoint)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_MOVETOEX", {XmlArgument(L"Id", EMR_MOVETOEX),
                                                       XmlArgument(L"Size", 16)});
            m_pOutputXml->WriteNode(L"Offset", oPoint);
            m_pOutputXml->WriteNodeEnd(L"EMR_MOVETOEX");
    }

    void CEmfInterpretatorXml::Save_EMR_SETARCDIRECTION(const unsigned int &unDirection)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETARCDIRECTION", {XmlArgument(L"Id", EMR_SETARCDIRECTION),
                                                              XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"arcDirection", unDirection);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETARCDIRECTION");
    }

    void CEmfInterpretatorXml::Save_EMR_FILLPATH(const TEmfRectL &oBounds)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_FILLPATH", {XmlArgument(L"Id", EMR_FILLPATH),
                                                       XmlArgument(L"Size", 24)});
            m_pOutputXml->WriteNode(L"Bounds", oBounds);
            m_pOutputXml->WriteNodeEnd(L"EMR_FILLPATH");
    }

    void CEmfInterpretatorXml::Save_EMR_SETMAPMODE(const unsigned int &unMapMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETMAPMODE", {XmlArgument(L"Id", EMR_SETMAPMODE),
                                                         XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"MapMode", unMapMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETMAPMODE");
    }

    void CEmfInterpretatorXml::Save_EMR_SETWINDOWORGEX(const TEmfPointL &oOrigin)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETWINDOWORGEX", {XmlArgument(L"Id", EMR_SETWINDOWORGEX),
                                                             XmlArgument(L"Size", 16)});
            m_pOutputXml->WriteNode(L"Origin", oOrigin);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETWINDOWORGEX");
    }

    void CEmfInterpretatorXml::Save_EMR_SETWINDOWEXTEX(const TEmfSizeL &oExtent)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETWINDOWEXTEX", {XmlArgument(L"Id", EMR_SETWINDOWEXTEX),
                                                             XmlArgument(L"Size", 16)});
            m_pOutputXml->WriteNode(L"Extent", oExtent);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETWINDOWEXTEX");
    }

    void CEmfInterpretatorXml::Save_EMR_SETVIEWPORTORGEX(const TEmfPointL &oOrigin)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETVIEWPORTORGEX", {XmlArgument(L"Id", EMR_SETVIEWPORTORGEX),
                                                               XmlArgument(L"Size", 16)});
            m_pOutputXml->WriteNode(L"Origin", oOrigin);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETVIEWPORTORGEX");
    }

    void CEmfInterpretatorXml::Save_EMR_SETVIEWPORTEXTEX(const TEmfSizeL &oExtent)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETVIEWPORTEXTEX", {XmlArgument(L"Id", EMR_SETVIEWPORTEXTEX),
                                                               XmlArgument(L"Size", 16)});
            m_pOutputXml->WriteNode(L"Extent", oExtent);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETVIEWPORTEXTEX");
    }

    void CEmfInterpretatorXml::Save_EMR_SETSTRETCHBLTMODE(const unsigned int &unStretchMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETSTRETCHBLTMODE", {XmlArgument(L"Id", EMR_SETSTRETCHBLTMODE),
                                                                XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"StretchMode", unStretchMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETSTRETCHBLTMODE");
    }

    void CEmfInterpretatorXml::Save_EMR_SETICMMODE(const unsigned int &unICMMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETICMMODE", {XmlArgument(L"Id", EMR_SETICMMODE),
                                                         XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"ICMMode", unICMMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETICMMODE");
    }

    void CEmfInterpretatorXml::Save_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush,
                                                                CDataStream &oDataStream, const unsigned int &unRecordSize)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_CREATEDIBPATTERNBRUSHPT", {XmlArgument(L"Id", EMR_CREATEDIBPATTERNBRUSHPT),
                                                                      XmlArgument(L"Size", unRecordSize + 8)});
            m_pOutputXml->WriteNode(L"ihBrush", unBrushIndex);
            m_pOutputXml->WriteNode(L"", oDibBrush);

            unsigned int unSize = unRecordSize - sizeof(TEmfDibPatternBrush) - 12;

            if (unSize > 0)
                m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

            m_pOutputXml->WriteNodeEnd(L"EMR_CREATEDIBPATTERNBRUSHPT");
    }

    void CEmfInterpretatorXml::Save_EMR_SELECTCLIPPATH(const unsigned int &unRegionMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SELECTCLIPPATH", {XmlArgument(L"Id", EMR_SELECTCLIPPATH),
                                                             XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"RegionMode", unRegionMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SELECTCLIPPATH");
    }

    void CEmfInterpretatorXml::Save_EMR_SETBKCOLOR(const TEmfColor &oColor)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETBKCOLOR", {XmlArgument(L"Id", EMR_SETBKCOLOR),
                                                         XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"Color", oColor);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETBKCOLOR");
    }

    void CEmfInterpretatorXml::Save_EMR_EXCLUDECLIPRECT(const TEmfRectL &oClip)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_EXCLUDECLIPRECT", {XmlArgument(L"Id", EMR_EXCLUDECLIPRECT),
                                                             XmlArgument(L"Size", 20)});
            m_pOutputXml->WriteNode(L"Clip", oClip);
            m_pOutputXml->WriteNodeEnd(L"EMR_EXCLUDECLIPRECT");
    }

    void CEmfInterpretatorXml::Save_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode,
                                                         CDataStream &oDataStream, const unsigned int &unRecordSize)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_EXTSELECTCLIPRGN", {XmlArgument(L"Id", EMR_EXTSELECTCLIPRGN),
                                                               XmlArgument(L"Size", unRecordSize + 8)});
            m_pOutputXml->WriteNode(L"RgnDataSize", unRgnDataSize);
            m_pOutputXml->WriteNode(L"RegionMode",  unRegionMode);

            unsigned int unSize = unRecordSize - 8;

	    if (unSize > 0)
		m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

            m_pOutputXml->WriteNodeEnd(L"EMR_EXTSELECTCLIPRGN");
    }

    void CEmfInterpretatorXml::Save_EMR_SETMETARGN()
    {
        m_pOutputXml->WriteNode(L"EMR_SETMETARGN", {XmlArgument(L"Id", EMR_SETMETARGN),
                                                    XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretatorXml::Save_EMR_SETROP2(const unsigned int &unRop2Mode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETROP2", {XmlArgument(L"Id", EMR_SETROP2),
                                                      XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"Rop2Mode", unRop2Mode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETROP2");
    }

    void CEmfInterpretatorXml::Save_EMR_CREATEPALETTE(const unsigned int &unPaletteIndex, const CEmfLogPalette *oEmfLogPalette, const unsigned int &unRecordSize)
    {
        if (NULL == oEmfLogPalette)
            return;

        m_pOutputXml->WriteNodeBegin(L"EMR_CREATEPALETTE", {XmlArgument(L"Id", EMR_CREATEPALETTE),
                                                            XmlArgument(L"Size", unRecordSize + 8)});
            m_pOutputXml->WriteNode(L"ihPal", unPaletteIndex);
            m_pOutputXml->WriteNode(L"LogPalette", *oEmfLogPalette);
            m_pOutputXml->WriteNodeEnd(L"EMR_CREATEPALETTE");
    }

    void CEmfInterpretatorXml::Save_EMR_SELECTPALETTE(const unsigned int &unPaletteIndex)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SELECTPALETTE", {XmlArgument(L"Id", EMR_SELECTPALETTE),
                                                            XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"ihPal", unPaletteIndex);
            m_pOutputXml->WriteNodeEnd(L"EMR_SELECTPALETTE");
    }

    void CEmfInterpretatorXml::Save_EMR_REALIZEPALETTE()
    {
        m_pOutputXml->WriteNode(L"EMR_REALIZEPALETTE", {XmlArgument(L"Id", EMR_REALIZEPALETTE),
                                                        XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretatorXml::Save_EMR_INTERSECTCLIPRECT(const TEmfRectL &oClip)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_INTERSECTCLIPRECT", {XmlArgument(L"Id", EMR_INTERSECTCLIPRECT),
                                                                XmlArgument(L"Size", 20)});
            m_pOutputXml->WriteNode(L"Clip", oClip);
            m_pOutputXml->WriteNodeEnd(L"EMR_INTERSECTCLIPRECT");
    }

    void CEmfInterpretatorXml::Save_EMR_SETLAYOUT(const unsigned int &unLayoutMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETLAYOUT", {XmlArgument(L"Id", EMR_SETLAYOUT),
                                                        XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"LayoutMode", unLayoutMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETLAYOUT");
    }

    void CEmfInterpretatorXml::Save_EMR_SETBRUSHORGEX(const TEmfPointL &oOrigin)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETBRUSHORGEX", {XmlArgument(L"Id", EMR_SETBRUSHORGEX),
                                                            XmlArgument(L"Size",16)});
            m_pOutputXml->WriteNode(L"Origin", oOrigin);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETBRUSHORGEX");
    }

    void CEmfInterpretatorXml::Save_EMR_ANGLEARC(const TEmfPointL &oCenter, const unsigned int &unRadius,
                                                 const double &dStartAngle, const double &dSweepAngle)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_ANGLEARC", {XmlArgument(L"Id", EMR_ANGLEARC),
                                                       XmlArgument(L"Size", 28)});
            m_pOutputXml->WriteNode(L"Center",     oCenter);
            m_pOutputXml->WriteNode(L"Radius",     unRadius);
            m_pOutputXml->WriteNode(L"StartAngle", dStartAngle);
            m_pOutputXml->WriteNode(L"SweepAngle", dSweepAngle);
            m_pOutputXml->WriteNodeEnd(L"EMR_ANGLEARC");
    }

    void CEmfInterpretatorXml::Save_EMR_ARC(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_ARC", {XmlArgument(L"Id", EMR_ARC),
                                                  XmlArgument(L"Size", 40)});
             m_pOutputXml->WriteNode(L"Rectangle", oBox);
             m_pOutputXml->WriteNode(L"StartPoint", oStart);
             m_pOutputXml->WriteNode(L"EndPoint", oEnd);
             m_pOutputXml->WriteNodeEnd(L"EMR_ARC");
    }

    void CEmfInterpretatorXml::Save_EMR_ARCTO(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_ARCTO", {XmlArgument(L"Id", EMR_ARCTO),
                                                    XmlArgument(L"Size", 40)});
            m_pOutputXml->WriteNode(L"Rectangle", oBox);
            m_pOutputXml->WriteNode(L"StartPoint", oStart);
            m_pOutputXml->WriteNode(L"EndPoint", oEnd);
            m_pOutputXml->WriteNodeEnd(L"EMR_ARCTO");
    }

    void CEmfInterpretatorXml::Save_EMR_CHORD(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_CHORD", {XmlArgument(L"Id", EMR_CHORD),
                                                    XmlArgument(L"Size", 40)});
            m_pOutputXml->WriteNode(L"Rectangle", oBox);
            m_pOutputXml->WriteNode(L"StartPoint", oStart);
            m_pOutputXml->WriteNode(L"EndPoint", oEnd);
            m_pOutputXml->WriteNodeEnd(L"EMR_CHORD");
    }

    void CEmfInterpretatorXml::Save_EMR_ELLIPSE(const TEmfRectL &oBox)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_ELLIPSE", {XmlArgument(L"Id", EMR_ELLIPSE),
                                                      XmlArgument(L"Size", 24)});
            m_pOutputXml->WriteNode(L"Rectangle", oBox);
            m_pOutputXml->WriteNodeEnd(L"EMR_ELLIPSE");
    }

    void CEmfInterpretatorXml::Save_EMR_EXTTEXTOUTA(const TEmfExtTextoutA &oTEmfExtTextoutA, const unsigned int &unRecordSize)
    {
        m_pOutputXml->WriteNode(L"EMR_EXTTEXTOUTA", oTEmfExtTextoutA, {XmlArgument(L"Id", EMR_EXTTEXTOUTA),
                                                                       XmlArgument(L"Size", unRecordSize + 8)});
    }

    void CEmfInterpretatorXml::Save_EMR_EXTTEXTOUTW(const TEmfExtTextoutW &oTEmfExtTextoutW, const unsigned int &unRecordSize)
    {
        m_pOutputXml->WriteNode(L"EMR_EXTTEXTOUTW", oTEmfExtTextoutW, {XmlArgument(L"Id", EMR_EXTTEXTOUTW),
                                                                       XmlArgument(L"Size", unRecordSize + 8)});
    }

    void CEmfInterpretatorXml::Save_EMR_LINETO(const TEmfPointL &oPoint)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_LINETO", {XmlArgument(L"Id", EMR_LINETO),
                                                     XmlArgument(L"Size",16)});
            m_pOutputXml->WriteNode(L"Point", oPoint);
            m_pOutputXml->WriteNodeEnd(L"EMR_LINETO");
    }

    void CEmfInterpretatorXml::Save_EMR_PIE(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_PIE", {XmlArgument(L"Id", EMR_PIE),
                                                  XmlArgument(L"Size", 40)});
                m_pOutputXml->WriteNode(L"Rectangle",	oBox);
                m_pOutputXml->WriteNode(L"StartPoint",	oStart);
                m_pOutputXml->WriteNode(L"EndPoint",	oEnd);
                m_pOutputXml->WriteNodeEnd(L"EMR_PIE");
    }

    void CEmfInterpretatorXml::Save_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointL>& arPoints)
    {
        Save_EMR_POLY_BASE<TEmfPointL>(RecordData(L"EMR_POLYBEZIER", EMR_POLYBEZIER, oBounds), arPoints);
    }

    void CEmfInterpretatorXml::Save_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointS>& arPoints)
    {
        Save_EMR_POLY_BASE<TEmfPointS>(RecordData(L"EMR_POLYBEZIER16", EMR_POLYBEZIER16, oBounds), arPoints);
    }

    void CEmfInterpretatorXml::Save_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointL>& arPoints)
    {
        Save_EMR_POLY_BASE<TEmfPointL>(RecordData(L"EMR_POLYBEZIERTO", EMR_POLYBEZIERTO, oBounds), arPoints);
    }

    void CEmfInterpretatorXml::Save_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointS>& arPoints)
    {
        Save_EMR_POLY_BASE<TEmfPointS>(RecordData(L"EMR_POLYBEZIERTO16", EMR_POLYBEZIERTO16, oBounds), arPoints);
    }

    void CEmfInterpretatorXml::Save_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointL *arPoints,
                                                 const unsigned int &unCount, const unsigned char *pAbTypes)
    {
        Save_EMR_POLYDRAW_BASE<TEmfPointL>(oBounds, arPoints, unCount, pAbTypes);
    }

    void CEmfInterpretatorXml::Save_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointS *arPoints,
                                                 const unsigned int &unCount, const unsigned char *pAbTypes)
    {
        Save_EMR_POLYDRAW_BASE<TEmfPointS>(oBounds, arPoints, unCount, pAbTypes);
    }

    void CEmfInterpretatorXml::Save_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
    {
        Save_EMR_POLY_BASE<TEmfPointL>(RecordData(L"EMR_POLYGON", EMR_POLYGON, oBounds), arPoints);
    }

    void CEmfInterpretatorXml::Save_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
    {
        Save_EMR_POLY_BASE<TEmfPointS>(RecordData(L"EMR_POLYGON16", EMR_POLYGON16, oBounds), arPoints);
    }

    void CEmfInterpretatorXml::Save_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
    {
        Save_EMR_POLY_BASE<TEmfPointL>(RecordData(L"EMR_POLYLINE", EMR_POLYLINE, oBounds), arPoints);
    }

    void CEmfInterpretatorXml::Save_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
    {
        Save_EMR_POLY_BASE<TEmfPointS>(RecordData(L"EMR_POLYLINE16", EMR_POLYLINE16, oBounds), arPoints);
    }

    void CEmfInterpretatorXml::Save_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
    {
        Save_EMR_POLY_BASE<TEmfPointL>(RecordData(L"EMR_POLYLINETO", EMR_POLYLINETO, oBounds), arPoints);
    }

    void CEmfInterpretatorXml::Save_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
    {
        Save_EMR_POLY_BASE<TEmfPointS>(RecordData(L"EMR_POLYLINETO16", EMR_POLYLINETO16, oBounds), arPoints);
    }

    void CEmfInterpretatorXml::Save_EMR_RECTANGLE(const TEmfRectL& oBox)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_RECTANGLE", {XmlArgument(L"Id", EMR_RECTANGLE),
                                                        XmlArgument(L"Size", 24)});
            m_pOutputXml->WriteNode(L"Rectangle", oBox);
            m_pOutputXml->WriteNodeEnd(L"EMR_RECTANGLE");
    }

    void CEmfInterpretatorXml::Save_EMR_ROUNDRECT(const TEmfRectL &oBox, const TEmfSizeL &oCorner)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_ROUNDRECT", {XmlArgument(L"Id", EMR_ROUNDRECT),
                                                        XmlArgument(L"Size", 32)});
            m_pOutputXml->WriteNode(L"Rectangle",   oBox);
            m_pOutputXml->WriteNode(L"Corner",      oCorner);
            m_pOutputXml->WriteNodeEnd(L"EMR_ROUNDRECT");
    }

    void CEmfInterpretatorXml::Save_EMR_SETPIXELV(const TEmfPointL &oPoint, const TEmfColor &oColor)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETPIXELV", {XmlArgument(L"Id", EMR_SETPIXELV),
                                                        XmlArgument(L"Size", 20)});
            m_pOutputXml->WriteNode(L"Pixel", oPoint);
            m_pOutputXml->WriteNode(L"Color", oColor);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETPIXELV");
    }

    void CEmfInterpretatorXml::Save_EMR_SMALLTEXTOUT(const TEmfSmallTextout &oText, const unsigned int& unRecordSize)
    {
        m_pOutputXml->WriteNode(L"EMR_SMALLTEXTOUT", oText, {XmlArgument(L"Id", EMR_SMALLTEXTOUT),
                                                             XmlArgument(L"Size", unRecordSize + 8)});
    }

    void CEmfInterpretatorXml::Save_EMR_STROKEANDFILLPATH(const TEmfRectL &oBounds)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_STROKEANDFILLPATH", {XmlArgument(L"Id", EMR_STROKEANDFILLPATH),
                                                                XmlArgument(L"Size", 24)});
            m_pOutputXml->WriteNode(L"Bounds", oBounds);
            m_pOutputXml->WriteNodeEnd(L"EMR_STROKEANDFILLPATH");
    }

    void CEmfInterpretatorXml::Save_EMR_STROKEPATH(const TEmfRectL &oBounds)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_STROKEPATH", {XmlArgument(L"Id", EMR_STROKEPATH),
                                                         XmlArgument(L"Size", 24)});
            m_pOutputXml->WriteNode(L"Bounds", oBounds);
            m_pOutputXml->WriteNodeEnd(L"EMR_STROKEPATH");
    }

    template<typename T>
    void CEmfInterpretatorXml::Save_EMR_POLY_BASE(const RecordData &oRecordData, const std::vector<T> &arPoints)
    {
        if (arPoints.empty())
            return;

        unsigned int unNumberPoints = arPoints.size();

	m_pOutputXml->WriteNodeBegin(oRecordData.m_wsName, {XmlArgument(L"Id", oRecordData.m_unId),
						    XmlArgument(L"Size", (unsigned int)(28 + unNumberPoints * sizeof (T)))});
	    m_pOutputXml->WriteNode(L"Bounds",		oRecordData.m_oBounds);
	    m_pOutputXml->WriteNode(L"NumberPoints",    unNumberPoints);

	    for (unsigned int unIndex = 0; unIndex < unNumberPoints; ++unIndex)
		m_pOutputXml->WriteNode(L"Point" + std::to_wstring(unIndex + 1), arPoints[unIndex]);

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


	m_pOutputXml->WriteNodeBegin(wsRecordName, {XmlArgument(L"Id", unRecordId),
						    XmlArgument(L"Size", (unsigned int)(28 + unCount * sizeof (T) + unCount * 1))});
	    m_pOutputXml->WriteNode(L"Bounds",	    oBounds);
	    m_pOutputXml->WriteNode(L"NumberPoints",    unCount);

	    for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
		m_pOutputXml->WriteNode(L"Point" + std::to_wstring(unIndex + 1), arPoints[unIndex]);

	    for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
		m_pOutputXml->WriteNode(L"abTypes" + std::to_wstring(unIndex + 1), pAbTypes[unIndex]);

            m_pOutputXml->WriteNodeEnd(wsRecordName);
    }
}
