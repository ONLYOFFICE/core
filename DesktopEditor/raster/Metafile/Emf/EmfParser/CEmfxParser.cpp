#include "CEmfxParser.h"

#include <iostream>

namespace MetaFile
{
    CEmfxParser::CEmfxParser():
        m_pOutput(new CXmlOutput(TypeXmlOutput::IsReader))
    {}

    CEmfxParser::~CEmfxParser()
    {
        delete m_pOutput;
    }

    bool CEmfxParser::OpenFromFile(const wchar_t *wsFilePath)
    {
        return m_pOutput->ReadFromFile(wsFilePath);
    }

    void CEmfxParser::PlayFile()
    {
        bool bEOF = false;

	if (NULL != m_pInterpretator)
		m_pInterpretator->Begin();

        do
        {
                unsigned int unRecorId;
                m_pOutput->ReadArguments(unRecorId, m_ulRecordSize);

                m_ulRecordSize -= 8;

                if (m_ulRecordSize < 0)
                    continue;

                if (unRecorId < EMR_MIN || unRecorId > EMR_MAX)
                {
                        if (ENHMETA_SIGNATURE != m_oHeader.ulSignature || 0x00010000 != m_oHeader.ulVersion)
                                return SetError();
                        else
                                break;
                }

		if (0 == unRecorId && EMR_HEADER != unRecorId)
			return SetError();

		switch (unRecorId)
		{
			//-----------------------------------------------------------
			// 2.3.1 Bitmap
			//-----------------------------------------------------------
			case EMR_ALPHABLEND:        Read_EMR_ALPHABLEND(); break;
			case EMR_BITBLT:            Read_EMR_BITBLT(); break;
			case EMR_STRETCHDIBITS:     Read_EMR_STRETCHDIBITS(); break;
			case EMR_SETDIBITSTODEVICE: Read_EMR_SETDIBITSTODEVICE(); break;
			case EMR_STRETCHBLT:        Read_EMR_STRETCHBLT(); break;
				//-----------------------------------------------------------
				// 2.3.2 Clipping
				//-----------------------------------------------------------
			case EMR_EXCLUDECLIPRECT:   Read_EMR_EXCLUDECLIPRECT(); break;
			case EMR_EXTSELECTCLIPRGN:  Read_EMR_EXTSELECTCLIPRGN(); break;
			case EMR_INTERSECTCLIPRECT: Read_EMR_INTERSECTCLIPRECT(); break;
			case EMR_SELECTCLIPPATH:    Read_EMR_SELECTCLIPPATH(); break;
			case EMR_SETMETARGN:        Read_EMR_SETMETARGN(); break;
				//-----------------------------------------------------------
				// 2.3.4 Control
				//-----------------------------------------------------------
			case EMR_HEADER: Read_EMR_HEADER(); break;
			case EMR_EOF:    Read_EMR_EOF(); bEOF = true; break;
				//-----------------------------------------------------------
				// 2.3.5 Drawing
				//-----------------------------------------------------------
			case EMR_ANGLEARC:          Read_EMR_ANGLEARC(); break;
			case EMR_ARC:		    Read_EMR_ARC(); break;
			case EMR_ARCTO:             Read_EMR_ARCTO(); break;
			case EMR_CHORD:             Read_EMR_CHORD(); break;
			case EMR_ELLIPSE:           Read_EMR_ELLIPSE(); break;
			case EMR_EXTTEXTOUTA:       Read_EMR_EXTTEXTOUTA(); break;
			case EMR_EXTTEXTOUTW:       Read_EMR_EXTTEXTOUTW(); break;
			case EMR_FILLPATH:          Read_EMR_FILLPATH(); break;
			case EMR_LINETO:            Read_EMR_LINETO(); break;
			case EMR_PIE:		    Read_EMR_PIE(); break;
			case EMR_POLYBEZIER:        Read_EMR_POLYBEZIER(); break;
			case EMR_POLYBEZIER16:      Read_EMR_POLYBEZIER16(); break;
			case EMR_POLYBEZIERTO:      Read_EMR_POLYBEZIERTO(); break;
			case EMR_POLYBEZIERTO16:    Read_EMR_POLYBEZIERTO16(); break;
			case EMR_POLYDRAW:          Read_EMR_POLYDRAW(); break;
			case EMR_POLYDRAW16:        Read_EMR_POLYDRAW16(); break;
			case EMR_POLYGON:           Read_EMR_POLYGON(); break;
			case EMR_POLYGON16:         Read_EMR_POLYGON16(); break;
			case EMR_POLYLINE:          Read_EMR_POLYLINE(); break;
			case EMR_POLYLINE16:        Read_EMR_POLYLINE16(); break;
			case EMR_POLYLINETO:        Read_EMR_POLYLINETO(); break;
			case EMR_POLYLINETO16:      Read_EMR_POLYLINETO16(); break;
			case EMR_POLYPOLYGON:       Read_EMR_POLYPOLYGON(); break;
			case EMR_POLYPOLYGON16:     Read_EMR_POLYPOLYGON16(); break;
			case EMR_POLYPOLYLINE:      Read_EMR_POLYPOLYLINE(); break;
			case EMR_POLYPOLYLINE16:    Read_EMR_POLYPOLYLINE16(); break;
			case EMR_POLYTEXTOUTA:      Read_EMR_POLYTEXTOUTA(); break;
			case EMR_POLYTEXTOUTW:      Read_EMR_POLYTEXTOUTW(); break;
			case EMR_RECTANGLE:         Read_EMR_RECTANGLE(); break;
			case EMR_ROUNDRECT:         Read_EMR_ROUNDRECT(); break;
			case EMR_SETPIXELV:         Read_EMR_SETPIXELV(); break;
			case EMR_SMALLTEXTOUT:      Read_EMR_SMALLTEXTOUT(); break;
			case EMR_STROKEANDFILLPATH: Read_EMR_STROKEANDFILLPATH(); break;
			case EMR_STROKEPATH:        Read_EMR_STROKEPATH(); break;
				//-----------------------------------------------------------
				// 2.3.7 Object Creation
				//-----------------------------------------------------------
			case EMR_CREATEBRUSHINDIRECT:	    Read_EMR_CREATEBRUSHINDIRECT(); break;
			case EMR_CREATEDIBPATTERNBRUSHPT:   Read_EMR_CREATEDIBPATTERNBRUSHPT(); break;
			case EMR_CREATEPALETTE:		    Read_EMR_CREATEPALETTE(); break;
			case EMR_CREATEPEN:		    Read_EMR_CREATEPEN(); break;
			case EMR_EXTCREATEFONTINDIRECTW:    Read_EMR_EXTCREATEFONTINDIRECTW(); break;
			case EMR_EXTCREATEPEN:		    Read_EMR_EXTCREATEPEN(); break;
				//-----------------------------------------------------------
				// 2.3.8 Object Manipulation
				//-----------------------------------------------------------
			case EMR_SELECTOBJECT:  Read_EMR_SELECTOBJECT(); break;
			case EMR_DELETEOBJECT:  Read_EMR_DELETEOBJECT(); break;
			case EMR_SELECTPALETTE: Read_EMR_SELECTPALETTE(); break;
				//-----------------------------------------------------------
				// 2.3.10 Path Bracket
				//-----------------------------------------------------------
			case EMR_BEGINPATH:   Read_EMR_BEGINPATH(); break;
			case EMR_ENDPATH:     Read_EMR_ENDPATH(); break;
			case EMR_CLOSEFIGURE: Read_EMR_CLOSEFIGURE(); break;
			case EMR_FLATTENPATH: Read_EMR_FLATTENPATH(); break;
			case EMR_WIDENPATH:   Read_EMR_WIDENPATH(); break;
			case EMR_ABORTPATH:   Read_EMR_ABORTPATH(); break;
				//-----------------------------------------------------------
				// 2.3.11 State
				//-----------------------------------------------------------
			case EMR_MOVETOEX:          Read_EMR_MOVETOEX(); break;
			case EMR_SETARCDIRECTION:   Read_EMR_SETARCDIRECTION(); break;
			case EMR_SAVEDC:            Read_EMR_SAVEDC(); break;
			case EMR_RESTOREDC:         Read_EMR_RESTOREDC(); break;
			case EMR_SETTEXTCOLOR:      Read_EMR_SETTEXTCOLOR(); break;
			case EMR_SETTEXTALIGN:      Read_EMR_SETTEXTALIGN(); break;
			case EMR_SETBKMODE:         Read_EMR_SETBKMODE(); break;
			case EMR_SETMITERLIMIT:     Read_EMR_SETMITERLIMIT(); break;
			case EMR_SETPOLYFILLMODE:   Read_EMR_SETPOLYFILLMODE(); break;
			case EMR_SETMAPMODE:        Read_EMR_SETMAPMODE(); break;
			case EMR_SETWINDOWORGEX:    Read_EMR_SETWINDOWORGEX(); break;
			case EMR_SETWINDOWEXTEX:    Read_EMR_SETWINDOWEXTEX(); break;
			case EMR_SETVIEWPORTORGEX:  Read_EMR_SETVIEWPORTORGEX(); break;
			case EMR_SETVIEWPORTEXTEX:  Read_EMR_SETVIEWPORTEXTEX(); break;
			case EMR_SETBKCOLOR:        Read_EMR_SETBKCOLOR(); break;
			case EMR_SETSTRETCHBLTMODE: Read_EMR_SETSTRETCHBLTMODE(); break;
			case EMR_SETICMMODE:        Read_EMR_SETICMMODE(); break;
			case EMR_SETROP2:           Read_EMR_SETROP2(); break;
			case EMR_REALIZEPALETTE:    Read_EMR_REALIZEPALETTE(); break;
			case EMR_SETLAYOUT:         Read_EMR_SETLAYOUT(); break;
			case EMR_SETBRUSHORGEX:     Read_EMR_SETBRUSHORGEX(); break;
				//-----------------------------------------------------------
				// 2.3.12 Transform
				//-----------------------------------------------------------
			case EMR_SETWORLDTRANSFORM:	Read_EMR_SETWORLDTRANSFORM(); break;
			case EMR_MODIFYWORLDTRANSFORM:	Read_EMR_MODIFYWORLDTRANSFORM(); break;
				//-----------------------------------------------------------
				// Неподдерживаемые записи
				//-----------------------------------------------------------
			case EMR_GDICOMMENT: Read_EMR_UNKNOWN(); break;
				//-----------------------------------------------------------
				// Неизвестные записи
				//-----------------------------------------------------------
			default:
			{
				Read_EMR_UNKNOWN();
				break;
			}
		}

	}while (!CheckError() && ! bEOF && m_pOutput->ReadNextNode());

	if (!CheckError())
		m_pOutput->MoveToStart();

	if (NULL != m_pInterpretator)
		m_pInterpretator->End();
    }

    void CEmfxParser::Scan()
    {
        m_pOutput->ReadNextNode();

        if (m_pOutput->GetName() != L"EMF")
            return SetError();

        unsigned int unRecordSize, unRecordId;
        m_pOutput->ReadArguments(unRecordId, unRecordSize);

        if (unRecordId < EMR_MIN || unRecordId > EMR_MAX ||
            unRecordSize < 8)
            return SetError();

        *m_pOutput >> m_oHeader;

	double dL = m_oHeader.oFrame.lLeft / 100.0 / m_oHeader.oMillimeters.cx * m_oHeader.oDevice.cx;
	double dR = m_oHeader.oFrame.lRight / 100.0 / m_oHeader.oMillimeters.cx * m_oHeader.oDevice.cx;
	double dT = m_oHeader.oFrame.lTop / 100.0 / m_oHeader.oMillimeters.cy * m_oHeader.oDevice.cy;
	double dB = m_oHeader.oFrame.lBottom / 100.0 / m_oHeader.oMillimeters.cy * m_oHeader.oDevice.cy;

	double dW = dR - dL;
	double dH = dB - dT;

	int nL = (int)floor(dL + 0.5);
	int nT = (int)floor(dT + 0.5);
	int nR = (int)floor(dW + 0.5) + nL;
	int nB = (int)floor(dH + 0.5) + nT;

	// По логике мы должны получать рект, точно такой же как и oBounds, но есть файлы, где это не так.
	m_oHeader.oFrameToBounds.nLeft   = nL;
	m_oHeader.oFrameToBounds.nRight  = nR;
	m_oHeader.oFrameToBounds.nTop    = nT;
	m_oHeader.oFrameToBounds.nBottom = nB;

	m_oHeader.oFramePx = m_oHeader.oFrameToBounds;

        m_pOutput->MoveToStart();
    }

    EmfParserType CEmfxParser::GetType()
    {
        return EmfParserType::EmfxParser;
    }

    bool CEmfxParser::ReadImage(unsigned int offBmi, unsigned int cbBmi, unsigned int offBits, unsigned int cbBits, unsigned int ulSkip, BYTE **ppBgraBuffer, unsigned int *pulWidth, unsigned int *pulHeight)
    {
        int lHeaderOffset         = offBmi - ulSkip;
        unsigned int ulHeaderSize = cbBmi;
        int lBitsOffset           = offBits - offBmi - cbBmi;
        unsigned int ulBitsSize   = cbBits;

        if (ulHeaderSize <= 0 || ulBitsSize <= 0 || lHeaderOffset < 0 || lBitsOffset < 0)
            return false;

        m_oStream.Skip(lHeaderOffset);

        BYTE* pHeaderBuffer = m_oStream.GetCurPtr();
        m_oStream.Skip(ulHeaderSize + lBitsOffset);
        BYTE* pBitsBuffer = m_oStream.GetCurPtr();

        unsigned int ulBitsSizeSkip = 0 == ulBitsSize ? 0 : ((int)(((double)ulBitsSize - 0.5) / 4) + 1) * 4;
        m_oStream.Skip(ulBitsSizeSkip);

        MetaFile::ReadImage(pHeaderBuffer, ulHeaderSize, pBitsBuffer, ulBitsSize, ppBgraBuffer, pulWidth, pulHeight);

        return true;
    }

    void CEmfxParser::Read_EMR_HEADER()
    {
        *m_pOutput >> m_oHeader;

        if (NULL != m_pInterpretator)
                m_pInterpretator->HANDLER_EMR_HEADER(m_oHeader, m_oStream, m_ulRecordSize);

	if (ENHMETA_SIGNATURE != m_oHeader.ulSignature || 0x00010000 != m_oHeader.ulVersion)
		return SetError();

	double dL = m_oHeader.oFrame.lLeft / 100.0 / m_oHeader.oMillimeters.cx * m_oHeader.oDevice.cx;
	double dR = m_oHeader.oFrame.lRight / 100.0 / m_oHeader.oMillimeters.cx * m_oHeader.oDevice.cx;
	double dT = m_oHeader.oFrame.lTop / 100.0 / m_oHeader.oMillimeters.cy * m_oHeader.oDevice.cy;
	double dB = m_oHeader.oFrame.lBottom / 100.0 / m_oHeader.oMillimeters.cy * m_oHeader.oDevice.cy;

	double dW = dR - dL;
	double dH = dB - dT;

	int nL = (int)floor(dL + 0.5);
	int nT = (int)floor(dT + 0.5);
	int nR = (int)floor(dW + 0.5) + nL;
	int nB = (int)floor(dH + 0.5) + nT;

	// По логике мы должны получать рект, точно такой же как и oBounds, но есть файлы, где это не так.
	m_oHeader.oFrameToBounds.nLeft   = nL;
	m_oHeader.oFrameToBounds.nRight  = nR;
	m_oHeader.oFrameToBounds.nTop    = nT;
	m_oHeader.oFrameToBounds.nBottom = nB;

        m_oHeader.oFramePx = m_oHeader.oFrameToBounds;
    }

    void CEmfxParser::Read_EMR_ALPHABLEND()
    {
        TEmfAlphaBlend oBitmap;

        *m_pOutput >> oBitmap;

        if (oBitmap.cbBitsSrc > 0)
            *m_pOutput >> m_oStream;

	if (NULL != m_pInterpretator)
		m_pInterpretator->HANDLER_EMR_ALPHABLEND(oBitmap, m_oStream, m_ulRecordSize);

        ImageProcessing(oBitmap);
    }

    void CEmfxParser::Read_EMR_STRETCHDIBITS()
    {
        TEmfStretchDIBITS oBitmap;

        *m_pOutput >> oBitmap;

        if (oBitmap.cbBitsSrc > 0)
            *m_pOutput >> m_oStream;

	if (NULL != m_pInterpretator)
		m_pInterpretator->HANDLER_EMR_STRETCHDIBITS(oBitmap, m_oStream, m_ulRecordSize);

        ImageProcessing(oBitmap);
    }

    void CEmfxParser::Read_EMR_BITBLT()
    {
        TEmfBitBlt oBitmap;

        *m_pOutput >> oBitmap;

        if (oBitmap.cbBitsSrc > 0)
            *m_pOutput >> m_oStream;

	if (NULL != m_pInterpretator)
		m_pInterpretator->HANDLER_EMR_BITBLT(oBitmap, m_oStream, m_ulRecordSize);

        ImageProcessing(oBitmap);
    }

    void CEmfxParser::Read_EMR_SETDIBITSTODEVICE()
    {
        TEmfSetDiBitsToDevice oBitmap;

        *m_pOutput >> oBitmap;

	if (NULL != m_pInterpretator)
		m_pInterpretator->HANDLER_EMR_SETDIBITSTODEVICE(oBitmap, m_oStream, m_ulRecordSize);

        ImageProcessing(oBitmap);
    }

    void CEmfxParser::Read_EMR_STRETCHBLT()
    {
        TEmfStretchBLT oBitmap;

        *m_pOutput >> oBitmap;

        if (oBitmap.cbBitsSrc > 0)
            *m_pOutput >> m_oStream;

	if (NULL != m_pInterpretator)
		m_pInterpretator->HANDLER_EMR_STRETCHBLT(oBitmap, m_oStream, m_ulRecordSize);

        ImageProcessing(oBitmap);
    }

    void CEmfxParser::Read_EMR_EOF()
    {
        unsigned int ulCount, ulOffset, ulSizeLast;

	*m_pOutput >> ulCount;
	*m_pOutput >> ulOffset;
	*m_pOutput >> ulSizeLast;

	if (NULL != m_pInterpretator)
	    m_pInterpretator->HANDLER_EMR_EOF(ulCount, ulOffset, ulSizeLast);
    }

    void CEmfxParser::Read_EMR_UNKNOWN()
    {
        m_pOutput->ReadNextRecord();
    }

    void CEmfxParser::Read_EMR_SAVEDC()
    {
        m_pDC = m_oPlayer.SaveDC();

	if (NULL != m_pInterpretator)
	    m_pInterpretator->HANDLER_EMR_SAVEDC();
    }

    void CEmfxParser::Read_EMR_RESTOREDC()
    {
        int lSavedDC;

        *m_pOutput >> lSavedDC;

	if (NULL != m_pInterpretator)
		m_pInterpretator->HANDLER_EMR_RESTOREDC(lSavedDC);

	if (lSavedDC >= 0)
	{
		SetError();
		return;
	}

	int lCount = -lSavedDC;
	for (int lIndex = 0; lIndex < lCount; lIndex++)
		m_oPlayer.RestoreDC();

	m_pDC = m_oPlayer.GetDC();
	UpdateOutputDC();
    }

    void CEmfxParser::Read_EMR_MODIFYWORLDTRANSFORM()
    {
            TEmfXForm oXForm;
            unsigned int ulMode;

	    *m_pOutput >> oXForm;
	    *m_pOutput >> ulMode;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_MODIFYWORLDTRANSFORM(oXForm, ulMode);

	    m_pDC->MultiplyTransform(oXForm, ulMode);
	    UpdateOutputDC();
    }
    void CEmfxParser::Read_EMR_SETWORLDTRANSFORM()
    {
            TEmfXForm oXForm;

            *m_pOutput >> oXForm;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETWORLDTRANSFORM(oXForm);

	    m_pDC->MultiplyTransform(oXForm, MWT_SET);
	    UpdateOutputDC();
    }
    void CEmfxParser::Read_EMR_CREATEBRUSHINDIRECT()
    {
            unsigned int ulBrushIndex;
            CEmfLogBrushEx* pBrush = new CEmfLogBrushEx();
            if (!pBrush)
                    return SetError();

	    *m_pOutput >> ulBrushIndex;
	    *m_pOutput >> *pBrush;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_CREATEBRUSHINDIRECT(ulBrushIndex, pBrush);

            m_oPlayer.RegisterObject(ulBrushIndex, (CEmfObjectBase*)pBrush);
    }
    void CEmfxParser::Read_EMR_SETTEXTCOLOR()
    {
            TEmfColor oColor;

            *m_pOutput >> oColor;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETTEXTCOLOR(oColor);

	    m_pDC->SetTextColor(oColor);
	    UpdateOutputDC();
    }
    void CEmfxParser::Read_EMR_SELECTOBJECT()
    {
            unsigned int ulObjectIndex;

            *m_pOutput >> ulObjectIndex;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SELECTOBJECT(ulObjectIndex);

	    m_oPlayer.SelectObject(ulObjectIndex);
	    UpdateOutputDC();
    }
    void CEmfxParser::Read_EMR_EXTCREATEFONTINDIRECTW()
    {
            unsigned int unSize = m_ulRecordSize - 4;
            bool bFixedLength = unSize <= 0x0140 ? true : false;

	    unsigned int ulIndex;
	    CEmfLogFont* pFont = new CEmfLogFont(bFixedLength);
	    if (!pFont)
		    return SetError();

	    *m_pOutput >> ulIndex;
	    *m_pOutput >> *pFont;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_EXTCREATEFONTINDIRECTW(ulIndex, pFont, m_ulRecordSize);

            m_oPlayer.RegisterObject(ulIndex, (CEmfObjectBase*)pFont);
    }
    void CEmfxParser::Read_EMR_SETTEXTALIGN()
    {
            unsigned int ulAlign;

            *m_pOutput >> ulAlign;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETTEXTALIGN(ulAlign);

	    m_pDC->SetTextAlign(ulAlign);
	    UpdateOutputDC();
    }
    void CEmfxParser::Read_EMR_SETBKMODE()
    {
            unsigned int ulBgMode;

            *m_pOutput >> ulBgMode;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETBKMODE(ulBgMode);

	    m_pDC->SetBgMode(ulBgMode);
	    UpdateOutputDC();
    }
    void CEmfxParser::Read_EMR_DELETEOBJECT()
    {
            unsigned int ulIndex;

            *m_pOutput >> ulIndex;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_DELETEOBJECT(ulIndex);

	    m_oPlayer.DeleteObject(ulIndex);
	    UpdateOutputDC();
    }
    void CEmfxParser::Read_EMR_SETMITERLIMIT()
    {
            unsigned int ulMiterLimit;

            *m_pOutput >> ulMiterLimit;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETMITERLIMIT(ulMiterLimit);

	    m_pDC->SetMiterLimit(ulMiterLimit);
	    UpdateOutputDC();
    }
    void CEmfxParser::Read_EMR_EXTCREATEPEN()
    {
            unsigned int ulPenIndex;

            *m_pOutput >> ulPenIndex;

            unsigned int current_size = m_ulRecordSize - 20;

	    CEmfLogPen* pPen = new CEmfLogPen();
	    if (!pPen)
		    return SetError();

	    m_pOutput->ReadNextNode();

	    *m_pOutput >> pPen->PenStyle;
	    *m_pOutput >> pPen->Width;
	    *m_pOutput >> pPen->Color;

	    *m_pOutput >> pPen->NumStyleEntries;

	    current_size -= 24;
	    if (pPen->NumStyleEntries > 0)
	    {
		    current_size -= pPen->NumStyleEntries * 4;
		    pPen->StyleEntry = new unsigned int[pPen->NumStyleEntries];
		    if (!pPen->StyleEntry)
		    {
			    delete pPen;
			    return SetError();
		    }

		    for (unsigned int ulIndex = 0; ulIndex < pPen->NumStyleEntries; ulIndex++)
			    *m_pOutput >> pPen->StyleEntry[ulIndex];
	    }
	    else
	    {
		    pPen->StyleEntry = NULL;
	    }

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_EXTCREATEPEN(ulPenIndex, pPen, m_ulRecordSize);

            m_oPlayer.RegisterObject(ulPenIndex, (CEmfObjectBase*)pPen);
    }
    void CEmfxParser::Read_EMR_CREATEPEN()
    {
            unsigned int ulPenIndex;

            *m_pOutput >> ulPenIndex;

	    CEmfLogPen* pPen = new CEmfLogPen();
	    if (!pPen)
		    return SetError();


	    m_pOutput->ReadNextNode();

	    *m_pOutput >> pPen->PenStyle;

	    unsigned int widthX, widthY;

	    *m_pOutput >> widthX;
	    *m_pOutput >> widthY;
	    *m_pOutput >> pPen->Color;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_CREATEPEN(ulPenIndex, widthX, pPen);

	    if (!widthX)
	    {//emf from Bonetti Martínez. cálculo estructural de pilotes y pilas.xlsx
		    widthX = 1 / m_pDC->GetPixelWidth();
	    }
	    pPen->Width = widthX;

            m_oPlayer.RegisterObject(ulPenIndex, (CEmfObjectBase*)pPen);
    }
    void CEmfxParser::Read_EMR_SETPOLYFILLMODE()
    {
            unsigned int ulFillMode;

            *m_pOutput >> ulFillMode;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETPOLYFILLMODE(ulFillMode);

	    m_pDC->SetFillMode(ulFillMode);
	    UpdateOutputDC();
    }
    void CEmfxParser::Read_EMR_BEGINPATH()
    {
            if (m_pPath)
                    delete m_pPath;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_BEGINPATH();

	    m_pPath = new CEmfPath();
	    if (!m_pPath)
		    SetError();

	    // Иногда MoveTo идет до BeginPath
	    TEmfPointL oPoint = m_pDC->GetCurPos();
	    double dX, dY;
	    TranslatePoint(oPoint, dX, dY);
	    m_pPath->MoveTo(dX, dY);
    }
    void CEmfxParser::Read_EMR_ENDPATH()
    {
            if (NULL != m_pInterpretator)
                    m_pInterpretator->HANDLER_EMR_ENDPATH();
            // Ничего не делаем
    }
    void CEmfxParser::Read_EMR_CLOSEFIGURE()
    {
            if (NULL != m_pInterpretator)
                    m_pInterpretator->HANDLER_EMR_CLOSEFIGURE();

	    if (m_pPath)
	    {
		    if (!m_pPath->Close())
			    return SetError();
	    }
    }
    void CEmfxParser::Read_EMR_FLATTENPATH()
    {
            if (NULL != m_pInterpretator)
                    m_pInterpretator->HANDLER_EMR_FLATTENPATH();
            // Ничего не делаем
    }
    void CEmfxParser::Read_EMR_WIDENPATH()
    {
            // TODO: реализовать
            if (NULL != m_pInterpretator)
                    m_pInterpretator->HANDLER_EMR_WIDENPATH();
    }
    void CEmfxParser::Read_EMR_ABORTPATH()
    {
            if (NULL != m_pInterpretator)
                    m_pInterpretator->HANDLER_EMR_ABORTPATH();

	    if (m_pPath)
	    {
		    delete m_pPath;
		    m_pPath = NULL;
	    }
    }
    void CEmfxParser::Read_EMR_MOVETOEX()
    {
            TEmfPointL oPoint;

            *m_pOutput >> oPoint;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_MOVETOEX(oPoint);

            MoveTo(oPoint);
    }
    void CEmfxParser::Read_EMR_SETARCDIRECTION()
    {
            unsigned int unDirection;

            *m_pOutput >> unDirection;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETARCDIRECTION(unDirection);

	    m_pDC->SetArcDirection(unDirection);
	    // Здесь не обновляем DC у Output, т.к. этот параметр разруливается внутри данного класса.
    }
    void CEmfxParser::Read_EMR_FILLPATH()
    {
            TEmfRectL oBounds;

            *m_pOutput >> oBounds;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_FILLPATH(oBounds);

	    if (m_pPath)
	    {
		    m_pPath->Draw(m_pInterpretator, false, true);
		    RELEASEOBJECT(m_pPath);
	    }
    }
    void CEmfxParser::Read_EMR_SETMAPMODE()
    {
            unsigned int ulMapMode;

            *m_pOutput >> ulMapMode;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETMAPMODE(ulMapMode);

            m_pDC->SetMapMode(ulMapMode);
    }
    void CEmfxParser::Read_EMR_SETWINDOWORGEX()
    {
            TEmfPointL oOrigin;

            *m_pOutput >> oOrigin;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETWINDOWORGEX(oOrigin);

            m_pDC->SetWindowOrigin(oOrigin);
    }
    void CEmfxParser::Read_EMR_SETWINDOWEXTEX()
    {
            TEmfSizeL oExtent;

            *m_pOutput >> oExtent;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETWINDOWEXTEX(oExtent);

            m_pDC->SetWindowExtents(oExtent);
    }
    void CEmfxParser::Read_EMR_SETVIEWPORTORGEX()
    {
            TEmfPointL oOrigin;

            *m_pOutput >> oOrigin;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETVIEWPORTORGEX(oOrigin);

            m_pDC->SetViewportOrigin(oOrigin);
    }
    void CEmfxParser::Read_EMR_SETVIEWPORTEXTEX()
    {
            TEmfSizeL oExtent;

            *m_pOutput >> oExtent;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETVIEWPORTEXTEX(oExtent);

            m_pDC->SetViewportExtents(oExtent);
    }
    void CEmfxParser::Read_EMR_SETSTRETCHBLTMODE()
    {
            unsigned int ulStretchMode;

            *m_pOutput >> ulStretchMode;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETSTRETCHBLTMODE(ulStretchMode);

            m_pDC->SetStretchMode(ulStretchMode);
    }
    void CEmfxParser::Read_EMR_SETICMMODE()
    {
            unsigned int ulICMMode;

            *m_pOutput >> ulICMMode;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETICMMODE(ulICMMode);

    }
    void CEmfxParser::Read_EMR_CREATEDIBPATTERNBRUSHPT()
    {
            unsigned int ulBrushIndex;
            TEmfDibPatternBrush oDibBrush;

	    *m_pOutput >> ulBrushIndex;
	    *m_pOutput >> oDibBrush;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_CREATEDIBPATTERNBRUSHPT(ulBrushIndex, oDibBrush,
									  m_oStream, m_ulRecordSize);

            ImageProcessing(oDibBrush, ulBrushIndex);
    }
    void CEmfxParser::Read_EMR_SELECTCLIPPATH()
    {
            unsigned int unRegionMode;

            *m_pOutput >> unRegionMode;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SELECTCLIPPATH(unRegionMode);

	    if (m_pPath)
	    {
		    m_pDC->ClipToPath(m_pPath, unRegionMode, GetDC()->GetFinalTransform(GM_ADVANCED));
		    RELEASEOBJECT(m_pPath);

		    UpdateOutputDC();
	    }
    }
    void CEmfxParser::Read_EMR_SETBKCOLOR()
    {
            TEmfColor oColor;

            *m_pOutput >> oColor;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETBKCOLOR(oColor);

	    m_pDC->SetBgColor(oColor);
	    UpdateOutputDC();
    }
    void CEmfxParser::Read_EMR_EXCLUDECLIPRECT()
    {
            // TODO: Проверить как найдется файл
            TEmfRectL oClip;

            *m_pOutput >> oClip;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_EXCLUDECLIPRECT(oClip);

	    TRectD oClipRect, oBB;

	    // Поскольку мы реализовываем данный тип клипа с помощью разницы внешнего ректа и заданного, и
	    // пересечением с полученной областью, то нам надо вычесть границу заданного ректа.
	    if (oClip.lLeft < oClip.lRight)
	    {
		    oClip.lLeft--;
		    oClip.lRight++;
	    }
	    else
	    {
		    oClip.lLeft++;
		    oClip.lRight--;
	    }

	    if (oClip.lTop < oClip.lBottom)
	    {
		    oClip.lTop--;
		    oClip.lBottom++;
	    }
	    else
	    {
		    oClip.lTop++;
		    oClip.lBottom--;
	    }

	    TranslatePoint(oClip.lLeft, oClip.lTop, oClipRect.dLeft, oClipRect.dTop);
	    TranslatePoint(oClip.lRight, oClip.lBottom, oClipRect.dRight, oClipRect.dBottom);

	    TRect* pRect = GetDCBounds();
	    TranslatePoint(pRect->nLeft, pRect->nTop, oBB.dLeft, oBB.dTop);
	    TranslatePoint(pRect->nRight, pRect->nBottom, oBB.dRight, oBB.dBottom);

	    m_pDC->GetClip()->Exclude(oClipRect, oBB);
	    UpdateOutputDC();
    }
    void CEmfxParser::Read_EMR_EXTSELECTCLIPRGN()
    {
            unsigned int ulRgnDataSize, ulRegionMode;

            *m_pOutput >> ulRgnDataSize;
            *m_pOutput >> ulRegionMode;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_EXTSELECTCLIPRGN(ulRgnDataSize, ulRegionMode,
								   m_oStream, m_ulRecordSize);

	    // Тут просто сбрасываем текущий клип. Ничего не добавляем в клип, т.е. реализовать регионы с
	    // текущим интерфейсом рендерера невозможно.
	    m_pDC->GetClip()->Reset();
    }
    void CEmfxParser::Read_EMR_SETMETARGN()
    {
            m_pDC->GetClip()->Reset();
            UpdateOutputDC();

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETMETARGN();
    }
    void CEmfxParser::Read_EMR_SETROP2()
    {
            unsigned int ulRop2Mode;

            *m_pOutput >> ulRop2Mode;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETROP2(ulRop2Mode);

	    m_pDC->SetRop2Mode(ulRop2Mode);
	    UpdateOutputDC();
    }
    void CEmfxParser::Read_EMR_CREATEPALETTE()
    {
            unsigned int ulPaletteIndex;
            CEmfLogPalette* pPalette = new CEmfLogPalette();
            if (!pPalette)
                    return SetError();

	    *m_pOutput >> ulPaletteIndex;
	    *m_pOutput >> *pPalette;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_CREATEPALETTE(ulPaletteIndex, pPalette, m_ulRecordSize);

            m_oPlayer.RegisterObject(ulPaletteIndex, (CEmfObjectBase*)pPalette);
    }
    void CEmfxParser::Read_EMR_SELECTPALETTE()
    {
            unsigned int ulIndex;

            *m_pOutput >> ulIndex;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SELECTPALETTE(ulIndex);

            m_oPlayer.SelectPalette(ulIndex);
    }
    void CEmfxParser::Read_EMR_REALIZEPALETTE()
    {
            // TODO: Реализовать

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_REALIZEPALETTE();
    }
    void CEmfxParser::Read_EMR_INTERSECTCLIPRECT()
    {
            TEmfRectL oClip;

            *m_pOutput >> oClip;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_INTERSECTCLIPRECT(oClip);

	    TRectD oClipRect;
	    TranslatePoint(oClip.lLeft, oClip.lTop, oClipRect.dLeft, oClipRect.dTop);
	    TranslatePoint(oClip.lRight, oClip.lBottom, oClipRect.dRight, oClipRect.dBottom);
	    m_pDC->GetClip()->Intersect(oClipRect);
    }
    void CEmfxParser::Read_EMR_SETLAYOUT()
    {
            unsigned int ulLayoutMode;

            *m_pOutput >> ulLayoutMode;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETLAYOUT(ulLayoutMode);

            // TODO: реализовать
    }
    void CEmfxParser::Read_EMR_SETBRUSHORGEX()
    {
            TEmfPointL oOrigin;

            *m_pOutput >> oOrigin;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_SETBRUSHORGEX(oOrigin);

            // TODO: реализовать
    }
    void CEmfxParser::Read_EMR_ANGLEARC()
    {
            // TODO: Как найдутся файлы проверить данную запись.
            TEmfPointL oCenter;
            unsigned int unRadius;
            double dStartAngle, dSweepAngle;

            *m_pOutput >> oCenter;
            *m_pOutput >> unRadius;

            *m_pOutput >> dStartAngle;
            *m_pOutput >> dSweepAngle;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_ANGLEARC(oCenter, unRadius, dStartAngle, dSweepAngle);

	    ArcTo(oCenter.x - unRadius, oCenter.y - unRadius, oCenter.x + unRadius, oCenter.y + unRadius, dStartAngle, dSweepAngle);
	    DrawPath(true, false);
    }
    void CEmfxParser::Read_EMR_ARC_BASE(TEmfRectL& oBox, TEmfPointL& oStart, TEmfPointL& oEnd, double& dStartAngle, double& dSweepAngle)
    {
            *m_pOutput >> oBox;
            *m_pOutput >> oStart;
            *m_pOutput >> oEnd;

	    dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
	    dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y) - dStartAngle;

	    // TODO: Проверить здесь
	    if (dSweepAngle < 0.001)
		    dSweepAngle += 360;

	    // TODO: Проверить здесь
	    if (AD_COUNTERCLOCKWISE != m_pDC->GetArcDirection())
	    {
		    dSweepAngle = dSweepAngle - 360;
	    }
    }
    void CEmfxParser::Read_EMR_ARC()
    {
            TEmfRectL oBox;
            TEmfPointL oStart, oEnd;
            double dStartAngle, dSweep;
            Read_EMR_ARC_BASE(oBox, oStart, oEnd, dStartAngle, dSweep);

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_ARC(oBox, oStart, oEnd);

	    MoveTo(oStart);
	    ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, dStartAngle, dSweep);
	    DrawPath(true, false);
    }
    void CEmfxParser::Read_EMR_ARCTO()
    {
            // TODO: Как найдутся файлы проверить данную запись.
            TEmfRectL oBox;
            TEmfPointL oStart, oEnd;
            double dStartAngle, dSweep;
            Read_EMR_ARC_BASE(oBox, oStart, oEnd, dStartAngle, dSweep);

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_ARCTO(oBox, oStart, oEnd);

            ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, dStartAngle, dSweep);
    }
    void CEmfxParser::Read_EMR_CHORD()
    {
            // TODO: Как найдутся файлы проверить данную запись.
            TEmfRectL oBox;
            TEmfPointL oStart, oEnd;
            double dStartAngle, dSweep;
            Read_EMR_ARC_BASE(oBox, oStart, oEnd, dStartAngle, dSweep);

	    if (NULL != m_pInterpretator)
		m_pInterpretator->HANDLER_EMR_CHORD(oBox, oStart, oEnd);

	    MoveTo(oStart);
	    ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, dStartAngle, dSweep);
	    LineTo(oStart);
	    DrawPath(true, true);
    }
    void CEmfxParser::Read_EMR_ELLIPSE()
    {
            TEmfRectL oBox;

            *m_pOutput >> oBox;

	    if (NULL != m_pInterpretator)
		m_pInterpretator->HANDLER_EMR_ELLIPSE(oBox);

	    if (m_pDC->GetArcDirection() == AD_COUNTERCLOCKWISE)
		ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, 0, 360);
	    else
		ArcTo(oBox.lLeft, oBox.lBottom, oBox.lRight, oBox.lTop, 0, 360);


            DrawPath(true, true);
    }
    void CEmfxParser::Read_EMR_EXTTEXTOUTA()
    {
            // TODO: Как найдутся файлы проверить данную запись.
            TEmfExtTextoutA oText;

            *m_pOutput >> oText;

	    if (NULL != m_pInterpretator)
		m_pInterpretator->HANDLER_EMR_EXTTEXTOUTA(oText, m_ulRecordSize);

            DrawTextA(oText.aEmrText, oText.iGraphicsMode);
    }
    void CEmfxParser::Read_EMR_EXTTEXTOUTW()
    {
            TEmfExtTextoutW oText;

            *m_pOutput >> oText;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_EXTTEXTOUTW(oText, m_ulRecordSize);

            DrawTextW(oText.wEmrText, oText.iGraphicsMode);
    }
    void CEmfxParser::Read_EMR_LINETO()
    {
            TEmfPointL oPoint;

            *m_pOutput >> oPoint;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_LINETO(oPoint);

            LineTo(oPoint);
    }
    void CEmfxParser::Read_EMR_PIE()
    {
            // TODO: Как найдутся файлы проверить данную запись.
            TEmfRectL oBox;
            TEmfPointL oStart, oEnd;
            double dStartAngle, dSweep;
            Read_EMR_ARC_BASE(oBox, oStart, oEnd, dStartAngle, dSweep);

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_PIE(oBox, oStart, oEnd);

	    ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, dStartAngle, dSweep);
	    LineTo((oBox.lLeft + oBox.lRight) / 2, (oBox.lTop + oBox.lBottom) / 2);
	    ClosePath();
	    DrawPath(true, true);
    }
    void CEmfxParser::Read_EMR_POLYBEZIER()
    {
            Read_EMR_POLYBEZIER_BASE<TEmfPointL>();
    }
    void CEmfxParser::Read_EMR_POLYBEZIER16()
    {
            Read_EMR_POLYBEZIER_BASE<TEmfPointS>();
    }
    template<typename T>void CEmfxParser::Read_EMR_POLYBEZIER_BASE()
    {
            TEmfRectL oBounds;
            unsigned int ulCount;

	    *m_pOutput >> oBounds;
	    *m_pOutput >> ulCount;

	    if (0 == ulCount)
		    return;

	    std::vector<T> arPoints(ulCount);

	    *m_pOutput >> arPoints[0];

	    MoveTo(arPoints[0]);

	    for (unsigned int ulIndex = 1; ulIndex < ulCount; ulIndex += 3)
	    {
		    *m_pOutput >> arPoints[ulIndex];
		    *m_pOutput >> arPoints[ulIndex + 1];
		    *m_pOutput >> arPoints[ulIndex + 2];

		    CurveTo(arPoints[ulIndex], arPoints[ulIndex + 1], arPoints[ulIndex + 2]);
	    }

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_POLYBEZIER(oBounds, arPoints);

            DrawPath(true, false);
    }
    void CEmfxParser::Read_EMR_POLYBEZIERTO()
    {
            Read_EMR_POLYBEZIERTO_BASE<TEmfPointL>();
    }
    void CEmfxParser::Read_EMR_POLYBEZIERTO16()
    {
            Read_EMR_POLYBEZIERTO_BASE<TEmfPointS>();
    }
    template<typename T>void CEmfxParser::Read_EMR_POLYBEZIERTO_BASE()
    {
            TEmfRectL oBounds;
            unsigned int ulCount;

	    *m_pOutput >> oBounds;
	    *m_pOutput >> ulCount;

	    std::vector<T> arPoints(ulCount);

	    for (unsigned int ulIndex = 0; ulIndex < ulCount; ulIndex += 3)
	    {
		    if (ulCount - ulIndex < 2)
			    return SetError();

		    *m_pOutput >> arPoints[ulIndex];
		    *m_pOutput >> arPoints[ulIndex + 1];
		    *m_pOutput >> arPoints[ulIndex + 2];

		    CurveTo(arPoints[ulIndex], arPoints[ulIndex + 1], arPoints[ulIndex + 2]);
	    }

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_POLYBEZIERTO(oBounds, arPoints);

    }
    void CEmfxParser::Read_EMR_POLYDRAW()
    {
            Read_EMR_POLYDRAW_BASE<TEmfPointL>();
    }
    void CEmfxParser::Read_EMR_POLYDRAW16()
    {
            Read_EMR_POLYDRAW_BASE<TEmfPointS>();
    }
    template<typename T>void CEmfxParser::Read_EMR_POLYDRAW_BASE()
    {
            // TODO: Как найдутся файлы проверить данную запись.
            //bug #35006 - не прочитывается весь рекорд ... выравнивание?

	    TEmfRectL oBounds;
	    unsigned int unCount;

	    *m_pOutput >> oBounds;
	    *m_pOutput >> unCount;

	    if (0 == unCount)
		    return;

	    T* pPoints = new T[unCount];
	    if (!pPoints)
		    return SetError();

	    for (unsigned int unIndex = 0; unIndex < unCount; unIndex++)
		    *m_pOutput >> pPoints[unIndex];

	    unsigned char* pAbTypes = new unsigned char[unCount];
	    if (!pAbTypes)
	    {
		    delete[] pPoints;
		    return SetError();
	    }

	    for (unsigned int unIndex = 0; unIndex < unCount; unIndex++)
		    *m_pOutput >> pAbTypes[unIndex];

	    T* pPoint1 = NULL, *pPoint2 = NULL;
	    for (unsigned int unIndex = 0, unPointIndex = 0; unIndex < unCount; unIndex++)
	    {
		    unsigned char unType = pAbTypes[unIndex];
		    T* pPoint = pPoints + unIndex;
		    if (PT_MOVETO == unType)
		    {
			    MoveTo(*pPoint);
			    unPointIndex = 0;
		    }
		    else if (PT_LINETO & unType)
		    {
			    LineTo(*pPoint);
			    if (PT_CLOSEFIGURE & unType)
				    ClosePath();
			    unPointIndex = 0;
		    }
		    else if (PT_BEZIERTO & unType)
		    {
			    if (0 == unPointIndex)
			    {
				    pPoint1 = pPoint;
				    unPointIndex = 1;
			    }
			    else if (1 == unPointIndex)
			    {
				    pPoint2 = pPoint;
				    unPointIndex = 2;
			    }
			    else if (2 == unPointIndex)
			    {
				    CurveTo(*pPoint1, *pPoint2, *pPoint);
				    unPointIndex = 0;

				    if (PT_CLOSEFIGURE & unType)
					    ClosePath();
			    }
			    else
			    {
				    SetError();
				    break;
			    }
		    }
	    }

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_POLYDRAW(oBounds, pPoints, unCount, pAbTypes);

	    delete[] pPoints;
	    delete[] pAbTypes;
    }
    void CEmfxParser::Read_EMR_POLYGON()
    {
            Read_EMR_POLYGON_BASE<TEmfPointL>();
    }
    void CEmfxParser::Read_EMR_POLYGON16()
    {
            Read_EMR_POLYGON_BASE<TEmfPointS>();
    }
    template<typename T>void CEmfxParser::Read_EMR_POLYGON_BASE()
    {
            TEmfRectL oBounds;
            unsigned int ulCount;

	    *m_pOutput >> oBounds;
	    *m_pOutput >> ulCount;

	    if (ulCount <= 0)
		    return;

	    std::vector<T> arPoints(ulCount);

	    *m_pOutput >> arPoints[0];

	    MoveTo(arPoints[0]);

	    for (unsigned int ulIndex = 1; ulIndex < ulCount; ulIndex++)
	    {
		    *m_pOutput >> arPoints[ulIndex];
		    LineTo(arPoints[ulIndex]);
	    }

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_POLYGON(oBounds, arPoints);

	    ClosePath();
	    DrawPath(true, true);
    }
    void CEmfxParser::Read_EMR_POLYLINE()
    {
            Read_EMR_POLYLINE_BASE<TEmfPointL>();
    }
    void CEmfxParser::Read_EMR_POLYLINE16()
    {
            Read_EMR_POLYLINE_BASE<TEmfPointS>();
    }
    template<typename T>void CEmfxParser::Read_EMR_POLYLINE_BASE()
    {
            TEmfRectL oBounds;
            unsigned int ulCount;

	    *m_pOutput >> oBounds;
	    *m_pOutput >> ulCount;

	    if (0 == ulCount)
		    return;

	    std::vector<T> arPoints(ulCount);

	    *m_pOutput >> arPoints[0];

	    MoveTo(arPoints[0]);

	    for (unsigned int ulIndex = 1; ulIndex < ulCount; ulIndex++)
	    {
		    *m_pOutput >> arPoints[ulIndex];

		    LineTo(arPoints[ulIndex]);
	    }

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_POLYLINE(oBounds, arPoints);

            DrawPath(true, false);
    }
    void CEmfxParser::Read_EMR_POLYLINETO()
    {
            Read_EMR_POLYLINETO_BASE<TEmfPointL>();
    }
    void CEmfxParser::Read_EMR_POLYLINETO16()
    {
            Read_EMR_POLYLINETO_BASE<TEmfPointS>();
    }
    template<typename T>void CEmfxParser::Read_EMR_POLYLINETO_BASE()
    {
            TEmfRectL oBounds;
            unsigned int ulCount;

	    *m_pOutput >> oBounds;
	    *m_pOutput >> ulCount;

	    std::vector<T> arPoints(ulCount);

	    for (unsigned int ulIndex = 0; ulIndex < ulCount; ulIndex++)
	    {
		    *m_pOutput >> arPoints[ulIndex];

		    LineTo(arPoints[ulIndex]);
	    }

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_POLYLINETO(oBounds, arPoints);

    }
    void CEmfxParser::Read_EMR_POLYPOLYGON()
    {
            Read_EMR_POLYPOLYGON_BASE<TEmfPointL>();
    }
    void CEmfxParser::Read_EMR_POLYPOLYGON16()
    {
            Read_EMR_POLYPOLYGON_BASE<TEmfPointS>();
    }
    template<typename T>void CEmfxParser::Read_EMR_POLYPOLYGON_BASE()
    {
            TEmfRectL oBounds;
            unsigned int ulNumberOfPolygons;
            unsigned int ulTotalPointsCount;

            //TODO: сделать сохранение в XML

//		if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsReader())
//		{
//			*m_pInterpretatorXml >> oBounds;
//			*m_pInterpretatorXml >> ulNumberOfPolygons;
//			*m_pInterpretatorXml >> ulTotalPointsCount;
//		}
//		else
//		{
//			*m_pOutput >> oBounds;
//			*m_pOutput >> ulNumberOfPolygons;
//			*m_pOutput >> ulTotalPointsCount;
//		}

	    *m_pOutput >> oBounds;
	    *m_pOutput >> ulNumberOfPolygons;
	    *m_pOutput >> ulTotalPointsCount;

	    unsigned int* pPolygonPointCount = new unsigned int[ulNumberOfPolygons];
	    if (!pPolygonPointCount)
		    return SetError();

//		std::wstring wsRecordName;
//		unsigned int unRecordId;

//		if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsWriter())
//		{
//			if (typeid (T).name() == "TEmfPointL")
//			{
//				wsRecordName = L"EMR_POLYPOLYGON";
//				unRecordId = EMR_POLYPOLYGON;
//			}
//			else
//			{
//				wsRecordName = L"EMR_POLYPOLYGON16";
//				unRecordId = EMR_POLYPOLYGON16;
//			}

//			m_pInterpretatorXml->WriteNodeBegin(wsRecordName, {XmlArgument(L"Id", unRecordId),
//								    XmlArgument(L"Size", m_ulRecordSize + 8)});
//				m_pInterpretatorXml->WriteNode(L"Bounds",		oBounds);
//				m_pInterpretatorXml->WriteNode(L"NumberOfPolygons",	ulNumberOfPolygons);
//				m_pInterpretatorXml->WriteNode(L"Count",		ulTotalPointsCount);

//		}

	    for (unsigned int ulIndex = 0; ulIndex < ulNumberOfPolygons; ulIndex++)
	    {
//			if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsReader())
//				*m_pInterpretatorXml >> pPolygonPointCount[ulIndex];
//			else
//				*m_pOutput >> pPolygonPointCount[ulIndex];

		    *m_pOutput >> pPolygonPointCount[ulIndex];

//			if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsWriter())
//				m_pInterpretatorXml->WriteNode(L"PolygonPointCount" + std::to_wstring(ulIndex + 1), pPolygonPointCount[ulIndex]);
	    }

	    unsigned int unIndexPoint = 0;

	    for (unsigned int ulPolygonIndex = 0, unStartPointIndex = 0; ulPolygonIndex < ulNumberOfPolygons; ulPolygonIndex++)
	    {
		    unsigned int ulCurrentPolygonPointsCount = pPolygonPointCount[ulPolygonIndex];
		    if (0 == ulCurrentPolygonPointsCount)
			    continue;

		    T oPoint;

//			if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsReader())
//				*m_pInterpretatorXml >> oPoint;
//			else
//				*m_pOutput >> oPoint;

		    *m_pOutput >> oPoint;

//			if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsWriter())
//			{
//				++unIndexPoint;
//				m_pInterpretatorXml->WriteNode(L"Point" + std::to_wstring(unIndexPoint), oPoint);
//			}

		    MoveTo(oPoint);

		    for (unsigned int ulPointIndex = 1; ulPointIndex < ulCurrentPolygonPointsCount; ulPointIndex++)
		    {
			    unsigned int ulRealPointIndex = ulPointIndex + unStartPointIndex;
			    if (ulRealPointIndex >= ulTotalPointsCount)
			    {
				    delete[] pPolygonPointCount;
				    return SetError();
			    }

//				if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsReader())
//					*m_pInterpretatorXml >> oPoint;
//				else
//					*m_pOutput >> oPoint;

			    *m_pOutput >> oPoint;

//				if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsWriter())
//				{
//					++unIndexPoint;
//					m_pInterpretatorXml->WriteNode(L"Point" + std::to_wstring(unIndexPoint), oPoint);
//				}

			    LineTo(oPoint);
		    }

		    ClosePath();
	    }

//		if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsWriter())
//		{
//			m_pInterpretatorXml->WriteNodeEnd(wsRecordName);
//		}

            DrawPath(true, true);

            delete[] pPolygonPointCount;
    }
    void CEmfxParser::Read_EMR_POLYPOLYLINE()
    {
            Read_EMR_POLYPOLYLINE_BASE<TEmfPointL>();
    }
    void CEmfxParser::Read_EMR_POLYPOLYLINE16()
    {
            Read_EMR_POLYPOLYLINE_BASE<TEmfPointS>();
    }
    template<typename T>void CEmfxParser::Read_EMR_POLYPOLYLINE_BASE()
    {
            TEmfRectL oBounds;
            unsigned int ulNumberOfPolylines;
            unsigned int ulTotalPointsCount;

            //TODO: сделать сохранение в XML

//		if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsReader())
//		{
//			*m_pInterpretatorXml >> oBounds;
//			*m_pInterpretatorXml >> ulNumberOfPolylines;
//			*m_pInterpretatorXml >> ulTotalPointsCount;
//		}
//		else
//		{
//			*m_pOutput >> oBounds;
//			*m_pOutput >> ulNumberOfPolylines;
//			*m_pOutput >> ulTotalPointsCount;
//		}

	    *m_pOutput >> oBounds;
	    *m_pOutput >> ulNumberOfPolylines;
	    *m_pOutput >> ulTotalPointsCount;

	    if (0 == ulNumberOfPolylines && 0 == ulTotalPointsCount)
		    return;
	    else if (0 == ulNumberOfPolylines || 0 == ulTotalPointsCount)
		    return SetError();


//		std::wstring wsRecordName;
//		unsigned int unRecordId;

//		if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsWriter())
//		{
//			if (typeid (T).name() == "TEmfPointL")
//			{
//				wsRecordName = L"EMR_POLYPOLYLINE";
//				unRecordId = EMR_POLYPOLYLINE;
//			}
//			else
//			{
//				wsRecordName = L"EMR_POLYPOLYLINE16";
//				unRecordId = EMR_POLYPOLYLINE16;
//			}

//			m_pInterpretatorXml->WriteNodeBegin(wsRecordName, {XmlArgument(L"Id", unRecordId),
//								    XmlArgument(L"Size", m_ulRecordSize + 8)});
//				m_pInterpretatorXml->WriteNode(L"Bounds",		oBounds);
//                                m_pInterpretatorXml->WriteNode(L"NumberOfPolylines",   ulNumberOfPolylines);
//                                m_pInterpretatorXml->WriteNode(L"Count",               ulTotalPointsCount);

//                }

	    unsigned int* pPolylinePointCount = new unsigned int[ulNumberOfPolylines];
	    for (unsigned int ulIndex = 0; ulIndex < ulNumberOfPolylines; ulIndex++)
	    {
//			if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsReader())
//				*m_pInterpretatorXml >> pPolylinePointCount[ulIndex];
//			else
//				*m_pOutput >> pPolylinePointCount[ulIndex];

		    *m_pOutput >> pPolylinePointCount[ulIndex];

//			if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsWriter())
//				m_pInterpretatorXml->WriteNode(L"PolylinePointCount" + std::to_wstring(ulIndex + 1), pPolylinePointCount[ulIndex]);
	    }

	    unsigned int unIndexPoint = 0;

	    for (unsigned int ulPolyIndex = 0, ulStartPointIndex = 0; ulPolyIndex < ulNumberOfPolylines; ulPolyIndex++)
	    {
		    unsigned int ulCurrentPolylinePointsCount = pPolylinePointCount[ulPolyIndex];
		    if (0 == ulCurrentPolylinePointsCount)
			    continue;

		    T oPoint;

//			if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsReader())
//				*m_pInterpretatorXml >> oPoint;
//			else
//				*m_pOutput >> oPoint;

		    *m_pOutput >> oPoint;

//			if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsWriter())
//			{
//				++unIndexPoint;
//				m_pInterpretatorXml->WriteNode(L"Point" + std::to_wstring(unIndexPoint), oPoint);
//			}

		    MoveTo(oPoint);

		    for (unsigned int ulPointIndex = 1; ulPointIndex < ulCurrentPolylinePointsCount; ulPointIndex++)
		    {
			    unsigned int ulRealPointIndex = ulPointIndex + ulStartPointIndex;
			    if (ulRealPointIndex >= ulTotalPointsCount)
			    {
				    delete[] pPolylinePointCount;
				    return SetError();
			    }

//				if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsReader())
//					*m_pInterpretatorXml >> oPoint;
//				else
//					*m_pOutput >> oPoint;

			    *m_pOutput >> oPoint;

//				if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsWriter())
//				{
//					++unIndexPoint;
//					m_pInterpretatorXml->WriteNode(L"Point" + std::to_wstring(unIndexPoint), oPoint);
//				}

			    LineTo(oPoint);
		    }
	    }

//		if (m_pInterpretator && NULL != m_pInterpretatorXml && m_pInterpretatorXml->IsWriter())
//		{
//			m_pInterpretatorXml->WriteNodeEnd(wsRecordName);
//		}

            DrawPath(true, false);

            delete[] pPolylinePointCount;
    }
    void CEmfxParser::Read_EMR_POLYTEXTOUTA()
    {
            // TODO: Как найдутся файлы проверить данную запись.
            TEmfPolyTextoutA oText;

            //TODO: сделать сохранение в XML и чтение из него
            return;

            m_oStream >> oText;

	    if (0 == oText.cStrings)
		    return;

	    if (!oText.aEmrText)
		    return SetError();

	    for (unsigned int unIndex = 0; unIndex < oText.cStrings; unIndex++)
	    {
		    DrawTextA(oText.aEmrText[unIndex], oText.iGraphicsMode);
	    }
    }
    void CEmfxParser::Read_EMR_POLYTEXTOUTW()
    {
            // TODO: Как найдутся файлы проверить данную запись.
            TEmfPolyTextoutW oText;

	    //TODO: сделать сохранение в XML и чтение из него
	    return;

	    m_oStream >> oText;

	    if (0 == oText.cStrings)
		    return;

	    if (!oText.wEmrText)
		    return SetError();

	    for (unsigned int unIndex = 0; unIndex < oText.cStrings; unIndex++)
	    {
		    DrawTextA(oText.wEmrText[unIndex], oText.iGraphicsMode);
	    }
    }
    void CEmfxParser::Read_EMR_RECTANGLE()
    {
            TEmfRectL oBox;

            *m_pOutput >> oBox;

	    if (NULL != m_pInterpretator)
		m_pInterpretator->HANDLER_EMR_RECTANGLE(oBox);

	    if (AD_COUNTERCLOCKWISE == m_pDC->GetArcDirection())
	    {
		    MoveTo(oBox.lLeft, oBox.lTop);
		    LineTo(oBox.lLeft, oBox.lBottom);
		    LineTo(oBox.lRight, oBox.lBottom);
		    LineTo(oBox.lRight, oBox.lTop);
	    }
	    else
	    {
		    MoveTo(oBox.lLeft, oBox.lTop);
		    LineTo(oBox.lRight, oBox.lTop);
		    LineTo(oBox.lRight, oBox.lBottom);
		    LineTo(oBox.lLeft, oBox.lBottom);
	    }
	    ClosePath();
	    DrawPath(true, true);
    }
    void CEmfxParser::Read_EMR_ROUNDRECT()
    {
            TEmfRectL oBox;
            TEmfSizeL oCorner;

	    *m_pOutput >> oBox;
	    *m_pOutput >> oCorner;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_ROUNDRECT(oBox, oCorner);

	    int lBoxW = oBox.lRight - oBox.lLeft;
	    int lBoxH = oBox.lBottom - oBox.lTop;

	    int lRoundW = (std::min)((int)oCorner.cx, lBoxW / 2);
	    int lRoundH = (std::min)((int)oCorner.cy, lBoxH / 2);

	    if (AD_COUNTERCLOCKWISE == m_pDC->GetArcDirection())
	    {
		    MoveTo(oBox.lLeft + lRoundW, oBox.lTop);
		    ArcTo(oBox.lLeft, oBox.lTop, oBox.lLeft + lRoundW, oBox.lTop + lRoundH, 270, -90);
		    LineTo(oBox.lLeft, oBox.lBottom - lRoundH);
		    ArcTo(oBox.lLeft, oBox.lBottom - lRoundH, oBox.lLeft + lRoundW, oBox.lBottom, 180, -90);
		    LineTo(oBox.lRight - lRoundW, oBox.lBottom);
		    ArcTo(oBox.lRight - lRoundW, oBox.lBottom - lRoundH, oBox.lRight, oBox.lBottom, 90, -90);
		    LineTo(oBox.lRight, oBox.lTop + lRoundH);
		    ArcTo(oBox.lRight - lRoundW, oBox.lTop, oBox.lRight, oBox.lTop + lRoundH, 0, -90);
		    LineTo(oBox.lLeft + lRoundW, oBox.lTop);
	    }
	    else
	    {
		    MoveTo(oBox.lLeft + lRoundW, oBox.lTop);
		    LineTo(oBox.lRight - lRoundW, oBox.lTop);
		    ArcTo(oBox.lRight - lRoundW, oBox.lTop, oBox.lRight, oBox.lTop + lRoundH, -90, 90);
		    LineTo(oBox.lRight, oBox.lBottom - lRoundH);
		    ArcTo(oBox.lRight - lRoundW, oBox.lBottom - lRoundH, oBox.lRight, oBox.lBottom, 0, 90);
		    LineTo(oBox.lLeft + lRoundW, oBox.lBottom);
		    ArcTo(oBox.lLeft, oBox.lBottom - lRoundH, oBox.lLeft + lRoundW, oBox.lBottom, 90, 90);
		    LineTo(oBox.lLeft, oBox.lTop + lRoundH);
		    ArcTo(oBox.lLeft, oBox.lTop, oBox.lLeft + lRoundW, oBox.lTop + lRoundH, 180, 90);
	    }

	    ClosePath();
	    DrawPath(true, true);
    }
    void CEmfxParser::Read_EMR_SETPIXELV()
    {
            TEmfPointL oPoint;
            TEmfColor oColor;

            *m_pOutput >> oPoint;
            *m_pOutput >> oColor;

            if (NULL != m_pInterpretator)
                    m_pInterpretator->HANDLER_EMR_SETPIXELV(oPoint, oColor);

	    // Делаем цветом кисти
	    BYTE pBgraBuffer[4];
	    pBgraBuffer[0] = oColor.b;
	    pBgraBuffer[1] = oColor.g;
	    pBgraBuffer[2] = oColor.r;
	    pBgraBuffer[3] = 0xff;

            DrawImage(oPoint.x, oPoint.y, 1, 1, pBgraBuffer, 1, 1);
    }
    void CEmfxParser::Read_EMR_SMALLTEXTOUT()
    {
            TEmfSmallTextout oText;

            *m_pOutput >> oText;

	    if (NULL != m_pInterpretator)
		m_pInterpretator->HANDLER_EMR_SMALLTEXTOUT(oText, m_ulRecordSize);

	    // Переводим oText в TEmfEmrText
	    TEmfEmrText oEmrText;
	    oEmrText.Chars        = oText.cChars;
	    oEmrText.offDx        = 0;
	    oEmrText.offString    = 0;
	    oEmrText.Options      = oText.fuOptions;
	    oEmrText.OutputString = oText.TextString;
	    oEmrText.Reference.x  = oText.x;
	    oEmrText.Reference.y  = oText.y;
	    oEmrText.OutputDx     = NULL;

	    DrawTextW(oEmrText, oText.iGraphicsMode);

	    // Поскольку мы просто скопировали ссылку на строку, а не скопировали сами строку обнуляем здесь, потому
	    // что на деструкторе структуры освобождается память.
	    oEmrText.OutputString = NULL;
    }
    void CEmfxParser::Read_EMR_STROKEANDFILLPATH()
    {
            TEmfRectL oBounds;

            *m_pOutput >> oBounds;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_STROKEANDFILLPATH(oBounds);

	    if (m_pInterpretator && m_pPath)
	    {
		    m_pPath->Draw(m_pInterpretator, true, true);
		    RELEASEOBJECT(m_pPath);
	    }
    }
    void CEmfxParser::Read_EMR_STROKEPATH()
    {
            TEmfRectL oBounds;

            *m_pOutput >> oBounds;

	    if (NULL != m_pInterpretator)
		    m_pInterpretator->HANDLER_EMR_STROKEPATH(oBounds);

	    if (m_pInterpretator && m_pPath)
	    {
		    m_pPath->Draw(m_pInterpretator, true, false);
		    RELEASEOBJECT(m_pPath);
	    }
    }
}
