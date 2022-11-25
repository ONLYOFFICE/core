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
#ifndef _METAFILE_EMF_EMFOBJECTS_H
#define _METAFILE_EMF_EMFOBJECTS_H

#include "EmfTypes.h"
//#include "../../common/Types.h"
#include "../Common/MetaFileObjects.h"
#include "../../../common/File.h"

namespace MetaFile
{
	typedef enum
	{
		EMF_OBJECT_UNKNOWN = 0x00,
		EMF_OBJECT_BRUSH   = 0x01,
		EMF_OBJECT_FONT    = 0x02,
		EMF_OBJECT_PEN     = 0x03,
		EMF_OBJECT_PALETTE = 0x04
	} EEmfObjectType;

	class CEmfObjectBase
	{
	public:
		CEmfObjectBase(){}
		virtual ~CEmfObjectBase(){}
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_UNKNOWN;
		}
	};

	class CEmfLogBrushEx : public CEmfObjectBase, public IBrush
	{
	public:

		CEmfLogBrushEx();
		virtual ~CEmfLogBrushEx();
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_BRUSH;
		}
		void SetDibPattern(unsigned char* pBuffer, unsigned int ulWidth, unsigned int ulHeight);

		// IBrush
		int          GetColor();
		int          GetColor2()
		{
			return 0;
		}
		unsigned int GetStyle()
		{
			return BrushStyle;
		}
		unsigned int GetStyleEx()
		{
			return 0;
		}
		unsigned int GetHatch()
		{
			return BrushHatch;
		}
		unsigned int GetAlpha()
		{
			return BrushAlpha;
		}
		unsigned int GetAlpha2()
		{
			return 0Xff;
		}
		std::wstring GetDibPatterPath()
		{
			return DibPatternPath;
		}
		void GetBounds(double& left, double& top, double& width, double& height) {}

		void GetCenterPoint(double& dX, double& dY){}

		void GetDibPattern(unsigned char** pBuffer, unsigned int &unWidth, unsigned int &unHeight)
		{
			*pBuffer	= DibBuffer;
			unWidth		= DibWidth;
			unHeight	= DibHeigth;
		}

	public:
		unsigned int   BrushStyle;
		TEmfColor      Color;
		unsigned int   BrushHatch;
		unsigned int   BrushAlpha;
		std::wstring   DibPatternPath;
		unsigned char* DibBuffer;
		unsigned int   DibWidth;
		unsigned int   DibHeigth;
	};

	class CEmfLogFont : public CEmfObjectBase, public IFont
	{
	public:
		CEmfLogFont(bool bFixedLength = false)
		{
			DesignVector.Values = NULL;
			m_bFixedLength = bFixedLength;
		}
		virtual ~CEmfLogFont()
		{
			if (DesignVector.Values)
				delete[] DesignVector.Values;
		}
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_FONT;
		}

		// IFont
		double GetHeight()
		{
			return LogFontEx.LogFont.Height;
		}
		std::wstring GetFaceName()
		{
			return std::wstring(NSFile::CUtf8Converter::GetWStringFromUTF16(LogFontEx.LogFont.FaceName, 32).c_str());
		}
		int GetWeight()
		{
			return LogFontEx.LogFont.Weight;
		}
		bool IsItalic()
		{
			return (0x01 == LogFontEx.LogFont.Italic ? true : false);
		}
		bool IsStrikeOut()
		{
			return (0x01 == LogFontEx.LogFont.StrikeOut ? true : false);
		}
		bool IsUnderline()
		{
			return (0x01 == LogFontEx.LogFont.Underline ? true : false);
		}
		int GetEscapement()
		{
			return LogFontEx.LogFont.Escapement;
		}
		int GetCharSet()
		{
			return LogFontEx.LogFont.CharSet;
		}
		bool IsFixedLength()
		{
		    return m_bFixedLength;
		}
		int GetOrientation()
		{
			return LogFontEx.LogFont.Orientation;
		}

	public:

		TEmfLogFontEx    LogFontEx;
		TEmfDesignVector DesignVector;

	private:

		bool m_bFixedLength;
	};

	class CEmfLogPen : public CEmfObjectBase, public IPen
	{
	public:
		CEmfLogPen() : PenStyle(PS_SOLID), Width(1), Color(0, 0, 0)
		{
			StyleEntry = NULL;
		}
		virtual ~CEmfLogPen()
		{
			if (StyleEntry)
				delete[] StyleEntry;
		}
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_PEN;
		}

		// IPen
		int          GetColor();
		unsigned int GetStyle()
		{
			return PenStyle;
		}
		double GetWidth()
		{
			return (double)Width;
		}
		unsigned int GetAlpha()
		{
			return 255;
		}
		double GetMiterLimit()
		{
			return 0;
		}
		double GetDashOffset()
		{
			return 0;
		}
		void GetDashData(double*& arDatas, unsigned int& unSize)
		{
			arDatas = NULL;
			unSize  = 0;
		}

	public:

		unsigned int  PenStyle;
		unsigned int  Width;
		TEmfColor     Color;
		unsigned int  NumStyleEntries;
		unsigned int* StyleEntry;
	};

	class CEmfLogPalette : public CEmfObjectBase
	{
	public:
		CEmfLogPalette()
		{
			NumberOfEntries = 0;
			PaletteEntries  = NULL;
		}
		virtual ~CEmfLogPalette()
		{
			if (PaletteEntries)
				delete[] PaletteEntries;
		}
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_PALETTE;
		}

	public:

		unsigned short       NumberOfEntries;
		TEmfLogPaletteEntry* PaletteEntries;
	};
}

#endif // _METAFILE_EMF_EMFOBJECTS_H
