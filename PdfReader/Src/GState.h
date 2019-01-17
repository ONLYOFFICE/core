/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _PDF_READER_GSTATE_H
#define _PDF_READER_GSTATE_H

#include "../../DesktopEditor/common/Types.h"
#include "Object.h"
#include "Function.h"
#include <math.h>

namespace PdfReader
{
	class Array;
	class GrFont;
	class PDFRectangle;
	class GrShading;

	//-------------------------------------------------------------------------------------------------------------------------------
	// GraphicsBlendMode
	//-------------------------------------------------------------------------------------------------------------------------------

	enum GraphicsBlendMode
	{
		grBlendNormal,
		grBlendMultiply,
		grBlendScreen,
		grBlendOverlay,
		grBlendDarken,
		grBlendLighten,
		grBlendColorDodge,
		grBlendColorBurn,
		grBlendHardLight,
		grBlendSoftLight,
		grBlendDifference,
		grBlendExclusion,
		grBlendHue,
		grBlendSaturation,
		grBlendColor,
		grBlendLuminosity
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrColorComp
	//-------------------------------------------------------------------------------------------------------------------------------

	// 16.16 
	typedef int GrColorComp;

#define GrColorComp1 0x10000

	static inline GrColorComp   DoubleToColor(double dValue)
	{
		return (GrColorComp)(dValue * GrColorComp1);
	}

	static inline double        ColorToDouble(GrColorComp nColor)
	{
		return (double)nColor / (double)GrColorComp1;
	}

	static inline GrColorComp   ByteToColor(unsigned char nByte)
	{
		// (nByte / 255) << 16  =  (0.0000000100000001... * nByte) << 16
		//                      =  ((nByte << 8) + (nByte) + (nByte >> 8) + ...) << 16
		//                      =   (nByte << 8) + (nByte) + (nByte >> 7)
		//                                      [для округления]
		return (GrColorComp)((nByte << 8) + nByte + (nByte >> 7));
	}

	static inline unsigned char ColorToByte(GrColorComp nColor)
	{
		// 255 * nColor + 0.5  =  256 * nColor - nColor + 0x8000
		return (unsigned char)(((nColor << 8) - nColor + 0x8000) >> 16);
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrColor
	//-------------------------------------------------------------------------------------------------------------------------------

#define GrColorMaxComps funcMaxOutputs

	struct GrColor
	{
		GrColorComp arrComp[GrColorMaxComps];
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrGray
	//-------------------------------------------------------------------------------------------------------------------------------

	typedef GrColorComp GrGray;

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrRGB
	//-------------------------------------------------------------------------------------------------------------------------------

	struct GrRGB
	{
		GrColorComp r, g, b;
	};

	//------------------------------------------------------------------------
	// GrCMYK
	//------------------------------------------------------------------------

	struct GrCMYK
	{
		GrColorComp c, m, y, k;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	// Константы GrColorSpaceModes и массив GrColorSpaceModeNames, определенный
	// в GState.cpp должны совпадать.
	enum GrColorSpaceMode
	{
		csDeviceGray,
		csCalGray,
		csDeviceRGB,
		csCalRGB,
		csDeviceCMYK,
		csLab,
		csICCBased,
		csIndexed,
		csSeparation,
		csDeviceN,
		csPattern
	};

	class GrColorSpace
	{
	public:

		GrColorSpace();
		virtual ~GrColorSpace();
		virtual GrColorSpace *Copy() = 0;
		virtual GrColorSpaceMode GetMode() = 0;

		static GrColorSpace *Parse(Object *pColorSpaceObject);

		// Конвертируем в -> Gray, RGB, или CMYK.
		virtual void GetGray(GrColor *pColor, GrGray *pGray) = 0;
		virtual void GetRGB(GrColor *pColor, GrRGB  *pRGB)  = 0;
		virtual void GetCMYK(GrColor *pColor, GrCMYK *pCMYK) = 0;

		// Количество компонент.
		virtual int  GetComponentsCount() = 0;

		// Получаем стандартное значение цвета в данном цветовом пространстве.
		virtual void GetDefaultColor(GrColor *pColor) = 0;

		// Стандартные границчные значения для каждой компоненты пикселя, где nMaxImagePixelValue - максимальное значение пикселя.
		virtual void GetDefaultRanges(double *pDecodeLow, double *pDecodeRange, int nMaxImagePixelValue);

		// True, если в данном цветовом пространстве на странице ничего не нарисовано.
		virtual bool IsNonMarking()
		{
			return false;
		}

		// Число различных цветовых пространств.
		static int GetColorSpaceModesCount();

		// Имя соответствующего цветового пространства.
		static char *GetColorSpaceModeName(int nIndex);

		DWORD GetDwordColor(GrColor *pColor)
		{
			GrRGB oRGB;
			GetRGB(pColor, &oRGB);
			unsigned char nR = ColorToByte(oRGB.r);
			unsigned char nG = ColorToByte(oRGB.g);
			unsigned char nB = ColorToByte(oRGB.b);
			return ((nB << 16) | (nG << 8) | nR);
		}

	private:
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrDeviceGrayColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrDeviceGrayColorSpace : public GrColorSpace
	{
	public:

		GrDeviceGrayColorSpace();
		virtual ~GrDeviceGrayColorSpace();
		virtual GrColorSpace *Copy();
		virtual GrColorSpaceMode GetMode()
		{
			return csDeviceGray;
		}

		virtual void GetGray(GrColor *pColor, GrGray *pGray);
		virtual void GetRGB(GrColor *pColor, GrRGB  *pRGB);
		virtual void GetCMYK(GrColor *pColor, GrCMYK *pCMYK);

		virtual int  GetComponentsCount()
		{
			return 1;
		}
		virtual void GetDefaultColor(GrColor *pColor);

	private:
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrCalGrayColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrCalGrayColorSpace : public GrColorSpace
	{
	public:

		GrCalGrayColorSpace();
		virtual ~GrCalGrayColorSpace();
		virtual GrColorSpace *Copy();
		virtual GrColorSpaceMode GetMode()
		{
			return csCalGray;
		}

		static GrColorSpace *Parse(Array *pArray);

		virtual void GetGray(GrColor *pColor, GrGray *pGray);
		virtual void GetRGB(GrColor *pColor, GrRGB  *pRGB);
		virtual void GetCMYK(GrColor *pColor, GrCMYK *pCMYK);

		virtual int GetComponentsCount()
		{
			return 1;
		}
		virtual void GetDefaultColor(GrColor *pColor);

		// Возвращаем значения, специфичные пространству CalGray.
		double GetWhiteX()
		{
			return m_dWhiteX;
		}
		double GetWhiteY()
		{
			return m_dWhiteY;
		}
		double GetWhiteZ()
		{
			return m_dWhiteZ;
		}
		double GetBlackX()
		{
			return m_dBlackX;
		}
		double GetBlackY()
		{
			return m_dBlackY;
		}
		double GetBlackZ()
		{
			return m_dBlackZ;
		}
		double GetGamma()
		{
			return m_dGamma;
		}

	private:

		double m_dWhiteX;   //
		double m_dWhiteY;   // White point
		double m_dWhiteZ;   //

		double m_dBlackX;   //
		double m_dBlackY;   // Black point
		double m_dBlackZ;   // 

		double m_dGamma;    // Gamma
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrDeviceRGBColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrDeviceRGBColorSpace : public GrColorSpace
	{
	public:

		GrDeviceRGBColorSpace();
		virtual ~GrDeviceRGBColorSpace();
		virtual GrColorSpace *Copy();
		virtual GrColorSpaceMode GetMode()
		{
			return csDeviceRGB;
		}


		virtual void GetGray(GrColor *pColor, GrGray *pGray);
		virtual void GetRGB(GrColor *pColor, GrRGB  *pRGB);
		virtual void GetCMYK(GrColor *pColor, GrCMYK *pCMYK);

		virtual int  GetComponentsCount()
		{
			return 3;
		}
		virtual void GetDefaultColor(GrColor *pColor);

	private:
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrCalRGBColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrCalRGBColorSpace : public GrColorSpace
	{
	public:

		GrCalRGBColorSpace();
		virtual ~GrCalRGBColorSpace();
		virtual GrColorSpace *Copy();
		virtual GrColorSpaceMode GetMode()
		{
			return csCalRGB;
		}

		static GrColorSpace *Parse(Array *pArray);

		virtual void GetGray(GrColor *pColor, GrGray *pGray);
		virtual void GetRGB(GrColor *pColor, GrRGB  *pRGB);
		virtual void GetCMYK(GrColor *pColor, GrCMYK *pCMYK);

		virtual int GetComponentsCount()
		{
			return 3;
		}
		virtual void GetDefaultColor(GrColor *pColor);

		// Возвращаем значения, специфичные пространству CalRBG.
		double  GetWhiteX()
		{
			return m_dWhiteX;
		}
		double  GetWhiteY()
		{
			return m_dWhiteY;
		}
		double  GetWhiteZ()
		{
			return m_dWhiteZ;
		}
		double  GetBlackX()
		{
			return m_dBlackX;
		}
		double  GetBlackY()
		{
			return m_dBlackY;
		}
		double  GetBlackZ()
		{
			return m_dBlackZ;
		}
		double  GetGammaR()
		{
			return m_dGammaR;
		}
		double  GetGammaG()
		{
			return m_dGammaG;
		}
		double  GetGammaB()
		{
			return m_dGammaB;
		}
		double *GetMatrix()
		{
			return m_arrdMatrix;
		}

	private:

		double m_dWhiteX;       //
		double m_dWhiteY;       // White point
		double m_dWhiteZ;       //

		double m_dBlackX;       //
		double m_dBlackY;       // Black point
		double m_dBlackZ;       //

		double m_dGammaR;       //
		double m_dGammaG;       // Gamma
		double m_dGammaB;       //

		double m_arrdMatrix[9]; // Матрица преобразования: ABC -> XYZ
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrDeviceCMYKColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrDeviceCMYKColorSpace : public GrColorSpace
	{
	public:

		GrDeviceCMYKColorSpace();
		virtual ~GrDeviceCMYKColorSpace();
		virtual GrColorSpace *Copy();
		virtual GrColorSpaceMode GetMode()
		{
			return csDeviceCMYK;
		}

		virtual void GetGray(GrColor *pColor, GrGray *pGray);
		virtual void GetRGB(GrColor *pColor, GrRGB  *pRGB);
		virtual void GetCMYK(GrColor *pColor, GrCMYK *pCMYK);

		virtual int  GetComponentsCount()
		{
			return 4;
		}
		virtual void GetDefaultColor(GrColor *pColor);

	private:
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrLabColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrLabColorSpace : public GrColorSpace
	{
	public:

		GrLabColorSpace();
		virtual ~GrLabColorSpace();
		virtual GrColorSpace *Copy();
		virtual GrColorSpaceMode GetMode()
		{
			return csLab;
		}

		static GrColorSpace *Parse(Array *pArray);

		virtual void GetGray(GrColor *pColor, GrGray *pGray);
		virtual void GetRGB(GrColor *pColor, GrRGB  *pRGB);
		virtual void GetCMYK(GrColor *pColor, GrCMYK *pCMYK);

		virtual int  GetComponentsCount()
		{
			return 3;
		}
		virtual void GetDefaultColor(GrColor *pColor);

		virtual void GetDefaultRanges(double *pDecodeLow, double *pDecodeRange, int nMaxImagePixelValue);

		// Возвращаем значения, специфичные пространству Lab.
		double GetWhiteX()
		{
			return m_dWhiteX;
		}
		double GetWhiteY()
		{
			return m_dWhiteY;
		}
		double GetWhiteZ()
		{
			return m_dWhiteZ;
		}
		double GetBlackX()
		{
			return m_dBlackX;
		}
		double GetBlackY()
		{
			return m_dBlackY;
		}
		double GetBlackZ()
		{
			return m_dBlackZ;
		}
		double GetMinA()
		{
			return m_dMinA;
		}
		double GetMaxA()
		{
			return m_dMaxA;
		}
		double GetMinB()
		{
			return m_dMinB;
		}
		double GetMaxB()
		{
			return m_dMaxB;
		}

	private:

		double m_dWhiteX;  //
		double m_dWhiteY;  // White point
		double m_dWhiteZ;  //

		double m_dBlackX;  //
		double m_dBlackY;  // Black point
		double m_dBlackZ;  // 

		double m_dMinA;    //
		double m_dMaxA;    // Границы для компонент А и В
		double m_dMinB;    //
		double m_dMaxB;    //

		double m_dMultR;   // 
		double m_dMultG;   // Дополнительные сомножители
		double m_dMultB;   //
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrICCBasedColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrICCBasedColorSpace : public GrColorSpace
	{
	public:

		GrICCBasedColorSpace(int nCompontnsCount, GrColorSpace *pAlternate, Ref *pICCProfileStream);
		virtual ~GrICCBasedColorSpace();
		virtual GrColorSpace *Copy();
		virtual GrColorSpaceMode GetMode()
		{
			return csICCBased;
		}


		static GrColorSpace *Parse(Array *pArray);

		virtual void GetGray(GrColor *pColor, GrGray *pGray);
		virtual void GetRGB(GrColor *pColor, GrRGB  *pRGB);
		virtual void GetCMYK(GrColor *pColor, GrCMYK *pCMYK);

		virtual int  GetComponentsCount()
		{
			return m_nComponentsCount;
		}
		virtual void GetDefaultColor(GrColor *pColor);

		virtual void GetDefaultRanges(double *pDecodeLow, double *pDecodeRange, int nMaxImagePixelValue);

		// Возвращаем значения, специфичные пространству Lab.
		GrColorSpace *GetAlternate()
		{
			return m_pAlternate;
		}

	private:

		int           m_nComponentsCount;  // Число компонент цвета (1, 3, or 4)
		GrColorSpace *m_pAlternate;        // Альтернативное цветовое пространство
		double        m_arrdRangeMin[4];   // Минимальные значения для каждой компоненты
		double        m_arrdRangeMax[4];   // Максимальные значения для каждой компоненты
		Ref           m_oICCProfileStream; // ICC 
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrIndexedColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrIndexedColorSpace : public GrColorSpace
	{
	public:

		GrIndexedColorSpace(GrColorSpace *pBase, int nHival);
		virtual ~GrIndexedColorSpace();
		virtual GrColorSpace *Copy();
		virtual GrColorSpaceMode GetMode()
		{
			return csIndexed;
		}

		static  GrColorSpace *Parse(Array *pArray);

		virtual void GetGray(GrColor *pColor, GrGray *pGray);
		virtual void GetRGB(GrColor *pColor, GrRGB  *pRGB);
		virtual void GetCMYK(GrColor *pColor, GrCMYK *pCMYK);

		virtual int  GetComponentsCount()
		{
			return 1;
		}
		virtual void GetDefaultColor(GrColor *pColor);

		virtual void GetDefaultRanges(double *pDecodeLow, double *pDecodeRange, int nMaxImagePixelValue);

		// Возвращаем значения, специфичные пространству Indexed.
		GrColorSpace  *GetBase()
		{
			return m_pBase;
		}
		int            GetHival()
		{
			return m_nHival;
		}
		unsigned char *GetLookup()
		{
			return m_pLookup;
		}

	private:

		GrColor *MapColorToBase(GrColor *pColor, GrColor *pBaseColor);

	private:

		GrColorSpace  *m_pBase;   // Базовое цветовое пространство
		int            m_nHival;  // Hival = max valid index value
		unsigned char *m_pLookup; // Таблица цветов
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrSeparationColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrSeparationColorSpace : public GrColorSpace
	{
	public:

		GrSeparationColorSpace(StringExt *seName, GrColorSpace *pAlternate, Function *pFunction);
		virtual ~GrSeparationColorSpace();
		virtual GrColorSpace *Copy();
		virtual GrColorSpaceMode GetMode()
		{
			return csSeparation;
		}

		static  GrColorSpace *Parse(Array *pArray);

		virtual void GetGray(GrColor *pColor, GrGray *pGray);
		virtual void GetRGB(GrColor *pColor, GrRGB  *pRGB);
		virtual void GetCMYK(GrColor *pColor, GrCMYK *pCMYK);

		virtual int  GetComponentsCount()
		{
			return 1;
		}
		virtual void GetDefaultColor(GrColor *pColor);

		virtual bool IsNonMarking()
		{
			return m_bNonMarking;
		}

		// Возвращаем значения, специфичные пространству Separation.
		StringExt    *GetName()
		{
			return m_seName;
		}
		GrColorSpace *GetAlternateSpace()
		{
			return m_pAlternateSpace;
		}
		Function     *GetTransformFunction()
		{
			return m_pFunction;
		}

	private:

		StringExt    *m_seName;          // Color space family name
		GrColorSpace *m_pAlternateSpace; // Альтернативное цветовое пространство
		Function     *m_pFunction;	     // tintTransform ( функция преобразования в альтернативное пространство)

		bool          m_bNonMarking;     // Будет ли видимой графика, использующая данное цветовое пространство
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrDeviceNColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrDeviceNColorSpace : public GrColorSpace
	{
	public:

		GrDeviceNColorSpace(int nComponentsCount, GrColorSpace *pAlternate, Function *pFunction);
		virtual ~GrDeviceNColorSpace();
		virtual GrColorSpace *Copy();
		virtual GrColorSpaceMode GetMode()
		{
			return csDeviceN;
		}


		static  GrColorSpace *Parse(Array *pArray);

		virtual void GetGray(GrColor *pColor, GrGray *pGray);
		virtual void GetRGB(GrColor *pColor, GrRGB  *pRGB);
		virtual void GetCMYK(GrColor *pColor, GrCMYK *pCMYK);

		virtual int  GetComponentsCount()
		{
			return m_nComponentsCount;
		}
		virtual void GetDefaultColor(GrColor *pColor);

		virtual bool IsNonMarking()
		{
			return m_bNonMarking;
		}

		// Возвращаем значения, специфичные пространству DeviceN.
		StringExt    *GetColorantName(int nIndex)
		{
			return m_arrseNames[nIndex];
		}
		GrColorSpace *GetAlternateColor()
		{
			return m_pAlternateSpace;
		}
		Function     *GetTransformFunction()
		{
			return m_pFunction;
		}

	private:

		int           m_nComponentsCount;            // Число компонент
		StringExt    *m_arrseNames[GrColorMaxComps]; // Объекты типа "Name" определяющие каждую цветовую компоненту
		GrColorSpace *m_pAlternateSpace;             // Альтернативное цветовое пространство
		Function     *m_pFunction;	                 // tintTransform ( функция преобразования в альтернативное пространство)

		bool          m_bNonMarking;                 // Будет ли видимой графика, использующая данное цветовое пространство
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrPatternColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrPatternColorSpace : public GrColorSpace
	{
	public:

		GrPatternColorSpace(GrColorSpace *pUnder);
		virtual ~GrPatternColorSpace();
		virtual GrColorSpace *Copy();
		virtual GrColorSpaceMode GetMode()
		{
			return csPattern;
		}


		static  GrColorSpace *Parse(Array *pArray);

		virtual void GetGray(GrColor *pColor, GrGray *pGray);
		virtual void GetRGB(GrColor *pColor, GrRGB  *pRGB);
		virtual void GetCMYK(GrColor *pColor, GrCMYK *pCMYK);

		virtual int  GetComponentsCount()
		{
			return 0;
		}
		virtual void GetDefaultColor(GrColor *pColor);

		// Возвращаем значения, специфичные пространству Pattern.
		GrColorSpace *GetUnder()
		{
			return m_pUnder;
		}

	private:

		GrColorSpace *m_pUnder;    // Цветовое пространство подкладки (для не цветовых Patterns)
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrPattern
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrPattern
	{
	public:

		GrPattern(int nType);
		virtual ~GrPattern();

		static  GrPattern *Parse(Object *pObject);

		virtual GrPattern *Copy() = 0;

		int GetType()
		{
			return m_nType;
		}

	private:

		int m_nType;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrTilingPattern
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrTilingPattern : public GrPattern
	{
	public:

		static GrTilingPattern *Parse(Object *pPatternObject);
		virtual ~GrTilingPattern();

		virtual GrPattern *Copy();

		int     GetPaintType()
		{
			return m_nPaintType;
		}
		int     GetTilingType()
		{
			return m_nTilingType;
		}
		double *GetBBox()
		{
			return m_arrBBox;
		}
		double  GetXStep()
		{
			return m_dXStep;
		}
		double  GetYStep()
		{
			return m_dYStep;
		}
		Dict   *GetResourcesDict()
		{
			return m_oResources.IsDict() ? m_oResources.GetDict() : (Dict *)NULL;
		}
		double *GetMatrix()
		{
			return m_arrMatrix;
		}
		Object *GetContentStream()
		{
			return &m_oContentStream;
		}

	private:

		GrTilingPattern(int nPaintType, int nTilingType, double *pBBox, double dXStep, double dYStep, Object *pResources, double *pMatrix, Object *pContentStream);

	private:

		int    m_nPaintType;
		int    m_nTilingType;
		double m_arrBBox[4];
		double m_dXStep;
		double m_dYStep;
		Object m_oResources;
		double m_arrMatrix[6];

		Object m_oContentStream;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrShadingPattern
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrShadingPattern : public GrPattern
	{
	public:

		static GrShadingPattern *Parse(Object *pPatternObject);
		virtual ~GrShadingPattern();

		virtual GrPattern *Copy();

		GrShading *GetShading()
		{
			return m_pShading;
		}
		double     *GetMatrix()
		{
			return m_arrMatrix;
		}

	private:

		GrShadingPattern(GrShading *pShading, double *pMatrix);

	private:

		GrShading *m_pShading;      // Сам объект Shading
		double     m_arrMatrix[6];  // Pattern matrix
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrShading
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrShading
	{
	public:

		GrShading(int nType);
		GrShading(GrShading *pShading);
		virtual ~GrShading();

		static GrShading *Parse(Object *pObject);

		virtual GrShading *Copy() = 0;

		int           GetType()
		{
			return m_nType;
		}
		GrColorSpace *GetColorSpace()
		{
			return m_pColorSpace;
		}
		GrColor      *GetBackground()
		{
			return &m_oBackground;
		}
		bool          GetHasBackground()
		{
			return m_bHasBackground;
		}
		void          GetBBox(double *pdXMin, double *pdYMin, double *pdXMax, double *pdYMax)
		{
			*pdXMin = m_dXMin;
			*pdYMin = m_dYMin;
			*pdXMax = m_dXMax;
			*pdYMax = m_dYMax;
		}
		bool          GetHasBBox()
		{
			return m_bHasBBox;
		}

	protected:

		bool Initialize(Dict *pDict);

	protected:

		int           m_nType;          // Тип
		GrColorSpace *m_pColorSpace;    // Цветовое пространство, в котором рисуем
		GrColor       m_oBackground;    // Цвет подкладки, если она есть
		bool          m_bHasBackground; // Есть ли подкладка?

		double        m_dXMin;          //
		double        m_dYMin;          // BBox
		double        m_dXMax;          //
		double        m_dYMax;          //
		bool          m_bHasBBox;       // Задан ли BBox?
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrFunctionShading
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrFunctionShading : public GrShading
	{
	public:

		GrFunctionShading(double dMinX, double dMinY, double dMaxX, double dMaxY, double *pMatrix, Function **ppFunctions, int nFuncsCount);
		GrFunctionShading(GrFunctionShading *pShading);
		virtual ~GrFunctionShading();

		static GrFunctionShading *Parse(Dict *pDict);

		virtual GrShading *Copy();

		void      GetDomain(double *pdMinX, double *pdMinY, double *pdMaxX, double *pdMaxY)
		{
			*pdMinX = m_dDomainMinX;
			*pdMinY = m_dDomainMinY;
			*pdMaxX = m_dDomainMaxX;
			*pdMaxY = m_dDomainMaxY;
		}
		double   *GetMatrix()
		{
			return m_arrMatrix;
		}
		int       GetFunctionsCount()
		{
			return m_nFunctionsCount;
		}
		Function *GetFunction(int nIndex)
		{
			return m_arrFunctions[nIndex];
		}
		void      GetColor(double dX, double dY, GrColor *pColor);

	private:

		double    m_dDomainMinX;   // 
		double    m_dDomainMinY;   //  Domain
		double    m_dDomainMaxX;   //
		double    m_dDomainMaxY;   //

		double    m_arrMatrix[6];  // Матрица преобразования и пространства, определенного Domain, в текущее пространство Shading
		Function *m_arrFunctions[GrColorMaxComps];
		int       m_nFunctionsCount;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrAxialShading
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrAxialShading : public GrShading
	{
	public:

		GrAxialShading(double dX0, double dY0, double dX1, double dY1, double dT0, double dT1, Function **ppFunctions, int nFuncsCount, bool bExtendStart, bool bExtendEnd);
		GrAxialShading(GrAxialShading *pShading);
		virtual ~GrAxialShading();

		static GrAxialShading *Parse(Dict *pDict);

		virtual GrShading *Copy();

		void      GetCoords(double *pdX0, double *pdY0, double *pdX1, double *pdY1)
		{
			*pdX0 = m_dAxisX0;
			*pdY0 = m_dAxisY0;
			*pdX1 = m_dAxisX1;
			*pdY1 = m_dAxisY1;
		}
		double    GetDomain0()
		{
			return m_dT0;
		}
		double    GetDomain1()
		{
			return m_dT1;
		}
		bool      GetExtendStart()
		{
			return m_bExtendStart;
		}
		bool      GetExtendEnd()
		{
			return m_bExtendEnd;
		}
		int       GetFunctionsCount()
		{
			return m_nFunctionsCount;
		}
		Function *GetFunctions(int nIndex)
		{
			return m_arrFunctions[nIndex];
		}
		void      GetColor(double dT, GrColor *pColor);

	private:

		double    m_dAxisX0;              // Начальная точка вектора
		double    m_dAxisY0;              // 
		double    m_dAxisX1;              // Конечная точка вектора
		double    m_dAxisY1;              //

		double    m_dT0;                  // Пределы параметра t(параметризация оси)
		double    m_dT1;

		Function *m_arrFunctions[GrColorMaxComps];
		int       m_nFunctionsCount;

		bool      m_bExtendStart;         // Продолжать ли за начальную точку оси
		bool      m_bExtendEnd;           // Продолжать ли за конечную точку оси
	};
	//-------------------------------------------------------------------------------------------------------------------------------
	// GrRadialShading
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrRadialShading : public GrShading
	{
	public:

		GrRadialShading(double dFirstX, double dFirstY, double dFirstRad, double dSecondX, double dSecondY, double dSecondRad, double dT0, double dT1, Function **ppFunctions, int nFuncsCount, bool bExtendFirst, bool bExtendSecond);
		GrRadialShading(GrRadialShading *pShading);
		virtual ~GrRadialShading();

		static GrRadialShading *Parse(Dict *pDict);

		virtual GrShading *Copy();

		void      GetCoords(double *pdFirstX, double *pdFirstY, double *pdFirstRad, double *pdSecondX, double *pdSecondY, double *pdSecondRad)
		{
			*pdFirstX    = m_dFirstX;
			*pdFirstY    = m_dFirstY;
			*pdFirstRad  = m_dFirstRad;
			*pdSecondX   = m_dSecondX;
			*pdSecondY   = m_dSecondY;
			*pdSecondRad = m_dSecondRad;
		}
		double    GetDomain0()
		{
			return m_dT0;
		}
		double    GetDomain1()
		{
			return m_dT1;
		}
		bool      GetExtendFirst()
		{
			return m_bExtendFirst;
		}
		bool      GetExtendSecond()
		{
			return m_bExtendSecond;
		}
		int       GetFunctionsCount()
		{
			return m_nFunctionsCount;
		}
		Function *GetFunctions(int nIndex)
		{
			return m_arrFunctions[nIndex];
		}
		void      GetColor(double dT, GrColor *pColor);

	private:

		double    m_dFirstX;          // Координаты центра первой окружности
		double    m_dFirstY;          //
		double    m_dFirstRad;        // Радиус первой окружности
		double    m_dSecondX;         // Координаты центра второй окружности
		double    m_dSecondY;         // 
		double    m_dSecondRad;       // Радиус второй окружности

		double    m_dT0;              // Границы параметра t
		double    m_dT1;              // 

		Function *m_arrFunctions[GrColorMaxComps];
		int       m_nFunctionsCount;
		bool      m_bExtendFirst;     // Продолжать ли рисовать за первый круг
		bool      m_bExtendSecond;    // Продолжать ли рисовать за второй круг
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrGouraudTriangleShading
	//-------------------------------------------------------------------------------------------------------------------------------

	struct GrGouraudVertex
	{
		double  dX;       // Координаты вершины
		double  dY;       //
		GrColor oColor;   // Цвет
	};

	class GrGouraudTriangleShading : public GrShading
	{
	public:

		GrGouraudTriangleShading(int nType, GrGouraudVertex *pVertexes, int nVertexsCount, int(*pTriangles)[3], int nTrianglesCount, Function **ppFunctions, int nFunctionsCount);
		GrGouraudTriangleShading(GrGouraudTriangleShading *pShading);
		virtual ~GrGouraudTriangleShading();

		static GrGouraudTriangleShading *Parse(int nType, Dict *pDict, Stream *pStream);

		virtual GrShading *Copy();

		int  GetTrianglesCount()
		{
			return m_nTrianglesCount;
		}
		void GetTriangle(int nIndex, double *pdX0, double *pdY0, GrColor *pColor0, double *pdX1, double *pdY1, GrColor *pColor1, double *pdX2, double *pdY2, GrColor *pColor2);

	private:

		GrGouraudVertex *m_arrVertexs;                    // Массив всех вершин
		int              m_nVertexsCount;                 // Количество вершин
		int(*m_arrTriangles)[3];              // Массив треугольников(треугольник определяется по номерам вершин)
		int              m_nTrianglesCount;               // Количество треугольников
		Function        *m_ppFunctions[GrColorMaxComps];  // Функции
		int              m_nFunctionsCount;               // Количество функций
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrPatchMeshShading
	//-------------------------------------------------------------------------------------------------------------------------------

	struct GrPatch
	{
		double  arrX[4][4];
		double  arrY[4][4];
		GrColor arrColor[2][2];
	};

	class GrPatchMeshShading : public GrShading
	{
	public:

		GrPatchMeshShading(int nType, GrPatch *pPatches, int nPatchesCount, Function **ppFuntions, int nFunctionsCount);
		GrPatchMeshShading(GrPatchMeshShading *pShading);

		virtual ~GrPatchMeshShading();

		static GrPatchMeshShading *Parse(int nType, Dict *pDict, Stream *pStream);

		virtual GrShading *Copy();

		int      GetPatchesCount()
		{
			return m_nPatchesCount;
		}
		GrPatch *GetPatch(int nIndex)
		{
			return &m_pPatches[nIndex];
		}

	private:

		GrPatch  *m_pPatches;                      // Массив частей
		int       m_nPatchesCount;                 // Количество частей
		Function *m_ppFunctions[GrColorMaxComps];  // Массив функций
		int       m_nFunctionsCount;               // Количество функций
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrImageColorMap
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrImageColorMap
	{
	public:

		GrImageColorMap(int nBitsPerComponent, Object *pDecode, GrColorSpace *pColorSpace);

		~GrImageColorMap();

		GrImageColorMap *Copy()
		{
			return new GrImageColorMap(this);
		}

		bool IsValid()
		{
			return m_bSuccess;
		}

		GrColorSpace *GetColorSpace()
		{
			return m_pColorSpace;
		}

		int GetComponentsCount()
		{
			return m_nComponentsCount;
		}
		int GetBitsPerComponent()
		{
			return m_nBitsPerComponent;
		}

		double GetDecodeLow(int nIndex)
		{
			return m_arrDecodeLow[nIndex];
		}
		double GetDecodeHigh(int nIndex)
		{
			return m_arrDecodeLow[nIndex] + m_arrDecodeRange[nIndex];
		}

		void GetGray(unsigned char *pColorValue, GrGray  *pGray);
		void GetRGB(unsigned char *pColorValue, GrRGB   *pRGB);
		void GetCMYK(unsigned char *pColorValue, GrCMYK  *pCMYK);
		void GetColor(unsigned char *pColorValue, GrColor *pColor);

	private:

		GrImageColorMap(GrImageColorMap *pColorMap);

	private:

		GrColorSpace *m_pColorSpace;                     // Цветовое пространство изображения
		int           m_nBitsPerComponent;               // bits per component
		int           m_nComponentsCount;                // Число цветовых компонент в пикселе
		GrColorSpace *m_pColorSpace2;                    // Второе цветовое пространство
		int           m_nComponentsCount2;               // Число цветовых компонент в пикселе(во втором пространстве)
		GrColorComp  *m_ppLookup[GrColorMaxComps];       // lookup table
		double        m_arrDecodeLow[GrColorMaxComps];   // Минимальные значения для каждой компоненты
		double        m_arrDecodeRange[GrColorMaxComps]; // Значения (Max - min) для каждой компоненты

		bool          m_bSuccess;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrSubpath and GrPath
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrSubpath
	{
	public:

		GrSubpath(double dX, double dY);

		~GrSubpath();

		GrSubpath *Copy()
		{
			return new GrSubpath(this);
		}

		int    GetPointsCount()
		{
			return m_nPointsCount;
		}
		double GetX(int nIndex)
		{
			return m_pX[nIndex];
		}
		double GetY(int nIndex)
		{
			return m_pY[nIndex];
		}
		bool   GetCurve(int nIndex)
		{
			return m_pbCurve[nIndex];
		}

		double GetLastX()
		{
			return m_pX[m_nPointsCount - 1];
		}
		double GetLastY()
		{
			return m_pY[m_nPointsCount - 1];
		}


		void LineTo(double dX, double dY);

		void CurveTo(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3);

		void Close();

		bool IsClosed()
		{
			return m_bClosed;
		}


		// Добавляем сдвиг (<dDx>, <dDy>) к каждой точке subpath.
		void Offset(double dDx, double dDy);
		void Transform(double *pMatrix);

		bool IsEqual(GrSubpath *pSubPath)
		{
			if (m_nPointsCount != pSubPath->m_nPointsCount || m_bClosed != pSubPath->m_bClosed)
				return false;


			for (int nIndex = 0; nIndex < m_nPointsCount; nIndex++)
			{
				if (fabs(m_pX[nIndex] - pSubPath->m_pX[nIndex]) > 0.001 || fabs(m_pY[nIndex] - pSubPath->m_pY[nIndex]) > 0.001 || m_pbCurve[nIndex] != pSubPath->m_pbCurve[nIndex])
					return false;
			}

			return true;
		}

	private:

		GrSubpath(GrSubpath *pSubpath);

	private:

		double *m_pX;           // Координаты точек в supath
		double *m_pY;           //

		bool   *m_pbCurve;      // Если m_bCurve[i] = true => точка i - контрольная точка для кривой Безье
		int     m_nPointsCount; // Количество точек
		int     m_nSize;        // Размер массивов m_pX/m_pY
		bool    m_bClosed;      // Закрыт ли path?
	};

	class GrPath
	{
	public:

		GrPath();

		~GrPath();

		GrPath *Copy()
		{
			return new GrPath(m_bJustStarted, m_dFirstX, m_dFirstY, m_ppSubpaths, m_nSubpathsCount, m_nSize);
		}

		// Есть ли текущая точка?
		bool IsCurPoint()
		{
			return m_nSubpathsCount > 0 || m_bJustStarted;
		}

		// Пустой ли path, т.е. есть ли в нем хотя бы один subpath?
		bool IsPathNonEmpty()
		{
			return m_nSubpathsCount > 0;
		}

		int        GetSubpathsCount()
		{
			return m_nSubpathsCount;
		}
		GrSubpath *GetSubpath(int nIndex)
		{
			return m_ppSubpaths[nIndex];
		}

		double     GetLastX()
		{
			return m_ppSubpaths[m_nSubpathsCount - 1]->GetLastX();
		}
		double     GetLastY()
		{
			return m_ppSubpaths[m_nSubpathsCount - 1]->GetLastY();
		}


		void MoveTo(double dX, double dY);
		void LineTo(double dX, double dY);
		void CurveTo(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3);
		void Close();
		void Append(GrPath *pPath);

		// Добавляем сдвиг (<dDx>, <dDy>) к каждой точке path.
		void Offset(double dDx, double dDy);

		bool IsEqual(GrPath *pPath)
		{
			if (m_nSubpathsCount != pPath->m_nSubpathsCount)
				return false;

			for (int nIndex = 0; nIndex < m_nSubpathsCount; nIndex++)
			{
				if (!m_ppSubpaths[nIndex]->IsEqual(pPath->m_ppSubpaths[nIndex]))
					return false;
			}

			return true;
		}

	private:

		GrPath(bool bJustStarted, double dFirstX, double dFirstY, GrSubpath **ppSubpaths, int nSubpathsCount, int nSize);

	private:
		bool        m_bJustStarted;   // True, если новый subpath был только что начат
		double      m_dFirstX;        // Координаты первой точки нового subpath
		double      m_dFirstY;        // 
		GrSubpath **m_ppSubpaths;     // Список subpaths
		int         m_nSubpathsCount; // Количетсво subpaths
		int         m_nSize;          // Размер массива m_ppSubpaths
	};


	//-------------------------------------------------------------------------------------------------------------------------------
	// GrTextClip
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrTextClip
	{
	public:

		GrTextClip()
		{
			m_nTextsCount = m_nSize = 0;
			m_pTexts  = NULL;
			m_pMatrix = NULL;
		}

		GrTextClip *Copy()
		{
			return new GrTextClip(this);
		}

		~GrTextClip()
		{
			for (int nIndex = 0; nIndex < m_nTextsCount; ++nIndex)
			{
				FreeWString(m_pTexts[nIndex].wsText);
				FreeWString(m_pTexts[nIndex].wsFontName);
				FreeWString(m_pTexts[nIndex].wsFontPath);
			}
			MemUtilsFree(m_pTexts);
			MemUtilsFree(m_pMatrix);
		}

		void    Reset()
		{
			for (int nIndex = 0; nIndex < m_nTextsCount; ++nIndex)
			{
				FreeWString(m_pTexts[nIndex].wsText);
				FreeWString(m_pTexts[nIndex].wsFontName);
				FreeWString(m_pTexts[nIndex].wsFontPath);
			}
			MemUtilsFree(m_pTexts);
			MemUtilsFree(m_pMatrix);

			m_nTextsCount = m_nSize = 0;
			m_pTexts  = NULL;
			m_pMatrix = NULL;
		}
		void    ClipToText(std::wstring& wsFontName, std::wstring& wsFontPath, double dFontSize, int nFontStyle, double *pMatrix, std::wstring& wsText, double dX, double dY, double dWidth = 0, double dHeight = 0, double dBaseLineOffset = 0)
		{
			Resize(1);

			m_pTexts[m_nTextsCount].wsFontName      = AllocWString(wsFontName);
			m_pTexts[m_nTextsCount].wsFontPath      = AllocWString(wsFontPath);
			m_pTexts[m_nTextsCount].dFontSize       = dFontSize;
			m_pTexts[m_nTextsCount].nFontStyle      = nFontStyle;
			m_pTexts[m_nTextsCount].wsText          = AllocWString(wsText);
			m_pTexts[m_nTextsCount].dX              = dX;
			m_pTexts[m_nTextsCount].dY              = dY;
			m_pTexts[m_nTextsCount].dWidth          = dWidth;
			m_pTexts[m_nTextsCount].dHeight         = dHeight;
			m_pTexts[m_nTextsCount].dBaseLineOffset = dBaseLineOffset;

			m_pMatrix[m_nTextsCount].FromDoublePointer(pMatrix);

			++m_nTextsCount;
			return;

		}
		int     GetTextsCount()
		{
			return m_nTextsCount;
		}
		WString GetText(int nTextIndex, double *pdX, double *pdY, double *pdWidth, double *pdHeight, double *pdBaseLineOffset, WString* pwsFontName, WString* pwsFontPath, double *pdFontSize, int* pnFontStyle)
		{
			if (nTextIndex < 0 || nTextIndex >= m_nTextsCount)
				return NULL;

			*pdX              = m_pTexts[nTextIndex].dX;
			*pdY              = m_pTexts[nTextIndex].dY;
			*pdWidth          = m_pTexts[nTextIndex].dWidth;
			*pdHeight         = m_pTexts[nTextIndex].dHeight;
			*pdBaseLineOffset = m_pTexts[nTextIndex].dBaseLineOffset;

			*pwsFontName      = m_pTexts[nTextIndex].wsFontName;
			*pwsFontPath      = m_pTexts[nTextIndex].wsFontPath;
			*pdFontSize       = m_pTexts[nTextIndex].dFontSize;
			*pnFontStyle      = m_pTexts[nTextIndex].nFontStyle;

			return m_pTexts[nTextIndex].wsText;
		}
		double* GetMatrix(int nTextIndex)
		{
			if (nTextIndex < 0 || nTextIndex >= m_nTextsCount)
				return NULL;

			return m_pMatrix[nTextIndex].ToDoublePointer();
		}
		bool    IsEqual(GrTextClip *pClip)
		{
			if (pClip->m_nTextsCount != m_nTextsCount)
				return false;

			for (int nIndex = 0; nIndex < m_nTextsCount; nIndex++)
			{
				if (!m_pTexts[nIndex].IsEqual(pClip->m_pTexts[nIndex]) || !m_pMatrix[nIndex].IsEqual(pClip->m_pMatrix[nIndex]))
					return false;
			}

			return true;
		}

	private:

		GrTextClip(GrTextClip *pTextClip)
		{
			m_nTextsCount = pTextClip->m_nTextsCount;
			m_nSize       = pTextClip->m_nSize;

			m_pTexts  = (Text   *)MemUtilsMallocArray(m_nSize, sizeof(Text));
			m_pMatrix = (Matrix *)MemUtilsMallocArray(m_nSize, sizeof(Matrix));

			for (int nIndex = 0; nIndex < m_nTextsCount; ++nIndex)
			{
				m_pTexts[nIndex].wsText          = AllocWString(pTextClip->m_pTexts[nIndex].wsText);
				m_pTexts[nIndex].dX              = pTextClip->m_pTexts[nIndex].dX;
				m_pTexts[nIndex].dY              = pTextClip->m_pTexts[nIndex].dY;
				m_pTexts[nIndex].dWidth          = pTextClip->m_pTexts[nIndex].dWidth;
				m_pTexts[nIndex].dHeight         = pTextClip->m_pTexts[nIndex].dHeight;
				m_pTexts[nIndex].dBaseLineOffset = pTextClip->m_pTexts[nIndex].dBaseLineOffset;

				m_pTexts[nIndex].wsFontName      = AllocWString(pTextClip->m_pTexts[nIndex].wsFontName);
				m_pTexts[nIndex].wsFontPath      = AllocWString(pTextClip->m_pTexts[nIndex].wsFontPath);
				m_pTexts[nIndex].dFontSize       = pTextClip->m_pTexts[nIndex].dFontSize;
				m_pTexts[nIndex].nFontStyle      = pTextClip->m_pTexts[nIndex].nFontStyle;

				m_pMatrix[nIndex]   = pTextClip->m_pMatrix[nIndex];
			}

		}
		void    Resize(int nTextsCount)
		{
			if (m_nTextsCount + nTextsCount > m_nSize)
			{
				if (m_nSize == 0)
				{
					m_nSize = 32;
				}
				while (m_nSize < m_nTextsCount + nTextsCount)
				{
					m_nSize *= 2;
				}

				m_pTexts  = (Text   *)MemUtilsReallocArray(m_pTexts, m_nSize, sizeof(Text));
				m_pMatrix = (Matrix *)MemUtilsReallocArray(m_pMatrix, m_nSize, sizeof(Matrix));
			}
		}
		void    Transform(double *pMatrix, double dX, double dY, double *pdX, double *pdY)
		{
			*pdX = dX * pMatrix[0] + dY * pMatrix[2] + pMatrix[4];
			*pdY = dX * pMatrix[1] + dY * pMatrix[3] + pMatrix[5];
		}

	private:

#pragma pack(push, 1)
		struct Matrix
		{
			double dA;
			double dB;
			double dC;
			double dD;
			double dE;
			double dF;

			double *ToDoublePointer()
			{
				return &dA;
			}

			void FromDoublePointer(double *pMatrix)
			{
				dA = pMatrix[0];
				dB = pMatrix[1];
				dC = pMatrix[2];
				dD = pMatrix[3];
				dE = pMatrix[4];
				dF = pMatrix[5];
			}
			bool IsEqual(Matrix &oMatrix)
			{
				if (fabs(dA - oMatrix.dA) < 0.001 && fabs(dB - oMatrix.dB) < 0.001 && fabs(dC - oMatrix.dC) < 0.001 && fabs(dD - oMatrix.dD) < 0.001 && fabs(dE - oMatrix.dE) < 0.001 && fabs(dF - oMatrix.dF) < 0.001)
					return true;

				return false;
			}
		};

		struct Text
		{
			WString wsFontName;
			WString wsFontPath;
			double  dFontSize;
			int     nFontStyle;

			WString wsText;
			double  dX;
			double  dY;
			double  dWidth;
			double  dHeight;
			double  dBaseLineOffset;

			bool IsEqual(Text &oText)
			{
				if (fabs(dX - oText.dX) < 0.001 && fabs(dY - oText.dY) < 0.001 && fabs(dWidth - oText.dWidth) < 0.001 && fabs(dHeight - oText.dHeight) < 0.001 && fabs(dBaseLineOffset - oText.dBaseLineOffset) < 0.001 && std::wstring(wsText) == std::wstring(oText.wsText))
					return true;

				return false;
			}
		};

#pragma pack(pop)

	private:

		Text*   m_pTexts;
		Matrix* m_pMatrix;
		int     m_nTextsCount;
		int     m_nSize;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrClip
	//-------------------------------------------------------------------------------------------------------------------------------

#define GrClipEOFlag       0x01	// Используем правило Even-odd

	class GrClip
	{
	public:

		GrClip(double dMinX, double dMinY, double dMaxX, double dMaxY);

		GrClip *Copy()
		{
			return new GrClip(this);
		}

		~GrClip();

		void ResetToRect(double dX0, double dY0, double dX1, double dY1);

		// Пересекаем Clip с заданным прямоугольником.
		void ClipToRect(double dX0, double dY0, double dX1, double dY1);

		// Пересекаем Clip с <pPath>.
		void ClipToPath(GrPath *pPath, double *pMatrix, bool bEO);

		// Пересекаем с текстом
		void ClipToText(std::wstring wsFontName, std::wstring wsFontPath, double dFontSize, int nFontStyle, double *pMatrix, std::wstring wsText, double dX, double dY, double dWidth, double dHeight, double dBaseLineOffset)
		{
			if (!m_pTextClip)
				m_pTextClip = new GrTextClip();

			m_pTextClip->ClipToText(wsFontName, wsFontPath, dFontSize, nFontStyle, pMatrix, wsText, dX, dY, dWidth, dHeight, dBaseLineOffset);
		}

		//
		void AppendTextClip(GrTextClip *pTextClip)
		{
			for (int nIndex = 0; nIndex < pTextClip->GetTextsCount(); nIndex++)
			{
				WString wsFontName, wsFontPath;
				int nFontStyle;
				double dFontSize = 10, dX = 0, dY = 0, dWidth = 0, dHeight = 0, dBaseLineOffset = 0;

				WString wsText  = pTextClip->GetText(nIndex, &dX, &dY, &dWidth, &dHeight, &dBaseLineOffset, &wsFontName, &wsFontPath, &dFontSize, &nFontStyle);
				double *pMatrix = pTextClip->GetMatrix(nIndex);

				ClipToText(std::wstring(wsFontName), std::wstring(wsFontPath), dFontSize, nFontStyle, pMatrix, std::wstring(wsText), dX, dY, dWidth, dHeight, dBaseLineOffset);
			}
		}

		int     GetMinX()
		{
			return m_nMinX;
		}
		int     GetMaxX()
		{
			return m_nMaxX;
		}
		int     GetMinY()
		{
			return m_nMinY;
		}
		int     GetMaxY()
		{
			return m_nMaxY;
		}

		int     GetPathsCount()
		{
			return m_nPathsCount;
		}

		GrPath *GetPath(int nPathIndex)
		{
			if (nPathIndex < 0 || nPathIndex >= m_nPathsCount)
				return NULL;

			return m_ppPaths[nPathIndex];
		}
		double *GetMatrix(int nPathIndex)
		{
			if (nPathIndex < 0 || nPathIndex >= m_nPathsCount)
				return NULL;

			return m_pMatrix[nPathIndex].ToDoublePointer();
		}
		int     GetFlag(int nPathIndex)
		{
			if (nPathIndex < 0 || nPathIndex >= m_nPathsCount)
				return 0;

			return m_pFlags[nPathIndex];
		}
		WString GetText(int nTextIndex, double *pdX, double *pdY, double *pdWidth, double *pdHeight, double *pdBaseLineOffset, WString* pwsFontName, WString* pwsFontPath, double *pdFontSize, int* pnFontStyle)
		{
			if (m_pTextClip)
			{
				return m_pTextClip->GetText(nTextIndex, pdX, pdY, pdWidth, pdHeight, pdBaseLineOffset, pwsFontName, pwsFontPath, pdFontSize, pnFontStyle);
			}

			return NULL;
		}
		int     GetTextsCount()
		{
			if (m_pTextClip)
			{
				return m_pTextClip->GetTextsCount();
			}
			return -1;
		}
		double *GetTextMatrix(int nTextIndex)
		{
			if (m_pTextClip)
				return m_pTextClip->GetMatrix(nTextIndex);

			return NULL;
		}

		bool IsEqual(GrClip *pClip, int &nPathIndex)
		{
			bool bResult = true;
			nPathIndex = -1;

			if (!m_pTextClip->IsEqual(pClip->m_pTextClip))
				return false;

			if (m_nPathsCount < pClip->m_nPathsCount)
				bResult = false;
			else if (m_nPathsCount > pClip->m_nPathsCount)
				return false;


			for (int nIndex = 0; nIndex < m_nPathsCount; nIndex++)
			{
				if (!m_pMatrix[nIndex].IsEqual(pClip->m_pMatrix[nIndex]) || m_pFlags[nIndex] != pClip->m_pFlags[nIndex] || !m_ppPaths[nIndex]->IsEqual(pClip->m_ppPaths[nIndex]))
					return false;
			}

			nPathIndex = m_nPathsCount;

			return bResult;
		}

	private:

		GrClip(GrClip *pClip);
		void    Resize(int nPathsCount);
		GrPath *PathRect(double dX0, double dY0, double dX1, double dY1);
		void    Transform(double *pMatrix, double dX, double dY, double *pdX, double *pdY)
		{
			*pdX = dX * pMatrix[0] + dY * pMatrix[2] + pMatrix[4];
			*pdY = dX * pMatrix[1] + dY * pMatrix[3] + pMatrix[5];
		}

	private:

#pragma pack(push, 1)
		struct Matrix
		{
			double dA;
			double dB;
			double dC;
			double dD;
			double dE;
			double dF;

			double *ToDoublePointer()
			{
				return &dA;
			}

			void FromDoublePointer(double *pMatrix)
			{
				dA = pMatrix[0];
				dB = pMatrix[1];
				dC = pMatrix[2];
				dD = pMatrix[3];
				dE = pMatrix[4];
				dF = pMatrix[5];
			}

			bool IsEqual(Matrix &oMatrix)
			{
				if (fabs(dA - oMatrix.dA) < 0.001 && fabs(dB - oMatrix.dB) < 0.001 && fabs(dC - oMatrix.dC) < 0.001 && fabs(dD - oMatrix.dD) < 0.001 && fabs(dE - oMatrix.dE) < 0.001 && fabs(dF - oMatrix.dF) < 0.001)
					return true;

				return false;
			}

		};
#pragma pack(pop)

	private:

		double          m_dMinX;
		double          m_dMinY;
		double          m_dMaxX;
		double          m_dMaxY;

		int             m_nMinX;
		int             m_nMinY;
		int             m_nMaxX;
		int             m_nMaxY;

		GrPath        **m_ppPaths;
		Matrix         *m_pMatrix;
		unsigned char  *m_pFlags;
		int             m_nPathsCount;
		int             m_nSize;

		GrTextClip     *m_pTextClip;
	};


	//-------------------------------------------------------------------------------------------------------------------------------
	// GrState
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrState
	{
	public:

		GrState(double dHorizDPI, double dVertDPI, PDFRectangle *pPageBox, int nRotate, bool bUpsideDown);

		~GrState();

		GrState *Copy()
		{
			return new GrState(this);
		}


		double            GetHorDPI()
		{
			return m_dHorDPI;
		}
		double            GetVerDPI()
		{
			return m_dVerDPI;
		}
		double           *GetCTM()
		{
			return m_arrCTM;
		}
		double            GetPageLeft()
		{
			return m_dPageLeft;
		}
		double            GetPageBottom()
		{
			return m_dPageBottom;
		}
		double            GetPageRight()
		{
			return m_dPageRight;
		}
		double            GetPageTop()
		{
			return m_dPageTop;
		}
		double            GetPageWidth()
		{
			return m_dPageWidth;
		}
		double            GetPageHeight()
		{
			return m_dPageHeight;
		}
		int               GetRotate()
		{
			return m_nRotate;
		}
		GrColor          *GetFillColor()
		{
			return &m_oFillColor;
		}
		GrColor          *GetStrokeColor()
		{
			return &m_oStrokeColor;
		}
		void              GetFillGray(GrGray *pGray)
		{
			m_pFillColorSpace->GetGray(&m_oFillColor, pGray);
		}
		void              GetStrokeGray(GrGray *pGray)
		{
			m_pStrokeColorSpace->GetGray(&m_oStrokeColor, pGray);
		}
		void              GetFillRGB(GrRGB *pRGB)
		{
			m_pFillColorSpace->GetRGB(&m_oFillColor, pRGB);
		}
		void              GetStrokeRGB(GrRGB *pRGB)
		{
			m_pStrokeColorSpace->GetRGB(&m_oStrokeColor, pRGB);
		}
		void              GetFillCMYK(GrCMYK *pCMYK)
		{
			m_pFillColorSpace->GetCMYK(&m_oFillColor, pCMYK);
		}
		void              GetStrokeCMYK(GrCMYK *pCMYK)
		{
			m_pStrokeColorSpace->GetCMYK(&m_oStrokeColor, pCMYK);
		}
		GrColorSpace     *GetFillColorSpace()
		{
			return m_pFillColorSpace;
		}
		GrColorSpace     *GetStrokeColorSpace()
		{
			return m_pStrokeColorSpace;
		}
		GrPattern        *GetFillPattern()
		{
			return m_pFillPattern;
		}
		GrPattern        *GetStrokePattern()
		{
			return m_pStrokePattern;
		}
		GraphicsBlendMode GetBlendMode()
		{
			return m_eBlendMode;
		}
		double            GetFillOpacity()
		{
			return m_dFillOpacity;
		}
		double            GetStrokeOpacity()
		{
			return m_dStrokeOpacity;
		}
		bool              GetFillOverprint()
		{
			return m_bFillOverprint;
		}
		bool              GetStrokeOverprint()
		{
			return m_bStrokeOverprint;
		}
		Function        **GetTransfer()
		{
			return m_ppTransfer;
		}
		double            GetLineWidth()
		{
			return m_dLineWidth;
		}
		void              GetLineDash(double **ppDash, int *pSize, double *pStart)
		{
			*ppDash = m_pLineDash;
			*pSize  = m_nLineDashSize;
			*pStart = m_dLineDashStart;
		}
		int               GetFlatness()
		{
			return m_nFlatness;
		}
		int               GetLineJoin()
		{
			return m_nLineJoin;
		}
		int               GetLineCap()
		{
			return m_nLineCap;
		}
		double            GetMiterLimit()
		{
			return m_dMiterLimit;
		}
		bool              GetStrokeAdjust()
		{
			return m_bStrokeAdjust;
		}
		GrFont           *GetFont()
		{
			return m_pFont;
		}
		double            GetFontSize()
		{
			return m_dFontSize;
		}
		double           *GetTextMatrix()
		{
			return m_arrTextMatrix;
		}
		double            GetCharSpace()
		{
			return m_dCharSpace;
		}
		double            GetWordSpace()
		{
			return m_dWordSpace;
		}
		double            GetHorizScaling()
		{
			return m_dHorizScaling;
		}
		double            GetLeading()
		{
			return m_dLeading;
		}
		double            GetRise()
		{
			return m_nRise;
		}
		int               GetRenderMode()
		{
			return m_nRenderMode;
		}
		GrPath           *GetPath()
		{
			return m_pPath;
		}
		double            GetCurX()
		{
			return m_dCurX;
		}
		double            GetCurY()
		{
			return m_dCurY;
		}
		void              GetClipBBox(double *pXMin, double *pYMin, double *pXMax, double *pYMax)
		{
			*pXMin = m_dClipXMin;
			*pYMin = m_dClipYMin;
			*pXMax = m_dClipXMax;
			*pYMax = m_dClipYMax;
		}
		void              GetUserClipBBox(double *pXMin, double *pYMin, double *pXMax, double *pYMax);
		double            GetTextLineX()
		{
			return m_dTextLineX;
		}
		double            GetTextLineY()
		{
			return m_dTextLineY;
		}

		GrClip           *GetClip()
		{
			return m_pClip;
		}
		// Есть ли текущая точка?
		bool IsCurPoint()
		{
			return m_pPath->IsCurPoint();
		}
		// Есть ли непустой path?
		bool IsPathNonEmpty()
		{
			return m_pPath->IsPathNonEmpty();
		}

		// Различные преобразования координат.
		void   Transform(double dSrcX, double dSrcY, double *pdResX, double *pdResY)
		{
			*pdResX = m_arrCTM[0] * dSrcX + m_arrCTM[2] * dSrcY + m_arrCTM[4];
			*pdResY = m_arrCTM[1] * dSrcX + m_arrCTM[3] * dSrcY + m_arrCTM[5];
		}
		void   TransformDelta(double dSrcX, double dSrcY, double *pdResX, double *pdResY)
		{
			*pdResX = m_arrCTM[0] * dSrcX + m_arrCTM[2] * dSrcY;
			*pdResY = m_arrCTM[1] * dSrcX + m_arrCTM[3] * dSrcY;
		}
		void   TextTransform(double dSrcX, double dSrcY, double *pdResX, double *pdResY)
		{
			*pdResX = m_arrTextMatrix[0] * dSrcX + m_arrTextMatrix[2] * dSrcY + m_arrTextMatrix[4];
			*pdResY = m_arrTextMatrix[1] * dSrcX + m_arrTextMatrix[3] * dSrcY + m_arrTextMatrix[5];
		}
		void   TextTransformDelta(double dSrcX, double dSrcY, double *pdResX, double *pdResY)
		{
			*pdResX = m_arrTextMatrix[0] * dSrcX + m_arrTextMatrix[2] * dSrcY;
			*pdResY = m_arrTextMatrix[1] * dSrcX + m_arrTextMatrix[3] * dSrcY;
		}
		double TransformWidth(double dWidth);
		double GetTransformedLineWidth()
		{
			return TransformWidth(m_dLineWidth);
		}
		double GetTransformedFontSize();
		void   GetFontTransformMatrix(double *pdM11, double *pdM12, double *pdM21, double *pdM22);

		void SetCTM(double dA, double dB, double dC, double dD, double dE, double dF);
		void ConcatCTM(double dA, double dB, double dC, double dD, double dE, double dF);
		void ShiftCTM(double dShiftX, double dShiftY);
		void SetFillColorSpace(GrColorSpace *pColorSpace);
		void SetStrokeColorSpace(GrColorSpace *pColorSpace);
		void SetFillColor(GrColor *pColor)
		{
			m_oFillColor = *pColor;
		}
		void SetStrokeColor(GrColor *pColor)
		{
			m_oStrokeColor = *pColor;
		}
		void SetFillPattern(GrPattern *pPattern);
		void SetStrokePattern(GrPattern *pPattern);
		void SetBlendMode(GraphicsBlendMode eMode)
		{
			m_eBlendMode = eMode;
		}
		void SetFillOpacity(double dOpacity)
		{
			m_dFillOpacity = dOpacity;
		}
		void SetStrokeOpacity(double dOpacity)
		{
			m_dStrokeOpacity = dOpacity;
		}
		void SetFillOverprint(bool bOverprint)
		{
			m_bFillOverprint = bOverprint;
		}
		void SetStrokeOverprint(bool bOverprint)
		{
			m_bStrokeOverprint = bOverprint;
		}
		void SetTransfer(Function **ppFunctions);
		void SetLineWidth(double dWidth)
		{
			m_dLineWidth = dWidth;
		}
		void SetLineDash(double *pDash, int nLength, double dStart);
		void SetFlatness(int nFlatness)
		{
			m_nFlatness = nFlatness;
		}
		void SetLineJoin(int nLineJoin)
		{
			m_nLineJoin = nLineJoin;
		}
		void SetLineCap(int nLineCap)
		{
			m_nLineCap = nLineCap;
		}
		void SetMiterLimit(double dMiterLimit)
		{
			m_dMiterLimit = dMiterLimit;
		}
		void SetStrokeAdjust(bool bStrokeAdjust)
		{
			m_bStrokeAdjust = bStrokeAdjust;
		}
		void SetFont(GrFont *pFont, double dFontSize)
		{
			m_pFont = pFont;
			m_dFontSize = dFontSize;
		}
		void SetTextMatrix(double dA, double dB, double dC, double dD, double dE, double dF)
		{
			m_arrTextMatrix[0] = dA;
			m_arrTextMatrix[1] = dB;
			m_arrTextMatrix[2] = dC;
			m_arrTextMatrix[3] = dD;
			m_arrTextMatrix[4] = dE;
			m_arrTextMatrix[5] = dF;
		}
		void SetCharSpace(double dCharSpace)
		{
			m_dCharSpace = dCharSpace;
		}
		void SetWordSpace(double dWordSpace)
		{
			m_dWordSpace = dWordSpace;
		}
		void SetHorizScaling(double dScale)
		{
			m_dHorizScaling = 0.01 * dScale;
		}
		void SetLeading(double dLeading)
		{
			m_dLeading = dLeading;
		}
		void SetRise(double dRise)
		{
			m_nRise = dRise;
		}
		void SetRenderMode(int nRenderMode)
		{
			m_nRenderMode = nRenderMode;
		}
		void SetPath(GrPath *pPath);
		void MoveTo(double dX, double dY)
		{
			m_pPath->MoveTo(m_dCurX = dX, m_dCurY = dY);
		}
		void LineTo(double dX, double dY)
		{
			m_pPath->LineTo(m_dCurX = dX, m_dCurY = dY);
		}
		void CurveTo(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3)
		{
			m_pPath->CurveTo(dX1, dY1, dX2, dY2, m_dCurX = dX3, m_dCurY = dY3);
		}
		void ClosePath()
		{
			m_pPath->Close();
			m_dCurX = m_pPath->GetLastX();
			m_dCurY = m_pPath->GetLastY();
		}
		void ClearPath();
		void Clip();
		void ClipToStrokePath();

		// Текст.
		void TextSetPos(double dX, double dY)
		{
			m_dTextLineX = dX;
			m_dTextLineY = dY;
		}
		void TextMoveTo(double dX, double dY)
		{
			m_dTextLineX = dX;
			m_dTextLineY = dY;
			TextTransform(dX, dY, &m_dCurX, &m_dCurY);
		}
		void TextShift(double dShiftX, double dShiftY);
		void Shift(double dShidtX, double dShiftY);

		// Работа со стеком.
		GrState *Save();
		GrState *Restore();
		bool HasSaves()
		{
			return m_pNext != NULL;
		}

		// 
		bool ParseBlendMode(Object *pObject, GraphicsBlendMode *peMode);

	private:

		GrState(GrState *pState);

	private:

		double m_dHorDPI;      // Разрешение по горизонтали
		double m_dVerDPI;      // Разрешение по вертикали
		double m_arrCTM[6];    // Матрица преобразования координат

		double m_dPageLeft;    // Координаты левого нижнего угла и
		double m_dPageBottom;  // правого верхнего в пользовательстких координатах
		double m_dPageRight;   //
		double m_dPageTop;     // 

		double m_dPageWidth;   // Ширина страницы в пикселях(т.е. с учетом разрешения m_dHorDPI)
		double m_dPageHeight;  // Высота страницы в пикселях(т.е. с учетом разрешения m_dVerDPI)

		int    m_nRotate;      // Угол поворота страницы

		GrColorSpace     *m_pFillColorSpace;  // Цветовое пространство для заливки
		GrColorSpace     *m_pStrokeColorSpace;// Цветовое пространство для обводки
		GrColor           m_oFillColor;       // Цвет для заливки
		GrColor           m_oStrokeColor;     // Цвет для обводки
		GrPattern        *m_pFillPattern;     // Указатель на объект, определяющий заливку
		GrPattern        *m_pStrokePattern;   // Указатель на объект, определяющий обводку

		GraphicsBlendMode m_eBlendMode;	      // Blend mode (для прозрачности)

		double            m_dFillOpacity;     // Прозрачность заливки
		double            m_dStrokeOpacity;   // Прозрачность обводки

		bool              m_bFillOverprint;   // Заливаем поверх всего?
		bool              m_bStrokeOverprint; // Обводим поверх всего?
		Function         *m_ppTransfer[4];	  // Функция переноса. Варианты входных параметров:
		// все NULL = тождествнная функция;
		// последние три NULL = одномерная функция;
		// все 4 не NULL = R,G,B,gray -функция

		double  m_dLineWidth;    // Толщина линии
		double *m_pLineDash;     // Параметры, если линия пунктирная
		int     m_nLineDashSize; // Размер массива m_pLineDash
		double  m_dLineDashStart;//
		int     m_nFlatness;     // Flatness Tolerance
		int     m_nLineJoin;     // Тип соединения линий
		int     m_nLineCap;      // Тип оконачния линий
		double  m_dMiterLimit;   // Miter limit
		bool    m_bStrokeAdjust; // True, автоматически обводить

		GrFont *m_pFont;            // Шрифт
		double  m_dFontSize;        // Размер шрифта
		double  m_arrTextMatrix[6]; // Матрица координатных преобразований текста
		double  m_dCharSpace;       // Расстояние между символами
		double  m_dWordSpace;       // Расстояние между словами
		double  m_dHorizScaling;    // Горизонтальное растяжение текста
		double  m_dLeading;         // Расстояние между строками
		double  m_nRise;            // Поднятие/опускание текста относительно baseline
		int     m_nRenderMode;      // Способ рисования букв

		GrPath *m_pPath;            // Массив графических Path
		double  m_dCurX;            // Координаты текущей точки
		double  m_dCurY;            // (в пользовательских координатах)
		double  m_dTextLineX;       // Координаты начальной точки, начиная с которой пишется текст
		double  m_dTextLineY;       // (в текстовых координатах)

		GrClip *m_pClip;
		double  m_dClipXMin;    //
		double  m_dClipYMin;    // Границы для clip region
		double  m_dClipXMax;    //
		double  m_dClipYMax;    //

		GrState *m_pNext;       // Следующий GrState в стэке
	};
}
#endif // _PDF_READER_GSTATE_H
