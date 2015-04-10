#ifndef _EMF_FILE_H
#define _EMF_FILE_H

#include "../Wmf/WmfUtils.h"
#include "../Wmf/WmfTypes.h"

#include "../Common.h"

#include "EmfTypes.h"
#include "EmfOutputDevice.h"
#include "EmfPlayer.h"

#include "../../../fontengine/FontManager.h"

namespace Metafile
{
	class CEmfFile
	{
	public:

		CEmfFile() : m_oPlayer(this)
		{
			m_pBufferData = NULL;
			m_bError      = false;
			m_pOutput     = NULL;

			m_oStream.SetStream(NULL, 0);

			m_pDC = m_oPlayer.GetDC();
		};

		~CEmfFile()
		{

		};

		bool OpenFromFile(const wchar_t* wsFilePath)
		{
			if (m_pBufferData)
				delete m_pBufferData;

			NSFile::CFileBinary oFile;
			oFile.OpenFile(wsFilePath);
			long lFileSize = oFile.GetFileSize();

			m_pBufferData = new BYTE[lFileSize];
			if (!m_pBufferData)
				return false;

			DWORD lReadedSize;
			oFile.ReadFile(m_pBufferData, lFileSize, lReadedSize);

			m_oStream.SetStream(m_pBufferData, lFileSize);

			return true;
		}
		void Close()
		{
			if (m_pBufferData)
				delete m_pBufferData;

			m_pOutput = NULL;
			m_oStream.SetStream(NULL, 0);
			m_bError = false;

			m_oPlayer.Clear();
			m_pDC = m_oPlayer.GetDC();
		}
		TEmfRectL GetBounds()
		{
			return m_oHeader.oFrame;
		}
		void SetOutputDevice(CEmfOutputDevice* pOutput)
		{
			m_pOutput = pOutput;
		}
		void Scan()
		{
			Read_EMR_HEADER();
		}

		bool CheckError()
		{
			return m_bError;
		}

		void SetFontManager(CFontManager* pManager)
		{
			m_pFontManager = pManager;
		}

		void PlayMetaFile()
		{
			unsigned long ulSize, ulType;
			unsigned long ulNumber = 0;

			bool bEof = false;

			do
			{
				m_oStream >> ulType;
				m_oStream >> ulSize;

				m_ulRecordSize = ulSize - 8;

				switch (ulType)
				{
					case EMR_STRETCHDIBITS:
					{
						Read_EMR_STRETCHDIBITS();
						break;
					}
					//-----------------------------------------------------------
					// 2.3.5 Drawing
					//-----------------------------------------------------------
					case EMR_EXTTEXTOUTW:
					{
						Read_EMR_EXTTEXTOUTW();
						break;
					}
					//-----------------------------------------------------------
					// 2.3.7 Object Creation
					//-----------------------------------------------------------
					case EMR_CREATEBRUSHINDIRECT:
					{
						Read_EMR_CREATEBRUSHINDIRECT();
						break;
					}
					//-----------------------------------------------------------
					// 2.3.11 State
					//-----------------------------------------------------------
					case EMR_SAVEDC:
					{
						Read_EMR_SAVEDC();
						break;
					}
					case EMR_RESTOREDC:
					{
						Read_EMR_RESTOREDC();
						break;
					}
					case EMR_SETTEXTCOLOR:
					{
						Read_EMR_SETTEXTCOLOR();
						break;
					}
					//-----------------------------------------------------------
					// 2.3.12 Transform
					//-----------------------------------------------------------
					case EMR_SETWORLDTRANSFORM:
					{
						Read_EMR_SETWORLDTRANSFORM();
						break;
					}
					case EMR_MODIFYWORLDTRANSFORM:
					{
						Read_EMR_MODIFYWORLDTRANSFORM();
						break;
					}


					case EMR_EOF:
					{
						Read_EMR_EOF();
						bEof = true;
						break;
					}
					case EMR_SETICMMODE:
					default:
					{						
						Read_EMR_UNKNOWN();
						break;
					}
				}

				if (bEof)
					break;

				if (!m_oStream.IsValid())
					SetError();

			} while (!CheckError());
		
		}

	private:

		void SetError()
		{
			m_bError = true;
		}

		void Read_EMR_HEADER()
		{
			unsigned long ulType, ulSize;

			m_oStream >> ulType;
			m_oStream >> ulSize;

			if (EMR_HEADER != ulType)
				return SetError();

			m_oStream >> m_oHeader.oBounds;
			m_oStream >> m_oHeader.oFrame;
			m_oStream >> m_oHeader.ulSignature;
			m_oStream >> m_oHeader.ulVersion;
			m_oStream >> m_oHeader.ulSize;
			m_oStream >> m_oHeader.ulRecords;
			m_oStream >> m_oHeader.ushObjects;
			m_oStream >> m_oHeader.ushReserved;
			m_oStream >> m_oHeader.ulSizeDescription;
			m_oStream >> m_oHeader.ulOffsetDescription;
			m_oStream >> m_oHeader.ulPalEntries;
			m_oStream >> m_oHeader.oDevice;
			m_oStream >> m_oHeader.oMillimeters;

			if (ENHMETA_SIGNATURE != m_oHeader.ulSignature || 0x00010000 != m_oHeader.ulVersion)
				return SetError();

			// Пропускаем остальную часть заголовка, т.к. она нас пока не интересует
			unsigned long ulRemaining = ulSize - 88; // 8 + sizeof(TEmfHeader)
			m_oStream.Skip(ulRemaining);
		}
		void Read_EMR_STRETCHDIBITS()
		{
			TEmfStretchDIBITS oBitmap;

			m_oStream >> oBitmap.Bounds;
			m_oStream >> oBitmap.xDest;
			m_oStream >> oBitmap.yDest;
			m_oStream >> oBitmap.xSrc;
			m_oStream >> oBitmap.ySrc;
			m_oStream >> oBitmap.cxSrc;
			m_oStream >> oBitmap.cySrc;
			m_oStream >> oBitmap.offBmiSrc;
			m_oStream >> oBitmap.cbBmiSrc;
			m_oStream >> oBitmap.offBitsSrc;
			m_oStream >> oBitmap.cbBitsSrc;
			m_oStream >> oBitmap.UsageSrc;
			m_oStream >> oBitmap.BitBltRasterOperation;
			m_oStream >> oBitmap.cxDest;
			m_oStream >> oBitmap.cyDest;

			unsigned long ulHeaderOffset = oBitmap.offBmiSrc - sizeof(TEmfStretchDIBITS) - 8;
			unsigned long ulHeaderSize   = oBitmap.cbBmiSrc;
			m_oStream.Skip(ulHeaderOffset);

			BYTE* pHeaderBuffer = new BYTE[ulHeaderSize];
			if (!pHeaderBuffer)
				return SetError();

			m_oStream.ReadBytes(pHeaderBuffer, ulHeaderSize);

			unsigned long ulBitsOffset = oBitmap.offBitsSrc - oBitmap.offBmiSrc - oBitmap.cbBmiSrc;
			unsigned long ulBitsSize   = oBitmap.cbBitsSrc;
			m_oStream.Skip(ulBitsOffset);
			BYTE* pBitsBuffer = new BYTE[ulBitsSize];
			if (!pBitsBuffer)
			{
				delete[] pHeaderBuffer;
				return SetError();
			}
			m_oStream.ReadBytes(pBitsBuffer, ulBitsSize);

			BYTE* pBgraBuffer;
			unsigned long ulWidth, ulHeight;
			ReadImage(pHeaderBuffer, ulHeaderSize, pBitsBuffer, ulBitsSize, &pBgraBuffer, &ulWidth, &ulHeight);

			if (m_pOutput)
				m_pOutput->Draw_Bitmap(oBitmap.xDest, oBitmap.yDest, oBitmap.cxDest, oBitmap.cyDest, pBgraBuffer, ulWidth, ulHeight);

			if (pBgraBuffer)
				delete[] pBgraBuffer;

			delete[] pBitsBuffer;
			delete[] pHeaderBuffer;
		}
		void Read_EMR_EOF()
		{
			unsigned long ulCount, ulOffset, ulSizeLast;

			m_oStream >> ulCount;
			m_oStream >> ulOffset;

			m_oStream.Skip(m_ulRecordSize - 8 - 4);

			m_oStream >> ulSizeLast;
		}
		void Read_EMR_UNKNOWN()
		{
			// Неизвестные и нереализованные записи мы пропускаем
			m_oStream.Skip(m_ulRecordSize);
		}
		void Read_EMR_SAVEDC()
		{
			m_pDC = m_oPlayer.SaveDC();
		}
		void Read_EMR_RESTOREDC()
		{
			long lSavedDC;
			m_oStream >> lSavedDC;

			if (lSavedDC >= 0)
			{
				SetError();
				return;
			}

			long lCount = -lSavedDC;
			for (long lIndex = 0; lIndex < lCount; lIndex++)
				m_oPlayer.RestoreDC();

			m_pDC = m_oPlayer.GetDC();
		}
		void Read_EMR_MODIFYWORLDTRANSFORM()
		{
			TEmfXForm oXForm;

			m_oStream >> oXForm.M11;
			m_oStream >> oXForm.M12;
			m_oStream >> oXForm.M21;
			m_oStream >> oXForm.M22;
			m_oStream >> oXForm.Dx;
			m_oStream >> oXForm.Dy;

			unsigned long ulMode;
			m_oStream >> ulMode;

			TEmfXForm* pCurTransform = m_pDC->GetTransform();
			pCurTransform->Multiply(oXForm, ulMode);
		}
		void Read_EMR_SETWORLDTRANSFORM()
		{
			TEmfXForm oXForm;

			m_oStream >> oXForm.M11;
			m_oStream >> oXForm.M12;
			m_oStream >> oXForm.M21;
			m_oStream >> oXForm.M22;
			m_oStream >> oXForm.Dx;
			m_oStream >> oXForm.Dy;

			TEmfXForm* pCurTransform = m_pDC->GetTransform();
			pCurTransform->Multiply(oXForm, MWT_SET);
		}
		void Read_EMR_CREATEBRUSHINDIRECT()
		{
			unsigned long ulBrushIndex;
			CEmfLogBrushEx* pBrush = new CEmfLogBrushEx();

			m_oStream >> ulBrushIndex;
			m_oStream >> *pBrush;

			m_oPlayer.RegisterObject(ulBrushIndex, (CEmfObjectBase*)pBrush);
		}
		void Read_EMR_SETTEXTCOLOR()
		{
			TEmfColor oColor;
			m_oStream >> oColor;

			m_pDC->SetTextColor(oColor);
		}
		void Read_EMR_EXTTEXTOUTW()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;

			unsigned long ulGrMode;
			double dExScale, dEyScale;

			m_oStream >> ulGrMode;
			m_oStream >> dExScale;
			m_oStream >> dEyScale;

			// EmrText
			
		}

	private:

		CDataStream       m_oStream;
		BYTE*             m_pBufferData;
		bool              m_bError;
		CFontManager*     m_pFontManager;
		TEmfHeader        m_oHeader;

		unsigned long     m_ulRecordSize;
		CEmfOutputDevice* m_pOutput;

		CEmfDC*           m_pDC;
		CEmfPlayer        m_oPlayer;

		friend class CEmfRendererOutput;
		friend class CEmfPlayer;
	};
}

#endif // _EMF_FILE_H