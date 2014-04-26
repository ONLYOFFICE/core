#pragma once
#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

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
					sResult.Format( _T("<a:t xml:space=\"%s\">"), m_oSpace->ToString() );
				else
					sResult = _T("<a:t>");

				sResult += m_sText;
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

				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
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
					CWCharWrapper sName = oReader.GetName();
					
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
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("b"),		m_oBold)
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("i"),		m_oItalic)
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("sz"),	m_oSz)
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("u"),		m_oUnderline)
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			// Attributes
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>					m_oBold;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>					m_oItalic;	
			nullable<SimpleTypes::CDouble>											m_oSz;
			nullable<SimpleTypes::CUnderline<SimpleTypes::underlineNone>>			m_oUnderline;
			
			//baseline //20.1.10.40
			//bmk//string
			//cap//20.1.10.64
			//dirty//bool
			//kern//20.1.10.73
			//kumimoji//bool
			//lang//22.9.2.6
			//noProof//bool
			//normalizeH//bool
			//smtClean//bool
			//smtId//unsignedInt
			//spc//20.1.10.74
			//strike//20.1.10.79

			// Childs
			EFillType                                         m_eFillType;   // “ËÔ Á‡ÎË‚ÍË
			nullable<OOX::Drawing::CBlipFillProperties>       m_oBlipFill;
			nullable<OOX::Drawing::CGradientFillProperties>   m_oGradFill;
			nullable<OOX::Drawing::CGroupFillProperties>      m_oGrpFill;
			nullable<OOX::Drawing::CNoFillProperties>         m_oNoFill;
			nullable<OOX::Drawing::CPatternFillProperties>    m_oPattFill;
			nullable<OOX::Drawing::CSolidColorFillProperties> m_oSolidFill;

			//cs (Complex Script Font) ß21.1.2.3.1
			//ea (East Asian Font) ß21.1.2.3.3
			//effectDag (Effect Container) ß20.1.8.25
			//effectLst (Effect Container) ß20.1.8.26
			//extLst (Extension List) ß20.1.2.2.15
			//highlight (Highlight Color) ß21.1.2.3.4
			//hlinkClick (Click Hyperlink) ß21.1.2.3.5
			//hlinkMouseOver (Mouse-Over Hyperlink) ß21.1.2.3.6
			//latin (Latin Font) ß21.1.2.3.7
			//ln (Outline) ß20.1.2.2.24
			//rtl (Right to Left Run) ß21.1.2.2.8
			//sym (Symbol Font) ß21.1.2.3.10
			//uFill (Underline Fill) ß21.1.2.3.12
			//uFillTx (Underline Fill Properties Follow Text) ß21.1.2.3.13
			//uLn (Underline Stroke) ß21.1.2.3.14
			//uLnTx (Underline Follows Text) ß21.1.2.3.15
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
					CWCharWrapper sName = oReader.GetName();
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
					CWCharWrapper sName = oReader.GetName();
					
					if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;

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
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("rtl"), m_oRtl)
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("lvl"), m_oLvl)
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("algn"), m_oAlgn)
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("fontAlgn"), m_oFontAlgn)
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("marL"), m_oMarR)
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("marR"), m_oMarL)
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			// Attributes
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>          m_oRtl;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>			m_oHangingPunct;
			nullable<SimpleTypes::CDecimalNumber<> >						m_oLvl;
			nullable<SimpleTypes::CDecimalNumber<> >						m_oLatinLnBrk;
			nullable<SimpleTypes::CDecimalNumber<> >						m_oIndent;
			nullable<SimpleTypes::CDecimalNumber<> >						m_oEaLnBrk;
			nullable<SimpleTypes::CDecimalNumber<> >						m_oDefTabSz;
			nullable<SimpleTypes::CTextAlignmentType<>>						m_oAlgn;
			nullable<SimpleTypes::CTextFontAlignType<>>						m_oFontAlgn;
			nullable<SimpleTypes::CCoordinate32>							m_oMarR;
			nullable<SimpleTypes::CCoordinate32>							m_oMarL;

			// Childs
			//buAutoNum //(Auto-Numbered Bullet) ß21.1.2.4.1
			//buBlip //(Picture Bullet) ß21.1.2.4.2
			//buChar //(Character Bullet) ß21.1.2.4.3
			//buClr //(Color Specified) ß21.1.2.4.4
			//buClrTx //(Follow Text) ß21.1.2.4.5
			//buFont //(Specified) ß21.1.2.4.6
			//buFontTx //(Follow text) ß21.1.2.4.7
			//buNone //(No Bullet) ß21.1.2.4.8
			//buSzPct //(Bullet Size Percentage) ß21.1.2.4.9
			//buSzPts //(Bullet Size Points) ß21.1.2.4.10
			//buSzTx //(Bullet Size Follows Text) ß21.1.2.4.11
			//defRPr //(Default Text Run Properties) ß21.1.2.3.2
			//lnSpc //(Line Spacing) ß21.1.2.2.5
			//spcAft// (Space After) ß21.1.2.2.9
			//spcBef //(Space Before) ß21.1.2.2.10
			//tabLst //(Tab List) ß21.1.2.2.14
			nullable<OOX::Drawing::COfficeArtExtensionList     > m_oExtLst;			//(Extension List) ß20.1.2.2.15
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
					CWCharWrapper sName = oReader.GetName();
					WritingElement *pItem = NULL;

					 if ( _T("a:pPr") == sName )
						m_oParagraphProperty = oReader; 
					else if ( _T("a:r") == sName )
						m_arrItems.Add( new CRun( oReader ));
					//	 if ( _T("a:br") == sName )
					//	pItem = new CBr( oReader );
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
