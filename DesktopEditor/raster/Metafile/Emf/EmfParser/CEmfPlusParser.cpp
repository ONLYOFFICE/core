#include "CEmfPlusParser.h"
#include "../../../../graphics/pro/Graphics.h"
#include <typeinfo>
#include <math.h>

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
#include "../../Wmf/WmfFile.h"
#include "../../Wmf/WmfInterpretator/CWmfInterpretatorSvg.h"

#include "../EmfInterpretator/CEmfInterpretator.h"
#include "../EmfInterpretator/CEmfInterpretatorSvg.h"
#include "../EmfInterpretator/CEmfInterpretatorArray.h"
#include "../EmfInterpretator/CEmfInterpretatorRender.h"

#ifdef METAFILE_SUPPORT_WMF_EMF_XML
#include "../EmfInterpretator/CEmfInterpretatorXml.h"
#endif

#include "../../Common/CPathConverter.h"

#include "../../../BgraFrame.h"

#define MAX_PICTURE_SIZE 2000.

namespace MetaFile
{	static std::map<unsigned short, std::wstring> ActionNamesEmfPlus =
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

	CEmfPlusParser::CEmfPlusParser(CEmfInterpretatorBase *pEmfInterpretator, const TEmfHeader& oHeader)
		: m_bBanEmfProcessing(false),
		  m_unLogicalDpiX(96),
		  m_unLogicalDpiY(96),
		  m_dUnitKoef(1)
	{
		m_oHeader = oHeader;

		if (NULL != pEmfInterpretator)
		{
			m_pInterpretator = pEmfInterpretator;

			m_pInterpretator->CreateConditional(this);
		}
	}

	CEmfPlusParser::~CEmfPlusParser()
	{
		ClearFile();

		for (EmfPlusObjects::const_iterator pIter = m_mObjects.begin(); pIter != m_mObjects.end(); ++pIter)
			delete pIter->second;
	}

	bool CEmfPlusParser::ReadFromBuffer(BYTE *pBuffer, unsigned int unSize, const bool& bIsExternal)
	{
		return false;
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

	void CEmfPlusParser::RegisterObject(CEmfPlusObject *pObject, unsigned int unIndex)
	{
		if (NULL == pObject) return;

		EmfPlusObjects::const_iterator oFoundObject = m_mObjects.find(unIndex);

		if (m_mObjects.end() != oFoundObject)
		{
			CEmfPlusObject* pOldObject = oFoundObject->second;
			delete pOldObject;
			m_mObjects.erase(unIndex);
		}

		m_mObjects.insert(std::pair<unsigned int, CEmfPlusObject*>(unIndex, pObject));
	}

	void CEmfPlusParser::PlayFile()
	{
		unsigned short unShType, unShFlags;
		unsigned int unSize;

		do
		{
			if (m_bEof)
				break;

			if (m_oStream.IsEof())
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
				case EMFPLUS_ENDOFFILE: Read_EMFPLUS_ENDOFFILE();       break;
				case EMFPLUS_GETDC:     Read_EMFPLUS_GETDC();           break;
				case EMFPLUS_HEADER:    Read_EMFPLUS_HEADER(unShFlags); break;

					//Drawing Record Types (Типы записей чертежа)
				case EMFPLUS_CLEAR:             Read_EMFPLUS_CLEAR();                           break;
				case EMFPLUS_DRAWARC:           Read_EMFPLUS_DRAWARC(unShFlags);                break;
				case EMFPLUS_DRAWBEZIERS:       Read_EMFPLUS_DRAWBEZIERS(unShFlags);            break;
				case EMFPLUS_DRAWCLOSEDCURVE:   Read_EMFPLUS_DRAWCLOSEDCURVE(unShFlags);        break;
				case EMFPLUS_DRAWCURVE:         Read_EMFPLUS_DRAWCURVE(unShFlags);              break;
				case EMFPLUS_DRAWDRIVERSTRING:  Read_EMFPLUS_DRAWDRIVERSTRING(unShFlags);       break;
				case EMFPLUS_DRAWELLIPSE:       Read_EMFPLUS_DRAWELLIPSE(unShFlags);            break;
				case EMFPLUS_DRAWIMAGE:         Read_EMFPLUS_DRAWIMAGE(unShFlags);              break;
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
		}while(m_oStream.CanRead() >= 12 && !m_bEof);

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

	double CEmfPlusParser::GetDpi()
	{
		return (double)m_unLogicalDpiX;
	}

	EmfParserType CEmfPlusParser::GetType()
	{
		return EmfParserType::EmfPlusParser;
	}

	bool CEmfPlusParser::ReadImage(unsigned int offBmi, unsigned int cbBmi, unsigned int offBits, unsigned int cbBits, unsigned int ulSkip, BYTE **ppBgraBuffer, unsigned int *pulWidth, unsigned int *pulHeight)
	{
		return false;
	}

	void CEmfPlusParser::ReadImage(unsigned short shObjectIndex, bool bIsContinuedObject)
	{
		if (!bIsContinuedObject)
		{
			CEmfPlusImage *pImage = new CEmfPlusImage;

			ReadImage(*pImage);

			RegisterObject(pImage, shObjectIndex);

			return;
		}

		CEmfPlusImage *pImage = NULL;

		EmfPlusObjects::const_iterator oFoundElement = m_mObjects.find(shObjectIndex);

		unsigned int unNeedRead = m_ulRecordSize;

		if(m_mObjects.end() == oFoundElement)
		{
			pImage = new CEmfPlusImage;

			unsigned int unTotalSize;

			m_oStream >> unTotalSize;

			pImage->SetSize(unTotalSize - 16);

			ReadImage(*pImage, false);

			RegisterObject(pImage, shObjectIndex);

			if (ImageDataTypeMetafile == pImage->GetImageDataType())
				unNeedRead -= 20;
			else if (ImageDataTypeBitmap == pImage->GetImageDataType())
				unNeedRead -= 32;
		}
		else
		{
			m_oStream.Skip(4);

			unNeedRead -= 4;

			if (ObjectTypeImage != oFoundElement->second->GetObjectType())
				return;

			pImage = (CEmfPlusImage*)oFoundElement->second;
		}


		BYTE* pBuffer = new BYTE[unNeedRead];

		m_oStream.ReadBytes(pBuffer, unNeedRead);

		pImage->AddData(pBuffer, unNeedRead);

		delete[] pBuffer;
	}

	void CEmfPlusParser::ReadImage(CEmfPlusImage &oImage, bool bReadData)
	{
		m_oStream.Skip(4); //version
		unsigned int unType;

		m_oStream >> unType;

		oImage.SetImageDataType(unType);

		if (unType == ImageDataTypeBitmap)
			ReadBitmap(oImage, bReadData);
		else if (unType == ImageDataTypeMetafile)
			ReadMetaFile(oImage, bReadData);
	}

	void CEmfPlusParser::ReadMetaFile(CEmfPlusImage& oImage, bool bReadData)
	{
		unsigned int unType;

		m_oStream >> unType;

		if (!oImage.SetMetafileType(unType))
			return;

		unsigned int unMetafileSize;

		m_oStream >> unMetafileSize;

		if (unMetafileSize < m_ulRecordSize - 16)
			unMetafileSize = m_ulRecordSize - 16;

		oImage.SetSize(unMetafileSize);

		if (!bReadData) return;

		BYTE* pBuffer = new BYTE[unMetafileSize];

		if (NULL == pBuffer)
			return;

		m_oStream.ReadBytes(pBuffer, unMetafileSize);

		oImage.AddData(pBuffer, unMetafileSize);

		delete[] pBuffer;
	}

	void CEmfPlusParser::ReadBitmap(CEmfPlusImage &oImage, bool bReadData)
	{
		unsigned int unType;
		int nWidth, nHeight, nStride;

		m_oStream >> nWidth;
		m_oStream >> nHeight;
		m_oStream >> nStride;

		m_oStream.Skip(4); //PixelFormat

		m_oStream >> unType;

		if (!bReadData) return;

		BYTE* pBuffer = new BYTE[m_ulRecordSize - 28];

		if (NULL == pBuffer)
			return;

		m_oStream.ReadBytes(pBuffer, m_ulRecordSize - 28);

		if (BitmapDataTypePixel == unType)
			oImage.SetImageSize(nWidth, nHeight);

		oImage.AddData(pBuffer, m_ulRecordSize - 28);

		delete[] pBuffer;
	}

	CEmfPlusImage* CEmfPlusParser::GetImage(unsigned int unImageIndex)
	{
		EmfPlusObjects::const_iterator oFoundElement = m_mObjects.find(unImageIndex);

		if (m_mObjects.end() != oFoundElement && oFoundElement->second->GetObjectType() == ObjectTypeImage)
			return (CEmfPlusImage*)oFoundElement->second;

		return NULL;
	}

	CEmfPlusBrush *CEmfPlusParser::ReadBrush()
	{
		unsigned int unType;

		m_oStream.Skip(4); //Version

		m_oStream >> unType;

		CEmfPlusBrush *pEmfPlusBrush = new CEmfPlusBrush();

		switch (unType)
		{
		case BrushTypeSolidColor:
		{
			pEmfPlusBrush->unStyle = BS_SOLID;
			m_oStream >> pEmfPlusBrush->oColor;
			break;
		}
		case BrushTypeHatchFill:
		{
			pEmfPlusBrush->unStyle = BS_HATCHED;
			m_oStream >> pEmfPlusBrush->unHatch;
			m_oStream >> pEmfPlusBrush->oColor;
			m_oStream >> pEmfPlusBrush->oColorBack;

			break;
		}
		case BrushTypeTextureFill:
		{
			// TODO: так как на данный момент нельзя регулировать повторение заливки, то будет отрисовывать изображение при самой заливки
			pEmfPlusBrush->unStyle = BS_PATTERN;

			unsigned int unBrushDataFlags, unSkip = 16;

			m_oStream >> unBrushDataFlags;
			m_oStream.Skip(4); // WrapMode

			if (unBrushDataFlags & BrushDataTransform)
			{
				m_oStream.Skip(24); // TransformMatrix
				unSkip += 24;
			}

			if (m_ulRecordSize <= 28)
				break;

			m_ulRecordSize -= unSkip;

			CEmfPlusImage oImage;

			ReadImage(oImage);

			std::wstring wsImagePath;

			if (SaveImage(oImage, wsImagePath))
				pEmfPlusBrush->wsDibPatternPath = wsImagePath;

			m_ulRecordSize += unSkip;
			break;
		}
		case BrushTypePathGradient:
		{
//			pEmfPlusBrush->Style = BS_PATHGRADIENT;
			//TODO: пока что данная кисть будет радиальной, но в дальнейшем необходимо реализовать все возможности данной кисти
			pEmfPlusBrush->unStyle = BS_RADIALGRADIENT;

			unsigned int unBrushDataFlags;

			m_oStream >> unBrushDataFlags;

			m_oStream.Skip(4); //WrapMode

			m_oStream >> pEmfPlusBrush->oColor;
			m_oStream >> pEmfPlusBrush->oCenterPoint;

			unsigned int unCountColors;

			m_oStream >> unCountColors;

			if (unCountColors > 0)
				m_oStream >> pEmfPlusBrush->oColorBack;

			m_oStream.Skip((unCountColors - 1) * 4);

			if (BrushDataPath & unBrushDataFlags)
			{
				int nSize;

				m_oStream >> nSize;

				if (nSize > 0)
				{
					CEmfPlusPath* pPath = ReadPath();

					if (NULL != pPath)
					{
						TRectD oRect = pPath->GetBounds();

						pEmfPlusBrush->oRectF.dX      = oRect.Left;
						pEmfPlusBrush->oRectF.dY      = oRect.Top;
						pEmfPlusBrush->oRectF.dWidth  = oRect.Right - oRect.Left;
						pEmfPlusBrush->oRectF.dHeight = oRect.Bottom - oRect.Top;

						delete pPath;
					}
				}
			}
			else
			{
				int nCountPoint;

				m_oStream >> nCountPoint;

				std::vector<TEmfPlusPointF> arPoints = ReadPoints<TEmfPlusPointF>(nCountPoint);

				//TODO::реализовать при встрече
			}

			if (BrushDataPresetColors & unBrushDataFlags)
			{
				unsigned int unPositionCount;

				m_oStream >> unPositionCount;

				std::vector<double> arBlendPositions(unPositionCount);

				for (unsigned int unIndex = 0; unIndex <  unPositionCount; ++unIndex)
					m_oStream >> arBlendPositions[unIndex];

				std::vector<TEmfPlusARGB> arBlendColors(unPositionCount);

				for (unsigned int unIndex = 0; unIndex <  unPositionCount; ++unIndex)
					m_oStream >> arBlendColors[unIndex];

				if (1 < unPositionCount)
				{
					pEmfPlusBrush->oColorBack = arBlendColors[0];
					pEmfPlusBrush->oColor     = arBlendColors.back();
				}
			}

			break;
		}
		case BrushTypeLinearGradient:
		{
			//TODO: реализовать
			pEmfPlusBrush->unStyle = BS_LINEARGRADIENT;

			m_oStream.Skip(8); // BrushDataFlags, WrapMode

			//				m_oStream >> pEmfPlusBrush->RectF;
			m_oStream.Skip(16);
			m_oStream >> pEmfPlusBrush->oColor;
			m_oStream >> pEmfPlusBrush->oColorBack;

			break;
		}
		default:
		{
			RELEASEOBJECT(pEmfPlusBrush);
			return NULL;
		}
		}

		return pEmfPlusBrush;
	}

	CEmfPlusBrush *CEmfPlusParser::GetBrush(unsigned int unBrushIndex)
	{
		EmfPlusObjects::const_iterator oFoundElement = m_mObjects.find(unBrushIndex);

		if (m_mObjects.end() != oFoundElement && oFoundElement->second->GetObjectType() == ObjectTypeBrush)
			return (CEmfPlusBrush*)oFoundElement->second;

		return NULL;
	}

	CEmfPlusPen *CEmfPlusParser::ReadPen()
	{
		m_oStream.Skip(4); //Version

		unsigned int unType;

		m_oStream >> unType;

		if(unType != 0)
			return NULL;

		CEmfPlusPen *pEmfPlusPen = new CEmfPlusPen;

		unsigned int unFlags, unUnitType;

		m_oStream >> unFlags;
		m_oStream >> unUnitType;
		m_oStream >> pEmfPlusPen->dWidth;

		if (unFlags & PEN_DATA_TRANSFORM)
			m_oStream.Skip(24); // TransformMatrix (24 bytes) - EmfPlusTransformMatrix object
		if (unFlags & PEN_DATA_STARTCAP)
		{
			int nStartCap;

			m_oStream >> nStartCap;

			switch (nStartCap)
			{
				case 0:	pEmfPlusPen->unStyle |= PS_STARTCAP_MASK & PS_STARTCAP_FLAT;   break;
				case 1: pEmfPlusPen->unStyle |= PS_STARTCAP_MASK & PS_STARTCAP_SQUARE; break;
				case 2: pEmfPlusPen->unStyle |= PS_STARTCAP_MASK & PS_STARTCAP_ROUND;  break;
			}
		}
		if (unFlags & PEN_DATA_ENDCAP)
		{
			int nEndCap;

			m_oStream >> nEndCap;

			switch (nEndCap)
			{
				case 0:	pEmfPlusPen->unStyle |= PS_ENDCAP_MASK & PS_ENDCAP_FLAT;   break;
				case 1: pEmfPlusPen->unStyle |= PS_ENDCAP_MASK & PS_ENDCAP_SQUARE; break;
				case 2: pEmfPlusPen->unStyle |= PS_ENDCAP_MASK & PS_ENDCAP_ROUND;  break;
			}
		}
		if (unFlags & PEN_DATA_JOIN)
		{
			int nJoin;

			m_oStream >> nJoin;

			switch (nJoin)
			{
				case 0:	pEmfPlusPen->unStyle |= PS_JOIN_MASK & PS_JOIN_MITER;   break;
				case 1: pEmfPlusPen->unStyle |= PS_JOIN_MASK & PS_JOIN_BEVEL;   break;
				case 2: pEmfPlusPen->unStyle |= PS_JOIN_MASK & PS_ENDCAP_ROUND; break;
			}
		}
		if (unFlags & PEN_DATA_MITERLIMIT)
		{
			m_oStream >> pEmfPlusPen->dMiterLimit;
		}
		if (unFlags & PEN_DATA_LINESTYLE)
		{
			int nLineStyle;

			m_oStream >> nLineStyle;

			pEmfPlusPen->unStyle |= PS_STYLE_MASK & nLineStyle;
		}
		if (unFlags & PEN_DATA_DASHEDLINECAP)
		{
			int nDashedLineCapType;

			m_oStream >> nDashedLineCapType;

			switch (nDashedLineCapType)
			{
			case 0:
			{
				pEmfPlusPen->unStyle |= (PS_STARTCAP_MASK & PS_STARTCAP_FLAT) |
				(PS_ENDCAP_MASK   & PS_STARTCAP_FLAT);
				break;
			}
			case 2:
			{
				pEmfPlusPen->unStyle |= (PS_STARTCAP_MASK & PS_STARTCAP_ROUND) |
				(PS_ENDCAP_MASK   & PS_STARTCAP_ROUND);
				break;
			}
			}
		}
		if (unFlags & PEN_DATA_DASHEDLINEOFFSET)
		{
			m_oStream >> pEmfPlusPen->dDashOffset;
		}
		if (unFlags & PEN_DATA_DASHEDLINE)
		{
			m_oStream >> pEmfPlusPen->unSizeDash;

			pEmfPlusPen->pDataDash = new double[pEmfPlusPen->unSizeDash];

			if (NULL != pEmfPlusPen->pDataDash)
			{
				for (unsigned int unIndex = 0; unIndex < pEmfPlusPen->unSizeDash; ++unIndex)
					m_oStream >> pEmfPlusPen->pDataDash[unIndex];
			}
		}
		if (unFlags & PEN_DATA_NONCENTER)
			m_oStream.Skip(4); // PenAlignment (4 bytes) - signed integer
		if (unFlags & PEN_DATA_COMPOUNDLINE)
		{
			unsigned int unCompoundLineDataSize;

			m_oStream >> unCompoundLineDataSize;

			m_oStream.Skip(4 * unCompoundLineDataSize); // EmfPlusCompoundLineData object
		}
		if (unFlags & PEN_DATA_CUSTOMSTARTCAP)
		{
			unsigned int unCustomStartCapSize;

			m_oStream >> unCustomStartCapSize;

			unsigned int unStart = m_oStream.Tell();

			m_oStream.Skip(4); //Version

			int nType;

			m_oStream >> nType;

			if (CustomLineCapDataTypeDefault == nType)
			{
				TEmfPlusCustomLineCapData *pLineCapData = new TEmfPlusCustomLineCapData;

				m_oStream >> *pLineCapData;

				if (CustomLineCapDataFillPath == pLineCapData->unCustomLineCapDataFlags ||
						CustomLineCapDataLinePath == pLineCapData->unCustomLineCapDataFlags)
				{
					m_oStream.Skip(4); // FillPathLength or LinePathLength
					pLineCapData->pPath = ReadPath();
				}

				pEmfPlusPen->pLineStartCapData = pLineCapData;
			}
			else if (CustomLineCapDataTypeAdjustableArrow == nType)
			{
				TEmfPlusCustomLineCapArrowData *pLineCapData = new TEmfPlusCustomLineCapArrowData;

				m_oStream >> *pLineCapData;

				pEmfPlusPen->pLineStartCapData = pLineCapData;
			}

			m_oStream.Skip(unCustomStartCapSize - (m_oStream.Tell() - unStart));
		}
		if (unFlags & PEN_DATA_CUSTOMENDCAP)
		{
			unsigned int unCustomEndCapSize;

			m_oStream >> unCustomEndCapSize;

			unsigned int unStart = m_oStream.Tell();

			m_oStream.Skip(4); //Version

			int nType;

			m_oStream >> nType;

			if (CustomLineCapDataTypeDefault == nType)
			{
				TEmfPlusCustomLineCapData *pLineCapData = new TEmfPlusCustomLineCapData;

				m_oStream >> *pLineCapData;

				if (CustomLineCapDataFillPath == pLineCapData->unCustomLineCapDataFlags ||
						CustomLineCapDataLinePath == pLineCapData->unCustomLineCapDataFlags)
				{
					m_oStream.Skip(4); // FillPathLength or LinePathLength
					pLineCapData->pPath = ReadPath();
				}

				pEmfPlusPen->pLineEndCapData = pLineCapData;
			}
			else if (CustomLineCapDataTypeAdjustableArrow == nType)
			{
				TEmfPlusCustomLineCapArrowData *pLineCapData = new TEmfPlusCustomLineCapArrowData;

				m_oStream >> *pLineCapData;

				pEmfPlusPen->pLineEndCapData = pLineCapData;
			}

			m_oStream.Skip(unCustomEndCapSize - (m_oStream.Tell() - unStart));
		}

		pEmfPlusPen->pBrush = ReadBrush();

		return pEmfPlusPen;
	}

	CEmfPlusPen *CEmfPlusParser::GetPen(unsigned int unPenIndex)
	{
		EmfPlusObjects::const_iterator oFoundElement = m_mObjects.find(unPenIndex);

		if (m_mObjects.end() != oFoundElement && oFoundElement->second->GetObjectType() == ObjectTypePen)
			return (CEmfPlusPen*)oFoundElement->second;

		return NULL;
	}

	CEmfPlusFont *CEmfPlusParser::ReadFont()
	{
		m_oStream.Skip(4); //Version

		CEmfPlusFont* pFont = new CEmfPlusFont();

		m_oStream >> pFont->m_dEmSize;
		m_oStream >> pFont->m_unSizeUnit;

		int nFontStyleFlags;

		m_oStream >> nFontStyleFlags;

		if (nFontStyleFlags & EEmfPlusFontStyle::FontStyleBold)
			pFont->m_bBold          = true;
		if (nFontStyleFlags & EEmfPlusFontStyle::FontStyleItalic)
			pFont->m_bItalic        = true;
		if (nFontStyleFlags & EEmfPlusFontStyle::FontStyleStrikeout)
			pFont->m_bStrikeout     = true;
		if (nFontStyleFlags & EEmfPlusFontStyle::FontStyleUnderline)
			pFont->m_bUnderline     = true;

		m_oStream.Skip(4); //Reserved

		unsigned int unLength;

		m_oStream >> unLength;

		if (unLength > 15) // 30 байта / 2 (размер ushort) = 15 символов максимальная длина имени Unicode шрифта
			unLength = 15;

		unsigned short* pString = new unsigned short[unLength + 1];
		if (pString)
		{
			pString[unLength] = 0x00;
			m_oStream.ReadBytes(pString, unLength);
		}

		pFont->m_wsFamilyName = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)pString, unLength);

		RELEASEARRAYOBJECTS(pString)

		return pFont;
	}

	CEmfPlusFont *CEmfPlusParser::GetFont(unsigned int unPenIndex)
	{
		EmfPlusObjects::const_iterator oFoundElement = m_mObjects.find(unPenIndex);

		if (m_mObjects.end() != oFoundElement && oFoundElement->second->GetObjectType() == ObjectTypeFont)
			return (CEmfPlusFont*)oFoundElement->second;

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

		CEmfPlusPath *pPath = new CEmfPlusPath;

		if ((unPathPointFlags >>(20)) & 1 )
		{
			//Определен флаг R (С игнорируется)
		}
		else if ((unPathPointFlags >>(14)) & 1 )
		{
			//Не определен флаг R, но определен флаг С
			std::vector<TEmfPlusPoint> arPoints     = ReadPoints<TEmfPlusPoint>(unPathPointCount);
			std::vector<char> arPointTypes          = ReadPointTypes(unPathPointCount);

			pPath->MoveTo(arPoints[0].X, arPoints[0].Y);

			for (unsigned int unIndex = 1; unIndex < unPathPointCount; ++unIndex)
			{
				switch (arPointTypes[unIndex] & 0xf)
				{
				case PathPointTypeStart:  pPath->MoveTo(arPoints[unIndex].X, arPoints[unIndex].Y); break;
				case PathPointTypeLine:   pPath->LineTo(arPoints[unIndex].X, arPoints[unIndex].Y); break;
				case PathPointTypeBezier:
				{
					if (unIndex + 2 >= unPathPointCount) break;

					pPath->CurveTo(arPoints[unIndex + 0].X, arPoints[unIndex + 0].Y,
					               arPoints[unIndex + 1].X, arPoints[unIndex + 1].Y,
					               arPoints[unIndex + 2].X, arPoints[unIndex + 2].Y);
					unIndex += 2;
					break;
				}
				}

				if ((((unsigned int)(arPointTypes[unIndex])) & 0xf0) == 0x80)
					pPath->Close();
			}

			unsigned int unSkip = 4 - (12 + 5 * unPathPointCount) % 4;

			if (unSkip < 4)
				m_oStream.Skip(unSkip);
		}
		else
		{
			std::vector<TEmfPlusPointF> arPoints    = ReadPoints<TEmfPlusPointF>(unPathPointCount);
			std::vector<char> arPointTypes          = ReadPointTypes(unPathPointCount);

			pPath->MoveTo(arPoints[0].X, arPoints[0].Y);

			for (unsigned int unIndex = 1; unIndex < unPathPointCount; ++unIndex)
			{
				switch (arPointTypes[unIndex] & 0xf)
				{
				case PathPointTypeStart:  pPath->MoveTo(arPoints[unIndex].X, arPoints[unIndex].Y); break;
				case PathPointTypeLine:   pPath->LineTo(arPoints[unIndex].X, arPoints[unIndex].Y); break;
				case PathPointTypeBezier:
				{
					if (unIndex + 2 >= unPathPointCount) break;

					pPath->CurveTo(arPoints[unIndex + 0].X, arPoints[unIndex + 0].Y,
					               arPoints[unIndex + 1].X, arPoints[unIndex + 1].Y,
					               arPoints[unIndex + 2].X, arPoints[unIndex + 2].Y);
					unIndex += 2;
					break;
				}
				}

				if ((((unsigned int)(arPointTypes[unIndex])) & 0xf0) == 0x80)
					pPath->Close();
			}

			unsigned int unSkip = 4 - (12 + 9 * unPathPointCount) % 4;

			if (unSkip < 4)
				m_oStream.Skip(unSkip);
			//Оба флага не определены
		}
		//TODO: реализовать

		return pPath;
	}

	void CEmfPlusParser::ReadBufferPath(CEmfPlusBuffer *pDataPath)
	{
		if (NULL == pDataPath)
			return;

		unsigned int unTotalSize;
		m_oStream >> unTotalSize;

		if (0 == pDataPath->GetSize())
			pDataPath->SetSize(unTotalSize - 8);
		else
			m_oStream.Skip(4);

		pDataPath->AddData(m_oStream.GetCurPtr(), m_ulRecordSize - 8);
		m_oStream.Skip(m_ulRecordSize - 8);
	}

	CEmfPlusBuffer *CEmfPlusParser::GetBuffer(unsigned int unPathIndex)
	{
		EmfPlusObjects::const_iterator oFoundElement = m_mObjects.find(unPathIndex);

		if (m_mObjects.end() != oFoundElement && oFoundElement->second->GetObjectType() == ObjectTypeBuffer)
			return (CEmfPlusBuffer*)oFoundElement->second;

		return NULL;
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
		CEmfPlusRegion *pRegion = new CEmfPlusRegion();

		unsigned int unVersion, unRegionCount;

		m_oStream >> unVersion;
		m_oStream >> unRegionCount;

		for (unsigned int unIndex = 0; unIndex < unRegionCount + 1; )
		{
			CEmfPlusRegionNode *pNode = ReadRegionNode(unIndex);

			if (NULL != pNode)
				pRegion->arNodes.push_back(pNode);
		}

		return pRegion;
	}

	CEmfPlusRegionNode *CEmfPlusParser::ReadRegionNode(unsigned int &unIndex)
	{
		unsigned int unType;

		m_oStream >> unType;

		CEmfPlusRegionNode *pNode = NULL;

		switch (unType)
		{
			case RegionNodeDataTypeAnd:
			{
				pNode = ReadRegionNodeChild(unIndex);
				pNode->eType = RegionNodeDataTypeAnd;
				break;
			}
			case RegionNodeDataTypeOr:
			{
				pNode = ReadRegionNodeChild(unIndex);
				pNode->eType = RegionNodeDataTypeOr;
				break;
			}
			case RegionNodeDataTypeXor:
			{
				pNode = ReadRegionNodeChild(unIndex);
				pNode->eType = RegionNodeDataTypeXor;
				break;
			}
			case RegionNodeDataTypeExclude:
			{
				pNode = ReadRegionNodeChild(unIndex);
				pNode->eType = RegionNodeDataTypeExclude;
				break;
			}
			case RegionNodeDataTypeComplement:
			{
				pNode = ReadRegionNodeChild(unIndex);
				pNode->eType = RegionNodeDataTypeComplement;
				break;
			}
			case RegionNodeDataTypeRect:
			{
				pNode = ReadRegionNodeRectF(unIndex);
				pNode->eType = RegionNodeDataTypeRect;
				break;
			}
			case RegionNodeDataTypePath:
			{
				pNode = ReadRegionNodePath(unIndex);
				pNode->eType = RegionNodeDataTypePath;
				break;
			}
			case RegionNodeDataTypeEmpty:
			{
				pNode = new CEmfPlusRegionNode;
				pNode->eType = RegionNodeDataTypeEmpty;
				++unIndex;
				break;
			}
			case RegionNodeDataTypeInfinite:
			{
				pNode = new CEmfPlusRegionNode;
				pNode->eType = RegionNodeDataTypeInfinite;
				++unIndex;
				break;
			}
		}

		return pNode;
	}

	CEmfPlusRegionNodePath *CEmfPlusParser::ReadRegionNodePath(unsigned int& unIndex)
	{
		CEmfPlusRegionNodePath* pNode = new CEmfPlusRegionNodePath();

		int nRegionNodePathLength;

		m_oStream >> nRegionNodePathLength;

		pNode->pPath = ReadPath();

		unIndex += nRegionNodePathLength;

		return pNode;
	}

	CEmfPlusRegionNodeRectF *CEmfPlusParser::ReadRegionNodeRectF(unsigned int& unIndex)
	{
		CEmfPlusRegionNodeRectF *pNode = new CEmfPlusRegionNodeRectF();

		TEmfPlusRectF *pRect = new TEmfPlusRectF;

		m_oStream >> *pRect;

		pNode->pRect = pRect;

		unIndex += 4;

		return pNode;
	}

	CEmfPlusRegionNodeChild *CEmfPlusParser::ReadRegionNodeChild(unsigned int &unIndex)
	{
		CEmfPlusRegionNodeChild* pNode = new CEmfPlusRegionNodeChild();

		pNode->pLeft  = ReadRegionNode(unIndex);
		pNode->pRigth = ReadRegionNode(unIndex);

		return pNode;
	}

	CEmfPlusRegion* CEmfPlusParser::GetRegion(unsigned int unRegionIndex)
	{
		EmfPlusObjects::const_iterator oFoundElement = m_mObjects.find(unRegionIndex);

		if (m_mObjects.end() != oFoundElement && oFoundElement->second->GetObjectType() == ObjectTypeRegion)
			return (CEmfPlusRegion*)oFoundElement->second;

		return NULL;
	}

	CEmfPlusStringFormat *CEmfPlusParser::GetStringFormat(unsigned int unStringFormatIndex)
	{
		EmfPlusObjects::const_iterator oFoundElement = m_mObjects.find(unStringFormatIndex);

		if (m_mObjects.end() != oFoundElement && oFoundElement->second->GetObjectType() == ObjectTypeStringFormat)
			return (CEmfPlusStringFormat*)oFoundElement->second;

		return NULL;
	}

	CEmfPlusImageAttributes *CEmfPlusParser::GetImageAttributes(unsigned int unImageAttributesIndex)
	{
		EmfPlusObjects::const_iterator oFoundElement = m_mObjects.find(unImageAttributesIndex);

		if (m_mObjects.end() != oFoundElement && oFoundElement->second->GetObjectType() == ObjectTypeImageAttributes)
			return (CEmfPlusImageAttributes*)oFoundElement->second;

		return NULL;
	}

	template<typename PointType>
	std::vector<PointType> CEmfPlusParser::ReadPoints(unsigned int unPointCount)
	{
		std::vector<PointType> arPoints(unPointCount);

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

	void CEmfPlusParser::DrawRectangle(const TEmfPlusRectF& oRectangle, bool bStroke, bool bFill)
	{
		if (NULL != m_pInterpretator)
		{
			if (AD_COUNTERCLOCKWISE != m_pDC->GetArcDirection())
			{
				m_pInterpretator->MoveTo(oRectangle.dX,  oRectangle.dY);
				m_pInterpretator->LineTo(oRectangle.dX,  oRectangle.dY + oRectangle.dHeight);
				m_pInterpretator->LineTo(oRectangle.dX + oRectangle.dWidth, oRectangle.dY + oRectangle.dHeight);
				m_pInterpretator->LineTo(oRectangle.dX + oRectangle.dWidth, oRectangle.dY);
			}
			else
			{
				m_pInterpretator->MoveTo(oRectangle.dX,  oRectangle.dY);
				m_pInterpretator->LineTo(oRectangle.dX + oRectangle.dWidth, oRectangle.dY);
				m_pInterpretator->LineTo(oRectangle.dX + oRectangle.dWidth, oRectangle.dY + oRectangle.dHeight);
				m_pInterpretator->LineTo(oRectangle.dX,  oRectangle.dY + oRectangle.dHeight);
			}

			m_pInterpretator->ClosePath();
			m_pInterpretator->DrawPath((bStroke ? 1 : 0) + (bFill ? 2 : 0));
			m_pInterpretator->EndPath();
		}
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
		CEmfPlusPath oPath;
		oPath.MoveTo(oClip.Left,  oClip.Top);
		oPath.LineTo(oClip.Right, oClip.Top);
		oPath.LineTo(oClip.Right, oClip.Bottom);
		oPath.LineTo(oClip.Left,  oClip.Bottom);
		oPath.Close();

		m_pDC->GetClip()->SetPath(oPath, nMode, *GetTransform());

		UpdateOutputDC();
	}

	void CEmfPlusParser::UpdateMatrix(TEmfPlusXForm &oMatrix)
	{
		oMatrix.M11 *= m_dUnitKoef;
		oMatrix.M12 *= m_dUnitKoef;
		oMatrix.M21 *= m_dUnitKoef;
		oMatrix.M22 *= m_dUnitKoef;
		oMatrix.Dx  *= m_dUnitKoef;
		oMatrix.Dy  *= m_dUnitKoef;
	}

	bool CEmfPlusParser::SaveImage(const CEmfPlusImage &oEmfPlusImage, std::wstring &wsPathToImage)
	{
		if (ImageDataTypeBitmap != oEmfPlusImage.GetImageDataType())
			return false;

		BYTE* pBytes;
		unsigned int unImageSize;

		oEmfPlusImage.GetData(pBytes, unImageSize);

		NSFile::CFileBinary oFile;

		std::wstring wsTempPath = oFile.GetTempPath() + L"/Temp" + std::to_wstring(unImageSize) + L".tmp";

		if (!oFile.CreateFileW(wsTempPath))
			return false;

		if (!oFile.WriteFile(pBytes, unImageSize))
		{
			oFile.CloseFile();
			return false;
		}

		oFile.CloseFile();

		wsPathToImage = wsTempPath;

		return true;
	}

	BYTE* GetClipedImage(const BYTE* pBuffer, LONG lWidth, LONG lHeight, TRectL& oNewRect)
	{
		if (NULL == pBuffer ||
			oNewRect.Left < 0 || oNewRect.Right  < 0 ||
			oNewRect.Top  < 0 || oNewRect.Bottom < 0)
			return NULL;

		if (lHeight < (oNewRect.Bottom - oNewRect.Top))
			oNewRect.Bottom = oNewRect.Top + lHeight;

		if (lWidth < (oNewRect.Right - oNewRect.Left))
			oNewRect.Right = oNewRect.Left + lWidth;

		if (lHeight == (oNewRect.Bottom - oNewRect.Top) &&
			lWidth  == (oNewRect.Right  - oNewRect.Left))
			return NULL;

		int nBeginX, nBeginY, nEndX, nEndY;

		nBeginX = (std::min)(oNewRect.Left, oNewRect.Right);
		nBeginY = (std::min)(oNewRect.Top,  oNewRect.Bottom);

		nEndX   = (std::max)(oNewRect.Left, oNewRect.Right);
		nEndY   = (std::max)(oNewRect.Top,  oNewRect.Bottom);

		int nWidth = nEndX - nBeginX;
		int nHeight = nEndY - nBeginY;

		BYTE* pNewBuffer = new BYTE[nWidth * nHeight * 4];

		ULONG ulPos = 0;

		for (ULONG ulPosY = nBeginY * 4; ulPosY < nEndY * 4; ulPosY += 4)
		{
			for (ULONG ulPosX = nBeginX * 4; ulPosX < nEndX * 4; ulPosX += 4)
			{
				pNewBuffer[ulPos++]   = (BYTE)pBuffer[ulPosY * lWidth + ulPosX + 0];
				pNewBuffer[ulPos++]   = (BYTE)pBuffer[ulPosY * lWidth + ulPosX + 1];
				pNewBuffer[ulPos++]   = (BYTE)pBuffer[ulPosY * lWidth + ulPosX + 2];
				pNewBuffer[ulPos++]   = (BYTE)pBuffer[ulPosY * lWidth + ulPosX + 3];
			}
		}

		return pNewBuffer;
	}

	HRESULT FlipYImage(BYTE* pImageBuffer, LONG lWidth, LONG lHeight)
	{
		if (NULL == pImageBuffer || lWidth == 0 || lHeight == 0)
			return S_FALSE;

		BYTE oBuffer[4] = {0, 0, 0, 0};

		for (ULONG ulPosY = 0; ulPosY < lHeight / 2 * 4; ulPosY += 4)
		{
			for (ULONG ulPosX = 0; ulPosX < lWidth * 4; ulPosX += 4)
			{
				oBuffer[0] = pImageBuffer[ulPosY * lWidth + ulPosX + 0];
				oBuffer[1] = pImageBuffer[ulPosY * lWidth + ulPosX + 1];
				oBuffer[2] = pImageBuffer[ulPosY * lWidth + ulPosX + 2];
				oBuffer[3] = pImageBuffer[ulPosY * lWidth + ulPosX + 3];

				pImageBuffer[ulPosY * lWidth + ulPosX + 0] = pImageBuffer[((lHeight - 1) * 4 - ulPosY) * lWidth + ulPosX + 0];
				pImageBuffer[ulPosY * lWidth + ulPosX + 1] = pImageBuffer[((lHeight - 1) * 4 - ulPosY) * lWidth + ulPosX + 1];
				pImageBuffer[ulPosY * lWidth + ulPosX + 2] = pImageBuffer[((lHeight - 1) * 4 - ulPosY) * lWidth + ulPosX + 2];
				pImageBuffer[ulPosY * lWidth + ulPosX + 3] = pImageBuffer[((lHeight - 1) * 4 - ulPosY) * lWidth + ulPosX + 3];

				pImageBuffer[((lHeight - 1) * 4 - ulPosY) * lWidth + ulPosX + 0] = oBuffer[0];
				pImageBuffer[((lHeight - 1) * 4 - ulPosY) * lWidth + ulPosX + 1] = oBuffer[1];
				pImageBuffer[((lHeight - 1) * 4 - ulPosY) * lWidth + ulPosX + 2] = oBuffer[2];
				pImageBuffer[((lHeight - 1) * 4 - ulPosY) * lWidth + ulPosX + 3] = oBuffer[3];
			}
		}

		return S_OK;
	}

	void CEmfPlusParser::DrawImagePoints(unsigned int unImageIndex, unsigned int unImageAttributeIndex,
	                                     const TEmfPlusRectF& oSrcRect, const std::vector<TEmfPlusPointF>& arPoints)
	{
		if (NULL == m_pInterpretator)
			return;

		CEmfPlusImage *pImage = GetImage(unImageIndex);

		if (NULL == pImage)
			return;

		BYTE* pBuffer;
		unsigned int unSizeBuffer;

		pImage->GetData(pBuffer, unSizeBuffer);

		if (NULL == pBuffer || unSizeBuffer == 0 || arPoints.size() != 3)
			return;

		if (ImageDataTypeBitmap == pImage->GetImageDataType())
		{
			unsigned int unWidth, unHeigth;

			pImage->GetImageSize(unWidth, unHeigth);

			DrawBitmap(pBuffer, unSizeBuffer, unWidth, unHeigth, oSrcRect, arPoints);
		}
		else if (ImageDataTypeMetafile == pImage->GetImageDataType())
		{
			switch ((int)pImage->GetMetafileType())
			{
				case MetafileDataTypeEmf:
				case MetafileDataTypeEmfPlusOnly:
				case MetafileDataTypeEmfPlusDual:
					return DrawMetafile<CEmfParser>(pBuffer, unSizeBuffer, oSrcRect, arPoints);
				case MetafileDataTypeWmf:
				case MetafileDataTypeWmfPlaceable:
					return DrawMetafile<CWmfParser>(pBuffer, unSizeBuffer, oSrcRect, arPoints);
			}
		}
	}

	template<typename MetafileType>
	void CEmfPlusParser::DrawMetafile(BYTE *pBuffer, unsigned int unSize, const TEmfPlusRectF &oSrcRect, const std::vector<TEmfPlusPointF> &arPoints)
	{
		if (NULL == pBuffer || 0 == unSize || 3 != arPoints.size())
			return;

		MetafileType oParser;
		oParser.SetStream(pBuffer, unSize);
		oParser.SetFontManager(GetFontManager());
		oParser.Scan();

		if (oParser.CheckError())
			return;

		const TRectL* pFileBounds = oParser.GetDCBounds();
		const double dFileWidth  = std::abs(pFileBounds->Right  - pFileBounds->Left);
		const double dFileHeight = std::abs(pFileBounds->Bottom - pFileBounds->Top);

		const TRectL* pParentBounds = GetDCBounds();
		const double dParentWidth   = std::abs(pParentBounds->Right  - pParentBounds->Left);
		const double dParentHeight  = std::abs(pParentBounds->Bottom - pParentBounds->Top);

		if (InterpretatorType::Render == m_pInterpretator->GetType())
		{
			NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();

			pGrRenderer->SetFontManager(GetFontManager());

			double dScale = ((CEmfInterpretatorRender*)m_pInterpretator)->GetRenderer()->GetWidth() * 96. / 25.4 / dParentWidth;

			const double dMaxWidth  = std::max(MAX_PICTURE_SIZE, dParentWidth);
			const double dMaxHeight = std::max(MAX_PICTURE_SIZE, dParentHeight); 

			if (dFileWidth > dMaxWidth || dFileHeight > dMaxHeight)
				dScale *= std::min(dMaxWidth / dFileWidth, dMaxHeight / dFileHeight);

			const int nWidth  = dFileWidth  * dScale;
			const int nHeight = dFileHeight * dScale;

			BYTE* pBgraData = new(std::nothrow) BYTE[nWidth * nHeight * 4];

			if (!pBgraData)
				return;

			unsigned int alfa = 0xffffff;
			//дефолтный тон должен быть прозрачным, а не белым
			//memset(pBgraData, 0x00, nWidth * nHeight * 4);
			for (int i = 0; i < nWidth * nHeight; i++)
				((unsigned int*)pBgraData)[i] = alfa;

			const double dWidth  = nWidth  * 25.4 / 96;
			const double dHeight = nHeight * 25.4 / 96;

			CBgraFrame oFrame;
			oFrame.put_Data(pBgraData);
			oFrame.put_Width(nWidth);
			oFrame.put_Height(nHeight);
			oFrame.put_Stride(4 * nWidth);

			pGrRenderer->CreateFromBgraFrame(&oFrame);
			pGrRenderer->SetSwapRGB(false);
			pGrRenderer->put_Width(dWidth);
			pGrRenderer->put_Height(dHeight);

			pGrRenderer->BeginCommand(c_nImageType);

			CMetaFileRenderer oEmfOut(&oParser, pGrRenderer, 0, 0, dWidth, dHeight);
			oParser.SetInterpretator(&oEmfOut);

			oParser.PlayFile();

			pGrRenderer->EndCommand(c_nImageType);

			BYTE* pPixels = oFrame.get_Data();

			TRectL oClipRect;

			oClipRect.Left   = oSrcRect.dX * dScale;
			oClipRect.Top    = oSrcRect.dY * dScale;
			oClipRect.Right  = (oSrcRect.dX + oSrcRect.dWidth)  * dScale;
			oClipRect.Bottom = (oSrcRect.dY + oSrcRect.dHeight) * dScale;

			BYTE* pNewBuffer = GetClipedImage(pPixels, nWidth, nHeight, oClipRect);

			const unsigned int unWidth  = std::min(((unsigned int)fabs(oClipRect.Right - oClipRect.Left)), ((unsigned int)nWidth ));
			const unsigned int unHeight = std::min(((unsigned int)fabs(oClipRect.Bottom - oClipRect.Top)), ((unsigned int)nHeight));

			m_pInterpretator->DrawBitmap(arPoints[0].X, arPoints[0].Y, arPoints[1].X - arPoints[0].X - m_pDC->GetPixelWidth(), arPoints[2].Y - arPoints[0].Y - m_pDC->GetPixelHeight(),
			                             (NULL != pNewBuffer) ? pNewBuffer : pPixels, unWidth, unHeight);

			RELEASEINTERFACE(pGrRenderer);
			RELEASEARRAYOBJECTS(pNewBuffer);
		}
		else if (InterpretatorType::Svg == m_pInterpretator->GetType())
		{
			((MetafileType*)&oParser)->SetInterpretator(InterpretatorType::Svg);

			oParser.PlayFile();

			TXForm *pXForm = m_pDC->GetTransform();

			TRectD oRect;

			oRect.Left   = arPoints[0].X;
			oRect.Top    = arPoints[0].Y;
			oRect.Right  = arPoints[1].X - m_pDC->GetPixelWidth();
			oRect.Bottom = arPoints[2].Y - m_pDC->GetPixelHeight();

			TRectD oTempSrcRect;

			oTempSrcRect.Left   = oSrcRect.dX - pFileBounds->Left;
			oTempSrcRect.Top    = oSrcRect.dY - pFileBounds->Top;
			oTempSrcRect.Right  = oTempSrcRect.Left + ((dFileWidth  > oSrcRect.dWidth)  ? oSrcRect.dWidth  - GetPixelWidth()  : dFileWidth);
			oTempSrcRect.Bottom = oTempSrcRect.Top  + ((dFileHeight > oSrcRect.dHeight) ? oSrcRect.dHeight - GetPixelHeight() : dFileHeight);

			TXForm oTransform;

			oTransform.Copy(pXForm);

			oTransform.Dx -= m_oHeader.oFramePx.Left;
			oTransform.Dy -= m_oHeader.oFramePx.Top;

			CInterpretatorSvgBase* pParserSvgInterpretator = dynamic_cast<CInterpretatorSvgBase*>(oParser.GetInterpretator());
			CInterpretatorSvgBase *pSvgInterpretator = dynamic_cast<CInterpretatorSvgBase*>(m_pInterpretator);

			if (NULL != pParserSvgInterpretator && NULL != pSvgInterpretator)
				pSvgInterpretator->IncludeSvg(pParserSvgInterpretator->GetFile(), oRect, oTempSrcRect, &oTransform);
		}
	}

	void CEmfPlusParser::DrawBitmap(BYTE *pBuffer, unsigned int unSize, unsigned int unWidth, unsigned int unHeight, const TEmfPlusRectF& oSrcRect, const std::vector<TEmfPlusPointF>& arPoints)
	{
		if (NULL == pBuffer || 0 == unSize)
			return;

		BYTE* pBytes = NULL;
		bool bExternalBuffer = false;

		if (unWidth != 0 && unHeight != 0)
		{
			pBytes = pBuffer;
			bExternalBuffer = true;
		}
		else
		{
			CBgraFrame oBgraFrame;

			if (!oBgraFrame.Decode(pBuffer, unSize))
				return;

			unWidth  = oBgraFrame.get_Width();
			unHeight = oBgraFrame.get_Height();
			pBytes   = oBgraFrame.get_Data();

			oBgraFrame.put_Data(NULL);

			FlipYImage(pBytes, unWidth, unHeight);
		}

		TRectL oClipRect;

		oClipRect.Left   = oSrcRect.dX;
		oClipRect.Top    = oSrcRect.dY;
		oClipRect.Right  = (oSrcRect.dX + oSrcRect.dWidth);
		oClipRect.Bottom = (oSrcRect.dY + oSrcRect.dHeight);

		BYTE* pNewBuffer = GetClipedImage(pBytes, unWidth, unHeight, oClipRect);

		m_pInterpretator->DrawBitmap(arPoints[0].X, arPoints[0].Y, arPoints[1].X - arPoints[0].X, arPoints[2].Y - arPoints[0].Y,
				(NULL != pNewBuffer) ? pNewBuffer : pBytes, fabs(oClipRect.Right - oClipRect.Left), fabs(oClipRect.Bottom - oClipRect.Top));

		if (!bExternalBuffer)
			RELEASEARRAYOBJECTS(pBytes);

		RELEASEARRAYOBJECTS(pNewBuffer);
	}

	void CEmfPlusParser::Read_EMFPLUS_HEADER(unsigned short unShFlags)
	{
		m_oStream.Skip(4); //Version

		unsigned int unEmfPlusFlags;

		m_oStream >> unEmfPlusFlags;
		m_oStream >> m_unLogicalDpiX;
		m_oStream >> m_unLogicalDpiY;

		m_bBanEmfProcessing = true;

		if (NULL != m_pInterpretator)
		{
			m_pInterpretator->Begin();
			m_pInterpretator->HANDLE_EMFPLUS_HEADER(unEmfPlusFlags, m_unLogicalDpiX, m_unLogicalDpiY);
		}
		//TODO: добавить установление нового Dpi (нужно ли?)
	}

	void CEmfPlusParser::Read_EMFPLUS_CLEAR()
	{
		TEmfPlusARGB oARGB;

		m_oStream >> oARGB;

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_CLEAR(oARGB);

		m_bBanEmfProcessing = true;
	}

	void CEmfPlusParser::Read_EMFPLUS_DRAWARC(unsigned short unShFlags)
	{
		if ((unShFlags >>(14)) & 1 )
			Read_EMFPLUS_DRAWARC_BASE<TEmfPlusRect>(unShFlags);
		else
			Read_EMFPLUS_DRAWARC_BASE<TEmfPlusRectF>(unShFlags);

		m_bBanEmfProcessing = true;
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

		CEmfPlusPen *pPen = GetPen(chOgjectIndex);

		if (NULL == pPen)
			return;

		m_pDC->SetPen(pPen);

		if (AD_COUNTERCLOCKWISE != m_pDC->GetArcDirection())
		{
			dSweepAngle = dSweepAngle - 360;
		}

		TEmfPlusRectF oConvertedRect = GetConvertedRectangle(oRect);

		MoveTo(oConvertedRect.dX, oConvertedRect.dY);
		ArcTo(oConvertedRect.dX, oConvertedRect.dY,
			  oConvertedRect.dX + oConvertedRect.dWidth,
			  oConvertedRect.dY + oConvertedRect.dHeight,
			  dStartAngle, dSweepAngle);
		DrawPath(true, false);

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_DRAWARC(chOgjectIndex, dStartAngle, dSweepAngle, oConvertedRect);

		m_pDC->RemovePen(pPen);
	}


	void CEmfPlusParser::Read_EMFPLUS_DRAWBEZIERS(unsigned short unShFlags)
	{
		if ((unShFlags >>(11)) & 1 )
		{
			//Определен флаг P (С игнорируется)
			//Read_EMFPLUS_DRAWBEZIERS_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
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

		m_bBanEmfProcessing = true;
	}

	template<typename T>
	void CEmfPlusParser::Read_EMFPLUS_DRAWBEZIERS_BASE(unsigned short unShFlags)
	{
		short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
		unsigned int unCountPoints;

		m_oStream >> unCountPoints;

		if (unCountPoints < 3)
			return;

		std::vector<T> arPoints(unCountPoints);

		for (unsigned int unIndex = 0; unIndex < unCountPoints; ++unIndex)
			m_oStream >> arPoints[unIndex];

		CEmfPlusPen *pPen = GetPen(shOgjectIndex);

		if (NULL == pPen)
			return;

		m_pDC->SetPen(pPen);

		std::vector<TEmfPlusPointF> arConvertedPoints = GetConvertedPoints(arPoints);

		MoveTo(arConvertedPoints[0].X, arConvertedPoints[0].Y);

		for (unsigned int unIndex = 1; unIndex < arPoints.size(); unIndex += 3)
			CurveTo(arConvertedPoints[unIndex + 0].X, arConvertedPoints[unIndex + 0].Y,
					arConvertedPoints[unIndex + 1].X, arConvertedPoints[unIndex + 1].Y,
					arConvertedPoints[unIndex + 2].X, arConvertedPoints[unIndex + 2].Y);

		DrawPath(true, false);

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_DRAWBEZIERS(shOgjectIndex, arConvertedPoints);

		m_pDC->RemovePen(pPen);
	}

	void CEmfPlusParser::Read_EMFPLUS_DRAWCLOSEDCURVE(unsigned short unShFlags)
	{
		if ((unShFlags >>(11)) & 1 )
		{
			//Определен флаг P (С игнорируется)
			//Read_EMFPLUS_DRAWCLOSEDCURVE_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
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

		m_bBanEmfProcessing = true;
	}

	template<typename T>
	void CEmfPlusParser::Read_EMFPLUS_DRAWCLOSEDCURVE_BASE(unsigned short unShFlags)
	{
		short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
		double dTension;
		unsigned int unCount;

		m_oStream >> dTension;
		m_oStream >> unCount;

		if (unCount < 3)
			return;

		std::vector<T> arPoints(unCount);

		for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
			m_oStream >> arPoints[0];

		CEmfPlusPen *pPen = GetPen(shOgjectIndex);

		if (NULL == pPen)
			return;

		m_pDC->SetPen(pPen);

		std::vector<TEmfPlusPointF> arConvertedPoints = GetConvertedPoints(arPoints);

		MoveTo(arConvertedPoints[0].X, arConvertedPoints[0].Y);

		for (unsigned int unIndex = 1; unIndex < unCount; unIndex += 3)
			CurveTo(arConvertedPoints[unIndex + 0].X, arConvertedPoints[unIndex + 0].Y,
			        arConvertedPoints[unIndex + 1].X, arConvertedPoints[unIndex + 1].Y,
			        arConvertedPoints[unIndex + 2].X, arConvertedPoints[unIndex + 2].Y);

		ClosePath();

		DrawPath(true, false);

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_DRAWCLOSEDCURVE(shOgjectIndex, dTension, arConvertedPoints);

		m_pDC->RemovePen(pPen);
	}

	void CEmfPlusParser::Read_EMFPLUS_DRAWCURVE(unsigned short unShFlags)
	{
		if ((unShFlags >>(14)) & 1 )
			Read_EMFPLUS_DRAWCURVE_BASE<TEmfPlusPoint>(unShFlags);
		else
			Read_EMFPLUS_DRAWCURVE_BASE<TEmfPlusPointF>(unShFlags);

		m_bBanEmfProcessing = true;
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
			m_oStream >> arPoints[0];

		CEmfPlusPen *pPen = GetPen(shOgjectIndex);

		if (NULL == pPen)
			return;

		m_pDC->SetPen(pPen);

		std::vector<TEmfPlusPointF> arConvertedPoints = GetConvertedPoints(arPoints);

		MoveTo(arConvertedPoints[0].X, arConvertedPoints[0].Y);

		for (unsigned int unIndex = 1; unIndex < unCountPoints; unIndex += 3)
			CurveTo(arConvertedPoints[unIndex + 0].X, arConvertedPoints[unIndex + 0].Y,
			        arConvertedPoints[unIndex + 1].X, arConvertedPoints[unIndex + 1].Y,
			        arConvertedPoints[unIndex + 2].X, arConvertedPoints[unIndex + 2].Y);

		DrawPath(true, false);

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_DRAWCURVE(shOgjectIndex, dTension, unOffset, unNumSegments, arConvertedPoints);

		m_pDC->RemovePen(pPen);
	}

	void CEmfPlusParser::Read_EMFPLUS_DRAWDRIVERSTRING(unsigned short unShFlags)
	{
		short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
		unsigned int unBrushId, unDriverStringOptionsFlags, unMatrixPresent, unGlyphCount;

		m_oStream >> unBrushId;
		m_oStream >> unDriverStringOptionsFlags;
		m_oStream >> unMatrixPresent;
		m_oStream >> unGlyphCount;

		if (unGlyphCount == 0)
			return;

		unsigned short* pString = new unsigned short[unGlyphCount + 1];
		if (pString)
		{
			pString[unGlyphCount] = 0x00;
			m_oStream.ReadBytes(pString, unGlyphCount);
		}

		std::wstring wsString;

		wsString = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)pString, unGlyphCount);

		RELEASEARRAYOBJECTS(pString)

		std::vector<TEmfPlusPointF> arGlyphPos = ReadPoints<TEmfPlusPointF>(unGlyphCount);

		if (0x00000001 == unMatrixPresent)
		{
			TEmfPlusXForm oMatrix;

			m_oStream >> oMatrix;

			m_bBanEmfProcessing = true;
		}
		//----------
		if (NULL == m_pInterpretator || wsString.length() != arGlyphPos.size())
			return;

		std::vector<TPointD> arDPoints(arGlyphPos.size());
		for (unsigned int unIndex = 0; unIndex < arGlyphPos.size(); ++unIndex)
		{
			arDPoints[unIndex].X = arGlyphPos[unIndex].X;
			arDPoints[unIndex].Y = arGlyphPos[unIndex].Y;
		}

		CEmfPlusFont *pFont = GetFont(shOgjectIndex);

		if (NULL == pFont)
			return;

		m_pDC->SetFont(pFont);

		if ((unShFlags >>(15)) & 1 )//BrushId = Color
		{
			TRGBA oColor;

			oColor.b = (unBrushId >> 0)  & 0xFF;
			oColor.g = (unBrushId >> 8)  & 0xFF;
			oColor.r = (unBrushId >> 16) & 0xFF;
			oColor.a = (unBrushId >> 24) & 0xFF;

			TRGBA oTextColor = m_pDC->GetTextColor();

			m_pDC->SetTextColor(oColor);

			m_pInterpretator->DrawDriverString(wsString, arDPoints);
			m_pInterpretator->HANDLE_EMFPLUS_DRAWDRIVERSTRING(shOgjectIndex, unBrushId, unDriverStringOptionsFlags, unMatrixPresent, NULL, wsString, arDPoints);

			m_pDC->SetTextColor(oTextColor);
		}
		else //BrushId = Brush id
		{
			CEmfPlusBrush *pBrush = GetBrush(unBrushId);

			if (NULL == pBrush) return;

			TRGBA oColor;

			oColor.b = pBrush->oColor.chBlue;
			oColor.g = pBrush->oColor.chGreen;
			oColor.r = pBrush->oColor.chRed;
			oColor.a = pBrush->oColor.chAlpha;

			TRGBA oTextColor = m_pDC->GetTextColor();

			m_pDC->SetTextColor(oColor);

			m_pInterpretator->DrawDriverString(wsString, arDPoints);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMFPLUS_DRAWDRIVERSTRING(shOgjectIndex, unBrushId, unDriverStringOptionsFlags, unMatrixPresent, NULL, wsString, arDPoints);

			m_pDC->SetTextColor(oTextColor);
		}

		m_pDC->RemoveFont(pFont);

		m_bBanEmfProcessing = true;
	}

	void CEmfPlusParser::Read_EMFPLUS_DRAWELLIPSE(unsigned short unShFlags)
	{
		if ((unShFlags >>(14)) & 1 )
			Read_EMFPLUS_DRAWELLIPSE_BASE<TEmfPlusRect>(unShFlags);
		else
			Read_EMFPLUS_DRAWELLIPSE_BASE<TEmfPlusRectF>(unShFlags);

		m_bBanEmfProcessing = true;
	}

	template<typename T>
	void CEmfPlusParser::Read_EMFPLUS_DRAWELLIPSE_BASE(unsigned short unShFlags)
	{
		short shObjectID = ExpressValue(unShFlags, 0, 7);
		T oRect;

		m_oStream >> oRect;

		CEmfPlusPen *pEmfPlusPen = GetPen(shObjectID);

		if (NULL == pEmfPlusPen) return;

		if (NULL != pEmfPlusPen->pBrush)
			m_pDC->SetBrush(pEmfPlusPen->pBrush);

		m_pDC->SetPen(pEmfPlusPen);

		TRectL oBox = oRect.ToRectL();

		if (m_pDC->GetArcDirection() == AD_COUNTERCLOCKWISE)
			ArcTo(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, 0, 360);
		else
			ArcTo(oBox.Left, oBox.Bottom, oBox.Right, oBox.Top, 0, 360);

		DrawPath(true, false);

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_DRAWELLIPSE(shObjectID, GetConvertedRectangle(oRect));

		if (NULL != pEmfPlusPen->pBrush)
			m_pDC->RemoveBrush(pEmfPlusPen->pBrush);

		m_pDC->RemovePen(pEmfPlusPen);
	}

	void CEmfPlusParser::Read_EMFPLUS_DRAWIMAGE(unsigned short unShFlags)
	{
		if ((unShFlags >>(14)) & 1 )
			Read_EMFPLUS_DRAWIMAGE_BASE<TEmfPlusRect>(unShFlags);
		else
			Read_EMFPLUS_DRAWIMAGE_BASE<TEmfPlusRectF>(unShFlags);

		m_bBanEmfProcessing = true;
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

		if (nSrcUnit != UnitTypePixel)
			return;

		m_oStream >> oSrcRect;

		T oRectData;

		m_oStream >> oRectData;

		TEmfPlusRectF oRect = GetConvertedRectangle(oRectData);

		std::vector<TEmfPlusPointF> arPoints = {{oRect.dX, oRect.dY}, {oRect.dX + oRect.dWidth, oRect.dY}, {oRect.dX + oRect.dWidth, oRect.dY + oRect.dHeight}};

		DrawImagePoints(shOgjectIndex, unImageAttributesID, oSrcRect, arPoints);
	}

	void CEmfPlusParser::Read_EMFPLUS_DRAWIMAGEPOINTS(unsigned short unShFlags)
	{
		if ((unShFlags >>(11)) & 1 )
		{
			//Определен флаг P (С игнорируется)
			//Read_EMFPLUS_DRAWIMAGEPOINTS_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
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

		m_bBanEmfProcessing = true;
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
			//Read_EMFPLUS_DRAWLINES_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
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

		m_bBanEmfProcessing = true;
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

		CEmfPlusPen *pEmfPlusPen = GetPen(shOgjectIndex);

		if (NULL == pEmfPlusPen) return;

		m_pDC->SetPen(pEmfPlusPen);

		if (NULL != pEmfPlusPen->pBrush)
			m_pDC->SetBrush(pEmfPlusPen->pBrush);

		DrawLines(GetConvertedPoints(arPoints), ((unShFlags >>(13)) & 1));

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_DRAWLINES(shOgjectIndex, GetConvertedPoints(arPoints));

		if (NULL != pEmfPlusPen->pBrush)
			m_pDC->RemoveBrush(pEmfPlusPen->pBrush);

		m_pDC->RemovePen(pEmfPlusPen);

		m_bBanEmfProcessing = true;
	}

	void CEmfPlusParser::Read_EMFPLUS_DRAWPATH(unsigned short unShFlags)
	{
		short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
		unsigned int unPenId;

		m_oStream >> unPenId;

		CEmfPlusPath *pPath = GetPath(shOgjectIndex);

		if (NULL != pPath)
		{
			CEmfPlusPen *pEmfPlusPen = GetPen(unPenId);

			if (NULL == pEmfPlusPen) return;

			m_pDC->SetPen(pEmfPlusPen);

			if (NULL != pEmfPlusPen->pBrush)
				m_pDC->SetBrush(pEmfPlusPen->pBrush);

			CPathConverter oPathConverter;
			CPath oNewPath, oLineCapPath;

			oPathConverter.GetUpdatedPath(oNewPath, oLineCapPath, *pPath, *pEmfPlusPen);

			oNewPath.DrawOn(m_pInterpretator, true, false);
			oLineCapPath.DrawOn(m_pInterpretator, false, true);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMFPLUS_DRAWPATH(shOgjectIndex, unPenId, &oNewPath);

			if (NULL != pEmfPlusPen->pBrush)
				m_pDC->RemoveBrush(pEmfPlusPen->pBrush);

			m_pDC->RemovePen(pEmfPlusPen);
		}

		m_bBanEmfProcessing = true;
	}

	void CEmfPlusParser::Read_EMFPLUS_DRAWPIE(unsigned short unShFlags)
	{
		if ((unShFlags >>(14)) & 1 )
			Read_EMFPLUS_DRAWPIE_BASE<TEmfPlusRect>(unShFlags);
		else
			Read_EMFPLUS_DRAWPIE_BASE<TEmfPlusRectF>(unShFlags);

		m_bBanEmfProcessing = true;
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

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_DRAWPIE(shOgjectIndex, dStartAngle, dSweepAngle, GetConvertedRectangle(oRect));

		//TODO: реализовать
	}

	void CEmfPlusParser::Read_EMFPLUS_DRAWRECTS(unsigned short unShFlags)
	{
		if ((unShFlags >>(14)) & 1 )
			Read_EMFPLUS_DRAWRECTS_BASE<TEmfPlusRect>(unShFlags);
		else
			Read_EMFPLUS_DRAWRECTS_BASE<TEmfPlusRectF>(unShFlags);

		m_bBanEmfProcessing = true;
	}

	template<typename T>
	void CEmfPlusParser::Read_EMFPLUS_DRAWRECTS_BASE(unsigned short unShFlags)
	{
		short shPenIndex = ExpressValue(unShFlags, 0, 7);
		unsigned int unCount;

		m_oStream >> unCount;

		if (unCount == 0)
			return;

		CEmfPlusPen *pEmfPlusPen = GetPen(shPenIndex);

		if (NULL == pEmfPlusPen) return;

		m_pDC->SetPen(pEmfPlusPen);

		if (NULL != pEmfPlusPen->pBrush)
			m_pDC->SetBrush(pEmfPlusPen->pBrush);

		std::vector<TEmfPlusRectF> arRects(unCount);
		T oTempRect;

		for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
		{
			m_oStream >> oTempRect;
			arRects[unIndex] = GetConvertedRectangle(oTempRect);
			DrawRectangle(arRects[unIndex], true, false);
		}

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_DRAWRECTS(shPenIndex, arRects);

		if (NULL != pEmfPlusPen->pBrush)
			m_pDC->RemoveBrush(pEmfPlusPen->pBrush);

		m_pDC->RemovePen(pEmfPlusPen);
	}

	void CEmfPlusParser::Read_EMFPLUS_DRAWSTRING(unsigned short unShFlags)
	{
		short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
		unsigned int unBrushId, unFormatID, unLength;
		TEmfPlusRectF oRect;

		m_oStream >> unBrushId;
		m_oStream >> unFormatID;
		m_oStream >> unLength;

		if (0 == unLength)
			return;

		m_oStream >> oRect;

		unsigned short* pString = new unsigned short[unLength + 1];
		if (pString)
		{
			pString[unLength] = 0x00;
			m_oStream.ReadBytes(pString, unLength);
		}

		std::wstring wsString;

		wsString = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)pString, unLength);

		RELEASEARRAYOBJECTS(pString)

		if (NULL == m_pInterpretator)
			return;

		CEmfPlusFont *pFont = GetFont(shOgjectIndex);

		if (NULL == pFont)
			return;

		m_pDC->SetFont(pFont);

		CEmfPlusStringFormat *pStringFormat = GetStringFormat(unFormatID);

		unsigned int unOldTextAlign = m_pDC->GetTextAlign();

		if (NULL != pStringFormat)
		{
			unsigned int unNewTextAlign = 0;

			if (StringAlignmentCenter == pStringFormat->unStringAlignment)
			{
				unNewTextAlign |= TA_CENTER;
				oRect.dX += oRect.dWidth / 2;
			}
			else if (StringAlignmentFar == pStringFormat->unStringAlignment)
			{
				unNewTextAlign |= TA_RIGHT;
				oRect.dX += oRect.dWidth;
			}

			if (StringAlignmentCenter == pStringFormat->unLineAlign)
			{
				unNewTextAlign |= VTA_CENTER << 8;
				oRect.dY += oRect.dHeight / 2;
			}
			else if (StringAlignmentFar == pStringFormat->unLineAlign)
			{
				unNewTextAlign |= VTA_BOTTOM << 8;
				oRect.dY += oRect.dHeight;
			}

			m_pDC->SetTextAlign(unNewTextAlign);
		}

		if ((unShFlags >>(15)) & 1 )//BrushId = Color
		{
			TRGBA oColor;

			oColor.b = (unBrushId >> 0)  & 0xFF;
			oColor.g = (unBrushId >> 8)  & 0xFF;
			oColor.r = (unBrushId >> 16) & 0xFF;
			oColor.a = (unBrushId >> 24) & 0xFF;

			TRGBA oTextColor = m_pDC->GetTextColor();

			m_pDC->SetTextColor(oColor);

			m_pInterpretator->DrawString(wsString, wsString.length(), oRect.dX, oRect.dY, NULL, GM_ADVANCED);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMFPLUS_DRAWSTRING(shOgjectIndex, unBrushId, unFormatID, wsString, oRect);

			m_pDC->SetTextColor(oTextColor);
		}
		else //BrushId = Brush id
		{
			CEmfPlusBrush *pBrush = GetBrush(unBrushId);

			if (NULL == pBrush) return;

			TRGBA oColor;

			oColor.b = pBrush->oColor.chBlue;
			oColor.g = pBrush->oColor.chGreen;
			oColor.r = pBrush->oColor.chRed;
			oColor.a = pBrush->oColor.chAlpha;

			TRGBA oTextColor = m_pDC->GetTextColor();

			m_pDC->SetTextColor(oColor);

			m_pInterpretator->DrawString(wsString, wsString.length(), oRect.dX, oRect.dX, NULL, GM_ADVANCED);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMFPLUS_DRAWSTRING(shOgjectIndex, unBrushId, unFormatID, wsString, oRect);

			m_pDC->SetTextColor(oTextColor);
		}

		m_pDC->SetTextAlign(unOldTextAlign);

		m_pDC->RemoveFont(pFont);

		m_bBanEmfProcessing = true;
	}

	void CEmfPlusParser::Read_EMFPLUS_FILLCLOSEDCURVE(unsigned short unShFlags)
	{
		if ((unShFlags >>(11)) & 1 )
		{
			//Определен флаг P (С игнорируется)
			//Read_EMFPLUS_FILLCLOSEDCURVE_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
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

		m_bBanEmfProcessing = true;
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

		m_bBanEmfProcessing = true;
	}

	template<typename T>
	void CEmfPlusParser::Read_EMFPLUS_FILLELLIPSE_BASE(unsigned short unShFlags)
	{
		unsigned int unBrushId;
		T oRect;

		m_oStream >> unBrushId;
		m_oStream >> oRect;

		TRectL oBox = oRect.ToRectL();

		if ((unShFlags >>(15)) & 1 )//BrushId = Color
		{
			CEmfPlusBrush oBrush;

			oBrush.oColor.chBlue     = (unBrushId >> 0)  & 0xFF;
			oBrush.oColor.chGreen    = (unBrushId >> 8)  & 0xFF;
			oBrush.oColor.chRed      = (unBrushId >> 16) & 0xFF;
			oBrush.oColor.chAlpha    = (unBrushId >> 24) & 0xFF;

			m_pDC->SetBrush(&oBrush);

			if (m_pDC->GetArcDirection() == AD_COUNTERCLOCKWISE)
				ArcTo(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, 0, 360);
			else
				ArcTo(oBox.Left, oBox.Bottom, oBox.Right, oBox.Top, 0, 360);

			DrawPath(false, true);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMFPLUS_FILLELLIPSE(unBrushId, GetConvertedRectangle(oRect));

			m_pDC->RemoveBrush(&oBrush);
		}
		else //BrushId = Brush id
		{
			CEmfPlusBrush *pBrush = GetBrush(unBrushId);

			if (NULL == pBrush) return;

			m_pDC->SetBrush(pBrush);

			if (m_pDC->GetArcDirection() == AD_COUNTERCLOCKWISE)
				ArcTo(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, 0, 360);
			else
				ArcTo(oBox.Left, oBox.Bottom, oBox.Right, oBox.Top, 0, 360);

			DrawPath(false, true);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMFPLUS_FILLELLIPSE(unBrushId, GetConvertedRectangle(oRect));

			m_pDC->RemoveBrush(pBrush);
		}
	}

	void CEmfPlusParser::Read_EMFPLUS_FILLPATH(unsigned short unShFlags)
	{
		short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
		int unBrushId;

		m_oStream >> unBrushId;

		CEmfPlusPath *pPath = GetPath(shOgjectIndex);

		if (NULL == pPath) return;

		if ((unShFlags >>(15)) & 1 )//BrushId = Color
		{
			CEmfPlusBrush oBrush;
			
			oBrush.oColor.chBlue  = (unBrushId >> 0)  & 0xFF;
			oBrush.oColor.chGreen = (unBrushId >> 8)  & 0xFF;
			oBrush.oColor.chRed   = (unBrushId >> 16) & 0xFF;
			oBrush.oColor.chAlpha = (unBrushId >> 24) & 0xFF;

			m_pDC->SetBrush(&oBrush);

			pPath->DrawOn(m_pInterpretator, false, true);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMFPLUS_FILLPATH(shOgjectIndex, unBrushId, pPath);

			m_pDC->RemoveBrush(&oBrush);
		}
		else //BrushId = Brush id
		{
			CEmfPlusBrush *pBrush = GetBrush(unBrushId);

			if (NULL == pBrush) return;

			m_pDC->SetBrush(pBrush);

			pPath->DrawOn(m_pInterpretator, false, true);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMFPLUS_FILLPATH(shOgjectIndex, unBrushId, pPath);

			m_pDC->RemoveBrush(pBrush);
		}

		m_bBanEmfProcessing = true;
	}

	void CEmfPlusParser::Read_EMFPLUS_FILLPIE(unsigned short unShFlags)
	{
		if ((unShFlags >>(14)) & 1 )
			Read_EMFPLUS_FILLPIE_BASE<TEmfPlusRect>(unShFlags);
		else
			Read_EMFPLUS_FILLPIE_BASE<TEmfPlusRectF>(unShFlags);

		m_bBanEmfProcessing = true;
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

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_FILLPIE(unBrushId, dStartAngle, dSweepAngle, GetConvertedRectangle(oRect));

		//TODO: реализовать
	}

	void CEmfPlusParser::Read_EMFPLUS_FILLPOLYGON(unsigned short unShFlags)
	{
		if ((unShFlags >>(11)) & 1 )
		{
			//Определен флаг P (С игнорируется)
			//Read_EMFPLUS_FILLPOLYGON_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
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

		m_bBanEmfProcessing = true;
	}

	template<typename T>
	void CEmfPlusParser::Read_EMFPLUS_FILLPOLYGON_BASE(unsigned short unShFlags)
	{
		unsigned int unBrushId, unCount;

		m_oStream >> unBrushId;
		m_oStream >> unCount;

		if (unCount < 2)
			return;

		std::vector<T> arPoints(unCount);

		for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
			m_oStream >> arPoints[unIndex];

		std::vector<TEmfPlusPointF> arNewPoints = GetConvertedPoints(arPoints);

		if ((unShFlags >>(15)) & 1 )//BrushId = Color
		{
			CEmfPlusBrush oBrush;

			oBrush.oColor.chBlue     = (unBrushId >> 0)  & 0xFF;
			oBrush.oColor.chGreen    = (unBrushId >> 8)  & 0xFF;
			oBrush.oColor.chRed      = (unBrushId >> 16) & 0xFF;
			oBrush.oColor.chAlpha    = (unBrushId >> 24) & 0xFF;

			m_pDC->SetBrush(&oBrush);

			MoveTo(arNewPoints[0].X, arNewPoints[0].Y);

			for (unsigned int unIndex = 1; unIndex < unCount; ++unIndex)
				LineTo(arNewPoints[unIndex].X, arNewPoints[unIndex].Y);

			ClosePath();
			DrawPath(false, true);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMFPLUS_FILLPOLYGON(unBrushId, arNewPoints);

			m_pDC->RemoveBrush(&oBrush);
		}
		else //BrushId = Brush id
		{
			CEmfPlusBrush *pEmfPlusBrush = GetBrush(unBrushId);

			if (NULL == pEmfPlusBrush) return;

			m_pDC->SetBrush(pEmfPlusBrush);

			MoveTo(arNewPoints[0].X, arNewPoints[0].Y);

			for (unsigned int unIndex = 1; unIndex < unCount; ++unIndex)
				LineTo(arNewPoints[unIndex].X, arNewPoints[unIndex].Y);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMFPLUS_FILLPOLYGON(unBrushId, arNewPoints);

			ClosePath();
			DrawPath(false, true);

			m_pDC->RemoveBrush(pEmfPlusBrush);
		}
	}

	void CEmfPlusParser::Read_EMFPLUS_FILLRECTS(unsigned short unShFlags)
	{
		if ((unShFlags >>(14)) & 1 )
			Read_EMFPLUS_FILLRECTS_BASE<TEmfPlusRect>(unShFlags);
		else
			Read_EMFPLUS_FILLRECTS_BASE<TEmfPlusRectF>(unShFlags);

		m_bBanEmfProcessing = true;
	}

	template<typename T>
	void CEmfPlusParser::Read_EMFPLUS_FILLRECTS_BASE(unsigned short unShFlags)
	{
		unsigned int unBrushId, unCount;

		m_oStream >> unBrushId;
		m_oStream >> unCount;

		if (unCount == 0)
			return;

		std::vector<TEmfPlusRectF> arRects(unCount);
		T oTempRect;

		for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
		{
			m_oStream >> oTempRect;
			arRects[unIndex] = GetConvertedRectangle(oTempRect);
		}

		if ((unShFlags >>(15)) & 1 )//BrushId = Color
		{
			CEmfPlusBrush oBrush;

			oBrush.oColor.chBlue     = (unBrushId >> 0)  & 0xFF;
			oBrush.oColor.chGreen    = (unBrushId >> 8)  & 0xFF;
			oBrush.oColor.chRed      = (unBrushId >> 16) & 0xFF;
			oBrush.oColor.chAlpha    = (unBrushId >> 24) & 0xFF;

			m_pDC->SetBrush(&oBrush);

			for (TEmfPlusRectF &oRectangle : arRects)
				DrawRectangle(oRectangle, false, true);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMFPLUS_FILLRECTS(unBrushId, arRects);

			m_pDC->RemoveBrush(&oBrush);
		}
		else //BrushId = Brush id
		{
			CEmfPlusBrush *pEmfPlusBrush = GetBrush(unBrushId);

			if (NULL == pEmfPlusBrush) return;

			m_pDC->SetBrush(pEmfPlusBrush);

			for (TEmfPlusRectF &oRectangle : arRects)
				DrawRectangle(oRectangle, false, true);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMFPLUS_FILLRECTS(unBrushId, arRects);

			m_pDC->RemoveBrush(pEmfPlusBrush);
		}
	}

	void CEmfPlusParser::Read_EMFPLUS_FILLREGION(unsigned short unShFlags)
	{
		short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
		unsigned int unBrushId;

		m_oStream >> unBrushId;

		//TODO: реализовать
		m_bBanEmfProcessing = true;
	}

	void CEmfPlusParser::Read_EMFPLUS_OBJECT(unsigned short unShFlags)
	{
		short shObjectIndex = ExpressValue(unShFlags, 0, 7);
		short shObjectType  = ExpressValue(unShFlags, 8, 14);

		switch (shObjectType)
		{
		case ObjectTypeInvalid: return;
		case ObjectTypeBrush:
		{
			LOGGING(L"Object Brush with index: " << shObjectIndex)

			CEmfPlusBrush *pEmfPlusBrush = ReadBrush();

			RegisterObject(pEmfPlusBrush, shObjectIndex);

			break;
		}
		case ObjectTypePen:
		{
			LOGGING(L"Object Pen with index: " << shObjectIndex)

			CEmfPlusPen *pEmfPlusPen = ReadPen();

			RegisterObject(pEmfPlusPen, shObjectIndex);

			break;
		}
		case ObjectTypePath:
		{
			LOGGING(L"Object Path with index: " << shObjectIndex)

			if ((unShFlags >>(15)) & 1)
			{
				CEmfPlusBuffer *pPathBuffer = GetBuffer(shObjectIndex);
				bool bRegister = true;

				if (NULL == pPathBuffer)
					pPathBuffer = new CEmfPlusBuffer();
				else
					bRegister = false;

				ReadBufferPath(pPathBuffer);

				if (bRegister)
					RegisterObject(pPathBuffer, shObjectIndex);

				if (0 == pPathBuffer->GetUnreadSize())
				{
					CDataStream oTempStream = m_oStream;

					BYTE* pTempPathBuffer;
					unsigned int unPathBufferSize;
					pPathBuffer->GetData(pTempPathBuffer, unPathBufferSize);

					m_oStream.SetStream(pTempPathBuffer, unPathBufferSize);

					CEmfPlusPath* pPath = ReadPath();
					RegisterObject(pPath, shObjectIndex);

					m_oStream = oTempStream;
				}
			}
			else
			{
				CEmfPlusPath* pPath = ReadPath();
				RegisterObject(pPath, shObjectIndex);
			}

			break;
		}
		case ObjectTypeRegion:
		{
			LOGGING(L"Object Region")

			CEmfPlusRegion *pEmfPlusRegion = ReadRegion();

			RegisterObject(pEmfPlusRegion, shObjectIndex);

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
			LOGGING(L"Object Font with index: " << shObjectIndex)

			CEmfPlusFont *pFont = ReadFont();

			RegisterObject(pFont, shObjectIndex);

			break;
		}
		case ObjectTypeStringFormat:
		{
			LOGGING(L"Object String Format")

			CEmfPlusStringFormat *pStringFormat = new CEmfPlusStringFormat;

			m_oStream >> *pStringFormat;

			RegisterObject(pStringFormat, shObjectIndex);

			break;
		}
		case ObjectTypeImageAttributes:
		{
			LOGGING(L"Object Image Attributes")

			CEmfPlusImageAttributes *pImageAttributes = new CEmfPlusImageAttributes();

			if (NULL != pImageAttributes)
			{
				m_oStream >> *pImageAttributes;
				RegisterObject(pImageAttributes, shObjectIndex);
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

		m_oPlayer.RestoreDC(unStackIndex);
		m_pDC = m_oPlayer.GetDC();
		UpdateOutputDC();
	}

	void CEmfPlusParser::Read_EMFPLUS_SAVE()
	{
		unsigned int unStackIndex;

		m_oStream >> unStackIndex;

		m_oPlayer.SaveDC(unStackIndex);
		UpdateOutputDC();
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

		m_pDC->MultiplyTransform(oMatrix, (unShFlags & 0x2000) ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
		UpdateOutputDC();

		m_bBanEmfProcessing = true;
	}

	void CEmfPlusParser::Read_EMFPLUS_RESETWORLDTRANSFORM()
	{
		m_pDC->ResetTransform();
		UpdateOutputDC();

		m_bBanEmfProcessing = true;
	}

	void CEmfPlusParser::Read_EMFPLUS_ROTATEWORLDTRANSFORM(unsigned short unShFlags)
	{
		double dAngle;

		m_oStream >> dAngle;

		dAngle *= M_PI / 180;

		double dCosTheta = cos(dAngle);
		double dSinTheta = sin(dAngle);

		TEmfPlusXForm oMatrix(dCosTheta, dSinTheta, -dSinTheta, dCosTheta, 0, 0);

		m_pDC->MultiplyTransform(oMatrix, (unShFlags & 0x2000) ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
		UpdateOutputDC();

		m_bBanEmfProcessing = true;
	}

	void CEmfPlusParser::Read_EMFPLUS_SCALEWORLDTRANSFORM(unsigned short unShFlags)
	{
		double dSx, dSy;

		m_oStream >> dSx;
		m_oStream >> dSy;

		TEmfPlusXForm oMatrix(dSx, 0, 0, dSy, 0, 0);

		m_pDC->MultiplyTransform(oMatrix, (unShFlags & 0x2000) ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
		UpdateOutputDC();

		m_bBanEmfProcessing = true;
	}

	void CEmfPlusParser::Read_EMFPLUS_SETPAGETRANSFORM(unsigned short unShFlags)
	{
		short shPageUnit = ExpressValue(unShFlags, 0, 7);

		m_oStream >> m_dUnitKoef;

		switch (shPageUnit)
		{
			case UnitTypePixel:
			case UnitTypeWorld:
			case UnitTypeDisplay:
			default: break;
			case UnitTypePoint:      m_dUnitKoef *= m_unLogicalDpiX * 72.f;  break;
			case UnitTypeInch:       m_dUnitKoef *= m_unLogicalDpiX;         break;
			case UnitTypeDocument:   m_dUnitKoef *= m_unLogicalDpiX / 300.f; break;
			case UnitTypeMillimeter: m_dUnitKoef *= m_unLogicalDpiX / 25.4f; break;
		}

		TXForm oMatrix(m_dUnitKoef, 0, 0, m_dUnitKoef, 0, 0);

		m_pDC->MultiplyTransform(oMatrix, MWT_LEFTMULTIPLY);
		UpdateOutputDC();

		m_bBanEmfProcessing = true;
	}

	void CEmfPlusParser::Read_EMFPLUS_SETWORLDTRANSFORM()
	{
		TEmfPlusXForm oMatrix;

		m_oStream >> oMatrix;

		UpdateMatrix(oMatrix);

		m_pDC->MultiplyTransform(oMatrix, MWT_SET);
		UpdateOutputDC();

		m_bBanEmfProcessing = true;
	}

	void CEmfPlusParser::Read_EMFPLUS_TRANSLATEWORLDTRANSFORM(unsigned short unShFlags)
	{
		double dX, dY;

		m_oStream >> dX;
		m_oStream >> dY;

		TEmfPlusXForm oMatrix(1, 0, 0, 1, dX, dY);

		m_pDC->MultiplyTransform(oMatrix, (unShFlags & 0x2000) ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
		UpdateOutputDC();

		m_bBanEmfProcessing = true;
	}

	void CEmfPlusParser::Read_EMFPLUS_ENDOFFILE()
	{
		if(NULL != m_pInterpretator)
			m_pInterpretator->End();

		m_bEof = true;
		m_bBanEmfProcessing = false;

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_ENDOFFILE();
	}

	void CEmfPlusParser::Read_EMFPLUS_GETDC()
	{
		m_bBanEmfProcessing = false;

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_GETDC();
	}

	void CEmfPlusParser::Read_EMRPLUS_OFFSETCLIP()
	{
		m_bBanEmfProcessing = true;

		double dX, dY;

		m_oStream >> dX;
		m_oStream >> dY;

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_OFFSETCLIP(dX, dY);

		//TODO: реализовать
	}

	void CEmfPlusParser::Read_EMRPLUS_RESETCLIP()
	{
		m_bBanEmfProcessing = true;

		m_pDC->GetClip()->Reset();
		UpdateOutputDC();

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_RESETCLIP();
	}

	void CEmfPlusParser::Read_EMFPLUS_SETCLIPPATH(unsigned short unShFlags)
	{
		m_bBanEmfProcessing = true;

		BYTE uchObjectId = ExpressValue(unShFlags, 0, 7);

		CEmfPlusPath* pPath = GetPath(uchObjectId);

		if (NULL == pPath) return;

		BYTE uchCM = ExpressValue(unShFlags, 8, 11);

		m_pDC->GetClip()->Reset();
		m_pDC->GetClip()->SetPath(*pPath, uchCM, *GetTransform());
		UpdateOutputDC();

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_SETCLIPPATH(unShFlags, pPath);
	}

	void CEmfPlusParser::Read_EMFPLUS_SETCLIPRECT(unsigned short unShFlags)
	{
		m_bBanEmfProcessing = true;

		short shCM = ExpressValue(unShFlags, 8, 11);
		TEmfPlusRectF oRect;

		m_oStream >> oRect;

		m_pDC->GetClip()->Reset();
		CombineClip(oRect.ToRectD(), shCM);
		UpdateOutputDC();

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_SETCLIPRECT(shCM, oRect);
	}

	void CEmfPlusParser::Read_EMFPLUS_SETCLIPREGION(unsigned short unShFlags)
	{
		m_bBanEmfProcessing = true;

		short shObjectIndex = ExpressValue(unShFlags, 0, 7);
		short shCM = ExpressValue(unShFlags, 8, 11);

		CEmfPlusRegion *pRegion = GetRegion(shObjectIndex);

		if (NULL != pRegion)
		{
			m_pDC->GetClip()->Reset();

			for (const CEmfPlusRegionNode* pNode : pRegion->arNodes)
			{
				switch(pNode->GetNodeType())
				{
					case EmfPLusRegionNodeTypeEmpty:
					{
						break;
					}
					case EmfPLusRegionNodeTypePath:
					{
						CEmfPlusRegionNodePath* pNodeRegionPath = (CEmfPlusRegionNodePath*)pNode;

						if (!pNodeRegionPath->Empty())
							m_pDC->GetClip()->SetPath(*pNodeRegionPath->GetPath(), shCM, *GetTransform());

						break;
					}
					case EmfPLusRegionNodeTypeRectF:
					{
						CEmfPlusRegionNodeRectF* pNodeRegionRectF = (CEmfPlusRegionNodeRectF*)pNode;

						if (!pNodeRegionRectF->Empty())
							CombineClip(pNodeRegionRectF->GetRect()->ToRectD(), shCM);

						break;
					}
					case EmfPLusRegionNodeTypeChild:
					{
						CEmfPlusRegionNodeChild* pNodeRegionChild = (CEmfPlusRegionNodeChild*)pNode;

						pNodeRegionChild->DrawOnClip(*m_pDC->GetClip(), *GetTransform(), GetDCBounds());

						break;
					}
				}
			}
		}

		UpdateOutputDC();

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_SETCLIPREGION(shObjectIndex, shCM, pRegion);
	}

	void CEmfPlusParser::Read_EMFPLUS_COMMENT()
	{
		m_oStream.Skip(m_ulRecordSize);

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMFPLUS_COMMENT(m_oStream, m_ulRecordSize);
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
		if (arPoints.empty())
			return;

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
		if (arPoints.empty())
			return {};

		std::vector<TEmfPlusPointF> arConvertdPoints(arPoints.size());

		if (typeid (TEmfPlusPoint) == typeid (T))
		{
			for (unsigned int unIndex = 0; unIndex < arPoints.size(); ++unIndex)
				arConvertdPoints[unIndex].Set((TEmfPlusPoint&)arPoints[unIndex]);
			
			return arConvertdPoints;
		}
		else if (typeid (TEmfPlusPointF) == typeid (T))
		{
			for (unsigned int unIndex = 0; unIndex < arPoints.size(); ++unIndex)
				arConvertdPoints[unIndex] = reinterpret_cast<TEmfPlusPointF&>(arPoints[unIndex]);

			return arConvertdPoints;
		}

		return arConvertdPoints;
	}

	template<typename T>
	TEmfPlusRectF CEmfPlusParser::GetConvertedRectangle(T oRectangle)
	{
		if (typeid (TEmfPlusRectF) == typeid (T))
			return (TEmfPlusRectF&)oRectangle;
		else if (typeid (TEmfPlusRect) == typeid (T))
			return TEmfPlusRectF((TEmfPlusRect&)oRectangle);
		else return TEmfPlusRectF();
	}
}
