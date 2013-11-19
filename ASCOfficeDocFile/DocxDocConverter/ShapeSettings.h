#pragma once

#include <XmlUtils.h>

#include "Logic/ShapeStyle.h"
#include "Logic/Wrap.h"
#include "Logic/Inline.h"
#include "Logic/Shadow.h"
#include "Logic/LineStyle.h"
#include "Logic/FillStyle.h"

#include "Unit.h"

#include "DocxToDocUtils.h"
#include "OfficeArt/OfficeArtFOPT.h"
#include "OfficeArt/MSOArray.h"

#include "ShapePath.h"
#include "StyleBooleans.h"

using namespace OfficeArt;

namespace AVSDocFileFormat
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

		BOOL SetVisible(const std::string& strVisible);
		void SetUseBehind(bool bUse);
		void SetPositioning(const std::string& posh, const std::string& posrelh, const std::string& posv, const std::string& posrelv);
		void SetPercentageSettings(const std::string& strPctHoriz, const std::string& strPctVert, const std::string& strSizeRelh, const std::string& strSizeRelv);
		BOOL Append(OfficeArtRGFOPTE& oTable, BOOL bLayout = FALSE);

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

		BOOL Read ();

		BOOL SetPath(const std::string& strPath, const std::string& strAdj, const std::string& strFormulas);
		BOOL SetRightBottom(const std::string& strCoordSize);
		BOOL SetConnection(const std::string& strType);
		BOOL SetSites(const std::string& strSites);
		BOOL SetSitesDir(const std::string& strSitesDir);
		BOOL SetAdjustValues(const std::string& strAdjust, bool bAdjustArc = false);
		BOOL SetInscribe(const std::string& inscribe);

		BOOL Append(OfficeArtRGFOPTE& oTable);

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

		BOOL Read(const OOX::Logic::FillStyle& oXml);
		BOOL Append(OfficeArtRGFOPTE& oTable);
		BOOL Append2(OfficeArtRGFOPTE& oTable);
		BOOL Rebuild();

		BOOL SetType(int nType);
		BOOL SetBlip(int nBlip);

		BOOL SetUseShapeAnchor(bool value);

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

		BOOL Read (const OOX::Logic::LineStyle& oXml);
		BOOL Append (OfficeArtRGFOPTE& oTable);

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

		BOOL Read (const OOX::Logic::Shadow& oXml);
		BOOL Append (OfficeArtRGFOPTE& oTable, bool bFilled = true);

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

		BOOL SetRotate (const std::string& strRotate, bool bPicture = false);
		BOOL Append (OfficeArtRGFOPTE& oTable);

		inline BOOL IsAngleBoundFlip ()	{	return m_bAngleBoundFlip;	}

	private:

		BOOL			m_bAngleBoundFlip;
		unsigned int	m_nRotate;
	};

	class CTextStyle : public CStyle
	{
	public:
		CTextStyle ();

		BOOL SetAnchor (const std::string& textAnchor);

		BOOL Append (OfficeArtRGFOPTE& oTable);

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

		BOOL SetPib (int nPib);
		BOOL Append (OfficeArtRGFOPTE& oTable);

	private:

		int					m_nPib;					// index in BLIP Container
		BlipBooleans		m_oBlipBooleans;
	};
}

namespace AVSDocFileFormat
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

		virtual BOOL SetupSecondaryOptions();

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

		virtual BOOL SetupSecondaryOptions();

		virtual OfficeArt::OfficeArtFOPT GetPrimaryOptions();
		virtual OfficeArt::OfficeArtSecondaryFOPT GetSecondaryOptions();

	protected:

		CBlipStyle					m_oBlipStyle;

		LineStyleBooleans			m_oLineStyleBooleans;	
		FillStyleBooleans			m_oFillStyleBooleans;
	};
}