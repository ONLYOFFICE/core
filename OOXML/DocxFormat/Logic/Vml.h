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

#include "../../Base/Nullable.h"

#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/SimpleTypes_Vml.h"
#include "../../XlsxFormat/Controls/Controls.h"

#include "VmlWord.h"
#include "Shape.h"

#include "../WritingElement.h"
#include "../RId.h"

#include "VmlOfficeDrawing.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CCellAnchor;
	}
	namespace Vml
	{
		class CVmlCommonElements : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CVmlCommonElements)

			CVmlCommonElements(OOX::Document *pMain = NULL);
			virtual ~CVmlCommonElements();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			virtual void mergeFrom(CVmlCommonElements* parent);

			void CreateElement(XmlUtils::CXmlLiteReader& oReader);
			
	// 1 AG_AllCoreAttributes
	// 1.1 AG_CoreAttributes
			nullable_string								m_sId;
			nullable<SimpleTypes::Vml::CCssStyle>		m_oStyle;
			nullable_string								m_sHref;
			nullable_string								m_sTarget;
			nullable_string								m_sClass;
			nullable_string								m_sTitle;
			nullable_string								m_sAlt;
			nullable<SimpleTypes::Vml::CVml_Vector2D>	m_oCoordSize;
			nullable<SimpleTypes::Vml::CVml_Vector2D>	m_oCoordOrigin;
			nullable<SimpleTypes::Vml::CVml_Polygon2D>	m_oWrapCoords;
			nullable_bool								m_oPrint;
	// 1.2 AG_OfficeCoreAttributes
			nullable_string								m_sSpId;
			nullable_bool								m_oOned;
			nullable_int								m_oRegroupId;
			nullable_bool								m_oDoubleClickNotify;
			nullable_bool								m_oButton;
			nullable_bool								m_oUserHidden;
			nullable_bool								m_oBullet;
			nullable_bool								m_oHr;
			nullable_bool								m_oHrStd;
			nullable_bool								m_oHrNoShade;
			nullable_double								m_oHrPct;
			nullable<SimpleTypes::CHrAlign>				m_oHrAlign;
			nullable_bool								m_oAllowInCell;
			nullable_bool								m_oAllowOverlap;
			nullable_bool								m_oUserDrawn;
			nullable<SimpleTypes::CColorType>			m_oBorderTopColor;
			nullable<SimpleTypes::CColorType>			m_oBorderLeftColor;
			nullable<SimpleTypes::CColorType>			m_oBorderBottomColor;
			nullable<SimpleTypes::CColorType>			m_oBorderRightColor;
			nullable<SimpleTypes::CDiagramLayout>		m_oDgmLayout;
			nullable_string								m_oDgmNodeKind;
			nullable<SimpleTypes::CDiagramLayout>		m_oDgmLayoutMru;
			nullable<SimpleTypes::CInsetMode>			m_oInsetMode;
	// 2 AG_AllShapeAttributes
	// 2.1 AG_ShapeAttributes
			nullable<SimpleTypes::CColorType>			m_oChromaKey;
			nullable<SimpleTypes::CTrueFalse>			m_oFilled;
			nullable<SimpleTypes::CColorType>			m_oFillColor;
			nullable<SimpleTypes::Vml::CVml_1_65536>	m_oOpacity;
			nullable<SimpleTypes::CTrueFalse>			m_oStroked;
			nullable<SimpleTypes::CColorType>			m_oStrokeColor;
			nullable<SimpleTypes::CEmu>					m_oStrokeWeight;
			nullable<SimpleTypes::CTrueFalse>			m_oInsetPen;
	// 2.2 AG_OfficeShapeAttributes
			nullable<SimpleTypes::CDecimalNumber>		m_oSpt;
			nullable<SimpleTypes::CConnectorType>		m_oConnectorType;
			nullable<SimpleTypes::CBWMode>				m_oBwMode;
			nullable<SimpleTypes::CBWMode>				m_oBwPure;
			nullable<SimpleTypes::CBWMode>				m_oBwNormal;
			nullable_bool								m_oForceDash;
			nullable_bool								m_oOleIcon;
			nullable_bool								m_oOle;
			nullable_bool								m_oPreferRelative;
			nullable_bool								m_oClipToWrap;
			nullable_bool								m_oClip;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void  ReadElements(XmlUtils::CXmlLiteReader& oReader) ;

			std::wstring WriteAttributes() const;
			std::wstring WriteElements() const;

//for internal
            bool m_bComment = false;
			bool m_bImage = false;
		};

		//--------------------------------------------------------------------------------
		// CArc 14.1.2.1 (Part4)
		//--------------------------------------------------------------------------------	
		class CArc : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CArc)

			CArc(OOX::Document *pMain = NULL);
			virtual ~CArc();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			SimpleTypes::CDecimalNumber m_oEndAngle = 90;
			SimpleTypes::CDecimalNumber m_oStartAngle;
		};

		//--------------------------------------------------------------------------------
		// CCurve 14.1.2.3 (Part4)
		//--------------------------------------------------------------------------------	
		class CCurve : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CCurve)

			CCurve(OOX::Document *pMain = NULL);
			virtual ~CCurve();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			SimpleTypes::Vml::CVml_Vector2D_Units m_oFrom;
			SimpleTypes::Vml::CVml_Vector2D_Units m_oControl1;
			SimpleTypes::Vml::CVml_Vector2D_Units m_oControl2;
			SimpleTypes::Vml::CVml_Vector2D_Units m_oTo;
		};

		//--------------------------------------------------------------------------------
		// CF 14.1.2.4 (Part4)
		//--------------------------------------------------------------------------------	
		class CF : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CF)

			CF();
			virtual ~CF();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			std::wstring m_sEqn;
		};

		//--------------------------------------------------------------------------------
		// CFill 14.1.2.5 (Part4)
		//--------------------------------------------------------------------------------	
		class CFill : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFill)

			CFill(OOX::Document *pMain = NULL);
			virtual ~CFill();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			typedef struct TIntermediateColor
			{
				double						dValue;
				SimpleTypes::CColorType		oColor;
			}TIntermediateColor;

			nullable<SimpleTypes::CTrueFalse>									m_oAlignShape;
			nullable_string														m_sAltHref;
			nullable<SimpleTypes::CDecimalNumber>								m_oAngle;
			nullable<SimpleTypes::CImageAspect>									m_oAspect;
			nullable<SimpleTypes::CColorType>									m_oColor;
			nullable<SimpleTypes::CColorType>									m_oColor2;
			std::vector<TIntermediateColor*>									m_arrColors;
			nullable<SimpleTypes::CTrueFalse>									m_oDetectMouseClick;
			nullable<SimpleTypes::CFixedPercentage >							m_oFocus;
			nullable<SimpleTypes::Vml::CVml_Vector2D_Percentage>				m_oFocusPosition;
			nullable<SimpleTypes::Vml::CVml_Vector2D_Percentage>				m_oFocusSize;
			nullable_string														m_sHref;
			nullable<SimpleTypes::CRelationshipId>								m_rId;
			nullable_string														m_sId;
			nullable<SimpleTypes::CFillMethod>									m_oMethod;
			nullable<SimpleTypes::CTrueFalse>									m_oOn;
			nullable<SimpleTypes::Vml::CVml_1_65536>							m_oOpacity;
			nullable<SimpleTypes::Vml::CVml_1_65536>							m_oOpacity2;
			nullable<SimpleTypes::Vml::CVml_Vector2D_1_65536>					m_oOrigin;
			nullable<SimpleTypes::Vml::CVml_Vector2D_1_65536>					m_oPosition;
			nullable<SimpleTypes::CTrueFalse>									m_oRecolor;
			nullable<SimpleTypes::CRelationshipId>								m_oRelId;
			nullable<SimpleTypes::CTrueFalse>									m_oRotate;
			nullable<SimpleTypes::Vml::CVml_Vector2D_Units>						m_oSize;
			nullable_string														m_sSrc;
			nullable_string														m_sTitle;
			SimpleTypes::CFillType												m_oType;

			nullable<OOX::VmlOffice::CFill>										m_oFill;

		};

		//--------------------------------------------------------------------------------
		// CBackground 14.1.2.2 (Part4)
		//--------------------------------------------------------------------------------	
		class CBackground : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CBackground)

			CBackground(OOX::Document *pMain = NULL);
			virtual ~CBackground();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			nullable<SimpleTypes::CScreenSize> m_oTargetScreenSize;
		};

		//--------------------------------------------------------------------------------
		// CFormulas 14.1.2.6 (Part4)
		//--------------------------------------------------------------------------------	
		class CFormulas : public WritingElementWithChilds<OOX::Vml::CF>
		{
		public:
			WritingElement_AdditionConstructors(CFormulas)

			CFormulas(OOX::Document *pMain = NULL);
			virtual ~CFormulas();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
			// Childs
		};

		//--------------------------------------------------------------------------------
		// CH 14.1.2.8 (Part4)
		//--------------------------------------------------------------------------------	
		class CH : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CH)

			CH();
			virtual ~CH();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			SimpleTypes::CTrueFalse                                   m_oInvX;
			SimpleTypes::CTrueFalse		                              m_oInvY;
			SimpleTypes::Vml::CVml_Vector2D                           m_oMap;
			nullable<SimpleTypes::Vml::CVml_Vector2D>                 m_oPolar;
			SimpleTypes::Vml::CVml_Vector2D_Position                  m_oPosition;
			SimpleTypes::Vml::CVml_Vector2D                           m_oRadiusRange;
			SimpleTypes::CTrueFalse                                   m_oSwitch;
			SimpleTypes::Vml::CVml_Vector2D                           m_oXRange;
			SimpleTypes::Vml::CVml_Vector2D                           m_oYRange;
		};

		//--------------------------------------------------------------------------------
		// CHandles 14.1.2.9 (Part4)
		//--------------------------------------------------------------------------------	
		class CHandles : public WritingElementWithChilds<OOX::Vml::CH>
		{
		public:
			WritingElement_AdditionConstructors(CHandles)

			CHandles(OOX::Document *pMain = NULL);
			virtual ~CHandles();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
			// Childs
		};

		//--------------------------------------------------------------------------------
		// CImage 14.1.2.10 (Part4)
		//--------------------------------------------------------------------------------	
		class CImage : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CImage)

			CImage(OOX::Document *pMain = NULL);
			virtual ~CImage();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string								m_pSrc;
			nullable<SimpleTypes::Vml::CVml_1_65536>	m_pCropLeft;
			nullable<SimpleTypes::Vml::CVml_1_65536>	m_pCropTop;
			nullable<SimpleTypes::Vml::CVml_1_65536>	m_pCropRight;
			nullable<SimpleTypes::Vml::CVml_1_65536>	m_pCropBottom;
			nullable_double								m_pGain;
			nullable_double								m_pBlackLevel;
			nullable_double								m_pGamma;
			nullable<SimpleTypes::CTrueFalse>			m_pGrayscale;
			nullable<SimpleTypes::CTrueFalse>			m_pBiLevel;
		};

		//--------------------------------------------------------------------------------
		// CImageData 14.1.2.11 (Part4)
		//--------------------------------------------------------------------------------	
		class CImageData : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CImageData)

			CImageData(OOX::Document *pMain = NULL);
			virtual ~CImageData();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string										m_sAltHref;
			nullable<SimpleTypes::CTrueFalse>					m_oBiLevel;
			nullable_double										m_oBlackLevel;
			nullable<SimpleTypes::CColorType>					m_oChromaKey;
			nullable<SimpleTypes::Vml::CVml_1_65536>			m_oCropLeft;
			nullable<SimpleTypes::Vml::CVml_1_65536>			m_oCropTop;
			nullable<SimpleTypes::Vml::CVml_1_65536>			m_oCropRight;
			nullable<SimpleTypes::Vml::CVml_1_65536>			m_oCropBottom;
			nullable<SimpleTypes::CTrueFalse>					m_oDetectMouseClick;
			nullable<SimpleTypes::CColorType>					m_oEmbossColor;
			nullable_double										m_oGain;
			nullable_double										m_oGamma;
			nullable<SimpleTypes::CTrueFalse>					m_oGrayscale;
			nullable<SimpleTypes::CRelationshipId>				m_rHref;
			nullable_string										m_oHref;
			nullable<SimpleTypes::CRelationshipId>				m_rId;
			nullable_string										m_oId;
			nullable_double										m_oMovie;
			nullable_string										m_oOleId;
			nullable<SimpleTypes::CRelationshipId>				m_rPict;
			nullable<SimpleTypes::CColorType>					m_oRecolorTarget;
			nullable<SimpleTypes::CRelationshipId>				m_oRelId;
			nullable_string										m_sSrc;
			nullable_string										m_sTitle;
		};

		//--------------------------------------------------------------------------------
		// CLine 14.1.2.12 (Part4)
		//--------------------------------------------------------------------------------	
		class CLine : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CLine)

			CLine(OOX::Document *pMain = NULL);
			virtual ~CLine();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			SimpleTypes::Vml::CVml_Vector2D_Units m_oFrom;
			SimpleTypes::Vml::CVml_Vector2D_Units m_oTo;
		};

		//--------------------------------------------------------------------------------
		// COval 14.1.2.13 (Part4)
		//--------------------------------------------------------------------------------	
		class COval : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(COval)

			COval(OOX::Document *pMain = NULL);
			virtual ~COval();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CPath 14.1.2.14 (Part4)
		//--------------------------------------------------------------------------------	
		class CPath : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPath)

			CPath(OOX::Document *pMain = NULL);
			virtual ~CPath();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTrueFalse>								m_oArrowOk;
			nullable_string													m_oConnectAngles;
			nullable_string													m_oConnectLocs;
			nullable<SimpleTypes::CConnectType>								m_oConnectType;
			nullable<SimpleTypes::CTrueFalse>								m_oExtrusionOk;
			nullable<SimpleTypes::CTrueFalse>								m_oFillOk;
			nullable<SimpleTypes::CTrueFalse>								m_oGradientShapeOk;
			nullable_string													m_oId;
			nullable<SimpleTypes::CTrueFalse>								m_oInsetPenOk;
			nullable<SimpleTypes::Vml::CVml_Vector2D_Units>					m_oLimo;
			nullable<SimpleTypes::CTrueFalse>								m_oShadowOk;
			nullable<SimpleTypes::CTrueFalse>								m_oStrokeOk;
			nullable<SimpleTypes::Vml::CVml_Polygon2D>						m_oTextBoxRect;
			nullable<SimpleTypes::CTrueFalse>								m_oTextPathOk;
			nullable<SimpleTypes::Vml::CVmlPath>							m_oV;
		};

		//--------------------------------------------------------------------------------
		// CPolyLine 14.1.2.15 (Part4)
		//--------------------------------------------------------------------------------	
		class CPolyLine : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CPolyLine)

			CPolyLine(OOX::Document *pMain = NULL);
			virtual ~CPolyLine();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			SimpleTypes::Vml::CVml_Polygon2D_Units m_oPoints;
		};

		//--------------------------------------------------------------------------------
		// CRect 14.1.2.16 (Part4)
		//--------------------------------------------------------------------------------	
		class CRect : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CRect)

			CRect(OOX::Document *pMain = NULL);
			virtual ~CRect();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		};

		//--------------------------------------------------------------------------------
		// CRoundRect 14.1.2.17 (Part4)
		//--------------------------------------------------------------------------------	
		class CRoundRect : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CRoundRect)

			CRoundRect(OOX::Document *pMain = NULL);
			virtual ~CRoundRect();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			SimpleTypes::Vml::CVml_1_65536_Or_Percentage m_oArcSize;
		};

		//--------------------------------------------------------------------------------
		// CShadow 14.1.2.18 (Part4)
		//--------------------------------------------------------------------------------	
		class CShadow : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CShadow)

			CShadow(OOX::Document *pMain = NULL);
			virtual ~CShadow();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			SimpleTypes::CColorType									m_oColor = SimpleTypes::colortypeRGB;
			SimpleTypes::CColorType									m_oColor2 = SimpleTypes::colortypeRGB;
			nullable_string											m_oId;
			nullable<SimpleTypes::Vml::CVml_Matrix>					m_oMatrix;
			SimpleTypes::CTrueFalse									m_oObscured;
			SimpleTypes::Vml::CVml_Vector2D_Units_Or_Percentage		m_oOffset;
			SimpleTypes::Vml::CVml_Vector2D_Units_Or_Percentage		m_oOffset2;
			SimpleTypes::CTrueFalse									m_oOn = SimpleTypes::booleanTrue;
			nullable<SimpleTypes::Vml::CVml_1_65536>				m_oOpacity;
			SimpleTypes::Vml::CVml_Vector2D_Percentage				m_oOrigin;
			SimpleTypes::CShadowType								m_oType;
		};

		//--------------------------------------------------------------------------------
		// CShapeType 14.1.2.20 (Part4)
		//--------------------------------------------------------------------------------	
		class CShapeType : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CShapeType)

			CShapeType(OOX::Document *pMain = NULL);
			virtual ~CShapeType();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string	m_sType;
			nullable_string	m_sAdj;
			nullable<SimpleTypes::Vml::CVmlPath> m_oPath;
			nullable_bool m_oMaster;
		};

		//--------------------------------------------------------------------------------
		// CShape 14.1.2.19 (Part4)
		//--------------------------------------------------------------------------------	
		class CShape : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CShape)

			CShape(OOX::Document *pMain = NULL);
			virtual ~CShape();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			void mergeFrom(CShapeType* shape_type);

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
            nullable_string							m_sType;
            nullable_string							m_sAdj;
            nullable<SimpleTypes::Vml::CVmlPath>	m_oPath;
            nullable_string							m_sGfxData;
            nullable_string							m_sEquationXML;
		};

		class CClientData : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CClientData)

			CClientData(OOX::Document *pMain = NULL);
			virtual ~CClientData();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			void getAnchorArray(std::vector<int>& aAnchor) const;

			bool toCellAnchor(OOX::Spreadsheet::CCellAnchor* pCellAnchor);
			void toFormControlPr(OOX::Spreadsheet::CFormControlPr* pFormControlPr);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			
			nullable<SimpleTypes::Vml::CVmlClientDataObjectType>	m_oObjectType;

			nullable_bool											m_oMoveWithCells;
			nullable_bool											m_oSizeWithCells;
			nullable_string											m_oAnchor;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oRow;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oColumn;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oMin;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oMax;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oInc;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oDx;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oPage;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oDropLines;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oSel;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oWidthMin;
			nullable<SimpleTypes::Spreadsheet::CDropStyle>			m_oDropStyle;
			nullable_bool											m_oFirstButton;
			nullable_bool											m_oDefaultSize;
			nullable_bool											m_oAutoFill;
			nullable_bool											m_oAutoScale;
			nullable_bool											m_oAutoLine;
			nullable_bool											m_oHoriz;
			nullable_bool											m_oVScroll;
			nullable_bool											m_oAutoPict;
			nullable_bool											m_oColored;
			nullable_bool											m_oMultiLine;
			nullable_bool											m_oNoThreeD;
			nullable_bool											m_oNoThreeD2;
			nullable_bool											m_oLockText;
			nullable_bool											m_oJustLastX;
			nullable_bool											m_oSecretEdit;
			nullable_string											m_oFmlaLink;
			nullable_string											m_oFmlaRange;
			nullable_string											m_oFmlaMacro;
			nullable_string											m_oFmlaTxbx;
			nullable_string											m_oFmlaGroup;
			nullable_string											m_oCf;
			nullable<SimpleTypes::Spreadsheet::CChecked>			m_oChecked;
			nullable_string											m_oMultiSel;
			nullable<SimpleTypes::Spreadsheet::CSelType>			m_oSelType;
			nullable_int											m_oVal;
			nullable<SimpleTypes::Spreadsheet::CHorizontalAlignment>m_oTextHAlign;
			nullable<SimpleTypes::Spreadsheet::CVerticalAlignment>	m_oTextVAlign;
			nullable_bool											m_oVisible;

//x:Accel
//x:Accel2
//x:Camera
//x:Cancel
//x:ClientData
//x:ColHidden
//x:DDE
//x:Default
//x:Disabled
//x:Dismiss
//x:FmlaPict
//x:Help
//x:LCT
//x:ListItem
//x:Locked
//x:MapOCX
//x:PrintObject
//x:RecalcAlways
//x:RowHidden
//x:ScriptExtended
//x:ScriptLanguage
//x:ScriptLocation
//x:ScriptText
//x:UIObj
//x:ValidIds
//x:VTEdit
		};

		//--------------------------------------------------------------------------------
		// CStroke 14.1.2.21 (Part4)
		//--------------------------------------------------------------------------------	
		class CStroke : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CStroke)

			CStroke(OOX::Document *pMain = NULL);
			virtual ~CStroke();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string																m_oId;
			nullable_string																m_sAltHref;
			nullable<SimpleTypes::CColorType>											m_oColor;
			nullable<SimpleTypes::CColorType>											m_oColor2;
			nullable<SimpleTypes::Vml::CVmlDashStyle>									m_oDahsStyle;
			nullable<SimpleTypes::CStrokeArrowType>										m_oEndArrow;
			nullable<SimpleTypes::CStrokeArrowLength>									m_oEndArrowLength;
			nullable<SimpleTypes::CStrokeArrowWidth>									m_oEndArrowWidth;
			nullable<SimpleTypes::CStrokeEndCap>										m_oEndCap;
			nullable<SimpleTypes::CFillType>											m_oFillType;
			nullable<SimpleTypes::CTrueFalse>											m_oForceDash;
			nullable_string																m_sHref;
			nullable<SimpleTypes::CRelationshipId>										m_rId;
			nullable<SimpleTypes::CTrueFalse>											m_oImageAlignShape;
			nullable<SimpleTypes::CImageAspect>											m_oImageAspect;
			nullable<SimpleTypes::Vml::CVml_Vector2D_Units>								m_oImageSize; 
			nullable<SimpleTypes::CTrueFalse>											m_oInsetPen;
			nullable<SimpleTypes::CStrokeJoinStyle>										m_oJoinStyle;
			nullable<SimpleTypes::CStrokeLineStyle>										m_oLineStyle;
			nullable<SimpleTypes::CDecimalNumber>										m_oMiterLimit;
			nullable<SimpleTypes::CTrueFalse>											m_oOn;
			nullable_double																m_oOpacity;
			nullable<SimpleTypes::CRelationshipId>										m_oRelId;
			nullable_string																m_sSrc;
			nullable<SimpleTypes::CStrokeArrowType>										m_oStartArrow;
			nullable<SimpleTypes::CStrokeArrowLength>									m_oStartArrowLength;
			nullable<SimpleTypes::CStrokeArrowWidth>									m_oStartArrowWidth;
			nullable_string																m_sTitle;
			nullable_double																m_oWeight;

			nullable<OOX::VmlOffice::CStrokeChild>                                m_oLeft;
			nullable<OOX::VmlOffice::CStrokeChild>                                m_oTop;
			nullable<OOX::VmlOffice::CStrokeChild>                                m_oRight;
			nullable<OOX::VmlOffice::CStrokeChild>                                m_oBottom;
			nullable<OOX::VmlOffice::CStrokeChild>                                m_oColumn;
		};

		//--------------------------------------------------------------------------------
		// CTextbox 14.1.2.22 (Part4)
		//--------------------------------------------------------------------------------	
		class CTextbox : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTextbox)

			CTextbox(OOX::Document *pMain = NULL);
			virtual ~CTextbox();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string											m_oId;
			nullable<SimpleTypes::Vml::CCssStyle>					m_oStyle;
			nullable<SimpleTypes::Vml::CVml_TextBoxInset>			m_oInset;
			SimpleTypes::CTrueFalse									m_oSingleClick;
			SimpleTypes::CInsetMode									m_oInsetMode = SimpleTypes::insetmodeCustom;

			nullable<OOX::Logic::CTxbxContent>						m_oTxtbxContent;
			nullable_string											m_oText; //m_arDivs;
		};

		//--------------------------------------------------------------------------------
		// CTextPath 14.1.2.23 (Part4)
		//--------------------------------------------------------------------------------	
		class CTextPath : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTextPath)

			CTextPath(OOX::Document *pMain = NULL);
			virtual ~CTextPath();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTrueFalse>		m_oFitPath;
			nullable<SimpleTypes::CTrueFalse>		m_oFitShape;
			nullable_string							m_oId;
			nullable<SimpleTypes::CTrueFalse>		m_oOn;
			nullable_string							m_sString;
			nullable<SimpleTypes::Vml::CCssStyle>	m_oStyle;
			nullable<SimpleTypes::CTrueFalse>		m_oTrim;
			nullable<SimpleTypes::CTrueFalse>		m_oXScale;
			
			nullable_string							m_sStringOriginal;
		};

		//--------------------------------------------------------------------------------
		// CGroup 14.1.2.7 (Part4)
		//--------------------------------------------------------------------------------	
		class CGroup : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CGroup);
			
			CGroup(OOX::Document *pMain = NULL);
			virtual ~CGroup();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			std::vector<CShapeType*>		m_arrShapeTypes;
			std::vector<WritingElement *>	m_arrElements;

			nullable<SimpleTypes::CEditAs>					m_oEditAs;
			nullable<SimpleTypes::Vml::CVml_TableLimits>	m_oTableLimits;
			SimpleTypes::Vml::CVml_TableProperties			m_oTableProperties;
		};

	} // namespace Vml
} // namespace OOX

namespace OOX
{
	namespace VmlOffice
	{
		//--------------------------------------------------------------------------------
		// CShapeDefaults 14.2.2.28 (Part 4)
		//--------------------------------------------------------------------------------	
		class CShapeDefaults : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CShapeDefaults)

			CShapeDefaults(OOX::Document *pMain = NULL);
			virtual ~CShapeDefaults();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			SimpleTypes::CTrueFalse									m_oAllowInCell;
			nullable<SimpleTypes::CExt>								m_oExt;
			SimpleTypes::CTrueFalse									m_oFill = SimpleTypes::booleanTrue;
			nullable<SimpleTypes::CColorType>						m_oFillColor;
			SimpleTypes::CDecimalNumber								m_oSpIdMax;
			SimpleTypes::CTrueFalse									m_oStroke = SimpleTypes::booleanTrue;
			SimpleTypes::CColorType									m_oStrokeColor;
			nullable<SimpleTypes::Vml::CCssStyle>					m_oStyle;

			nullable<OOX::Vml::CFill>								m_oVmlFill;
			nullable<OOX::Vml::CStroke>								m_oVmlStroke;
			nullable<OOX::Vml::CTextbox>							m_oVmlTextbox;
			nullable<OOX::Vml::CShadow>								m_oVmlShadow;

			nullable<OOX::VmlOffice::CSkew>							m_oSkew;
			nullable<OOX::VmlOffice::CExtrusion>					m_oExtrusion;
			nullable<OOX::VmlOffice::CCallout>						m_oCallout;
			nullable<OOX::VmlOffice::CLock>							m_oLock;
			nullable<OOX::VmlOffice::CColorMru>						m_oColorMru;
			nullable<OOX::VmlOffice::CColorMenu>					m_oColorMenu;
		};

	} // VmlOffice
} // namespace OOX
