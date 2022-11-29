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
#include "DocParts.h"
#include "Sdt.h"
#include "Paragraph.h"
#include "Annotations.h"
#include "Table.h"
#include "../Math/oMathPara.h"
#include "../Math/OMath.h"

namespace OOX
{
	namespace Logic
	{
		CDocParts::CDocParts(OOX::Document *pMain) : WritingElementWithChilds<CDocPart>(pMain)
		{
		}
		CDocParts::~CDocParts()
		{
		}
		void CDocParts::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CDocParts::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if (L"w:docPart" == sName)
				{
					CDocPart *pItem = new CDocPart(m_pMainDocument);
					m_arrItems.push_back(pItem);

					pItem->fromXML(oReader);
				}
			}
		}
		std::wstring CDocParts::toXML() const
		{
			std::wstring sResult = L"<w:docParts>";
			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					sResult += m_arrItems[i]->toXML();
				}
			}
			sResult += L"</w:docParts>";
			return sResult;
		}
		EElementType CDocParts::getType() const
		{
			return et_w_docParts;
		}

		CDocPart::CDocPart(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CDocPart::~CDocPart() {}
		void CDocPart::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		void CDocPart::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = oReader.GetName();

				if (L"w:docPartPr" == sName)
				{
					m_oDocPartPr = new CDocPartPr(m_pMainDocument);
					m_oDocPartPr->fromXML(oReader);
				}
				else if (L"w:docPartBody" == sName)
				{
					m_oDocPartBody = new CDocPartBody(m_pMainDocument);
					m_oDocPartBody->fromXML(oReader);
				}
			}
		}
		std::wstring CDocPart::toXML() const
		{
			std::wstring sResult = L"<w:docPart>";

			if (m_oDocPartPr.IsInit())
				sResult += m_oDocPartPr->toXML();

			if (m_oDocPartBody.IsInit())
				sResult += m_oDocPartBody->toXML();

			sResult += L"</w:docPart>";
			return sResult;
		}
		EElementType CDocPart::getType() const
		{
			return et_w_docPart;
		}

		CDocPartBody::CDocPartBody(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
		}
		void CDocPartBody::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNodes oChilds;
			if (oNode.GetNodes(L"*", oChilds))
			{
				XmlUtils::CXmlNode oItem;
				for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
				{
					if (oChilds.GetAt(nIndex, oItem))
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if (L"w:bookmarkEnd" == sName)
							pItem = new CBookmarkEnd(oItem);
						else if (L"w:bookmarkStart" == sName)
							pItem = new CBookmarkStart(oItem);
						else if (L"w:commentRangeEnd" == sName)
							pItem = new CCommentRangeEnd(oItem);
						else if (L"w:commentRangeStart" == sName)
							pItem = new CCommentRangeStart(oItem);
						//else if ( L"w:customXml" == sName )
						//	pItem = new CCustomXml( oItem );
						else if (L"w:customXmlDelRangeEnd" == sName)
							pItem = new CCustomXmlDelRangeEnd(oItem);
						else if (L"w:customXmlDelRangeStart" == sName)
							pItem = new CCustomXmlDelRangeStart(oItem);
						else if (L"w:customXmlInsRangeEnd" == sName)
							pItem = new CCustomXmlInsRangeEnd(oItem);
						else if (L"w:customXmlInsRangeStart" == sName)
							pItem = new CCustomXmlInsRangeStart(oItem);
						else if (L"w:customXmlMoveFromRangeEnd" == sName)
							pItem = new CCustomXmlMoveFromRangeEnd(oItem);
						else if (L"w:customXmlMoveFromRangeStart" == sName)
							pItem = new CCustomXmlMoveFromRangeStart(oItem);
						else if (L"w:customXmlMoveToRangeEnd" == sName)
							pItem = new CCustomXmlMoveToRangeEnd(oItem);
						else if (L"w:customXmlMoveToRangeStart" == sName)
							pItem = new CCustomXmlMoveToRangeStart(oItem);
						else if (L"w:del" == sName)
							pItem = new CDel(oItem);
						else if (L"w:ins" == sName)
							pItem = new CIns(oItem);
						else if (L"w:moveFrom" == sName)
							pItem = new CMoveFrom(oItem);
						else if (L"w:moveFromRangeEnd" == sName)
							pItem = new CMoveFromRangeEnd(oItem);
						else if (L"w:moveFromRangeStart" == sName)
							pItem = new CMoveFromRangeStart(oItem);
						else if (L"w:moveTo" == sName)
							pItem = new CMoveTo(oItem);
						else if (L"w:moveToRangeEnd" == sName)
							pItem = new CMoveToRangeEnd(oItem);
						else if (L"w:moveToRangeStart" == sName)
							pItem = new CMoveToRangeStart(oItem);
						else if (L"m:oMath" == sName)
							pItem = new COMath(oItem);
						else if (L"m:oMathPara" == sName)
							pItem = new COMathPara(oItem);
						else if (L"w:p" == sName)
							pItem = new CParagraph(oItem);
						else if (L"w:permEnd" == sName)
							pItem = new CPermEnd(oItem);
						else if (L"w:permStart" == sName)
							pItem = new CPermStart(oItem);
						else if (L"w:proofErr" == sName)
							pItem = new CProofErr(oItem);
						else if (L"w:sdt" == sName)
							pItem = new CSdt(oItem);
						else if (L"w:tbl" == sName)
							pItem = new CTbl(oItem);

						if (pItem)
							m_arrItems.push_back(pItem);
					}
				}
			}
		}
		void CDocPartBody::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if (L"w:bookmarkEnd" == sName)
					pItem = new CBookmarkEnd(oReader);
				else if (L"w:bookmarkStart" == sName)
					pItem = new CBookmarkStart(oReader);
				else if (L"w:commentRangeEnd" == sName)
					pItem = new CCommentRangeEnd(oReader);
				else if (L"w:commentRangeStart" == sName)
					pItem = new CCommentRangeStart(oReader);
				//else if ( L"w:customXml" == sName )
				//	pItem = new CCustomXml( oReader );
				else if (L"w:customXmlDelRangeEnd" == sName)
					pItem = new CCustomXmlDelRangeEnd(oReader);
				else if (L"w:customXmlDelRangeStart" == sName)
					pItem = new CCustomXmlDelRangeStart(oReader);
				else if (L"w:customXmlInsRangeEnd" == sName)
					pItem = new CCustomXmlInsRangeEnd(oReader);
				else if (L"w:customXmlInsRangeStart" == sName)
					pItem = new CCustomXmlInsRangeStart(oReader);
				else if (L"w:customXmlMoveFromRangeEnd" == sName)
					pItem = new CCustomXmlMoveFromRangeEnd(oReader);
				else if (L"w:customXmlMoveFromRangeStart" == sName)
					pItem = new CCustomXmlMoveFromRangeStart(oReader);
				else if (L"w:customXmlMoveToRangeEnd" == sName)
					pItem = new CCustomXmlMoveToRangeEnd(oReader);
				else if (L"w:customXmlMoveToRangeStart" == sName)
					pItem = new CCustomXmlMoveToRangeStart(oReader);
				else if (L"w:del" == sName)
					pItem = new CDel(oReader);
				else if (L"w:ins" == sName)
					pItem = new CIns(oReader);
				else if (L"w:moveFrom" == sName)
					pItem = new CMoveFrom(oReader);
				else if (L"w:moveFromRangeEnd" == sName)
					pItem = new CMoveFromRangeEnd(oReader);
				else if (L"w:moveFromRangeStart" == sName)
					pItem = new CMoveFromRangeStart(oReader);
				else if (L"w:moveTo" == sName)
					pItem = new CMoveTo(oReader);
				else if (L"w:moveToRangeEnd" == sName)
					pItem = new CMoveToRangeEnd(oReader);
				else if (L"w:moveToRangeStart" == sName)
					pItem = new CMoveToRangeStart(oReader);
				else if (L"m:oMath" == sName)
					pItem = new COMath(oReader);
				else if (L"m:oMathPara" == sName)
					pItem = new COMathPara(oReader);
				else if (L"w:p" == sName)
					pItem = new CParagraph(oReader);
				else if (L"w:permEnd" == sName)
					pItem = new CPermEnd(oReader);
				else if (L"w:permStart" == sName)
					pItem = new CPermStart(oReader);
				else if (L"w:proofErr" == sName)
					pItem = new CProofErr(oReader);
				else if (L"w:sdt" == sName)
					pItem = new CSdt(oReader);
				else if (L"w:tbl" == sName)
					pItem = new CTbl(oReader);

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
		std::wstring CDocPartBody::toXML() const
		{
			std::wstring sResult = L"<w:docPartBody>";

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					sResult += m_arrItems[i]->toXML();
				}
			}

			sResult += L"</w:docPartBody>";
			return sResult;
		}
		EElementType CDocPartBody::getType() const
		{
			return et_w_docPartBody;
		}

		CDocPartPr::CDocPartPr(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CDocPartPr::~CDocPartPr() {}
		void CDocPartPr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		void CDocPartPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if (L"w:name" == sName)
					m_oName = oReader;
				else if (L"w:description" == sName)
					m_oDescription = oReader;
				else if (L"w:guid" == sName)
					m_oGuid = oReader;
				else if (L"w:style" == sName)
					m_oStyle = oReader;
				else if ( L"w:category" == sName )
					m_oCategory = oReader;
				else if (L"w:types" == sName)
					m_oTypes = oReader;
				else if (L"w:behaviors" == sName)
					m_oBehaviors = oReader;
			}
		}
		std::wstring CDocPartPr::toXML() const
		{
			std::wstring sResult = L"<w:docPartPr>";

			if (m_oName.IsInit())			sResult += m_oName->ValNode(L"w:name");
			if (m_oDescription.IsInit())	sResult += m_oDescription->ValNode(L"w:description");
			if (m_oCategory.IsInit())		sResult += m_oCategory->toXML();
			if (m_oTypes.IsInit())			sResult += m_oTypes->toXML();
			if (m_oBehaviors.IsInit())		sResult += m_oBehaviors->toXML();
			if (m_oGuid.IsInit())			sResult += m_oGuid->ValNode(L"w:guid");

			sResult += L"</w:docPartPr>";
			return sResult;
		}
		EElementType CDocPartPr::getType() const
		{
			return et_w_docPartPr;
		}

		CDocPartCategory::CDocPartCategory(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CDocPartCategory::~CDocPartCategory() {}
		void CDocPartCategory::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		void CDocPartCategory::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if (L"w:name" == sName)
					m_oName = oReader;
				else if (L"w:gallery" == sName)
					m_oGallery = oReader;
			}
		}
		std::wstring CDocPartCategory::toXML() const
		{
			std::wstring sResult = L"<w:category>";

			if (m_oName.IsInit())		sResult += m_oName->ValNode(L"w:name");
			if (m_oGallery.IsInit())	sResult += m_oGallery->ValNode(L"w:gallery");

			sResult += L"</w:category>";
			return sResult;
		}
		EElementType CDocPartCategory::getType() const
		{
			return et_w_docPartCategory;
		}

		CDocPartBehaviors::CDocPartBehaviors(OOX::Document *pMain) : WritingElementWithChilds<ComplexTypes::Word::CDocPartBehavior>(pMain)
		{
		}
		CDocPartBehaviors::~CDocPartBehaviors() {}
		void CDocPartBehaviors::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();

				if (L"w:behavior" == sName)
				{
					m_arrItems.push_back(new ComplexTypes::Word::CDocPartBehavior(oReader));
				}
			}
		}
		void CDocPartBehaviors::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CDocPartBehaviors::toXML() const
		{
			std::wstring sResult = L"<w:behaviors>";

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					sResult += m_arrItems[i]->ValNode(L"w:behavior");
				}
			}

			sResult += L"</w:behaviors>";
			return sResult;
		}
		EElementType CDocPartBehaviors::getType() const
		{
			return et_w_docPartBehaviors;
		}

		CDocPartTypes::CDocPartTypes(OOX::Document *pMain) : WritingElementWithChilds<ComplexTypes::Word::String>(pMain)
		{
		}
		CDocPartTypes::~CDocPartTypes() {}
		void CDocPartTypes::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();

				if (L"w:type" == sName)
				{
					m_arrItems.push_back(new ComplexTypes::Word::String(oReader));
				}
			}
		}
		void CDocPartTypes::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CDocPartTypes::toXML() const
		{
			std::wstring sResult = L"<w:types>";

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					sResult += m_arrItems[i]->ValNode(L"w:type");
				}
			}

			sResult += L"</w:types>";
			return sResult;
		}
		EElementType CDocPartTypes::getType() const
		{
			return et_w_docPartTypes;
		}
		void CDocPartTypes::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_ReadSingle(oReader, L"w:all", m_oAll)
			WritingElement_ReadAttributes_End(oReader)
		}

	} // namespace Logic
} // namespace OOX
