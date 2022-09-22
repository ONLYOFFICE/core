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
#ifndef _BUILD_GRAPHICS_STRUCTURES_H_
#define _BUILD_GRAPHICS_STRUCTURES_H_

#include "../common/Array.h"
#include "../graphics/aggplustypes.h"
#include "../agg-2.4/include/agg_color_rgba.h"
#include "shading_info.h"
#include <cmath>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>

// pen -----------------------------------------------------------
const long c_ag_LineCapFlat = 0;
const long c_ag_LineCapSquare = 1;
const long c_ag_LineCapTriangle = 3;
const long c_ag_LineCapNoAnchor = 16;
const long c_ag_LineCapSquareAnchor = 17;
const long c_ag_LineCapRoundAnchor = 18;
const long c_ag_LineCapDiamondAnchor = 19;
const long c_ag_LineCapArrowAnchor = 20;
const long c_ag_LineCapAnchorMask = 240;
const long c_ag_LineCapCustom = 255;

const long c_ag_DashCapFlat = 0;
const long c_ag_DashCapRound = 2;
const long c_ag_DashCapTriangle = 3;

const long c_ag_LineJoinMiter = 0;
const long c_ag_LineJoinBevel = 1;
const long c_ag_LineJoinRound = 2;
const long c_ag_LineJoinMiterClipped = 3;

const long c_ag_PenAlignmentCenter = 0;
const long c_ag_PenAlignmentInset = 1;
const long c_ag_PenAlignmentOutset = 2;
const long c_ag_PenAlignmentLeft = 3;
const long c_ag_PenAlignmentRight = 4;
// --------------------------------------------------------------
// brush --------------------------------------------------------
// old constants for brush type
const long c_BrushTypeSolid_ = 0;
const long c_BrushTypeHorizontal_ = 1;
const long c_BrushTypeVertical_ = 2;
const long c_BrushTypeDiagonal1_ = 3;
const long c_BrushTypeDiagonal2_ = 4;
const long c_BrushTypeCenter_ = 5;
const long c_BrushTypePathGradient1_ = 6;
const long c_BrushTypePathGradient2_ = 7;
const long c_BrushTypeTexture_ = 8;
const long c_BrushTypeHatch1_ = 9;
const long c_BrushTypeHatch53_ = 61;
const long c_BrushTypeGradient1_ = 62;
const long c_BrushTypeGradient6_ = 70;

const long c_BrushTypeSolid = 1000;
const long c_BrushTypeHorizontal = 2001;
const long c_BrushTypeVertical = 2002;
const long c_BrushTypeDiagonal1 = 2003;
const long c_BrushTypeDiagonal2 = 2004;
const long c_BrushTypeCenter = 2005;
const long c_BrushTypePathGradient1 = 2006; // left for comparability
const long c_BrushTypePathGradient2 = 2007; // left for comparability
const long c_BrushTypeCylinderHor = 2008;
const long c_BrushTypeCylinderVer = 2009;
const long c_BrushTypeTexture = 3008;
const long c_BrushTypePattern = 3009;
const long c_BrushTypeHatch1 = 4009;
const long c_BrushTypeHatch53 = 4061;
const long c_BrushTypeNoFill = 5000;
const long c_BrushTypeNotSet = 5001;

const long c_BrushTypeMyTestGradient = 6000;
const long c_BrushTypePathRadialGradient = 6001;
const long c_BrushTypePathConicalGradient = 6002;
const long c_BrushTypePathDiamondGradient = 6003;
const long c_BrushTypePathNewLinearGradient = 6004;
const long c_BrushTypeTriagnleMeshGradient = 6005;
const long c_BrushTypeCurveGradient     = 6006;
const long c_BrushTypeTensorCurveGradient = 6007;


const long c_BrushTextureModeStretch = 0;
const long c_BrushTextureModeTile = 1;
const long c_BrushTextureModeTileCenter = 2;
// --------------------------------------------------------------

namespace NSStructures
{


	class CPen
	{
	public:
		long Color;
		long Alpha;
		double Size;

		unsigned char DashStyle;
		unsigned char LineStartCap;
		unsigned char LineEndCap;
		unsigned char LineJoin;

		double *DashPattern;
		long Count;

		double DashOffset;

		long Align;
		double MiterLimit;

	public:
		void GetDashPattern(double *arrDashPattern, long &nCount) const
		{
			if (nCount == Count)
			{
				for (int i = 0; i < Count; ++i)
				{
					arrDashPattern[i] = DashPattern[i];
				}
			}
		}
		void SetDashPattern(double *arrDashPattern, long nCount)
		{
			if ((arrDashPattern == NULL) || (nCount == 0))
			{
				Count = 0;
				RELEASEARRAYOBJECTS(DashPattern);
			}
			else
			{
				if (Count != nCount)
				{
					Count = nCount;
					RELEASEARRAYOBJECTS(DashPattern);
					DashPattern = new double[Count];
				}

				for (int i = 0; i < Count; ++i)
				{
					DashPattern[i] = arrDashPattern[i];
				}
			}
		}

		void ScaleAlpha(double dScale)
		{
			long dNewAlpha = long(Alpha * dScale + 0.5);

			if (dNewAlpha > 255)
				dNewAlpha = 255;
			else if (dNewAlpha < 0)
				dNewAlpha = 0;

			Alpha = dNewAlpha;
		}

		INT IsEqual(CPen *pPen)
		{
			if (NULL == pPen)
				return FALSE;

			return ((Color == pPen->Color) && (Alpha == pPen->Alpha) && (Size == pPen->Size) &&
					(DashStyle == pPen->DashStyle) && (LineStartCap == pPen->LineStartCap) &&
					(LineEndCap == pPen->LineEndCap) && (LineJoin == pPen->LineJoin));
		}
		void SetDefaultParams()
		{
			Color = 0;
			Alpha = 255;
			Size = 1;

			DashStyle = 0;
			LineStartCap = 0;
			LineEndCap = 0;
			LineJoin = 0;

			DashPattern = NULL;
			Count = 0;

			DashOffset = 0;
			Align = Aggplus::PenAlignmentCenter;
			MiterLimit = 0.5;
		}

	public:
		CPen()
		{
			SetDefaultParams();
		}
		CPen(const CPen &other)
		{
			SetDefaultParams();
			*this = other;
		}
		CPen &operator=(const CPen &other)
		{
			Color = other.Color;
			Alpha = other.Alpha;
			Size = other.Size;

			DashStyle = other.DashStyle;
			LineStartCap = other.LineStartCap;
			LineEndCap = other.LineEndCap;
			LineJoin = other.LineJoin;

			RELEASEARRAYOBJECTS(DashPattern);
			Count = other.Count;
			if (Count != 0)
			{
				DashPattern = new double[Count];
				memcpy(DashPattern, other.DashPattern, Count * sizeof(double));
			}

			DashOffset = other.DashOffset;

			Align = other.Align;
			MiterLimit = other.MiterLimit;

			return *this;
		}
		virtual ~CPen()
		{
			RELEASEARRAYOBJECTS(DashPattern);
		}
	};

	class CBrush
	{
	public:
		int test;
		struct TSubColor
		{
			long color;
			long position; // [0..65536]
		};

	public:
		long Type;

		long Color1;
		long Color2;
		long Alpha1;
		long Alpha2;

		std::wstring TexturePath;
		long TextureAlpha;
		long TextureMode;

		int Rectable;
		Aggplus::RectF Rect;
		Aggplus::CDoubleRect Bounds;

		double LinearAngle;
		std::vector<TSubColor> m_arrSubColors;
		NSStructures::GradientInfo m_oGradientInfo;

	public:
		void LoadSubColors(const std::string &str)
		{
			m_arrSubColors.clear();

			if (str.empty())
				return;

			TSubColor subcolor;
			int start = 0;

			for (;;)
			{
				int pos = (int)str.find((wchar_t)',', start);
				if (pos < 0)
					break;

				subcolor.color = ::atoi(str.substr(start, pos - start).c_str());
				start = pos + 1;

				pos = (int)str.find((wchar_t)';', start);
				if (pos < 0)
					break;

				subcolor.position = ::atoi(str.substr(start, pos - start).c_str());
				start = pos + 1;

				m_arrSubColors.push_back(subcolor);
			}
		}

		inline long ConstantCompatible(long nConstant)
		{
			if (c_BrushTypeDiagonal1_ == nConstant)
				nConstant = c_BrushTypeDiagonal2_;
			else if (c_BrushTypeDiagonal2_ == nConstant)
				nConstant = c_BrushTypeDiagonal1_;

			if (1000 <= nConstant)
				return nConstant;
			if (c_BrushTypeSolid_ == nConstant)
				return nConstant + 1000;
			if (c_BrushTypeHorizontal_ <= nConstant && c_BrushTypePathGradient2_ >= nConstant)
				return nConstant + 2000;
			if (c_BrushTypeTexture_ == nConstant)
				return nConstant + 3000;
			if (c_BrushTypeHatch1_ <= nConstant && c_BrushTypeHatch53_ >= nConstant)
				return nConstant + 4000;
			if (c_BrushTypeGradient1_ <= nConstant && c_BrushTypeGradient6_ >= nConstant)
				return nConstant + 2000 - 61;

			return 1000;
		}

		void ScaleAlpha1(double dScale)
		{
			long dNewAlpha = long(Alpha1 * dScale + 0.5);

			if (dNewAlpha > 255)
				dNewAlpha = 255;
			else if (dNewAlpha < 0)
				dNewAlpha = 0;

			Alpha1 = dNewAlpha;
		}
		void ScaleAlpha2(double dScale)
		{
			long dNewAlpha = long(Alpha2 * dScale + 0.5);

			if (dNewAlpha > 255)
				dNewAlpha = 255;
			else if (dNewAlpha < 0)
				dNewAlpha = 0;

			Alpha2 = dNewAlpha;
		}

		void ScaleTextureAlpha(double dScale)
		{
			long dNewAlpha = long(TextureAlpha * dScale + 0.5);

			if (dNewAlpha > 255)
				dNewAlpha = 255;
			else if (dNewAlpha < 0)
				dNewAlpha = 0;

			TextureAlpha = dNewAlpha;
		}

        INT IsEqual(const CBrush *pBrush)
		{
			if (NULL == pBrush)
				return FALSE;

			return ((Type == pBrush->Type) &&
					(Color1 == pBrush->Color1) && (Color2 == pBrush->Color2) &&
					(Alpha1 == pBrush->Alpha1) && (Alpha2 == pBrush->Alpha2) && (LinearAngle == pBrush->LinearAngle) &&
					(TexturePath == pBrush->TexturePath) && (TextureAlpha == pBrush->TextureAlpha) && (TextureMode == pBrush->TextureMode) &&
                    (Rectable == pBrush->Rectable) && (Rect.Equals(pBrush->Rect)));
		}

		void SetDefaultParams()
		{
			Type = c_BrushTypeSolid;

			Color1 = 0;
			Alpha1 = 255;
			Color2 = 0;
			Alpha2 = 255;

			TextureAlpha = 255;
			TextureMode = c_BrushTextureModeStretch;

			LinearAngle = 0;

			TexturePath = L"";

			Rectable = FALSE;

			Rect.X = 0.0F;
			Rect.Y = 0.0F;
			Rect.Width = 0.0F;
			Rect.Height = 0.0F;

			Bounds.left = 0;
			Bounds.top = 0;
			Bounds.right = 0;
			Bounds.bottom = 0;

			m_arrSubColors.clear();
		}

	public:
		CBrush()
		{
			SetDefaultParams();
		}
		CBrush(const CBrush &other)
		{
			Type = other.Type;

			Color1 = other.Color1;
			Alpha1 = other.Alpha1;
			Color2 = other.Color2;
			Alpha2 = other.Alpha2;

			TexturePath = other.TexturePath;
			TextureAlpha = other.TextureAlpha;
			TextureMode = other.TextureMode;

			Rectable = other.Rectable;
			Rect = other.Rect;

			Bounds = other.Bounds;

			LinearAngle = other.LinearAngle;
			m_arrSubColors = other.m_arrSubColors;
            m_oGradientInfo = other.m_oGradientInfo;
		}
		CBrush &operator=(const CBrush &other)
		{
			Type = other.Type;

			Color1 = other.Color1;
			Alpha1 = other.Alpha1;
			Color2 = other.Color2;
			Alpha2 = other.Alpha2;

			TexturePath = other.TexturePath;
			TextureAlpha = other.TextureAlpha;
			TextureMode = other.TextureMode;

			Rectable = other.Rectable;
			Rect = other.Rect;
			Bounds = other.Bounds;

			LinearAngle = other.LinearAngle;
			m_arrSubColors = other.m_arrSubColors;
            m_oGradientInfo = other.m_oGradientInfo;

			return *this;
		}
		virtual ~CBrush()
		{
		}

		int IsTexture()
		{
			return (c_BrushTypeTexture == Type || c_BrushTypePattern == Type);
		}
		int IsOneColor()
		{
			return (c_BrushTypeSolid == Type);
		}
		int IsTwoColor()
		{
			return ((c_BrushTypeHorizontal <= Type && c_BrushTypeCylinderVer >= Type) ||
					(c_BrushTypeHatch1 <= Type && c_BrushTypeHatch53 >= Type));
		}
	};
	class CFont
	{
	public:
		std::wstring Path;
		std::wstring Name;
		double Size;
		int Bold;
		int Italic;
		unsigned char Underline;
		unsigned char Strikeout;

		int StringGID;
		double CharSpace;

		int FaceIndex;

        int IsEqual(const CFont *pFont)
		{
			if (NULL == pFont)
				return FALSE;

			return ((Name == pFont->Name) && (Path == pFont->Path) && (FaceIndex == pFont->FaceIndex) && (StringGID == pFont->StringGID) && (Size == pFont->Size) &&
					(Bold == pFont->Bold) && (Italic == pFont->Italic) &&
					(Underline == pFont->Underline) && (Strikeout == pFont->Strikeout));
		}

        int IsEqual2(const CFont *pFont)
		{
			if (NULL == pFont)
				return FALSE;

			if (Path != pFont->Path)
				return FALSE;

			if (Path.empty() && Name != pFont->Name)
				return FALSE;

			return ((FaceIndex == pFont->FaceIndex) && (StringGID == pFont->StringGID) && (Size == pFont->Size) &&
				(Bold == pFont->Bold) && (Italic == pFont->Italic) &&
				(Underline == pFont->Underline) && (Strikeout == pFont->Strikeout));
		}

		long GetStyle() const
		{
			long lStyle = 0;
			if (Bold)
				lStyle |= 0x01;
			if (Italic)
				lStyle |= 0x02;
			lStyle |= Underline << 2;
			lStyle |= Strikeout << 7;
			return lStyle;
		}
		long GetStyle2() const
		{
			long lStyle = 0;
			if (Bold)
				lStyle |= 0x01;
			if (Italic)
				lStyle |= 0x02;
			return lStyle;
		}
		void SetStyle(long const &lStyle)
		{
			Bold = (0x01 == (0x01 & lStyle));
			Italic = (0x02 == (0x02 & lStyle));
			Underline = (unsigned char)(0x7C & lStyle) >> 2;
			Strikeout = (unsigned char)(0x0180 & lStyle) >> 7;
		}
		void SetDefaultParams()
		{
			Name = L"Arial";
			Path = L"";

			Size = 0;
			Bold = FALSE;
			Italic = FALSE;
			Underline = 0;
			Strikeout = 0;

			StringGID = FALSE;
			CharSpace = 0.0;
			FaceIndex = 0;
		}
		LONG GetTextDecorationStyle()
		{
			if ((0 == Underline) && (0 == Strikeout))
				return 0;
			if ((1 == Underline) && (0 == Strikeout))
				return 1;
			if ((0 == Underline) && (1 == Strikeout))
				return 2;
			if ((1 == Underline) && (1 == Strikeout))
				return 3;
			return 4;
		}

        LONG GetTextFontStyle() const
        {
            if ((0 == Bold) && (0 == Italic))
                return 0;
            if ((0 == Bold) && (1 == Italic))
                return 1;
            if ((1 == Bold) && (0 == Italic))
                return 2;
            if ((1 == Bold) && (1 == Italic))
                return 3;
            return 4;
        }

		CFont()
		{
			SetDefaultParams();
		}
		CFont(const CFont &other)
		{
			*this = other;
		}
		CFont &operator=(const CFont &other)
		{
            Name = other.Name;
			Path = other.Path;
			Size = other.Size;
			Bold = other.Bold;
			Italic = other.Italic;
			Underline = other.Underline;
			Strikeout = other.Strikeout;

			StringGID = other.StringGID;
			CharSpace = other.CharSpace;

			FaceIndex = other.FaceIndex;

			return *this;
		}
		virtual ~CFont()
		{
		}
	};
	class CShadow
	{
	public:
		INT Visible;
		double DistanceX;
		double DistanceY;
		double BlurSize;
		long Color;
		long Alpha;

	public:
		void SetDefaultParams()
		{
			Visible = FALSE;
			DistanceX = 15;
			DistanceY = 15;
			BlurSize = 0;
			Color = 0;
			Alpha = 120;
		}

	public:
		CShadow()
		{
			SetDefaultParams();
		}
		CShadow(const CShadow &other)
		{
			Visible = other.Visible;
			DistanceX = other.DistanceX;
			DistanceY = other.DistanceY;
			BlurSize = other.BlurSize;
			Color = other.Color;
			Alpha = other.Alpha;
		}
		CShadow &operator=(const CShadow &other)
		{
			Visible = other.Visible;
			DistanceX = other.DistanceX;
			DistanceY = other.DistanceY;
			BlurSize = other.BlurSize;
			Color = other.Color;
			Alpha = other.Alpha;

			return *this;
		}
		virtual ~CShadow()
		{
		}
	};

	class CEdgeText
	{
	public:
		long Visible;
		double Dist;
		long Color;
		long Alpha;

	public:
		void SetDefaultParams()
		{
			Visible = 0;
			Dist = 5;
			Color = 0;
			Alpha = 255;
		}

	public:
		CEdgeText()
		{
			SetDefaultParams();
		}
		CEdgeText(const CEdgeText &other)
		{
			Visible = other.Visible;
			Dist = other.Dist;
			Color = other.Color;
			Alpha = other.Alpha;
		}
		CEdgeText &operator=(const CEdgeText &other)
		{
			Visible = other.Visible;
			Dist = other.Dist;
			Color = other.Color;
			Alpha = other.Alpha;

			return *this;
		}
		virtual ~CEdgeText()
		{
		}
	};

}

#endif // _BUILD_GRAPHICS_STRUCTURES_H_
