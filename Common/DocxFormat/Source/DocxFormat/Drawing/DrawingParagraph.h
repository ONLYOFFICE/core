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
						m_oExtLst = Reader;

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
			//defRPr //(Default Text Run Properties) §21.1.2.3.2
			nullable<OOX::Drawing::COfficeArtExtensionList     > m_oExtLst;			//(Extension List) §20.1.2.2.15
			//lnSpc //(Line Spacing) §21.1.2.2.5
			//spcAft// (Space After) §21.1.2.2.9
			//spcBef //(Space Before) §21.1.2.2.10
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
					CWCharWrapper sName = oReader.GetName();
					WritingElement *pItem = NULL;

					 if ( _T("a:pPr") == sName )
						m_arrItems.Add( new CParagraphProperty( oReader )); 
					else if ( _T("a:r") == sName )
						m_arrItems.Add( new CRun( oReader ));

					//	 if ( _T("a:br") == sName )
					//	pItem = new CBr( oReader );
					//else if ( _T("a:fld") == sName )
					//	pItem = new CField( oReader );
					//else if ( _T("a:endParaRPr") == sName )
					//	pItem = new CEndParaRPr( oReader );
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
		};

	} // namespace Drawing
} // namespace OOX
