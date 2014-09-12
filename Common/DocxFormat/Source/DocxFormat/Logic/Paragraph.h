#pragma once
#ifndef OOX_LOGIC_PARAGRAPH_INCLUDE_H_
#define OOX_LOGIC_PARAGRAPH_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../WritingElement.h"
#include "../../Common/SimpleTypes_Word.h"

#include "RunProperty.h"
#include "Run.h"
#include "Hyperlink.h"
#include "SmartTag.h"
#include "Dir.h"
#include "Bdo.h"
#include "../Math/oMathPara.h"

namespace OOX
{
	namespace Logic
	{
		class CParagraphProperty;
		//--------------------------------------------------------------------------------
		// CParagraph 17.3.1.22 (Part 1)
		//--------------------------------------------------------------------------------	
		class CParagraph : public WritingElementWithChilds<>
		{
		public:
			CParagraph()
			{
				m_oParagraphProperty = NULL;
			}
			CParagraph(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CParagraph(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CParagraph()
			{
				m_oParagraphProperty = NULL;
			}

		public:

			const CParagraph &operator =(const XmlUtils::CXmlNode& oNode)
			{
				Clear();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}

			const CParagraph &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				Clear();
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

		public:

			void Clear()
			{
				m_oRsidDel.reset();
				m_oRsidP.reset();
				m_oRsidR.reset();
				m_oRsidRDefault.reset();
				m_oRsidRPr.reset();
				
				for (unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}

				m_arrItems.clear();
				m_oParagraphProperty = NULL;
			}

			void AddRun(CRun *pRun);
			void AddText(CString& sText);
			void AddText(CString& sText, CRunProperty *pProperty);
			void AddTab();
			void AddTab(CRunProperty *pProperty);
			void AddBreak(SimpleTypes::EBrType eType);
			void AddSpace(const int nCount);
			void AddSpace(const int nCount, CRunProperty *pProperty);
			void AddBookmarkStart(int nId, CString& sName);
			void AddBookmarkEnd  (int nId);
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const;
			virtual EElementType getType() const
			{
				return et_w_p;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CLongHexNumber<> > m_oParaId;
			nullable<SimpleTypes::CLongHexNumber<> > m_oTextId;

			// Attributes
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidDel;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidP;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidR;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidRDefault;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidRPr;

			// Childs
			CParagraphProperty *					 m_oParagraphProperty; // копия того что в m_arrItems...  - для быстрого доступа/анализа
			// по идее нужно сделать как в Drawing::Paragraph - то есть единственные подобъекты вынести отдельно
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_PARAGRAPH_INCLUDE_H_
