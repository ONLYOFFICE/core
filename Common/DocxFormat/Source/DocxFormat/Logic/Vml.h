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
            CVmlCommonElements(){m_bComment = false;}
			
			virtual ~CVmlCommonElements(){}

			virtual void fromXML(XmlUtils::CXmlNode& oNode){}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes	(oReader);
				ReadElements	(oReader);
			}
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return et_v_shape_elements;
			}
			
	// 1 AG_AllCoreAttributes
	// 1.1 AG_CoreAttributes
			nullable_string											m_sId;
			nullable<SimpleTypes::Vml::CCssStyle>					m_oStyle;
			nullable_string											m_sHref;
			nullable_string											m_sTarget;
			nullable_string											m_sClass;
			nullable_string											m_sTitle;
			nullable_string											m_sAlt;
			nullable<SimpleTypes::Vml::CVml_Vector2D>				m_oCoordSize;
			nullable<SimpleTypes::Vml::CVml_Vector2D>				m_oCoordOrigin;
			nullable<SimpleTypes::Vml::CVml_Polygon2D>				m_oWrapCoords;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>		m_oPrint;
	// 1.2 AG_OfficeCoreAttributes
			nullable_string											m_sSpId;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oOned;
			nullable<SimpleTypes::CDecimalNumber<>>					m_oRegroupId;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oDoubleClickNotify;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oButton;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oUserHidden;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oBullet;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oHr;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oHrStd;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oHrNoShade;
			SimpleTypes::CDouble									m_oHrPct;
			SimpleTypes::CHrAlign<SimpleTypes::hralignLeft>			m_oHrAlign;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oAllowInCell;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>		m_oAllowOverlap;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oUserDrawn;
			nullable<SimpleTypes::CColorType<>>						m_oBorderTopColor;
			nullable<SimpleTypes::CColorType<>>						m_oBorderLeftColor;
			nullable<SimpleTypes::CColorType<>>						m_oBorderBottomColor;
			nullable<SimpleTypes::CColorType<>>						m_oBorderRightColor;
			nullable<SimpleTypes::CDiagramLayout<>>					m_oDgmLayout;
			nullable_string											m_oDgmNodeKind;
			nullable<SimpleTypes::CDiagramLayout<>>					m_oDgmLayoutMru;
			SimpleTypes::CInsetMode<SimpleTypes::insetmodeAuto>		m_oInsetMode;
	// 2 AG_AllShapeAttributes
	// 2.1 AG_ShapeAttributes
			nullable<SimpleTypes::CColorType<>>						m_oChromaKey;
			nullable<SimpleTypes::CTrueFalse<>>						m_oFilled;
			nullable<SimpleTypes::CColorType<>>						m_oFillColor;
			nullable<SimpleTypes::Vml::CVml_1_65536>				m_oOpacity;
			nullable<SimpleTypes::CTrueFalse<>>						m_oStroked;
			nullable<SimpleTypes::CColorType<>>						m_oStrokeColor;
			nullable<SimpleTypes::CEmu>								m_oStrokeWeight;
			nullable<SimpleTypes::CTrueFalse<>>						m_oInsetPen;
	// 2.2 AG_OfficeShapeAttributes
			nullable<SimpleTypes::CDecimalNumber<>>					m_oSpt;
			nullable<SimpleTypes::CConnectorType<>>					m_oConnectorType;
			nullable<SimpleTypes::CBWMode<>>						m_oBwMode;
			nullable<SimpleTypes::CBWMode<>>						m_oBwPure;
			nullable<SimpleTypes::CBWMode<>>						m_oBwNormal;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oForceDash;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oOleIcon;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oOle;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oPreferRelative;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oClipToWrap;
			nullable<SimpleTypes::CTrueFalse<>>						m_oClip;

			void	ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void	ReadElements(XmlUtils::CXmlLiteReader& oReader) ;

			std::wstring WriteAttributes() const;
			std::wstring WriteElements() const;

//internal
            bool m_bComment;
		};

		//--------------------------------------------------------------------------------
		// CArc 14.1.2.1 (Part4)
		//--------------------------------------------------------------------------------	
		class CArc : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CArc)
			CArc(){}
			virtual ~CArc(){}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CArc::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
				
				CVmlCommonElements::ReadAttributes( oReader );
				CVmlCommonElements::ReadElements( oReader );
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<v:arc ");

				sResult += CVmlCommonElements::WriteAttributes();

				sResult += _T("startangle=\"") + m_oStartAngle.ToString() + _T("\" ");
				sResult += _T("endangle=\"")   + m_oEndAngle.ToString()   + _T("\">");

				sResult += CVmlCommonElements::WriteElements();

				sResult += _T("</v:arc>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_v_arc;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					wchar_t wsChar = wsName[0];
					switch ( wsChar )
					{
					case 'e':
						if      ( _T("endangle") == wsName || _T("endAngle") == wsName ) m_oEndAngle   = oReader.GetText();
						break;

					case 's':
						if      ( _T("startangle") == wsName || _T("startAngle") == wsName ) m_oStartAngle = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			// Attributes
			SimpleTypes::CDecimalNumber<90> m_oEndAngle;
			SimpleTypes::CDecimalNumber<0>  m_oStartAngle;

		};
		//--------------------------------------------------------------------------------
		// CCurve 14.1.2.3 (Part4)
		//--------------------------------------------------------------------------------	
		class CCurve : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CCurve)
			CCurve(){}
			virtual ~CCurve(){}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CCurve::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
				CVmlCommonElements::ReadAttributes( oReader );
				CVmlCommonElements::ReadElements( oReader );
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<v:curve ");

				sResult += CVmlCommonElements::WriteAttributes();

				sResult += _T("from=\"")     + m_oFrom.ToString()     + _T("\" ");
				sResult += _T("control1=\"") + m_oControl1.ToString() + _T("\" ");
				sResult += _T("control2=\"") + m_oControl2.ToString() + _T("\" ");
				sResult += _T("to=\"")       + m_oTo.ToString()       + _T("\">");

				sResult += CVmlCommonElements::WriteElements();

				sResult += _T("</v:curve>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_v_curve;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					wchar_t wsChar = wsName[0];
					switch ( wsChar )
					{
					case 'c':
						if      ( _T("control1") == wsName ) m_oControl1 = oReader.GetText();
						else if ( _T("control2") == wsName ) m_oControl2 = oReader.GetText();
						break;

					case 'f':
						if      ( _T("from")     == wsName ) m_oFrom     = oReader.GetText();
						break;

					case 't':
						if      ( _T("to")       == wsName ) m_oTo       = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			// Attributes
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
			CF()
			{
			}
			virtual ~CF()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CF::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = _T("<v:f eqn=\"") + m_sEqn + _T("\"/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_v_f;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					wchar_t wsChar = wsName[0];
					switch ( wsChar )
					{
					case 'e':
						if      ( _T("eqn") == wsName ) m_sEqn = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();

				// TO DO: Сделать парсер формул ( или использовать уже сделанный парсер в OfficeDrawing\Shapes)
			}

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
			CFill()
			{
			}
			virtual ~CFill()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CFill::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("o:fill") == sName )
						m_oFill = oReader;
				}
			}
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_v_fill;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:

			typedef struct TIntermediateColor
			{
				double                    dValue;
				SimpleTypes::CColorType<> oColor;
			};

			// Attributes
			nullable<SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>>			m_oAlignShape;
			nullable_string														m_sAltHref;
			nullable<SimpleTypes::CDecimalNumber<>>								m_oAngle;
			nullable<SimpleTypes::CImageAspect<SimpleTypes::imageaspectIgnore>> m_oAspect;
			nullable<SimpleTypes::CColorType<>>									m_oColor;
			nullable<SimpleTypes::CColorType<>>									m_oColor2;
			std::vector<TIntermediateColor*>									m_arrColors;
			nullable<SimpleTypes::CTrueFalse<>>									m_oDetectMouseClick;
			nullable<SimpleTypes::CFixedPercentage >							m_oFocus;
			nullable<SimpleTypes::Vml::CVml_Vector2D_Percentage>				m_oFocusPosition;
			nullable<SimpleTypes::Vml::CVml_Vector2D_Percentage>				m_oFocusSize;
			nullable_string														m_sHref;
			nullable<SimpleTypes::CRelationshipId>								m_rId;
			nullable_string														m_sId;
			nullable<SimpleTypes::CFillMethod<SimpleTypes::fillmethodSigma>>	m_oMethod;
			nullable<SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>>			m_oOn;
			nullable<SimpleTypes::Vml::CVml_1_65536>							m_oOpacity;
			nullable<SimpleTypes::Vml::CVml_1_65536>							m_oOpacity2;
			nullable<SimpleTypes::Vml::CVml_Vector2D_1_65536>					m_oOrigin;
			nullable<SimpleTypes::Vml::CVml_Vector2D_1_65536>					m_oPosition;
			nullable<SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>>		m_oRecolor;
			nullable<SimpleTypes::CRelationshipId>								m_oRelId;
			nullable<SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>>		m_oRotate;
			nullable<SimpleTypes::Vml::CVml_Vector2D_Units>						m_oSize;
			nullable_string														m_sSrc;
			nullable_string														m_sTitle;
			SimpleTypes::CFillType<SimpleTypes::filltypeSolid, 0>				m_oType;

			// Childs
			nullable<OOX::VmlOffice::CFill>										m_oFill;

		};
		//--------------------------------------------------------------------------------
		// CBackground 14.1.2.2 (Part4)
		//--------------------------------------------------------------------------------	
		class CBackground : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CBackground)
			CBackground()
			{
			}
			virtual ~CBackground()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CBackground::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				CVmlCommonElements::ReadAttributes( oReader );
				CVmlCommonElements::ReadElements( oReader );

			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<v:background ");

				sResult += CVmlCommonElements::WriteAttributes();
								
				ComplexTypes_WriteAttribute ( _T("o:targetscreensize=\""),	m_oTargetScreenSize );

				sResult += _T(">");

				sResult += CVmlCommonElements::WriteElements();

				sResult += _T("</v:background>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_v_background;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("o:targetscreensize"), m_oTargetScreenSize)
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

	// Attributes
			nullable<SimpleTypes::CScreenSize<>> m_oTargetScreenSize;

		};
		//--------------------------------------------------------------------------------
		// CFormulas 14.1.2.6 (Part4)
		//--------------------------------------------------------------------------------	
		class CFormulas : public WritingElementWithChilds<OOX::Vml::CF>
		{
		public:
			WritingElement_AdditionConstructors(CFormulas)
			CFormulas()
			{
			}
			virtual ~CFormulas()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CFormulas::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("v:f") == sName )
					{
						OOX::Vml::CF *oF = new OOX::Vml::CF(oReader);
						if (oF) m_arrItems.push_back( oF );
					}
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<v:formulas>");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        sResult += m_arrItems[i]->toXML();
                    }
                }

				sResult += _T("</v:formulas>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_v_formulas;
			}


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
			CH()
			{
			}
			virtual ~CH()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CH::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_v_h;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:

			// Attributes
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>        m_oInvX;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>        m_oInvY;
			SimpleTypes::Vml::CVml_Vector2D                           m_oMap;
			nullable<SimpleTypes::Vml::CVml_Vector2D>                 m_oPolar;
			SimpleTypes::Vml::CVml_Vector2D_Position                  m_oPosition;
			SimpleTypes::Vml::CVml_Vector2D                           m_oRadiusRange;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>        m_oSwitch;
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
			CHandles()
			{
			}
			virtual ~CHandles()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CHandles::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("v:h") == sName )
					{
						OOX::Vml::CH *oH = new OOX::Vml::CH(oReader);
						if (oH) m_arrItems.push_back( oH );
					}
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<v:handles>");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        sResult += m_arrItems[i]->toXML();
                    }
                }
				sResult += _T("</v:handles>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_v_handles;
			}


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
			CImage(){}
			virtual ~CImage(){}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CImage::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
				CVmlCommonElements::ReadAttributes( oReader );
				CVmlCommonElements::ReadElements( oReader );
			}
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_v_image;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:

			// Attributes
			std::wstring                                            m_sSrc;
			SimpleTypes::Vml::CVml_1_65536                     m_oCropLeft;
			SimpleTypes::Vml::CVml_1_65536                     m_oCropTop;
			SimpleTypes::Vml::CVml_1_65536                     m_oCropRight;
			SimpleTypes::Vml::CVml_1_65536                     m_oCropBottom;
			SimpleTypes::CDouble                               m_oGain;
			SimpleTypes::CDouble                               m_oBlackLevel;
			SimpleTypes::CDouble                               m_oGamma;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse> m_oGrayscale;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse> m_oBiLevel;
		};
		//--------------------------------------------------------------------------------
		// CImageData 14.1.2.11 (Part4)
		//--------------------------------------------------------------------------------	
		class CImageData : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CImageData)
			CImageData()
			{
			}
			virtual ~CImageData()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CImageData::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_v_imagedata;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:

			// Attributes
			nullable_string										m_sAltHref;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>	m_oBiLevel;
			SimpleTypes::CDouble								m_oBlackLevel;
			nullable<SimpleTypes::CColorType<>>					m_oChromaKey;
			nullable<SimpleTypes::Vml::CVml_1_65536>			m_oCropLeft;
			nullable<SimpleTypes::Vml::CVml_1_65536>			m_oCropTop;
			nullable<SimpleTypes::Vml::CVml_1_65536>			m_oCropRight;
			nullable<SimpleTypes::Vml::CVml_1_65536>			m_oCropBottom;
			nullable<SimpleTypes::CTrueFalse<>>					m_oDetectMouseClick;
			nullable<SimpleTypes::CColorType<>>					m_oEmbossColor;
			SimpleTypes::CDouble								m_oGain;
			SimpleTypes::CDouble								m_oGamma;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>	m_oGrayscale;
			nullable<SimpleTypes::CRelationshipId>				m_rHref;
			nullable_string										m_oHref;
			nullable<SimpleTypes::CRelationshipId>				m_rId;
			nullable_string										m_oId;
			nullable<SimpleTypes::CDouble>						m_oMovie;
			nullable_string										m_oOleId;
			nullable<SimpleTypes::CRelationshipId>				m_rPict;
			nullable<SimpleTypes::CColorType<>>					m_oRecolorTarget;
			nullable<SimpleTypes::CRelationshipId>				m_oRelId;
			std::wstring										m_sSrc;
			nullable_string										m_sTitle;
		};
		//--------------------------------------------------------------------------------
		// CLine 14.1.2.12 (Part4)
		//--------------------------------------------------------------------------------	
		class CLine : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CLine)
			CLine(){}
			virtual ~CLine(){}
		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CLine::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
				CVmlCommonElements::ReadAttributes( oReader );
				CVmlCommonElements::ReadElements( oReader );
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<v:line ");

				sResult += CVmlCommonElements::WriteAttributes();

				sResult += _T("from=\"") + m_oFrom.ToString() + _T("\" ");
				sResult += _T("to=\"")   + m_oTo.ToString()   + _T("\">");

				sResult += CVmlCommonElements::WriteElements();

				sResult += _T("</v:line>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_v_line;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Выставляем значения по умолчанию
				m_oFrom.SetValue( 0, 0 );
				m_oTo.SetValue( 10, 10 );

				// Читаем атрибуты
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					wchar_t wsChar = wsName[0];
					switch ( wsChar )
					{
					case 'f':
						if      ( _T("from") == wsName ) m_oFrom = oReader.GetText();
						break;

					case 't':
						if      ( _T("to")   == wsName ) m_oTo   = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

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
			COval(){}
			virtual ~COval(){}
		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать COval::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				CVmlCommonElements::ReadAttributes( oReader );
				CVmlCommonElements::ReadElements( oReader );
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<v:oval ");

				sResult += CVmlCommonElements::WriteAttributes();

				sResult += _T(">");

				sResult += CVmlCommonElements::WriteElements();

				sResult += _T("</v:oval>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_v_oval;
			}

		};
		//--------------------------------------------------------------------------------
		// CPath 14.1.2.14 (Part4)
		//--------------------------------------------------------------------------------	
		class CPath : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPath)
			CPath()
			{
			}
			virtual ~CPath()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CPath::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_v_path;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:

			// Attributes
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oArrowOk;
			nullable_string											m_oConnectAngles;
			nullable_string											m_oConnectLocs;
			SimpleTypes::CConnectType<SimpleTypes::connecttypeNone>	m_oConnectType;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>		m_oExtrusionOk;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>		m_oFillOk;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oGradientShapeOk;
			nullable_string											m_oId;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oInsetPenOk;
			SimpleTypes::Vml::CVml_Vector2D_Units					m_oLimo;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>		m_oShadowOk;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>		m_oStrokeOk;
			nullable<SimpleTypes::Vml::CVml_Polygon2D>				m_oTextBoxRect;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oTextPathOk;
			nullable<SimpleTypes::Vml::CVmlPath>					m_oV;
		};
		//--------------------------------------------------------------------------------
		// CPolyLine 14.1.2.15 (Part4)
		//--------------------------------------------------------------------------------	
		class CPolyLine : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CPolyLine)
			CPolyLine(){}
			virtual ~CPolyLine(){}
			virtual void         fromXML(XmlUtils::CXmlNode& oNode){}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
				CVmlCommonElements::ReadAttributes( oReader );
				CVmlCommonElements::ReadElements( oReader );
			}
			virtual std::wstring      toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_v_polyline;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Выставляем значения по умолчанию
				m_oPoints.SetDelimiter( ' ' );

				// Читаем атрибуты
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					wchar_t wsChar = wsName[0];
					switch ( wsChar )
					{
					case 'p':
						if      ( _T("points") == wsName ) m_oPoints = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

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
			CRect(){}
			virtual ~CRect(){}
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CRect::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				CVmlCommonElements::ReadAttributes( oReader );
				CVmlCommonElements::ReadElements( oReader );
			}
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = _T("<v:rect ");

				sResult += CVmlCommonElements::WriteAttributes();

				sResult += _T(">");

				sResult += CVmlCommonElements::WriteElements();

				sResult += _T("</v:rect>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_v_rect;
			}

		};
		//--------------------------------------------------------------------------------
		// CRoundRect 14.1.2.17 (Part4)
		//--------------------------------------------------------------------------------	
		class CRoundRect : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CRoundRect)
			CRoundRect(){}
			virtual ~CRoundRect(){}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CRoundRect::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
				CVmlCommonElements::ReadAttributes( oReader );
				CVmlCommonElements::ReadElements( oReader );
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<v:roundrect ");

				sResult += CVmlCommonElements::WriteAttributes();

				sResult += _T("arcsize=\"") + m_oArcSize.ToString() + _T("\" ");

				sResult += CVmlCommonElements::WriteElements();

				sResult += _T("</v:roundrect>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_v_roundrect;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Выставляем значения по умолчанию
				m_oArcSize.SetPercentage( 20 );

				// Читаем атрибуты
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					wchar_t wsChar = wsName[0];
					switch ( wsChar )
					{
					case 'a':
						if      ( _T("arcsize") == wsName ) m_oArcSize = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			// Attributes
			SimpleTypes::Vml::CVml_1_65536_Or_Percentage m_oArcSize;

		};
		//--------------------------------------------------------------------------------
		// CShadow 14.1.2.18 (Part4)
		//--------------------------------------------------------------------------------	
		class CShadow : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CShadow)
			CShadow()
			{
			}
			virtual ~CShadow()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CShadow::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<v:shadow ");

				ComplexTypes_WriteAttribute3( L"id=\"", m_oId );

				if ( SimpleTypes::booleanTrue != m_oOn.GetValue() )
					sResult += L"on=\"false\" ";

				if ( SimpleTypes::shadowtypeSingle != m_oType.GetValue() )
					sResult += _T("type=\"") + m_oType.ToString() + _T("\" ");

				if ( SimpleTypes::booleanFalse != m_oObscured.GetValue() )
					sResult += _T("obscured=\"true\" ");

				if ( 128 != m_oColor.Get_R() || 128 != m_oColor.Get_G() || 128 != m_oColor.Get_B() )
					sResult += _T("color=\"") + m_oColor.ToString() + _T("\" ");

				if (m_oOpacity.IsInit())
					sResult += _T("opacity=\"") + m_oOpacity->ToString() + _T("\" ");

				sResult += _T("offset=\"") + m_oOffset.ToString() + _T("\" ");

				if ( 203 != m_oColor2.Get_R() || 203 != m_oColor2.Get_G() || 203 != m_oColor2.Get_B() )
					sResult += _T("color2=\"") + m_oColor2.ToString() + _T("\" ");

				sResult += _T("offset2=\"") + m_oOffset2.ToString() + _T("\" ");

				if ( 0 != m_oOrigin.GetX() || 0 != m_oOrigin.GetY() )
					sResult += _T("origin=\"") + m_oOrigin.ToString() + _T("\" ");

				ComplexTypes_WriteAttribute ( _T("matrix=\""), m_oMatrix );
				
				sResult += _T("/>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_v_shadow;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Выставляем значения по умолчанию
				m_oColor.SetRGB( 128, 128, 128 );
				m_oColor2.SetRGB( 203, 203, 203 );
				m_oOffset.SetValue_Points( 2, 2 );
				m_oOffset2.SetValue_Points( -2, -2 );
				m_oOrigin.SetValue( 0, 0 );

				// Читаем атрибуты
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					wchar_t wsChar = wsName[0];
					switch ( wsChar )
					{
					case 'c':
						if      ( _T("color")      == wsName ) m_oColor      = oReader.GetText();
						else if ( _T("color2")     == wsName ) m_oColor2     = oReader.GetText();
						break;

					case 'i':
						if      ( _T("id")         == wsName ) m_oId         = oReader.GetText();
						break;

					case 'm':
						if      ( _T("matrix")     == wsName ) m_oMatrix     = oReader.GetText();
						break;

					case 'o':
						if      ( _T("obscured")   == wsName ) m_oObscured   = oReader.GetText();
						else if ( _T("offset")     == wsName ) m_oOffset     = oReader.GetText();
						else if ( _T("offset2")    == wsName ) m_oOffset2    = oReader.GetText();
						else if ( _T("on")         == wsName ) m_oOn         = oReader.GetText();
						else if ( _T("opacity")    == wsName ) m_oOpacity    = oReader.GetText();
						else if ( _T("origin")     == wsName ) m_oOrigin     = oReader.GetText();
						break;

					case 't':
						if      ( _T("type")       == wsName ) m_oType       = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:
			SimpleTypes::CColorType<SimpleTypes::colortypeRGB>		m_oColor;
			SimpleTypes::CColorType<SimpleTypes::colortypeRGB>		m_oColor2;
			nullable_string											m_oId;
			nullable<SimpleTypes::Vml::CVml_Matrix>					m_oMatrix;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oObscured;
			SimpleTypes::Vml::CVml_Vector2D_Units_Or_Percentage		m_oOffset;
			SimpleTypes::Vml::CVml_Vector2D_Units_Or_Percentage		m_oOffset2;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>		m_oOn;
			nullable<SimpleTypes::Vml::CVml_1_65536>				m_oOpacity;
			SimpleTypes::Vml::CVml_Vector2D_Percentage				m_oOrigin;
			SimpleTypes::CShadowType<SimpleTypes::shadowtypeSingle>	m_oType;
		};
		//--------------------------------------------------------------------------------
		// CShape 14.1.2.19 (Part4)
		//--------------------------------------------------------------------------------	
		class CShape : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CShape)
			CShape(){}
			virtual ~CShape(){}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode){}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
				CVmlCommonElements::ReadAttributes( oReader );
				CVmlCommonElements::ReadElements( oReader );
			}
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_v_shape;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
			
				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					wchar_t wsChar = wsName[0];
					switch ( wsChar )
					{
					case 'a':
                        if      ( _T("adj")         == wsName ) m_sAdj         = oReader.GetText();
						break;
					case 'e':
						if      ( _T("equationxml") == wsName ) m_sEquationXML = oReader.GetText();
						break;
					case 'o':
                        if      ( _T("o:gfxdata")   == wsName ) m_sGfxData       = oReader.GetText();
						break;
					case 'p':
						if      ( _T("path")        == wsName ) m_oPath        = oReader.GetText();
						break;
					case 't':
						if      ( _T("type")        == wsName ) m_sType        = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();

				// TO DO: Сделать парсер Adj
			}

		public:

			// Attributes
            nullable_string		                  m_sType;
            nullable_string		                  m_sAdj;
            nullable<SimpleTypes::Vml::CVmlPath>    m_oPath;
            nullable_string		                  m_sGfxData;
            nullable_string		                  m_sEquationXML;
		};
		//--------------------------------------------------------------------------------
		// CShapeType 14.1.2.20 (Part4)
		//--------------------------------------------------------------------------------	
		class CShapeType : public CVmlCommonElements
		{
		public:
			WritingElement_AdditionConstructors(CShapeType)
			CShapeType(){}
			virtual ~CShapeType(){}
			virtual void fromXML(XmlUtils::CXmlNode& oNode){}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
				CVmlCommonElements::ReadAttributes( oReader );
				CVmlCommonElements::ReadElements( oReader );
			}
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_v_shapetype;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;				
				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring sGfxData;
				
				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					wchar_t wsChar = wsName[0];
					switch ( wsChar )
					{
					case 'o':
						if      ( _T("o:master") == wsName ) m_oMaster = oReader.GetText();
						break;
					case 'p':
						if      ( _T("path")     == wsName ) m_oPath   = oReader.GetText();
						break;
					case 'a':
                        if      ( _T("adj")      == wsName ) m_sAdj    = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();

				// TO DO: Сделать парсер Adj
			}

		public:

            nullable_string		                              m_sAdj;
            nullable<SimpleTypes::Vml::CVmlPath>                m_oPath;
            SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>  m_oMaster;
		};
		class CClientData : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CClientData)
			CClientData() {}
			virtual ~CClientData() {}

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_v_ClientData;
			}
			
			void getAnchorArray(std::vector<int>& aAnchor) const
			{
				aAnchor.clear();
				if(m_oAnchor.IsInit())
				{
                    std::vector<std::wstring> arSplit;
                    boost::algorithm::split(arSplit, m_oAnchor.get(), boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);
                    for (size_t i = 0 ; i < arSplit.size(); i++)
                    {
                        aAnchor.push_back(XmlUtils::GetInteger(arSplit[i]));
                    }
				}
			}
			void toCellAnchor(OOX::Spreadsheet::CCellAnchor* pCellAnchor);
			void toFormControlPr(OOX::Spreadsheet::CFormControlPr* pFormControlPr);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, _T("ObjectType"), m_oObjectType )
				WritingElement_ReadAttributes_End( oReader )
			}

			
			nullable<SimpleTypes::Vml::CVmlClientDataObjectType<>>	m_oObjectType;

			nullable_bool											m_oMoveWithCells;
			nullable_bool											m_oSizeWithCells;
			nullable_string											m_oAnchor;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oRow;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oColumn;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oMin;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oMax;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oInc;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oDx;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oPage;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oDropLines;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oSel;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oWidthMin;
			nullable<SimpleTypes::Spreadsheet::CDropStyle<>>		m_oDropStyle;
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
			nullable<SimpleTypes::Spreadsheet::CChecked<>>			m_oChecked;
			nullable_string											m_oMultiSel;
			nullable<SimpleTypes::Spreadsheet::CSelType<>>			m_oSelType;
			nullable_int											m_oVal;
			nullable<SimpleTypes::Spreadsheet::CHorizontalAlignment<>>	m_oTextHAlign;
			nullable<SimpleTypes::Spreadsheet::CVerticalAlignment<>>m_oTextVAlign;
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
			CStroke()
			{
			}
			virtual ~CStroke()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CStroke::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if ( _T("o:left") == sName )
						m_oLeft = oReader;
					else if ( _T("o:top") == sName )
						m_oTop = oReader;
					else if ( _T("o:right") == sName )
						m_oRight = oReader;
					else if ( _T("o:bottom") == sName )
						m_oBottom = oReader;
					else if ( _T("o:column") == sName )
						m_oColumn = oReader;
				}
			}
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_v_stroke;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string																m_oId;
			nullable_string																m_sAltHref;
			nullable<SimpleTypes::CColorType<SimpleTypes::colortypeBlack>>				m_oColor;
			nullable<SimpleTypes::CColorType<SimpleTypes::colortypeBlack>>				m_oColor2;
			SimpleTypes::Vml::CVmlDashStyle<SimpleTypes::Vml::vmldashstyleSolid>		m_oDahsStyle;
			SimpleTypes::CStrokeArrowType<SimpleTypes::strokearrowtypeNone>				m_oEndArrow;
			SimpleTypes::CStrokeArrowLength<SimpleTypes::strokearrowlengthMedium>		m_oEndArrowLength;
			SimpleTypes::CStrokeArrowWidth<SimpleTypes::strokearrowwidthMedium>			m_oEndArrowWidth;
			SimpleTypes::CStrokeEndCap<SimpleTypes::strokeendcapFlat>					m_oEndCap;
			SimpleTypes::CFillType<SimpleTypes::filltypeSolid, 0>						m_oFillType;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>							m_oForceDash;
			nullable_string																m_sHref;
			nullable<SimpleTypes::CRelationshipId>										m_rId;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>							m_oImageAlignShape;
			SimpleTypes::CImageAspect<SimpleTypes::imageaspectIgnore>					m_oImageAspect;
			nullable<SimpleTypes::Vml::CVml_Vector2D_Units>								m_oImageSize; 
			nullable<SimpleTypes::CTrueFalse<>>											m_oInsetPen;
			SimpleTypes::CStrokeJoinStyle<SimpleTypes::strokejoinstyleRound>			m_oJoinStyle;
			nullable<SimpleTypes::CStrokeLineStyle<SimpleTypes::strokelinestyleSingle>>	m_oLineStyle;
			SimpleTypes::CDecimalNumber<8>												m_oMiterLimit;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>							m_oOn;
			nullable<SimpleTypes::CDouble>												m_oOpacity;
			nullable<SimpleTypes::CRelationshipId>										m_oRelId;
			nullable_string																m_sSrc;
			SimpleTypes::CStrokeArrowType<SimpleTypes::strokearrowtypeNone>				m_oStartArrow;
			SimpleTypes::CStrokeArrowLength<SimpleTypes::strokearrowlengthMedium>		m_oStartArrowLength;
			SimpleTypes::CStrokeArrowWidth<SimpleTypes::strokearrowwidthMedium>			m_oStartArrowWidth;
			nullable_string																m_sTitle;
			SimpleTypes::CDouble														m_oWeight;

			// Childs
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
			CTextbox()
			{
			}
			virtual ~CTextbox()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CTextbox::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_v_textbox;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string											m_oId;
			nullable<SimpleTypes::Vml::CCssStyle>					m_oStyle;
			nullable<SimpleTypes::Vml::CVml_TextBoxInset>			m_oInset;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oSingleClick;
			SimpleTypes::CInsetMode<SimpleTypes::insetmodeCustom>	m_oInsetMode;

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
			CTextPath()
			{
			}
			virtual ~CTextPath()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CTextPath::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				m_sStringOriginal = oReader.GetText2();

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<v:textpath ");

				ComplexTypes_WriteAttribute3( L"id=\"",    m_oId );
				ComplexTypes_WriteAttribute ( L"style=\"", m_oStyle );

				if ( (m_oOn.IsInit()) && (SimpleTypes::booleanFalse != m_oOn->GetValue()) )
					sResult += _T("on=\"true\" ");

				if ( (m_oFitShape.IsInit()) && ( SimpleTypes::booleanFalse != m_oFitShape->GetValue()) )
					sResult += _T("fitshape=\"true\" ");

				if ( (m_oFitPath.IsInit()) && ( SimpleTypes::booleanFalse != m_oFitPath->GetValue()) )
					sResult += _T("fitpath=\"true\" ");

				if ( (m_oTrim.IsInit()) && ( SimpleTypes::booleanFalse != m_oTrim->GetValue()) )
					sResult += _T("trim=\"true\" ");

				if ( (m_oXScale.IsInit()) && ( SimpleTypes::booleanFalse != m_oXScale->GetValue()) )
					sResult += _T("xscale=\"true\" ");

				ComplexTypes_WriteAttribute3( _T("string=\""), m_sString );

				sResult += _T("/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_v_textpath;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{				
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					wchar_t wsChar = wsName[0];
					switch ( wsChar )
					{
					case 'f':
						if      ( _T("fitpath")  == wsName ) m_oFitPath  = oReader.GetText();
						else if ( _T("fitshape") == wsName ) m_oFitShape = oReader.GetText();
						break;
					case 'i':
						if      ( _T("id")       == wsName ) m_oId       = oReader.GetText();
						break;
					case 'o':
						if      ( _T("on")       == wsName ) m_oOn       = oReader.GetText();
						break;
					case 's':
						if      ( _T("string")   == wsName ) m_sString   = oReader.GetText();
						else if ( _T("style")    == wsName ) m_oStyle    = oReader.GetText();
						break;
					case 't':
						if      ( _T("trim")     == wsName ) m_oTrim     = oReader.GetText();
						break;
					case 'x':
						if      ( _T("xscale")   == wsName ) m_oXScale   = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:
			nullable<SimpleTypes::CTrueFalse<>>		m_oFitPath;
			nullable<SimpleTypes::CTrueFalse<>>		m_oFitShape;
			nullable_string							m_oId;
			nullable<SimpleTypes::CTrueFalse<>>		m_oOn;
			nullable_string							m_sString;
			nullable<SimpleTypes::Vml::CCssStyle>	m_oStyle;
			nullable<SimpleTypes::CTrueFalse<>>		m_oTrim;
			nullable<SimpleTypes::CTrueFalse<>>		m_oXScale;
			
			nullable_string							m_sStringOriginal;
		};
		//--------------------------------------------------------------------------------
		// CGroup 14.1.2.7 (Part4)
		//--------------------------------------------------------------------------------	
		class CGroup : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CGroup);
			
			CGroup(){}
			virtual ~CGroup(){}
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_v_group;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			// AG_AllCoreAttributes
			nullable_string											m_sId;
			nullable<SimpleTypes::Vml::CCssStyle>					m_oStyle;
			nullable_string											m_sHref;
			nullable_string											m_sTarget;
			nullable_string											m_sClass;
			nullable_string											m_sTitle;
			nullable_string											m_sAlt;
			nullable<SimpleTypes::Vml::CVml_Vector2D>				m_oCoordSize;
			nullable<SimpleTypes::Vml::CVml_Vector2D>				m_oCoordOrigin;
			nullable<SimpleTypes::Vml::CVml_Polygon2D>				m_oWrapCoords;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>		m_oPrint;
			nullable_string											m_sSpId;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oOned;
			nullable<SimpleTypes::CDecimalNumber<>>					m_oRegroupId;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oDoubleClickNotify;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oButton;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oUserHidden;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oBullet;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oHr;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oHrStd;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oHrNoShade;
			SimpleTypes::CDouble									m_oHrPct;
			SimpleTypes::CHrAlign<SimpleTypes::hralignLeft>			m_oHrAlign;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oAllowInCell;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>		m_oAllowOverlap;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>		m_oUserDrawn;
			nullable<SimpleTypes::CColorType<>>						m_oBorderTopColor;
			nullable<SimpleTypes::CColorType<>>						m_oBorderLeftColor;
			nullable<SimpleTypes::CColorType<>>						m_oBorderBottomColor;
			nullable<SimpleTypes::CColorType<>>						m_oBorderRightColor;
			nullable<SimpleTypes::CDiagramLayout<>>					m_oDgmLayout;
			nullable_string											m_oDgmNodeKind;
			nullable<SimpleTypes::CDiagramLayout<>>					m_oDgmLayoutMru;
			SimpleTypes::CInsetMode<SimpleTypes::insetmodeCustom>	m_oInsetMode;

			SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>		m_oFilled;
			nullable<SimpleTypes::CColorType<>>						m_oFillColor;
			nullable<SimpleTypes::CEditAs<>>						m_oEditAs;
			nullable<SimpleTypes::Vml::CVml_TableLimits>			m_oTableLimits;
			SimpleTypes::Vml::CVml_TableProperties<0>				m_oTableProperties;                 
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
			CShapeDefaults()
			{
			}
			virtual ~CShapeDefaults()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CShapeDefaults::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_o_shapedefaults;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>   m_oAllowInCell;
			nullable<SimpleTypes::CExt<>>                        m_oExt;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>    m_oFill;
			nullable<SimpleTypes::CColorType<>>                  m_oFillColor;
			SimpleTypes::CDecimalNumber<0>                       m_oSpIdMax;
			SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>    m_oStroke;
			SimpleTypes::CColorType<SimpleTypes::colortypeBlack> m_oStrokeColor;
			nullable<SimpleTypes::Vml::CCssStyle>                m_oStyle;

			nullable<OOX::Vml::CFill>                            m_oVmlFill;
			nullable<OOX::Vml::CStroke>                          m_oVmlStroke;
			nullable<OOX::Vml::CTextbox>                         m_oVmlTextbox;
			nullable<OOX::Vml::CShadow>                          m_oVmlShadow;

			nullable<OOX::VmlOffice::CSkew>                      m_oSkew;
			nullable<OOX::VmlOffice::CExtrusion>                 m_oExtrusion;
			nullable<OOX::VmlOffice::CCallout>                   m_oCallout;
			nullable<OOX::VmlOffice::CLock>                      m_oLock;
			nullable<OOX::VmlOffice::CColorMru>                  m_oColorMru;
			nullable<OOX::VmlOffice::CColorMenu>                 m_oColorMenu;
		};
	} // VmlOffice
} // namespace OOX
