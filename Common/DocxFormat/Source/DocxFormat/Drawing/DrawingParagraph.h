/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "DrawingParagraphElements.h"
#include "DrawingRun.h"

#include "../WritingElement.h"

namespace OOX
{
	namespace Drawing
	{
		//--------------------------------------------------------------------------------
		// CText 21.1.2.3.11 t (Text String)
		//--------------------------------------------------------------------------------	
		class CText : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CText)
			CText() {}
			virtual ~CText() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("xml:space"), m_oSpace );

				m_sText = oNode.GetText();
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_sText = oReader.GetText2();
			}
			virtual CString      toXML() const
			{
				CString sResult;
				
				if ( m_oSpace.IsInit() )
					sResult = _T("<a:t xml:space=\"") + m_oSpace->ToString() + _T("\">");
				else
					sResult = _T("<a:t>");

                CString s = m_sText;
                s.Replace(_T("&"),	_T("&amp;"));
                s.Replace(_T("'"),	_T("&apos;"));
                s.Replace(_T("<"),	_T("&lt;"));
                s.Replace(_T(">"),	_T("&gt;"));
                s.Replace(_T("\""),	_T("&quot;"));

                sResult += s;

				sResult += _T("</a:t>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_a_t;
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
					if ( _T("xml:space") == wsName )
					{
						m_oSpace = oReader.GetText();
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
			nullable<SimpleTypes::CXmlSpace<> > m_oSpace;

			// Value
			CString                             m_sText;

		};
		//--------------------------------------------------------------------------------
		// 21.1.2.3.9 rPr (Text Run Properties)
		//--------------------------------------------------------------------------------	
		class CRunProperty : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRunProperty)
			CRunProperty()
			{
			}
			virtual ~CRunProperty()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{

				// TO DO
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					
					if ( _T("a:blipFill") == sName )
					{
						m_oBlipFill = oReader;
						m_eFillType = filltypeBlip;
					}
					else if ( _T("a:gradFill") == sName )
					{
						m_oGradFill = oReader;
						m_eFillType = filltypeGradient;
					}
					else if ( _T("a:grpFill") == sName )
					{
						m_oGrpFill  = oReader;
						m_eFillType = filltypeGroup;
					}
					else if ( _T("a:noFill") == sName )
					{
						m_oNoFill   = oReader;
						m_eFillType = filltypeNo;
					}
					else if ( _T("a:pattFill") == sName )
					{
						m_oPattFill = oReader;
						m_eFillType = filltypePattern;
					}
					else if ( _T("a:solidFill") == sName )
					{
						m_oSolidFill = oReader;
						m_eFillType  = filltypeSolid;
					}
					else if ( _T("a:ln") == sName )
						m_oOutline = oReader;
					else if ( _T("a:cs") == sName )
						m_oComplexFont = oReader;					
					else if ( _T("a:ea") == sName )
						m_oAsianFont = oReader;				
					else if ( _T("a:latin") == sName )
						m_oLatinFont = oReader;			
					else if ( _T("a:sym") == sName )
						m_oSymbolFont = oReader;		
					else if ( _T("a:hlinkClick") == sName )
						m_oHlinkClick = oReader;			
					else if ( _T("a:effectDag") == sName )
						m_oEffectContainer = oReader;			
					else if ( _T("a:effectLst") == sName )
						m_oEffectList = oReader;			
					else if ( _T("a:extLst") == sName )
						m_oExtensionList = oReader;			
				}			

			}
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return et_a_rPr;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
					WritingElement_ReadAttributes_Read_if	  ( oReader, _T("b"),		m_oBold)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("i"),		m_oItalic)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("sz"),		m_oSz)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("u"),		m_oUnderline)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("baseline"),m_oBaseline)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("spc"),		m_oSpace)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("lang"),	m_oLanguage)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("kumimoji"),m_oKumimoji)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("dirty"),	m_oDirty)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("normalizeH"),	m_oNormalizeH)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("noProof"),	m_oNoProof)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("smtClean"),m_oSmtClean)
				WritingElement_ReadAttributes_End	( oReader )		
			}
		public:
			// Attributes
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>					m_oBold;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>					m_oItalic;	
			nullable<SimpleTypes::CDouble>											m_oSz;
			nullable<SimpleTypes::CUnderline<SimpleTypes::underlineNone>>			m_oUnderline;
			nullable<SimpleTypes::CPercentage>										m_oBaseline;
			nullable<SimpleTypes::CPoint>											m_oSpace;//на самом деле нужно - 20.1.10.74 ST_TextPoint (Text Point)
			nullable<SimpleTypes::CLang>											m_oLanguage;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>					m_oKumimoji;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>					m_oNormalizeH;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>					m_oNoProof;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>					m_oDirty;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>					m_oSmtClean;
			//bmk//string
			//cap//20.1.10.64
			//kern//20.1.10.73
			//smtId//unsignedInt
			//strike//20.1.10.79

			// Childs
			EFillType                                         m_eFillType;   // Тип заливки
			nullable<OOX::Drawing::CBlipFillProperties>       m_oBlipFill;
			nullable<OOX::Drawing::CGradientFillProperties>   m_oGradFill;
			nullable<OOX::Drawing::CGroupFillProperties>      m_oGrpFill;
			nullable<OOX::Drawing::CNoFillProperties>         m_oNoFill;
			nullable<OOX::Drawing::CPatternFillProperties>    m_oPattFill;
			nullable<OOX::Drawing::CSolidColorFillProperties> m_oSolidFill;
			nullable<OOX::Drawing::CLineProperties>			  m_oOutline;
			nullable<OOX::Drawing::CTextFont>				  m_oComplexFont;
			nullable<OOX::Drawing::CTextFont>				  m_oAsianFont;
			nullable<OOX::Drawing::CTextFont>				  m_oLatinFont;
			nullable<OOX::Drawing::CTextFont>				  m_oSymbolFont;
			nullable<OOX::Drawing::CHyperlink>				  m_oHlinkClick;
			nullable<OOX::Drawing::CEffectContainer>		  m_oEffectContainer;
			nullable<OOX::Drawing::CEffectList>				  m_oEffectList;
			nullable<OOX::Drawing::COfficeArtExtensionList>	  m_oExtensionList;
			
			//highlight (Highlight Color) §21.1.2.3.4
			//hlinkMouseOver (Mouse-Over Hyperlink) §21.1.2.3.6
			//rtl (Right to Left Run) §21.1.2.2.8
			//uFill (Underline Fill) §21.1.2.3.12
			//uFillTx (Underline Fill Properties Follow Text) §21.1.2.3.13
			//uLn (Underline Stroke) §21.1.2.3.14
			//uLnTx (Underline Follows Text) §21.1.2.3.15
		};

		//--------------------------------------------------------------------------------
		// 21.1.2.3.8  r (Text Run)
		//--------------------------------------------------------------------------------
		class CRun : public WritingElement
		{
		public:
			CRun()
			{
			}
			CRun(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CRun(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CRun()
			{
				Clear();
			}

		public:

			const CRun &operator =(const XmlUtils::CXmlNode& oNode)
			{
				Clear();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CRun &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				Clear();
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

			void Clear()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{

			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("a:rPr") == sName )
						m_oRunProperty= new CRunProperty( oReader );
					else if ( _T("a:t") == sName )
						m_oText= new CText( oReader );

				}
			}
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return et_a_r;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{

			}

		public:
			
			nullable<OOX::Drawing::CRunProperty >		m_oRunProperty;	
			nullable<OOX::Drawing::CText		>		m_oText;	

		};
		//--------------------------------------------------------------------------------
		// 21.1.2.2.1 br (Text Line Break)
		//--------------------------------------------------------------------------------
		class CTextLineBreak : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTextLineBreak)
			CTextLineBreak()
			{
			}
			virtual ~CTextLineBreak()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					
					if ( _T("a:rPr") == sName )
						m_oRunProperty = oReader;
				}
			}
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return et_a_br;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			// Childs
			nullable<CRunProperty>			m_oRunProperty;
		};
		//--------------------------------------------------------------------------------
		// 21.1.2.2.7 pPr (Text Paragraph Properties)
		//--------------------------------------------------------------------------------	
		class CParagraphProperty : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CParagraphProperty)
			CParagraphProperty()
			{
			}
			virtual ~CParagraphProperty()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{

				// TO DO
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					
					if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
					else if ( _T("a:lnSpc") == sName )
						m_oLineSpacing = oReader;
					else if ( _T("a:defRPr") == sName )
						m_oDefRunProperty = oReader;
					else if ( _T("a:spcAft") == sName )
						m_oAfterSpacing = oReader;
					else if ( _T("a:spcBef") == sName )
						m_oBeforeSpacing = oReader;
				}
			}
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return et_a_pPr;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
					WritingElement_ReadAttributes_Read_if	  ( oReader, _T("rtl"),		m_oRtl)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("lvl"),		m_oLvl)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("algn"),	m_oAlgn)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("fontAlgn"), m_oFontAlgn)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("marL"),	m_oMarR)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("marR"),	m_oMarL)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("indent"),	m_oIndent)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("defTabSz"),m_oDefTabSz)					
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			// Attributes
			nullable<SimpleTypes::COnOff<>>									m_oRtl;
			nullable<SimpleTypes::COnOff<>>									m_oHangingPunct;
			nullable<SimpleTypes::CDecimalNumber<> >						m_oLvl;
			nullable<SimpleTypes::COnOff<>>									m_oLatinLnBrk;
			nullable<SimpleTypes::CTextIndent<> >							m_oIndent;
			nullable<SimpleTypes::COnOff<>>									m_oEaLnBrk;
			nullable<SimpleTypes::CCoordinate32 >							m_oDefTabSz;
			nullable<SimpleTypes::CTextAlignmentType<>>						m_oAlgn;
			nullable<SimpleTypes::CTextFontAlignType<>>						m_oFontAlgn;
			nullable<SimpleTypes::CTextMargin<>>							m_oMarR;
			nullable<SimpleTypes::CTextMargin<>>							m_oMarL;
			
			// Childs
			nullable<CRunProperty>											m_oDefRunProperty;
			nullable<CLineSpacing>											m_oLineSpacing;
			nullable<CLineSpacing>											m_oAfterSpacing;
			nullable<CLineSpacing>											m_oBeforeSpacing;
			nullable<OOX::Drawing::COfficeArtExtensionList>					m_oExtLst;			

			//buAutoNum //(Auto-Numbered Bullet) §21.1.2.4.1
			//buBlip //(Picture Bullet) §21.1.2.4.2
			//buChar //(Character Bullet) §21.1.2.4.3
			//buClr //(Color Specified) §21.1.2.4.4
			//buClrTx //(Follow Text) §21.1.2.4.5
			//buFont //(Specified) §21.1.2.4.6
			//buFontTx //(Follow text) §21.1.2.4.7
			//buNone //(No Bullet) §21.1.2.4.8
			//buSzPct //(Bullet Size Percentage) §21.1.2.4.9
			//buSzPts //(Bullet Size Points) §21.1.2.4.10
			//buSzTx //(Bullet Size Follows Text) §21.1.2.4.11

			//tabLst //(Tab List) §21.1.2.2.14
		};

		//--------------------------------------------------------------------------------
		// 21.1.2.2.6 p (Text Paragraphs)
		//--------------------------------------------------------------------------------	
		class CParagraph : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CParagraph)
			CParagraph()
			{
			}
			virtual ~CParagraph()
			{
			}
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{

				// TO DO
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					WritingElement *pItem = NULL;

					 if ( _T("a:pPr") == sName )
						m_oParagraphProperty = oReader; 
					else if ( _T("a:r") == sName )
						m_arrItems.push_back( new CRun( oReader ));
					else if ( _T("a:br") == sName )
						m_arrItems.push_back( new CTextLineBreak( oReader ));
					//else if ( _T("a:fld") == sName )
					//	pItem = new CField( oReader );
					//else if ( _T("a:endParaRPr") == sName )
						//m_oEndParagraphProperty = oReader; 
				}
			}
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return et_a_p;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			nullable<OOX::Drawing::CParagraphProperty    > m_oParagraphProperty;
		};

	} // namespace Drawing
} // namespace OOX
