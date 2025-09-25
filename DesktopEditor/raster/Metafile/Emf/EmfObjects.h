/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
		CEmfObjectBase();
		virtual ~CEmfObjectBase();
		virtual EEmfObjectType GetType() const;
	};

	class CEmfLogBrushEx : public CEmfObjectBase, public IBrush
	{
	public:
		CEmfLogBrushEx();
		virtual ~CEmfLogBrushEx();
		virtual EEmfObjectType GetType() const override;
		
		void SetDibPattern(unsigned char* pBuffer, unsigned int ulWidth, unsigned int ulHeight);

		// IBrush
		int          GetColor()   const override;
		int          GetColor2()  const override;
		unsigned int GetStyle()   const override;
		unsigned int GetStyleEx() const override;
		unsigned int GetHatch()   const override;
		unsigned int GetAlpha()   const override;
		unsigned int GetAlpha2()  const override;
		std::wstring GetDibPatterPath() const override;
		void         GetBounds(double& left, double& top, double& width, double& height) const override;
		void         GetCenterPoint(double& dX, double& dY) const override;
		void         GetDibPattern(unsigned char** pBuffer, unsigned int &unWidth, unsigned int &unHeight) const override;

		void GetGradientColors(std::vector<long>& arColors, std::vector<double>& arPositions) const override;
	public:
		unsigned int   unBrushStyle;
		TRGBA          oColor;
		unsigned int   unBrushHatch;
		std::wstring   wsDibPatternPath;
		unsigned char* pDibBuffer;
		unsigned int   unDibWidth;
		unsigned int   unDibHeigth;
	};

	class CEmfLogFont : public CEmfObjectBase, public IFont
	{
	public:
		CEmfLogFont(bool bFixedLength = false);
		virtual ~CEmfLogFont();
		virtual EEmfObjectType GetType() const override;

		bool         IsFixedLength()      const;

		// IFont
		double       GetHeight()      const override;
		int          GetWeight()      const override;
		bool         IsItalic()       const override;
		bool         IsStrikeOut()    const override;
		bool         IsUnderline()    const override;
		int          GetEscapement()  const override;
		int          GetCharSet()     const override;
		int          GetOrientation() const override;
		std::wstring GetFaceName()    const override;
	public:
		TEmfLogFontEx    oLogFontEx;
		TEmfDesignVector oDesignVector;

	private:
		bool m_bFixedLength;
	};

	class CEmfLogPen : public CEmfObjectBase, public IPen
	{
	public:
		CEmfLogPen();
		virtual ~CEmfLogPen();
		virtual EEmfObjectType GetType() const override;

		// IPen
		int             GetColor()        const override;
		unsigned int    GetStyle()        const override;
		double          GetWidth()        const override;
		unsigned int    GetAlpha()        const override;
		double          GetMiterLimit()   const override;
		double          GetDashOffset()   const override;
		void            GetDashData(double*& arDatas, unsigned int& unSize) const override;
		const ILineCap* GetStartLineCap() const override;
		const ILineCap* GetEndLineCap()   const override;
	public:
		unsigned int  unPenStyle;
		unsigned int  unWidth;
		TRGBA         oColor;
		unsigned int  unNumStyleEntries;
		unsigned int* pStyleEntry;
	};

	class CEmfLogPalette : public CEmfObjectBase
	{
	public:
		CEmfLogPalette();
		virtual ~CEmfLogPalette();
		virtual EEmfObjectType GetType() const override;
	public:
		unsigned short       ushNumberOfEntries;
		TEmfLogPaletteEntry* pPaletteEntries;
	};
}

#endif // _METAFILE_EMF_EMFOBJECTS_H
