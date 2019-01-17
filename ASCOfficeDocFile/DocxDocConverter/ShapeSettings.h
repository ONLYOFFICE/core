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
#pragma once

#include "../../Common/DocxFormat/Source/DocxFormat/Drawing/DrawingShape.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Drawing/DrawingStyles.h"
//#include "../../Common/DocxFormat/Source/DocxFormat/Logic/Wrap.h"
//#include "../../Common/DocxFormat/Source/DocxFormat/Logic/Inline.h"
//#include "../../Common/DocxFormat/Source/DocxFormat/Logic/Shadow.h"
//#include "../../Common/DocxFormat/Source/DocxFormat/Logic/LineStyle.h"
//#include "../../Common/DocxFormat/Source/DocxFormat/Logic/FillStyle.h"

#include "../../Common/DocxFormat/Source/Utility/Unit.h"

#include "DocxToDocUtils.h"
#include "OfficeArt/OfficeArtFOPT.h"
#include "OfficeArt/MSOArray.h"

#include "ShapePath.h"
#include "StyleBooleans.h"

#include "../../Common/DocxFormat/Source/XML/xmlutils.h"

using namespace OfficeArt;

namespace ASCDocFileFormat
{
	class CStyle
	{
	public:

		CStyle();

	protected:

		void AppendEntry(int nID, int nValue, bool bBid = false, bool bComplex = false);
		void AppendEntry2(int nID, int nValue, bool bBid = false, bool bComplex = false);
		template<class T> void AppenComplexEntry(int nID, bool bBid, T& oArray);

		std::vector<OfficeArt::ComplexOfficeArtProperty>	m_arTable;
		std::vector<OfficeArt::ComplexOfficeArtProperty>	m_arTable2;
	};

	class CGroupShapeStyle : public CStyle
	{
	public:
		CGroupShapeStyle();

		bool SetVisible(const std::string& strVisible);
		void SetUseBehind(bool bUse);
		void SetPositioning(const std::string& posh, const std::string& posrelh, const std::string& posv, const std::string& posrelv);
		void SetPercentageSettings(const std::string& strPctHoriz, const std::string& strPctVert, const std::string& strSizeRelh, const std::string& strSizeRelv);
		bool Append(OfficeArtRGFOPTE& oTable, bool bLayout = FALSE);

	private:

		int					m_posh;
		int					m_posrelh;
		int					m_posv;
		int					m_posrelv;

		GroupShapeBooleans	m_groupShapeBooleanProperties;

		// group shape 2

		unsigned int		m_pctHoriz;
		unsigned int		m_pctVert;
		unsigned int		m_sizerelh;
		unsigned int		m_sizerelv;
	};

	class CGeometryStyle : public CStyle
	{
	public:
		CGeometryStyle();

		bool Read ();

		bool SetPath(const std::string& strPath, const std::string& strAdj, const std::string& strFormulas);
		bool SetRightBottom(const std::string& strCoordSize);
		bool SetConnection(const std::string& strType);
		bool SetSites(const std::string& strSites);
		bool SetSitesDir(const std::string& strSitesDir);
		bool SetAdjustValues(const std::string& strAdjust, bool bAdjustArc = false);
		bool SetInscribe(const std::string& inscribe);

		bool Append(OfficeArtRGFOPTE& oTable);

	private:

		int							m_nGeoLeft;
		int							m_nGeoTop;
		int							m_nGeoRight;
		int							m_nGeoBottom;

		CShapePath					m_oShapePath;

		bool						m_bAdjustArc;
		std::vector<int>			m_arAdjustValues;

		CMSOArray<CPoint32>			m_oConnectionSites;
		CMSOArray<unsigned int>		m_oConnectionSitesDir;
		CMSOArray<ADJH>				m_oAdjustHandles;
		CMSOArray<SG>				m_oGuides;
		CMSOArray<OfficeArt::RECT>	m_pInscribe;

		int							m_nCxk;

		GeometryBooleans			m_oGeometryBooleans;
	};

	class CFillStyle : public CStyle
	{
	public:

		CFillStyle();

		bool Read(const OOX::Logic::FillStyle& oXml);
		bool Append(OfficeArtRGFOPTE& oTable);
		bool Append2(OfficeArtRGFOPTE& oTable);
		bool Rebuild();

		bool SetType(int nType);
		bool SetBlip(int nBlip);

		bool SetUseShapeAnchor(bool value);

		inline const FillStyleBooleans& GetFill() {return m_oFillBooleans;}

	private:

		bool				m_bOK;

		int					m_nType;
		int					m_nColor;
		int					m_nOpacity;
		int					m_nBlip;

		FillStyleBooleans	m_oFillBooleans;
	};

	class CLineStyle : public CStyle
	{
	public:

		CLineStyle();

		bool Read (const OOX::Logic::LineStyle& oXml);
		bool Append (OfficeArtRGFOPTE& oTable);

		// READ

		int GetLineStyle (const std::string& lineType);
		int GetLineDash (const std::string& dashType);
		int GetEndCapStyle (const std::string& capStyle);
		int GetLineArrowHead (const std::string& strStyle);
		int GetLineArrowWidth (const std::string& strStyle);
		int GetLineArrowLength (const std::string& strStyle);

	private:

		int					m_nColor;
		int					m_nOpacity;
		int					m_nType;
		int					m_nStyle;
		int					m_nWidth;
		int					m_nDashing;
		int					m_nStartArrow;
		int					m_nEndArrow;
		int					m_nStartArrowWidth;
		int					m_nStartArrowLength;
		int					m_nEndArrowWidth;
		int					m_nEndArrowLength;
		int					m_nEndCapStyle;

		LineStyleBooleans	m_oLineBooleans;
	};

	class CShadowStyle : public CStyle
	{
	public:

		CShadowStyle();

		bool Read (const OOX::Logic::Shadow& oXml);
		bool Append (OfficeArtRGFOPTE& oTable, bool bFilled = true);

	private:

		bool				m_bOK;

		int					m_nType;
		int					m_nColor;
		int					m_nOpacity;
		DOCX::CPointF		m_oOffset;
		DOCX::CMatrixF		m_oTransform;
		DOCX::CPointF		m_oOrigin;

		ShadowBooleans		m_oShadowBooleans;
	};

	class CTransformStyle : public CStyle
	{
	public:
		CTransformStyle ();

		bool SetRotate (const std::string& strRotate, bool bPicture = false);
		bool Append (OfficeArtRGFOPTE& oTable);

		inline bool IsAngleBoundFlip ()	{	return m_bAngleBoundFlip;	}

	private:

		bool			m_bAngleBoundFlip;
		unsigned int	m_nRotate;
	};

	class CTextStyle : public CStyle
	{
	public:
		CTextStyle ();

		bool SetAnchor (const std::string& textAnchor);

		bool Append (OfficeArtRGFOPTE& oTable);

	private:

		int					m_nLTxID;

		int					m_nDXTextLeft;
		int					m_nDXTextTop;
		int					m_nDXTextRight;
		int					m_nDXTextBottom;
		int					m_nWrapMode;
		int					m_nAnchorText;


		TextBooleans		m_oTextBooleans;
	};

	class CBlipStyle : public CStyle
	{
	public:
		CBlipStyle();

		bool SetPib (int nPib);
		bool Append (OfficeArtRGFOPTE& oTable);

	private:

		int					m_nPib;					// index in BLIP Container
		BlipBooleans		m_oBlipBooleans;
	};
}

namespace ASCDocFileFormat
{
	class CShapeSettings
	{
	public:

		CShapeSettings();

		inline CGroupShapeStyle&	GetGroupShapeStyle()	{	return m_oGroupShapeStyle;	}	
		inline CGeometryStyle&		GetGeometryStyle()		{	return m_oGeometry;			}
		inline CFillStyle&			GetFillStyle()			{	return m_oFill;				}
		inline CLineStyle&			GetLineStyle()			{	return m_oLine;				}
		inline CShadowStyle&		GetShadowStyle()		{	return m_oShadow;			}
		inline CTransformStyle&		GetTransformStyle()		{	return m_oTransform;		}
		inline CTextStyle&			GetTextStyle()			{	return m_oText;				}


		// TODO : deprecated в классы свойств перенести

		void SetUseBehind	(bool bUse);
		void SetUseBehind	(const OOX::Logic::ShapeStyle& refStyle);
		void SetWrapDist	(const OOX::Logic::ShapeStyle& refStyle);

		virtual bool SetupSecondaryOptions();

		virtual OfficeArt::OfficeArtFOPT GetPrimaryOptions();
		virtual OfficeArt::OfficeArtSecondaryFOPT GetSecondaryOptions();

	protected:

		void AddEntry (int nID, int nValue, bool bBid = false, bool bComplex = false);
		void AddOptionalEntry (OfficeArt::OfficeArtRGFOPTE& oTable, int nID, int nValue, bool bBid = false, bool bComplex = false);

	protected:

		OfficeArt::OfficeArtRGFOPTE			m_oPrimary;
		OfficeArt::OfficeArtRGFOPTE			m_oSecondary;

		CGroupShapeStyle					m_oGroupShapeStyle;
		CGeometryStyle						m_oGeometry;
		CFillStyle							m_oFill;
		CLineStyle							m_oLine;
		CTransformStyle						m_oTransform;
		CShadowStyle						m_oShadow;
		CTextStyle							m_oText;

		int									m_nDXWrapDistLeft;
		int									m_nDYWrapDistTop;
		int									m_nDXWrapDistRight;
		int									m_nDYWrapDistBottom;
	};

	class CImageSettings : public CShapeSettings
	{
	public:

		CImageSettings();

		inline CBlipStyle& GetBlipStyle() {return m_oBlipStyle;}

		//

		void SetWrapDist (const OOX::Logic::Inline& oInline);

		virtual bool SetupSecondaryOptions();

		virtual OfficeArt::OfficeArtFOPT GetPrimaryOptions();
		virtual OfficeArt::OfficeArtSecondaryFOPT GetSecondaryOptions();

	protected:

		CBlipStyle					m_oBlipStyle;

		LineStyleBooleans			m_oLineStyleBooleans;	
		FillStyleBooleans			m_oFillStyleBooleans;
	};
}
