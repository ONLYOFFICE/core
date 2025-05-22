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
#include "Vsdx.h"
#include "VisioDocument.h"
#include "VisioPages.h"
#include "VisioConnections.h"
#include "VisioOthers.h"
#include "Shapes.h"
#include "../PPTXFormat/Theme.h"
#include "../../DesktopEditor/common/SystemUtils.h"
#include "../Binary/Presentation/BinaryFileReaderWriter.h"
#include "../Binary/Presentation/XmlWriter.h"

namespace OOX
{
namespace Draw
{
	EElementType CPublishedPage::getType() const
	{
		return et_dr_PublishedPage;
	}
	void CPublishedPage::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CPublishedPage::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
	}
	void CPublishedPage::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ID);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CPublishedPage::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				ID = pReader->GetULong();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CPublishedPage::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"PublishedPage");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"PublishedPage");
	}
	EElementType CRefreshableData::getType() const
	{
		return et_dr_RefreshableData;
	}
	void CRefreshableData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CRefreshableData::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
	}
	void CRefreshableData::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ID);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CRefreshableData::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				ID = pReader->GetULong();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CRefreshableData::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"RefreshableData");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"RefreshableData");
	}
	EElementType CPublishSettings::getType() const
	{
		return et_dr_PublishSettings;
	}
	void CPublishSettings::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			WritingElement* pItem = NULL;
			if (L"PublishedPage" == sName)
			{
				pItem = new CPublishedPage();
			}
			else if (L"RefreshableData" == sName)
			{
				pItem = new CRefreshableData();
			}
			if (pItem)
			{
				pItem->fromXML(oReader);
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CPublishSettings::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			int type = 0xff;					 //todooo predefine type for ???
			switch (m_arrItems[i]->getType())
			{
			case et_dr_PublishedPage: type = 0; break;
			case et_dr_RefreshableData: type = 1; break;
			}
			if (type != 0xff)
				pWriter->WriteRecord2(type, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
		}
	}
	void CPublishSettings::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CPublishedPage());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			case 1:
			{
				m_arrItems.push_back(new CRefreshableData());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CPublishSettings::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"PublishSettings");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"PublishSettings");
	}
//----------------------------------------------------------------------------------------------------------------
	EElementType CHeaderFooter::getType() const
	{
		return et_dr_HeaderFooter;
	}
	void CHeaderFooter::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "HeaderMargin", HeaderMargin)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "FooterMargin", FooterMargin)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "HeaderLeft", HeaderLeft)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "HeaderCenter", HeaderCenter)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "HeaderRight", HeaderRight)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "FooterLeft", FooterLeft)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "FooterCenter", FooterCenter)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "FooterRight", FooterRight)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CHeaderFooter::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
	}
	void CHeaderFooter::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, HeaderMargin);
		pWriter->WriteString2(1, FooterMargin);
		pWriter->WriteString2(2, HeaderLeft);
		pWriter->WriteString2(3, HeaderCenter);
		pWriter->WriteString2(4, HeaderRight);
		pWriter->WriteString2(5, FooterLeft);
		pWriter->WriteString2(6, FooterCenter);
		pWriter->WriteString2(7, FooterRight);
		pWriter->WriteString2(8, HeaderFooterColor);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CHeaderFooter::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				HeaderMargin = pReader->GetString2();
			}break;
			case 1:
			{
				FooterMargin = pReader->GetString2();
			}break;
			case 2:
			{
				HeaderLeft = pReader->GetString2();
			}break;
			case 3:
			{
				HeaderCenter = pReader->GetString2();
			}break;
			case 4:
			{
				HeaderRight = pReader->GetString2();
			}break;
			case 5:
			{
				FooterLeft = pReader->GetString2();
			}break;
			case 6:
			{
				FooterCenter = pReader->GetString2();
			}break;
			case 7:
			{
				FooterRight = pReader->GetString2();
			}break;
			case 8:
			{
				HeaderFooterColor = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CHeaderFooter::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"HeaderFooter");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"HeaderMargin", HeaderMargin);
		pWriter->WriteAttribute2(L"FooterMargin", FooterMargin);
		pWriter->WriteAttribute2(L"HeaderLeft", HeaderLeft);
		pWriter->WriteAttribute2(L"HeaderCenter", HeaderCenter);
		pWriter->WriteAttribute2(L"HeaderRight", HeaderRight);
		pWriter->WriteAttribute2(L"FooterLeft", FooterLeft);
		pWriter->WriteAttribute2(L"FooterCenter", FooterCenter);
		pWriter->WriteAttribute2(L"FooterRight", FooterRight);
		pWriter->WriteAttribute2(L"HeaderFooterColor", HeaderFooterColor);
		pWriter->EndAttributes();
// todooo color, font
		pWriter->WriteNodeEnd(L"HeaderFooter");
	}
	EElementType CDocumentSheet::getType() const
	{
		return et_dr_DocumentSheet;
	}
	void CDocumentSheet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "UniqueID", UniqueID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "NameU", NameU)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Name", Name)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "IsCustomName", IsCustomName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "IsCustomNameU", IsCustomNameU)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "LineStyle", LineStyle)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "FillStyle", FillStyle)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "TextStyle", TextStyle)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CDocumentSheet::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			WritingElement* pItem = NULL;
			if (L"Cell" == sName)
			{
				pItem = new CCell();
			}
			else if (L"Trigger" == sName)
			{
				pItem = new CTrigger();
			}
			else if (L"Section" == sName)
			{
				pItem = new CSection();
			}
			if (pItem)
			{
				pItem->fromXML(oReader);
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CDocumentSheet::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, UniqueID);
		pWriter->WriteString2(1, NameU);
		pWriter->WriteString2(2, Name);
		pWriter->WriteBool2(3, IsCustomName);
		pWriter->WriteBool2(4, IsCustomNameU);
		pWriter->WriteUInt2(5, LineStyle);
		pWriter->WriteUInt2(6, FillStyle);
		pWriter->WriteUInt2(7, TextStyle);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			int type = 0xff;					 //todooo predefine type for ???
			switch (m_arrItems[i]->getType())
			{
				case et_dr_Cell: type = 0; break;
				case et_dr_Trigger: type = 1; break;
				case et_dr_Section: type = 2; break;
			}
			if (type != 0xff)
				pWriter->WriteRecord2(type, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
		}
	}
	void CDocumentSheet::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				UniqueID = pReader->GetString2();
			}break;
			case 1:
			{
				NameU = pReader->GetString2();
			}break;
			case 2:
			{
				Name = pReader->GetString2();
			}break;
			case 3:
			{
				IsCustomName = pReader->GetBool();
			}break;
			case 4:
			{
				IsCustomNameU = pReader->GetBool();
			}break;
			case 5:
			{
				LineStyle = pReader->GetULong();
			}break;
			case 6:
			{
				FillStyle = pReader->GetULong();
			}break;
			case 7:
			{
				TextStyle = pReader->GetULong();
			}break;
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CCell());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			case 1:
			{
				m_arrItems.push_back(new CTrigger());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			case 2:
			{
				m_arrItems.push_back(new CSection());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CDocumentSheet::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"DocumentSheet");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"NameU", NameU);
		pWriter->WriteAttribute2(L"Name", Name);
		pWriter->WriteAttribute(L"IsCustomName", IsCustomName);
		pWriter->WriteAttribute(L"IsCustomNameU", IsCustomNameU);
		pWriter->WriteAttribute2(L"UniqueID", UniqueID);
		pWriter->WriteAttribute2(L"LineStyle", LineStyle);
		pWriter->WriteAttribute2(L"FillStyle", FillStyle);
		pWriter->WriteAttribute2(L"TextStyle", TextStyle);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"DocumentSheet");
	}
	EElementType CEventItem::getType() const
	{
		return et_dr_EventItem;
	}
	void CEventItem::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Action", Action)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "EventCode", EventCode)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Enabled", Enabled)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Target", Target)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "TargetArgs", TargetArgs)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CEventItem::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
	}
	void CEventItem::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ID);
		pWriter->WriteUInt2(1, Action);
		pWriter->WriteUInt2(2, EventCode);
		pWriter->WriteBool2(3, Enabled);
		pWriter->WriteString2(4, Target);
		pWriter->WriteString2(5, TargetArgs);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CEventItem::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				ID = pReader->GetULong();
			}break;
			case 1:
			{
				Action = pReader->GetULong();
			}break;
			case 2:
			{
				EventCode = pReader->GetULong();
			}break;
			case 3:
			{
				Enabled = pReader->GetBool();
			}break;
			case 4:
			{
				Target = pReader->GetString2();
			}break;
			case 5:
			{
				TargetArgs = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CEventItem::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"EventItem");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		pWriter->WriteAttribute2(L"Action", Action);
		pWriter->WriteAttribute2(L"EventCode", EventCode);
		pWriter->WriteAttribute(L"Enabled", Enabled);
		pWriter->WriteAttribute2(L"Target", Target);
		pWriter->WriteAttribute2(L"TargetArgs", TargetArgs);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"EventItem");
	}
	void CEventList::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"EventItem" == sName)
			{
				CEventItem* pItem = new CEventItem();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	EElementType CEventList::getType() const
	{
		return et_dr_EventList;
	}
	void CEventList::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CEventItem());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}
	void CEventList::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CEventList::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"EventList");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"EventList");
	}
	EElementType CStyleSheet::getType() const
	{
		return et_dr_StyleSheet;
	}
	void CStyleSheet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "NameU", NameU)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Name", Name)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "IsCustomName", IsCustomName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "IsCustomNameU", IsCustomNameU)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "LineStyle", LineStyle)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "FillStyle", FillStyle)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "TextStyle", TextStyle)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CStyleSheet::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			WritingElement* pItem = NULL;
			if (L"Cell" == sName)
			{
				pItem = new CCell();
			}
			else if (L"Trigger" == sName)
			{
				pItem = new CTrigger();
			}
			else if (L"Section" == sName)
			{
				pItem = new CSection();
			}
			if (pItem)
			{
				pItem->fromXML(oReader);
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CStyleSheet::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ID);
		pWriter->WriteString2(1, NameU);
		pWriter->WriteString2(2, Name);
		pWriter->WriteBool2(3, IsCustomName);
		pWriter->WriteBool2(4, IsCustomNameU);
		pWriter->WriteUInt2(5, LineStyle);
		pWriter->WriteUInt2(6, FillStyle);
		pWriter->WriteUInt2(7, TextStyle);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			int type = 0xff;					 //todooo predefine type for ???
			switch (m_arrItems[i]->getType())
			{
				case et_dr_Cell: type = 0; break;
				case et_dr_Trigger: type = 1; break;
				case et_dr_Section: type = 2; break;
			}
			if (type != 0xff)
				pWriter->WriteRecord2(type, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
		}
	}
	void CStyleSheet::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				ID = pReader->GetULong();
			}break;
			case 1:
			{
				NameU = pReader->GetString2();
			}break;
			case 2:
			{
				Name = pReader->GetString2();
			}break;
			case 3:
			{
				IsCustomName = pReader->GetBool();
			}break;
			case 4:
			{
				IsCustomNameU = pReader->GetBool();
			}break;
			case 5:
			{
				LineStyle = pReader->GetULong();
			}break;
			case 6:
			{
				FillStyle = pReader->GetULong();
			}break;
			case 7:
			{
				TextStyle = pReader->GetULong();
			}break;
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CCell());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			case 1:
			{
				m_arrItems.push_back(new CTrigger());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			case 2:
			{
				m_arrItems.push_back(new CSection());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CStyleSheet::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"StyleSheet");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		pWriter->WriteAttribute2(L"NameU", NameU);
		pWriter->WriteAttribute2(L"Name", Name);
		pWriter->WriteAttribute(L"IsCustomName", IsCustomName);
		pWriter->WriteAttribute(L"IsCustomNameU", IsCustomNameU);
		pWriter->WriteAttribute2(L"LineStyle", LineStyle);
		pWriter->WriteAttribute2(L"FillStyle", FillStyle);
		pWriter->WriteAttribute2(L"TextStyle", TextStyle);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"StyleSheet");
	}
	void CStyleSheets::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"StyleSheet" == sName)
			{
				CStyleSheet* pItem = new CStyleSheet();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	EElementType CStyleSheets::getType() const
	{
		return et_dr_StyleSheets;
	}
	void CStyleSheets::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CStyleSheet());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}
	void CStyleSheets::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CStyleSheets::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"StyleSheets");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"StyleSheets");
	}
	EElementType CColorEntry::getType() const
	{
		return et_dr_ColorEntry;
	}
	void CColorEntry::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "IX", IX)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "RGB", RGB)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CColorEntry::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
	}
	void CColorEntry::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, IX);
		pWriter->WriteString2(1, RGB);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CColorEntry::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				IX = pReader->GetULong();
			}break;
			case 1:
			{
				RGB = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CColorEntry::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"ColorEntry");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"IX", IX);
		pWriter->WriteAttribute2(L"RGB", RGB);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"ColorEntry");
	}
	void CColors::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"ColorEntry" == sName)
			{
				CColorEntry* pItem = new CColorEntry();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	EElementType CColors::getType() const
	{
		return et_dr_Colors;
	}
	void CColors::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CColorEntry());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}
	void CColors::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CColors::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Colors");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Colors");
	}
	EElementType CFaceName::getType() const
	{
		return et_dr_FaceName;
	}
	void CFaceName::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "NameU", NameU)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "UnicodeRanges", UnicodeRanges)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "CharSets", CharSets)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Panos", Panos)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Panose", Panose)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Flags", Flags)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CFaceName::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
	}
	void CFaceName::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, NameU);
		pWriter->WriteString2(1, UnicodeRanges);
		pWriter->WriteString2(2, CharSets);
		pWriter->WriteString2(3, Panos);
		pWriter->WriteString2(4, Panose);
		pWriter->WriteUInt2(5, Flags);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CFaceName::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
				case 0:
				{
					NameU = pReader->GetString2();
				}break;
		
				case 1:
				{
					UnicodeRanges = pReader->GetString2();
				}break;
				case 2:
				{
					CharSets = pReader->GetString2();
				}break;	
				case 3:
				{
					Panos = pReader->GetString2();
				}break;
				case 4:
				{
					Panose = pReader->GetString2();
				}break;

				case 5:
				{
					Flags = pReader->GetULong();
				}break;		
			}
		}
		pReader->Seek(_end_rec);
	}
	void CFaceName::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"FaceName");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"NameU", NameU);
		pWriter->WriteAttribute2(L"UnicodeRanges", UnicodeRanges);
		pWriter->WriteAttribute2(L"CharSets", CharSets);
		pWriter->WriteAttribute2(L"Panos", Panos);
		pWriter->WriteAttribute2(L"Panose", Panose);
		pWriter->WriteAttribute2(L"Flags", Flags);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"FaceName");
	}
	void CFaceNames::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"FaceName" == sName)
			{
				CFaceName* pItem = new CFaceName();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	EElementType CFaceNames::getType() const
	{
		return et_dr_FaceNames;
	}
	void CFaceNames::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CFaceName());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}
	void CFaceNames::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CFaceNames::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"FaceNames");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"FaceNames");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CDocumentSettings::getType() const
	{
		return et_dr_DocumentSettings;
	}
	void CDocumentSettings::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "TopPage", TopPage)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "DefaultTextStyle", DefaultTextStyle)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "DefaultLineStyle", DefaultLineStyle)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "DefaultFillStyle", DefaultFillStyle)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "DefaultGuideStyle", DefaultGuideStyle)
		WritingElement_ReadAttributes_EndChar_No_NS( oReader )
	}
	void CDocumentSettings::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
		if (oReader.IsEmptyNode())
			return;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			const char* sName = XmlUtils::GetNameNoNS(oReader.GetNameChar());
			if (strcmp("GlueSettings", sName) == 0)
			{
				GlueSettings = oReader.GetText();
			}
			else if (strcmp("SnapSettings", sName) == 0)
			{
				SnapSettings = oReader.GetText();
			}
			else if (strcmp("SnapExtensions", sName) == 0)
			{
				SnapExtensions = oReader.GetText();
			}
			else if (strcmp("SnapAngles", sName) == 0)
			{
				SnapAngles = oReader.GetText();
			}
			else if (strcmp("DynamicGridEnabled", sName) == 0)
			{
				DynamicGridEnabled = oReader.GetText();
			}
			else if (strcmp("ProtectStyles", sName) == 0)
			{
				ProtectStyles = oReader.GetText();
			}
			else if (strcmp("ProtectShapes", sName) == 0)
			{
				ProtectShapes = oReader.GetText();
			}
			else if (strcmp("ProtectBkgnds", sName) == 0)
			{
				ProtectBkgnds = oReader.GetText();
			}
			else if (strcmp("ProtectMasters", sName) == 0)
			{
				ProtectMasters = oReader.GetText();
			}
			else if (strcmp("CustomMenusFile", sName) == 0)
			{
				CustomMenusFile = oReader.GetText();
			}
			else if (strcmp("CustomToolbarsFile", sName) == 0)
			{
				CustomToolbarsFile = oReader.GetText();
			}
			else if (strcmp("AttachedToolbars", sName) == 0)
			{
				AttachedToolbars = oReader.GetText();
			}
		}
	}
	void CDocumentSettings::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, TopPage);
			pWriter->WriteUInt2(1, DefaultTextStyle);
			pWriter->WriteUInt2(2, DefaultLineStyle);
			pWriter->WriteUInt2(3, DefaultFillStyle);
			pWriter->WriteUInt2(4, DefaultGuideStyle);		
			pWriter->WriteInt2(5, GlueSettings);
			pWriter->WriteInt2(6, SnapSettings);
			pWriter->WriteInt2(7, SnapExtensions);
			pWriter->WriteInt2(8, SnapAngles);
			pWriter->WriteBool2(9, DynamicGridEnabled);
			pWriter->WriteBool2(10, ProtectStyles);
			pWriter->WriteBool2(11, ProtectShapes);
			pWriter->WriteBool2(12, ProtectBkgnds);
			pWriter->WriteBool2(13, ProtectMasters);
			pWriter->WriteString2(14, CustomMenusFile);
			pWriter->WriteString2(15, CustomToolbarsFile);
			pWriter->WriteString2(16, AttachedToolbars);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CDocumentSettings::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				TopPage = pReader->GetULong();
			}break;
			case 1:
			{
				DefaultTextStyle = pReader->GetULong();
			}break;
			case 2:
			{
				DefaultLineStyle = pReader->GetULong();
			}break;
			case 3:
			{
				DefaultFillStyle = pReader->GetULong();
			}break;
			case 4:
			{
				DefaultGuideStyle = pReader->GetULong();
			}break;
			case 5:
			{
				GlueSettings = pReader->GetLong();
			}break;
			case 6:
			{
				SnapSettings = pReader->GetLong();
			}break;
			case 7:
			{
				SnapExtensions = pReader->GetLong();
			}break;
			case 8:
			{
				SnapAngles = pReader->GetLong();
			}break;
			case 9:
			{
				DynamicGridEnabled = pReader->GetBool();
			}break;
			case 10:
			{
				ProtectStyles = pReader->GetBool();
			}break;
			case 11:
			{
				ProtectShapes = pReader->GetBool();
			}break;
			case 12:
			{
				ProtectBkgnds = pReader->GetBool();
			}break;
			case 13:
			{
				ProtectMasters = pReader->GetBool();
			}break;
			case 14:
			{
				CustomMenusFile = pReader->GetString2();
			}break;
			case 15:
			{
				CustomToolbarsFile = pReader->GetString2();
			}break;
			case 16:
			{
				AttachedToolbars = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CDocumentSettings::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"DocumentSettings");
		pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"TopPage", TopPage);
			pWriter->WriteAttribute2(L"DefaultTextStyle", DefaultTextStyle);
			pWriter->WriteAttribute2(L"DefaultLineStyle", DefaultLineStyle);
			pWriter->WriteAttribute2(L"DefaultFillStyle", DefaultFillStyle);
			pWriter->WriteAttribute2(L"DefaultGuideStyle", DefaultGuideStyle);
		pWriter->EndAttributes();
	//todooo
		pWriter->WriteNodeEnd(L"DocumentSettings");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	OOX::Draw::CDocumentFile::CDocumentFile(OOX::Document* pMain) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bMacroEnabled = false;
		m_bVisioPages = true;
	}
	OOX::Draw::CDocumentFile::CDocumentFile(OOX::Document* pMain, const CPath& uri) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bMacroEnabled = false;
		m_bVisioPages = true;
		
		read(uri.GetDirectory(), uri);
	}
	OOX::Draw::CDocumentFile::CDocumentFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::IFileContainer(pMain), OOX::File(pMain)
	{
		m_bMacroEnabled = false;
		m_bVisioPages = true;

		read(oRootPath, oPath);
	}
	OOX::Draw::CDocumentFile::~CDocumentFile()
	{
	}
	void OOX::Draw::CDocumentFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void OOX::Draw::CDocumentFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		IFileContainer::Read(oRootPath, oFilePath);

		smart_ptr<OOX::File> pFile = this->Find(OOX::FileTypes::VbaProject);
		if (pFile.IsInit())
		{
			m_bMacroEnabled = true;

		}
		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_strFilename = oFilePath.GetPath();

		std::wstring sName = oReader.GetName();
		if (L"VisioDocument" == sName && !oReader.IsEmptyNode())
		{
			int nDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nDepth))
			{
				sName = oReader.GetName();

				if (L"DocumentSettings" == sName)
				{
					DocumentSettings = oReader;
				}
				else if (L"Colors" == sName)
				{
					Colors = oReader;
				}
				else if (L"FaceNames" == sName)
				{
					FaceNames = oReader;
				}
				else if (L"StyleSheets" == sName)
				{
					StyleSheets = oReader;
				}
				else if (L"DocumentSheet" == sName)
				{
					DocumentSheet = oReader;
				}
				else if (L"EventList" == sName)
				{
					EventList = oReader;
				}
				else if (L"HeaderFooter" == sName)
				{
					HeaderFooter = oReader;
				}
			}
		}
	}
	void OOX::Draw::CDocumentFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_VSDX;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
		IFileContainer::Write(oFilePath, oDirectory, oContent);
	}
	const OOX::FileType OOX::Draw::CDocumentFile::type() const
	{
		if (m_bMacroEnabled)	return OOX::Draw::FileTypes::DocumentMacro;
		else					return OOX::Draw::FileTypes::Document;
	}
	const OOX::CPath OOX::Draw::CDocumentFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath OOX::Draw::CDocumentFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void OOX::Draw::CDocumentFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		pReader->Skip(1); // type
		
		smart_ptr<OOX::IFileContainer> rels_old = pReader->GetRels();
		pReader->SetRels(dynamic_cast<OOX::IFileContainer*>((CDocumentFile*)this));

		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				DocumentSettings.Init();
				DocumentSettings->fromPPTY(pReader);
			}break;
			case 1:
			{
				Colors.Init();
				Colors->fromPPTY(pReader);
			}break;
			case 2:
			{
				FaceNames.Init();
				FaceNames->fromPPTY(pReader);
			}break;
			case 3:
			{
				StyleSheets.Init();
				StyleSheets->fromPPTY(pReader);
			}break;
			case 4:
			{
				DocumentSheet.Init();
				DocumentSheet->fromPPTY(pReader);
			}break;
			case 5:
			{
				EventList.Init();
				EventList->fromPPTY(pReader);
			}break;
			case 6:
			{
				HeaderFooter.Init();
				HeaderFooter->fromPPTY(pReader);
			}break;
			case 7:
			{
				CMastersFile* pMaster = new CMastersFile(((OOX::File*)this)->m_pMainDocument);
				pMaster->fromPPTY(pReader);
				smart_ptr<OOX::File> oFile(pMaster);

				pReader->GetRels()->Add(oFile);
			}break;
			case 8:
			{
				CPagesFile* pPage = new CPagesFile(((OOX::File*)this)->m_pMainDocument);
				pPage->fromPPTY(pReader);
				smart_ptr<OOX::File> oFile(pPage);

				pReader->GetRels()->Add(oFile);
			}break;
			case 9:
			{
				CConnectionsFile* pConnections = new CConnectionsFile(((OOX::File*)this)->m_pMainDocument);
				pConnections->fromPPTY(pReader);
				smart_ptr<OOX::File> oFile(pConnections);

				pReader->GetRels()->Add(oFile);
			}break;
			case 10:
			{
				CRecordsetsFile* pRecordsets = new CRecordsetsFile(((OOX::File*)this)->m_pMainDocument);
				pRecordsets->fromPPTY(pReader);
				smart_ptr<OOX::File> oFile(pRecordsets);

				pReader->GetRels()->Add(oFile);
			}break;
			case 11:
			{
				CSolutionsFile* pSolutions = new CSolutionsFile(((OOX::File*)this)->m_pMainDocument);
				pSolutions->fromPPTY(pReader);
				smart_ptr<OOX::File> oFile(pSolutions);

				pReader->GetRels()->Add(oFile);
			}break;
			case 12:
			{
				CValidationFile* pValidation = new CValidationFile(((OOX::File*)this)->m_pMainDocument);
				pValidation->fromPPTY(pReader);
				smart_ptr<OOX::File> oFile(pValidation);

				pReader->GetRels()->Add(oFile);
			}break;
			case 13:
			{
				CCommentsFile* pComments = new CCommentsFile(((OOX::File*)this)->m_pMainDocument);
				pComments->fromPPTY(pReader);
				smart_ptr<OOX::File> oFile(pComments);

				pReader->GetRels()->Add(oFile);
			}break;
			case 14:
			{
				CWindowsFile* pWindows = new CWindowsFile(((OOX::File*)this)->m_pMainDocument);
				pWindows->fromPPTY(pReader);
				smart_ptr<OOX::File> oFile(pWindows);

				pReader->GetRels()->Add(oFile);
			}break;
			case 15:
			{
				pReader->Skip(4); // len

				PPTX::Theme* pTheme = new PPTX::Theme(((OOX::File*)this)->m_pMainDocument);
				pTheme->fromPPTY(pReader);
				smart_ptr<OOX::File> oFile(pTheme);

				pReader->GetRels()->Add(oFile);
			}break;
			case 16:
			{
				if (m_bMacroEnabled)
				{
					OOX::VbaProject* pVbaProject = new OOX::VbaProject(((OOX::File*)this)->m_pMainDocument);
					pVbaProject->fromPPTY(pReader);
					smart_ptr<OOX::File> oFile(pVbaProject);

					pReader->GetRels()->Add(oFile);
				}
				else
				{
					pReader->SkipRecord();
				}
			}break;
			case 17:
			{
				_INT32 _len = pReader->GetRecordSize();

				BYTE* pData = pReader->GetPointer(_len);
				OOX::CPath oJsaProject = OOX::FileTypes::JsaProject.DefaultFileName();
				std::wstring filePath = pReader->m_strFolder + FILE_SEPARATOR_STR + _T("visio") + FILE_SEPARATOR_STR + oJsaProject.GetPath();

				NSFile::CFileBinary oBinaryFile;
				oBinaryFile.CreateFileW(filePath);
				oBinaryFile.WriteFile(pData, _len);
				oBinaryFile.CloseFile();

				smart_ptr<OOX::JsaProject> oFileJsaProject(new OOX::JsaProject(NULL));
				smart_ptr<OOX::File> oFile = oFileJsaProject.smart_dynamic_cast<OOX::File>();
				pReader->GetRels()->Add(oFile);

				pReader->m_pRels->m_pManager->m_pContentTypes->AddDefault(oJsaProject.GetExtention(false));
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
		pReader->SetRels(rels_old);
	}
	void OOX::Draw::CDocumentFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->StartRecord(0);

		pWriter->WriteRecord2(0, DocumentSettings);
		pWriter->WriteRecord2(1, Colors);
		pWriter->WriteRecord2(2, FaceNames);
		pWriter->WriteRecord2(3, StyleSheets);
		pWriter->WriteRecord2(4, DocumentSheet);
		pWriter->WriteRecord2(5, EventList);
		pWriter->WriteRecord2(6, HeaderFooter);

		smart_ptr<OOX::File> pFile = this->Find(OOX::Draw::FileTypes::Masters);
		CMastersFile* pMasters= dynamic_cast<CMastersFile*>(pFile.GetPointer());
		if (pMasters)
		{
			pWriter->StartRecord(7);
			pMasters->toPPTY(pWriter);
			pWriter->EndRecord();
		}

		pFile = this->Find(OOX::Draw::FileTypes::Pages);
		CPagesFile* pPages = dynamic_cast<CPagesFile*>(pFile.GetPointer());
		if (pPages)
		{
			pWriter->StartRecord(8);
			pPages->toPPTY(pWriter);
			pWriter->EndRecord();
		}

		pFile = this->Find(OOX::Draw::FileTypes::Connections);
		CConnectionsFile* pConnections = dynamic_cast<CConnectionsFile*>(pFile.GetPointer());
		if (pConnections)
		{
			pWriter->StartRecord(9);
			pConnections->toPPTY(pWriter);
			pWriter->EndRecord();
		}

		pFile = this->Find(OOX::Draw::FileTypes::Recordsets);
		CRecordsetsFile* pRecordsets = dynamic_cast<CRecordsetsFile*>(pFile.GetPointer());
		if (pRecordsets)
		{
			pWriter->StartRecord(10);
			pRecordsets->toPPTY(pWriter);
			pWriter->EndRecord();
		}
		
		pFile = this->Find(OOX::Draw::FileTypes::Solutions);
		CSolutionsFile* pSolutions = dynamic_cast<CSolutionsFile*>(pFile.GetPointer());
		if (pSolutions)
		{
			pWriter->StartRecord(11);
			pSolutions->toPPTY(pWriter);
			pWriter->EndRecord();
		}

		pFile = this->Find(OOX::Draw::FileTypes::Validation);
		CValidationFile* pValidation = dynamic_cast<CValidationFile*>(pFile.GetPointer());
		if (pValidation)
		{
			pWriter->StartRecord(12);
			pValidation->toPPTY(pWriter);
			pWriter->EndRecord();
		}		
		
		pFile = this->Find(OOX::Draw::FileTypes::Comments);
		CCommentsFile* pComments = dynamic_cast<CCommentsFile*>(pFile.GetPointer());
		if (pComments)
		{
			pWriter->StartRecord(13);
			pComments->toPPTY(pWriter);
			pWriter->EndRecord();
		}

		pFile = this->Find(OOX::Draw::FileTypes::Windows);
		CWindowsFile* pWindows = dynamic_cast<CWindowsFile*>(pFile.GetPointer());
		if (pPages)
		{
			pWriter->StartRecord(14);
			pWindows->toPPTY(pWriter);
			pWriter->EndRecord();
		}
		bool bThemes = false;
		std::vector<smart_ptr<OOX::File>>& container = ((OOX::IFileContainer*)(this))->GetContainer();
		for (size_t k = 0; k < container.size(); ++k)
		{
			if (OOX::FileTypes::Theme == container[k]->type())
			{
				PPTX::Theme* pTheme = dynamic_cast<PPTX::Theme*>(container[k].GetPointer());

				pWriter->StartRecord(15);
				pTheme->toPPTY(pWriter);
				pWriter->EndRecord();
				
				bThemes = true;
			}
		}
		if (false == bThemes)
		{//некорректный контейнер ???
			OOX::Draw::CVsdx *vsdx = dynamic_cast<OOX::Draw::CVsdx*>(((OOX::File*)this)->m_pMainDocument);

			if (vsdx && vsdx->m_pContentTypes.IsInit())
			{
				typedef std::multimap<std::wstring, smart_ptr<ContentTypes::COverride>> _mapContentTypes;
				std::pair<_mapContentTypes::iterator, _mapContentTypes::iterator> oFind;

				oFind = vsdx->m_pContentTypes->m_mapOverridesByType.equal_range(L"application/vnd.openxmlformats-officedocument.theme+xml");

				for (_mapContentTypes::iterator it = oFind.first; it != oFind.second; ++it)
				{
					if (it->second.IsInit())
					{
						PPTX::Theme oTheme(vsdx, vsdx->m_sDocumentPath + FILE_SEPARATOR_STR + it->second->filename());

						pWriter->StartRecord(15);
						oTheme.toPPTY(pWriter);
						pWriter->EndRecord();
					}
				}
			}	
		}
		pFile = this->Find(OOX::FileTypes::VbaProject);
		OOX::VbaProject *pVbaProject = dynamic_cast<OOX::VbaProject*>(pFile.GetPointer());
		if (pVbaProject)
		{
			pWriter->StartRecord(16);
			pVbaProject->toPPTY(pWriter);
			pWriter->EndRecord();
		}
		pFile = this->Find(OOX::FileTypes::JsaProject);
		OOX::JsaProject* pJsaProject = dynamic_cast<OOX::JsaProject*>(pFile.GetPointer());
		if (pJsaProject)
		{
			if (pJsaProject->IsExist() && !pJsaProject->IsExternal())
			{
				std::wstring pathJsa = pJsaProject->filename().GetPath();
				//if (std::wstring::npos != pathJsa.find(pWriter->m_strMainFolder)) //out path
				{
					BYTE* pData = NULL;
					DWORD nBytesCount;
					if (NSFile::CFileBinary::ReadAllBytes(pJsaProject->filename().GetPath(), &pData, nBytesCount))
					{
						pWriter->StartRecord(17);
						pWriter->WriteBYTEArray(pData, nBytesCount);
						pWriter->EndRecord();

						RELEASEARRAYOBJECTS(pData);
					}
				}
			}
			if (pJsaProject->IsExternal())
			{
				//pWriter->StartRecord(18);
				//m_oBcw.m_oStream.WriteStringW3(pJsaProject->filename().GetPath());
				//pWriter->EndRecord();
			}
		}
		pWriter->EndRecord();
	}
	void CDocumentFile::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"VisioDocument");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xmlns", L"http://schemas.microsoft.com/office/visio/2012/main");
		pWriter->WriteAttribute2(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		pWriter->WriteAttribute2(L"xml:space", L"preserve");
		pWriter->EndAttributes();

		if (DocumentSettings.IsInit())
			DocumentSettings->toXmlWriter(pWriter);
		if (Colors.IsInit())
			Colors->toXmlWriter(pWriter);
		if (FaceNames.IsInit())
			FaceNames->toXmlWriter(pWriter);
		if (StyleSheets.IsInit())
			StyleSheets->toXmlWriter(pWriter);
		if (DocumentSheet.IsInit())
			DocumentSheet->toXmlWriter(pWriter);
		if (EventList.IsInit())
			EventList->toXmlWriter(pWriter);
		if (HeaderFooter.IsInit())
			HeaderFooter->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"VisioDocument");
	}
	//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CSnapAngle::getType() const
	{
		return et_dr_SnapAngle;
	}
	void CSnapAngle::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		content = oReader.GetText2();
	}
	void CSnapAngle::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteDoubleReal2(0, content);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CSnapAngle::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				content = pReader->GetDoubleReal();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CSnapAngle::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->WriteNodeValue(L"SnapAngle", content);
	}
//-----------------------------------------------------------------------------------------------------------------------------
	void CSnapAngles::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"SnapAngle" == sName)
			{
				CSnapAngle* pItem = new CSnapAngle();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	EElementType CSnapAngles::getType() const
	{
		return et_dr_SnapAngles;
	}
	void CSnapAngles::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CSnapAngle());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}
	void CSnapAngles::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CSnapAngles::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"SnapAngles");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"SnapAngles");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CWindow::getType() const
	{
		return et_dr_Window;
	}
	void CWindow::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "WindowType", WindowType)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "WindowState", WindowState)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "WindowLeft", WindowLeft)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "WindowTop", WindowTop)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "WindowWidth", WindowWidth)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "WindowHeight", WindowHeight)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ContainerType", ContainerType)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Container", Container)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Page", Page)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Sheet", Sheet)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ViewScale", ViewScale)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ViewCenterX", ViewCenterX)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ViewCenterY", ViewCenterY)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Document", Document)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ParentWindow", ParentWindow)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ReadOnly", ReadOnly)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CWindow::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"ShowRulers" == sName)
			{
				ShowRulers = oReader.GetText2();
			}
			else if (L"ShowGrid" == sName)
			{
				ShowGrid = oReader.GetText2();
			} 
			else if (L"ShowPageBreaks" == sName)
			{
				ShowPageBreaks = oReader.GetText2();
			}
			else if (L"GlueSettings" == sName)
			{
				GlueSettings = oReader.GetText2();
			}
			else if (L"ShowGuides" == sName)
			{
				ShowGuides = oReader.GetText2();
			}
			else if (L"ShowConnectionPoints" == sName)
			{
				ShowConnectionPoints = oReader.GetText2();
			}
			else if (L"SnapSettings" == sName)
			{
				SnapSettings = oReader.GetText2();
			}
			else if (L"SnapExtensions" == sName)
			{
				SnapExtensions = oReader.GetText2();
			}
			else if (L"SnapAngles" == sName)
			{
				SnapAngles = oReader;
			}
			else if (L"DynamicGridEnabled" == sName)
			{
				DynamicGridEnabled = oReader.GetText2();
			}
			else if (L"TabSplitterPos" == sName)
			{
				TabSplitterPos = oReader.GetText2();
			}
			else if (L"StencilGroup" == sName)
			{
				StencilGroup = oReader.GetText2();
			}
			else if (L"StencilGroupPos" == sName)
			{
				StencilGroupPos = oReader.GetText2();
			}
		}

	}
	void CWindow::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ID);
		if (WindowType.IsInit()) pWriter->WriteByte1(1, WindowType->GetValue());
		pWriter->WriteUInt2(2, WindowState);
		pWriter->WriteInt2(3, WindowLeft);
		pWriter->WriteInt2(4, WindowTop);
		pWriter->WriteUInt2(5, WindowWidth);
		pWriter->WriteUInt2(6, WindowHeight);
		if (ContainerType.IsInit()) pWriter->WriteByte1(7, ContainerType->GetValue());
		pWriter->WriteUInt2(8, Container);
		pWriter->WriteUInt2(9, Page);
		pWriter->WriteUInt2(10, Sheet);
		pWriter->WriteDoubleReal2(11, ViewScale);
		pWriter->WriteDoubleReal2(12, ViewCenterX);
		pWriter->WriteDoubleReal2(13, ViewCenterY);
		pWriter->WriteString2(14, Document);
		pWriter->WriteUInt2(15, ParentWindow);
		pWriter->WriteBool2(16, ReadOnly);
		pWriter->WriteBool2(17, ShowRulers);
		pWriter->WriteBool2(18, ShowGrid);
		pWriter->WriteBool2(19, ShowPageBreaks);
		pWriter->WriteBool2(20, ShowGuides);
		pWriter->WriteBool2(21, ShowConnectionPoints);
		pWriter->WriteUInt2(22, GlueSettings);
		pWriter->WriteUInt2(23, SnapSettings);
		pWriter->WriteUInt2(24, SnapExtensions);
		pWriter->WriteBool2(26, DynamicGridEnabled);
		pWriter->WriteDoubleReal2(27, TabSplitterPos);
		pWriter->WriteUInt2(28, StencilGroup);
		pWriter->WriteUInt2(29, StencilGroupPos);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		
		pWriter->WriteRecord2(0, SnapAngles);
	}

	void CWindow::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				ID = pReader->GetULong();
			}break;
			case 1:
			{
				WindowType.Init(); WindowType->SetValueFromByte(pReader->GetUChar());
			}break;
			case 2:
			{
				WindowState = pReader->GetULong();
			}break;
			case 3:
			{
				WindowLeft = pReader->GetLong();
			}break;
			case 4:
			{
				WindowTop = pReader->GetLong();
			}break;
			case 5:
			{
				WindowWidth = pReader->GetULong();
			}break;
			case 6:
			{
				WindowHeight = pReader->GetULong();
			}break;
			case 7:
			{
				ContainerType.Init(); ContainerType->SetValueFromByte(pReader->GetUChar());
			}break;
			case 8:
			{
				Container = pReader->GetULong();
			}break;
			case 9:
			{
				Page = pReader->GetULong();
			}break;
			case 10:
			{
				Sheet = pReader->GetULong();
			}break;
			case 11:
			{
				ViewScale = pReader->GetDoubleReal();
			}break;
			case 12:
			{
				ViewCenterX = pReader->GetDoubleReal();
			}break;
			case 13:
			{
				ViewCenterY = pReader->GetDoubleReal();
			}break;
			case 14:
			{
				Document = pReader->GetString2();
			}break;
			case 15:
			{
				ParentWindow = pReader->GetULong();
			}break;
			case 16:
			{
				ReadOnly = pReader->GetBool();
			}break;
			case 17:
			{
				ShowRulers = pReader->GetBool();
			}break;
			case 18:
			{
				ShowGrid = pReader->GetBool();
			}break;
			case 19:
			{
				ShowPageBreaks = pReader->GetBool();
			}break;
			case 20:
			{
				ShowGuides = pReader->GetBool();
			}break;
			case 21:
			{
				ShowConnectionPoints = pReader->GetBool();
			}break;
			case 22:
			{
				GlueSettings = pReader->GetULong();
			}break;
			case 23:
			{
				SnapSettings = pReader->GetULong();
			}break;
			case 24:
			{
				SnapExtensions = pReader->GetULong();
			}break;
			case 26:
			{
				DynamicGridEnabled = pReader->GetBool();
			}break;
			case 27:
			{
				TabSplitterPos = pReader->GetDoubleReal();
			}break;			
			case 28:
			{
				StencilGroup = pReader->GetULong();
			}break;
			case 29:
			{
				StencilGroupPos = pReader->GetULong();
			}break;
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				SnapAngles.Init();
				SnapAngles->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CWindow::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Window");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"ID", ID);
		if (WindowType.IsInit()) 
			pWriter->WriteAttribute2(L"WindowType", WindowType->ToString());
		pWriter->WriteAttribute2(L"WindowState", WindowState);
		pWriter->WriteAttribute(L"WindowLeft", WindowLeft);
		pWriter->WriteAttribute(L"WindowTop", WindowTop);
		pWriter->WriteAttribute2(L"WindowWidth", WindowWidth);
		pWriter->WriteAttribute2(L"WindowHeight", WindowHeight);
		if (ContainerType.IsInit())
			pWriter->WriteAttribute2(L"ContainerType", ContainerType->ToString());
		pWriter->WriteAttribute2(L"Container", Container);
		pWriter->WriteAttribute2(L"Page", Page);
		pWriter->WriteAttribute2(L"Sheet", Sheet);
		pWriter->WriteAttribute(L"ViewScale", ViewScale);
		pWriter->WriteAttribute(L"ViewCenterX", ViewCenterX);
		pWriter->WriteAttribute(L"ViewCenterY", ViewCenterY);
		pWriter->WriteAttribute2(L"Document", Document);
		pWriter->WriteAttribute2(L"ParentWindow", ParentWindow);
		pWriter->WriteAttribute(L"ReadOnly", ReadOnly);
		pWriter->EndAttributes();

		pWriter->WriteNodeValue(L"ShowRulers", ShowRulers);
		pWriter->WriteNodeValue(L"ShowGrid", ShowGrid);
		pWriter->WriteNodeValue(L"ShowPageBreaks", ShowPageBreaks);
		pWriter->WriteNodeValue(L"ShowGuides", ShowGuides);
		pWriter->WriteNodeValue(L"ShowConnectionPoints", ShowConnectionPoints);
		pWriter->WriteNodeValue(L"GlueSettings", GlueSettings);
		pWriter->WriteNodeValue(L"SnapSettings", SnapSettings);
		pWriter->WriteNodeValue(L"SnapExtensions", SnapExtensions);
		pWriter->WriteNodeValue(L"DynamicGridEnabled", DynamicGridEnabled);
		pWriter->WriteNodeValue(L"TabSplitterPos", TabSplitterPos);
		pWriter->WriteNodeValue(L"StencilGroup", StencilGroup);
		pWriter->WriteNodeValue(L"StencilGroupPos", StencilGroupPos);

		if (SnapAngles.IsInit())
			SnapAngles->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Window");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	void CWindows::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"Window" == sName)
			{
				CWindow* pItem = new CWindow();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	void CWindows::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "ClientWidth", ClientWidth)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ClientHeight", ClientHeight)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	EElementType CWindows::getType() const
	{
		return et_dr_Windows;
	}
	void CWindows::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				ClientWidth = pReader->GetULong();
			}break;
			case 1:
			{
				ClientHeight = pReader->GetULong();
			}break;
			}
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CWindow());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}
	void CWindows::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, ClientWidth);
		pWriter->WriteUInt2(1, ClientHeight);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CWindows::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Windows");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xmlns", L"http://schemas.microsoft.com/office/visio/2012/main");
		pWriter->WriteAttribute2(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		pWriter->WriteAttribute2(L"xml:space", L"preserve");
		pWriter->WriteAttribute2(L"ClientWidth", ClientWidth);
		pWriter->WriteAttribute2(L"ClientHeight", ClientHeight);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"Windows");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	OOX::Draw::CWindowsFile::CWindowsFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	OOX::Draw::CWindowsFile::CWindowsFile(OOX::Document* pMain, const CPath& uri) : OOX::File(pMain)
	{
		read(uri.GetDirectory(), uri);
	}
	OOX::Draw::CWindowsFile::CWindowsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	OOX::Draw::CWindowsFile::~CWindowsFile()
	{
	}
	void OOX::Draw::CWindowsFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void OOX::Draw::CWindowsFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_strFilename = oFilePath.GetPath();

		Windows = oReader;
	}
	void OOX::Draw::CWindowsFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_VSDX;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
	}
	const OOX::FileType OOX::Draw::CWindowsFile::type() const
	{
		return FileTypes::Windows;
	}
	const OOX::CPath OOX::Draw::CWindowsFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath OOX::Draw::CWindowsFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void OOX::Draw::CWindowsFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				Windows.Init();
				Windows->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}
	void OOX::Draw::CWindowsFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteRecord2(0, Windows);
	}
	void OOX::Draw::CWindowsFile::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		if (Windows.IsInit())
			Windows->toXmlWriter(pWriter);
	}
}
} // namespace OOX
