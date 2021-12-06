#include "CEmfPlusParser.h"

#define EMRPLUS_OFFSETCLIP              0x4035
#define EMRPLUS_RESETCLIP               0x4031
#define EMFPLUS_SETCLIPPATH             0x4033
#define EMFPLUS_SETCLIPRECT             0x4032
#define EMFPLUS_SETCLIPREGION           0x4034

#define EMFPLUS_COMMENT                 0x4003

#define EMFPLUS_ENDOFFILE               0x4002
#define EMFPLUS_GETDC                   0x4004
#define EMFPLUS_HEADER                  0x4001

#define EMFPLUS_CLEAR                   0x4009
#define EMFPLUS_DRAWARC                 0x4012
#define EMFPLUS_DRAWBEZIERS             0x4019
#define EMFPLUS_DRAWCLOSEDCURVE         0x4017
#define EMFPLUS_DRAWCURVE               0x4018
#define EMFPLUS_DRAWDRIVERSTRING        0x4036
#define EMFPLUS_DRAWELLIPSE             0x400F
#define EMFPLUS_DRAWIMAGE               0x401A
#define EMFPLUS_DRAWIMAGEPOINTS         0x401B
#define EMFPLUS_DRAWLINES               0x400D
#define EMFPLUS_DRAWPATH                0x4015
#define EMFPLUS_DRAWPIE                 0x4011
#define EMFPLUS_DRAWRECTS               0x400B
#define EMFPLUS_DRAWSTRING              0x401C
#define EMFPLUS_FILLCLOSEDCURVE         0x4016
#define EMFPLUS_FILLELLIPSE             0x400E
#define EMFPLUS_FILLPATH                0x4014
#define EMFPLUS_FILLPIE                 0x4010
#define EMFPLUS_FILLPOLYGON             0x400C
#define EMFPLUS_FILLRECTS               0x400A
#define EMFPLUS_FILLREGION              0x4013

#define EMFPLUS_OBJECT                  0x4008
#define EMFPLUS_SERIALIZABLEOBJECT      0x4038

#define EMFPLUS_SETANTIALIASMODE        0x401E
#define EMFPLUS_SETCOMPOSITINGMODE      0x4023
#define EMFPLUS_SETCOMPOSITINGQUALITY   0x4024
#define EMFPLUS_SETINTERPOLATIONMODE    0x4021
#define EMFPLUS_SETPIXELOFFSETMODE      0x4022
#define EMFPLUS_SETRENDERINGORIGIN      0x401D
#define EMFPLUS_SETTEXTCONTRAST         0x4020
#define EMFPLUS_SETTEXTRENDERINGHINT    0x401F

#define EMFPLUS_BEGINCONTAINER          0x4027
#define EMFPLUS_BEGINCONTAINERNOPARAMS  0x4028
#define EMFPLUS_ENDCONTAINER            0x4029
#define EMFPLUS_RESTORE                 0x4026
#define EMFPLUS_SAVE                    0x4025

#define EMFPLUS_SETTSCLIP               0x403A
#define EMFPLUS_SETTSGRAPHICS           0x4039

#define EMFPLUS_MULTIPLYWORLDTRANSFORM  0x402C
#define EMFPLUS_RESETWORLDTRANSFORM     0x402B
#define EMFPLUS_ROTATEWORLDTRANSFORM    0x402F
#define EMFPLUS_SCALEWORLDTRANSFORM     0x402E
#define EMFPLUS_SETPAGETRANSFORM        0x4030
#define EMFPLUS_SETWORLDTRANSFORM       0x402A
#define EMFPLUS_TRANSLATEWORLDTRANSFORM 0x402D

#include <map>
#include <iostream>

#include "CEmfParser.h"
#include "../EmfInterpretator/CEmfInterpretator.h"
#include "../EmfInterpretator/CEmfInterpretatorXml.h"
#include "../EmfInterpretator/CEmfInterpretatorArray.h"
#include "../EmfInterpretator/CEmfInterpretatorRender.h"

namespace MetaFile
{
        static std::map<unsigned short, std::wstring> ActionNamesEmfPlus =
        {
                {0x4035, L"EMRPLUS_OFFSETCLIP"},
                {0x4031, L"EMRPLUS_RESETCLIP"},
                {0x4033, L"EMFPLUS_SETCLIPPATH"},
                {0x4032, L"EMFPLUS_SETCLIPRECT"},
                {0x4034, L"EMFPLUS_SETCLIPREGION"},

                {0x4003, L"EMFPLUS_COMMENT"},

                {0x4002, L"EMFPLUS_ENDOFFILE"},
                {0x4004, L"EMFPLUS_GETDC"},
                {0x4001, L"EMFPLUS_HEADER"},

                {0x4009, L"EMFPLUS_CLEAR"},
                {0x4012, L"EMFPLUS_DRAWARC"},
                {0x4019, L"EMFPLUS_DRAWBEZIERS"},
                {0x4017, L"EMFPLUS_DRAWCLOSEDCURVE"},
                {0x4018, L"EMFPLUS_DRAWCURVE"},
                {0x4036, L"EMFPLUS_DRAWDRIVERSTRING"},
                {0x400F, L"EMFPLUS_DRAWELLIPSE"},
                {0x401A, L"EMFPLUS_DRAWIMAGE"},
                {0x401B, L"EMFPLUS_DRAWIMAGEPOINTS"},
                {0x400D, L"EMFPLUS_DRAWLINES"},
                {0x4015, L"EMFPLUS_DRAWPATH"},
                {0x4011, L"EMFPLUS_DRAWPIE"},
                {0x400B, L"EMFPLUS_DRAWRECTS"},
                {0x401C, L"EMFPLUS_DRAWSTRING"},
                {0x4016, L"EMFPLUS_FILLCLOSEDCURVE"},
                {0x400E, L"EMFPLUS_FILLELLIPSE"},
                {0x4014, L"EMFPLUS_FILLPATH"},
                {0x4010, L"EMFPLUS_FILLPIE"},
                {0x400C, L"EMFPLUS_FILLPOLYGON"},
                {0x400A, L"EMFPLUS_FILLRECTS"},
                {0x4013, L"EMFPLUS_FILLREGION"},

                {0x4008, L"EMFPLUS_OBJECT"},
                {0x4038, L"EMFPLUS_SERIALIZABLEOBJECT"},

                {0x401E, L"EMFPLUS_SETANTIALIASMODE"},
                {0x4023, L"EMFPLUS_SETCOMPOSITINGMODE"},
                {0x4024, L"EMFPLUS_SETCOMPOSITINGQUALITY"},
                {0x4021, L"EMFPLUS_SETINTERPOLATIONMODE"},
                {0x4022, L"EMFPLUS_SETPIXELOFFSETMODE"},
                {0x401D, L"EMFPLUS_SETRENDERINGORIGIN"},
                {0x4020, L"EMFPLUS_SETTEXTCONTRAST"},
                {0x401F, L"EMFPLUS_SETTEXTRENDERINGHINT"},

                {0x4027, L"EMFPLUS_BEGINCONTAINER"},
                {0x4028, L"EMFPLUS_BEGINCONTAINERNOPARAMS"},
                {0x4029, L"EMFPLUS_ENDCONTAINER"},
                {0x4026, L"EMFPLUS_RESTORE"},
                {0x4025, L"EMFPLUS_SAVE"},

                {0x403A, L"EMFPLUS_SETTSCLIP"},
                {0x4039, L"EMFPLUS_SETTSGRAPHICS"},

                {0x402C, L"EMFPLUS_MULTIPLYWORLDTRANSFORM"},
                {0x402B, L"EMFPLUS_RESETWORLDTRANSFORM"},
                {0x402F, L"EMFPLUS_ROTATEWORLDTRANSFORM"},
                {0x402E, L"EMFPLUS_SCALEWORLDTRANSFORM"},
                {0x4030, L"EMFPLUS_SETPAGETRANSFORM"},
                {0x402A, L"EMFPLUS_SETWORLDTRANSFORM"},
                {0x402D, L"EMFPLUS_TRANSLATEWORLDTRANSFORM"}
        };

        CEmfPlusParser::CEmfPlusParser(const CEmfInterpretatorBase *pEmfInterpretator, const TEmfHeader& oHeader)
                : m_bBanEmfProcessing(false),
                  m_unLogicalDpiX(0),
                  m_unLogicalDpiY(0),
                  m_pContineudObject(NULL)
        {
                if (NULL != pEmfInterpretator)
                {
                        m_oHeader = oHeader;

                        if (pEmfInterpretator->GetType() == Emf)
                        {
                                m_pInterpretator = new CEmfInterpretator(*(CEmfInterpretator*)pEmfInterpretator);
                        }
                        else if (pEmfInterpretator->GetType() == Render)
                        {
                                m_pInterpretator = new CEmfInterpretatorRender(*(CEmfInterpretatorRender*)pEmfInterpretator, this);
                        }
                        else if (pEmfInterpretator->GetType() == XML)
                        {
                                m_pInterpretator = new CEmfInterpretatorXml(*(CEmfInterpretatorXml*)pEmfInterpretator);
                        }
                        else if (pEmfInterpretator->GetType() == Array)
                        {
                                m_pInterpretator = new CEmfInterpretatorArray(*(CEmfInterpretatorArray*)pEmfInterpretator);
                        }
                }
        }

        CEmfPlusParser::~CEmfPlusParser()
        {
                ClearFile();
                RELEASEOBJECT(m_pInterpretator);

                for (EmfPlusObjects::const_iterator pIter = m_mObjects.begin(); pIter != m_mObjects.end(); ++pIter)
                        delete[] pIter->second;
        }

        bool CEmfPlusParser::OpenFromFile(const wchar_t *wsFilePath)
        {
                return false;
        }

        void CEmfPlusParser::SetStream(BYTE *pBytes, unsigned int unSize)
        {
                m_oStream.SetStream(pBytes, unSize);
        }

        bool CEmfPlusParser::GetBanEMFProcesses()
        {
                return m_bBanEmfProcessing;
        }

        void CEmfPlusParser::PlayFile()
        {
                unsigned short unShType, unShFlags;
                unsigned int unSize;

                do
                {
                        if (m_oStream.IsEof())
                                break;

                        if (m_oStream.CanRead() < 12)
                                break;

                        m_oStream >> unShType;
                        m_oStream >> unShFlags;

                        m_oStream >> unSize;
                        m_oStream >> m_ulRecordSize;

                        unsigned int unRecordPos = m_oStream.Tell();

                        LOGGING(ActionNamesEmfPlus[unShType] << L"  DataSize = " << m_ulRecordSize)

                        switch (unShType)
                        {
                                //Clipping Record Types (Типы записей отсечения)
                                case EMRPLUS_OFFSETCLIP:    Read_EMRPLUS_OFFSETCLIP();              break;
                                case EMRPLUS_RESETCLIP:     Read_EMRPLUS_RESETCLIP();               break;
                                case EMFPLUS_SETCLIPPATH:   Read_EMFPLUS_SETCLIPPATH(unShFlags);    break;
                                case EMFPLUS_SETCLIPRECT:   Read_EMFPLUS_SETCLIPRECT(unShFlags);    break;
                                case EMFPLUS_SETCLIPREGION: Read_EMFPLUS_SETCLIPREGION(unShFlags);  break;

                                //Comment Record Types (Типы записей комментариев)
                                case EMFPLUS_COMMENT: Read_EMFPLUS_COMMENT(); break;

                                //Control Record Types (Типы управляющих записей)
                                case EMFPLUS_ENDOFFILE: Read_EMFPLUS_ENDOFFILE();       break; break;
                                case EMFPLUS_GETDC:     Read_EMFPLUS_GETDC();           break;
                                case EMFPLUS_HEADER:    Read_EMRPLUS_HEADER(unShFlags); break;

                                //Drawing Record Types (Типы записей чертежа)
                                case EMFPLUS_CLEAR:             Read_EMFPLUS_CLEAR();                           break;
                                case EMFPLUS_DRAWARC:           Read_EMFPLUS_DRAWARC(unShFlags);                break;
                                case EMFPLUS_DRAWBEZIERS:       Read_EMFPLUS_DRAWBEZIERS(unShFlags);            break;
                                case EMFPLUS_DRAWCLOSEDCURVE:   Read_EMFPLUS_DRAWCLOSEDCURVE(unShFlags);        break;
                                case EMFPLUS_DRAWCURVE:         Read_EMFPLUS_DRAWCURVE(unShFlags);              break;
                                case EMFPLUS_DRAWDRIVERSTRING:  Read_EMFPLUS_DRAWDRIVERSTRING(unShFlags);       break;
                                case EMFPLUS_DRAWELLIPSE:       Read_EMFPLUS_DRAWELLIPSE(unShFlags);            break;
                                case EMFPLUS_DRAWIMAGEPOINTS:   Read_EMFPLUS_DRAWIMAGEPOINTS(unShFlags);        break;
                                case EMFPLUS_DRAWLINES:         Read_EMFPLUS_DRAWLINES(unShFlags);              break;
                                case EMFPLUS_DRAWPATH:          Read_EMFPLUS_DRAWPATH(unShFlags);               break;
                                case EMFPLUS_DRAWPIE:           Read_EMFPLUS_DRAWPIE(unShFlags);                break;
                                case EMFPLUS_DRAWRECTS:         Read_EMFPLUS_DRAWRECTS(unShFlags);              break;
                                case EMFPLUS_DRAWSTRING:        Read_EMFPLUS_DRAWSTRING(unShFlags);             break;
                                case EMFPLUS_FILLCLOSEDCURVE:   Read_EMFPLUS_FILLCLOSEDCURVE(unShFlags);        break;
                                case EMFPLUS_FILLELLIPSE:       Read_EMFPLUS_FILLELLIPSE(unShFlags);            break;
                                case EMFPLUS_FILLPATH:          Read_EMFPLUS_FILLPATH(unShFlags);               break;
                                case EMFPLUS_FILLPIE:           Read_EMFPLUS_FILLPIE(unShFlags);                break;
                                case EMFPLUS_FILLPOLYGON:       Read_EMFPLUS_FILLPOLYGON(unShFlags);            break;
                                case EMFPLUS_FILLRECTS:         Read_EMFPLUS_FILLRECTS(unShFlags);              break;
                                case EMFPLUS_FILLREGION:        Read_EMFPLUS_FILLREGION(unShFlags);             break;

                                //Object Record Types (Типы записей объектов)
                                case EMFPLUS_OBJECT:            Read_EMFPLUS_OBJECT(unShFlags);                 break;
                                case EMFPLUS_SERIALIZABLEOBJECT:Read_EMFPLUS_SERIALIZABLEOBJECT(unShFlags);     break;

                                //Property Record Types (Типы записей свойств)
                                case EMFPLUS_SETANTIALIASMODE:          Read_EMFPLUS_SETANTIALIASMODE(unShFlags);       break;
                                case EMFPLUS_SETCOMPOSITINGMODE:        Read_EMFPLUS_SETCOMPOSITINGMODE(unShFlags);     break;
                                case EMFPLUS_SETCOMPOSITINGQUALITY:     Read_EMFPLUS_SETCOMPOSITINGQUALITY(unShFlags);  break;
                                case EMFPLUS_SETINTERPOLATIONMODE:      Read_EMFPLUS_SETINTERPOLATIONMODE(unShFlags);   break;
                                case EMFPLUS_SETPIXELOFFSETMODE:        Read_EMFPLUS_SETPIXELOFFSETMODE(unShFlags);     break;
                                case EMFPLUS_SETRENDERINGORIGIN:        Read_EMFPLUS_SETRENDERINGORIGIN();              break;
                                case EMFPLUS_SETTEXTCONTRAST:           Read_EMFPLUS_SETTEXTCONTRAST(unShFlags);        break;
                                case EMFPLUS_SETTEXTRENDERINGHINT:      Read_EMRPLUS_SETTEXTRENDERINGHINT(unShFlags);   break;

                                //State Record Types (Типы записей состояния)
                                case EMFPLUS_BEGINCONTAINER:            Read_EMFPLUS_BEGINCONTAINER(unShFlags); break;
                                case EMFPLUS_BEGINCONTAINERNOPARAMS:    Read_EMFPLUS_BEGINCONTAINERNOPARAMS();  break;
                                case EMFPLUS_ENDCONTAINER:              Read_EMFPLUS_ENDCONTAINER();            break;
                                case EMFPLUS_RESTORE:                   Read_EMFPLUS_RESTORE();                 break;
                                case EMFPLUS_SAVE:                      Read_EMFPLUS_SAVE();                    break;

                                //Terminal Server Record (Запись сервера Терминалов)
                                case EMFPLUS_SETTSCLIP:         Read_EMFPLUS_SETTSCLIP(unShFlags);      break;
                                case EMFPLUS_SETTSGRAPHICS:     Read_EMFPLUS_SETTSGRAPHICS(unShFlags);  break;

                                //Transform Record Types (Преобразование Типов записей)
                                case EMFPLUS_MULTIPLYWORLDTRANSFORM:    Read_EMFPLUS_MULTIPLYWORLDTRANSFORM(unShFlags); break;
                                case EMFPLUS_RESETWORLDTRANSFORM:       Read_EMFPLUS_RESETWORLDTRANSFORM();             break;
                                case EMFPLUS_ROTATEWORLDTRANSFORM:      Read_EMFPLUS_ROTATEWORLDTRANSFORM(unShFlags);   break;
                                case EMFPLUS_SCALEWORLDTRANSFORM:       Read_EMFPLUS_SCALEWORLDTRANSFORM(unShFlags);    break;
                                case EMFPLUS_SETPAGETRANSFORM:          Read_EMFPLUS_SETPAGETRANSFORM(unShFlags);       break;
                                case EMFPLUS_SETWORLDTRANSFORM:         Read_EMFPLUS_SETWORLDTRANSFORM();               break;
                                case EMFPLUS_TRANSLATEWORLDTRANSFORM:   Read_EMFPLUS_TRANSLATEWORLDTRANSFORM(unShFlags);break;
                        }

                        int nNeedSkip = (unRecordPos + m_ulRecordSize) - m_oStream.Tell();
                        m_oStream.Skip(nNeedSkip);

                        LOGGING(L"Skip: " << nNeedSkip)

                        m_ulRecordSize = 0;
                }while(m_oStream.CanRead() > 4);

                if (!CheckError())
                        m_oStream.SeekToStart();

                LOGGING(L"_____________________________________________________")
        }

        void CEmfPlusParser::Scan()
        {
                CEmfInterpretatorBase* pInterpretator = m_pInterpretator;
                m_pInterpretator = NULL;
                PlayFile();
                m_pInterpretator = pInterpretator;
                this->ClearFile();
        }

        EmfParserType CEmfPlusParser::GetType()
        {
                return EmfParserType::EmfPlusParser;
        }

        bool CEmfPlusParser::ReadImage(unsigned int offBmi, unsigned int cbBmi, unsigned int offBits, unsigned int cbBits, unsigned int ulSkip, BYTE **ppBgraBuffer, unsigned int *pulWidth, unsigned int *pulHeight)
        {
                return false;
        }

        void CEmfPlusParser::ReadImage(unsigned short shObjectIndex, bool bIsContineudObject)
        {
                if (NULL == m_pContineudObject)
                {
                        CEmfPlusImage *pImage = new CEmfPlusImage(false);

                        m_oStream >> *pImage;

                        m_mObjects[shObjectIndex] = pImage;
                }
                else
                {
                        if (m_pContineudObject->IsFirstReading())
                        {
                                CEmfPlusImage *pImage = new CEmfPlusImage(true);

                                m_oStream >> *pImage;

                                if (NULL == pImage->pImageData)
                                {
                                        RELEASEOBJECT(pImage)
                                        RELEASEOBJECT(m_pContineudObject);
                                        return;
                                }

                                m_mObjects[shObjectIndex] = pImage;

                                if (NULL != m_pContineudObject)
                                        m_pContineudObject->SetEmfPlusImage(pImage);

                                m_pContineudObject->SetNeedReadSize(m_ulRecordSize - pImage->GetSize() - ((bIsContineudObject) ? 4 : 0));
                                m_oStream >> *m_pContineudObject;
                        }
                        else
                        {
                                m_pContineudObject->SetNeedReadSize(m_ulRecordSize - 4);

                                m_oStream >> *m_pContineudObject;
                        }

                        if (m_pContineudObject->IsLastReading())
                        {
                                m_pContineudObject->UpdateImage();
                                RELEASEOBJECT(m_pContineudObject);
                        }
                        }
        }

        CEmfPlusImage* CEmfPlusParser::GetImage(unsigned int unImageIndex)
        {
                EmfPlusObjects::const_iterator oFoundElement = m_mObjects.find(unImageIndex);

                if (m_mObjects.end() != oFoundElement && oFoundElement->second->GetObjectType() == ObjectTypeImage)
                        return (CEmfPlusImage*)oFoundElement->second;

                return NULL;
        }

        CEmfPlusPath* CEmfPlusParser::ReadPath()
        {
                unsigned int unVersion, unPathPointCount, unPathPointFlags;

                m_oStream >> unVersion;
                m_oStream >> unPathPointCount;

                if (unPathPointCount == 0)
                        return NULL;

                m_oStream >> unPathPointFlags;

                if ((unPathPointFlags >>(20)) & 1 )
                {
                        //Определен флаг R (С игнорируется)
                        return NULL;
                }
                else if ((unPathPointFlags >>(17)) & 1 )
                {
                        //Не определен флаг R, но определен флаг С
                        return NULL;
                }
                else
                {
                        std::vector<TEmfPlusPointF> arPoints    = ReadPointsF(unPathPointCount);
                        std::vector<char> arPointTypes          = ReadPointTypes(unPathPointCount);

                        std::vector<std::pair<char, char>> arFlTp(unPathPointCount);

                        CEmfPlusPath *pPath = new CEmfPlusPath();

                        pPath->MoveTo(arPoints[0].X, arPoints[0].Y);

                        for (unsigned int unIndex = 1; unIndex < unPathPointCount; ++unIndex)
                        {
                                if (ExpressValue(arPointTypes[unIndex], 0, 3) == 0)
                                        pPath->MoveTo(arPoints[unIndex].X, arPoints[unIndex].Y);
                                else
                                        pPath->LineTo(arPoints[unIndex].X, arPoints[unIndex].Y);
                        }

                        return pPath;

                        //Оба флага не определены
                }
                //TODO: реализовать
        }

        CEmfPlusPath *CEmfPlusParser::GetPath(unsigned int unPathIndex)
        {
                EmfPlusObjects::const_iterator oFoundElement = m_mObjects.find(unPathIndex);

                if (m_mObjects.end() != oFoundElement && oFoundElement->second->GetObjectType() == ObjectTypePath)
                        return (CEmfPlusPath*)oFoundElement->second;

                return NULL;
        }

        CEmfPlusRegion *CEmfPlusParser::ReadRegion()
        {
                CEmfPlusRegion *pRegion = new CEmfPlusRegion;

                if (NULL != pRegion)
                        m_oStream >> *pRegion;

                return pRegion;
        }

        CEmfPlusRegion* CEmfPlusParser::GetRegion(unsigned int unRegionIndex)
        {
                EmfPlusObjects::const_iterator oFoundElement = m_mObjects.find(unRegionIndex);

                if (m_mObjects.end() != oFoundElement && oFoundElement->second->GetObjectType() == ObjectTypePath)
                        return (CEmfPlusRegion*)oFoundElement->second;

                return NULL;
        }

        CEmfPlusImageAttributes *CEmfPlusParser::GetImageAttributes(unsigned int unImageAttributesIndex)
        {
                EmfPlusObjects::const_iterator oFoundElement = m_mObjects.find(unImageAttributesIndex);

                if (m_mObjects.end() != oFoundElement && oFoundElement->second->GetObjectType() == ObjectTypeImageAttributes)
                        return (CEmfPlusImageAttributes*)oFoundElement->second;

                return NULL;
        }

        std::vector<TEmfPlusPointF> CEmfPlusParser::ReadPointsF(unsigned int unPointCount)
        {
                std::vector<TEmfPlusPointF> arPoints(unPointCount);

                for (unsigned int unIndex = 0; unIndex < unPointCount; ++unIndex)
                        m_oStream >> arPoints[unIndex];

                return arPoints;
        }

        std::vector<char> CEmfPlusParser::ReadPointTypes(unsigned int unPointCount)
        {
                std::vector<char> arPointTypes(unPointCount);

                for (unsigned int unIndex = 0; unIndex < unPointCount; ++unIndex)
                        m_oStream >> arPointTypes[unIndex];

                return arPointTypes;
        }

        void CEmfPlusParser::DrawRectangle(TEmfPlusRectF oRectangle, bool bStroke, bool bFill)
        {
                if (NULL != m_pInterpretator)
                {
                        TRectD oRectD = oRectangle.GetRectD();
                        if (AD_COUNTERCLOCKWISE != m_pDC->GetArcDirection())
                        {
                                m_pInterpretator->MoveTo(oRectD.dLeft,  oRectD.dTop);
                                m_pInterpretator->LineTo(oRectD.dLeft,  oRectD.dBottom);
                                m_pInterpretator->LineTo(oRectD.dRight, oRectD.dBottom);
                                m_pInterpretator->LineTo(oRectD.dRight, oRectD.dTop);
                        }
                        else
                        {
                                m_pInterpretator->MoveTo(oRectD.dLeft,  oRectD.dTop);
                                m_pInterpretator->LineTo(oRectD.dRight, oRectD.dTop);
                                m_pInterpretator->LineTo(oRectD.dRight, oRectD.dBottom);
                                m_pInterpretator->LineTo(oRectD.dLeft,  oRectD.dBottom);
                        }

                        m_pInterpretator->ClosePath();
                        m_pInterpretator->DrawPath((bStroke ? 1 : 0) + (bFill ? 2 : 0));
                        m_pInterpretator->EndPath();
                }
        }

        void CEmfPlusParser::DrawRectangle(TEmfPlusRect oRectangle, bool bStroke, bool bFill)
        {

        }

        TEmfPlusARGB CEmfPlusParser::ApplyImageAttributes(TEmfPlusRectF &oRectangle, const CEmfPlusImageAttributes &oImageAttributes)
        {
                if (oImageAttributes.eWrapMode == WrapModeClamp)
                {
                        if (oRectangle.dX < 0 || oRectangle.dX < 0 ||
                            oRectangle.dX >= oRectangle.dWidth || oRectangle.dY >= oRectangle.dHeight)
                                return oImageAttributes.oClampColor;

                }
                else
                {
                        if (oRectangle.dX < 0)
                                oRectangle.dX = oRectangle.dWidth * 2 + (int)oRectangle.dX % (int)(oRectangle.dWidth * 2);

                        if (oRectangle.dY < 0)
                                oRectangle.dY = oRectangle.dHeight * 2 + (int)oRectangle.dY % (int)(oRectangle.dHeight * 2);

                        if (oImageAttributes.eWrapMode & WrapModeTileFlipX)
                        {
                                if ((int)(oRectangle.dX / oRectangle.dWidth) % 2 == 0)
                                        oRectangle.dX = (int)oRectangle.dX % (int)oRectangle.dWidth;
                                else
                                        oRectangle.dX = oRectangle.dWidth - 1 - (int)oRectangle.dX % (int)oRectangle.dWidth;
                        }
                        else
                                oRectangle.dX = (int)oRectangle.dX % (int)oRectangle.dWidth;

                        if (oImageAttributes.eWrapMode & WrapModeTileFlipY)
                        {
                                if ((int)(oRectangle.dY / oRectangle.dHeight) % 2 == 0)
                                        oRectangle.dY = (int)oRectangle.dY % (int)oRectangle.dHeight;
                                else
                                        oRectangle.dY = oRectangle.dHeight - 1 - (int)oRectangle.dY % (int)oRectangle.dHeight;
                        }
                        else
                                oRectangle.dY = (int)oRectangle.dY % (int)oRectangle.dHeight;
                }

                return oImageAttributes.oClampColor;
        }

        void CEmfPlusParser::CombineClip(TRectD oClip, int nMode)
        {
                switch (nMode)
                {
                        case CombineModeReplace: break;
                        case CombineModeIntersect: m_pDC->GetClip()->Intersect(oClip); break;
                        case CombineModeUnion: break;
                        case CombineModeXOR: break;
                        case CombineModeExclude:
                        {
                                TRectD oBB;

                                // Поскольку мы реализовываем данный тип клипа с помощью разницы внешнего ректа и заданного, и
                                // пересечением с полученной областью, то нам надо вычесть границу заданного ректа.
                                if (oClip.dLeft < oClip.dRight)
                                {
                                        oClip.dLeft--;
                                        oClip.dRight++;
                                }
                                else
                                {
                                        oClip.dLeft++;
                                        oClip.dRight--;
                                }

                                if (oClip.dTop < oClip.dBottom)
                                {
                                        oClip.dTop--;
                                        oClip.dBottom++;
                                }
                                else
                                {
                                        oClip.dTop++;
                                        oClip.dBottom--;
                                }

                                TRect* pRect = GetDCBounds();
                                TranslatePoint(pRect->nLeft, pRect->nTop, oBB.dLeft, oBB.dTop);
                                TranslatePoint(pRect->nRight, pRect->nBottom, oBB.dRight, oBB.dBottom);

                                m_pDC->GetClip()->Exclude(oClip, oBB);
                                UpdateOutputDC();

                                break;
                        }
                        case CombineModeComplement: break;
                }
        }

        void CEmfPlusParser::DrawImagePoints(unsigned int unImageIndex, unsigned int unImageAttributeIndex,
                                             TEmfPlusRectF oSrcRect, std::vector<TEmfPlusPointF> arPoints)
        {
                if (NULL == m_pInterpretator)
                        return;

                CEmfPlusImage *pImage = GetImage(unImageIndex);

                if (NULL == pImage)
                        return;

                unsigned int unMetafileSize = pImage->GetMetafileSize();

                if (unMetafileSize == 0 || arPoints.size() != 3)
                        return;

                CEmfParser oEmfParser(m_pInterpretator);
                oEmfParser.SetStream(pImage->GetMetafileData(), unMetafileSize);
                oEmfParser.SetFontManager(GetFontManager());
                oEmfParser.m_bView = true;
                oEmfParser.Scan();

                if (!oEmfParser.CheckError())
                {
                        double dM11 = 1, dM12 = 0, dM21 = 0,  dM22 = 1, dX = 0, dY = 0;                            

                        TEmfPlusRectF oRect;
                        oRect.dX = arPoints[0].X;
                        oRect.dY = arPoints[0].Y;
                        oRect.dWidth = arPoints[1].X - arPoints[0].X;
                        oRect.dHeight = arPoints[2].Y - arPoints[0].Y;





                        dM11 = (arPoints[1].X - arPoints[0].X) / oSrcRect.dWidth;
                        dM21 = (arPoints[2].X - arPoints[0].X) / oSrcRect.dHeight;
                        dX = arPoints[0].X - dM11 * oSrcRect.dX - dM21 * oSrcRect.dY;

                        dM12 = (arPoints[1].Y - arPoints[0].Y) / oSrcRect.dWidth;
                        dM22 = (arPoints[2].Y - arPoints[0].Y) / oSrcRect.dHeight;
                        dY = arPoints[0].Y - dM12 * oSrcRect.dX - dM22 * oSrcRect.dY;


                        m_pDC->GetTransform()->Apply(arPoints[0].X, arPoints[0].Y);

                        dX += arPoints[0].X;
                        dY += arPoints[0].Y;

                        CEmfPlusImageAttributes *pImageAttributes = GetImageAttributes(unImageAttributeIndex);

                        if (NULL != pImageAttributes)
                        {
                                ApplyImageAttributes(oSrcRect, *pImageAttributes);

                                if (pImageAttributes->eWrapMode & WrapModeTileFlipX)
                                        dM11 *= -1;

                                if (pImageAttributes->eWrapMode & WrapModeTileFlipY)
                                        dM22 *= -1;
                        }

                        TEmfPlusXForm oNewTransform(dM11, 0, 0, dM22, dX, dY);

                        oEmfParser.SelectWorkspace(oSrcRect.GetRectD());
                        oEmfParser.SetTrasform(oNewTransform);
                        oEmfParser.PlayFile();
                }
        }

        void CEmfPlusParser::Read_EMRPLUS_HEADER(unsigned short unShFlags)
        {
                m_oStream.Skip(4); //Version

                unsigned int unEmfPlusFlags;

                m_oStream >> unEmfPlusFlags;
                m_oStream >> m_unLogicalDpiX;
                m_oStream >> m_unLogicalDpiY;

                if(NULL != m_pInterpretator)
                        m_pInterpretator->Begin();

                //TODO: добавить установление нового Dpi
        }

        void CEmfPlusParser::Read_EMFPLUS_CLEAR()
        {
                TEmfPlusARGB oARGB;

                m_oStream >> oARGB;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_DRAWARC(unsigned short unShFlags)
        {
                if ((unShFlags >>(14)) & 1 )
                        Read_EMFPLUS_DRAWARC_BASE<TEmfPlusRect>(unShFlags);
                else
                        Read_EMFPLUS_DRAWARC_BASE<TEmfPlusRectF>(unShFlags);
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_DRAWARC_BASE(unsigned short unShFlags)
        {
                char chOgjectIndex = ExpressValue(unShFlags, 0, 7);
                double dStartAngle, dSweepAngle;
                T oRect;

                m_oStream >> dStartAngle;
                m_oStream >> dSweepAngle;
                m_oStream >> oRect;
                //TODO: реализовать
        }


        void CEmfPlusParser::Read_EMFPLUS_DRAWBEZIERS(unsigned short unShFlags)
        {
                if ((unShFlags >>(11)) & 1 )
                {
                        //Определен флаг P (С игнорируется)
                        Read_EMFPLUS_DRAWBEZIERS_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
                }
                else if ((unShFlags >>(14)) & 1 )
                {
                        //Не определен флаг P, но определен флаг С
                        Read_EMFPLUS_DRAWBEZIERS_BASE<TEmfPlusPoint>(unShFlags);  // относительное расположение
                }
                else
                {
                        //Оба флага не определены
                        Read_EMFPLUS_DRAWBEZIERS_BASE<TEmfPlusPointF>(unShFlags); // абсолютное расположение
                }

        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_DRAWBEZIERS_BASE(unsigned short unShFlags)
        {
                short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
                unsigned int unCountPoints;

                m_oStream >> unCountPoints;

                std::vector<T> arPoints(unCountPoints);

                for (unsigned int unIndex = 0; unIndex < unCountPoints; ++unIndex)
                {

                }

                //TODO: реализовать
                //TODO: для EmfPlusPointR опредилиться с реализацией
        }

        void CEmfPlusParser::Read_EMFPLUS_DRAWCLOSEDCURVE(unsigned short unShFlags)
        {
                if ((unShFlags >>(11)) & 1 )
                {
                        //Определен флаг P (С игнорируется)
                        Read_EMFPLUS_DRAWCLOSEDCURVE_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
                }
                else if ((unShFlags >>(14)) & 1 )
                {
                        //Не определен флаг P, но определен флаг С
                        Read_EMFPLUS_DRAWCLOSEDCURVE_BASE<TEmfPlusPoint>(unShFlags);  // абсолютное расположение с 16-разрядными координатами
                }
                else
                {
                        //Оба флага не определены
                        Read_EMFPLUS_DRAWCLOSEDCURVE_BASE<TEmfPlusPointF>(unShFlags); // абсолютное расположение с 32-разрядными координатами.
                }
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_DRAWCLOSEDCURVE_BASE(unsigned short unShFlags)
        {
                short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
                double dTension;
                unsigned int unCount;

                m_oStream >> dTension;
                m_oStream >> unCount;

                std::vector<T> arPoints(unCount);

                for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
                {
                        //TODO: реализовать
                }
        }

        void CEmfPlusParser::Read_EMFPLUS_DRAWCURVE(unsigned short unShFlags)
        {
                if ((unShFlags >>(14)) & 1 )
                        Read_EMFPLUS_DRAWCURVE_BASE<TEmfPlusPoint>(unShFlags);
                else
                        Read_EMFPLUS_DRAWCURVE_BASE<TEmfPlusPointF>(unShFlags);
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_DRAWCURVE_BASE(unsigned short unShFlags)
        {
                short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
                double dTension;
                unsigned int unOffset, unNumSegments, unCountPoints;

                m_oStream >> dTension;
                m_oStream >> unOffset;
                m_oStream >> unNumSegments;
                m_oStream >> unCountPoints;

                std::vector<T> arPoints(unCountPoints);

                for (unsigned int unIndex = 0; unIndex < unCountPoints; ++unIndex)
                {
                        //TODO: реализовать
                }
        }

        void CEmfPlusParser::Read_EMFPLUS_DRAWDRIVERSTRING(unsigned short unShFlags)
        {
                short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
                unsigned int unBrushId, unDriverStringOptionsFlags, unMatrixPresent, unGlyphCount;

                m_oStream >> unBrushId;
                m_oStream >> unDriverStringOptionsFlags;
                m_oStream >> unMatrixPresent;
                m_oStream >> unGlyphCount;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_DRAWELLIPSE(unsigned short unShFlags)
        {
                if ((unShFlags >>(14)) & 1 )
                        Read_EMFPLUS_DRAWELLIPSE_BASE<TEmfPlusRect>(unShFlags);
                else
                        Read_EMFPLUS_DRAWELLIPSE_BASE<TEmfPlusRectF>(unShFlags);
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_DRAWELLIPSE_BASE(unsigned short unShFlags)
        {
                short shOgjectIndex = ExpressValue(unShFlags, 0, 7);

                T oRect;

                m_oStream >> oRect;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_DRAWIMAGE(unsigned short unShFlags)
        {
                if ((unShFlags >>(14)) & 1 )
                        Read_EMFPLUS_DRAWIMAGE_BASE<TEmfPlusRect>(unShFlags);
                else
                        Read_EMFPLUS_DRAWIMAGE_BASE<TEmfPlusRectF>(unShFlags);
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_DRAWIMAGE_BASE(unsigned short unShFlags)
        {
                short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
                unsigned int unImageAttributesID;
                int nSrcUnit;
                TEmfPlusRectF oSrcRect;

                m_oStream >> unImageAttributesID;
                m_oStream >> nSrcUnit;
                m_oStream >> oSrcRect;

                T oRectData;

                m_oStream >> oRectData;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_DRAWIMAGEPOINTS(unsigned short unShFlags)
        {
                if ((unShFlags >>(11)) & 1 )
                {
                        //Определен флаг P (С игнорируется)
                        Read_EMFPLUS_DRAWIMAGEPOINTS_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
                }
                else if ((unShFlags >>(14)) & 1 )
                {
                        //Не определен флаг P, но определен флаг С
                        Read_EMFPLUS_DRAWIMAGEPOINTS_BASE<TEmfPlusPoint>(unShFlags);  // абсолютное расположение с 16-разрядными целочисленными координатами
                }
                else
                {
                        //Оба флага не определены
                        Read_EMFPLUS_DRAWIMAGEPOINTS_BASE<TEmfPlusPointF>(unShFlags); // абсолютное расположение с 32-разрядными координатами с плавующей запятой
                }
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_DRAWIMAGEPOINTS_BASE(unsigned short unShFlags)
        {
                short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
                unsigned int unImageAttributesID, unCount;
                int nSrcUnit;
                TEmfPlusRectF oSrcRect;

                m_oStream >> unImageAttributesID;
                m_oStream >> nSrcUnit;
                m_oStream >> oSrcRect;
                m_oStream >> unCount;

                if (nSrcUnit != UnitTypePixel && unCount != 3)
                        return;

                std::vector<T> arPoints(unCount);

                for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
                        m_oStream >> arPoints[unIndex];

                DrawImagePoints(shOgjectIndex, unImageAttributesID, oSrcRect, GetConvertedPoints(arPoints));
        }

        void CEmfPlusParser::Read_EMFPLUS_DRAWLINES(unsigned short unShFlags)
        {
                if ((unShFlags >>(11)) & 1 )
                {
                        //Определен флаг P (С игнорируется)
                        Read_EMFPLUS_DRAWLINES_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
                }
                else if ((unShFlags >>(14)) & 1 )
                {
                        //Не определен флаг P, но определен флаг С
                        Read_EMFPLUS_DRAWLINES_BASE<TEmfPlusPoint>(unShFlags);  // абсолютное расположение с 16-разрядными целочисленными координатами
                }
                else
                {
                        //Оба флага не определены
                        Read_EMFPLUS_DRAWLINES_BASE<TEmfPlusPointF>(unShFlags); // абсолютное расположение с 32-разрядными координатами с плавующей запятой
                }
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_DRAWLINES_BASE(unsigned short unShFlags)
        {
                short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
                unsigned int unCount;

                m_oStream >> unCount;

                if (unCount == 0)
                        return;

                std::vector<T> arPoints(unCount);

                for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
                        m_oStream >> arPoints[unIndex];

                m_oPlayer.SelectObject(shOgjectIndex);

                DrawLines(GetConvertedPoints(arPoints), ((unShFlags >>(13)) & 1));
        }

        void CEmfPlusParser::Read_EMFPLUS_DRAWPATH(unsigned short unShFlags)
        {
                short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
                unsigned int unPenId;

                m_oStream >> unPenId;

                CEmfPlusPath *pPath = GetPath(shOgjectIndex);

                if (pPath)
                {
                        m_oPlayer.SelectObject(unPenId);
                        pPath->Draw(m_pInterpretator, true, false);
                }
        }

        void CEmfPlusParser::Read_EMFPLUS_DRAWPIE(unsigned short unShFlags)
        {
                if ((unShFlags >>(14)) & 1 )
                        Read_EMFPLUS_DRAWPIE_BASE<TEmfPlusRect>(unShFlags);
                else
                        Read_EMFPLUS_DRAWPIE_BASE<TEmfPlusRectF>(unShFlags);
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_DRAWPIE_BASE(unsigned short unShFlags)
        {
                short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
                double dStartAngle, dSweepAngle;
                T oRect;

                m_oStream >> dStartAngle;
                m_oStream >> dSweepAngle;
                m_oStream >> oRect;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_DRAWRECTS(unsigned short unShFlags)
        {
                if ((unShFlags >>(14)) & 1 )
                        Read_EMFPLUS_DRAWRECTS_BASE<TEmfPlusRect>(unShFlags);
                else
                        Read_EMFPLUS_DRAWRECTS_BASE<TEmfPlusRectF>(unShFlags);
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_DRAWRECTS_BASE(unsigned short unShFlags)
        {
                short shPenIndex = ExpressValue(unShFlags, 0, 7);
                unsigned int unCount;

                m_oStream >> unCount;

                if (unCount == 0)
                        return;

                m_oPlayer.SelectObject(shPenIndex);

                std::vector<T> arRects(unCount);

                for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
                {
                        m_oStream >> arRects[unIndex];
                        DrawRectangle(arRects[unIndex], true, false);
                }
        }

        void CEmfPlusParser::Read_EMFPLUS_DRAWSTRING(unsigned short unShFlags)
        {
                short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
                unsigned int unBrushId, unFormatID, unLength;
                TEmfPlusRectF oRect;

                m_oStream >> unBrushId;
                m_oStream >> unFormatID;
                m_oStream >> unLength;
                m_oStream >> oRect;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_FILLCLOSEDCURVE(unsigned short unShFlags)
        {
                if ((unShFlags >>(11)) & 1 )
                {
                        //Определен флаг P (С игнорируется)
                        Read_EMFPLUS_FILLCLOSEDCURVE_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
                }
                else if ((unShFlags >>(14)) & 1 )
                {
                        //Не определен флаг P, но определен флаг С
                        Read_EMFPLUS_FILLCLOSEDCURVE_BASE<TEmfPlusPoint>(unShFlags);  // абсолютное расположение с 16-разрядными целочисленными координатами
                }
                else
                {
                        //Оба флага не определены
                        Read_EMFPLUS_FILLCLOSEDCURVE_BASE<TEmfPlusPointF>(unShFlags); // абсолютное расположение с 32-разрядными координатами с плавующей запятой
                }
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_FILLCLOSEDCURVE_BASE(unsigned short unShFlags)
        {
                unsigned int unBrushId, unCount;
                double dTension;

                m_oStream >> unBrushId;
                m_oStream >> dTension;
                m_oStream >> unCount;

                std::vector<T> arPoints(unCount);

                for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
                {

                }

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_FILLELLIPSE(unsigned short unShFlags)
        {
                if ((unShFlags >>(14)) & 1 )
                        Read_EMFPLUS_FILLELLIPSE_BASE<TEmfPlusRect>(unShFlags);
                else
                        Read_EMFPLUS_FILLELLIPSE_BASE<TEmfPlusRectF>(unShFlags);
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_FILLELLIPSE_BASE(unsigned short unShFlags)
        {
                unsigned int unBrushId;
                T oRect;

                m_oStream >> unBrushId;
                m_oStream >> oRect;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_FILLPATH(unsigned short unShFlags)
        {
                int nFillPathLength;

                m_oStream >> nFillPathLength;

                CEmfPlusPath* pPath = ReadPath();

                if (NULL != pPath)
                {
                        pPath->Draw(m_pInterpretator, true, true);
                        RELEASEOBJECT(pPath);
                }
        }

        void CEmfPlusParser::Read_EMFPLUS_FILLPIE(unsigned short unShFlags)
        {
                if ((unShFlags >>(14)) & 1 )
                        Read_EMFPLUS_FILLPIE_BASE<TEmfPlusRect>(unShFlags);
                else
                        Read_EMFPLUS_FILLPIE_BASE<TEmfPlusRectF>(unShFlags);
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_FILLPIE_BASE(unsigned short unShFlags)
        {
                unsigned int unBrushId;
                double dStartAngle, dSweepAngle;
                T oRect;

                m_oStream >> unBrushId;
                m_oStream >> dStartAngle;
                m_oStream >> dSweepAngle;
                m_oStream >> oRect;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_FILLPOLYGON(unsigned short unShFlags)
        {
                if ((unShFlags >>(11)) & 1 )
                {
                        //Определен флаг P (С игнорируется)
                        Read_EMFPLUS_FILLPOLYGON_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
                }
                else if ((unShFlags >>(14)) & 1 )
                {
                        //Не определен флаг P, но определен флаг С
                        Read_EMFPLUS_FILLPOLYGON_BASE<TEmfPlusPoint>(unShFlags);  // абсолютное расположение с 16-разрядными целочисленными координатами
                }
                else
                {
                        //Оба флага не определены
                        Read_EMFPLUS_FILLPOLYGON_BASE<TEmfPlusPointF>(unShFlags); // абсолютное расположение с 32-разрядными координатами с плавующей запятой
                }
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_FILLPOLYGON_BASE(unsigned short unShFlags)
        {
                unsigned int unBrushId, unCount;

                m_oStream >> unBrushId;
                m_oStream >> unCount;

                std::vector<T> arPoints(unCount);

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_FILLRECTS(unsigned short unShFlags)
        {
                if ((unShFlags >>(14)) & 1 )
                        Read_EMFPLUS_FILLRECTS_BASE<TEmfPlusRect>(unShFlags);
                else
                        Read_EMFPLUS_FILLRECTS_BASE<TEmfPlusRectF>(unShFlags);
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_FILLRECTS_BASE(unsigned short unShFlags)
        {
                unsigned int unBrushId, unCount;

                m_oStream >> unBrushId;
                m_oStream >> unCount;

                if (unCount == 0)
                        return;

                std::vector<T> arRects(unCount);

                for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
                        m_oStream >> arRects[unIndex];

                if ((unShFlags >>(15)) & 1 )//BrushId = Color
                {
                        unsigned int unBlue  = (unBrushId >> 0)  & 0xFF;
                        unsigned int unGreen = (unBrushId >> 8)  & 0xFF;
                        unsigned int unRed   = (unBrushId >> 16) & 0xFF;

                        CEmfLogBrushEx oBrush;
                        oBrush.Color.Set(unRed, unGreen, unBlue);

                        m_pDC->SetBrush(&oBrush);

                        for (T &oBox : arRects)
                                DrawRectangle(oBox, false, true);

                        m_pDC->RemoveBrush(&oBrush);
                }
                else //BrushId = Brush id
                {
                        m_oPlayer.SelectObject(unBrushId);
                        for (T &oBox : arRects)
                                DrawRectangle(oBox, false, true);
                }
        }

        void CEmfPlusParser::Read_EMFPLUS_FILLREGION(unsigned short unShFlags)
        {
                short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
                unsigned int unBrushId;

                m_oStream >> unBrushId;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_OBJECT(unsigned short unShFlags)
        {
                short shObjectIndex = ExpressValue(unShFlags, 0, 7);
                short shObjectType  = ExpressValue(unShFlags, 8, 14);

                if ((unShFlags >>(15)) & 1)
                {
                        unsigned int unTotalSize;
                        m_oStream >> unTotalSize;

                        if (InitContineudObject())
                                m_pContineudObject->SetDataSize(unTotalSize);
                }

                switch (shObjectType)
                {
                        case ObjectTypeInvalid: return;
                        case ObjectTypeBrush:
                        {
                                LOGGING(L"Object Brush with index: " << shObjectIndex)
                                CEmfPlusBrush *pEmfPlusBrush = new CEmfPlusBrush;
                                m_oStream >> *pEmfPlusBrush;

                                m_oPlayer.RegisterObject(shObjectIndex, (CEmfObjectBase*)pEmfPlusBrush);

                                break;
                        }
                        case ObjectTypePen:
                        {
                                LOGGING(L"Object Pen with index: " << shObjectIndex)
                                CEmfPlusPen *pEmfPlusPen = new CEmfPlusPen;
                                m_oStream >> *pEmfPlusPen;

                                m_oPlayer.RegisterObject(shObjectIndex, (CEmfObjectBase*)pEmfPlusPen);

                                break;
                        }
                        case ObjectTypePath:
                        {
                                LOGGING(L"Object Path")
                                CEmfPlusPath* pPath = ReadPath();

                                if (NULL != pPath)
                                        m_mObjects[shObjectIndex] = pPath;

                                break;
                        }
                        case ObjectTypeRegion:
                        {
                                LOGGING(L"Object Region")
                                CEmfPlusRegion *pEmfPlusRegion = ReadRegion();

                                if (NULL != pEmfPlusRegion)
                                        m_mObjects[shObjectIndex] = pEmfPlusRegion;

                                break;
                        }
                        case ObjectTypeImage:
                        {
                                LOGGING(L"Object Image")
                                ReadImage(shObjectIndex, ((unShFlags >>(15)) & 1));
                                break;
                        }
                        case ObjectTypeFont:
                        {
                                LOGGING(L"Object Font")
                                break;
                        }
                        case ObjectTypeStringFormat:
                        {
                                LOGGING(L"Object String Format")
                                break;
                        }
                        case ObjectTypeImageAttributes:
                        {
                                LOGGING(L"Object Image Attributes")

                                CEmfPlusImageAttributes *pImageAttributes = new CEmfPlusImageAttributes();

                                if (NULL != pImageAttributes)
                                {
                                        m_oStream >> *pImageAttributes;
                                        m_mObjects[shObjectIndex] = pImageAttributes;
                                }

                                break;
                        }
                        case ObjectTypeCustomLineCap:
                        {
                                LOGGING(L"Object Custom Line Cap")
                                break;
                        }
                        default: return;
                }
                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_SERIALIZABLEOBJECT(unsigned short unShFlags)
        {
                TGUID oGUID;
                unsigned int unBufferSize;

                m_oStream >> oGUID;
                m_oStream >> unBufferSize;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_SETANTIALIASMODE(unsigned short unShFlags)
        {
                short shSmoothingMode = ExpressValue(unShFlags, 1,  7);

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_SETCOMPOSITINGMODE(unsigned short unShFlags)
        {
                short shCompositingMode = ExpressValue(unShFlags, 0, 7);

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_SETCOMPOSITINGQUALITY(unsigned short unShFlags)
        {
                short shCompositingQuality = ExpressValue(unShFlags, 0, 7);

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_SETINTERPOLATIONMODE(unsigned short unShFlags)
        {
                short shInterpolationMode = ExpressValue(unShFlags, 0, 7);
                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_SETPIXELOFFSETMODE(unsigned short unShFlags)
        {
                short shPixelOffsetMode = ExpressValue(unShFlags, 0, 7);

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_SETRENDERINGORIGIN()
        {
                int nX, nY;

                m_oStream >> nX;
                m_oStream >> nY;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_SETTEXTCONTRAST(unsigned short unShFlags)
        {
                short shTextContrast  = ExpressValue(unShFlags, 0, 11);

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMRPLUS_SETTEXTRENDERINGHINT(unsigned short unShFlags)
        {
                short shTextRenderingHint = ExpressValue(unShFlags, 0, 7);
                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_BEGINCONTAINER(unsigned short unShFlags)
        {
                short shPageUnit = ExpressValue(unShFlags, 8, 15);
                TEmfPlusRectF oDestRect, oSrcRect;
                unsigned int unStackIndex;

                m_oStream >> oDestRect;
                m_oStream >> oSrcRect;
                m_oStream >> unStackIndex;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_BEGINCONTAINERNOPARAMS()
        {
                unsigned int unStackIndex;

                m_oStream >> unStackIndex;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_ENDCONTAINER()
        {
                unsigned int unStackIndex;

                m_oStream >> unStackIndex;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_RESTORE()
        {
                unsigned int unStackIndex;

                m_oStream >> unStackIndex;

//                m_pDC = m_oPlayer.SaveDC();

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_SAVE()
        {
                unsigned int unStackIndex;

                m_oStream >> unStackIndex;

//                m_pDC = m_oPlayer.RestoreDC();

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_SETTSCLIP(unsigned short unShFlags)
        {
                if ((unShFlags >>(15)) & 1 )
                        Read_EMFPLUS_SETTSCLIP_BASE<TEmfPlusRectR>(unShFlags);
                else
                        Read_EMFPLUS_SETTSCLIP_BASE<TEmfPlusRect>(unShFlags);
        }

        template<typename T>
        void CEmfPlusParser::Read_EMFPLUS_SETTSCLIP_BASE(unsigned short unShFlags)
        {
                short shNumRects = ExpressValue(unShFlags, 0, 14);

                std::vector<T> arRects(shNumRects);

                for (unsigned int unIndex = 0; unIndex < shNumRects; ++unIndex)
                {
                        T oRect;
                        m_oStream >> arRects[unIndex];
                }

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_SETTSGRAPHICS(unsigned short unShFlags)
        {
                unsigned char unChAntiAliasMode, unChTextRenderHint, unChCompositingMode, unChCompositingQuality, unChFilterType, unChPixelOffset;
                short shRenderOriginX, shRenderOriginY;
                unsigned short unShTextContrast;
                TEmfPlusXForm oMatrix;

                m_oStream >> unChAntiAliasMode;
                m_oStream >> unChTextRenderHint;
                m_oStream >> unChCompositingMode;
                m_oStream >> unChCompositingQuality;
                m_oStream >> shRenderOriginX;
                m_oStream >> shRenderOriginY;
                m_oStream >> unShTextContrast;
                m_oStream >> unChFilterType;
                m_oStream >> unChPixelOffset;
                m_oStream >> oMatrix;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_MULTIPLYWORLDTRANSFORM(unsigned short unShFlags)
        {
                TEmfPlusXForm oMatrix;

                m_oStream >> oMatrix;

                //TODO: проверить правильность действий
                m_pDC->MultiplyTransform(oMatrix, (((unShFlags >>(14)) & 1 )) ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
                UpdateOutputDC();
        }

        void CEmfPlusParser::Read_EMFPLUS_RESETWORLDTRANSFORM()
        {
                m_pDC->ResetTransform();
                UpdateOutputDC();
        }

        void CEmfPlusParser::Read_EMFPLUS_ROTATEWORLDTRANSFORM(unsigned short unShFlags)
        {
                double dAngle;

                m_oStream >> dAngle;

                TEmfPlusXForm oMatrix(sinf(dAngle), cosf(dAngle), cosf(dAngle), -sinf(dAngle), 0, 0);
                m_pDC->MultiplyTransform(oMatrix, MWT_RIGHTMULTIPLY);

                UpdateOutputDC();
        }

        void CEmfPlusParser::Read_EMFPLUS_SCALEWORLDTRANSFORM(unsigned short unShFlags)
        {
                double dSx, dSy;

                m_oStream >> dSx;
                m_oStream >> dSy;

                TEmfPlusXForm oMatrix(dSx, 0, 0, dSy, 0, 0);
                m_pDC->MultiplyTransform(oMatrix, MWT_RIGHTMULTIPLY);

                UpdateOutputDC();
        }

        void CEmfPlusParser::Read_EMFPLUS_SETPAGETRANSFORM(unsigned short unShFlags)
        {
                short shPageUnit = ExpressValue(unShFlags, 0, 7);
                double dPageScale;

                m_oStream >> dPageScale;

                TXForm oMatrix(dPageScale, 0, 0, dPageScale, 0, 0);

                m_pDC->MultiplyTransform(oMatrix, MWT_RIGHTMULTIPLY);
        }

        void CEmfPlusParser::Read_EMFPLUS_SETWORLDTRANSFORM()
        {
                TEmfPlusXForm oMatrix;

                m_oStream >> oMatrix;

                m_pDC->MultiplyTransform(oMatrix, 4);
                UpdateOutputDC();
        }

        void CEmfPlusParser::Read_EMFPLUS_TRANSLATEWORLDTRANSFORM(unsigned short unShFlags)
        {
                double dX, dY;

                m_oStream >> dX;
                m_oStream >> dY;

                TEmfPlusXForm oMatrix(1, 0, 0, 1, dX, dY);
                m_pDC->MultiplyTransform(oMatrix, MWT_RIGHTMULTIPLY);
                UpdateOutputDC();
        }

        bool CEmfPlusParser::InitContineudObject()
        {
                if (NULL != m_pContineudObject)
                        return false;

                m_pContineudObject = new CEmfPlusContineudObjectRecord();

                return true;
        }

        void CEmfPlusParser::Read_EMFPLUS_ENDOFFILE()
        {
                if(NULL != m_pInterpretator)
                        m_pInterpretator->End();
                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_GETDC()
        {
                m_bBanEmfProcessing = false;
        }

        void CEmfPlusParser::Read_EMRPLUS_OFFSETCLIP()
        {
                m_bBanEmfProcessing = true;

                double dX, dY;

                m_oStream >> dX;
                m_oStream >> dY;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMRPLUS_RESETCLIP()
        {
                m_bBanEmfProcessing = true;

//                m_pDC->GetClip()->Reset();
        }

        void CEmfPlusParser::Read_EMFPLUS_SETCLIPPATH(unsigned short unShFlags)
        {
                m_bBanEmfProcessing = true;

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_SETCLIPRECT(unsigned short unShFlags)
        {
                m_bBanEmfProcessing = true;

                short shCM = ExpressValue(unShFlags, 8, 11);
                TEmfPlusRectF oRect;

                m_oStream >> oRect;

                return; // TODO: при добавлении поддержки регионов становится хуже
                CombineClip(oRect.GetRectD(), shCM);
        }

        void CEmfPlusParser::Read_EMFPLUS_SETCLIPREGION(unsigned short unShFlags)
        {
                m_bBanEmfProcessing = true;

                short shObjectIndex = ExpressValue(unShFlags, 0, 7);
                short shCM = ExpressValue(unShFlags, 8, 11);

                return; // TODO: при добавлении поддержки регионов становится хуже
                CEmfPlusRegion *pRegion = GetRegion(shObjectIndex);

                if (NULL != pRegion)
                {
                        for (CEmfPlusRegionNode oNode : pRegion->arNodes)
                        {
                                if (oNode.eType == RegionNodeDataTypeInfinite)
                                {
                                        TRect* pRect = GetDCBounds();
                                        TRectD oBB;
                                        TranslatePoint(pRect->nLeft, pRect->nTop, oBB.dLeft, oBB.dTop);
                                        TranslatePoint(pRect->nRight, pRect->nBottom, oBB.dRight, oBB.dBottom);
                                        CombineClip(oBB, CombineModeIntersect);
                                }
                                else if (oNode.eType == RegionNodeDataTypeRect)
                                        CombineClip((*oNode.GetRect()).GetRectD(), shCM);
                        }
                }

                //TODO: реализовать
        }

        void CEmfPlusParser::Read_EMFPLUS_COMMENT()
        {
                m_oStream.Skip(m_ulRecordSize);
        }

        template<typename T>
        short CEmfPlusParser::ExpressValue(T Flags, unsigned int unStartIndex, unsigned int unEndIndex) const
        {
                if (unStartIndex >= unEndIndex ||
                    unStartIndex > (sizeof (T) * 8 - 2) ||
                    unEndIndex > (sizeof (T) * 8 - 1) ||
                    unEndIndex - unStartIndex > 16)
                        return 0;

                short shValue = 0;
                unsigned int unValueIndex = 0;

                for (unsigned int unIndex = unStartIndex; unIndex <= unEndIndex; ++unIndex, ++unValueIndex)
                        shValue |= (((Flags >> unIndex) & 1) << unValueIndex);

                return shValue;
        }

        void CEmfPlusParser::DrawLines(std::vector<TEmfPlusPointF> arPoints, bool bCloseFigure)
        {
                MoveTo(arPoints[0].X, arPoints[0].Y);

                for (unsigned int unIndex = 1; unIndex < arPoints.size(); ++unIndex)
                        LineTo(arPoints[unIndex].X, arPoints[unIndex].Y);

                if (bCloseFigure)
                        ClosePath();

                DrawPath(true, false);
        }

        template<typename T>
        std::vector<TEmfPlusPointF> CEmfPlusParser::GetConvertedPoints(std::vector<T> arPoints)
        {
                std::vector<TEmfPlusPointF> arConvertdPoints;

                const std::string sStructName = typeid (T).name();

                if (sStructName == "struct MetaFile::TEmfPlusPointR")
                {
                        //TODO: реализовать
                }
                else if (sStructName == "struct MetaFile::TEmfPlusPoint")
                {
                        //TODO: реализовать
                }
                else if (sStructName == "struct MetaFile::TEmfPlusPointF")
                {
                        arConvertdPoints.resize(arPoints.size());

                        for (unsigned int unIndex = 0; unIndex < arPoints.size(); ++unIndex)
                                arConvertdPoints[unIndex] = static_cast<TEmfPlusPointF>(arPoints[unIndex]);

                        return arConvertdPoints;
                }

                return arConvertdPoints;
        }

}
