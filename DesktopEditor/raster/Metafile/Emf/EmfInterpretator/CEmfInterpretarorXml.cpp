#include "CEmfInterpretarorXml.h"

namespace MetaFile
{
    CEmfInterpretarorXml::CEmfInterpretarorXml() :
        m_pOutputXml(new CXmlOutput(TypeXmlOutput::IsWriter))
    {
        m_pOutputXml->WriteString(L"<?xml version=\"1.0\"?>\n");
        m_pOutputXml->WriteNodeBegin(L"EMF");
    }

    CEmfInterpretarorXml::~CEmfInterpretarorXml()
    {
        if (NULL != m_pOutputXml)
        {
            m_pOutputXml->WriteNodeEnd(L"EMF");
            m_pOutputXml->SaveToFile(NSFile::GetProcessDirectory() + L"/test.xml");
            delete m_pOutputXml;
        }
    }

    void CEmfInterpretarorXml::Save_EMR_HEADER(const TEmfHeader &oTEmfHeader, CDataStream &oDataStream,
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

    void CEmfInterpretarorXml::Save_EMR_ALPHABLEND(const TEmfAlphaBlend &oTEmfAlphaBlend, CDataStream &oDataStream,
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

    void CEmfInterpretarorXml::Save_EMR_STRETCHDIBITS(const TEmfStretchDIBITS &oTEmfStretchDIBITS, CDataStream &oDataStream,
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

    void CEmfInterpretarorXml::Save_EMR_BITBLT(const TEmfBitBlt &oTEmfBitBlt, CDataStream &oDataStream,
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

    void CEmfInterpretarorXml::Save_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice, CDataStream &oDataStream,
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

    void CEmfInterpretarorXml::Save_EMR_STRETCHBLT(const TEmfStretchBLT &oTEmfStretchBLT, CDataStream &oDataStream,
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

    void CEmfInterpretarorXml::Save_EMR_EOF(const unsigned int &unCount,    const unsigned int &unOffset,
                                            const unsigned int &unSizeLast)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_EOF", {XmlArgument(L"Id", EMR_EOF),
                                                  XmlArgument(L"Size", 20)});
            m_pOutputXml->WriteNode(L"nPalEntries",	    unCount);
            m_pOutputXml->WriteNode(L"offPalEntries",   unOffset);
            m_pOutputXml->WriteNode(L"SizeLast",	    unSizeLast);
            m_pOutputXml->WriteNodeEnd(L"EMR_EOF");
    }

    void CEmfInterpretarorXml::Save_EMR_SAVEDC()
    {
        m_pOutputXml->WriteNode(L"EMR_SAVEDC", {XmlArgument(L"Id", EMR_SAVEDC),
                                                XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretarorXml::Save_EMR_RESTOREDC(const int &nIndexDC)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_RESTOREDC", {XmlArgument(L"Id", EMR_RESTOREDC),
                                                        XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"SavedDC", nIndexDC);
            m_pOutputXml->WriteNodeEnd(L"EMR_RESTOREDC");
    }

    void CEmfInterpretarorXml::Save_EMR_MODIFYWORLDTRANSFORM(const TXForm &oXForm, const unsigned int &unMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_MODIFYWORLDTRANSFORM", {XmlArgument(L"Id", EMR_MODIFYWORLDTRANSFORM),
                                                                   XmlArgument(L"Size", 36)});
            m_pOutputXml->WriteNode(L"XForm",			oXForm);
            m_pOutputXml->WriteNode(L"ModifyWorldTransformMode",	unMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_MODIFYWORLDTRANSFORM");
    }

    void CEmfInterpretarorXml::Save_EMR_SETWORLDTRANSFORM(const TXForm &oXForm)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETWORLDTRANSFORM", {XmlArgument(L"Id", EMR_SETWORLDTRANSFORM),
                                                                XmlArgument(L"Size", 32)});
            m_pOutputXml->WriteNode(L"XForm", oXForm);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETWORLDTRANSFORM");
    }

    void CEmfInterpretarorXml::Save_EMR_CREATEBRUSHINDIRECT(const unsigned int &unBrushIndex, const CEmfLogBrushEx *pBrush)
    {
        if (NULL == pBrush)
            return;

        m_pOutputXml->WriteNodeBegin(L"EMR_CREATEBRUSHINDIRECT", {XmlArgument(L"Id", EMR_CREATEBRUSHINDIRECT),
                                                                  XmlArgument(L"Size", 24)});
            m_pOutputXml->WriteNode(L"ihBrush",	unBrushIndex);
            m_pOutputXml->WriteNode(L"LogBrush ",   *pBrush);
            m_pOutputXml->WriteNodeEnd(L"EMR_CREATEBRUSHINDIRECT");
    }

    void CEmfInterpretarorXml::Save_EMR_SETTEXTCOLOR(const TEmfColor &oColor)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETTEXTCOLOR", {XmlArgument(L"Id", EMR_SETTEXTCOLOR),
                                                           XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"Color", oColor);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETTEXTCOLOR");
    }

    void CEmfInterpretarorXml::Save_EMR_SELECTOBJECT(const unsigned int &unObjectIndex)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SELECTOBJECT", {XmlArgument(L"Id", EMR_SELECTOBJECT),
                                                           XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"Index", unObjectIndex);
            m_pOutputXml->WriteNodeEnd(L"EMR_SELECTOBJECT");
    }

    void CEmfInterpretarorXml::Save_EMR_EXTCREATEFONTINDIRECTW(const unsigned int &unIndex, CEmfLogFont *oLogFont,
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

    void CEmfInterpretarorXml::Save_EMR_SETTEXTALIGN(const unsigned int &unAlign)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETTEXTALIGN", {XmlArgument(L"Id", EMR_SETTEXTALIGN),
                                                           XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"TextAlign", unAlign);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETTEXTALIGN");
    }

    void CEmfInterpretarorXml::Save_EMR_SETBKMODE(const unsigned int &unBgMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETBKMODE", {XmlArgument(L"Id", EMR_SETBKMODE),
                                                        XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"BgMode", unBgMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETBKMODE");
    }

    void CEmfInterpretarorXml::Save_EMR_DELETEOBJECT(const unsigned int &unObjectIndex)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_DELETEOBJECT", {XmlArgument(L"Id", EMR_DELETEOBJECT),
                                                           XmlArgument(L"Size",12)});
            m_pOutputXml->WriteNode(L"Index", unObjectIndex);
            m_pOutputXml->WriteNodeEnd(L"EMR_DELETEOBJECT");
    }

    void CEmfInterpretarorXml::Save_EMR_SETMITERLIMIT(const unsigned int &unMeterLimit)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETMITERLIMIT", {XmlArgument(L"Id", EMR_SETMITERLIMIT),
                                                            XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"MiterLimit", unMeterLimit);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETMITERLIMIT");
    }

    void CEmfInterpretarorXml::Save_EMR_EXTCREATEPEN(const unsigned int &unPenIndex, CEmfLogPen *pPen,
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

    void CEmfInterpretarorXml::Save_EMR_CREATEPEN(const unsigned int &unPenIndex, const unsigned int &unWidthX, const CEmfLogPen *pPen)
    {
        if (NULL == pPen)
            return;

        m_pOutputXml->WriteNodeBegin(L"EMR_CREATEPEN", {XmlArgument(L"Id", EMR_CREATEPEN),
                                                        XmlArgument(L"Size", 28)});
            m_pOutputXml->WriteNode(L"ihPen", unPenIndex);
            m_pOutputXml->WriteNodeBegin(L"LogPen");
                m_pOutputXml->WriteNode(L"PenStyle", pPen->PenStyle);
                m_pOutputXml->WriteNode(L"Width", unWidthX);
                m_pOutputXml->WriteNodeEnd(L"LogPen");
            m_pOutputXml->WriteNode(L"COLORREF", pPen->Color);
            m_pOutputXml->WriteNodeEnd(L"EMR_CREATEPEN");
    }

    void CEmfInterpretarorXml::Save_EMR_SETPOLYFILLMODE(const unsigned int &unFillMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETPOLYFILLMODE", {XmlArgument(L"Id", EMR_SETPOLYFILLMODE),
                                                              XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"PolygonFillMode", unFillMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETPOLYFILLMODE");
    }

    void CEmfInterpretarorXml::Save_EMR_BEGINPATH()
    {
        m_pOutputXml->WriteNode(L"EMR_BEGINPATH", {XmlArgument(L"Id", EMR_BEGINPATH),
                                                   XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretarorXml::Save_EMR_ENDPATH()
    {
        m_pOutputXml->WriteNode(L"EMR_ENDPATH", {XmlArgument(L"Id", EMR_ENDPATH),
                                                 XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretarorXml::Save_EMR_CLOSEFIGURE()
    {
        m_pOutputXml->WriteNode(L"EMR_CLOSEFIGURE", {XmlArgument(L"Id", EMR_CLOSEFIGURE),
                                                     XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretarorXml::Save_EMR_FLATTENPATH()
    {
        m_pOutputXml->WriteNode(L"EMR_FLATTENPATH", {XmlArgument(L"Id", EMR_FLATTENPATH),
                                                     XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretarorXml::Save_EMR_WIDENPATH()
    {
        m_pOutputXml->WriteNode(L"EMR_WIDENPATH", {XmlArgument(L"Id", EMR_WIDENPATH),
                                                   XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretarorXml::Save_EMR_ABORTPATH()
    {
        m_pOutputXml->WriteNode(L"EMR_ABORTPATH", {XmlArgument(L"Id", EMR_ABORTPATH),
                                                   XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretarorXml::Save_EMR_MOVETOEX(const TEmfPointL &oPoint)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_MOVETOEX", {XmlArgument(L"Id", EMR_MOVETOEX),
                                                       XmlArgument(L"Size", 16)});
            m_pOutputXml->WriteNode(L"Offset", oPoint);
            m_pOutputXml->WriteNodeEnd(L"EMR_MOVETOEX");
    }

    void CEmfInterpretarorXml::Save_EMR_SETARCDIRECTION(const unsigned int &unDirection)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETARCDIRECTION", {XmlArgument(L"Id", EMR_SETARCDIRECTION),
                                                              XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"arcDirection", unDirection);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETARCDIRECTION");
    }

    void CEmfInterpretarorXml::Save_EMR_FILLPATH(const TEmfRectL &oBounds)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_FILLPATH", {XmlArgument(L"Id", EMR_FILLPATH),
                                                       XmlArgument(L"Size", 24)});
            m_pOutputXml->WriteNode(L"Bounds", oBounds);
            m_pOutputXml->WriteNodeEnd(L"EMR_FILLPATH");
    }

    void CEmfInterpretarorXml::Save_EMR_SETMAPMODE(const unsigned int &unMapMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETMAPMODE", {XmlArgument(L"Id", EMR_SETMAPMODE),
                                                         XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"MapMode", unMapMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETMAPMODE");
    }

    void CEmfInterpretarorXml::Save_EMR_SETWINDOWORGEX(const TEmfPointL &oOrigin)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETWINDOWORGEX", {XmlArgument(L"Id", EMR_SETWINDOWORGEX),
                                                             XmlArgument(L"Size", 16)});
            m_pOutputXml->WriteNode(L"Origin", oOrigin);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETWINDOWORGEX");
    }

    void CEmfInterpretarorXml::Save_EMR_SETWINDOWEXTEX(const TEmfSizeL &oExtent)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETWINDOWEXTEX", {XmlArgument(L"Id", EMR_SETWINDOWEXTEX),
                                                             XmlArgument(L"Size", 16)});
            m_pOutputXml->WriteNode(L"Extent", oExtent);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETWINDOWEXTEX");
    }

    void CEmfInterpretarorXml::Save_EMR_SETVIEWPORTORGEX(const TEmfPointL &oOrigin)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETVIEWPORTORGEX", {XmlArgument(L"Id", EMR_SETVIEWPORTORGEX),
                                                               XmlArgument(L"Size", 16)});
            m_pOutputXml->WriteNode(L"Origin", oOrigin);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETVIEWPORTORGEX");
    }

    void CEmfInterpretarorXml::Save_EMR_SETVIEWPORTEXTEX(const TEmfSizeL &oExtent)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETVIEWPORTEXTEX", {XmlArgument(L"Id", EMR_SETVIEWPORTEXTEX),
                                                               XmlArgument(L"Size", 16)});
            m_pOutputXml->WriteNode(L"Extent", oExtent);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETVIEWPORTEXTEX");
    }

    void CEmfInterpretarorXml::Save_EMR_SETSTRETCHBLTMODE(const unsigned int &unStretchMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETSTRETCHBLTMODE", {XmlArgument(L"Id", EMR_SETSTRETCHBLTMODE),
                                                                XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"StretchMode", unStretchMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETSTRETCHBLTMODE");
    }

    void CEmfInterpretarorXml::Save_EMR_SETICMMODE(const unsigned int &unICMMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETICMMODE", {XmlArgument(L"Id", EMR_SETICMMODE),
                                                         XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"ICMMode", unICMMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETICMMODE");
    }

    void CEmfInterpretarorXml::Save_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush,
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

    void CEmfInterpretarorXml::Save_EMR_SELECTCLIPPATH(const unsigned int &unRegionMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SELECTCLIPPATH", {XmlArgument(L"Id", EMR_SELECTCLIPPATH),
                                                             XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"RegionMode", unRegionMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SELECTCLIPPATH", true);
    }

    void CEmfInterpretarorXml::Save_EMR_SETBKCOLOR(const TEmfColor &oColor)
    {
        m_pOutputXml->WriteNode(L"EMR_SETBKCOLOR", oColor, {XmlArgument(L"Id", EMR_SETBKCOLOR),
                                                            XmlArgument(L"Size", 12)});
    }

    void CEmfInterpretarorXml::Save_EMR_EXCLUDECLIPRECT(const TEmfRectL &oClip)
    {
        m_pOutputXml->WriteNode(L"EMR_EXCLUDECLIPRECT", oClip, {XmlArgument(L"Id", EMR_EXCLUDECLIPRECT),
                                                                XmlArgument(L"Size", 20)});
    }

    void CEmfInterpretarorXml::Save_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode,
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

    void CEmfInterpretarorXml::Save_EMR_SETMETARGN()
    {
        m_pOutputXml->WriteNode(L"EMR_SETMETARGN", {XmlArgument(L"Id", EMR_SETMETARGN),
                                                    XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretarorXml::Save_EMR_SETROP2(const unsigned int &unRop2Mode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETROP2", {XmlArgument(L"Id", EMR_SETROP2),
                                                      XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"Rop2Mode", unRop2Mode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETROP2");
    }

    void CEmfInterpretarorXml::Save_EMR_CREATEPALETTE(const unsigned int &unPaletteIndex, const CEmfLogPalette *oEmfLogPalette, const unsigned int &unRecordSize)
    {
        if (NULL == oEmfLogPalette)
            return;

        m_pOutputXml->WriteNodeBegin(L"EMR_CREATEPALETTE", {XmlArgument(L"Id", EMR_CREATEPALETTE),
                                                            XmlArgument(L"Size", unRecordSize + 8)});
            m_pOutputXml->WriteNode(L"ihPal", unPaletteIndex);
            m_pOutputXml->WriteNode(L"LogPalette", *oEmfLogPalette);
            m_pOutputXml->WriteNodeEnd(L"EMR_CREATEPALETTE");
    }

    void CEmfInterpretarorXml::Save_EMR_SELECTPALETTE(const unsigned int &unPaletteIndex)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SELECTPALETTE", {XmlArgument(L"Id", EMR_SELECTPALETTE),
                                                            XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"ihPal", unPaletteIndex);
            m_pOutputXml->WriteNodeEnd(L"EMR_SELECTPALETTE");
    }

    void CEmfInterpretarorXml::Save_EMR_REALIZEPALETTE()
    {
        m_pOutputXml->WriteNode(L"EMR_REALIZEPALETTE", {XmlArgument(L"Id", EMR_REALIZEPALETTE),
                                                        XmlArgument(L"Size", 8)});
    }

    void CEmfInterpretarorXml::Save_EMR_INTERSECTCLIPRECT(const TEmfRectL &oClip)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_INTERSECTCLIPRECT", {XmlArgument(L"Id", EMR_INTERSECTCLIPRECT),
                                                                XmlArgument(L"Size", 20)});
            m_pOutputXml->WriteNode(L"Clip", oClip);
            m_pOutputXml->WriteNodeEnd(L"EMR_INTERSECTCLIPRECT");
    }

    void CEmfInterpretarorXml::Save_EMR_SETLAYOUT(const unsigned int &unLayoutMode)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETLAYOUT", {XmlArgument(L"Id", EMR_SETLAYOUT),
                                                        XmlArgument(L"Size", 12)});
            m_pOutputXml->WriteNode(L"LayoutMode", unLayoutMode);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETLAYOUT");
    }

    void CEmfInterpretarorXml::Save_EMR_SETBRUSHORGEX(const TEmfPointL &oOrigin)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_SETBRUSHORGEX", {XmlArgument(L"Id", EMR_SETBRUSHORGEX),
                                                            XmlArgument(L"Size",16)});
            m_pOutputXml->WriteNode(L"Origin", oOrigin);
            m_pOutputXml->WriteNodeEnd(L"EMR_SETBRUSHORGEX");
    }

    void CEmfInterpretarorXml::Save_EMR_ANGLEARC(const TEmfPointL &oCenter, const unsigned int &unRadius,
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

    void CEmfInterpretarorXml::Save_EMR_ARC(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_ARC", {XmlArgument(L"Id", EMR_ARC),
                                                  XmlArgument(L"Size", 40)});
             m_pOutputXml->WriteNode(L"Rectangle", oBox);
             m_pOutputXml->WriteNode(L"StartPoint", oStart);
             m_pOutputXml->WriteNode(L"EndPoint", oEnd);
             m_pOutputXml->WriteNodeEnd(L"EMR_ARC");
    }

    void CEmfInterpretarorXml::Save_EMR_ARCTO(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_ARCTO", {XmlArgument(L"Id", EMR_ARCTO),
                                                    XmlArgument(L"Size", 40)});
            m_pOutputXml->WriteNode(L"Rectangle", oBox);
            m_pOutputXml->WriteNode(L"StartPoint", oStart);
            m_pOutputXml->WriteNode(L"EndPoint", oEnd);
            m_pOutputXml->WriteNodeEnd(L"EMR_ARCTO");
    }

    void CEmfInterpretarorXml::Save_EMR_CHORD(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_CHORD", {XmlArgument(L"Id", EMR_CHORD),
                                                    XmlArgument(L"Size", 40)});
            m_pOutputXml->WriteNode(L"Rectangle", oBox);
            m_pOutputXml->WriteNode(L"StartPoint", oStart);
            m_pOutputXml->WriteNode(L"EndPoint", oEnd);
            m_pOutputXml->WriteNodeEnd(L"EMR_CHORD");
    }

    void CEmfInterpretarorXml::Save_EMR_ELLIPSE(const TEmfRectL &oBox)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_ELLIPSE", {XmlArgument(L"Id", EMR_ELLIPSE),
                                                      XmlArgument(L"Size", 24)});
            m_pOutputXml->WriteNode(L"Rectangle", oBox);
            m_pOutputXml->WriteNodeEnd(L"EMR_ELLIPSE");
    }
}
