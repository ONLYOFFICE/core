#include "CEmfInterpretatorSvg.h"

#include "../../Common/MetaFileUtils.h"

namespace MetaFile
{
        CEmfInterpretatorSvg::CEmfInterpretatorSvg(const wchar_t *wsFilePath)
                : m_wsSvgFilePath(wsFilePath)
        {
        }

        CEmfInterpretatorSvg::CEmfInterpretatorSvg(const CEmfInterpretatorSvg &oInterpretator)
                : m_wsSvgFilePath(oInterpretator.m_wsSvgFilePath)
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
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_ALPHABLEND(const TEmfAlphaBlend &oTEmfAlphaBlend, CDataStream &oDataStream)
        {
                unsigned int unSize = oTEmfAlphaBlend.cbBitsSrc + oTEmfAlphaBlend.cbBmiSrc;

                if (0 == unSize)
                        return;

                BYTE *pData = new BYTE[unSize];
                oDataStream.ReadBytes(pData, unSize);
                oDataStream.SeekBack(unSize);

                int nSize = NSBase64::Base64EncodeGetRequiredLength(unSize);
                unsigned char* ucValue = new unsigned char[nSize];

                NSBase64::Base64Encode(pData, unSize, ucValue, &nSize);
                std::wstring wsValue(ucValue, ucValue + nSize);

                WriteNode(L"image", {{L"x", std::to_wstring(oTEmfAlphaBlend.xDest)},
                                     {L"y", std::to_wstring(oTEmfAlphaBlend.yDest)},
                                     {L"width", std::to_wstring(oTEmfAlphaBlend.cxDest)},
                                     {L"height", std::to_wstring(oTEmfAlphaBlend.cyDest)},
                                     {L"xlink:href", L"data:image/png;base64," + wsValue}});

                delete [] ucValue;
                delete [] pData;
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_STRETCHDIBITS(const TEmfStretchDIBITS &oTEmfStretchDIBITS, CDataStream &oDataStream)
        {
                unsigned int unSize = oTEmfStretchDIBITS.cbBitsSrc + oTEmfStretchDIBITS.cbBmiSrc;

                if (0 == unSize)
                        return;

                BYTE *pData = new BYTE[unSize];
                oDataStream.ReadBytes(pData, unSize);
                oDataStream.SeekBack(unSize);

                int nSize = NSBase64::Base64EncodeGetRequiredLength(unSize);
                unsigned char* ucValue = new unsigned char[nSize];

                NSBase64::Base64Encode(pData, unSize, ucValue, &nSize);
                std::wstring wsValue(ucValue, ucValue + nSize);

                WriteNode(L"image", {{L"x", std::to_wstring(oTEmfStretchDIBITS.xDest)},
                                     {L"y", std::to_wstring(oTEmfStretchDIBITS.yDest)},
                                     {L"width", std::to_wstring(oTEmfStretchDIBITS.cxDest)},
                                     {L"height", std::to_wstring(oTEmfStretchDIBITS.cyDest)},
                                     {L"xlink:href", L"data:image/png;base64," + wsValue}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_BITBLT(const TEmfBitBlt &oTEmfBitBlt, CDataStream &oDataStream)
        {
//                WriteNode(L"image", {{L"x", std::to_wstring(oTEmfBitBlt.xDest)},
//                                     {L"y", std::to_wstring(oTEmfBitBlt.yDest)},
//                                     {L"width", std::to_wstring(oTEmfBitBlt.cxDest)},
//                                     {L"height", std::to_wstring(oTEmfBitBlt.cyDest)},
//                                     {L"xlink:href", L"data:image/png;base64," + wsValue}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice, CDataStream &oDataStream)
        {
                unsigned int unSize = oTEmfSetDiBitsToDevice.cbBitsSrc + oTEmfSetDiBitsToDevice.cbBmiSrc;

                if (0 == unSize)
                        return;

                BYTE *pData = new BYTE[unSize];
                oDataStream.ReadBytes(pData, unSize);
                oDataStream.SeekBack(unSize);

                int nSize = NSBase64::Base64EncodeGetRequiredLength(unSize);
                unsigned char* ucValue = new unsigned char[nSize];

                NSBase64::Base64Encode(pData, unSize, ucValue, &nSize);
                std::wstring wsValue(ucValue, ucValue + nSize);

                WriteNode(L"image", {{L"x", std::to_wstring(oTEmfSetDiBitsToDevice.xDest)},
                                     {L"y", std::to_wstring(oTEmfSetDiBitsToDevice.yDest)},
                                     {L"width", std::to_wstring(oTEmfSetDiBitsToDevice.cxSrc)},
                                     {L"height", std::to_wstring(oTEmfSetDiBitsToDevice.cySrc)},
                                     {L"xlink:href", L"data:image/png;base64," + wsValue}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_STRETCHBLT(const TEmfStretchBLT &oTEmfStretchBLT, CDataStream &oDataStream)
        {
                unsigned int unSize = oTEmfStretchBLT.cbBitsSrc + oTEmfStretchBLT.cbBmiSrc;

                if (0 == unSize)
                        return;

                BYTE *pData = new BYTE[unSize];
                oDataStream.ReadBytes(pData, unSize);
                oDataStream.SeekBack(unSize);

                int nSize = NSBase64::Base64EncodeGetRequiredLength(unSize);
                unsigned char* ucValue = new unsigned char[nSize];

                NSBase64::Base64Encode(pData, unSize, ucValue, &nSize);
                std::wstring wsValue(ucValue, ucValue + nSize);

                WriteNode(L"image", {{L"x", std::to_wstring(oTEmfStretchBLT.xDest)},
                                     {L"y", std::to_wstring(oTEmfStretchBLT.yDest)},
                                     {L"width", std::to_wstring(oTEmfStretchBLT.cxDest)},
                                     {L"height", std::to_wstring(oTEmfStretchBLT.cyDest)},
                                     {L"xlink:href", L"data:image/png;base64," + wsValue}});
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
                double dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
                double dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y) - dStartAngle;

                std::wstring wsValue = L"M " + std::to_wstring(oStart.x) + L' ' + std::to_wstring(oStart.y);

                wsValue += L" A " + std::to_wstring((oBox.lRight - oBox.lLeft) / 2) + L' ' +
                                    std::to_wstring((oBox.lBottom - oBox.lTop) / 2) + L' ' +
                                    std::to_wstring(dStartAngle) + L' ' +
                                    L"0 " +
                                    (((dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
                                    std::to_wstring(oEnd.x) + L' ' +
                                    std::to_wstring(oEnd.y);

                WriteNode(L"path" , {{L"d", wsValue},
                                     {L"fill", L"none"},
                                     {L"stroke", L"black"}});

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_ARCTO(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
        {
                double dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
                double dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y) - dStartAngle;

                std::wstring wsValue = L"M " + std::to_wstring(oStart.x) + L' ' + std::to_wstring(oStart.y);

                wsValue += L" A " + std::to_wstring((oBox.lRight - oBox.lLeft) / 2) + L' ' +
                                    std::to_wstring((oBox.lBottom - oBox.lTop) / 2) + L' ' +
                                    std::to_wstring(dStartAngle) + L' ' +
                                    L"0 " +
                                    (((dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
                                    std::to_wstring(oEnd.x) + L' ' +
                                    std::to_wstring(oEnd.y);

                WriteNode(L"path" , {{L"d", wsValue},
                                     {L"fill", L"none"},
                                     {L"stroke", L"black"}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_CHORD(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
        {

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_ELLIPSE(const TEmfRectL &oBox)
        {
                WriteNode(L"ellipse", {{L"cx", std::to_wstring((oBox.lLeft + oBox.lRight) / 2)},
                                       {L"cy", std::to_wstring((oBox.lTop + oBox.lBottom) / 2)},
                                       {L"rx", std::to_wstring((oBox.lRight - oBox.lLeft) / 2)},
                                       {L"ry", std::to_wstring((oBox.lBottom - oBox.lTop) / 2)}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_EXTTEXTOUTA(const TEmfExtTextoutA &oTEmfExtTextoutA)
        {
                WriteNode(L"text", {{L"x", std::to_wstring(oTEmfExtTextoutA.aEmrText.Reference.x)},
                                    {L"y", std::to_wstring(oTEmfExtTextoutA.aEmrText.Reference.y)}},
                          NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfExtTextoutA.aEmrText.OutputString, oTEmfExtTextoutA.aEmrText.Chars));

        }

        void CEmfInterpretatorSvg::HANDLE_EMR_EXTTEXTOUTW(const TEmfExtTextoutW &oTEmfExtTextoutW)
        {
                WriteNode(L"text", {{L"x", std::to_wstring(oTEmfExtTextoutW.wEmrText.Reference.x)},
                                    {L"y", std::to_wstring(oTEmfExtTextoutW.wEmrText.Reference.y)}},
                          NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfExtTextoutW.wEmrText.OutputString, oTEmfExtTextoutW.wEmrText.Chars));
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_LINETO(const TEmfPointL &oPoint)
        {
                WriteNode(L"line", {{L"x1", std::to_wstring(m_oParameters.oCurPos.x)},
                                    {L"y1", std::to_wstring(m_oParameters.oCurPos.y)},
                                    {L"x2", std::to_wstring(oPoint.x)},
                                    {L"y2", std::to_wstring(oPoint.y)},
                                    {L"stroke", L"black"}});

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
                        wsValue += L"C " + std::to_wstring(arPoints[unIndex].x) + L' ' + std::to_wstring(arPoints[unIndex].y) + L' ' +
                                           std::to_wstring(arPoints[unIndex + 1].x) + L' ' + std::to_wstring(arPoints[unIndex + 1].y) + L' ' +
                                           std::to_wstring(arPoints[unIndex + 2].x) + L' ' + std::to_wstring(arPoints[unIndex + 2].y);

                WriteNode(L"path", {{L"d", wsValue},
                                    {L"fill", L"none"},
                                    {L"stroke", L"black"}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                if (arPoints.size() < 4)
                        return;

                std::wstring wsValue = L"M " + std::to_wstring(arPoints[0].x) + L' ' + std::to_wstring(arPoints[0].y) + L' ';

                for (unsigned int unIndex = 1; unIndex < arPoints.size(); unIndex += 3)
                        wsValue += L"C " + std::to_wstring(arPoints[unIndex].x) + L' ' + std::to_wstring(arPoints[unIndex].y) + L' ' +
                                           std::to_wstring(arPoints[unIndex + 1].x) + L' ' + std::to_wstring(arPoints[unIndex + 1].y) + L' ' +
                                           std::to_wstring(arPoints[unIndex + 2].x) + L' ' + std::to_wstring(arPoints[unIndex + 2].y);

                WriteNode(L"path", {{L"d", wsValue},
                                    {L"fill", L"none"},
                                    {L"stroke", L"black"}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {
                if (arPoints.size() < 4)
                        return;

                if (arPoints.size() < 4)
                        return;

                std::wstring wsValue = L"M " + std::to_wstring(arPoints[0].x) + L' ' + std::to_wstring(arPoints[0].y) + L' ';

                for (unsigned int unIndex = 1; unIndex < arPoints.size(); unIndex += 3)
                        wsValue += L"C " + std::to_wstring(arPoints[unIndex].x) + L' ' + std::to_wstring(arPoints[unIndex].y) + L' ' +
                                           std::to_wstring(arPoints[unIndex + 1].x) + L' ' + std::to_wstring(arPoints[unIndex + 1].y) + L' ' +
                                           std::to_wstring(arPoints[unIndex + 2].x) + L' ' + std::to_wstring(arPoints[unIndex + 2].y);

                WriteNode(L"path", {{L"d", wsValue},
                                    {L"fill", L"none"},
                                    {L"stroke", L"black"}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                if (arPoints.size() < 4)
                        return;

                std::wstring wsValue = L"M " + std::to_wstring(arPoints[0].x) + L' ' + std::to_wstring(arPoints[0].y) + L' ';

                for (unsigned int unIndex = 1; unIndex < arPoints.size(); unIndex += 3)
                        wsValue += L"C " + std::to_wstring(arPoints[unIndex].x) + L' ' + std::to_wstring(arPoints[unIndex].y) + L' ' +
                                           std::to_wstring(arPoints[unIndex + 1].x) + L' ' + std::to_wstring(arPoints[unIndex + 1].y) + L' ' +
                                           std::to_wstring(arPoints[unIndex + 2].x) + L' ' + std::to_wstring(arPoints[unIndex + 2].y);

                WriteNode(L"path", {{L"d", wsValue},
                                    {L"fill", L"none"},
                                    {L"stroke", L"black"}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointL *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
        {
                if (0 == unCount || NULL == arPoints || NULL == pAbTypes)
                        return;

                std::wstring wsValue;

                for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
                {
                        if (0x02 == pAbTypes[unIndex])
                                wsValue += L" L " + std::to_wstring(arPoints[unIndex].x) + L' ' + std::to_wstring(arPoints[unIndex].x);
                        else if (0x04 == pAbTypes[unIndex] && unIndex + 3 < unCount)
                        {
                                wsValue += L" C " + std::to_wstring(arPoints[unIndex].x) + L' ' + std::to_wstring(arPoints[unIndex].y) + L' ' +
                                                    std::to_wstring(arPoints[unIndex + 1].x) + L' ' + std::to_wstring(arPoints[unIndex + 1].y) + L' ' +
                                                    std::to_wstring(arPoints[unIndex + 2].x) + L' ' + std::to_wstring(arPoints[unIndex + 2].y);

                                unIndex += 3;
                        }
                        else if (0x06 == pAbTypes[unIndex]) //MoveTo
                                wsValue += L" M " + std::to_wstring(arPoints[unIndex].x) + L' ' + std::to_wstring(arPoints[unIndex].x);
                }
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointS *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
        {
                if (0 == unCount || NULL == arPoints || NULL == pAbTypes)
                        return;

                std::wstring wsValue;

                for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
                {
                        if (0x02 == pAbTypes[unIndex])
                                wsValue += L" L " + std::to_wstring(arPoints[unIndex].x) + L' ' + std::to_wstring(arPoints[unIndex].x);
                        else if (0x04 == pAbTypes[unIndex] && unIndex + 3 < unCount)
                        {
                                wsValue += L" C " + std::to_wstring(arPoints[unIndex].x) + L' ' + std::to_wstring(arPoints[unIndex].y) + L' ' +
                                                    std::to_wstring(arPoints[unIndex + 1].x) + L' ' + std::to_wstring(arPoints[unIndex + 1].y) + L' ' +
                                                    std::to_wstring(arPoints[unIndex + 2].x) + L' ' + std::to_wstring(arPoints[unIndex + 2].y);

                                unIndex += 3;
                        }
                        else if (0x06 == pAbTypes[unIndex]) //MoveTo
                                wsValue += L" M " + std::to_wstring(arPoints[unIndex].x) + L' ' + std::to_wstring(arPoints[unIndex].x);
                }
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {
                if (arPoints.empty())
                        return;

                std::wstring wsValue;

                for (const TEmfPointL& oPoint : arPoints)
                        wsValue += std::to_wstring(oPoint.x) + L',' + std::to_wstring(oPoint.y) + L' ';

                WriteNode(L"polygon", {{L"points", wsValue},
                                       {L"fill", L"none"},
                                       {L"stroke", L"black"}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                if (arPoints.empty())
                        return;

                std::wstring wsValue;

                for (const TEmfPointS& oPoint : arPoints)
                        wsValue += std::to_wstring(oPoint.x) + L',' + std::to_wstring(oPoint.y) + L' ';

                WriteNode(L"polygon", {{L"points", wsValue},
                                       {L"fill", L"none"},
                                       {L"stroke", L"black"}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {
                if (arPoints.empty())
                        return;

                std::wstring wsValue;

                for (const TEmfPointL& oPoint : arPoints)
                     wsValue += std::to_wstring(oPoint.x) + L',' + std::to_wstring(oPoint.y) + L' ';

                wsValue.pop_back();

                WriteNode(L"polyline", {{L"points", wsValue},
                                        {L"fill", L"none"},
                                        {L"stroke", L"black"}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                if (arPoints.empty())
                        return;

                std::wstring wsValue;

                for (const TEmfPointS& oPoint : arPoints)
                     wsValue += std::to_wstring(oPoint.x) + L',' + std::to_wstring(oPoint.y) + L' ';

                wsValue.pop_back();

                WriteNode(L"polyline", {{L"points", wsValue},
                                        {L"fill", L"none"},
                                        {L"stroke", L"black"}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {
                if (arPoints.empty())
                        return;

                std::wstring wsValue = std::to_wstring(m_oParameters.oCurPos.x) + L',' + std::to_wstring(m_oParameters.oCurPos.y);

                for (const TEmfPointL& oPoint : arPoints)
                     wsValue += L' ' + std::to_wstring(oPoint.x) + L',' + std::to_wstring(oPoint.y);

                WriteNode(L"polyline", {{L"points", wsValue},
                                        {L"fill", L"none"},
                                        {L"stroke", L"black"}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                if (arPoints.empty())
                        return;

                std::wstring wsValue = std::to_wstring(m_oParameters.oCurPos.x) + L',' + std::to_wstring(m_oParameters.oCurPos.y);

                for (const TEmfPointS& oPoint : arPoints)
                     wsValue += L' ' + std::to_wstring(oPoint.x) + L',' + std::to_wstring(oPoint.y);

                WriteNode(L"polyline", {{L"points", wsValue},
                                        {L"fill", L"none"},
                                        {L"stroke", L"black"}});
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
                WriteNode(L"rect", {{L"x", std::to_wstring(oBox.lLeft)},
                                    {L"y", std::to_wstring(oBox.lTop)},
                                    {L"width", std::to_wstring(oBox.lRight - oBox.lLeft)},
                                    {L"height", std::to_wstring(oBox.lBottom - oBox.lTop)},
                                    {L"fill", L"none"},
                                    {L"stroke", L"black"}});
        }

        void CEmfInterpretatorSvg::HANDLE_EMR_ROUNDRECT(const TEmfRectL &oBox, const TEmfSizeL &oCorner)
        {
                WriteNode(L"rect", {{L"x", std::to_wstring(oBox.lLeft)},
                                    {L"y", std::to_wstring(oBox.lTop)},
                                    {L"width", std::to_wstring(oBox.lRight - oBox.lLeft)},
                                    {L"height", std::to_wstring(oBox.lBottom - oBox.lTop)},
                                    {L"rx", std::to_wstring(oCorner.cx)},
                                    {L"ry", std::to_wstring(oCorner.cy)},
                                    {L"fill", L"none"},
                                    {L"stroke", L"black"}});
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
                m_oXmlWriter.WriteNodeEnd(L"svg", true, false);
        }

        void CEmfInterpretatorSvg::End()
        {
                m_oXmlWriter.WriteNodeEnd(L"svg", false, false);
                m_oXmlWriter.SaveToFile((!m_wsSvgFilePath.empty()) ? m_wsSvgFilePath : L"temp.svg");
        }

        void CEmfInterpretatorSvg::WriteNode(const std::wstring &wsNodeName, const std::vector<std::pair<std::wstring, std::wstring>> &arAttributes, const std::wstring &wsValueNode)
        {
                m_oXmlWriter.WriteNodeBegin(wsNodeName, true);

                for (const std::pair<std::wstring, std::wstring>& oAttribute : arAttributes)
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

}


