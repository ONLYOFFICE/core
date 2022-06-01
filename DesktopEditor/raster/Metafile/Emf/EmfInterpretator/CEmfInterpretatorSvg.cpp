#include "CEmfInterpretatorSvg.h"

#include "../../Common/MetaFileUtils.h"

#include "../Image.h"

namespace MetaFile
{               
        CEmfInterpretatorSvg::CEmfInterpretatorSvg(const wchar_t *wsFilePath, CEmfParserBase* pParser)
                : m_wsSvgFilePath(wsFilePath), m_pParser(pParser), m_dScaleX(1), m_dScaleY(1)
        {
        }

        CEmfInterpretatorSvg::CEmfInterpretatorSvg(const CEmfInterpretatorSvg &oInterpretator)
                : m_wsSvgFilePath(oInterpretator.m_wsSvgFilePath), m_pParser(NULL), m_dScaleX(1), m_dScaleY(1)
        {
        }

        CEmfInterpretatorSvg::~CEmfInterpretatorSvg()
        {

        }

        InterpretatorType CEmfInterpretatorSvg::GetType() const
        {
                return InterpretatorType::Svg;
        }

        void CEmfInterpretatorSvg::SetOutputDevice(const wchar_t *wsFilePath)
        {
                m_wsSvgFilePath = wsFilePath;
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_HEADER(const TEmfHeader &oTEmfHeader)
        {
                //TODO: задаются размеры изображения

                m_oViewport.dWidth      = oTEmfHeader.oDevice.cx;
                m_oViewport.dHeight     = oTEmfHeader.oDevice.cy;

                m_oXmlWriter.WriteNodeEnd(L"svg", true, false);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_ALPHABLEND(const TEmfAlphaBlend &oTEmfAlphaBlend, CDataStream &oDataStream)
        {
                //отрисовка в DrawBitmap
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_STRETCHDIBITS(const TEmfStretchDIBITS &oTEmfStretchDIBITS, CDataStream &oDataStream)
        {
                //отрисовка в DrawBitmap
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_BITBLT(const TEmfBitBlt &oTEmfBitBlt, CDataStream &oDataStream)
        {
                //отрисовка в DrawBitmap
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice, CDataStream &oDataStream)
        {
                //отрисовка в DrawBitmap
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_STRETCHBLT(const TEmfStretchBLT &oTEmfStretchBLT, CDataStream &oDataStream)
        {
                //отрисовка в DrawBitmap
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_EOF()
        {
                //TODO: конец EMF файла
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SAVEDC()
        {
                //TODO: сохранение состояния
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_RESTOREDC(const int &nIndexDC)
        {
                //TODO: восстановление состояния
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_MODIFYWORLDTRANSFORM(const TXForm &oXForm, const unsigned int &unMode)
        {
                //TODO: изменение мирового пространства
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETWORLDTRANSFORM(const TXForm &oXForm)
        {
                //TODO: изменение мирового пространства
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_CREATEBRUSHINDIRECT(const unsigned int &unBrushIndex, const CEmfLogBrushEx *pBrush)
        {
                //TODO: изменение мирового пространства
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETTEXTCOLOR(const TEmfColor &oColor)
        {
                m_oParameters.oTextParameters.oColor = oColor;
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SELECTOBJECT(const unsigned int &unObjectIndex)
        {
                //TODO: выбор объекта
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_EXTCREATEFONTINDIRECTW(const unsigned int &unIndex, CEmfLogFont *oLogFont)
        {
                if (NULL == oLogFont)
                        return;

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETTEXTALIGN(const unsigned int &unAlign)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETBKMODE(const unsigned int &unBgMode)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_DELETEOBJECT(const unsigned int &unObjectIndex)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETMITERLIMIT(const unsigned int &unMeterLimit)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_EXTCREATEPEN(const unsigned int &unPenIndex, CEmfLogPen *pPen, const std::vector<unsigned int> &arUnused)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_CREATEPEN(const unsigned int &unPenIndex, const unsigned int &unWidthX, const CEmfLogPen *pPen)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETPOLYFILLMODE(const unsigned int &unFillMode)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_BEGINPATH()
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_ENDPATH()
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_CLOSEFIGURE()
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_FLATTENPATH()
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_WIDENPATH()
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_ABORTPATH()
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_MOVETOEX(const TEmfPointL &oPoint)
        {
                m_oParameters.oCurPos = oPoint;
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETARCDIRECTION(const unsigned int &unDirection)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_FILLPATH(const TEmfRectL &oBounds)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETMAPMODE(const unsigned int &unMapMode)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETWINDOWORGEX(const TEmfPointL &oOrigin)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETWINDOWEXTEX(const TEmfSizeL &oExtent)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETVIEWPORTORGEX(const TEmfPointL &oOrigin)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETVIEWPORTEXTEX(const TEmfSizeL &oExtent)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETSTRETCHBLTMODE(const unsigned int &unStretchMode)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETICMMODE(const unsigned int &unICMMode)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_CREATEMONOBRUSH(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush, CDataStream &oDataStream)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush, CDataStream &oDataStream)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SELECTCLIPPATH(const unsigned int &unRegionMode)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETBKCOLOR(const TEmfColor &oColor)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_EXCLUDECLIPRECT(const TEmfRectL &oClip)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode, CDataStream &oDataStream)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETMETARGN()
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETROP2(const unsigned int &unRop2Mode)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_CREATEPALETTE(const unsigned int &unPaletteIndex, const CEmfLogPalette *oEmfLogPalette)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SELECTPALETTE(const unsigned int &unPaletteIndex)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_REALIZEPALETTE()
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_INTERSECTCLIPRECT(const TEmfRectL &oClip)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETLAYOUT(const unsigned int &unLayoutMode)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETBRUSHORGEX(const TEmfPointL &oOrigin)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_ANGLEARC(const TEmfPointL &oCenter, const unsigned int &unRadius, const double &dStartAngle, const double &dSweepAngle)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_ARC(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
        {
                TRectD oNewRect = TranslateRect(oBox);

                double dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
                double dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y) - dStartAngle;

                double dXCenter = (oNewRect.dRight - oNewRect.dLeft) / 2;
                double dYCenter = (oNewRect.dBottom - oNewRect.dTop) / 2;

//                double dStartX = oNewRect.dLeft + dXCenter + dXCenter  * cos(dStartAngle);
//                double dStartY = oNewRect.dTop + dYCenter + dYCenter  * sin(dStartAngle);

                std::wstring wsValue = L"M " + std::to_wstring(TranslateX(oStart.x)) + L' ' + std::to_wstring(TranslateY(oStart.y));

                wsValue += L" A " + std::to_wstring(dXCenter) + L' ' +
                                    std::to_wstring(dYCenter) + L' ' +
                                    std::to_wstring(dStartAngle) + L' ' +
                                    L"0 " +
                                    (((dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
                                    std::to_wstring(TranslateX(oEnd.x)) + L' ' +
                                    std::to_wstring(TranslateY(oEnd.y));

                NodeAttributes arAttributes = {{L"d", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, oNewRect);

                WriteNode(L"path" , arAttributes);

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_ARCTO(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
        {                
                TRectD oNewRect = TranslateRect(oBox);

                double dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
                double dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y) - dStartAngle;

                std::wstring wsValue = L"M " + std::to_wstring(TranslateX(oStart.x)) + L' ' + std::to_wstring(TranslateY(oStart.y));

                wsValue += L" A " + std::to_wstring((oNewRect.dRight - oNewRect.dLeft) / 2) + L' ' +
                                    std::to_wstring((oNewRect.dBottom - oNewRect.dTop) / 2) + L' ' +
                                    std::to_wstring(dStartAngle) + L' ' +
                                    L"0 " +
                                    (((dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
                                    std::to_wstring(TranslateX(oEnd.x)) + L' ' +
                                    std::to_wstring(TranslateY(oEnd.y));

                NodeAttributes arAttributes = {{L"d", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, oNewRect);

                WriteNode(L"path" , arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_CHORD(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_ELLIPSE(const TEmfRectL &oBox)
        {
                TRectD oNewRect = TranslateRect(oBox);

                NodeAttributes arAttributes = {{L"cx", std::to_wstring((oNewRect.dLeft   + oNewRect.dRight)  / 2)},
                                               {L"cy", std::to_wstring((oNewRect.dTop    + oNewRect.dBottom) / 2)},
                                               {L"rx", std::to_wstring((oNewRect.dRight  - oNewRect.dLeft)   / 2)},
                                               {L"ry", std::to_wstring((oNewRect.dBottom - oNewRect.dTop)    / 2)}};
                AddStroke(arAttributes);
                AddFill(arAttributes);

                UpdateTransform(arAttributes, oNewRect);

                WriteNode(L"ellipse", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_EXTTEXTOUTA(const TEmfExtTextoutA &oTEmfExtTextoutA)
        {                
                std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfExtTextoutA.aEmrText.OutputString, oTEmfExtTextoutA.aEmrText.Chars);

                WriteText(wsText, oTEmfExtTextoutA.aEmrText.Reference.x, oTEmfExtTextoutA.aEmrText.Reference.y, oTEmfExtTextoutA.Bounds);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_EXTTEXTOUTW(const TEmfExtTextoutW &oTEmfExtTextoutW)
        {
                std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfExtTextoutW.wEmrText.OutputString, oTEmfExtTextoutW.wEmrText.Chars);

                WriteText(wsText, oTEmfExtTextoutW.wEmrText.Reference.x, oTEmfExtTextoutW.wEmrText.Reference.y, oTEmfExtTextoutW.Bounds);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_LINETO(const TEmfPointL &oPoint)
        {
                NodeAttributes arAttributes = {{L"x1", std::to_wstring(TranslateX(m_oParameters.oCurPos.x))},
                                               {L"y1", std::to_wstring(TranslateY(m_oParameters.oCurPos.y))},
                                               {L"x2", std::to_wstring(TranslateX(oPoint.x))},
                                               {L"y2", std::to_wstring(TranslateY(oPoint.y))}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, oPoint.x, oPoint.y);

                WriteNode(L"line", arAttributes);

                m_oParameters.oCurPos = oPoint;
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_PIE(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {
                if (arPoints.size() < 4)
                        return;

                std::wstring wsValue = L"M " + std::to_wstring(arPoints[0].x) + L' ' + std::to_wstring(arPoints[0].y) + L' ';

                for (unsigned int unIndex = 1; unIndex < arPoints.size(); unIndex += 3)
                        wsValue += L"C " + std::to_wstring(TranslateX(arPoints[unIndex].x))     + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y))     + L' ' +
                                           std::to_wstring(TranslateX(arPoints[unIndex + 1].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 1].y)) + L' ' +
                                           std::to_wstring(TranslateX(arPoints[unIndex + 2].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 2].y));

                NodeAttributes arAttributes = {{L"d", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, arPoints);

                WriteNode(L"path", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                if (arPoints.size() < 4)
                        return;

                std::wstring wsValue = L"M " + std::to_wstring(TranslateX(arPoints[0].x)) + L' ' + std::to_wstring(TranslateY(arPoints[0].y)) + L' ';

                for (unsigned int unIndex = 1; unIndex < arPoints.size(); unIndex += 3)
                        wsValue += L"C " + std::to_wstring(TranslateX(arPoints[unIndex].x))     + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y))     + L' ' +
                                           std::to_wstring(TranslateX(arPoints[unIndex + 1].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 1].y)) + L' ' +
                                           std::to_wstring(TranslateX(arPoints[unIndex + 2].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 2].y));

                NodeAttributes arAttributes = {{L"d", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, arPoints);

                WriteNode(L"path", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {
                if (arPoints.size() < 4)
                        return;

                if (arPoints.size() < 4)
                        return;

                std::wstring wsValue = L"M " + std::to_wstring(TranslateX(arPoints[0].x)) + L' ' + std::to_wstring(TranslateY(arPoints[0].y)) + L' ';

                for (unsigned int unIndex = 1; unIndex < arPoints.size(); unIndex += 3)
                        wsValue += L"C " + std::to_wstring(TranslateX(arPoints[unIndex].x))     + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y))     + L' ' +
                                           std::to_wstring(TranslateX(arPoints[unIndex + 1].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 1].y)) + L' ' +
                                           std::to_wstring(TranslateX(arPoints[unIndex + 2].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 2].y));

                NodeAttributes arAttributes = {{L"d", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, arPoints);

                WriteNode(L"path", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                if (arPoints.size() < 4)
                        return;

                std::wstring wsValue = L"M " + std::to_wstring(TranslateX(arPoints[0].x)) + L' ' + std::to_wstring(TranslateY(arPoints[0].y)) + L' ';

                for (unsigned int unIndex = 1; unIndex < arPoints.size(); unIndex += 3)
                        wsValue += L"C " + std::to_wstring(TranslateX(arPoints[unIndex].x))     + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y))     + L' ' +
                                           std::to_wstring(TranslateX(arPoints[unIndex + 1].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 1].y)) + L' ' +
                                           std::to_wstring(TranslateX(arPoints[unIndex + 2].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 2].y));

                NodeAttributes arAttributes = {{L"d", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, arPoints);

                WriteNode(L"path", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointL *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
        {
                if (0 == unCount || NULL == arPoints || NULL == pAbTypes)
                        return;

                std::wstring wsValue;

                for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
                {
                        if (0x02 == pAbTypes[unIndex])
                                wsValue += L" L " + std::to_wstring(TranslateX(arPoints[unIndex].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y));
                        else if (0x04 == pAbTypes[unIndex] && unIndex + 3 < unCount)
                        {
                                wsValue += L" C " + std::to_wstring(TranslateX(arPoints[unIndex].x))     + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y))     + L' ' +
                                                    std::to_wstring(TranslateX(arPoints[unIndex + 1].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 1].y)) + L' ' +
                                                    std::to_wstring(TranslateX(arPoints[unIndex + 2].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 2].y));

                                unIndex += 3;
                        }
                        else if (0x06 == pAbTypes[unIndex]) //MoveTo
                                wsValue += L" M " + std::to_wstring(TranslateX(arPoints[unIndex].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y));
                }

                NodeAttributes arAttributes = {{L"d", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, arPoints, unCount);

                WriteNode(L"path", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointS *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
        {
                if (0 == unCount || NULL == arPoints || NULL == pAbTypes)
                        return;

                std::wstring wsValue;

                for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
                {
                        if (0x02 == pAbTypes[unIndex])
                                wsValue += L" L " + std::to_wstring(TranslateX(arPoints[unIndex].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y));
                        else if (0x04 == pAbTypes[unIndex] && unIndex + 3 < unCount)
                        {
                                wsValue += L" C " + std::to_wstring(TranslateX(arPoints[unIndex].x))     + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y))     + L' ' +
                                                    std::to_wstring(TranslateX(arPoints[unIndex + 1].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 1].y)) + L' ' +
                                                    std::to_wstring(TranslateX(arPoints[unIndex + 2].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex + 2].y));

                                unIndex += 3;
                        }
                        else if (0x06 == pAbTypes[unIndex]) //MoveTo
                                wsValue += L" M " + std::to_wstring(TranslateX(arPoints[unIndex].x)) + L' ' + std::to_wstring(TranslateY(arPoints[unIndex].y));
                }

                NodeAttributes arAttributes = {{L"d", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, arPoints, unCount);

                WriteNode(L"path", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {
                if (arPoints.empty())
                        return;

                std::wstring wsValue;

                for (const TEmfPointL& oPoint : arPoints)
                        wsValue += std::to_wstring(TranslateX(oPoint.x)) + L',' + std::to_wstring(TranslateY(oPoint.y)) + L' ';

                NodeAttributes arAttributes = {{L"points", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, arPoints);

                WriteNode(L"polygon", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                if (arPoints.empty())
                        return;

                std::wstring wsValue;

                for (const TEmfPointS& oPoint : arPoints)
                        wsValue += std::to_wstring(TranslateX(oPoint.x)) + L',' + std::to_wstring(TranslateX(oPoint.y)) + L' ';

                NodeAttributes arAttributes = {{L"points", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, arPoints);

                WriteNode(L"polygon", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {
                if (arPoints.empty())
                        return;

                std::wstring wsValue;

                for (const TEmfPointL& oPoint : arPoints)
                     wsValue += std::to_wstring(TranslateX(oPoint.x)) + L',' + std::to_wstring(TranslateX(oPoint.y)) + L' ';

                NodeAttributes arAttributes = {{L"points", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, arPoints);

                WriteNode(L"polyline", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                if (arPoints.empty())
                        return;

                std::wstring wsValue;

                for (const TEmfPointS& oPoint : arPoints)
                     wsValue += std::to_wstring(TranslateX(oPoint.x)) + L',' + std::to_wstring(TranslateY(oPoint.y)) + L' ';

                wsValue.pop_back();

                NodeAttributes arAttributes = {{L"points", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, arPoints);

                WriteNode(L"polyline", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {
                if (arPoints.empty())
                        return;

                std::wstring wsValue = std::to_wstring(TranslateX(m_oParameters.oCurPos.x)) + L',' + std::to_wstring(TranslateY(m_oParameters.oCurPos.y));

                for (const TEmfPointL& oPoint : arPoints)
                     wsValue += L' ' + std::to_wstring(TranslateX(oPoint.x)) + L',' + std::to_wstring(TranslateY(oPoint.y));

                NodeAttributes arAttributes = {{L"points", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, arPoints);

                WriteNode(L"polyline", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                if (arPoints.empty())
                        return;

                std::wstring wsValue = std::to_wstring(TranslateX(m_oParameters.oCurPos.x)) + L',' + std::to_wstring(TranslateY(m_oParameters.oCurPos.y));

                for (const TEmfPointS& oPoint : arPoints)
                     wsValue += L' ' + std::to_wstring(TranslateX(oPoint.x)) + L',' + std::to_wstring(TranslateY(oPoint.y));

                NodeAttributes arAttributes = {{L"points", wsValue},
                                               {L"fill", L"none"}};

                AddStroke(arAttributes);

                UpdateTransform(arAttributes, arPoints);

                WriteNode(L"polyline", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYGON(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointL>> &arPoints)
        {
                for (const std::vector<TEmfPointL>& arPolygonPoints : arPoints)
                        HANDLE_EMR_POLYGON(oBounds, arPolygonPoints);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYGON(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointS>> &arPoints)
        {
                for (const std::vector<TEmfPointS>& arPolygonPoints : arPoints)
                        HANDLE_EMR_POLYGON(oBounds, arPolygonPoints);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYLINE(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointL>> &arPoints)
        {
                for (const std::vector<TEmfPointL>& arPolygonPoints : arPoints)
                        HANDLE_EMR_POLYLINE(oBounds, arPolygonPoints);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYLINE(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointS>> &arPoints)
        {
                for (const std::vector<TEmfPointS>& arPolygonPoints : arPoints)
                        HANDLE_EMR_POLYLINE(oBounds, arPolygonPoints);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_RECTANGLE(const TEmfRectL &oBox)
        {
                TRectD oNewRect = TranslateRect(oBox);

                NodeAttributes arAttributes = {{L"x", std::to_wstring(oNewRect.dLeft)},
                                               {L"y", std::to_wstring(oNewRect.dTop)},
                                               {L"width", std::to_wstring(oNewRect.dRight - oNewRect.dLeft)},
                                               {L"height", std::to_wstring(oNewRect.dBottom - oNewRect.dTop)}};

                AddStroke(arAttributes);
                AddFill(arAttributes);

                UpdateTransform(arAttributes, oNewRect);

                WriteNode(L"rect", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_ROUNDRECT(const TEmfRectL &oBox, const TEmfSizeL &oCorner)
        {
                TRectD oNewRect = TranslateRect(oBox);

                NodeAttributes arAttributes = {{L"x", std::to_wstring(oNewRect.dLeft)},
                                               {L"y", std::to_wstring(oNewRect.dTop)},
                                               {L"width", std::to_wstring(oNewRect.dRight - oNewRect.dLeft)},
                                               {L"height", std::to_wstring(oNewRect.dBottom - oNewRect.dTop)},
                                               {L"rx", std::to_wstring(TranslateX(oCorner.cx))},
                                               {L"ry", std::to_wstring(TranslateY(oCorner.cy))}};

                AddStroke(arAttributes);
                AddFill(arAttributes);

                UpdateTransform(arAttributes, oNewRect);

                WriteNode(L"rect", arAttributes);
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETPIXELV(const TEmfPointL &oPoint, const TEmfColor &oColor)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SMALLTEXTOUT(const TEmfSmallTextout &oText)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_STROKEANDFILLPATH(const TEmfRectL &oBounds)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_STROKEPATH(const TEmfRectL &oBounds)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_UNKNOWN(CDataStream &oDataStream)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_FILLRGN(const TEmfRectL &oBounds, unsigned int unIhBrush, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
        {

        }

        void CEmfInterpretatorSvg::Begin()
        {
                m_oXmlWriter.WriteNodeBegin(L"svg", true);
                m_oXmlWriter.WriteAttribute(L"xmlns", L"http://www.w3.org/2000/svg");
                m_oXmlWriter.WriteAttribute(L"xmlns:xlink", L"http://www.w3.org/1999/xlink");
        }

        void CEmfInterpretatorSvg::End()
        {
                m_oXmlWriter.WriteNodeEnd(L"svg", false, false);

                std::wstring wsXml = m_oXmlWriter.GetXmlString();

                wsXml.insert(5, L"viewBox=\"" + std::to_wstring(m_oViewport.dX) + L' ' + std::to_wstring(m_oViewport.dY) + L' ' + std::to_wstring(m_oViewport.dWidth) + L' ' + std::to_wstring(m_oViewport.dHeight) + L"\" ");

                m_oXmlWriter.SetXmlString(wsXml);

                m_oXmlWriter.SaveToFile((!m_wsSvgFilePath.empty()) ? m_wsSvgFilePath : L"temp.svg");
        }

        void CEmfInterpretatorSvg::DrawBitmap(double dX, double dY, double dW, double dH, BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight)
        {
                if (0 == unWidth || 0 == unHeight)
                        return;

                Aggplus::CImage oImage;
                BYTE* pBufferPtr = new BYTE[4 * unWidth * unHeight];
                oImage.Create(pBufferPtr, unWidth, unHeight, 4 * unWidth);

		for (int nIndex = 0, nSize = 4 * unWidth * unHeight; nIndex < nSize; nIndex += 4)
		{
			pBufferPtr[0] = (BYTE)pBuffer[nIndex + 0];
			pBufferPtr[1] = (BYTE)pBuffer[nIndex + 1];
			pBufferPtr[2] = (BYTE)pBuffer[nIndex + 2];
			pBufferPtr[3] = (BYTE)pBuffer[nIndex + 3];
			pBufferPtr += 4;
		}

		oImage.SaveFile(L"temp.png", 4);

		NSFile::CFileBinary oFile;

                BYTE* pImageBytes = NULL;
                DWORD ulImageSize;

                oFile.ReadAllBytes(L"temp.png", &pImageBytes, ulImageSize);

                if (0 == ulImageSize)
                        return;

                int nSize = NSBase64::Base64EncodeGetRequiredLength(ulImageSize);
                unsigned char* ucValue = new unsigned char[nSize];

                if (NULL == ucValue)
                        return;

                NSBase64::Base64Encode(pImageBytes, ulImageSize, ucValue, &nSize);
                std::wstring wsValue(ucValue, ucValue + nSize);

                NodeAttributes arAttributes = {{L"x", std::to_wstring(dX)},
                                               {L"y", std::to_wstring(dY)},
                                               {L"width", std::to_wstring(dW)},
                                               {L"height", std::to_wstring(dH)},
                                               {L"xlink:href", L"data:image/png;base64," + wsValue}};;

                UpdateTransform(arAttributes, dX, dY);

                WriteNode(L"image", arAttributes);

                NSFile::CFileBinary::Remove(L"temp.png");

                delete [] ucValue;

                if (NULL != pImageBytes)
                        delete [] pImageBytes;
        }

        void CEmfInterpretatorSvg::WriteNode(const std::wstring &wsNodeName, const NodeAttributes &arAttributes, const std::wstring &wsValueNode)
        {
                m_oXmlWriter.WriteNodeBegin(wsNodeName, true);

                for (const NodeAttribute& oAttribute : arAttributes)
                        m_oXmlWriter.WriteAttribute(oAttribute.first, oAttribute.second);

                if (wsValueNode.empty())
                {
                        m_oXmlWriter.WriteNodeEnd(wsNodeName, true, true);
                }
                else
                {
                        m_oXmlWriter.WriteNodeEnd(wsNodeName, true, false);

                        m_oXmlWriter.WriteString(wsValueNode);

                        m_oXmlWriter.WriteNodeEnd(wsNodeName, false, false);
                }
        }

        void CEmfInterpretatorSvg::WriteText(const std::wstring &wsText, double dX, double dY, const TEmfRectL& oBounds)
        {
                NodeAttributes arNodeAttributes;

                std::wstring wsXCoord = std::to_wstring(TranslateX(dX));
                std::wstring wsYCoord = std::to_wstring(TranslateY(dY));

                if (NULL != m_pParser && NULL != m_pParser->GetFont())
                {
                        if (OPAQUE == m_pParser->GetTextBgMode())
                        {
                                std::wstring wsFillRect = L"rgba(" + INTCOLOR_TO_RGB(m_pParser->GetTextBgColor()) + L", 255)";

                                WriteNode(L"rect", {{L"x",      std::to_wstring((oBounds.lLeft))},
                                                    {L"y",      std::to_wstring((oBounds.lTop))},
                                                    {L"width",  std::to_wstring((oBounds.lRight - oBounds.lLeft))},
                                                    {L"height", std::to_wstring((oBounds.lBottom - oBounds.lTop))},
                                                    {L"fill", wsFillRect},
                                                    {L"stroke", L"none"}});
                        }

                        unsigned int ulTextAlign = m_pParser->GetTextAlign();
                        if (ulTextAlign & TA_BASELINE)
                        {
                                // Ничего не делаем
                        }
                        else if (ulTextAlign & TA_BOTTOM)
                        {
                                arNodeAttributes.push_back({L"alignment-baseline", L"bottom"});
                        }
                        else // if (ulTextAlign & TA_TOP)
                        {
                                arNodeAttributes.push_back({L"alignment-baseline", L"top"});
                        }

                        if (ulTextAlign & TA_CENTER)
                        {
                                arNodeAttributes.push_back({L"text-anchor", L"middle"});
                        }
                        else if (ulTextAlign & TA_RIGHT)
                        {
                                arNodeAttributes.push_back({L"text-anchor", L"end"});
                        }
                        else //if (ulTextAlign & TA_LEFT)
                        {
                                // Ничего не делаем
                        }

                        IFont *pFont = m_pParser->GetFont();

                        double dFontHeight = TranslateY(pFont->GetHeight());

			if (dFontHeight < 0)
				dFontHeight = -dFontHeight;
			if (dFontHeight < 0.01)
				dFontHeight = 18;

			arNodeAttributes.push_back({L"font-size", std::to_wstring(dFontHeight)});

			std::wstring wsFaceName = pFont->GetFaceName();

                        if (!wsFaceName.empty())
                                arNodeAttributes.push_back({L"font-family", wsFaceName});

                        if (pFont->GetWeight() > 550)
                                arNodeAttributes.push_back({L"font-weight", L"bold"});

                        if (pFont->IsItalic())
                                arNodeAttributes.push_back({L"font-style", L"italic"});

                        if (pFont->IsUnderline())
                                arNodeAttributes.push_back({L"text-decoration", L"underline"});

                        if (pFont->IsStrikeOut())
                                arNodeAttributes.push_back({L"text-decoration", L"line-through"});

                        double dFontCharSpace = TranslateX(pFont->GetCharSet());

                        if (dFontCharSpace > 0)
                                arNodeAttributes.push_back({L"letter-spacing", std::to_wstring(dFontCharSpace)});

                        if (0 != pFont->GetEscapement())
                                arNodeAttributes.push_back({L"transform", L"rotate(" + std::to_wstring(pFont->GetEscapement() / -10) + L' ' + wsXCoord + L' ' + wsYCoord + L')'});
                }

                arNodeAttributes.push_back({L"x", wsXCoord});
                arNodeAttributes.push_back({L"y", wsYCoord});

//                UpdateTransform(arNodeAttributes, TranslateRect(oBounds));
                UpdateTransform(arNodeAttributes, TranslateX(dX), TranslateX(dY));

                WriteNode(L"text", arNodeAttributes, StringNormalization(wsText));
        }

        void CEmfInterpretatorSvg::AddStroke(NodeAttributes &arAttributes)
        {
                if (NULL != m_pParser && NULL != m_pParser->GetPen())
                {
                        arAttributes.push_back({L"stroke", L"rgba(" + INTCOLOR_TO_RGB(m_pParser->GetPen()->GetColor()) + L"," + std::to_wstring(m_pParser->GetPen()->GetAlpha()) + L")"});

                        double dStrokeWidth = TranslateY(m_pParser->GetPen()->GetWidth());

			if (dStrokeWidth < 1)
				dStrokeWidth = 1;

			if (dStrokeWidth > 0)
				arAttributes.push_back({L"stroke-width", std::to_wstring(dStrokeWidth)});

			unsigned int unMetaPenStyle = m_pParser->GetPen()->GetStyle();
//			unsigned int ulPenType      = unMetaPenStyle & PS_TYPE_MASK;
			unsigned int ulPenStartCap  = unMetaPenStyle & PS_STARTCAP_MASK;
//			unsigned int ulPenEndCap    = unMetaPenStyle & PS_ENDCAP_MASK; // svg не поддерживает разные стили для сторон
			unsigned int ulPenJoin      = unMetaPenStyle & PS_JOIN_MASK;
			unsigned int ulPenStyle     = unMetaPenStyle & PS_STYLE_MASK;

			if (PS_STARTCAP_ROUND == ulPenStartCap)
				arAttributes.push_back({L"stroke-linecap", L"round"});
			else if (PS_STARTCAP_SQUARE == ulPenStartCap)
				arAttributes.push_back({L"stroke-linecap", L"square"});
			else if (PS_STARTCAP_FLAT == ulPenStartCap)
				arAttributes.push_back({L"stroke-linecap", L"butt"});

			if (PS_JOIN_ROUND == ulPenJoin)
				arAttributes.push_back({L"stroke-linejoin", L"round"});
			else if (PS_JOIN_BEVEL == ulPenJoin)
				arAttributes.push_back({L"stroke-linejoin", L"bevel"});
			else if (PS_JOIN_MITER == ulPenJoin)
				arAttributes.push_back({L"stroke-linejoin", L"miter"});

                        if (PS_DASH == ulPenStyle)
                                arAttributes.push_back({L"stroke-dasharray", std::to_wstring(dStrokeWidth * 4) + L' ' + std::to_wstring(dStrokeWidth * 2)});
                        else if (PS_DOT == ulPenStyle)
                                arAttributes.push_back({L"stroke-dasharray", std::to_wstring(dStrokeWidth) + L' ' + std::to_wstring(dStrokeWidth)});
                        else if (PS_DASHDOT == ulPenStyle)
                                arAttributes.push_back({L"stroke-dasharray", std::to_wstring(dStrokeWidth * 4) + L' ' + std::to_wstring(dStrokeWidth * 2) + L' ' + std::to_wstring(dStrokeWidth) + L' ' + std::to_wstring(dStrokeWidth * 2)});
                        else if (PS_DASHDOTDOT == ulPenStyle)
                                arAttributes.push_back({L"stroke-dasharray", std::to_wstring(dStrokeWidth * 4) + L' ' + std::to_wstring(dStrokeWidth * 2) + L' ' + std::to_wstring(dStrokeWidth) + L' ' + std::to_wstring(dStrokeWidth * 2) + L' ' + std::to_wstring(dStrokeWidth) + L' ' + std::to_wstring(dStrokeWidth * 2)});
                }
                else arAttributes.push_back({L"stroke", L"black"});
        }

        void CEmfInterpretatorSvg::AddFill(NodeAttributes &arAttributes)
        {
                if (NULL != m_pParser && NULL != m_pParser->GetBrush())
                {
                        if (BS_SOLID == m_pParser->GetBrush()->GetStyle())
                                arAttributes.push_back({L"fill", L"rgba(" + INTCOLOR_TO_RGB(m_pParser->GetBrush()->GetColor()) + L"," + std::to_wstring(m_pParser->GetBrush()->GetAlpha()) + L")"});
                        else
                                arAttributes.push_back({L"fill", L"none"});
                }
                else arAttributes.push_back({L"fill", L"none"});
        }

        void CEmfInterpretatorSvg::UpdateTransform(NodeAttributes &arAttributes, double dX, double dY)
        {
                if (dX < m_oViewport.dX)
                        m_oViewport.dX = dX;

                if (dY < m_oViewport.dY)
                        m_oViewport.dY = dY;
                return;

                double dShiftX = 0, dShiftY;

                if (dX < 0)
                        dShiftX = -dX + 1;

                if (dY < 0)
                        dShiftY = -dY + 1;

                if (0 != dShiftX || 0 != dShiftY)
                        arAttributes.push_back({L"transform", L"translate(" + std::to_wstring(dShiftX) + L' ' + std::to_wstring(dShiftY) + L')'});

        }

        void CEmfInterpretatorSvg::UpdateTransform(NodeAttributes &arAttributes, const TRectD &oRect)
        {
                if (std::min(oRect.dLeft, oRect.dRight) < m_oViewport.dX)
                        m_oViewport.dX = std::min(oRect.dLeft, oRect.dRight);

                if (std::min(oRect.dBottom, oRect.dTop) < m_oViewport.dY)
                        m_oViewport.dY = std::min(oRect.dBottom, oRect.dTop);

                return;

                double nMinX = std::min(oRect.dLeft, oRect.dRight);

                if (nMinX < 0)
                        nMinX = -nMinX + 1;
                else nMinX = 0;

                if ((oRect.dRight - oRect.dLeft) < 0)
                        nMinX += oRect.dRight - oRect.dLeft;

                double nMinY = std::min(oRect.dTop, oRect.dBottom);

                if (nMinY < 0)
                        nMinY = -nMinY + 1;
                else nMinY = 0;

                if ((oRect.dBottom - oRect.dTop) < 0)
                        nMinY += oRect.dBottom - oRect.dTop;

                if (0 != nMinX || 0 != nMinY)
                        arAttributes.push_back({L"transform", L"translate(" + std::to_wstring(nMinX) + L' ' + std::to_wstring(nMinY) + L')'});
        }

        void CEmfInterpretatorSvg::UpdateTransform(NodeAttributes &arAttributes, const std::vector<TEmfPointL> &arPoints)
        {
                double dMinX = 0, dMinY = 0;
                for (const TEmfPointL& oPoint : arPoints)
                {
                        if (oPoint.x < dMinX) dMinX = oPoint.x;
                        if (oPoint.y < dMinY) dMinY = oPoint.y;
                }

                if (0 != dMinX || 0 != dMinY)
                        UpdateTransform(arAttributes, dMinX, dMinY);
        }

        void CEmfInterpretatorSvg::UpdateTransform(NodeAttributes &arAttributes, const std::vector<TEmfPointS> &arPoints)
        {
                short shMinX = 0, shMinY = 0;
                for (const TEmfPointS& oPoint : arPoints)
                {
                        if (oPoint.x < shMinX) shMinX = oPoint.x;
                        if (oPoint.y < shMinY) shMinY = oPoint.y;
                }

                if (0 != shMinX || 0 != shMinY)
                        UpdateTransform(arAttributes, shMinX, shMinY);
        }

        void CEmfInterpretatorSvg::UpdateTransform(NodeAttributes &arAttributes, TEmfPointL *arPoints, unsigned int unCount)
        {
                if (NULL == arPoints || 0 == unCount)
                        return;

                double dMinX = 0, dMinY = 0;
                for (unsigned int unIndex = 0; unIndex < unCount; ++unCount)
                {
                        if (arPoints[unIndex].x < dMinX) dMinX = arPoints[unIndex].x;
                        if (arPoints[unIndex].y < dMinY) dMinY = arPoints[unIndex].y;
                }

                if (0 != dMinX || 0 != dMinY)
                        UpdateTransform(arAttributes, dMinX, dMinY);
        }

        void CEmfInterpretatorSvg::UpdateTransform(NodeAttributes &arAttributes, TEmfPointS *arPoints, unsigned int unCount)
        {
                if (NULL == arPoints || 0 == unCount)
                        return;

                short shMinX = 0, shMinY = 0;
                for (unsigned int unIndex = 0; unIndex < unCount; ++unCount)
                {
                        if (arPoints[unIndex].x < shMinX) shMinX = arPoints[unIndex].x;
                        if (arPoints[unIndex].y < shMinY) shMinY = arPoints[unIndex].y;
                }

                if (0 != shMinX || 0 != shMinY)
                        UpdateTransform(arAttributes, shMinX, shMinY);
        }

        double CEmfInterpretatorSvg::TranslateX(double dX)
        {
                if (NULL != m_pParser && NULL != m_pParser->GetTransform())
                        return dX * m_dScaleX * m_pParser->GetTransform()->M11;

                return  dX * m_dScaleX;
        }

        double CEmfInterpretatorSvg::TranslateY(double dY)
        {
                if (NULL != m_pParser && NULL != m_pParser->GetTransform())
                        return dY * m_dScaleY * m_pParser->GetTransform()->M22;

                return dY * m_dScaleY;
        }

        TRectD CEmfInterpretatorSvg::TranslateRect(const TEmfRectL &oRect)
        {
                TRectD oNewRect;

                oNewRect.dLeft   = TranslateX(oRect.lLeft);
                oNewRect.dTop    = TranslateY(oRect.lTop);
                oNewRect.dRight  = TranslateX(oRect.lRight);
                oNewRect.dBottom = TranslateY(oRect.lBottom);

                if (oNewRect.dRight < oNewRect.dLeft)
                {
                        double dTempValue = oNewRect.dLeft;
                        oNewRect.dLeft    = oNewRect.dRight;
                        oNewRect.dRight   = dTempValue;
                }

                if (oNewRect.dBottom < oNewRect.dTop)
                {
                        double dTempValue = oNewRect.dTop;
                        oNewRect.dTop     = oNewRect.dBottom;
                        oNewRect.dBottom  = dTempValue;
                }

                return oNewRect;
        }
}


