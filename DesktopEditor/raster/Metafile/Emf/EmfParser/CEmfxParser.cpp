#include "CEmfxParser.h"

#include <iostream>

namespace MetaFile
{
        CEmfxParser::CEmfxParser():
                m_pOutput(new CXmlOutput(TypeXmlOutput::IsReader)){}

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

                HANDLE_EMR_HEADER(m_oHeader);

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

                HANDLE_EMR_HEADER(m_oHeader);
        }

        void CEmfxParser::Read_EMR_ALPHABLEND()
        {
                TEmfAlphaBlend oBitmap;

                *m_pOutput >> oBitmap;

                if (oBitmap.cbBitsSrc > 0)
                        *m_pOutput >> m_oStream;

                HANDLE_EMR_ALPHABLEND(oBitmap);
        }

        void CEmfxParser::Read_EMR_STRETCHDIBITS()
        {
                TEmfStretchDIBITS oBitmap;

                *m_pOutput >> oBitmap;

                if (oBitmap.cbBitsSrc > 0)
                        *m_pOutput >> m_oStream;

                HANDLE_EMR_STRETCHDIBITS(oBitmap);
        }

        void CEmfxParser::Read_EMR_BITBLT()
        {
                TEmfBitBlt oBitmap;

                *m_pOutput >> oBitmap;

                if (oBitmap.cbBitsSrc > 0)
                        *m_pOutput >> m_oStream;

                HANDLE_EMR_BITBLT(oBitmap);
        }

        void CEmfxParser::Read_EMR_SETDIBITSTODEVICE()
        {
            TEmfSetDiBitsToDevice oBitmap;

            *m_pOutput >> oBitmap;

            if (oBitmap.cbBitsSrc > 0)
                    *m_pOutput >> m_oStream;

            HANDLE_EMR_SETDIBITSTODEVICE(oBitmap);
        }

        void CEmfxParser::Read_EMR_STRETCHBLT()
        {
                TEmfStretchBLT oBitmap;

                *m_pOutput >> oBitmap;

                if (oBitmap.cbBitsSrc > 0)
                        *m_pOutput >> m_oStream;

                HANDLE_EMR_STRETCHBLT(oBitmap);
        }

        void CEmfxParser::Read_EMR_EOF()
        {
                unsigned int ulCount, ulOffset, ulSizeLast;

                *m_pOutput >> ulCount;
                *m_pOutput >> ulOffset;
                *m_pOutput >> ulSizeLast;

                HANDLE_EMR_EOF(ulCount, ulOffset, ulSizeLast);
        }

        void CEmfxParser::Read_EMR_UNKNOWN()
        {
                m_pOutput->ReadNextRecord();
        }

        void CEmfxParser::Read_EMR_SAVEDC()
        {
                HANDLE_EMR_SAVEDC();
        }

        void CEmfxParser::Read_EMR_RESTOREDC()
        {
                int lSavedDC;

                *m_pOutput >> lSavedDC;

                HANDLE_EMR_RESTOREDC(lSavedDC);
        }

        void CEmfxParser::Read_EMR_MODIFYWORLDTRANSFORM()
        {
                TEmfXForm oXForm;
                unsigned int ulMode;

                *m_pOutput >> oXForm;
                *m_pOutput >> ulMode;

                HANDLE_EMR_MODIFYWORLDTRANSFORM(oXForm, ulMode);
        }

        void CEmfxParser::Read_EMR_SETWORLDTRANSFORM()
        {
                TEmfXForm oXForm;

                *m_pOutput >> oXForm;

                HANDLE_EMR_SETWORLDTRANSFORM(oXForm);
        }

        void CEmfxParser::Read_EMR_CREATEBRUSHINDIRECT()
        {
                unsigned int ulBrushIndex;
                CEmfLogBrushEx* pBrush = new CEmfLogBrushEx();
                if (!pBrush)
                        return SetError();

                *m_pOutput >> ulBrushIndex;
                *m_pOutput >> *pBrush;

                HANDLE_EMR_CREATEBRUSHINDIRECT(ulBrushIndex, pBrush);
        }

        void CEmfxParser::Read_EMR_SETTEXTCOLOR()
        {
                TEmfColor oColor;

                *m_pOutput >> oColor;

                HANDLE_EMR_SETTEXTCOLOR(oColor);
        }

        void CEmfxParser::Read_EMR_SELECTOBJECT()
        {
                unsigned int ulObjectIndex;

                *m_pOutput >> ulObjectIndex;

                HANDLE_EMR_SELECTOBJECT(ulObjectIndex);
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

                HANDLE_EMR_EXTCREATEFONTINDIRECTW(ulIndex, pFont);
        }

        void CEmfxParser::Read_EMR_SETTEXTALIGN()
        {
                unsigned int ulAlign;

                *m_pOutput >> ulAlign;

                HANDLE_EMR_SETTEXTALIGN(ulAlign);
        }

        void CEmfxParser::Read_EMR_SETBKMODE()
        {
                unsigned int ulBgMode;

                *m_pOutput >> ulBgMode;

                HANDLE_EMR_SETBKMODE(ulBgMode);
        }

        void CEmfxParser::Read_EMR_DELETEOBJECT()
        {
                unsigned int ulIndex;

                *m_pOutput >> ulIndex;

                HANDLE_EMR_DELETEOBJECT(ulIndex);
        }

        void CEmfxParser::Read_EMR_SETMITERLIMIT()
        {
                unsigned int ulMiterLimit;

                *m_pOutput >> ulMiterLimit;

                HANDLE_EMR_SETMITERLIMIT(ulMiterLimit);
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

                HANDLE_EMR_EXTCREATEPEN(ulPenIndex, pPen);
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

                HANDLE_EMR_CREATEPEN(ulPenIndex, widthX, pPen);
        }

        void CEmfxParser::Read_EMR_SETPOLYFILLMODE()
        {
                unsigned int ulFillMode;

                *m_pOutput >> ulFillMode;

                HANDLE_EMR_SETPOLYFILLMODE(ulFillMode);
        }

        void CEmfxParser::Read_EMR_BEGINPATH()
        {
                HANDLE_EMR_BEGINPATH();
        }

        void CEmfxParser::Read_EMR_ENDPATH()
        {
               HANDLE_EMR_ENDPATH();
        }

        void CEmfxParser::Read_EMR_CLOSEFIGURE()
        {
                HANDLE_EMR_CLOSEFIGURE();
        }

        void CEmfxParser::Read_EMR_FLATTENPATH()
        {
                HANDLE_EMR_FLATTENPATH();
        }

        void CEmfxParser::Read_EMR_WIDENPATH()
        {
                HANDLE_EMR_WIDENPATH();
        }

        void CEmfxParser::Read_EMR_ABORTPATH()
        {
                HANDLE_EMR_ABORTPATH();
        }

        void CEmfxParser::Read_EMR_MOVETOEX()
        {
                TEmfPointL oPoint;

                *m_pOutput >> oPoint;

                HANDLE_EMR_MOVETOEX(oPoint);
        }

        void CEmfxParser::Read_EMR_SETARCDIRECTION()
        {
                unsigned int unDirection;

                *m_pOutput >> unDirection;

                HANDLE_EMR_SETARCDIRECTION(unDirection);
        }

        void CEmfxParser::Read_EMR_FILLPATH()
        {
                TEmfRectL oBounds;

                *m_pOutput >> oBounds;

                HANDLE_EMR_FILLPATH(oBounds);
        }

        void CEmfxParser::Read_EMR_SETMAPMODE()
        {
                unsigned int ulMapMode;

                *m_pOutput >> ulMapMode;

                HANDLE_EMR_SETMAPMODE(ulMapMode);
        }

        void CEmfxParser::Read_EMR_SETWINDOWORGEX()
        {
                TEmfPointL oOrigin;

                *m_pOutput >> oOrigin;

                HANDLE_EMR_SETWINDOWORGEX(oOrigin);
        }

        void CEmfxParser::Read_EMR_SETWINDOWEXTEX()
        {
                TEmfSizeL oExtent;

                *m_pOutput >> oExtent;

                HANDLE_EMR_SETWINDOWEXTEX(oExtent);
        }

        void CEmfxParser::Read_EMR_SETVIEWPORTORGEX()
        {
                TEmfPointL oOrigin;

                *m_pOutput >> oOrigin;

                HANDLE_EMR_SETVIEWPORTORGEX(oOrigin);
        }

        void CEmfxParser::Read_EMR_SETVIEWPORTEXTEX()
        {
                TEmfSizeL oExtent;

                *m_pOutput >> oExtent;

                HANDLE_EMR_SETVIEWPORTEXTEX(oExtent);
        }

        void CEmfxParser::Read_EMR_SETSTRETCHBLTMODE()
        {
                unsigned int ulStretchMode;

                *m_pOutput >> ulStretchMode;

                HANDLE_EMR_SETSTRETCHBLTMODE(ulStretchMode);
        }

        void CEmfxParser::Read_EMR_SETICMMODE()
        {
                unsigned int ulICMMode;

                *m_pOutput >> ulICMMode;

                HANDLE_EMR_SETICMMODE(ulICMMode);
        }

        void CEmfxParser::Read_EMR_CREATEDIBPATTERNBRUSHPT()
        {
                unsigned int ulBrushIndex;
                TEmfDibPatternBrush oDibBrush;

                *m_pOutput >> ulBrushIndex;
                *m_pOutput >> oDibBrush;

                if (oDibBrush.cbBits > 0)
                        *m_pOutput >> m_oStream;

                HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(ulBrushIndex, oDibBrush);
        }

        void CEmfxParser::Read_EMR_SELECTCLIPPATH()
        {
                unsigned int unRegionMode;

                *m_pOutput >> unRegionMode;

                HANDLE_EMR_SELECTCLIPPATH(unRegionMode);
        }

        void CEmfxParser::Read_EMR_SETBKCOLOR()
        {
                TEmfColor oColor;

                *m_pOutput >> oColor;

                HANDLE_EMR_SETBKCOLOR(oColor);
        }

        void CEmfxParser::Read_EMR_EXCLUDECLIPRECT()
        {
                // TODO: Проверить как найдется файл
                TEmfRectL oClip;

                *m_pOutput >> oClip;

                HANDLE_EMR_EXCLUDECLIPRECT(oClip);
        }

        void CEmfxParser::Read_EMR_EXTSELECTCLIPRGN()
        {
                unsigned int ulRgnDataSize, ulRegionMode;

                *m_pOutput >> ulRgnDataSize;
                *m_pOutput >> ulRegionMode;

                HANDLE_EMR_EXTSELECTCLIPRGN(ulRgnDataSize, ulRegionMode);
        }

        void CEmfxParser::Read_EMR_SETMETARGN()
        {
                HANDLE_EMR_SETMETARGN();
        }

        void CEmfxParser::Read_EMR_SETROP2()
        {
                unsigned int ulRop2Mode;

                *m_pOutput >> ulRop2Mode;

                HANDLE_EMR_SETROP2(ulRop2Mode);
        }

        void CEmfxParser::Read_EMR_CREATEPALETTE()
        {
                unsigned int ulPaletteIndex;
                CEmfLogPalette* pPalette = new CEmfLogPalette();
                if (!pPalette)
                        return SetError();

                *m_pOutput >> ulPaletteIndex;
                *m_pOutput >> *pPalette;

                HANDLE_EMR_CREATEPALETTE(ulPaletteIndex, pPalette);
        }

        void CEmfxParser::Read_EMR_SELECTPALETTE()
        {
                unsigned int ulIndex;

                *m_pOutput >> ulIndex;

                HANDLE_EMR_SELECTPALETTE(ulIndex);
        }

        void CEmfxParser::Read_EMR_REALIZEPALETTE()
        {
                HANDLE_EMR_REALIZEPALETTE();
        }

        void CEmfxParser::Read_EMR_INTERSECTCLIPRECT()
        {
                TEmfRectL oClip;

                *m_pOutput >> oClip;

                HANDLE_EMR_INTERSECTCLIPRECT(oClip);
        }

        void CEmfxParser::Read_EMR_SETLAYOUT()
        {
                unsigned int ulLayoutMode;

                *m_pOutput >> ulLayoutMode;

                HANDLE_EMR_SETLAYOUT(ulLayoutMode);
        }

        void CEmfxParser::Read_EMR_SETBRUSHORGEX()
        {
                TEmfPointL oOrigin;

                *m_pOutput >> oOrigin;

                HANDLE_EMR_SETBRUSHORGEX(oOrigin);
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

                HANDLE_EMR_ANGLEARC(oCenter, unRadius, dStartAngle, dSweepAngle);
        }

        void CEmfxParser::Read_EMR_ARC_BASE(TEmfRectL& oBox, TEmfPointL& oStart, TEmfPointL& oEnd)
        {
                *m_pOutput >> oBox;
                *m_pOutput >> oStart;
                *m_pOutput >> oEnd;
        }

        void CEmfxParser::Read_EMR_ARC()
        {
                TEmfRectL oBox;
                TEmfPointL oStart, oEnd;

                Read_EMR_ARC_BASE(oBox, oStart, oEnd);
                HANDLE_EMR_ARC(oBox, oStart, oEnd);
        }

        void CEmfxParser::Read_EMR_ARCTO()
        {
                // TODO: Как найдутся файлы проверить данную запись.
                TEmfRectL oBox;
                TEmfPointL oStart, oEnd;

                Read_EMR_ARC_BASE(oBox, oStart, oEnd);
                HANDLE_EMR_ARCTO(oBox, oStart, oEnd);
        }

        void CEmfxParser::Read_EMR_CHORD()
        {
                // TODO: Как найдутся файлы проверить данную запись.
                TEmfRectL oBox;
                TEmfPointL oStart, oEnd;

                Read_EMR_ARC_BASE(oBox, oStart, oEnd);
                HANDLE_EMR_CHORD(oBox, oStart, oEnd);
        }

        void CEmfxParser::Read_EMR_ELLIPSE()
        {
                TEmfRectL oBox;

                *m_pOutput >> oBox;

                HANDLE_EMR_ELLIPSE(oBox);
        }

        void CEmfxParser::Read_EMR_EXTTEXTOUTA()
        {
                // TODO: Как найдутся файлы проверить данную запись.
                TEmfExtTextoutA oText;

                *m_pOutput >> oText;

                HANDLE_EMR_EXTTEXTOUTA(oText);
        }

        void CEmfxParser::Read_EMR_EXTTEXTOUTW()
        {
                TEmfExtTextoutW oText;

                *m_pOutput >> oText;

                HANDLE_EMR_EXTTEXTOUTW(oText);
        }

        void CEmfxParser::Read_EMR_LINETO()
        {
                TEmfPointL oPoint;

                *m_pOutput >> oPoint;

                HANDLE_EMR_LINETO(oPoint);
        }

        void CEmfxParser::Read_EMR_PIE()
        {
                // TODO: Как найдутся файлы проверить данную запись.
                TEmfRectL oBox;
                TEmfPointL oStart, oEnd;

                Read_EMR_ARC_BASE(oBox, oStart, oEnd);
                HANDLE_EMR_PIE(oBox, oStart, oEnd);
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

                for (unsigned int ulIndex = 1; ulIndex < ulCount; ulIndex += 3)
                {
                        *m_pOutput >> arPoints[ulIndex];
                        *m_pOutput >> arPoints[ulIndex + 1];
                        *m_pOutput >> arPoints[ulIndex + 2];
                }

                HANDLE_EMR_POLYBEZIER(oBounds, arPoints);
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
                }

                HANDLE_EMR_POLYBEZIERTO_BASE(oBounds, arPoints);
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

                HANDLE_EMR_POLYDRAW_BASE(oBounds, pPoints, unCount, pAbTypes);

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

                for (unsigned int ulIndex = 1; ulIndex < ulCount; ulIndex++)
                        *m_pOutput >> arPoints[ulIndex];

                HANDLE_EMR_POLYGON_BASE(oBounds, arPoints);
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

                for (unsigned int ulIndex = 1; ulIndex < ulCount; ulIndex++)
                        *m_pOutput >> arPoints[ulIndex];

                HANDLE_EMR_POLYLINE_BASE(oBounds, arPoints);
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
                        *m_pOutput >> arPoints[ulIndex];

                HANDLE_EMR_POLYLINETO_BASE(oBounds, arPoints);
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

                *m_pOutput >> oBounds;
                *m_pOutput >> ulNumberOfPolygons;
                *m_pOutput >> ulTotalPointsCount;

                unsigned int* pPolygonPointCount = new unsigned int[ulNumberOfPolygons];
                if (!pPolygonPointCount)
                        return SetError();

                for (unsigned int ulIndex = 0; ulIndex < ulNumberOfPolygons; ulIndex++)
                        *m_pOutput >> pPolygonPointCount[ulIndex];

                for (unsigned int ulPolygonIndex = 0, unStartPointIndex = 0; ulPolygonIndex < ulNumberOfPolygons; ulPolygonIndex++)
                {
                        unsigned int ulCurrentPolygonPointsCount = pPolygonPointCount[ulPolygonIndex];
                        if (0 == ulCurrentPolygonPointsCount)
                                continue;

                        T oPoint;
                        *m_pOutput >> oPoint;

                        MoveTo(oPoint);

                        for (unsigned int ulPointIndex = 1; ulPointIndex < ulCurrentPolygonPointsCount; ulPointIndex++)
                        {
                                unsigned int ulRealPointIndex = ulPointIndex + unStartPointIndex;
                                if (ulRealPointIndex >= ulTotalPointsCount)
                                {
                                        delete[] pPolygonPointCount;
                                        return SetError();
                                }

                                *m_pOutput >> oPoint;

                                LineTo(oPoint);
                        }

                        ClosePath();
                }

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

                *m_pOutput >> oBounds;
                *m_pOutput >> ulNumberOfPolylines;
                *m_pOutput >> ulTotalPointsCount;

                if (0 == ulNumberOfPolylines && 0 == ulTotalPointsCount)
                        return;
                else if (0 == ulNumberOfPolylines || 0 == ulTotalPointsCount)
                        return SetError();

                unsigned int* pPolylinePointCount = new unsigned int[ulNumberOfPolylines];
                for (unsigned int ulIndex = 0; ulIndex < ulNumberOfPolylines; ulIndex++)
                        *m_pOutput >> pPolylinePointCount[ulIndex];

                for (unsigned int ulPolyIndex = 0, ulStartPointIndex = 0; ulPolyIndex < ulNumberOfPolylines; ulPolyIndex++)
                {
                        unsigned int ulCurrentPolylinePointsCount = pPolylinePointCount[ulPolyIndex];
                        if (0 == ulCurrentPolylinePointsCount)
                                continue;

                        T oPoint;
                        *m_pOutput >> oPoint;

                        MoveTo(oPoint);

                        for (unsigned int ulPointIndex = 1; ulPointIndex < ulCurrentPolylinePointsCount; ulPointIndex++)
                        {
                                unsigned int ulRealPointIndex = ulPointIndex + ulStartPointIndex;
                                if (ulRealPointIndex >= ulTotalPointsCount)
                                {
                                        delete[] pPolylinePointCount;
                                        return SetError();
                                }

                                *m_pOutput >> oPoint;

                                LineTo(oPoint);
                        }
                }

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

                HANDLE_EMR_RECTANGLE(oBox);
        }

        void CEmfxParser::Read_EMR_ROUNDRECT()
        {
                TEmfRectL oBox;
                TEmfSizeL oCorner;

                *m_pOutput >> oBox;
                *m_pOutput >> oCorner;

                HANDLE_EMR_ROUNDRECT(oBox, oCorner);
        }
        void CEmfxParser::Read_EMR_SETPIXELV()
        {
                TEmfPointL oPoint;
                TEmfColor oColor;

                *m_pOutput >> oPoint;
                *m_pOutput >> oColor;

                if (NULL != m_pInterpretator)
                        m_pInterpretator->HANDLE_EMR_SETPIXELV(oPoint, oColor);

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

                HANDLE_EMR_SMALLTEXTOUT(oText);
        }
        void CEmfxParser::Read_EMR_STROKEANDFILLPATH()
        {
                TEmfRectL oBounds;

                *m_pOutput >> oBounds;

                HANDLE_EMR_STROKEANDFILLPATH(oBounds);
        }
        void CEmfxParser::Read_EMR_STROKEPATH()
        {
                TEmfRectL oBounds;

                *m_pOutput >> oBounds;

                HANDLE_EMR_STROKEPATH(oBounds);
        }
}
