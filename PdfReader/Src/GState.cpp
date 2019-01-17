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
#include <stddef.h>
#include <string.h>
#include "MemoryUtils.h"
#include "Object.h"
#include "Array.h"
#include "Page.h"
#include "GState.h"
#include "Dict.h"
#include "Stream.h"

namespace PdfReader
{
	static inline GrColorComp ClipToBounds(GrColorComp nColor)
	{
		return (nColor < 0) ? 0 : (nColor > GrColorComp1) ? GrColorComp1 : nColor;
	}

	static inline double ClipToBounds(double dValue)
	{
		return (dValue < 0) ? 0 : (dValue > 1) ? 1 : dValue;
	}

	//-------------------------------------------------------------------------------------------------------------------------------

	struct GrBlendModeInfo
	{
		char              *sName;
		GraphicsBlendMode  eMode;
	};

	static GrBlendModeInfo c_arrsGrBlendModeNames[] =
	{
		{ "Normal", grBlendNormal },
		{ "Compatible", grBlendNormal },
		{ "Multiply", grBlendMultiply },
		{ "Screen", grBlendScreen },
		{ "Overlay", grBlendOverlay },
		{ "Darken", grBlendDarken },
		{ "Lighten", grBlendLighten },
		{ "ColorDodge", grBlendColorDodge },
		{ "ColorBurn", grBlendColorBurn },
		{ "HardLight", grBlendHardLight },
		{ "SoftLight", grBlendSoftLight },
		{ "Difference", grBlendDifference },
		{ "Exclusion", grBlendExclusion },
		{ "Hue", grBlendHue },
		{ "Saturation", grBlendSaturation },
		{ "Color", grBlendColor },
		{ "Luminosity", grBlendLuminosity }
	};

#define GrBlendModeNamesCount  ((int)((sizeof(c_arrsGrBlendModeNames) / sizeof(GrBlendModeInfo))))

	//-------------------------------------------------------------------------------------------------------------------------------

	static char *c_arrsGrColorSpaceModeNames[] =
	{
		"DeviceGray",
		"CalGray",
		"DeviceRGB",
		"CalRGB",
		"DeviceCMYK",
		"Lab",
		"ICCBased",
		"Indexed",
		"Separation",
		"DeviceN",
		"Pattern"
	};

#define GrColorSpaceModesCount ((sizeof(c_arrsGrColorSpaceModeNames) / sizeof(char *)))

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	GrColorSpace::GrColorSpace()
	{
	}

	GrColorSpace::~GrColorSpace()
	{
	}

	GrColorSpace *GrColorSpace::Parse(Object *pColorSpaceObject)
	{
		GrColorSpace *pColorSpace = NULL;

		if (pColorSpaceObject->IsName())
		{
			if (pColorSpaceObject->IsName("DeviceGray") || pColorSpaceObject->IsName("G"))
			{
				pColorSpace = new GrDeviceGrayColorSpace();
			}
			else if (pColorSpaceObject->IsName("DeviceRGB") || pColorSpaceObject->IsName("RGB"))
			{
				pColorSpace = new GrDeviceRGBColorSpace();
			}
			else if (pColorSpaceObject->IsName("DeviceCMYK") || pColorSpaceObject->IsName("CMYK"))
			{
				pColorSpace = new GrDeviceCMYKColorSpace();
			}
			else if (pColorSpaceObject->IsName("Pattern"))
			{
				pColorSpace = new GrPatternColorSpace(NULL);
			}
			else
			{
				// TO DO: Error "Bad color space"
			}
		}
		else if (pColorSpaceObject->IsArray())
		{
			Object oTemp;
			pColorSpaceObject->ArrayGet(0, &oTemp);
			if (oTemp.IsName("DeviceGray") || oTemp.IsName("G"))
			{
				pColorSpace = new GrDeviceGrayColorSpace();
			}
			else if (oTemp.IsName("DeviceRGB") || oTemp.IsName("RGB"))
			{
				pColorSpace = new GrDeviceRGBColorSpace();
			}
			else if (oTemp.IsName("DeviceCMYK") || oTemp.IsName("CMYK"))
			{
				pColorSpace = new GrDeviceCMYKColorSpace();
			}
			else if (oTemp.IsName("CalGray"))
			{
				pColorSpace = GrCalGrayColorSpace::Parse(pColorSpaceObject->GetArray());
			}
			else if (oTemp.IsName("CalRGB"))
			{
				pColorSpace = GrCalRGBColorSpace::Parse(pColorSpaceObject->GetArray());
			}
			else if (oTemp.IsName("Lab"))
			{
				pColorSpace = GrLabColorSpace::Parse(pColorSpaceObject->GetArray());
			}
			else if (oTemp.IsName("ICCBased"))
			{
				pColorSpace = GrICCBasedColorSpace::Parse(pColorSpaceObject->GetArray());
			}
			else if (oTemp.IsName("Indexed") || oTemp.IsName("I"))
			{
				pColorSpace = GrIndexedColorSpace::Parse(pColorSpaceObject->GetArray());
			}
			else if (oTemp.IsName("Separation"))
			{
				pColorSpace = GrSeparationColorSpace::Parse(pColorSpaceObject->GetArray());
			}
			else if (oTemp.IsName("DeviceN"))
			{
				pColorSpace = GrDeviceNColorSpace::Parse(pColorSpaceObject->GetArray());
			}
			else if (oTemp.IsName("Pattern"))
			{
				pColorSpace = GrPatternColorSpace::Parse(pColorSpaceObject->GetArray());
			}
			else
			{
				// TO DO: Error "Bad color space"
			}
			oTemp.Free();
		}
		else
		{
			// TO DO: Error "Bad color space - expected name or array"
		}
		return pColorSpace;
	}

	void GrColorSpace::GetDefaultRanges(double *pDecodeLow, double *pDecodeRange, int nMaxImagePixelValue)
	{
		for (int nIndex = 0; nIndex < GetComponentsCount(); ++nIndex)
		{
			pDecodeLow[nIndex] = 0;
			pDecodeRange[nIndex] = 1;
		}
	}

	int GrColorSpace::GetColorSpaceModesCount()
	{
		return GrColorSpaceModesCount;
	}

	char *GrColorSpace::GetColorSpaceModeName(int nIndex)
	{
		return c_arrsGrColorSpaceModeNames[nIndex];
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrDeviceGrayColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	GrDeviceGrayColorSpace::GrDeviceGrayColorSpace()
	{
	}

	GrDeviceGrayColorSpace::~GrDeviceGrayColorSpace()
	{
	}

	GrColorSpace *GrDeviceGrayColorSpace::Copy()
	{
		return new GrDeviceGrayColorSpace();
	}

	void GrDeviceGrayColorSpace::GetGray(GrColor *pColor, GrGray *pGray)
	{
		*pGray = ClipToBounds(pColor->arrComp[0]);
	}

	void GrDeviceGrayColorSpace::GetRGB(GrColor *pColor, GrRGB  *pRGB)
	{
		pRGB->r = pRGB->g = pRGB->b = ClipToBounds(pColor->arrComp[0]);
	}

	void GrDeviceGrayColorSpace::GetCMYK(GrColor *pColor, GrCMYK *pCMYK)
	{
		pCMYK->c = pCMYK->m = pCMYK->y = 0;
		pCMYK->k = ClipToBounds(GrColorComp1 - pColor->arrComp[0]);
	}

	void GrDeviceGrayColorSpace::GetDefaultColor(GrColor *pColor)
	{
		pColor->arrComp[0] = 0;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrCalGrayColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	GrCalGrayColorSpace::GrCalGrayColorSpace()
	{
		m_dWhiteX = m_dWhiteY = m_dWhiteZ = 1;
		m_dBlackX = m_dBlackY = m_dBlackZ = 0;
		m_dGamma = 1;
	}

	GrCalGrayColorSpace::~GrCalGrayColorSpace()
	{
	}

	GrColorSpace *GrCalGrayColorSpace::Copy()
	{
		GrCalGrayColorSpace *pColorSpace = new GrCalGrayColorSpace();
		pColorSpace->m_dWhiteX = m_dWhiteX;
		pColorSpace->m_dWhiteY = m_dWhiteY;
		pColorSpace->m_dWhiteZ = m_dWhiteZ;
		pColorSpace->m_dBlackX = m_dBlackX;
		pColorSpace->m_dBlackY = m_dBlackY;
		pColorSpace->m_dBlackZ = m_dBlackZ;
		pColorSpace->m_dGamma  = m_dGamma;
		return pColorSpace;
	}

	GrColorSpace *GrCalGrayColorSpace::Parse(Array *pArray)
	{
		Object oDict;
		pArray->Get(1, &oDict);

		if (!oDict.IsDict())
		{
			// TO DO: Error "Bad CalGray color space"
			oDict.Free();
			return NULL;
		}

		GrCalGrayColorSpace *pColorSpace = new GrCalGrayColorSpace();
		Object oDictValue;
		if (oDict.DictLookup("WhitePoint", &oDictValue)->IsArray() && oDictValue.ArrayGetLength() == 3)
		{
			Object oTemp;
			oDictValue.ArrayGet(0, &oTemp);
			pColorSpace->m_dWhiteX = oTemp.GetNum();
			oTemp.Free();

			oDictValue.ArrayGet(1, &oTemp);
			pColorSpace->m_dWhiteY = oTemp.GetNum();
			oTemp.Free();

			oDictValue.ArrayGet(2, &oTemp);
			pColorSpace->m_dWhiteZ = oTemp.GetNum();
			oTemp.Free();
		}
		oDictValue.Free();

		if (oDict.DictLookup("BlackPoint", &oDictValue)->IsArray() && oDictValue.ArrayGetLength() == 3)
		{
			Object oTemp;
			oDictValue.ArrayGet(0, &oTemp);
			pColorSpace->m_dBlackX = oTemp.GetNum();
			oTemp.Free();

			oDictValue.ArrayGet(1, &oTemp);
			pColorSpace->m_dBlackY = oTemp.GetNum();
			oTemp.Free();

			oDictValue.ArrayGet(2, &oTemp);
			pColorSpace->m_dBlackZ = oTemp.GetNum();
			oTemp.Free();
		}
		oDictValue.Free();

		if (oDict.DictLookup("Gamma", &oDictValue)->IsNum())
		{
			pColorSpace->m_dGamma = oDictValue.GetNum();
		}
		oDictValue.Free();

		oDict.Free();
		return pColorSpace;
	}

	void GrCalGrayColorSpace::GetGray(GrColor *pColor, GrGray *pGray)
	{
		*pGray = ClipToBounds(pColor->arrComp[0]);
	}

	void GrCalGrayColorSpace::GetRGB(GrColor *pColor, GrRGB  *pRGB)
	{
		pRGB->r = pRGB->g = pRGB->b = ClipToBounds(pColor->arrComp[0]);
	}

	void GrCalGrayColorSpace::GetCMYK(GrColor *pColor, GrCMYK *pCMYK)
	{
		pCMYK->c = pCMYK->m = pCMYK->y = 0;
		pCMYK->k = ClipToBounds(GrColorComp1 - pColor->arrComp[0]);
	}

	void GrCalGrayColorSpace::GetDefaultColor(GrColor *pColor)
	{
		pColor->arrComp[0] = 0;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrDeviceRGBColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	GrDeviceRGBColorSpace::GrDeviceRGBColorSpace()
	{
	}

	GrDeviceRGBColorSpace::~GrDeviceRGBColorSpace()
	{
	}

	GrColorSpace *GrDeviceRGBColorSpace::Copy()
	{
		return new GrDeviceRGBColorSpace();
	}

	void GrDeviceRGBColorSpace::GetGray(GrColor *pColor, GrGray *pGray)
	{
		*pGray = ClipToBounds((GrColorComp)(0.3  * pColor->arrComp[0] + 0.59 * pColor->arrComp[1] + 0.11 * pColor->arrComp[2] + 0.5));
	}

	void GrDeviceRGBColorSpace::GetRGB(GrColor *pColor, GrRGB  *pRGB)
	{
		pRGB->r = ClipToBounds(pColor->arrComp[0]);
		pRGB->g = ClipToBounds(pColor->arrComp[1]);
		pRGB->b = ClipToBounds(pColor->arrComp[2]);
	}

	void GrDeviceRGBColorSpace::GetCMYK(GrColor *pColor, GrCMYK *pCMYK)
	{
		GrColorComp nC = ClipToBounds(GrColorComp1 - pColor->arrComp[0]);
		GrColorComp nM = ClipToBounds(GrColorComp1 - pColor->arrComp[1]);
		GrColorComp nY = ClipToBounds(GrColorComp1 - pColor->arrComp[2]);
		GrColorComp nK = nC;

		if (nM < nK)
		{
			nK = nM;
		}
		if (nY < nK)
		{
			nK = nY;
		}
		pCMYK->c = nC - nK;
		pCMYK->m = nM - nK;
		pCMYK->y = nY - nK;
		pCMYK->k = nK;
	}

	void GrDeviceRGBColorSpace::GetDefaultColor(GrColor *pColor)
	{
		pColor->arrComp[0] = 0;
		pColor->arrComp[1] = 0;
		pColor->arrComp[2] = 0;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrCalRGBColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	GrCalRGBColorSpace::GrCalRGBColorSpace()
	{
		m_dWhiteX = m_dWhiteY = m_dWhiteZ = 1;
		m_dBlackX = m_dBlackY = m_dBlackZ = 0;
		m_dGammaR = m_dGammaG = m_dGammaB = 1;
		m_arrdMatrix[0] = 1; m_arrdMatrix[1] = 0; m_arrdMatrix[2] = 0;
		m_arrdMatrix[3] = 0; m_arrdMatrix[4] = 1; m_arrdMatrix[5] = 0;
		m_arrdMatrix[6] = 0; m_arrdMatrix[7] = 0; m_arrdMatrix[8] = 1;
	}

	GrCalRGBColorSpace::~GrCalRGBColorSpace()
	{
	}

	GrColorSpace *GrCalRGBColorSpace::Copy()
	{
		GrCalRGBColorSpace *pColorSpace = new GrCalRGBColorSpace();
		pColorSpace->m_dWhiteX = m_dWhiteX;
		pColorSpace->m_dWhiteY = m_dWhiteY;
		pColorSpace->m_dWhiteZ = m_dWhiteZ;
		pColorSpace->m_dBlackX = m_dBlackX;
		pColorSpace->m_dBlackY = m_dBlackY;
		pColorSpace->m_dBlackZ = m_dBlackZ;
		pColorSpace->m_dGammaR = m_dGammaR;
		pColorSpace->m_dGammaG = m_dGammaG;
		pColorSpace->m_dGammaB = m_dGammaB;
		for (int nIndex = 0; nIndex < 9; ++nIndex)
		{
			pColorSpace->m_arrdMatrix[nIndex] = m_arrdMatrix[nIndex];
		}
		return pColorSpace;
	}

	GrColorSpace *GrCalRGBColorSpace::Parse(Array *pArray)
	{
		Object oDict;
		pArray->Get(1, &oDict);
		if (!oDict.IsDict())
		{
			// TO DO: Error "Bad CalRGB color space"
			oDict.Free();
			return NULL;
		}

		GrCalRGBColorSpace *pColorSpace = new GrCalRGBColorSpace();
		Object oDictValue;
		if (oDict.DictLookup("WhitePoint", &oDictValue)->IsArray() && oDictValue.ArrayGetLength() == 3)
		{
			Object oTemp;
			oDictValue.ArrayGet(0, &oTemp);
			pColorSpace->m_dWhiteX = oTemp.GetNum();
			oTemp.Free();

			oDictValue.ArrayGet(1, &oTemp);
			pColorSpace->m_dWhiteY = oTemp.GetNum();
			oTemp.Free();

			oDictValue.ArrayGet(2, &oTemp);
			pColorSpace->m_dWhiteZ = oTemp.GetNum();
			oTemp.Free();
		}
		oDictValue.Free();

		if (oDict.DictLookup("BlackPoint", &oDictValue)->IsArray() && oDictValue.ArrayGetLength() == 3)
		{
			Object oTemp;
			oDictValue.ArrayGet(0, &oTemp);
			pColorSpace->m_dBlackX = oTemp.GetNum();
			oTemp.Free();

			oDictValue.ArrayGet(1, &oTemp);
			pColorSpace->m_dBlackY = oTemp.GetNum();
			oTemp.Free();

			oDictValue.ArrayGet(2, &oTemp);
			pColorSpace->m_dBlackZ = oTemp.GetNum();
			oTemp.Free();
		}
		oDictValue.Free();

		if (oDict.DictLookup("Gamma", &oDictValue)->IsArray() && oDictValue.ArrayGetLength() == 3)
		{
			Object oTemp;
			oDictValue.ArrayGet(0, &oTemp);
			pColorSpace->m_dGammaR = oTemp.GetNum();
			oTemp.Free();

			oDictValue.ArrayGet(1, &oTemp);
			pColorSpace->m_dGammaG = oTemp.GetNum();
			oTemp.Free();

			oDictValue.ArrayGet(2, &oTemp);
			pColorSpace->m_dGammaB = oTemp.GetNum();
			oTemp.Free();
		}
		oDictValue.Free();

		if (oDict.DictLookup("Matrix", &oDictValue)->IsArray() && oDictValue.ArrayGetLength() == 9)
		{
			for (int nIndex = 0; nIndex < 9; ++nIndex)
			{
				Object oTemp;
				oDictValue.ArrayGet(nIndex, &oTemp);
				pColorSpace->m_arrdMatrix[nIndex] = oTemp.GetNum();
				oTemp.Free();
			}
		}
		oDictValue.Free();
		oDict.Free();
		return pColorSpace;
	}

	void GrCalRGBColorSpace::GetGray(GrColor *pColor, GrGray *pGray)
	{
		*pGray = ClipToBounds((GrColorComp)(0.299 * pColor->arrComp[0] + 0.587 * pColor->arrComp[1] + 0.114 * pColor->arrComp[2] + 0.5));
	}

	void GrCalRGBColorSpace::GetRGB(GrColor *pColor, GrRGB  *pRGB)
	{
		pRGB->r = ClipToBounds(pColor->arrComp[0]);
		pRGB->g = ClipToBounds(pColor->arrComp[1]);
		pRGB->b = ClipToBounds(pColor->arrComp[2]);
	}

	void GrCalRGBColorSpace::GetCMYK(GrColor *pColor, GrCMYK *pCMYK)
	{
		GrColorComp nC = ClipToBounds(GrColorComp1 - pColor->arrComp[0]);
		GrColorComp nM = ClipToBounds(GrColorComp1 - pColor->arrComp[1]);
		GrColorComp nY = ClipToBounds(GrColorComp1 - pColor->arrComp[2]);
		GrColorComp nK = nC;

		if (nM < nK)
		{
			nK = nM;
		}
		if (nY < nK)
		{
			nK = nY;
		}
		pCMYK->c = nC - nK;
		pCMYK->m = nM - nK;
		pCMYK->y = nY - nK;
		pCMYK->k = nK;
	}

	void GrCalRGBColorSpace::GetDefaultColor(GrColor *pColor)
	{
		pColor->arrComp[0] = 0;
		pColor->arrComp[1] = 0;
		pColor->arrComp[2] = 0;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrDeviceCMYKColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	GrDeviceCMYKColorSpace::GrDeviceCMYKColorSpace()
	{
	}

	GrDeviceCMYKColorSpace::~GrDeviceCMYKColorSpace()
	{
	}

	GrColorSpace *GrDeviceCMYKColorSpace::Copy()
	{
		return new GrDeviceCMYKColorSpace();
	}

	void GrDeviceCMYKColorSpace::GetGray(GrColor *pColor, GrGray *pGray)
	{
		*pGray = ClipToBounds((GrColorComp)(GrColorComp1 - pColor->arrComp[3] - 0.3  * pColor->arrComp[0] - 0.59 * pColor->arrComp[1] - 0.11 * pColor->arrComp[2] + 0.5));
	}

	void GrDeviceCMYKColorSpace::GetRGB(GrColor *pColor, GrRGB  *pRGB)
	{
		double dR = 0, dG = 0, dB = 0, dX = 0;

		double dC = ColorToDouble(pColor->arrComp[0]);
		double dM = ColorToDouble(pColor->arrComp[1]);
		double dY = ColorToDouble(pColor->arrComp[2]);
		double dK = ColorToDouble(pColor->arrComp[3]);

		double dRevC = 1 - dC;
		double dRevM = 1 - dM;
		double dRevY = 1 - dY;
		double dRevK = 1 - dK;

		//                                     C M Y K
		dX = dRevC * dRevM * dRevY * dRevK; // 0 0 0 0
		dR = dG = dB = dX;

		dX = dRevC * dRevM * dRevY * dK;    // 0 0 0 1
		dR += 0.1373 * dX;
		dG += 0.1216 * dX;
		dB += 0.1255 * dX;

		dX = dRevC * dRevM * dY  * dRevK;   // 0 0 1 0
		dR += dX;
		dG += 0.9490 * dX;

		dX = dRevC * dRevM * dY  * dK;      // 0 0 1 1
		dR += 0.1098 * dX;
		dG += 0.1020 * dX;

		dX = dRevC * dM  * dRevY * dRevK;   // 0 1 0 0
		dR += 0.9255 * dX;
		dB += 0.5490 * dX;

		dX = dRevC * dM  * dRevY * dK;      // 0 1 0 1
		dR += 0.1412 * dX;

		dX = dRevC * dM  * dY  * dRevK;     // 0 1 1 0
		dR += 0.9294 * dX;
		dG += 0.1098 * dX;
		dB += 0.1412 * dX;

		dX = dRevC * dM  * dY  * dK;        // 0 1 1 1
		dR += 0.1333 * dX;

		dX = dC  * dRevM * dRevY * dRevK;   // 1 0 0 0
		dG += 0.6784 * dX;
		dB += 0.9373 * dX;

		dX = dC  * dRevM * dRevY * dK;      // 1 0 0 1
		dG += 0.0588 * dX;
		dB += 0.1412 * dX;

		dX = dC  * dRevM * dY  * dRevK;     // 1 0 1 0
		dG += 0.6510 * dX;
		dB += 0.3137 * dX;

		dX = dC  * dRevM * dY  * dK;        // 1 0 1 1
		dG += 0.0745 * dX;

		dX = dC  * dM  * dRevY * dRevK;     // 1 1 0 0
		dR += 0.1804 * dX;
		dG += 0.1922 * dX;
		dB += 0.5725 * dX;

		dX = dC  * dM  * dRevY * dK;        // 1 1 0 1
		dB += 0.0078 * dX;

		dX = dC  * dM  * dY  * dRevK;       // 1 1 1 0
		dR += 0.2118 * dX;
		dG += 0.2119 * dX;
		dB += 0.2235 * dX;

		pRGB->r = ClipToBounds(DoubleToColor(dR));
		pRGB->g = ClipToBounds(DoubleToColor(dG));
		pRGB->b = ClipToBounds(DoubleToColor(dB));
	}

	void GrDeviceCMYKColorSpace::GetCMYK(GrColor *pColor, GrCMYK *pCMYK)
	{
		pCMYK->c = ClipToBounds(pColor->arrComp[0]);
		pCMYK->m = ClipToBounds(pColor->arrComp[1]);
		pCMYK->y = ClipToBounds(pColor->arrComp[2]);
		pCMYK->k = ClipToBounds(pColor->arrComp[3]);
	}

	void GrDeviceCMYKColorSpace::GetDefaultColor(GrColor *pColor)
	{
		pColor->arrComp[0] = 0;
		pColor->arrComp[1] = 0;
		pColor->arrComp[2] = 0;
		pColor->arrComp[3] = GrColorComp1;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrLabColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	// Это обратная матрица к матрице LMN, данной в примере 4.10 в спецификации
	// PostScript Language Reference, Third Edition.
	static double c_arrLMNReverse[3][3] =
	{
		{ 3.240449, -1.537136, -0.498531 },
		{ -0.969265, 1.876011, 0.041556 },
		{ 0.055643, -0.204026, 1.057229 }
	};

	GrLabColorSpace::GrLabColorSpace()
	{
		m_dWhiteX = m_dWhiteY = m_dWhiteZ = 1;
		m_dBlackX = m_dBlackY = m_dBlackZ = 0;
		m_dMinA = m_dMinB = -100;
		m_dMaxA = m_dMaxB = 100;
	}

	GrLabColorSpace::~GrLabColorSpace()
	{
	}

	GrColorSpace *GrLabColorSpace::Copy()
	{
		GrLabColorSpace *pColorSpace = new GrLabColorSpace();
		pColorSpace->m_dWhiteX = m_dWhiteX;
		pColorSpace->m_dWhiteY = m_dWhiteY;
		pColorSpace->m_dWhiteZ = m_dWhiteZ;
		pColorSpace->m_dBlackX = m_dBlackX;
		pColorSpace->m_dBlackY = m_dBlackY;
		pColorSpace->m_dBlackZ = m_dBlackZ;
		pColorSpace->m_dMinA = m_dMinA;
		pColorSpace->m_dMaxA = m_dMaxA;
		pColorSpace->m_dMinB = m_dMinB;
		pColorSpace->m_dMaxB = m_dMaxB;
		pColorSpace->m_dMultR = m_dMultR;
		pColorSpace->m_dMultG = m_dMultG;
		pColorSpace->m_dMultB = m_dMultB;
		return pColorSpace;
	}

	GrColorSpace *GrLabColorSpace::Parse(Array *pArray)
	{
		Object obj1, obj2, obj3;

		Object oDict;
		pArray->Get(1, &oDict);
		if (!oDict.IsDict())
		{
			// TO DO: Error "Bad Lab color space"
			oDict.Free();
			return NULL;
		}
		GrLabColorSpace *pColorSpace = new GrLabColorSpace();

		Object oDictItem;
		if (oDict.DictLookup("WhitePoint", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 3)
		{
			Object oTemp;
			oDictItem.ArrayGet(0, &oTemp);
			pColorSpace->m_dWhiteX = oTemp.GetNum();
			oTemp.Free();

			oDictItem.ArrayGet(1, &oTemp);
			pColorSpace->m_dWhiteY = oTemp.GetNum();
			oTemp.Free();

			oDictItem.ArrayGet(2, &oTemp);
			pColorSpace->m_dWhiteZ = oTemp.GetNum();
			oTemp.Free();
		}
		oDictItem.Free();

		if (oDict.DictLookup("BlackPoint", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 3)
		{
			Object oTemp;
			oDictItem.ArrayGet(0, &oTemp);
			pColorSpace->m_dBlackX = oTemp.GetNum();
			oTemp.Free();

			oDictItem.ArrayGet(1, &oTemp);
			pColorSpace->m_dBlackY = oTemp.GetNum();
			oTemp.Free();

			oDictItem.ArrayGet(2, &oTemp);
			pColorSpace->m_dBlackZ = oTemp.GetNum();
			oTemp.Free();
		}
		oDictItem.Free();

		if (oDict.DictLookup("Range", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 4)
		{
			Object oTemp;
			oDictItem.ArrayGet(0, &oTemp);
			pColorSpace->m_dMinA = oTemp.GetNum();
			oTemp.Free();

			oDictItem.ArrayGet(1, &oTemp);
			pColorSpace->m_dMaxA = oTemp.GetNum();
			oTemp.Free();

			oDictItem.ArrayGet(2, &oTemp);
			pColorSpace->m_dMinB = oTemp.GetNum();
			oTemp.Free();

			oDictItem.ArrayGet(3, &oTemp);
			pColorSpace->m_dMaxB = oTemp.GetNum();
			oTemp.Free();
		}
		oDictItem.Free();
		oDict.Free();


		pColorSpace->m_dMultR = 1 / (c_arrLMNReverse[0][0] * pColorSpace->m_dWhiteX + c_arrLMNReverse[0][1] * pColorSpace->m_dWhiteY + c_arrLMNReverse[0][2] * pColorSpace->m_dWhiteZ);
		pColorSpace->m_dMultG = 1 / (c_arrLMNReverse[1][0] * pColorSpace->m_dWhiteX + c_arrLMNReverse[1][1] * pColorSpace->m_dWhiteY + c_arrLMNReverse[1][2] * pColorSpace->m_dWhiteZ);
		pColorSpace->m_dMultB = 1 / (c_arrLMNReverse[2][0] * pColorSpace->m_dWhiteX + c_arrLMNReverse[2][1] * pColorSpace->m_dWhiteY + c_arrLMNReverse[2][2] * pColorSpace->m_dWhiteZ);

		return pColorSpace;
	}

	void GrLabColorSpace::GetGray(GrColor *pColor, GrGray *pGray)
	{
		GrRGB oRGB;

		GetRGB(pColor, &oRGB);
		*pGray = ClipToBounds((GrColorComp)(0.299 * oRGB.r + 0.587 * oRGB.g + 0.114 * oRGB.b + 0.5));
	}

	void GrLabColorSpace::GetRGB(GrColor *pColor, GrRGB  *pRGB)
	{
		double dX, dY, dZ;

		// L*a*b* -> CIE 1931 XYZ 
		double dTemp1 = (ColorToDouble(pColor->arrComp[0]) + 16) / 116;
		double dTemp2 = dTemp1 + ColorToDouble(pColor->arrComp[1]) / 500;
		if (dTemp2 >= (6.0 / 29.0))
		{
			dX = dTemp2 * dTemp2 * dTemp2;
		}
		else
		{
			dX = (108.0 / 841.0) * (dTemp2 - (4.0 / 29.0));
		}
		dX *= m_dWhiteX;
		if (dTemp1 >= (6.0 / 29.0))
		{
			dY = dTemp1 * dTemp1 * dTemp1;
		}
		else
		{
			dY = (108.0 / 841.0) * (dTemp1 - (4.0 / 29.0));
		}
		dY *= m_dWhiteY;
		dTemp2 = dTemp1 - ColorToDouble(pColor->arrComp[2]) / 200;
		if (dTemp2 >= (6.0 / 29.0))
		{
			dZ = dTemp2 * dTemp2 * dTemp2;
		}
		else
		{
			dZ = (108.0 / 841.0) * (dTemp2 - (4.0 / 29.0));
		}
		dZ *= m_dWhiteZ;

		// XYZ -> RGB ( учитывая гамма коррекцию )
		double dR = c_arrLMNReverse[0][0] * dX + c_arrLMNReverse[0][1] * dY + c_arrLMNReverse[0][2] * dZ;
		double dG = c_arrLMNReverse[1][0] * dX + c_arrLMNReverse[1][1] * dY + c_arrLMNReverse[1][2] * dZ;
		double dB = c_arrLMNReverse[2][0] * dX + c_arrLMNReverse[2][1] * dY + c_arrLMNReverse[2][2] * dZ;
		pRGB->r = DoubleToColor(pow(ClipToBounds(dR * m_dMultR), 0.5));
		pRGB->g = DoubleToColor(pow(ClipToBounds(dG * m_dMultG), 0.5));
		pRGB->b = DoubleToColor(pow(ClipToBounds(dB * m_dMultB), 0.5));
	}

	void GrLabColorSpace::GetCMYK(GrColor *pColor, GrCMYK *pCMYK)
	{
		GrRGB oRGB;

		GetRGB(pColor, &oRGB);
		GrColorComp nC = ClipToBounds(GrColorComp1 - oRGB.r);
		GrColorComp nM = ClipToBounds(GrColorComp1 - oRGB.g);
		GrColorComp nY = ClipToBounds(GrColorComp1 - oRGB.b);
		GrColorComp nK = nC;

		if (nM < nK)
		{
			nK = nM;
		}
		if (nY < nK)
		{
			nK = nY;
		}
		pCMYK->c = nC - nK;
		pCMYK->m = nM - nK;
		pCMYK->y = nY - nK;
		pCMYK->k = nK;
	}

	void GrLabColorSpace::GetDefaultColor(GrColor *pColor)
	{
		pColor->arrComp[0] = 0;
		if (m_dMinA > 0)
		{
			pColor->arrComp[1] = DoubleToColor(m_dMinA);
		}
		else if (m_dMaxA < 0)
		{
			pColor->arrComp[1] = DoubleToColor(m_dMaxA);
		}
		else
		{
			pColor->arrComp[1] = 0;
		}
		if (m_dMinB > 0)
		{
			pColor->arrComp[2] = DoubleToColor(m_dMinB);
		}
		else if (m_dMaxB < 0)
		{
			pColor->arrComp[2] = DoubleToColor(m_dMaxB);
		}
		else
		{
			pColor->arrComp[2] = 0;
		}
	}

	void GrLabColorSpace::GetDefaultRanges(double *pDecodeLow, double *pDecodeRange, int nMaxImagePixelValue)
	{
		pDecodeLow[0]   = 0;
		pDecodeRange[0] = 100;
		pDecodeLow[1]   = m_dMinA;
		pDecodeRange[1] = m_dMaxA - m_dMinA;
		pDecodeLow[2]   = m_dMinB;
		pDecodeRange[2] = m_dMaxB - m_dMinB;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrICCBasedColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	GrICCBasedColorSpace::GrICCBasedColorSpace(int nComponentsCount, GrColorSpace *pAlternate, Ref *pICCProfileStream)
	{
		m_nComponentsCount = nComponentsCount;
		m_pAlternate = pAlternate;
		m_oICCProfileStream = *pICCProfileStream;
		m_arrdRangeMin[0] = m_arrdRangeMin[1] = m_arrdRangeMin[2] = m_arrdRangeMin[3] = 0;
		m_arrdRangeMax[0] = m_arrdRangeMax[1] = m_arrdRangeMax[2] = m_arrdRangeMax[3] = 1;
	}

	GrICCBasedColorSpace::~GrICCBasedColorSpace()
	{
		if (m_pAlternate)
			delete m_pAlternate;
	}

	GrColorSpace *GrICCBasedColorSpace::Copy()
	{
		GrICCBasedColorSpace *pColorSpaces = new GrICCBasedColorSpace(m_nComponentsCount, m_pAlternate->Copy(), &m_oICCProfileStream);
		for (int nIndex = 0; nIndex < 4; ++nIndex)
		{
			pColorSpaces->m_arrdRangeMin[nIndex] = m_arrdRangeMin[nIndex];
			pColorSpaces->m_arrdRangeMax[nIndex] = m_arrdRangeMax[nIndex];
		}
		return pColorSpaces;
	}

	GrColorSpace *GrICCBasedColorSpace::Parse(Array *pArray)
	{
		Ref oICCProfileStream;
		Object oStream;
		pArray->GetCopy(1, &oStream);
		if (oStream.IsRef())
		{
			oICCProfileStream = oStream.GetRef();
		}
		else
		{
			oICCProfileStream.nNum = 0;
			oICCProfileStream.nGen = 0;
		}
		oStream.Free();
		pArray->Get(1, &oStream);
		if (!oStream.IsStream())
		{
			// TO DO: Error "Bad ICCBased color space (stream)"
			oStream.Free();
			return NULL;
		}
		Dict *pDict = oStream.StreamGetDict();

		Object oDictItem;
		if (!pDict->Search("N", &oDictItem)->IsInt())
		{
			// TO DO: Error "Bad ICCBased color space (N)"
			oDictItem.Free();
			oStream.Free();
			return NULL;
		}
		int nCompCount = oDictItem.GetInt();
		oDictItem.Free();

		if (nCompCount > GrColorMaxComps)
		{
			// TO DO: Error "ICCBased color space with too many (%d > %d) components"
			nCompCount = GrColorMaxComps;
		}

		GrColorSpace *pAlternativeCS;
		if (pDict->Search("Alternate", &oDictItem)->IsNull() || !(pAlternativeCS = GrColorSpace::Parse(&oDictItem)))
		{
			switch (nCompCount)
			{
				case 1:
				pAlternativeCS = new GrDeviceGrayColorSpace();
				break;
				case 3:
				pAlternativeCS = new GrDeviceRGBColorSpace();
				break;
				case 4:
				pAlternativeCS = new GrDeviceCMYKColorSpace();
				break;
				default:
				// TO DO: Error "Bad ICCBased color space - invalid N"
				oDictItem.Free();
				oStream.Free();
				return NULL;
			}
		}
		oDictItem.Free();
		GrICCBasedColorSpace *pColorSpace = new GrICCBasedColorSpace(nCompCount, pAlternativeCS, &oICCProfileStream);

		if (pDict->Search("Range", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 2 * nCompCount)
		{
			for (int nIndex = 0; nIndex < nCompCount; ++nIndex)
			{
				Object oTemp;
				oDictItem.ArrayGet(2 * nIndex, &oTemp);
				pColorSpace->m_arrdRangeMin[nIndex] = oTemp.GetNum();
				oTemp.Free();

				oDictItem.ArrayGet(2 * nIndex + 1, &oTemp);
				pColorSpace->m_arrdRangeMax[nIndex] = oTemp.GetNum();
				oTemp.Free();
			}
		}
		oDictItem.Free();
		oStream.Free();
		return pColorSpace;
	}

	void GrICCBasedColorSpace::GetGray(GrColor *pColor, GrGray *pGray)
	{
		m_pAlternate->GetGray(pColor, pGray);
	}

	void GrICCBasedColorSpace::GetRGB(GrColor *pColor, GrRGB  *pRGB)
	{
		m_pAlternate->GetRGB(pColor, pRGB);
	}

	void GrICCBasedColorSpace::GetCMYK(GrColor *pColor, GrCMYK *pCMYK)
	{
		m_pAlternate->GetCMYK(pColor, pCMYK);
	}

	void GrICCBasedColorSpace::GetDefaultColor(GrColor *pColor)
	{
		for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
		{
			if (m_arrdRangeMin[nIndex] > 0)
			{
				pColor->arrComp[nIndex] = DoubleToColor(m_arrdRangeMin[nIndex]);
			}
			else if (m_arrdRangeMax[nIndex] < 0)
			{
				pColor->arrComp[nIndex] = DoubleToColor(m_arrdRangeMax[nIndex]);
			}
			else
			{
				pColor->arrComp[nIndex] = 0;
			}
		}
	}

	void GrICCBasedColorSpace::GetDefaultRanges(double *pDecodeLow, double *pDecodeRange, int nMaxImagePixelValue)
	{
		m_pAlternate->GetDefaultRanges(pDecodeLow, pDecodeRange, nMaxImagePixelValue);

#if 0
		// Так должно работать, но некоторые PDF фалй не содержат корректных данных в полях Range
		// в словаре ICCBased.

		for ( int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex ) 
		{
			pDecodeLow[nIndex] = m_arrdRangeMin[nIndex];
			pDecodeRange[nIndex] = m_arrdRangeMax[nIndex] - m_arrdRangeMin[nIndex];
		}
#endif
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrIndexedColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	GrIndexedColorSpace::GrIndexedColorSpace(GrColorSpace *pBase, int nHival)
	{
		m_pBase  = pBase;
		m_nHival = nHival;
		m_pLookup = (unsigned char *)MemUtilsMallocArray((m_nHival + 1) * m_pBase->GetComponentsCount(), sizeof(unsigned char));
	}

	GrIndexedColorSpace::~GrIndexedColorSpace()
	{
		if (m_pBase)
			delete m_pBase;

		MemUtilsFree(m_pLookup);
	}

	GrColorSpace *GrIndexedColorSpace::Copy()
	{
		GrIndexedColorSpace *pColorSpace = new GrIndexedColorSpace(m_pBase->Copy(), m_nHival);
		memcpy(pColorSpace->m_pLookup, m_pLookup, (m_nHival + 1) * m_pBase->GetComponentsCount() * sizeof(unsigned char));
		return pColorSpace;
	}

	GrColorSpace *GrIndexedColorSpace::Parse(Array *pArray)
	{
		// [ /Indexed base hival lookup ] - массив из 4 элементов должен быть
		if (pArray->GetCount() != 4)
		{
			// TO DO: Error "Bad Indexed color space"
			return NULL;
		}

		Object oArrayItem;
		pArray->Get(1, &oArrayItem);
		GrColorSpace *pBase = NULL;
		if (!(pBase = GrColorSpace::Parse(&oArrayItem)))
		{
			// TO DO: Error "Bad Indexed color space (base color space)"
			oArrayItem.Free();
			return NULL;
		}
		oArrayItem.Free();

		if (!pArray->Get(2, &oArrayItem)->IsInt())
		{
			// TO DO: Error "Bad Indexed color space (hival)"
			if (pBase)
				delete pBase;
			oArrayItem.Free();
			return NULL;
		}
		int nHival = oArrayItem.GetInt();

		if (nHival < 0 || nHival > 255) // По спецификации PDF значение hival должно лежать в отрезке [0,255]
		{
			// TO DO: Error "Bad Indexed color space (invalid hival value)"
			if (pBase)
				delete pBase;
			oArrayItem.Free();
			return NULL;
		}
		oArrayItem.Free();
		GrIndexedColorSpace *pColorSpace = new GrIndexedColorSpace(pBase, nHival);

		pArray->Get(3, &oArrayItem);
		int nCompCount = pBase->GetComponentsCount();

		if (oArrayItem.IsStream())
		{
			oArrayItem.StreamReset();
			for (int nIndex = 0; nIndex <= nHival; ++nIndex)
			{
				for (int nComp = 0; nComp < nCompCount; ++nComp)
				{
					int nChar = 0;
					if ((nChar = oArrayItem.StreamGetChar()) == EOF)
					{
						// TO DO: Error "Bad Indexed color space (lookup table stream too short)"
						if (pColorSpace)
							delete pColorSpace;
						oArrayItem.Free();
						return NULL;
					}
					pColorSpace->m_pLookup[nIndex * nCompCount + nComp] = (unsigned char)nChar;
				}
			}
			oArrayItem.StreamClose();
		}
		else if (oArrayItem.IsString())
		{
			if (oArrayItem.GetString()->GetLength() < (nHival + 1) * nCompCount)
			{
				// TO DO: Error "Bad Indexed color space (lookup table string too short)"
				if (pColorSpace)
					delete pColorSpace;
				oArrayItem.Free();
				return NULL;
			}
			char *sString = oArrayItem.GetString()->GetBuffer();
			for (int nIndex = 0; nIndex <= nHival; ++nIndex)
			{
				for (int nComp = 0; nComp < nCompCount; ++nComp)
				{
					pColorSpace->m_pLookup[nIndex * nCompCount + nComp] = (unsigned char)*sString++;
				}
			}
		}
		else
		{
			// TO DO: Error "Bad Indexed color space (lookup table)"
			if (pColorSpace)
				delete pColorSpace;
			oArrayItem.Free();
			return NULL;
		}
		oArrayItem.Free();
		return pColorSpace;
	}

	GrColor *GrIndexedColorSpace::MapColorToBase(GrColor *pColor, GrColor *pBaseColor)
	{
		double arrLow[GrColorMaxComps], arrRange[GrColorMaxComps];

		int nCompsCount = m_pBase->GetComponentsCount();
		m_pBase->GetDefaultRanges(arrLow, arrRange, m_nHival);
		unsigned char *pLookup = &m_pLookup[(int)(ColorToDouble(pColor->arrComp[0]) + 0.5) * nCompsCount];
		for (int nIndex = 0; nIndex < nCompsCount; ++nIndex)
		{
			pBaseColor->arrComp[nIndex] = DoubleToColor(arrLow[nIndex] + (pLookup[nIndex] / 255.0) * arrRange[nIndex]);
		}
		return pBaseColor;
	}

	void GrIndexedColorSpace::GetGray(GrColor *pColor, GrGray *pGray)
	{
		GrColor oTempColor;

		m_pBase->GetGray(MapColorToBase(pColor, &oTempColor), pGray);
	}

	void GrIndexedColorSpace::GetRGB(GrColor *pColor, GrRGB  *pRGB)
	{
		GrColor oTempColor;

		m_pBase->GetRGB(MapColorToBase(pColor, &oTempColor), pRGB);
	}

	void GrIndexedColorSpace::GetCMYK(GrColor *pColor, GrCMYK *pCMYK)
	{
		GrColor oTempColor;

		m_pBase->GetCMYK(MapColorToBase(pColor, &oTempColor), pCMYK);
	}

	void GrIndexedColorSpace::GetDefaultColor(GrColor *pColor)
	{
		pColor->arrComp[0] = 0;
	}

	void GrIndexedColorSpace::GetDefaultRanges(double *pDecodeLow, double *pDecodeRange, int nMaxImagePixelValue)
	{
		pDecodeLow[0]   = 0;
		pDecodeRange[0] = nMaxImagePixelValue;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrSeparationColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	GrSeparationColorSpace::GrSeparationColorSpace(StringExt *seName, GrColorSpace *pAlternate, Function *pFunction)
	{
		m_seName          = seName;
		m_pAlternateSpace = pAlternate;
		m_pFunction       = pFunction;
		m_bNonMarking     = !m_seName->Compare("None");
	}

	GrSeparationColorSpace::~GrSeparationColorSpace()
	{
		if (m_seName)
			delete m_seName;
		if (m_pAlternateSpace)
			delete m_pAlternateSpace;
		if (m_pFunction)
			delete m_pFunction;
	}

	GrColorSpace *GrSeparationColorSpace::Copy()
	{
		return new GrSeparationColorSpace(m_seName->Copy(), m_pAlternateSpace->Copy(), m_pFunction->Copy());
	}

	GrColorSpace *GrSeparationColorSpace::Parse(Array *pArray)
	{
		// [ /Separation name alternateSpace tintTransform ] - должен быть массив из 4 элементов
		if (pArray->GetCount() != 4)
		{
			// TO DO: Error "Bad Separation color space"
			return NULL;
		}

		Object oArrayItem;
		if (!pArray->Get(1, &oArrayItem)->IsName())
		{
			// TO DO: Error "Bad Separation color space (name)"
			oArrayItem.Free();
			return NULL;
		}

		StringExt *seName = new StringExt(oArrayItem.GetName());
		oArrayItem.Free();

		pArray->Get(2, &oArrayItem);
		GrColorSpace *pAlternate = NULL;
		if (!(pAlternate = GrColorSpace::Parse(&oArrayItem)))
		{
			// TO DO: Error "Bad Separation color space (alternate color space)"
			if (seName)
				delete seName;
			oArrayItem.Free();
			return NULL;
		}
		oArrayItem.Free();

		pArray->Get(3, &oArrayItem);
		Function *pFunction = NULL;
		if (!(pFunction = Function::Parse(&oArrayItem)))
		{
			if (pAlternate)
				delete pAlternate;
			if (seName)
				delete seName;
			oArrayItem.Free();
			return NULL;
		}
		oArrayItem.Free();

		GrSeparationColorSpace *pColorSpace = new GrSeparationColorSpace(seName, pAlternate, pFunction);
		return pColorSpace;
	}

	void GrSeparationColorSpace::GetGray(GrColor *pColor, GrGray *pGray)
	{
		double arrDestColor[GrColorMaxComps];
		GrColor oAltColor;

		double dSrcColor = ColorToDouble(pColor->arrComp[0]);
		m_pFunction->Transform(&dSrcColor, arrDestColor);
		for (int nIndex = 0; nIndex < m_pAlternateSpace->GetComponentsCount(); ++nIndex)
		{
			oAltColor.arrComp[nIndex] = DoubleToColor(arrDestColor[nIndex]);
		}
		m_pAlternateSpace->GetGray(&oAltColor, pGray);
	}

	void GrSeparationColorSpace::GetRGB(GrColor *pColor, GrRGB  *pRGB)
	{
		double arrDestColor[GrColorMaxComps];
		GrColor oAltColor;

		double dSrcColor = ColorToDouble(pColor->arrComp[0]);
		m_pFunction->Transform(&dSrcColor, arrDestColor);
		for (int nIndex = 0; nIndex < m_pAlternateSpace->GetComponentsCount(); ++nIndex)
		{
			oAltColor.arrComp[nIndex] = DoubleToColor(arrDestColor[nIndex]);
		}
		m_pAlternateSpace->GetRGB(&oAltColor, pRGB);
	}

	void GrSeparationColorSpace::GetCMYK(GrColor *pColor, GrCMYK *pCMYK)
	{
		double arrDestColor[GrColorMaxComps];
		GrColor oAltColor;

		double dSrcColor = ColorToDouble(pColor->arrComp[0]);
		m_pFunction->Transform(&dSrcColor, arrDestColor);
		for (int nIndex = 0; nIndex < m_pAlternateSpace->GetComponentsCount(); ++nIndex)
		{
			oAltColor.arrComp[nIndex] = DoubleToColor(arrDestColor[nIndex]);
		}
		m_pAlternateSpace->GetCMYK(&oAltColor, pCMYK);
	}

	void GrSeparationColorSpace::GetDefaultColor(GrColor *pColor)
	{
		pColor->arrComp[0] = GrColorComp1;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrDeviceNColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	GrDeviceNColorSpace::GrDeviceNColorSpace(int nComponentsCount, GrColorSpace *pAlternate, Function *pFunction)
	{
		m_nComponentsCount = nComponentsCount;
		m_pAlternateSpace  = pAlternate;
		m_pFunction        = pFunction;
		m_bNonMarking      = false;
	}

	GrDeviceNColorSpace::~GrDeviceNColorSpace()
	{
		for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
		{
			if (m_arrseNames[nIndex])
				delete m_arrseNames[nIndex];
		}
		if (m_pAlternateSpace)
			delete m_pAlternateSpace;
		if (m_pFunction)
			delete m_pFunction;
	}

	GrColorSpace *GrDeviceNColorSpace::Copy()
	{
		GrDeviceNColorSpace *pColorSpace = new GrDeviceNColorSpace(m_nComponentsCount, m_pAlternateSpace->Copy(), m_pFunction->Copy());
		for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
		{
			pColorSpace->m_arrseNames[nIndex] = m_arrseNames[nIndex]->Copy();
		}
		pColorSpace->m_bNonMarking = m_bNonMarking;
		return pColorSpace;
	}

	GrColorSpace *GrDeviceNColorSpace::Parse(Array *pArray)
	{
		StringExt *arrseNames[GrColorMaxComps];

		// Запись о данном цветом пространстве должны быть одной из следующих двух:
		// [ /DeviceN names alternateSpace tintTransform ]
		// [ /DeviceN names alternateSpace tintTransform attributes ]

		if (pArray->GetCount() != 4 && pArray->GetCount() != 5)
		{
			// TO DO: Error "Bad DeviceN color space"
			return NULL;
		}

		Object oArrayItem;
		if (!pArray->Get(1, &oArrayItem)->IsArray())
		{
			// TO DO: Error "Bad DeviceN color space (names)"
			oArrayItem.Free();
			return NULL;
		}

		int nCompsCount = oArrayItem.ArrayGetLength();
		if (nCompsCount > GrColorMaxComps)
		{
			// TO DO: Error "DeviceN color space with too many components"
			nCompsCount = GrColorMaxComps;
		}
		for (int nIndex = 0; nIndex < nCompsCount; ++nIndex)
		{
			Object oName;
			if (!oArrayItem.ArrayGet(nIndex, &oName)->IsName())
			{
				// TO DO: Error "Bad DeviceN color space (names)"
				oName.Free();
				oArrayItem.Free();
				return NULL;
			}
			arrseNames[nIndex] = new StringExt(oName.GetName());
			oName.Free();
		}
		oArrayItem.Free();

		pArray->Get(2, &oArrayItem);
		GrColorSpace *pAlternate = NULL;
		if (!(pAlternate = GrColorSpace::Parse(&oArrayItem)))
		{
			// TO DO: Error "Bad DeviceN color space (alternate color space)"
			for (int nIndex = 0; nIndex < nCompsCount; ++nIndex)
			{
				if (arrseNames[nIndex])
					delete arrseNames[nIndex];
			}
			oArrayItem.Free();
			return NULL;
		}
		oArrayItem.Free();

		pArray->Get(3, &oArrayItem);
		Function *pFunction = NULL;
		if (!(pFunction = Function::Parse(&oArrayItem)))
		{
			if (pAlternate)
				delete pAlternate;
			for (int nIndex = 0; nIndex < nCompsCount; ++nIndex)
			{
				if (arrseNames[nIndex])
					delete arrseNames[nIndex];
			}
			oArrayItem.Free();
			return NULL;
		}
		oArrayItem.Free();

		GrDeviceNColorSpace *pColorSpace = new GrDeviceNColorSpace(nCompsCount, pAlternate, pFunction);
		pColorSpace->m_bNonMarking = true;

		for (int nIndex = 0; nIndex < nCompsCount; ++nIndex)
		{
			pColorSpace->m_arrseNames[nIndex] = arrseNames[nIndex];
			if (arrseNames[nIndex]->Compare("None"))
			{
				pColorSpace->m_bNonMarking = false;
			}
		}
		return pColorSpace;
	}

	void GrDeviceNColorSpace::GetGray(GrColor *pColor, GrGray *pGray)
	{
		double arrSrcColor[GrColorMaxComps], arrDstColor[GrColorMaxComps];
		GrColor oAltColor;

		for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
		{
			arrSrcColor[nIndex] = ColorToDouble(pColor->arrComp[nIndex]);
		}
		m_pFunction->Transform(arrSrcColor, arrDstColor);
		for (int nIndex = 0; nIndex < m_pAlternateSpace->GetComponentsCount(); ++nIndex)
		{
			oAltColor.arrComp[nIndex] = DoubleToColor(arrDstColor[nIndex]);
		}
		m_pAlternateSpace->GetGray(&oAltColor, pGray);
	}

	void GrDeviceNColorSpace::GetRGB(GrColor *pColor, GrRGB  *pRGB)
	{
		double arrSrcColor[GrColorMaxComps], arrDstColor[GrColorMaxComps];
		GrColor oAltColor;

		for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
		{
			arrSrcColor[nIndex] = ColorToDouble(pColor->arrComp[nIndex]);
		}
		m_pFunction->Transform(arrSrcColor, arrDstColor);
		for (int nIndex = 0; nIndex < m_pAlternateSpace->GetComponentsCount(); ++nIndex)
		{
			oAltColor.arrComp[nIndex] = DoubleToColor(arrDstColor[nIndex]);
		}
		m_pAlternateSpace->GetRGB(&oAltColor, pRGB);
	}

	void GrDeviceNColorSpace::GetCMYK(GrColor *pColor, GrCMYK *pCMYK)
	{
		double arrSrcColor[GrColorMaxComps], arrDstColor[GrColorMaxComps];
		GrColor oAltColor;

		for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
		{
			arrSrcColor[nIndex] = ColorToDouble(pColor->arrComp[nIndex]);
		}
		m_pFunction->Transform(arrSrcColor, arrDstColor);
		for (int nIndex = 0; nIndex < m_pAlternateSpace->GetComponentsCount(); ++nIndex)
		{
			oAltColor.arrComp[nIndex] = DoubleToColor(arrDstColor[nIndex]);
		}
		m_pAlternateSpace->GetCMYK(&oAltColor, pCMYK);
	}

	void GrDeviceNColorSpace::GetDefaultColor(GrColor *pColor)
	{
		for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
		{
			pColor->arrComp[nIndex] = GrColorComp1;
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrPatternColorSpace
	//-------------------------------------------------------------------------------------------------------------------------------

	GrPatternColorSpace::GrPatternColorSpace(GrColorSpace *pUnder)
	{
		m_pUnder = pUnder;
	}

	GrPatternColorSpace::~GrPatternColorSpace()
	{
		if (m_pUnder)
		{
			delete m_pUnder;
		}
	}

	GrColorSpace *GrPatternColorSpace::Copy()
	{
		return new GrPatternColorSpace(m_pUnder ? m_pUnder->Copy() : (GrColorSpace *)NULL);
	}

	GrColorSpace *GrPatternColorSpace::Parse(Array *pArray)
	{
		if (pArray->GetCount() != 1 && pArray->GetCount() != 2)
		{
			// TO DO: Error "Bad Pattern color space"
			return NULL;
		}
		GrColorSpace *pUnder = NULL;
		if (pArray->GetCount() == 2)
		{
			Object oTemp;
			pArray->Get(1, &oTemp);
			if (!(pUnder = GrColorSpace::Parse(&oTemp)))
			{
				// TO DO: Error "Bad Pattern color space (underlying color space)"
				oTemp.Free();
				return NULL;
			}
			oTemp.Free();
		}
		GrPatternColorSpace *pColorSpace = new GrPatternColorSpace(pUnder);
		return pColorSpace;
	}

	void GrPatternColorSpace::GetGray(GrColor *pColor, GrGray *pGray)
	{
		*pGray = 0;
	}

	void GrPatternColorSpace::GetRGB(GrColor *pColor, GrRGB  *pRGB)
	{
		pRGB->r = pRGB->g = pRGB->b = 0;
	}

	void GrPatternColorSpace::GetCMYK(GrColor *pColor, GrCMYK *pCMYK)
	{
		pCMYK->c = pCMYK->m = pCMYK->y = 0;
		pCMYK->k = 1;
	}

	void GrPatternColorSpace::GetDefaultColor(GrColor *pColor)
	{
		// не используется
	}

	//==============================================================================================================================

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrPattern
	//-------------------------------------------------------------------------------------------------------------------------------

	GrPattern::GrPattern(int nType)
	{
		m_nType = nType;
	}

	GrPattern::~GrPattern()
	{
	}

	GrPattern *GrPattern::Parse(Object *pObject)
	{
		Object oTemp;

		if (pObject->IsDict())
		{
			pObject->DictLookup("PatternType", &oTemp);
		}
		else if (pObject->IsStream())
		{
			pObject->StreamGetDict()->Search("PatternType", &oTemp);
		}
		else
		{
			return NULL;
		}
		GrPattern *pPattern = NULL;

		if (oTemp.IsInt() && oTemp.GetInt() == 1)
		{
			pPattern = GrTilingPattern::Parse(pObject);
		}
		else if (oTemp.IsInt() && oTemp.GetInt() == 2)
		{
			pPattern = GrShadingPattern::Parse(pObject);
		}
		oTemp.Free();
		return pPattern;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrTilingPattern
	//-------------------------------------------------------------------------------------------------------------------------------

	GrTilingPattern *GrTilingPattern::Parse(Object *pPatternObject)
	{
		int nPaintType = 0, nTilingType = 0;
		double dXStep = 0, dYStep = 0;
		Object oResources;

		if (!pPatternObject->IsStream())
		{
			return NULL;
		}
		Dict *pDict = pPatternObject->StreamGetDict();

		Object oDictItem;
		if (pDict->Search("PaintType", &oDictItem)->IsInt())
		{
			nPaintType = oDictItem.GetInt();
		}
		else
		{
			nPaintType = 1;
			// TO DO: Error "Invalid or missing PaintType in pattern"
		}
		oDictItem.Free();

		if (pDict->Search("TilingType", &oDictItem)->IsInt())
		{
			nTilingType = oDictItem.GetInt();
		}
		else
		{
			nTilingType = 1;
			// TO DO: Error "Invalid or missing TilingType in pattern"
		}
		oDictItem.Free();

		double arrBBox[4] ={ 0, 0, 1, 1 };

		if (pDict->Search("BBox", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 4)
		{
			for (int nIndex = 0; nIndex < 4; ++nIndex)
			{
				Object oTemp;
				if (oDictItem.ArrayGet(nIndex, &oTemp)->IsNum())
				{
					arrBBox[nIndex] = oTemp.GetNum();
				}
				oTemp.Free();
			}
		}
		else
		{
			// TO DO: Error "Invalid or missing BBox in pattern"
		}
		oDictItem.Free();

		if (pDict->Search("XStep", &oDictItem)->IsNum())
		{
			dXStep = oDictItem.GetNum();
		}
		else
		{
			dXStep = 1;
			// TO DO: Error "Invalid or missing XStep in pattern"
		}
		oDictItem.Free();

		if (pDict->Search("YStep", &oDictItem)->IsNum())
		{
			dYStep = oDictItem.GetNum();
		}
		else
		{
			dYStep = 1;
			// TO DO: Error "Invalid or missing YStep in pattern"
		}
		oDictItem.Free();

		if (!pDict->Search("Resources", &oResources)->IsDict())
		{
			oResources.Free();
			oResources.InitNull();
			// TO DO: Error "Invalid or missing Resources in pattern"
		}

		double arrMatrix[6] ={ 1, 0, 0, 1, 0, 0 };

		if (pDict->Search("Matrix", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 6)
		{
			for (int nIndex = 0; nIndex < 6; ++nIndex)
			{
				Object oTemp;
				if (oDictItem.ArrayGet(nIndex, &oTemp)->IsNum())
				{
					arrMatrix[nIndex] = oTemp.GetNum();
				}
				oTemp.Free();
			}
		}
		oDictItem.Free();

		GrTilingPattern *pPattern = new GrTilingPattern(nPaintType, nTilingType, arrBBox, dXStep, dYStep, &oResources, arrMatrix, pPatternObject);
		oResources.Free();
		return pPattern;
	}

	GrTilingPattern::GrTilingPattern(int nPaintType, int nTilingType, double *pBBox, double dXStep, double dYStep, Object *pResources, double *pMatrix, Object *pContentStream) :
		GrPattern(1)
	{
		m_nPaintType  = nPaintType;
		m_nTilingType = nTilingType;
		for (int nIndex = 0; nIndex < 4; ++nIndex)
		{
			m_arrBBox[nIndex] = pBBox[nIndex];
		}
		m_dXStep = dXStep;
		m_dYStep = dYStep;
		pResources->Copy(&m_oResources);

		for (int nIndex = 0; nIndex < 6; ++nIndex)
		{
			m_arrMatrix[nIndex] = pMatrix[nIndex];
		}
		pContentStream->Copy(&m_oContentStream);
	}

	GrTilingPattern::~GrTilingPattern()
	{
		m_oResources.Free();
		m_oContentStream.Free();
	}

	GrPattern *GrTilingPattern::Copy()
	{
		return new GrTilingPattern(m_nPaintType, m_nTilingType, m_arrBBox, m_dXStep, m_dYStep, &m_oResources, m_arrMatrix, &m_oContentStream);
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrShadingPattern
	//-------------------------------------------------------------------------------------------------------------------------------

	GrShadingPattern *GrShadingPattern::Parse(Object *pPatternObject)
	{
		if (!pPatternObject->IsDict())
		{
			return NULL;
		}
		Dict *pDict = pPatternObject->GetDict();

		Object oDictItem;
		pDict->Search("Shading", &oDictItem);
		GrShading *pShading = GrShading::Parse(&oDictItem);
		oDictItem.Free();

		if (!pShading)
		{
			return NULL;
		}

		double arrMatrix[6] ={ 1, 0, 0, 1, 0, 0 };
		if (pDict->Search("Matrix", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 6)
		{
			for (int nIndex = 0; nIndex < 6; ++nIndex)
			{
				Object oTemp;
				if (oDictItem.ArrayGet(nIndex, &oTemp)->IsNum())
				{
					arrMatrix[nIndex] = oTemp.GetNum();
				}
				oTemp.Free();
			}
		}
		oDictItem.Free();

		// TO DO: Надо сделать чтение поля ExtGState

		return new GrShadingPattern(pShading, arrMatrix);
	}

	GrShadingPattern::GrShadingPattern(GrShading *pShading, double *pMatrix) :
		GrPattern(2)
	{
		m_pShading = pShading;
		for (int nIndex = 0; nIndex < 6; ++nIndex)
		{
			m_arrMatrix[nIndex] = pMatrix[nIndex];
		}
	}

	GrShadingPattern::~GrShadingPattern()
	{
		delete m_pShading;
	}

	GrPattern *GrShadingPattern::Copy()
	{
		return new GrShadingPattern(m_pShading->Copy(), m_arrMatrix);
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrShading
	//-------------------------------------------------------------------------------------------------------------------------------

	GrShading::GrShading(int nType)
	{
		m_nType       = nType;
		m_pColorSpace = NULL;
	}

	GrShading::GrShading(GrShading *pShading)
	{
		m_nType = pShading->m_nType;
		m_pColorSpace = pShading->m_pColorSpace->Copy();

		for (int nIndex = 0; nIndex < GrColorMaxComps; ++nIndex)
		{
			m_oBackground.arrComp[nIndex] = pShading->m_oBackground.arrComp[nIndex];
		}
		m_bHasBackground = pShading->m_bHasBackground;
		m_dXMin = pShading->m_dXMin;
		m_dYMin = pShading->m_dYMin;
		m_dXMax = pShading->m_dXMax;
		m_dYMax = pShading->m_dYMax;
		m_bHasBBox = pShading->m_bHasBBox;
	}

	GrShading::~GrShading()
	{
		if (m_pColorSpace)
		{
			delete m_pColorSpace;
		}
	}

	GrShading *GrShading::Parse(Object *pObject)
	{
		GrShading *pShading = NULL;

		Dict *pDict = NULL;
		if (pObject->IsDict())
		{
			pDict = pObject->GetDict();
		}
		else if (pObject->IsStream())
		{
			pDict = pObject->StreamGetDict();
		}
		else
		{
			return NULL;
		}

		Object oDictItem;
		if (!pDict->Search("ShadingType", &oDictItem)->IsInt())
		{
			// TO DO: Error "Invalid ShadingType in shading dictionary"
			oDictItem.Free();
			return NULL;
		}
		int nType = oDictItem.GetInt();
		oDictItem.Free();

		switch (nType)
		{
			case 1:
			pShading = GrFunctionShading::Parse(pDict);
			break;
			case 2:
			pShading = GrAxialShading::Parse(pDict);
			break;
			case 3:
			pShading = GrRadialShading::Parse(pDict);
			break;
			case 4:
			if (pObject->IsStream())
			{
				pShading = GrGouraudTriangleShading::Parse(4, pDict, pObject->GetStream());
			}
			else
			{
				// TO DO: Error "Invalid Type 4 shading object"
				return NULL;
			}
			break;
			case 5:
			if (pObject->IsStream())
			{
				pShading = GrGouraudTriangleShading::Parse(5, pDict, pObject->GetStream());
			}
			else
			{
				// TO DO: Error "Invalid Type 5 shading object"
				return NULL;
			}
			break;
			case 6:
			if (pObject->IsStream())
			{
				pShading = GrPatchMeshShading::Parse(6, pDict, pObject->GetStream());
			}
			else
			{
				// TO DO: Error "Invalid Type 6 shading object"
				return NULL;
			}
			break;
			case 7:
			if (pObject->IsStream())
			{
				pShading = GrPatchMeshShading::Parse(7, pDict, pObject->GetStream());
			}
			else
			{
				// TO DO: Error "Invalid Type 7 shading object"
				return NULL;
			}
			break;
			default:
			// TO DO: Error "Unimplemented shading type"
			return NULL;
		}

		// TO DO: Добавить чтение поля AntiAlias

		return pShading;
	}

	bool GrShading::Initialize(Dict *pDict)
	{
		Object oDictItem;
		pDict->Search("ColorSpace", &oDictItem);
		if (!(m_pColorSpace = GrColorSpace::Parse(&oDictItem)))
		{
			// TO DO: Error "Bad color space in shading dictionary"
			oDictItem.Free();
			return false;
		}
		oDictItem.Free();

		for (int nIndex = 0; nIndex < GrColorMaxComps; ++nIndex)
		{
			m_oBackground.arrComp[nIndex] = 0;
		}
		m_bHasBackground = false;

		if (pDict->Search("Background", &oDictItem)->IsArray())
		{
			if (oDictItem.ArrayGetLength() == m_pColorSpace->GetComponentsCount())
			{
				m_bHasBackground = true;
				for (int nIndex = 0; nIndex < m_pColorSpace->GetComponentsCount(); ++nIndex)
				{
					Object oTemp;
					m_oBackground.arrComp[nIndex] = DoubleToColor(oDictItem.ArrayGet(nIndex, &oTemp)->GetNum());
					oTemp.Free();
				}
			}
			else
			{
				// TO DO: Error "Bad Background in shading dictionary"
			}
		}
		oDictItem.Free();

		m_dXMin = m_dYMin = m_dXMax = m_dYMax = 0;
		m_bHasBBox = false;
		if (pDict->Search("BBox", &oDictItem)->IsArray())
		{
			if (oDictItem.ArrayGetLength() == 4)
			{
				Object oTemp;
				m_bHasBBox = true;
				m_dXMin = oDictItem.ArrayGet(0, &oTemp)->GetNum();
				oTemp.Free();

				m_dYMin = oDictItem.ArrayGet(1, &oTemp)->GetNum();
				oTemp.Free();

				m_dXMax = oDictItem.ArrayGet(2, &oTemp)->GetNum();
				oTemp.Free();

				m_dYMax = oDictItem.ArrayGet(3, &oTemp)->GetNum();
				oTemp.Free();
			}
			else
			{
				// TO DO: Error "Bad BBox in shading dictionary"
			}
		}
		oDictItem.Free();

		// TO DO: Добавить чтение поля AntiAlias

		return true;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrFunctionShading
	//-------------------------------------------------------------------------------------------------------------------------------

	GrFunctionShading::GrFunctionShading(double dMinX, double dMinY, double dMaxX, double dMaxY, double *pMatrix, Function **ppFunctions, int nFuncsCount) :
		GrShading(1)
	{
		m_dDomainMinX = dMinX;
		m_dDomainMinY = dMinY;
		m_dDomainMaxX = dMaxX;
		m_dDomainMaxY = dMaxY;

		for (int nIndex = 0; nIndex < 6; ++nIndex)
		{
			m_arrMatrix[nIndex] = pMatrix[nIndex];
		}
		m_nFunctionsCount = nFuncsCount;

		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			m_arrFunctions[nIndex] = ppFunctions[nIndex];
		}
	}

	GrFunctionShading::GrFunctionShading(GrFunctionShading *pShading) :
		GrShading(pShading)
	{
		m_dDomainMinX = pShading->m_dDomainMinX;
		m_dDomainMinY = pShading->m_dDomainMinY;
		m_dDomainMaxX = pShading->m_dDomainMaxX;
		m_dDomainMaxY = pShading->m_dDomainMaxY;

		for (int nIndex = 0; nIndex < 6; ++nIndex)
		{
			m_arrMatrix[nIndex] = pShading->m_arrMatrix[nIndex];
		}
		m_nFunctionsCount = pShading->m_nFunctionsCount;
		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			m_arrFunctions[nIndex] = pShading->m_arrFunctions[nIndex]->Copy();
		}
	}

	GrFunctionShading::~GrFunctionShading()
	{
		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			if (m_arrFunctions[nIndex])
				delete m_arrFunctions[nIndex];
		}
	}

	GrFunctionShading *GrFunctionShading::Parse(Dict *pDict)
	{
		Function *arrFunctions[GrColorMaxComps];

		double dMinX = 0, dMinY = 0;
		double dMaxX = 1, dMaxY = 1;
		Object oDictItem;
		if (pDict->Search("Domain", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 4)
		{
			Object oTemp;
			dMinX = oDictItem.ArrayGet(0, &oTemp)->GetNum();
			oTemp.Free();
			// TO DO: Проверить здесь чтение(было не по спецификации)
			dMaxX = oDictItem.ArrayGet(1, &oTemp)->GetNum();
			oTemp.Free();
			dMinY = oDictItem.ArrayGet(2, &oTemp)->GetNum();
			oTemp.Free();
			dMaxY = oDictItem.ArrayGet(3, &oTemp)->GetNum();
			oTemp.Free();
		}
		oDictItem.Free();

		double arrMatrix[6] ={ 1, 0, 0, 1, 0, 0 };
		if (pDict->Search("Matrix", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 6)
		{
			Object oTemp;
			arrMatrix[0] = oDictItem.ArrayGet(0, &oTemp)->GetNum();
			oTemp.Free();
			arrMatrix[1] = oDictItem.ArrayGet(1, &oTemp)->GetNum();
			oTemp.Free();
			arrMatrix[2] = oDictItem.ArrayGet(2, &oTemp)->GetNum();
			oTemp.Free();
			arrMatrix[3] = oDictItem.ArrayGet(3, &oTemp)->GetNum();
			oTemp.Free();
			arrMatrix[4] = oDictItem.ArrayGet(4, &oTemp)->GetNum();
			oTemp.Free();
			arrMatrix[5] = oDictItem.ArrayGet(5, &oTemp)->GetNum();
			oTemp.Free();
		}
		oDictItem.Free();

		pDict->Search("Function", &oDictItem);
		int nFuncsCount = 0;
		if (oDictItem.IsArray())
		{
			nFuncsCount = oDictItem.ArrayGetLength();
			if (nFuncsCount > GrColorMaxComps)
			{
				// TO DO: Error "Invalid Function array in shading dictionary"
				oDictItem.Free();
				return NULL;
			}
			for (int nIndex = 0; nIndex < nFuncsCount; ++nIndex)
			{
				Object oTemp;
				oDictItem.ArrayGet(nIndex, &oTemp);
				if (!(arrFunctions[nIndex] = Function::Parse(&oTemp)))
				{
					oTemp.Free();
					oDictItem.Free();
					return NULL;
				}
				oTemp.Free();
			}
		}
		else
		{
			nFuncsCount = 1;
			if (!(arrFunctions[0] = Function::Parse(&oDictItem)))
			{
				oDictItem.Free();
				return NULL;
			}
		}
		oDictItem.Free();

		GrFunctionShading *pShading = new GrFunctionShading(dMinX, dMinY, dMaxX, dMaxY, arrMatrix, arrFunctions, nFuncsCount);
		if (!pShading->Initialize(pDict))
		{
			delete pShading;
			return NULL;
		}
		return pShading;
	}

	GrShading *GrFunctionShading::Copy()
	{
		return new GrFunctionShading(this);
	}

	void GrFunctionShading::GetColor(double dX, double dY, GrColor *pColor)
	{
		double arrInput[2], arrOutput[GrColorMaxComps];

		for (int nIndex = 0; nIndex < GrColorMaxComps; ++nIndex)
		{
			arrOutput[nIndex] = 0;
		}
		arrInput[0] = dX;
		arrInput[1] = dY;
		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			m_arrFunctions[nIndex]->Transform(arrInput, &arrOutput[nIndex]);
		}
		for (int nIndex = 0; nIndex < GrColorMaxComps; ++nIndex)
		{
			pColor->arrComp[nIndex] = DoubleToColor(arrOutput[nIndex]);
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrAxialShading
	//-------------------------------------------------------------------------------------------------------------------------------

	GrAxialShading::GrAxialShading(double dX0, double dY0, double dX1, double dY1, double dT0, double dT1, Function **ppFunctions, int nFuncsCount, bool bExtendStart, bool bExtendEnd) :
		GrShading(2)
	{
		m_dAxisX0 = dX0;
		m_dAxisY0 = dY0;
		m_dAxisX1 = dX1;
		m_dAxisY1 = dY1;
		m_dT0 = dT0;
		m_dT1 = dT1;
		m_nFunctionsCount = nFuncsCount;
		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			m_arrFunctions[nIndex] = ppFunctions[nIndex];
		}
		m_bExtendStart = bExtendStart;
		m_bExtendEnd   = bExtendEnd;
	}

	GrAxialShading::GrAxialShading(GrAxialShading *pShading) :
		GrShading(pShading)
	{
		m_dAxisX0 = pShading->m_dAxisX0;
		m_dAxisY0 = pShading->m_dAxisY0;
		m_dAxisX1 = pShading->m_dAxisX1;
		m_dAxisY1 = pShading->m_dAxisY1;
		m_dT0 = pShading->m_dT0;
		m_dT1 = pShading->m_dT1;
		m_nFunctionsCount = pShading->m_nFunctionsCount;

		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			m_arrFunctions[nIndex] = pShading->m_arrFunctions[nIndex]->Copy();
		}
		m_bExtendStart = pShading->m_bExtendStart;
		m_bExtendEnd   = pShading->m_bExtendEnd;
	}

	GrAxialShading::~GrAxialShading()
	{
		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			delete m_arrFunctions[nIndex];
		}
	}

	GrAxialShading *GrAxialShading::Parse(Dict *pDict)
	{
		Function *arrFunctions[GrColorMaxComps];

		double dX0 = 0, dY0 = 0, dX1 = 0, dY1 = 0;
		Object oDictItem;
		if (pDict->Search("Coords", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 4)
		{
			Object oTemp;
			dX0 = oDictItem.ArrayGet(0, &oTemp)->GetNum();
			oTemp.Free();
			dY0 = oDictItem.ArrayGet(1, &oTemp)->GetNum();
			oTemp.Free();
			dX1 = oDictItem.ArrayGet(2, &oTemp)->GetNum();
			oTemp.Free();
			dY1 = oDictItem.ArrayGet(3, &oTemp)->GetNum();
			oTemp.Free();
		}
		else
		{
			// TO DO: Error "Missing or invalid Coords in shading dictionary"
			oDictItem.Free();
			return NULL;
		}
		oDictItem.Free();

		double dT0 = 0, dT1 = 1;
		if (pDict->Search("Domain", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 2)
		{
			Object oTemp;
			dT0 = oDictItem.ArrayGet(0, &oTemp)->GetNum();
			oTemp.Free();
			dT1 = oDictItem.ArrayGet(1, &oTemp)->GetNum();
			oTemp.Free();
		}
		oDictItem.Free();

		pDict->Search("Function", &oDictItem);
		int nFuncsCount = 0;
		if (oDictItem.IsArray())
		{
			nFuncsCount = oDictItem.ArrayGetLength();
			if (nFuncsCount > GrColorMaxComps)
			{
				// TO DO: Error "Invalid Function array in shading dictionary"
				oDictItem.Free();
				return NULL;
			}
			for (int nIndex = 0; nIndex < nFuncsCount; ++nIndex)
			{
				Object oTemp;
				oDictItem.ArrayGet(nIndex, &oTemp);
				if (!(arrFunctions[nIndex] = Function::Parse(&oTemp)))
				{
					oDictItem.Free();
					oTemp.Free();
					return NULL;
				}
				oTemp.Free();
			}
		}
		else
		{
			nFuncsCount = 1;
			if (!(arrFunctions[0] = Function::Parse(&oDictItem)))
			{
				oDictItem.Free();
				return NULL;
			}
		}
		oDictItem.Free();

		bool bExtendStart = false, bExtendEnd = false;
		if (pDict->Search("Extend", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 2)
		{
			Object oTemp;
			bExtendStart = oDictItem.ArrayGet(0, &oTemp)->GetBool();
			oTemp.Free();
			bExtendEnd   = oDictItem.ArrayGet(1, &oTemp)->GetBool();
			oTemp.Free();
		}
		oDictItem.Free();

		GrAxialShading *pShading = new GrAxialShading(dX0, dY0, dX1, dY1, dT0, dT1, arrFunctions, nFuncsCount, bExtendStart, bExtendEnd);
		if (!pShading->Initialize(pDict))
		{
			delete pShading;
			return NULL;
		}
		return pShading;
	}

	GrShading *GrAxialShading::Copy()
	{
		return new GrAxialShading(this);
	}

	void GrAxialShading::GetColor(double dT, GrColor *pColor)
	{
		double arrOutput[GrColorMaxComps];

		for (int nIndex = 0; nIndex < GrColorMaxComps; ++nIndex)
		{
			arrOutput[nIndex] = 0;
		}
		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			m_arrFunctions[nIndex]->Transform(&dT, &arrOutput[nIndex]);
		}
		for (int nIndex = 0; nIndex < GrColorMaxComps; ++nIndex)
		{
			pColor->arrComp[nIndex] = DoubleToColor(arrOutput[nIndex]);
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrRadialShading
	//-------------------------------------------------------------------------------------------------------------------------------

	GrRadialShading::GrRadialShading(double dFirstX, double dFirstY, double dFirstRad, double dSecondX, double dSecondY, double dSecondRad, double dT0, double dT1, Function **ppFunctions, int nFuncsCount, bool bExtendFirst, bool bExtendSecond) :
		GrShading(3)
	{
		m_dFirstX = dFirstX;
		m_dFirstY = dFirstY;
		m_dFirstRad = dFirstRad;
		m_dSecondX = dSecondX;
		m_dSecondY = dSecondY;
		m_dSecondRad = dSecondRad;
		m_dT0 = dT0;
		m_dT1 = dT1;
		m_nFunctionsCount = nFuncsCount;
		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			m_arrFunctions[nIndex] = ppFunctions[nIndex];
		}
		m_bExtendFirst = bExtendFirst;
		m_bExtendSecond = bExtendSecond;
	}

	GrRadialShading::GrRadialShading(GrRadialShading *pShading) :
		GrShading(pShading)
	{
		m_dFirstX    = pShading->m_dFirstX;
		m_dFirstY    = pShading->m_dFirstY;
		m_dFirstRad  = pShading->m_dFirstRad;
		m_dSecondX   = pShading->m_dSecondX;
		m_dSecondY   = pShading->m_dSecondY;
		m_dSecondRad = pShading->m_dSecondRad;
		m_dT0 = pShading->m_dT0;
		m_dT1 = pShading->m_dT1;
		m_nFunctionsCount = pShading->m_nFunctionsCount;
		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			m_arrFunctions[nIndex] = pShading->m_arrFunctions[nIndex]->Copy();
		}
		m_bExtendFirst  = pShading->m_bExtendFirst;
		m_bExtendSecond = pShading->m_bExtendSecond;
	}

	GrRadialShading::~GrRadialShading()
	{
		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			if (m_arrFunctions[nIndex])
				delete m_arrFunctions[nIndex];
		}
	}

	GrRadialShading *GrRadialShading::Parse(Dict *pDict)
	{
		Function *arrFunctions[GrColorMaxComps];

		Object oDictItem;
		double dFirstX = 0, dFirstY = 0, dFirstRad = 0, dSecondX = 0, dSecondY = 0, dSecondRad = 0;
		if (pDict->Search("Coords", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 6)
		{
			Object oTemp;
			dFirstX    = oDictItem.ArrayGet(0, &oTemp)->GetNum();
			oTemp.Free();
			dFirstY    = oDictItem.ArrayGet(1, &oTemp)->GetNum();
			oTemp.Free();
			dFirstRad  = oDictItem.ArrayGet(2, &oTemp)->GetNum();
			oTemp.Free();
			dSecondX   = oDictItem.ArrayGet(3, &oTemp)->GetNum();
			oTemp.Free();
			dSecondY   = oDictItem.ArrayGet(4, &oTemp)->GetNum();
			oTemp.Free();
			dSecondRad = oDictItem.ArrayGet(5, &oTemp)->GetNum();
			oTemp.Free();
		}
		else
		{
			// TO DO: Error "Missing or invalid Coords in shading dictionary"
			oDictItem.Free();
			return NULL;
		}
		oDictItem.Free();

		double dT0 = 0, dT1 = 1;
		if (pDict->Search("Domain", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 2)
		{
			Object oTemp;
			dT0 = oDictItem.ArrayGet(0, &oTemp)->GetNum();
			oTemp.Free();
			dT1 = oDictItem.ArrayGet(1, &oTemp)->GetNum();
			oTemp.Free();
		}
		oDictItem.Free();

		pDict->Search("Function", &oDictItem);
		int nFuncsCount = 0;
		if (oDictItem.IsArray())
		{
			nFuncsCount = oDictItem.ArrayGetLength();
			if (nFuncsCount > GrColorMaxComps)
			{
				// TO DO: Error "Invalid Function array in shading dictionary"
				oDictItem.Free();
				return NULL;
			}
			for (int nIndex = 0; nIndex < nFuncsCount; ++nIndex)
			{
				Object oTemp;
				oDictItem.ArrayGet(nIndex, &oTemp);
				if (!(arrFunctions[nIndex] = Function::Parse(&oTemp)))
				{
					oDictItem.Free();
					oTemp.Free();
					return NULL;
				}
				oTemp.Free();
			}
		}
		else
		{
			nFuncsCount = 1;
			if (!(arrFunctions[0] = Function::Parse(&oDictItem)))
			{
				oDictItem.Free();
				return NULL;
			}
		}
		oDictItem.Free();

		bool bExtendFirst = false, bExtendSecond = false;
		if (pDict->Search("Extend", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 2)
		{
			Object oTemp;
			bExtendFirst  = oDictItem.ArrayGet(0, &oTemp)->GetBool();
			oTemp.Free();
			bExtendSecond = oDictItem.ArrayGet(1, &oTemp)->GetBool();
			oTemp.Free();
		}
		oDictItem.Free();

		GrRadialShading *pShading = new GrRadialShading(dFirstX, dFirstY, dFirstRad, dSecondX, dSecondY, dSecondRad, dT0, dT1, arrFunctions, nFuncsCount, bExtendFirst, bExtendSecond);
		if (!pShading->Initialize(pDict))
		{
			delete pShading;
			return NULL;
		}
		return pShading;
	}

	GrShading *GrRadialShading::Copy()
	{
		return new GrRadialShading(this);
	}

	void GrRadialShading::GetColor(double dT, GrColor *pColor)
	{
		double arrOutput[GrColorMaxComps];

		for (int nIndex = 0; nIndex < GrColorMaxComps; ++nIndex)
		{
			arrOutput[nIndex] = 0;
		}
		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			m_arrFunctions[nIndex]->Transform(&dT, &arrOutput[nIndex]);
		}
		for (int nIndex = 0; nIndex < GrColorMaxComps; ++nIndex)
		{
			pColor->arrComp[nIndex] = DoubleToColor(arrOutput[nIndex]);
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrShadingBitBuffer
	//-------------------------------------------------------------------------------------------------------------------------------

	class GrShadingBitBuffer
	{
	public:

		GrShadingBitBuffer(Stream *pStream)
		{
			m_pStream = pStream;
			m_pStream->Reset();
			m_nBitBuffer = 0;
			m_nBitsCount = 0;
		}
		~GrShadingBitBuffer()
		{
			m_pStream->Close();
		}
		bool GetBits(int nCount, unsigned int *pValue)
		{
			int nResult = 0;

			if (m_nBitsCount >= nCount)
			{
				nResult = (m_nBitBuffer >> (m_nBitsCount - nCount)) & ((1 << nCount) - 1);
				m_nBitsCount -= nCount;
			}
			else
			{
				nResult = 0;
				if (m_nBitsCount > 0)
				{
					nResult = m_nBitBuffer & ((1 << m_nBitsCount) - 1);
					nCount -= m_nBitsCount;
					m_nBitsCount = 0;
				}
				while (nCount > 0)
				{
					if ((m_nBitBuffer = m_pStream->GetChar()) == EOF)
					{
						m_nBitsCount = 0;
						return false;
					}
					if (nCount >= 8)
					{
						nResult = (nResult << 8) | m_nBitBuffer;
						nCount -= 8;
					}
					else
					{
						nResult = (nResult << nCount) | (m_nBitBuffer >> (8 - nCount));
						m_nBitsCount = 8 - nCount;
						nCount = 0;
					}
				}
			}
			*pValue = nResult;
			return true;
		}

		void FlushBits()
		{
			m_nBitBuffer = 0;
			m_nBitsCount = 0;
		}

	private:

		Stream *m_pStream;
		int     m_nBitBuffer;
		int     m_nBitsCount;
	};
	//-------------------------------------------------------------------------------------------------------------------------------
	// GrGouraudTriangleShading
	//-------------------------------------------------------------------------------------------------------------------------------

	GrGouraudTriangleShading::GrGouraudTriangleShading(int nType, GrGouraudVertex *pVertexes, int nVertexesCount, int(*pTriangles)[3], int nTrianglesCount, Function **ppFunctions, int nFunctionsCount) :
		GrShading(nType)
	{
		m_arrVertexs    = pVertexes;
		m_nVertexsCount = nVertexesCount;
		m_arrTriangles    = pTriangles;
		m_nTrianglesCount = nTrianglesCount;
		m_nFunctionsCount = nFunctionsCount;

		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			m_ppFunctions[nIndex] = ppFunctions[nIndex];
		}
	}

	GrGouraudTriangleShading::GrGouraudTriangleShading(GrGouraudTriangleShading *pShading) :
		GrShading(pShading)
	{
		m_nVertexsCount = pShading->m_nVertexsCount;
		m_arrVertexs = (GrGouraudVertex *)MemUtilsMallocArray(m_nVertexsCount, sizeof(GrGouraudVertex));
		memcpy(m_arrVertexs, pShading->m_arrVertexs, m_nVertexsCount * sizeof(GrGouraudVertex));

		m_nTrianglesCount = pShading->m_nTrianglesCount;
		m_arrTriangles = (int(*)[3])MemUtilsMallocArray(m_nTrianglesCount * 3, sizeof(int));
		memcpy(m_arrTriangles, pShading->m_arrTriangles, m_nTrianglesCount * 3 * sizeof(int));

		m_nFunctionsCount = pShading->m_nFunctionsCount;
		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			m_ppFunctions[nIndex] = pShading->m_ppFunctions[nIndex]->Copy();
		}
	}

	GrGouraudTriangleShading::~GrGouraudTriangleShading()
	{
		MemUtilsFree(m_arrVertexs);
		MemUtilsFree(m_arrTriangles);

		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			if (m_ppFunctions[nIndex])
				delete m_ppFunctions[nIndex];
		}
	}

	GrGouraudTriangleShading *GrGouraudTriangleShading::Parse(int nType, Dict *pDict, Stream *pStream)
	{
		int nBitsPerCoordinate = 0, nBitsPerComponent = 0;

		int nIndex = 0;

		Object oDictItem;
		if (pDict->Search("BitsPerCoordinate", &oDictItem)->IsInt())
		{
			nBitsPerCoordinate = oDictItem.GetInt();
		}
		else
		{
			// TO DO: Error "Missing or invalid BitsPerCoordinate in shading dictionary"
			oDictItem.Free();
			return NULL;
		}
		oDictItem.Free();

		if (pDict->Search("BitsPerComponent", &oDictItem)->IsInt())
		{
			nBitsPerComponent = oDictItem.GetInt();
		}
		else
		{
			// TO DO: Error "Missing or invalid BitsPerComponent in shading dictionary"
			oDictItem.Free();
			return NULL;
		}
		oDictItem.Free();

		int nBitsPerFlag = 0, nVerticesPerRow = 0;
		if (nType == 4)
		{
			if (pDict->Search("BitsPerFlag", &oDictItem)->IsInt())
			{
				nBitsPerFlag = oDictItem.GetInt();
			}
			else
			{
				// TO DO: Error "Missing or invalid BitsPerFlag in shading dictionary"
				oDictItem.Free();
				return NULL;
			}
			oDictItem.Free();
		}
		else
		{
			if (pDict->Search("VerticesPerRow", &oDictItem)->IsInt())
			{
				nVerticesPerRow = oDictItem.GetInt();
			}
			else
			{
				// TO DO: Error "Missing or invalid VerticesPerRow in shading dictionary"
				oDictItem.Free();
				return NULL;
			}
			oDictItem.Free();
		}

		// [ Xmin Xmax Ymin Ymax C1,min C1,max ... Cn,min Cn,max ], поэтому как минимум массив должен быть из 6 элементов
		double dXMin, dXMax, dYMin, dYMax;
		double arrCMin[GrColorMaxComps], arrCMax[GrColorMaxComps];
		double dXMul, dYMul;
		double arrCMul[GrColorMaxComps];
		int nComponentsCount = 0;
		if (pDict->Search("Decode", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() >= 6)
		{
			Object oTemp;
			dXMin = oDictItem.ArrayGet(0, &oTemp)->GetNum();
			oTemp.Free();
			dXMax = oDictItem.ArrayGet(1, &oTemp)->GetNum();
			oTemp.Free();
			dXMul = (dXMax - dXMin) / (pow(2.0, nBitsPerCoordinate) - 1);
			dYMin = oDictItem.ArrayGet(2, &oTemp)->GetNum();
			oTemp.Free();
			dYMax = oDictItem.ArrayGet(3, &oTemp)->GetNum();
			oTemp.Free();
			dYMul = (dYMax - dYMin) / (pow(2.0, nBitsPerCoordinate) - 1);

			for (nIndex = 0; 5 + 2 * nIndex < oDictItem.ArrayGetLength() && nIndex < GrColorMaxComps; ++nIndex)
			{
				arrCMin[nIndex] = oDictItem.ArrayGet(4 + 2 * nIndex, &oTemp)->GetNum();
				oTemp.Free();
				arrCMax[nIndex] = oDictItem.ArrayGet(5 + 2 * nIndex, &oTemp)->GetNum();
				oTemp.Free();
				arrCMul[nIndex] = (arrCMax[nIndex] - arrCMin[nIndex]) / (double)((1 << nBitsPerComponent) - 1);
			}
			nComponentsCount = nIndex;
		}
		else
		{
			// TO DO: Error "Missing or invalid Decode array in shading dictionary"
			oDictItem.Free();
			return NULL;
		}
		oDictItem.Free();

		int nFunctionsCount = 0;
		Function *ppFunctions[GrColorMaxComps];
		if (!pDict->Search("Function", &oDictItem)->IsNull())
		{
			if (oDictItem.IsArray())
			{
				nFunctionsCount = oDictItem.ArrayGetLength();
				if (nFunctionsCount > GrColorMaxComps)
				{
					// TO DO: Error "Invalid Function array in shading dictionary"
					oDictItem.Free();
					return NULL;
				}
				for (int nIndex = 0; nIndex < nFunctionsCount; ++nIndex)
				{
					Object oTemp;
					oDictItem.ArrayGet(nIndex, &oTemp);
					if (!(ppFunctions[nIndex] = Function::Parse(&oTemp)))
					{
						oDictItem.Free();
						oTemp.Free();
						return NULL;
					}
					oTemp.Free();
				}
			}
			else
			{
				nFunctionsCount = 1;
				if (!(ppFunctions[0] = Function::Parse(&oDictItem)))
				{
					oDictItem.Free();
					return NULL;
				}
			}
		}
		else
		{
			nFunctionsCount = 0;
		}
		oDictItem.Free();

		// Дальше читаем данные из потока(набор вершин)
		int nVerticesCount = 0, nTrianglesCount = 0;
		GrGouraudVertex *pVertices = NULL;
		int(*pTriangles)[3] = NULL;
		int nVertSize = 0, nTriSize = 0;
		int nState = 0;
		unsigned int unFlag = 0, unX = 0, unY = 0;
		unsigned int arrunC[GrColorMaxComps];
		GrShadingBitBuffer *pBitBuffer = new GrShadingBitBuffer(pStream);
		if (NULL == pBitBuffer)
			return NULL;

		while (1)
		{
			if (nType == 4) // В типе 5 нет флага вначале
			{
				if (!pBitBuffer->GetBits(nBitsPerFlag, &unFlag))
				{
					break;
				}
			}
			if (!pBitBuffer->GetBits(nBitsPerCoordinate, &unX) || !pBitBuffer->GetBits(nBitsPerCoordinate, &unY))
			{
				break;
			}
			for (nIndex = 0; nIndex < nComponentsCount; ++nIndex)
			{
				if (!pBitBuffer->GetBits(nBitsPerComponent, &arrunC[nIndex]))
				{
					break;
				}
			}
			if (nIndex < nComponentsCount)
			{
				break;
			}
			if (nVerticesCount == nVertSize)
			{
				nVertSize = (nVertSize == 0) ? 16 : 2 * nVertSize;
				pVertices = (GrGouraudVertex *)MemUtilsReallocArray(pVertices, nVertSize, sizeof(GrGouraudVertex));
			}
			pVertices[nVerticesCount].dX = dXMin + dXMul * (double)unX;
			pVertices[nVerticesCount].dY = dYMin + dYMul * (double)unY;

			for (int nIndex = 0; nIndex < nComponentsCount; ++nIndex)
			{
				pVertices[nVerticesCount].oColor.arrComp[nIndex] = DoubleToColor(arrCMin[nIndex] + arrCMul[nIndex] * (double)arrunC[nIndex]);
			}
			++nVerticesCount;

			pBitBuffer->FlushBits();
			if (nType == 4)
			{
				if (nState == 0 || nState == 1)
				{
					++nState;
				}
				else if (nState == 2 || unFlag > 0)
				{
					if (nTrianglesCount == nTriSize)
					{
						nTriSize = (nTriSize == 0) ? 16 : 2 * nTriSize;
						pTriangles = (int(*)[3]) MemUtilsReallocArray(pTriangles, nTriSize * 3, sizeof(int));
					}
					if (nState == 2)
					{
						pTriangles[nTrianglesCount][0] = nVerticesCount - 3;
						pTriangles[nTrianglesCount][1] = nVerticesCount - 2;
						pTriangles[nTrianglesCount][2] = nVerticesCount - 1;
						++nState;
					}
					else if (unFlag == 1)
					{
						pTriangles[nTrianglesCount][0] = pTriangles[nTrianglesCount - 1][1];
						pTriangles[nTrianglesCount][1] = pTriangles[nTrianglesCount - 1][2];
						pTriangles[nTrianglesCount][2] = nVerticesCount - 1;
					}
					else // unFlag == 2
					{
						pTriangles[nTrianglesCount][0] = pTriangles[nTrianglesCount - 1][0];
						pTriangles[nTrianglesCount][1] = pTriangles[nTrianglesCount - 1][2];
						pTriangles[nTrianglesCount][2] = nVerticesCount - 1;
					}
					++nTrianglesCount;
				}
				else  // nState == 3 && unFlag == 0
				{
					nState = 1;
				}
			}
		}
		delete pBitBuffer;
		if (nType == 5)
		{
			int nRowsCount = nVerticesCount / nVerticesPerRow;
			nTrianglesCount = (nRowsCount - 1) * 2 * (nVerticesPerRow - 1);
			pTriangles = (int(*)[3])MemUtilsMallocArray(nTrianglesCount * 3, sizeof(int));
			int nTriangleIndex = 0;
			for (int nRowIndex = 0; nRowIndex < nRowsCount - 1; ++nRowIndex)
			{
				for (int nVertIndex = 0; nVertIndex < nVerticesPerRow - 1; ++nVertIndex)
				{
					pTriangles[nTriangleIndex][0] =  nRowIndex      * nVerticesPerRow + nVertIndex;
					pTriangles[nTriangleIndex][1] =  nRowIndex      * nVerticesPerRow + nVertIndex + 1;
					pTriangles[nTriangleIndex][2] = (nRowIndex + 1) * nVerticesPerRow + nVertIndex;
					++nTriangleIndex;
					pTriangles[nTriangleIndex][0] =  nRowIndex      * nVerticesPerRow + nVertIndex + 1;
					pTriangles[nTriangleIndex][1] = (nRowIndex + 1) * nVerticesPerRow + nVertIndex;
					pTriangles[nTriangleIndex][2] = (nRowIndex + 1) * nVerticesPerRow + nVertIndex + 1;
					++nTriangleIndex;
				}
			}
		}

		GrGouraudTriangleShading *pShading = new GrGouraudTriangleShading(nType, pVertices, nVerticesCount, pTriangles, nTrianglesCount, ppFunctions, nFunctionsCount);
		if (!pShading)
			return NULL;
		if (!pShading->Initialize(pDict))
		{
			delete pShading;
			return NULL;
		}
		return pShading;
	}

	GrShading *GrGouraudTriangleShading::Copy()
	{
		return new GrGouraudTriangleShading(this);
	}

	void GrGouraudTriangleShading::GetTriangle(int nIndex, double *pdX0, double *pdY0, GrColor *pColor0, double *pdX1, double *pdY1, GrColor *pColor1, double *pdX2, double *pdY2, GrColor *pColor2)
	{
		double dIn = 0;
		double arrOut[GrColorMaxComps];
		int nVertexIndex;

		nVertexIndex = m_arrTriangles[nIndex][0];
		*pdX0 = m_arrVertexs[nVertexIndex].dX;
		*pdY0 = m_arrVertexs[nVertexIndex].dY;
		if (m_nFunctionsCount > 0)
		{
			dIn = ColorToDouble(m_arrVertexs[nVertexIndex].oColor.arrComp[0]);
			for (int nJ = 0; nJ < m_nFunctionsCount; ++nJ)
			{
				m_ppFunctions[nJ]->Transform(&dIn, &arrOut[nJ]);
			}
			for (int nJ = 0; nJ < GrColorMaxComps; ++nJ)
			{
				pColor0->arrComp[nJ] = DoubleToColor(arrOut[nJ]);
			}
		}
		else
		{
			*pColor0 = m_arrVertexs[nVertexIndex].oColor;
		}

		nVertexIndex = m_arrTriangles[nIndex][1];
		*pdX1 = m_arrVertexs[nVertexIndex].dX;
		*pdY1 = m_arrVertexs[nVertexIndex].dY;
		if (m_nFunctionsCount > 0)
		{
			dIn = ColorToDouble(m_arrVertexs[nVertexIndex].oColor.arrComp[0]);
			for (int nJ = 0; nJ < m_nFunctionsCount; ++nJ)
			{
				m_ppFunctions[nJ]->Transform(&dIn, &arrOut[nJ]);
			}
			for (int nJ = 0; nJ < GrColorMaxComps; ++nJ)
			{
				pColor1->arrComp[nJ] = DoubleToColor(arrOut[nJ]);
			}
		}
		else
		{
			*pColor1 = m_arrVertexs[nVertexIndex].oColor;
		}

		nVertexIndex = m_arrTriangles[nIndex][2];
		*pdX2 = m_arrVertexs[nVertexIndex].dX;
		*pdY2 = m_arrVertexs[nVertexIndex].dY;
		if (m_nFunctionsCount > 0)
		{
			dIn = ColorToDouble(m_arrVertexs[nVertexIndex].oColor.arrComp[0]);
			for (int nJ = 0; nJ < m_nFunctionsCount; ++nJ)
			{
				m_ppFunctions[nJ]->Transform(&dIn, &arrOut[nJ]);
			}
			for (int nJ = 0; nJ < GrColorMaxComps; ++nJ)
			{
				pColor2->arrComp[nJ] = DoubleToColor(arrOut[nJ]);
			}
		}
		else
		{
			*pColor2 = m_arrVertexs[nVertexIndex].oColor;
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrPatchMeshShading
	//-------------------------------------------------------------------------------------------------------------------------------

	GrPatchMeshShading::GrPatchMeshShading(int nType, GrPatch *pPatches, int nPatchesCount, Function **ppFunctions, int nFunctionsCount) :
		GrShading(nType)
	{
		m_pPatches        = pPatches;
		m_nPatchesCount   = nPatchesCount;
		m_nFunctionsCount = nFunctionsCount;
		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			m_ppFunctions[nIndex] = ppFunctions[nIndex];
		}
	}

	GrPatchMeshShading::GrPatchMeshShading(GrPatchMeshShading *pShading) :
		GrShading(pShading)
	{
		m_nPatchesCount = pShading->m_nPatchesCount;
		m_pPatches = (GrPatch *)MemUtilsMallocArray(m_nPatchesCount, sizeof(GrPatch));
		memcpy(m_pPatches, pShading->m_pPatches, m_nPatchesCount * sizeof(GrPatch));

		m_nFunctionsCount = pShading->m_nFunctionsCount;
		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			m_ppFunctions[nIndex] = pShading->m_ppFunctions[nIndex]->Copy();
		}
	}

	GrPatchMeshShading::~GrPatchMeshShading()
	{
		MemUtilsFree(m_pPatches);
		for (int nIndex = 0; nIndex < m_nFunctionsCount; ++nIndex)
		{
			if (m_ppFunctions[nIndex])
				delete m_ppFunctions[nIndex];
		}
	}

	GrPatchMeshShading *GrPatchMeshShading::Parse(int nType, Dict *pDict, Stream *pStream)
	{
		int nBitsPerCoordinate = 0, nBitsPerComponent = 0, nBitsPerFlag = 0;
		int nIndex = 0;

		Object oDictItem;
		if (pDict->Search("BitsPerCoordinate", &oDictItem)->IsInt())
		{
			nBitsPerCoordinate = oDictItem.GetInt();
		}
		else
		{
			// TO DO: Error "Missing or invalid BitsPerCoordinate in shading dictionary"
			oDictItem.Free();
			return NULL;
		}
		oDictItem.Free();

		if (pDict->Search("BitsPerComponent", &oDictItem)->IsInt())
		{
			nBitsPerComponent = oDictItem.GetInt();
		}
		else
		{
			// TO DO: Error "Missing or invalid BitsPerComponent in shading dictionary"
			oDictItem.Free();
			return NULL;
		}
		oDictItem.Free();

		if (pDict->Search("BitsPerFlag", &oDictItem)->IsInt())
		{
			nBitsPerFlag = oDictItem.GetInt();
		}
		else
		{
			// TO DO: Error "Missing or invalid BitsPerFlag in shading dictionary"
			oDictItem.Free();
			return NULL;
		}
		oDictItem.Free();

		double dXMin, dXMax, dYMin, dYMax;
		double arrCMin[GrColorMaxComps], arrCMax[GrColorMaxComps];
		double dXMul, dYMul;
		double arrCMul[GrColorMaxComps];
		int nComponentsCount = 0;

		// [ Xmin Xmax Ymin Ymax C1,min C1,max ... Cn,min Cn,max ], поэтому как минимум массив должен быть из 6 элементов
		if (pDict->Search("Decode", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() >= 6)
		{
			Object oTemp;
			dXMin = oDictItem.ArrayGet(0, &oTemp)->GetNum();
			oTemp.Free();
			dXMax = oDictItem.ArrayGet(1, &oTemp)->GetNum();
			oTemp.Free();
			dXMul = (dXMax - dXMin) / (pow(2.0, nBitsPerCoordinate) - 1);
			dYMin = oDictItem.ArrayGet(2, &oTemp)->GetNum();
			oTemp.Free();
			dYMax = oDictItem.ArrayGet(3, &oTemp)->GetNum();
			oTemp.Free();
			dYMul = (dYMax - dYMin) / (pow(2.0, nBitsPerCoordinate) - 1);
			for (nIndex = 0; 5 + 2 * nIndex < oDictItem.ArrayGetLength() && nIndex < GrColorMaxComps; ++nIndex)
			{
				arrCMin[nIndex] = oDictItem.ArrayGet(4 + 2 * nIndex, &oTemp)->GetNum();
				oTemp.Free();
				arrCMax[nIndex] = oDictItem.ArrayGet(5 + 2 * nIndex, &oTemp)->GetNum();
				oTemp.Free();
				arrCMul[nIndex] = (arrCMax[nIndex] - arrCMin[nIndex]) / (double)((1 << nBitsPerComponent) - 1);
			}
			nComponentsCount = nIndex;
		}
		else
		{
			// TO DO: Error "Missing or invalid Decode array in shading dictionary"
			oDictItem.Free();
			return NULL;
		}
		oDictItem.Free();

		int nFunctionsCount = 0;
		Function *ppFunctions[GrColorMaxComps];
		if (!pDict->Search("Function", &oDictItem)->IsNull())
		{
			if (oDictItem.IsArray())
			{
				nFunctionsCount = oDictItem.ArrayGetLength();
				if (nFunctionsCount > GrColorMaxComps)
				{
					// TO DO: Error "Invalid Function array in shading dictionary"
					oDictItem.Free();
					return NULL;
				}
				for (nIndex = 0; nIndex < nFunctionsCount; ++nIndex)
				{
					Object oTemp;
					oDictItem.ArrayGet(nIndex, &oTemp);
					if (!(ppFunctions[nIndex] = Function::Parse(&oTemp)))
					{
						oDictItem.Free();
						oTemp.Free();
						return NULL;
					}
					oDictItem.Free();
				}
			}
			else
			{
				nFunctionsCount = 1;
				if (!(ppFunctions[0] = Function::Parse(&oDictItem)))
				{
					oDictItem.Free();
					return NULL;
				}
			}
		}
		else
		{
			nFunctionsCount = 0;
		}
		oDictItem.Free();

		int nPatchesCount = 0, nPatchesSize = 0;
		GrPatch *pPatches = NULL;
		GrShadingBitBuffer *pBitBuffer = new GrShadingBitBuffer(pStream);

		double arrX[16], arrY[16];
		GrColorComp arrC[4][GrColorMaxComps];

		while (1)
		{
			unsigned int unFlag = 0;
			int nPointsCount = 0, nColorsCount = 0;
			if (!pBitBuffer->GetBits(nBitsPerFlag, &unFlag))
			{
				break;
			}
			if (nType == 6)
			{
				switch (unFlag)
				{
					case 0: nPointsCount = 12; nColorsCount = 4; break;
					case 1:
					case 2:
					case 3:
					default: nPointsCount =  8; nColorsCount = 2; break;
				}
			}
			else
			{
				switch (unFlag)
				{
					case 0: nPointsCount = 16; nColorsCount = 4; break;
					case 1:
					case 2:
					case 3:
					default: nPointsCount = 12; nColorsCount = 2; break;
				}
			}
			for (nIndex = 0; nIndex < nPointsCount; ++nIndex)
			{
				unsigned int unX = 0, unY = 0;
				if (!pBitBuffer->GetBits(nBitsPerCoordinate, &unX) || !pBitBuffer->GetBits(nBitsPerCoordinate, &unY))
				{
					break;
				}
				arrX[nIndex] = dXMin + dXMul * (double)unX;
				arrY[nIndex] = dYMin + dYMul * (double)unY;
			}
			if (nIndex < nPointsCount)
			{
				break;
			}
			for (nIndex = 0; nIndex < nColorsCount; ++nIndex)
			{
				int nJ = 0;
				unsigned int arrunC[4];
				for (nJ = 0; nJ < nComponentsCount; ++nJ)
				{
					if (!pBitBuffer->GetBits(nBitsPerComponent, &arrunC[nJ]))
					{
						break;
					}
					arrC[nIndex][nJ] = DoubleToColor(arrCMin[nJ] + arrCMul[nJ] * (double)arrunC[nJ]);
				}
				if (nJ < nComponentsCount)
				{
					break;
				}
			}
			if (nIndex < nColorsCount)
			{
				break;
			}
			if (nPatchesCount == nPatchesSize)
			{
				nPatchesSize = (nPatchesSize == 0) ? 16 : 2 * nPatchesSize;
				pPatches = (GrPatch *)MemUtilsReallocArray(pPatches, nPatchesSize, sizeof(GrPatch));
			}
			GrPatch *pCurPatch = &pPatches[nPatchesCount];
			if (nType == 6)
			{
				switch (unFlag)
				{
					case 0:
					pCurPatch->arrX[0][0] = arrX[0];  pCurPatch->arrY[0][0] = arrY[0];
					pCurPatch->arrX[0][1] = arrX[1];  pCurPatch->arrY[0][1] = arrY[1];
					pCurPatch->arrX[0][2] = arrX[2];  pCurPatch->arrY[0][2] = arrY[2];
					pCurPatch->arrX[0][3] = arrX[3];  pCurPatch->arrY[0][3] = arrY[3];
					pCurPatch->arrX[1][3] = arrX[4];  pCurPatch->arrY[1][3] = arrY[4];
					pCurPatch->arrX[2][3] = arrX[5];  pCurPatch->arrY[2][3] = arrY[5];
					pCurPatch->arrX[3][3] = arrX[6];  pCurPatch->arrY[3][3] = arrY[6];
					pCurPatch->arrX[3][2] = arrX[7];  pCurPatch->arrY[3][2] = arrY[7];
					pCurPatch->arrX[3][1] = arrX[8];  pCurPatch->arrY[3][1] = arrY[8];
					pCurPatch->arrX[3][0] = arrX[9];  pCurPatch->arrY[3][0] = arrY[9];
					pCurPatch->arrX[2][0] = arrX[10]; pCurPatch->arrY[2][0] = arrY[10];
					pCurPatch->arrX[1][0] = arrX[11]; pCurPatch->arrY[1][0] = arrY[11];
					for (int nJ = 0; nJ < nComponentsCount; ++nJ)
					{
						pCurPatch->arrColor[0][0].arrComp[nJ] = arrC[0][nJ];
						pCurPatch->arrColor[0][1].arrComp[nJ] = arrC[1][nJ];
						pCurPatch->arrColor[1][1].arrComp[nJ] = arrC[2][nJ];
						pCurPatch->arrColor[1][0].arrComp[nJ] = arrC[3][nJ];
					}
					break;
					case 1:
					pCurPatch->arrX[0][0] = pPatches[nPatchesCount - 1].arrX[0][3]; pCurPatch->arrY[0][0] = pPatches[nPatchesCount - 1].arrY[0][3];
					pCurPatch->arrX[0][1] = pPatches[nPatchesCount - 1].arrX[1][3]; pCurPatch->arrY[0][1] = pPatches[nPatchesCount - 1].arrY[1][3];
					pCurPatch->arrX[0][2] = pPatches[nPatchesCount - 1].arrX[2][3]; pCurPatch->arrY[0][2] = pPatches[nPatchesCount - 1].arrY[2][3];
					pCurPatch->arrX[0][3] = pPatches[nPatchesCount - 1].arrX[3][3]; pCurPatch->arrY[0][3] = pPatches[nPatchesCount - 1].arrY[3][3];
					pCurPatch->arrX[1][3] = arrX[0]; pCurPatch->arrY[1][3] = arrY[0];
					pCurPatch->arrX[2][3] = arrX[1]; pCurPatch->arrY[2][3] = arrY[1];
					pCurPatch->arrX[3][3] = arrX[2]; pCurPatch->arrY[3][3] = arrY[2];
					pCurPatch->arrX[3][2] = arrX[3]; pCurPatch->arrY[3][2] = arrY[3];
					pCurPatch->arrX[3][1] = arrX[4]; pCurPatch->arrY[3][1] = arrY[4];
					pCurPatch->arrX[3][0] = arrX[5]; pCurPatch->arrY[3][0] = arrY[5];
					pCurPatch->arrX[2][0] = arrX[6]; pCurPatch->arrY[2][0] = arrY[6];
					pCurPatch->arrX[1][0] = arrX[7]; pCurPatch->arrY[1][0] = arrY[7];
					for (int nJ = 0; nJ < nComponentsCount; ++nJ)
					{
						pCurPatch->arrColor[0][0].arrComp[nJ] = pPatches[nPatchesCount - 1].arrColor[0][1].arrComp[nJ];
						pCurPatch->arrColor[0][1].arrComp[nJ] = pPatches[nPatchesCount - 1].arrColor[1][1].arrComp[nJ];
						pCurPatch->arrColor[1][1].arrComp[nJ] = arrC[0][nJ];
						pCurPatch->arrColor[1][0].arrComp[nJ] = arrC[1][nJ];
					}
					break;
					case 2:
					pCurPatch->arrX[0][0] = pPatches[nPatchesCount - 1].arrX[3][3]; pCurPatch->arrY[0][0] = pPatches[nPatchesCount - 1].arrY[3][3];
					pCurPatch->arrX[0][1] = pPatches[nPatchesCount - 1].arrX[3][2]; pCurPatch->arrY[0][1] = pPatches[nPatchesCount - 1].arrY[3][2];
					pCurPatch->arrX[0][2] = pPatches[nPatchesCount - 1].arrX[3][1]; pCurPatch->arrY[0][2] = pPatches[nPatchesCount - 1].arrY[3][1];
					pCurPatch->arrX[0][3] = pPatches[nPatchesCount - 1].arrX[3][0]; pCurPatch->arrY[0][3] = pPatches[nPatchesCount - 1].arrY[3][0];
					pCurPatch->arrX[1][3] = arrX[0]; pCurPatch->arrY[1][3] = arrY[0];
					pCurPatch->arrX[2][3] = arrX[1]; pCurPatch->arrY[2][3] = arrY[1];
					pCurPatch->arrX[3][3] = arrX[2]; pCurPatch->arrY[3][3] = arrY[2];
					pCurPatch->arrX[3][2] = arrX[3]; pCurPatch->arrY[3][2] = arrY[3];
					pCurPatch->arrX[3][1] = arrX[4]; pCurPatch->arrY[3][1] = arrY[4];
					pCurPatch->arrX[3][0] = arrX[5]; pCurPatch->arrY[3][0] = arrY[5];
					pCurPatch->arrX[2][0] = arrX[6]; pCurPatch->arrY[2][0] = arrY[6];
					pCurPatch->arrX[1][0] = arrX[7]; pCurPatch->arrY[1][0] = arrY[7];
					for (int nJ = 0; nJ < nComponentsCount; ++nJ)
					{
						pCurPatch->arrColor[0][0].arrComp[nJ] = pPatches[nPatchesCount - 1].arrColor[1][1].arrComp[nJ];
						pCurPatch->arrColor[0][1].arrComp[nJ] = pPatches[nPatchesCount - 1].arrColor[1][0].arrComp[nJ];
						pCurPatch->arrColor[1][1].arrComp[nJ] = arrC[0][nJ];
						pCurPatch->arrColor[1][0].arrComp[nJ] = arrC[1][nJ];
					}
					break;
					case 3:
					pCurPatch->arrX[0][0] = pPatches[nPatchesCount - 1].arrX[3][0]; pCurPatch->arrY[0][0] = pPatches[nPatchesCount - 1].arrY[3][0];
					pCurPatch->arrX[0][1] = pPatches[nPatchesCount - 1].arrX[2][0]; pCurPatch->arrY[0][1] = pPatches[nPatchesCount - 1].arrY[2][0];
					pCurPatch->arrX[0][2] = pPatches[nPatchesCount - 1].arrX[1][0]; pCurPatch->arrY[0][2] = pPatches[nPatchesCount - 1].arrY[1][0];
					pCurPatch->arrX[0][3] = pPatches[nPatchesCount - 1].arrX[0][0]; pCurPatch->arrY[0][3] = pPatches[nPatchesCount - 1].arrY[0][0];
					pCurPatch->arrX[1][3] = arrX[0]; pCurPatch->arrY[1][3] = arrY[0];
					pCurPatch->arrX[2][3] = arrX[1]; pCurPatch->arrY[2][3] = arrY[1];
					pCurPatch->arrX[3][3] = arrX[2]; pCurPatch->arrY[3][3] = arrY[2];
					pCurPatch->arrX[3][2] = arrX[3]; pCurPatch->arrY[3][2] = arrY[3];
					pCurPatch->arrX[3][1] = arrX[4]; pCurPatch->arrY[3][1] = arrY[4];
					pCurPatch->arrX[3][0] = arrX[5]; pCurPatch->arrY[3][0] = arrY[5];
					pCurPatch->arrX[2][0] = arrX[6]; pCurPatch->arrY[2][0] = arrY[6];
					pCurPatch->arrX[1][0] = arrX[7]; pCurPatch->arrY[1][0] = arrY[7];
					for (int nJ = 0; nJ < nComponentsCount; ++nJ)
					{
						pCurPatch->arrColor[0][0].arrComp[nJ] = pPatches[nPatchesCount - 1].arrColor[1][0].arrComp[nJ];
						pCurPatch->arrColor[0][1].arrComp[nJ] = pPatches[nPatchesCount - 1].arrColor[0][0].arrComp[nJ];
						pCurPatch->arrColor[1][1].arrComp[nJ] = arrC[0][nJ];
						pCurPatch->arrColor[1][0].arrComp[nJ] = arrC[1][nJ];
					}
					break;
				}
			}
			else
			{
				switch (unFlag)
				{
					case 0:
					pCurPatch->arrX[0][0] = arrX[0];  pCurPatch->arrY[0][0] = arrY[0];
					pCurPatch->arrX[0][1] = arrX[1];  pCurPatch->arrY[0][1] = arrY[1];
					pCurPatch->arrX[0][2] = arrX[2];  pCurPatch->arrY[0][2] = arrY[2];
					pCurPatch->arrX[0][3] = arrX[3];  pCurPatch->arrY[0][3] = arrY[3];
					pCurPatch->arrX[1][3] = arrX[4];  pCurPatch->arrY[1][3] = arrY[4];
					pCurPatch->arrX[2][3] = arrX[5];  pCurPatch->arrY[2][3] = arrY[5];
					pCurPatch->arrX[3][3] = arrX[6];  pCurPatch->arrY[3][3] = arrY[6];
					pCurPatch->arrX[3][2] = arrX[7];  pCurPatch->arrY[3][2] = arrY[7];
					pCurPatch->arrX[3][1] = arrX[8];  pCurPatch->arrY[3][1] = arrY[8];
					pCurPatch->arrX[3][0] = arrX[9];  pCurPatch->arrY[3][0] = arrY[9];
					pCurPatch->arrX[2][0] = arrX[10]; pCurPatch->arrY[2][0] = arrY[10];
					pCurPatch->arrX[1][0] = arrX[11]; pCurPatch->arrY[1][0] = arrY[11];
					pCurPatch->arrX[1][1] = arrX[12]; pCurPatch->arrY[1][1] = arrY[12];
					pCurPatch->arrX[1][2] = arrX[13]; pCurPatch->arrY[1][2] = arrY[13];
					pCurPatch->arrX[2][2] = arrX[14]; pCurPatch->arrY[2][2] = arrY[14];
					pCurPatch->arrX[2][1] = arrX[15]; pCurPatch->arrY[2][1] = arrY[15];
					for (int nJ = 0; nJ < nComponentsCount; ++nJ)
					{
						pCurPatch->arrColor[0][0].arrComp[nJ] = arrC[0][nJ];
						pCurPatch->arrColor[0][1].arrComp[nJ] = arrC[1][nJ];
						pCurPatch->arrColor[1][1].arrComp[nJ] = arrC[2][nJ];
						pCurPatch->arrColor[1][0].arrComp[nJ] = arrC[3][nJ];
					}
					break;
					case 1:
					pCurPatch->arrX[0][0] = pPatches[nPatchesCount - 1].arrX[0][3]; pCurPatch->arrY[0][0] = pPatches[nPatchesCount - 1].arrY[0][3];
					pCurPatch->arrX[0][1] = pPatches[nPatchesCount - 1].arrX[1][3]; pCurPatch->arrY[0][1] = pPatches[nPatchesCount - 1].arrY[1][3];
					pCurPatch->arrX[0][2] = pPatches[nPatchesCount - 1].arrX[2][3]; pCurPatch->arrY[0][2] = pPatches[nPatchesCount - 1].arrY[2][3];
					pCurPatch->arrX[0][3] = pPatches[nPatchesCount - 1].arrX[3][3]; pCurPatch->arrY[0][3] = pPatches[nPatchesCount - 1].arrY[3][3];
					pCurPatch->arrX[1][3] = arrX[0];  pCurPatch->arrY[1][3] = arrY[0];
					pCurPatch->arrX[2][3] = arrX[1];  pCurPatch->arrY[2][3] = arrY[1];
					pCurPatch->arrX[3][3] = arrX[2];  pCurPatch->arrY[3][3] = arrY[2];
					pCurPatch->arrX[3][2] = arrX[3];  pCurPatch->arrY[3][2] = arrY[3];
					pCurPatch->arrX[3][1] = arrX[4];  pCurPatch->arrY[3][1] = arrY[4];
					pCurPatch->arrX[3][0] = arrX[5];  pCurPatch->arrY[3][0] = arrY[5];
					pCurPatch->arrX[2][0] = arrX[6];  pCurPatch->arrY[2][0] = arrY[6];
					pCurPatch->arrX[1][0] = arrX[7];  pCurPatch->arrY[1][0] = arrY[7];
					pCurPatch->arrX[1][1] = arrX[8];  pCurPatch->arrY[1][1] = arrY[8];
					pCurPatch->arrX[1][2] = arrX[9];  pCurPatch->arrY[1][2] = arrY[9];
					pCurPatch->arrX[2][2] = arrX[10]; pCurPatch->arrY[2][2] = arrY[10];
					pCurPatch->arrX[2][1] = arrX[11]; pCurPatch->arrY[2][1] = arrY[11];
					for (int nJ = 0; nJ < nComponentsCount; ++nJ)
					{
						pCurPatch->arrColor[0][0].arrComp[nJ] = pPatches[nPatchesCount - 1].arrColor[0][1].arrComp[nJ];
						pCurPatch->arrColor[0][1].arrComp[nJ] = pPatches[nPatchesCount - 1].arrColor[1][1].arrComp[nJ];
						pCurPatch->arrColor[1][1].arrComp[nJ] = arrC[0][nJ];
						pCurPatch->arrColor[1][0].arrComp[nJ] = arrC[1][nJ];
					}
					break;
					case 2:
					pCurPatch->arrX[0][0] = pPatches[nPatchesCount - 1].arrX[3][3]; pCurPatch->arrY[0][0] = pPatches[nPatchesCount - 1].arrY[3][3];
					pCurPatch->arrX[0][1] = pPatches[nPatchesCount - 1].arrX[3][2]; pCurPatch->arrY[0][1] = pPatches[nPatchesCount - 1].arrY[3][2];
					pCurPatch->arrX[0][2] = pPatches[nPatchesCount - 1].arrX[3][1]; pCurPatch->arrY[0][2] = pPatches[nPatchesCount - 1].arrY[3][1];
					pCurPatch->arrX[0][3] = pPatches[nPatchesCount - 1].arrX[3][0]; pCurPatch->arrY[0][3] = pPatches[nPatchesCount - 1].arrY[3][0];
					pCurPatch->arrX[1][3] = arrX[0];  pCurPatch->arrY[1][3] = arrY[0];
					pCurPatch->arrX[2][3] = arrX[1];  pCurPatch->arrY[2][3] = arrY[1];
					pCurPatch->arrX[3][3] = arrX[2];  pCurPatch->arrY[3][3] = arrY[2];
					pCurPatch->arrX[3][2] = arrX[3];  pCurPatch->arrY[3][2] = arrY[3];
					pCurPatch->arrX[3][1] = arrX[4];  pCurPatch->arrY[3][1] = arrY[4];
					pCurPatch->arrX[3][0] = arrX[5];  pCurPatch->arrY[3][0] = arrY[5];
					pCurPatch->arrX[2][0] = arrX[6];  pCurPatch->arrY[2][0] = arrY[6];
					pCurPatch->arrX[1][0] = arrX[7];  pCurPatch->arrY[1][0] = arrY[7];
					pCurPatch->arrX[1][1] = arrX[8];  pCurPatch->arrY[1][1] = arrY[8];
					pCurPatch->arrX[1][2] = arrX[9];  pCurPatch->arrY[1][2] = arrY[9];
					pCurPatch->arrX[2][2] = arrX[10]; pCurPatch->arrY[2][2] = arrY[10];
					pCurPatch->arrX[2][1] = arrX[11]; pCurPatch->arrY[2][1] = arrY[11];
					for (int nJ = 0; nJ < nComponentsCount; ++nJ)
					{
						pCurPatch->arrColor[0][0].arrComp[nJ] = pPatches[nPatchesCount - 1].arrColor[1][1].arrComp[nJ];
						pCurPatch->arrColor[0][1].arrComp[nJ] = pPatches[nPatchesCount - 1].arrColor[1][0].arrComp[nJ];
						pCurPatch->arrColor[1][1].arrComp[nJ] = arrC[0][nJ];
						pCurPatch->arrColor[1][0].arrComp[nJ] = arrC[1][nJ];
					}
					break;
					case 3:
					pCurPatch->arrX[0][0] = pPatches[nPatchesCount - 1].arrX[3][0]; pCurPatch->arrY[0][0] = pPatches[nPatchesCount - 1].arrY[3][0];
					pCurPatch->arrX[0][1] = pPatches[nPatchesCount - 1].arrX[2][0]; pCurPatch->arrY[0][1] = pPatches[nPatchesCount - 1].arrY[2][0];
					pCurPatch->arrX[0][2] = pPatches[nPatchesCount - 1].arrX[1][0]; pCurPatch->arrY[0][2] = pPatches[nPatchesCount - 1].arrY[1][0];
					pCurPatch->arrX[0][3] = pPatches[nPatchesCount - 1].arrX[0][0]; pCurPatch->arrY[0][3] = pPatches[nPatchesCount - 1].arrY[0][0];
					pCurPatch->arrX[1][3] = arrX[0];  pCurPatch->arrY[1][3] = arrY[0];
					pCurPatch->arrX[2][3] = arrX[1];  pCurPatch->arrY[2][3] = arrY[1];
					pCurPatch->arrX[3][3] = arrX[2];  pCurPatch->arrY[3][3] = arrY[2];
					pCurPatch->arrX[3][2] = arrX[3];  pCurPatch->arrY[3][2] = arrY[3];
					pCurPatch->arrX[3][1] = arrX[4];  pCurPatch->arrY[3][1] = arrY[4];
					pCurPatch->arrX[3][0] = arrX[5];  pCurPatch->arrY[3][0] = arrY[5];
					pCurPatch->arrX[2][0] = arrX[6];  pCurPatch->arrY[2][0] = arrY[6];
					pCurPatch->arrX[1][0] = arrX[7];  pCurPatch->arrY[1][0] = arrY[7];
					pCurPatch->arrX[1][1] = arrX[8];  pCurPatch->arrY[1][1] = arrY[8];
					pCurPatch->arrX[1][2] = arrX[9];  pCurPatch->arrY[1][2] = arrY[9];
					pCurPatch->arrX[2][2] = arrX[10]; pCurPatch->arrY[2][2] = arrY[10];
					pCurPatch->arrX[2][1] = arrX[11]; pCurPatch->arrY[2][1] = arrY[11];
					for (int nJ = 0; nJ < nComponentsCount; ++nJ)
					{
						pCurPatch->arrColor[0][0].arrComp[nJ] = pPatches[nPatchesCount - 1].arrColor[1][0].arrComp[nJ];
						pCurPatch->arrColor[0][1].arrComp[nJ] = pPatches[nPatchesCount - 1].arrColor[0][0].arrComp[nJ];
						pCurPatch->arrColor[1][1].arrComp[nJ] = arrC[0][nJ];
						pCurPatch->arrColor[1][0].arrComp[nJ] = arrC[1][nJ];
					}
					break;
				}
			}
			++nPatchesCount;
			pBitBuffer->FlushBits();
		}
		delete pBitBuffer;

		if (nType == 6)
		{
			for (nIndex = 0; nIndex < nPatchesCount; ++nIndex)
			{
				GrPatch *p = &pPatches[nIndex];
				p->arrX[1][1] = (-4 * p->arrX[0][0] + 6 * (p->arrX[0][1] + p->arrX[1][0]) - 2 * (p->arrX[0][3] + p->arrX[3][0]) + 3 * (p->arrX[3][1] + p->arrX[1][3]) - p->arrX[3][3]) / 9;
				p->arrY[1][1] = (-4 * p->arrY[0][0] + 6 * (p->arrY[0][1] + p->arrY[1][0]) - 2 * (p->arrY[0][3] + p->arrY[3][0]) + 3 * (p->arrY[3][1] + p->arrY[1][3]) - p->arrY[3][3]) / 9;

				p->arrX[1][2] = (-4 * p->arrX[0][3] + 6 * (p->arrX[0][2] + p->arrX[1][3]) - 2 * (p->arrX[0][0] + p->arrX[3][3]) + 3 * (p->arrX[3][2] + p->arrX[1][0]) - p->arrX[3][0]) / 9;
				p->arrY[1][2] = (-4 * p->arrY[0][3] + 6 * (p->arrY[0][2] + p->arrY[1][3]) - 2 * (p->arrY[0][0] + p->arrY[3][3]) + 3 * (p->arrY[3][2] + p->arrY[1][0]) - p->arrY[3][0]) / 9;

				p->arrX[2][1] = (-4 * p->arrX[3][0] + 6 * (p->arrX[3][1] + p->arrX[2][0]) - 2 * (p->arrX[3][3] + p->arrX[0][0]) + 3 * (p->arrX[0][1] + p->arrX[2][3]) - p->arrX[0][3]) / 9;
				p->arrY[2][1] = (-4 * p->arrY[3][0] + 6 * (p->arrY[3][1] + p->arrY[2][0]) - 2 * (p->arrY[3][3] + p->arrY[0][0]) + 3 * (p->arrY[0][1] + p->arrY[2][3]) - p->arrY[0][3]) / 9;

				p->arrX[2][2] = (-4 * p->arrX[3][3] + 6 * (p->arrX[3][2] + p->arrX[2][3]) - 2 * (p->arrX[3][0] + p->arrX[0][3]) + 3 * (p->arrX[0][2] + p->arrX[2][0]) - p->arrX[0][0]) / 9;
				p->arrY[2][2] = (-4 * p->arrY[3][3] + 6 * (p->arrY[3][2] + p->arrY[2][3]) - 2 * (p->arrY[3][0] + p->arrY[0][3]) + 3 * (p->arrY[0][2] + p->arrY[2][0]) - p->arrY[0][0]) / 9;
			}
		}

		GrPatchMeshShading *pShading = new GrPatchMeshShading(nType, pPatches, nPatchesCount, ppFunctions, nFunctionsCount);
		if (!pShading)
			return NULL;
		if (!pShading->Initialize(pDict))
		{
			delete pShading;
			return NULL;
		}
		return pShading;
	}

	GrShading *GrPatchMeshShading::Copy()
	{
		return new GrPatchMeshShading(this);
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrImageColorMap
	//-------------------------------------------------------------------------------------------------------------------------------

	GrImageColorMap::GrImageColorMap(int nBitsPerComponent, Object *pDecode, GrColorSpace *pColorSpace)
	{
		m_bSuccess = true;

		m_nBitsPerComponent = nBitsPerComponent;
		int nMaxPixelIndex = (1 << m_nBitsPerComponent) - 1;
		m_pColorSpace = pColorSpace;

		for (int nIndex = 0; nIndex < GrColorMaxComps; ++nIndex)
		{
			m_ppLookup[nIndex] = NULL;
		}

		if (pDecode->IsNull())
		{
			m_nComponentsCount = m_pColorSpace->GetComponentsCount();
			m_pColorSpace->GetDefaultRanges(m_arrDecodeLow, m_arrDecodeRange, nMaxPixelIndex);
		}
		else if (pDecode->IsArray())
		{
			m_nComponentsCount = pDecode->ArrayGetLength() / 2;
			if (m_nComponentsCount != m_pColorSpace->GetComponentsCount())
			{
				m_bSuccess = false;
				return;
			}
			for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
			{
				Object oTemp;
				pDecode->ArrayGet(2 * nIndex, &oTemp);
				if (!oTemp.IsNum())
				{
					oTemp.Free();
					m_bSuccess = false;
					return;
				}
				m_arrDecodeLow[nIndex] = oTemp.GetNum();
				oTemp.Free();

				pDecode->ArrayGet(2 * nIndex + 1, &oTemp);
				if (!oTemp.IsNum())
				{
					oTemp.Free();
					m_bSuccess = false;
					return;
				}
				m_arrDecodeRange[nIndex] = oTemp.GetNum() - m_arrDecodeLow[nIndex];
				oTemp.Free();
			}
		}
		else
		{
			m_bSuccess = false;
			return;
		}

		// Строим таблицу поиска, в которой будут храниться, предварительно вычисленные(декодированные),
		// значения какждой компоненты. 
		// Оптимизация для цветовых пространств Indexed and Separation(имеющих одну компоненту): 
		// в таблице будет хранится значения цвета, а не значения компонент.
		m_pColorSpace2 = NULL;
		m_nComponentsCount2 = 0;
		double arrX[GrColorMaxComps], arrY[GrColorMaxComps];
		if (m_pColorSpace->GetMode() == csIndexed)
		{
			// Возможно, что nHival != nMaxPixelIndex 
			GrIndexedColorSpace *pIndexedCS = (GrIndexedColorSpace *)m_pColorSpace;
			m_pColorSpace2 = pIndexedCS->GetBase();
			int nHival = pIndexedCS->GetHival();
			m_nComponentsCount2 = m_pColorSpace2->GetComponentsCount();
			unsigned char *pLookup2 = pIndexedCS->GetLookup();
			m_pColorSpace2->GetDefaultRanges(arrX, arrY, nHival);

			for (int nComp = 0; nComp < m_nComponentsCount2; ++nComp)
			{
				m_ppLookup[nComp] = (GrColorComp *)MemUtilsMallocArray(nMaxPixelIndex + 1, sizeof(GrColorComp));
				for (int nIndex = 0; nIndex <= nMaxPixelIndex; ++nIndex)
				{
					int nCurIndex = (int)(m_arrDecodeLow[0] + (nIndex * m_arrDecodeRange[0]) / nMaxPixelIndex + 0.5);
					if (nCurIndex < 0)
					{
						nCurIndex = 0;
					}
					else if (nCurIndex > nHival)
					{
						nCurIndex = nHival;
					}
					m_ppLookup[nComp][nIndex] = DoubleToColor(arrX[nComp] + (pLookup2[nCurIndex * m_nComponentsCount2 + nComp] / 255.0) * arrY[nComp]);
				}
			}
		}
		else if (m_pColorSpace->GetMode() == csSeparation)
		{
			GrSeparationColorSpace *pSeparateCS = (GrSeparationColorSpace *)m_pColorSpace;
			m_pColorSpace2 = pSeparateCS->GetAlternateSpace();
			m_nComponentsCount2 = m_pColorSpace2->GetComponentsCount();
			Function *pSepFunc = pSeparateCS->GetTransformFunction();

			for (int nComp = 0; nComp < m_nComponentsCount2; ++nComp)
			{
				m_ppLookup[nComp] = (GrColorComp *)MemUtilsMallocArray(nMaxPixelIndex + 1, sizeof(GrColorComp));
				for (int nIndex = 0; nIndex <= nMaxPixelIndex; ++nIndex)
				{
					arrX[0] = m_arrDecodeLow[0] + (nIndex * m_arrDecodeRange[0]) / nMaxPixelIndex;
					pSepFunc->Transform(arrX, arrY);
					m_ppLookup[nComp][nIndex] = DoubleToColor(arrY[nComp]);
				}
			}
		}
		else
		{
			for (int nComp = 0; nComp < m_nComponentsCount; ++nComp)
			{
				m_ppLookup[nComp] = (GrColorComp *)MemUtilsMallocArray(nMaxPixelIndex + 1, sizeof(GrColorComp));
				for (int nIndex = 0; nIndex <= nMaxPixelIndex; ++nIndex)
				{
					m_ppLookup[nComp][nIndex] = DoubleToColor(m_arrDecodeLow[nComp] + (nIndex * m_arrDecodeRange[nComp]) / nMaxPixelIndex);
				}
			}
		}

		return;
	}

	GrImageColorMap::GrImageColorMap(GrImageColorMap *pColorMap)
	{
		m_pColorSpace       = pColorMap->m_pColorSpace->Copy();
		m_nBitsPerComponent = pColorMap->m_nBitsPerComponent;
		m_nComponentsCount  = pColorMap->m_nComponentsCount;
		m_nComponentsCount2 = pColorMap->m_nComponentsCount2;
		m_pColorSpace2 = NULL;

		for (int nIndex = 0; nIndex < GrColorMaxComps; ++nIndex)
		{
			m_ppLookup[nIndex] = NULL;
		}
		int nBitsCount = 1 << m_nBitsPerComponent;
		if (m_pColorSpace->GetMode() == csIndexed)
		{
			m_pColorSpace2 = ((GrIndexedColorSpace *)m_pColorSpace)->GetBase();
			for (int nIndex = 0; nIndex < m_nComponentsCount2; ++nIndex)
			{
				m_ppLookup[nIndex] = (GrColorComp *)MemUtilsMallocArray(nBitsCount, sizeof(GrColorComp));
				memcpy(m_ppLookup[nIndex], pColorMap->m_ppLookup[nIndex], nBitsCount * sizeof(GrColorComp));
			}
		}
		else if (m_pColorSpace->GetMode() == csSeparation)
		{
			m_pColorSpace2 = ((GrSeparationColorSpace *)m_pColorSpace)->GetAlternateSpace();
			for (int nIndex = 0; nIndex < m_nComponentsCount2; ++nIndex)
			{
				m_ppLookup[nIndex] = (GrColorComp *)MemUtilsMallocArray(nBitsCount, sizeof(GrColorComp));
				memcpy(m_ppLookup[nIndex], pColorMap->m_ppLookup[nIndex], nBitsCount * sizeof(GrColorComp));
			}
		}
		else
		{
			for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
			{
				m_ppLookup[nIndex] = (GrColorComp *)MemUtilsMallocArray(nBitsCount, sizeof(GrColorComp));
				memcpy(m_ppLookup[nIndex], pColorMap->m_ppLookup[nIndex], nBitsCount * sizeof(GrColorComp));
			}
		}
		for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
		{
			m_arrDecodeLow[nIndex]   = pColorMap->m_arrDecodeLow[nIndex];
			m_arrDecodeRange[nIndex] = pColorMap->m_arrDecodeRange[nIndex];
		}
		m_bSuccess = true;
	}

	GrImageColorMap::~GrImageColorMap()
	{
		if (m_pColorSpace)
			delete m_pColorSpace;

		for (int nIndex = 0; nIndex < GrColorMaxComps; ++nIndex)
		{
			MemUtilsFree(m_ppLookup[nIndex]);
		}
	}

	void GrImageColorMap::GetGray(unsigned char *pColorValue, GrGray  *pGray)
	{
		GrColor oColor;

		if (m_pColorSpace2)
		{
			for (int nIndex = 0; nIndex < m_nComponentsCount2; ++nIndex)
			{
				oColor.arrComp[nIndex] = m_ppLookup[nIndex][pColorValue[0]];
			}
			m_pColorSpace2->GetGray(&oColor, pGray);
		}
		else
		{
			for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
			{
				oColor.arrComp[nIndex] = m_ppLookup[nIndex][pColorValue[nIndex]];
			}
			m_pColorSpace->GetGray(&oColor, pGray);
		}
	}

	void GrImageColorMap::GetRGB(unsigned char *pColorValue, GrRGB   *pRGB)
	{
		GrColor oColor;

		if (m_pColorSpace2)
		{
			for (int nIndex = 0; nIndex < m_nComponentsCount2; ++nIndex)
			{
				oColor.arrComp[nIndex] = m_ppLookup[nIndex][pColorValue[0]];
			}
			m_pColorSpace2->GetRGB(&oColor, pRGB);
		}
		else
		{
			for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
			{
				oColor.arrComp[nIndex] = m_ppLookup[nIndex][pColorValue[nIndex]];
			}
			m_pColorSpace->GetRGB(&oColor, pRGB);
		}
	}

	void GrImageColorMap::GetCMYK(unsigned char *pColorValue, GrCMYK  *pCMYK)
	{
		GrColor oColor;

		if (m_pColorSpace2)
		{
			for (int nIndex = 0; nIndex < m_nComponentsCount2; ++nIndex)
			{
				oColor.arrComp[nIndex] = m_ppLookup[nIndex][pColorValue[0]];
			}
			m_pColorSpace2->GetCMYK(&oColor, pCMYK);
		}
		else
		{
			for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
			{
				oColor.arrComp[nIndex] = m_ppLookup[nIndex][pColorValue[nIndex]];
			}
			m_pColorSpace->GetCMYK(&oColor, pCMYK);
		}
	}

	void GrImageColorMap::GetColor(unsigned char *pColorValue, GrColor *pColor)
	{
		int nMaxPixel = (1 << m_nBitsPerComponent) - 1;
		for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
		{
			pColor->arrComp[nIndex] = DoubleToColor(m_arrDecodeLow[nIndex] + (pColorValue[nIndex] * m_arrDecodeRange[nIndex]) / nMaxPixel);
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrSubpath and GrPath
	//-------------------------------------------------------------------------------------------------------------------------------

	GrSubpath::GrSubpath(double dX, double dY)
	{
		m_nSize = 16;
		m_pX = (double *)MemUtilsMallocArray(m_nSize, sizeof(double));
		m_pY = (double *)MemUtilsMallocArray(m_nSize, sizeof(double));
		m_pbCurve = (bool *)MemUtilsMallocArray(m_nSize, sizeof(bool));
		m_nPointsCount = 1;
		m_pX[0] = dX;
		m_pY[0] = dY;
		m_pbCurve[0] = false;
		m_bClosed = false;
	}

	GrSubpath::~GrSubpath()
	{
		MemUtilsFree(m_pX);
		MemUtilsFree(m_pY);
		MemUtilsFree(m_pbCurve);
	}

	GrSubpath::GrSubpath(GrSubpath *pSubpath)
	{
		m_nSize        = pSubpath->m_nSize;
		m_nPointsCount = pSubpath->m_nPointsCount;
		m_pX = (double *)MemUtilsMallocArray(m_nSize, sizeof(double));
		m_pY = (double *)MemUtilsMallocArray(m_nSize, sizeof(double));
		m_pbCurve = (bool *)MemUtilsMallocArray(m_nSize, sizeof(bool));
		memcpy(m_pX, pSubpath->m_pX, m_nPointsCount * sizeof(double));
		memcpy(m_pY, pSubpath->m_pY, m_nPointsCount * sizeof(double));
		memcpy(m_pbCurve, pSubpath->m_pbCurve, m_nPointsCount * sizeof(bool));
		m_bClosed = pSubpath->m_bClosed;
	}

	void GrSubpath::LineTo(double dX, double dY)
	{
		if (m_nPointsCount >= m_nSize)
		{
			m_nSize += 16;
			m_pX = (double *)MemUtilsReallocArray(m_pX, m_nSize, sizeof(double));
			m_pY = (double *)MemUtilsReallocArray(m_pY, m_nSize, sizeof(double));
			m_pbCurve = (bool *)MemUtilsReallocArray(m_pbCurve, m_nSize, sizeof(bool));
		}
		m_pX[m_nPointsCount] = dX;
		m_pY[m_nPointsCount] = dY;
		m_pbCurve[m_nPointsCount] = false;
		++m_nPointsCount;
	}

	void GrSubpath::CurveTo(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3)
	{
		if (m_nPointsCount + 3 > m_nSize)
		{
			m_nSize += 16;
			m_pX = (double *)MemUtilsReallocArray(m_pX, m_nSize, sizeof(double));
			m_pY = (double *)MemUtilsReallocArray(m_pY, m_nSize, sizeof(double));
			m_pbCurve = (bool *)MemUtilsReallocArray(m_pbCurve, m_nSize, sizeof(bool));
		}
		m_pX[m_nPointsCount + 0] = dX1;
		m_pY[m_nPointsCount + 0] = dY1;
		m_pX[m_nPointsCount + 1] = dX2;
		m_pY[m_nPointsCount + 1] = dY2;
		m_pX[m_nPointsCount + 2] = dX3;
		m_pY[m_nPointsCount + 2] = dY3;
		m_pbCurve[m_nPointsCount + 0] = m_pbCurve[m_nPointsCount + 1] = true;
		m_pbCurve[m_nPointsCount + 2] = false;
		m_nPointsCount += 3;
	}

	void GrSubpath::Close()
	{
		if (m_pX[m_nPointsCount - 1] != m_pX[0] || m_pY[m_nPointsCount - 1] != m_pY[0])
		{
			LineTo(m_pX[0], m_pY[0]);
		}
		m_bClosed = true;
	}

	void GrSubpath::Offset(double dDx, double dDy)
	{
		for (int nIndex = 0; nIndex < m_nPointsCount; ++nIndex)
		{
			m_pX[nIndex] += dDx;
			m_pY[nIndex] += dDy;
		}
	}

	void GrSubpath::Transform(double *pMatrix)
	{
		for (int nIndex = 0; nIndex < m_nPointsCount; nIndex++)
		{
			double dOldX = m_pX[nIndex];
			double dOldY = m_pY[nIndex];

			m_pX[nIndex] = dOldX * pMatrix[0] + dOldY * pMatrix[2] + pMatrix[4];
			m_pY[nIndex] = dOldX * pMatrix[1] + dOldY * pMatrix[3] + pMatrix[5];
		}
	}
	GrPath::GrPath()
	{
		m_bJustStarted = false;
		m_nSize = 16;
		m_nSubpathsCount = 0;
		m_dFirstX = m_dFirstY = 0;
		m_ppSubpaths = (GrSubpath **)MemUtilsMallocArray(m_nSize, sizeof(GrSubpath *));
	}

	GrPath::~GrPath()
	{
		for (int nIndex = 0; nIndex < m_nSubpathsCount; ++nIndex)
		{
			if (m_ppSubpaths[nIndex])
				delete m_ppSubpaths[nIndex];
		}
		MemUtilsFree(m_ppSubpaths);
	}

	GrPath::GrPath(bool bJustStarted, double dFirstX, double dFirstY, GrSubpath **ppSubpaths, int nSubpathCount, int nSize)
	{
		m_bJustStarted = bJustStarted;
		m_dFirstX = dFirstX;
		m_dFirstY = dFirstY;
		m_nSize   = nSize;
		m_nSubpathsCount = nSubpathCount;
		m_ppSubpaths = (GrSubpath **)MemUtilsMallocArray(m_nSize, sizeof(GrSubpath *));
		for (int nIndex = 0; nIndex < m_nSubpathsCount; ++nIndex)
			m_ppSubpaths[nIndex] = ppSubpaths[nIndex]->Copy();
	}

	void GrPath::MoveTo(double dX, double dY)
	{
		m_bJustStarted = true;
		m_dFirstX = dX;
		m_dFirstY = dY;
	}

	void GrPath::LineTo(double dX, double dY)
	{
		if (m_bJustStarted)
		{
			if (m_nSubpathsCount >= m_nSize)
			{
				m_nSize += 16;
				m_ppSubpaths = (GrSubpath **)MemUtilsReallocArray(m_ppSubpaths, m_nSize, sizeof(GrSubpath *));
			}
			m_ppSubpaths[m_nSubpathsCount] = new GrSubpath(m_dFirstX, m_dFirstY);
			++m_nSubpathsCount;
			m_bJustStarted = false;
		}
		m_ppSubpaths[m_nSubpathsCount - 1]->LineTo(dX, dY);
	}

	void GrPath::CurveTo(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3)
	{
		if (m_bJustStarted)
		{
			if (m_nSubpathsCount >= m_nSize)
			{
				m_nSize += 16;
				m_ppSubpaths = (GrSubpath **)MemUtilsReallocArray(m_ppSubpaths, m_nSize, sizeof(GrSubpath *));
			}
			m_ppSubpaths[m_nSubpathsCount] = new GrSubpath(m_dFirstX, m_dFirstY);
			++m_nSubpathsCount;
			m_bJustStarted = false;
		}
		m_ppSubpaths[m_nSubpathsCount - 1]->CurveTo(dX1, dY1, dX2, dY2, dX3, dY3);
	}

	void GrPath::Close()
	{
		if (m_bJustStarted)
		{
			if (m_nSubpathsCount >= m_nSize)
			{
				m_nSize += 16;
				m_ppSubpaths = (GrSubpath **)MemUtilsReallocArray(m_ppSubpaths, m_nSize, sizeof(GrSubpath *));
			}
			m_ppSubpaths[m_nSubpathsCount] = new GrSubpath(m_dFirstX, m_dFirstY);
			++m_nSubpathsCount;
			m_bJustStarted = false;
		}
		m_ppSubpaths[m_nSubpathsCount - 1]->Close();
	}

	void GrPath::Append(GrPath *pPath)
	{
		if (m_nSubpathsCount + pPath->m_nSubpathsCount > m_nSize)
		{
			m_nSize = m_nSubpathsCount + pPath->m_nSubpathsCount;
			m_ppSubpaths = (GrSubpath **)MemUtilsReallocArray(m_ppSubpaths, m_nSize, sizeof(GrSubpath *));
		}
		for (int nIndex = 0; nIndex < pPath->m_nSubpathsCount; ++nIndex)
		{
			m_ppSubpaths[m_nSubpathsCount++] = pPath->m_ppSubpaths[nIndex]->Copy();
		}
		m_bJustStarted = false;
	}

	void GrPath::Offset(double dDx, double dDy)
	{
		for (int nIndex = 0; nIndex < m_nSubpathsCount; ++nIndex)
		{
			m_ppSubpaths[nIndex]->Offset(dDx, dDy);
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// GrClip
	//-------------------------------------------------------------------------------------------------------------------------------

	GrClip::GrClip(double dMinX, double dMinY, double dMaxX, double dMaxY)
	{
		if (dMinX < dMaxX)
		{
			m_dMinX = dMinX;
			m_dMaxX = dMaxX;
		}
		else
		{
			m_dMinX = dMaxX;
			m_dMaxX = dMinX;
		}
		if (dMinY < dMaxY)
		{
			m_dMinY = dMinY;
			m_dMaxY = dMaxY;
		}
		else
		{
			m_dMinY = dMaxY;
			m_dMaxY = dMinY;
		}

		m_nMinX = (int)floor(m_dMinX);
		m_nMinY = (int)floor(m_dMinY);
		m_nMaxX = (int)floor(m_dMaxX);
		m_nMaxY = (int)floor(m_dMaxY);

		m_nPathsCount = m_nSize = 0;
		m_ppPaths   = NULL;
		m_pMatrix   = NULL;
		m_pFlags    = NULL;

		m_pTextClip = new GrTextClip();
	}

	GrClip::GrClip(GrClip *pClip)
	{
		m_dMinX = pClip->m_dMinX;
		m_dMinY = pClip->m_dMinY;
		m_dMaxX = pClip->m_dMaxX;
		m_dMaxY = pClip->m_dMaxY;
		m_nMinX = pClip->m_nMinX;
		m_nMinY = pClip->m_nMinY;
		m_nMaxX = pClip->m_nMaxX;
		m_nMaxY = pClip->m_nMaxY;

		m_nPathsCount = pClip->m_nPathsCount;
		m_nSize = pClip->m_nSize;

		m_ppPaths   = (GrPath        **)MemUtilsMallocArray(m_nSize, sizeof(GrPath *));
		m_pMatrix   = (Matrix         *)MemUtilsMallocArray(m_nSize, sizeof(Matrix));
		m_pFlags    = (unsigned char  *)MemUtilsMallocArray(m_nSize, sizeof(unsigned char));

		for (int nIndex = 0; nIndex < m_nPathsCount; ++nIndex)
		{
			m_ppPaths[nIndex]   = pClip->m_ppPaths[nIndex]->Copy();
			m_pMatrix[nIndex]   = pClip->m_pMatrix[nIndex];
			m_pFlags[nIndex]    = pClip->m_pFlags[nIndex];
		}

		m_pTextClip = pClip->m_pTextClip->Copy();
	}

	GrClip::~GrClip()
	{
		for (int nIndex = 0; nIndex < m_nPathsCount; ++nIndex)
		{
			if (m_ppPaths[nIndex])
			{
				delete m_ppPaths[nIndex];
			}
		}

		MemUtilsFree(m_ppPaths);
		MemUtilsFree(m_pMatrix);
		MemUtilsFree(m_pFlags);

		if (m_pTextClip)
			delete m_pTextClip;
	}

	void GrClip::Resize(int nPathsCount)
	{
		if (m_nPathsCount + nPathsCount > m_nSize)
		{
			if (m_nSize == 0)
			{
				m_nSize = 32;
			}
			while (m_nSize < m_nPathsCount + nPathsCount)
			{
				m_nSize *= 2;
			}

			m_ppPaths   = (GrPath        **)MemUtilsReallocArray(m_ppPaths, m_nSize, sizeof(GrPath *));
			m_pMatrix   = (Matrix         *)MemUtilsReallocArray(m_pMatrix, m_nSize, sizeof(Matrix));
			m_pFlags    = (unsigned char  *)MemUtilsReallocArray(m_pFlags, m_nSize, sizeof(unsigned char));
		}
	}

	void GrClip::ResetToRect(double dX0, double dY0, double dX1, double dY1)
	{
		for (int nIndex = 0; nIndex < m_nPathsCount; ++nIndex)
		{
			if (m_ppPaths[nIndex])
			{
				delete m_ppPaths[nIndex];
			}
		}

		MemUtilsFree(m_ppPaths);
		MemUtilsFree(m_pMatrix);
		MemUtilsFree(m_pFlags);

		m_ppPaths   = NULL;
		m_pFlags    = NULL;

		m_nPathsCount = m_nSize = 0;

		if (dX0 < dX1)
		{
			m_dMinX = dX0;
			m_dMaxX = dX1;
		}
		else
		{
			m_dMinX = dX1;
			m_dMaxX = dX0;
		}

		if (dY0 < dY1)
		{
			m_dMinY = dY0;
			m_dMaxY = dY1;
		}
		else
		{
			m_dMinY = dY1;
			m_dMaxY = dY0;
		}

		m_nMinX = (int)floor(m_dMinX);
		m_nMinY = (int)floor(m_dMinY);
		m_nMaxX = (int)floor(m_dMaxX);
		m_nMaxY = (int)floor(m_dMaxY);
	}

	void GrClip::ClipToRect(double dX0, double dY0, double dX1, double dY1)
	{
		if (dX0 < dX1)
		{
			if (dX0 > m_dMinX)
			{
				m_dMinX = dX0;
				m_nMinX = (int)floor(m_dMinX);
			}
			if (dX1 < m_dMaxX)
			{
				m_dMaxX = dX1;
				m_nMaxX = (int)floor(m_dMaxX);
			}
		}
		else
		{
			if (dX1 > m_dMinX)
			{
				m_dMinX = dX1;
				m_nMinX = (int)floor(m_dMinX);
			}
			if (dX0 < m_dMaxX)
			{
				m_dMaxX = dX0;
				m_nMaxX = (int)floor(m_dMaxX);
			}
		}

		if (dY0 < dY1)
		{
			if (dY0 > m_dMinY)
			{
				m_dMinY = dY0;
				m_nMinY = (int)floor(m_dMinY);
			}
			if (dY1 < m_dMaxY)
			{
				m_dMaxY = dY1;
				m_nMaxY = (int)floor(m_dMaxY);
			}
		}
		else
		{
			if (dY1 > m_dMinY)
			{
				m_dMinY = dY1;
				m_nMinY = (int)floor(m_dMinY);
			}
			if (dY0 < m_dMaxY)
			{
				m_dMaxY = dY0;
				m_nMaxY = (int)floor(m_dMaxY);
			}
		}

		return;
	}

	void GrClip::ClipToPath(GrPath *pPath, double *pMatrix, bool bEO)
	{
		Resize(1);

		m_ppPaths[m_nPathsCount] = pPath;
		m_pMatrix[m_nPathsCount].FromDoublePointer(pMatrix);
		m_pFlags[m_nPathsCount]  = (bEO ? GrClipEOFlag : 0);
		++m_nPathsCount;
		return;
	}
	GrPath *GrClip::PathRect(double dX0, double dY0, double dX1, double dY1)
	{
		GrPath *pPath = new GrPath();

		pPath->MoveTo(dX0, dY0);
		pPath->LineTo(dX1, dY0);
		pPath->LineTo(dX1, dY1);
		pPath->LineTo(dX0, dY1);
		pPath->Close();

		return pPath;
	}
	//-------------------------------------------------------------------------------------------------------------------------------
	// GrState
	//-------------------------------------------------------------------------------------------------------------------------------

	GrState::GrState(double dHorizDPI, double dVertDPI, PDFRectangle *pPageBox, int nRotateAngle, bool bUpsideDown)
	{
		m_dHorDPI = dHorizDPI;
		m_dVerDPI = dVertDPI;
		m_nRotate = nRotateAngle;
		m_dPageLeft   = pPageBox->m_dLeft;
		m_dPageBottom = pPageBox->m_dBottom;
		m_dPageRight  = pPageBox->m_dRight;
		m_dPageTop    = pPageBox->m_dTop;

		double dKoefX = m_dHorDPI / 72.0;
		double dKoefY = m_dVerDPI / 72.0;
		if (m_nRotate == 90)
		{
			m_arrCTM[0] = 0;
			m_arrCTM[1] = bUpsideDown ? dKoefY : -dKoefY;
			m_arrCTM[2] = dKoefX;
			m_arrCTM[3] = 0;
			m_arrCTM[4] = -dKoefX * m_dPageBottom;
			m_arrCTM[5] = dKoefY * (bUpsideDown ? -m_dPageLeft : m_dPageRight);
			m_dPageWidth  = dKoefX * (m_dPageTop - m_dPageBottom);
			m_dPageHeight = dKoefY * (m_dPageRight - m_dPageLeft);
		}
		else if (m_nRotate == 180)
		{
			m_arrCTM[0] = -dKoefX;
			m_arrCTM[1] = 0;
			m_arrCTM[2] = 0;
			m_arrCTM[3] = bUpsideDown ? dKoefY : -dKoefY;
			m_arrCTM[4] = dKoefX * m_dPageRight;
			m_arrCTM[5] = dKoefY * (bUpsideDown ? -m_dPageBottom : m_dPageTop);
			m_dPageWidth  = dKoefX * (m_dPageRight - m_dPageLeft);
			m_dPageHeight = dKoefY * (m_dPageTop - m_dPageBottom);
		}
		else if (m_nRotate == 270)
		{
			m_arrCTM[0] = 0;
			m_arrCTM[1] = bUpsideDown ? -dKoefY : dKoefY;
			m_arrCTM[2] = -dKoefX;
			m_arrCTM[3] = 0;
			m_arrCTM[4] = dKoefX * m_dPageTop;
			m_arrCTM[5] = dKoefY * (bUpsideDown ? m_dPageRight : -m_dPageLeft);
			m_dPageWidth  = dKoefX * (m_dPageTop - m_dPageBottom);
			m_dPageHeight = dKoefY * (m_dPageRight - m_dPageLeft);
		}
		else
		{
			m_arrCTM[0] = dKoefX;
			m_arrCTM[1] = 0;
			m_arrCTM[2] = 0;
			m_arrCTM[3] = bUpsideDown ? -dKoefY : dKoefY;
			m_arrCTM[4] = -dKoefX * m_dPageLeft;
			m_arrCTM[5] = dKoefY * (bUpsideDown ? m_dPageTop : -m_dPageBottom);
			m_dPageWidth  = dKoefX * (m_dPageRight - m_dPageLeft);
			m_dPageHeight = dKoefY * (m_dPageTop - m_dPageBottom);
		}

		m_pFillColorSpace   = new GrDeviceGrayColorSpace();
		m_pStrokeColorSpace = new GrDeviceGrayColorSpace();
		m_oFillColor.arrComp[0]   = 0;
		m_oStrokeColor.arrComp[0] = 0;
		m_pFillPattern   = NULL;
		m_pStrokePattern = NULL;
		m_eBlendMode = grBlendNormal;
		m_dFillOpacity   = 1;
		m_dStrokeOpacity = 1;
		m_bFillOverprint   = false;
		m_bStrokeOverprint = false;
		m_ppTransfer[0] = m_ppTransfer[1] = m_ppTransfer[2] = m_ppTransfer[3] = NULL;

		m_dLineWidth     = 1;
		m_pLineDash      = NULL;
		m_nLineDashSize  = 0;
		m_dLineDashStart = 0;
		m_nFlatness      = 1;
		m_nLineJoin      = 0;
		m_nLineCap       = 0;
		m_dMiterLimit     = 10;
		m_bStrokeAdjust  = false;

		m_pFont = NULL;
		m_dFontSize = 0;
		m_arrTextMatrix[0] = 1; m_arrTextMatrix[1] = 0;
		m_arrTextMatrix[2] = 0; m_arrTextMatrix[3] = 1;
		m_arrTextMatrix[4] = 0; m_arrTextMatrix[5] = 0;
		m_dCharSpace    = 0;
		m_dWordSpace    = 0;
		m_dHorizScaling = 1;
		m_dLeading      = 0;
		m_nRise         = 0;
		m_nRenderMode   = 0;

		m_pPath = new GrPath();
		m_dCurX = m_dCurY = 0;
		m_dTextLineX = m_dTextLineY = 0;

		m_pClip     = new GrClip(0, 0, m_dPageWidth, m_dPageHeight);
		m_dClipXMin = 0;
		m_dClipYMin = 0;
		m_dClipXMax = m_dPageWidth;
		m_dClipYMax = m_dPageHeight;

		m_pNext = NULL;
	}

	GrState::~GrState()
	{
		if (m_pFillColorSpace)
		{
			delete m_pFillColorSpace;
		}
		if (m_pStrokeColorSpace)
		{
			delete m_pStrokeColorSpace;
		}
		if (m_pFillPattern)
		{
			delete m_pFillPattern;
		}
		if (m_pStrokePattern)
		{
			delete m_pStrokePattern;
		}
		for (int nIndex = 0; nIndex < 4; ++nIndex)
		{
			if (m_ppTransfer[nIndex])
			{
				delete m_ppTransfer[nIndex];
			}
		}
		MemUtilsFree(m_pLineDash);
		if (m_pPath)
		{
			delete m_pPath;
		}
		if (m_pClip)
		{
			delete m_pClip;
		}
		if (m_pNext)
		{
			delete m_pNext;
		}
	}

	GrState::GrState(GrState *pState)
	{
		memcpy(this, pState, sizeof(GrState));

		if (m_pFillColorSpace)
		{
			m_pFillColorSpace = pState->m_pFillColorSpace->Copy();
		}
		if (m_pStrokeColorSpace)
		{
			m_pStrokeColorSpace = pState->m_pStrokeColorSpace->Copy();
		}
		if (m_pFillPattern)
		{
			m_pFillPattern = pState->m_pFillPattern->Copy();
		}
		if (m_pStrokePattern)
		{
			m_pStrokePattern = pState->m_pStrokePattern->Copy();
		}
		for (int nIndex = 0; nIndex < 4; ++nIndex)
		{
			if (m_ppTransfer[nIndex])
			{
				m_ppTransfer[nIndex] = pState->m_ppTransfer[nIndex]->Copy();
			}
		}
		if (m_nLineDashSize > 0)
		{
			m_pLineDash = (double *)MemUtilsMallocArray(m_nLineDashSize, sizeof(double));
			memcpy(m_pLineDash, pState->m_pLineDash, m_nLineDashSize * sizeof(double));
		}
		if (m_pClip)
		{
			m_pClip = pState->m_pClip->Copy();
		}
		m_pNext = NULL;
	}

	void GrState::SetPath(GrPath *pPath)
	{
		if (m_pPath)
			delete m_pPath;
		m_pPath = pPath;
	}

	void GrState::GetUserClipBBox(double *pdXMin, double *pdYMin, double *pdXMax, double *pdYMax)
	{
		double arrInvCTM[6];
		double dXMin, dYMin, dXMax, dYMax;

		double dDet_ = m_arrCTM[0] * m_arrCTM[3] - m_arrCTM[1] * m_arrCTM[2];
		if (fabs(dDet_) < FLT_EPSILON)
		{
			*pdXMin = 0;
			*pdYMin = 0;
			*pdXMax = 0;
			*pdYMax = 0;
			return;
		}

		// Обратная матрица для матрицы CTM
		double dDet = 1 / dDet_;
		arrInvCTM[0] =  m_arrCTM[3] * dDet;
		arrInvCTM[1] = -m_arrCTM[1] * dDet;
		arrInvCTM[2] = -m_arrCTM[2] * dDet;
		arrInvCTM[3] =  m_arrCTM[0] * dDet;
		arrInvCTM[4] = (m_arrCTM[2] * m_arrCTM[5] - m_arrCTM[3] * m_arrCTM[4]) * dDet;
		arrInvCTM[5] = (m_arrCTM[1] * m_arrCTM[4] - m_arrCTM[0] * m_arrCTM[5]) * dDet;

		dXMin = dXMax = m_dClipXMin * arrInvCTM[0] + m_dClipYMin * arrInvCTM[2] + arrInvCTM[4];
		dYMin = dYMax = m_dClipXMin * arrInvCTM[1] + m_dClipYMin * arrInvCTM[3] + arrInvCTM[5];
		double dTransX = m_dClipXMin * arrInvCTM[0] + m_dClipYMax * arrInvCTM[2] + arrInvCTM[4];
		double dTransY = m_dClipXMin * arrInvCTM[1] + m_dClipYMax * arrInvCTM[3] + arrInvCTM[5];
		if (dTransX < dXMin)
		{
			dXMin = dTransX;
		}
		else if (dTransX > dXMax)
		{
			dXMax = dTransX;
		}
		if (dTransY < dYMin)
		{
			dYMin = dTransY;
		}
		else if (dTransY > dYMax)
		{
			dYMax = dTransY;
		}
		dTransX = m_dClipXMax * arrInvCTM[0] + m_dClipYMin * arrInvCTM[2] + arrInvCTM[4];
		dTransY = m_dClipXMax * arrInvCTM[1] + m_dClipYMin * arrInvCTM[3] + arrInvCTM[5];
		if (dTransX < dXMin)
		{
			dXMin = dTransX;
		}
		else if (dTransX > dXMax)
		{
			dXMax = dTransX;
		}
		if (dTransY < dYMin)
		{
			dYMin = dTransY;
		}
		else if (dTransY > dYMax)
		{
			dYMax = dTransY;
		}
		dTransX = m_dClipXMax * arrInvCTM[0] + m_dClipYMax * arrInvCTM[2] + arrInvCTM[4];
		dTransY = m_dClipXMax * arrInvCTM[1] + m_dClipYMax * arrInvCTM[3] + arrInvCTM[5];
		if (dTransX < dXMin)
		{
			dXMin = dTransX;
		}
		else if (dTransX > dXMax)
		{
			dXMax = dTransX;
		}
		if (dTransY < dYMin)
		{
			dYMin = dTransY;
		}
		else if (dTransY > dYMax)
		{
			dYMax = dTransY;
		}

		*pdXMin = dXMin;
		*pdYMin = dYMin;
		*pdXMax = dXMax;
		*pdYMax = dYMax;
	}

	double GrState::TransformWidth(double dWidth)
	{
		double dX = m_arrCTM[0] + m_arrCTM[2];
		double dY = m_arrCTM[1] + m_arrCTM[3];
		return dWidth * sqrt(0.5 * (dX * dX + dY * dY));
	}

	double GrState::GetTransformedFontSize()
	{
		double dX1 = m_arrTextMatrix[2] * m_dFontSize;
		double dY1 = m_arrTextMatrix[3] * m_dFontSize;
		double dX2 = m_arrCTM[0] * dX1 + m_arrCTM[2] * dY1;
		double dY2 = m_arrCTM[1] * dX1 + m_arrCTM[3] * dY1;
		return sqrt(dX2 * dX2 + dY2 * dY2);
	}

	void GrState::GetFontTransformMatrix(double *pdM11, double *pdM12, double *pdM21, double *pdM22)
	{
		*pdM11 = (m_arrTextMatrix[0] * m_arrCTM[0] + m_arrTextMatrix[1] * m_arrCTM[2]) * m_dFontSize;
		*pdM12 = (m_arrTextMatrix[0] * m_arrCTM[1] + m_arrTextMatrix[1] * m_arrCTM[3]) * m_dFontSize;
		*pdM21 = (m_arrTextMatrix[2] * m_arrCTM[0] + m_arrTextMatrix[3] * m_arrCTM[2]) * m_dFontSize;
		*pdM22 = (m_arrTextMatrix[2] * m_arrCTM[1] + m_arrTextMatrix[3] * m_arrCTM[3]) * m_dFontSize;
	}

	void GrState::SetCTM(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		m_arrCTM[0] = dA;
		m_arrCTM[1] = dB;
		m_arrCTM[2] = dC;
		m_arrCTM[3] = dD;
		m_arrCTM[4] = dE;
		m_arrCTM[5] = dF;

		for (int nIndex = 0; nIndex < 6; ++nIndex)
		{
			if (m_arrCTM[nIndex] > 1e10)
			{
				m_arrCTM[nIndex] = 1e10;
			}
			else if (m_arrCTM[nIndex] < -1e10)
			{
				m_arrCTM[nIndex] = -1e10;
			}
		}
	}

	void GrState::ConcatCTM(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		double dOldA = m_arrCTM[0];
		double dOldB = m_arrCTM[1];
		double dOldC = m_arrCTM[2];
		double dOldD = m_arrCTM[3];

		m_arrCTM[0] = dA * dOldA + dB * dOldC;
		m_arrCTM[1] = dA * dOldB + dB * dOldD;
		m_arrCTM[2] = dC * dOldA + dD * dOldC;
		m_arrCTM[3] = dC * dOldB + dD * dOldD;
		m_arrCTM[4] = dE * dOldA + dF * dOldC + m_arrCTM[4];
		m_arrCTM[5] = dE * dOldB + dF * dOldD + m_arrCTM[5];

		for (int nIndex = 0; nIndex < 6; ++nIndex)
		{
			if (m_arrCTM[nIndex] > 1e10)
			{
				m_arrCTM[nIndex] = 1e10;
			}
			else if (m_arrCTM[nIndex] < -1e10)
			{
				m_arrCTM[nIndex] = -1e10;
			}
		}
	}

	void GrState::ShiftCTM(double dShiftX, double dShiftY)
	{
		m_arrCTM[4] += dShiftX;
		m_arrCTM[5] += dShiftY;
		m_dClipXMin += dShiftX;
		m_dClipYMin += dShiftY;
		m_dClipXMax += dShiftX;
		m_dClipYMax += dShiftY;
	}

	void GrState::SetFillColorSpace(GrColorSpace *pColorSpace)
	{
		if (m_pFillColorSpace)
		{
			delete m_pFillColorSpace;
		}
		m_pFillColorSpace = pColorSpace;
	}

	void GrState::SetStrokeColorSpace(GrColorSpace *pColorSpace)
	{
		if (m_pStrokeColorSpace)
		{
			delete m_pStrokeColorSpace;
		}
		m_pStrokeColorSpace = pColorSpace;
	}

	void GrState::SetFillPattern(GrPattern *pPattern)
	{
		if (m_pFillPattern)
		{
			delete m_pFillPattern;
		}
		m_pFillPattern = pPattern;
	}

	void GrState::SetStrokePattern(GrPattern *pPattern)
	{
		if (m_pStrokePattern)
		{
			delete m_pStrokePattern;
		}
		m_pStrokePattern = pPattern;
	}

	void GrState::SetTransfer(Function **ppFunctions)
	{
		for (int nIndex = 0; nIndex < 4; ++nIndex)
		{
			if (m_ppTransfer[nIndex])
			{
				delete m_ppTransfer[nIndex];
			}
			m_ppTransfer[nIndex] = ppFunctions[nIndex];
		}
	}

	void GrState::SetLineDash(double *pDash, int nSize, double dStart)
	{
		MemUtilsFree(m_pLineDash);
		m_pLineDash = pDash;
		m_nLineDashSize = nSize;
		m_dLineDashStart = dStart;
	}

	void GrState::ClearPath()
	{
		if (m_pPath)
			delete m_pPath;
		m_pPath = new GrPath();
	}

	void GrState::Clip()
	{
		double dXMin = 0, dYMin = 0, dXMax = 0, dYMax = 0, dX = 0, dY = 0;

		for (int nSubPathIndex = 0; nSubPathIndex < m_pPath->GetSubpathsCount(); ++nSubPathIndex)
		{
			GrSubpath *pSubpath = m_pPath->GetSubpath(nSubPathIndex);
			for (int nPointIndex = 0; nPointIndex < pSubpath->GetPointsCount(); ++nPointIndex)
			{
				Transform(pSubpath->GetX(nPointIndex), pSubpath->GetY(nPointIndex), &dX, &dY);
				if (nSubPathIndex == 0 && nPointIndex == 0)
				{
					dXMin = dXMax = dX;
					dYMin = dYMax = dY;
				}
				else
				{
					if (dX < dXMin)
					{
						dXMin = dX;
					}
					else if (dX > dXMax)
					{
						dXMax = dX;
					}
					if (dY < dYMin)
					{
						dYMin = dY;
					}
					else if (dY > dYMax)
					{
						dYMax = dY;
					}
				}
			}
		}
		if (dXMin > m_dClipXMin)
		{
			m_dClipXMin = dXMin;
		}
		if (dYMin > m_dClipYMin)
		{
			m_dClipYMin = dYMin;
		}
		if (dXMax < m_dClipXMax)
		{
			m_dClipXMax = dXMax;
		}
		if (dYMax < m_dClipYMax)
		{
			m_dClipYMax = dYMax;
		}
	}

	void GrState::ClipToStrokePath()
	{
		double dXMin = 0, dYMin = 0, dXMax = 0, dYMax = 0, dX = 0, dY = 0;

		for (int nSubPathIndex = 0; nSubPathIndex < m_pPath->GetSubpathsCount(); ++nSubPathIndex)
		{
			GrSubpath *pSubpath = m_pPath->GetSubpath(nSubPathIndex);
			for (int nPointIndex = 0; nPointIndex < pSubpath->GetPointsCount(); ++nPointIndex)
			{
				Transform(pSubpath->GetX(nPointIndex), pSubpath->GetY(nPointIndex), &dX, &dY);
				if (nSubPathIndex == 0 && nPointIndex == 0)
				{
					dXMin = dXMax = dX;
					dYMin = dYMax = dY;
				}
				else
				{
					if (dX < dXMin)
					{
						dXMin = dX;
					}
					else if (dX > dXMax)
					{
						dXMax = dX;
					}
					if (dY < dYMin)
					{
						dYMin = dY;
					}
					else if (dY > dYMax)
					{
						dYMax = dY;
					}
				}
			}
		}

		// Учитываем толщину линии
		double dT0 = fabs(m_arrCTM[0]);
		double dT1 = fabs(m_arrCTM[2]);
		if (dT0 > dT1)
		{
			dXMin -= 0.5 * m_dLineWidth * dT0;
			dXMax += 0.5 * m_dLineWidth * dT0;
		}
		else
		{
			dXMin -= 0.5 * m_dLineWidth * dT1;
			dXMax += 0.5 * m_dLineWidth * dT1;
		}
		// TO DO: Проверит здесь!!!! Сдается, что нужно m_arrCTM[1] поставить
		dT0 = fabs(m_arrCTM[0]);
		dT1 = fabs(m_arrCTM[3]);
		if (dT0 > dT1)
		{
			dYMin -= 0.5 * m_dLineWidth * dT0;
			dYMax += 0.5 * m_dLineWidth * dT0;
		}
		else
		{
			dYMin -= 0.5 * m_dLineWidth * dT1;
			dYMax += 0.5 * m_dLineWidth * dT1;
		}

		if (dXMin > m_dClipXMin)
		{
			m_dClipXMin = dXMin;
		}
		if (dYMin > m_dClipYMin)
		{
			m_dClipYMin = dYMin;
		}
		if (dXMax < m_dClipXMax)
		{
			m_dClipXMax = dXMax;
		}
		if (dYMax < m_dClipYMax)
		{
			m_dClipYMax = dYMax;
		}
	}

	void GrState::TextShift(double dShiftX, double dShiftY)
	{
		double dDx = 0, dDy = 0;

		TextTransformDelta(dShiftX, dShiftY, &dDx, &dDy);
		m_dCurX += dDx;
		m_dCurY += dDy;
	}

	void GrState::Shift(double dShiftX, double dShiftY)
	{
		m_dCurX += dShiftX;
		m_dCurY += dShiftY;
	}

	GrState *GrState::Save()
	{
		GrState *pNewState = Copy();
		pNewState->m_pNext = this;
		return pNewState;
	}

	GrState *GrState::Restore()
	{
		GrState *pOldState = NULL;

		if (m_pNext)
		{
			pOldState = m_pNext;

			// Следующие значения не сохраняются/восстанавливаются с помощью операций q/Q
			pOldState->m_pPath = m_pPath;
			pOldState->m_dCurX = m_dCurX;
			pOldState->m_dCurY = m_dCurY;
			pOldState->m_dTextLineX = m_dTextLineX;
			pOldState->m_dTextLineY = m_dTextLineY;

			m_pPath = NULL;
			m_pNext = NULL;
			delete this;

		}
		else
		{
			pOldState = this;
		}

		return pOldState;
	}

	bool GrState::ParseBlendMode(Object *pObject, GraphicsBlendMode *peMode)
	{
		if (pObject->IsName())
		{
			for (int nIndex = 0; nIndex < GrBlendModeNamesCount; ++nIndex)
			{
				if (!strcmp(pObject->GetName(), c_arrsGrBlendModeNames[nIndex].sName))
				{
					*peMode = c_arrsGrBlendModeNames[nIndex].eMode;
					return true;
				}
			}
			return false;
		}
		else if (pObject->IsArray())
		{
			for (int nIndex = 0; nIndex < pObject->ArrayGetLength(); ++nIndex)
			{
				Object oTemp;
				pObject->ArrayGet(nIndex, &oTemp);
				if (!oTemp.IsName())
				{
					oTemp.Free();
					return false;
				}
				for (int nJ = 0; nJ < GrBlendModeNamesCount; ++nJ)
				{
					if (!strcmp(oTemp.GetName(), c_arrsGrBlendModeNames[nJ].sName))
					{
						oTemp.Free();
						*peMode = c_arrsGrBlendModeNames[nJ].eMode;
						return true;
					}
				}
				oTemp.Free();
			}
			*peMode = grBlendNormal;
			return true;
		}
		else
		{
			return false;
		}
	}
}
