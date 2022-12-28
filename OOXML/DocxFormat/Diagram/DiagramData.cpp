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

#include "DiagramData.h"
#include "../../Binary/Presentation/BinaryFileReaderWriter.h"

namespace OOX
{
	template<typename E>
	void Diagram::ComplexType<E>::fromXML(XmlUtils::CXmlNode& node)
	{
	}

	template<typename E>
	void Diagram::ComplexType<E>::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
	}

	template<typename E>
	EElementType Diagram::ComplexType<E>::getType() const
	{
		return et_dgm_ComplexType;
	}

	template<typename E>
	void Diagram::ComplexType<E>::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

	template<typename E>
	BYTE Diagram::ComplexType<E>::ReadByteFromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		BYTE value = 0;
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)
			{
				value = pReader->GetUChar();
			}
		}
		pReader->Seek(end); 
		return value;
	}

	template<typename E>
	int Diagram::ComplexType<E>::ReadIntFromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		int value = 0;
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)
			{
				value = pReader->GetLong();
			}
		}
		pReader->Seek(end);
		return value;
	}

	template<typename E>
	void Diagram::ComplexType<E>::WriteByteToPPTY(BYTE val, NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteByte1(0, val);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}

	template<typename E>
	void Diagram::ComplexType<E>::WriteIntToPPTY(int val, NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteInt1(0, val);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}

	template<typename E>
	void Diagram::ComplexType<E>::toXmlWriter(const std::wstring & node_name, NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(node_name);

		pWriter->StartAttributes();
		if (m_oVal.IsInit())
		{
			pWriter->WriteAttribute(L"val", m_oVal.IsInit() ? m_oVal->ToString() : L"");
		}
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(node_name);
	}

	template<typename E>
	std::wstring Diagram::ComplexType<E>::toXML(const std::wstring & node_name) const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(node_name, &oWriter);

		return oWriter.GetXmlString();
	}

//------------------------------------------------------------------------------------------------------

	Diagram::CAnimLvl::CAnimLvl() {}

	EElementType Diagram::CAnimLvl::getType() const
	{
		return et_dgm_AnimLvl;
	}

	std::wstring Diagram::CAnimLvl::toXML() const
	{
		return Diagram::ComplexType<SimpleTypes::CAnimLvlStr>::toXML(L"dgm:animLvl");
	}

	void Diagram::CAnimLvl::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		return Diagram::ComplexType<SimpleTypes::CAnimLvlStr>::toXmlWriter(L"dgm:animLvl", pWriter);
	}

	void Diagram::CAnimLvl::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		m_oVal.Init();
		m_oVal->SetValueFromByte(ReadByteFromPPTY(pReader));
	}

	void Diagram::CAnimLvl::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		WriteByteToPPTY(m_oVal.IsInit() ? (BYTE)m_oVal->GetValue() : 0, pWriter);
	}

//------------------------------------------------------------------------------------------------------

	Diagram::CAnimOne::CAnimOne() {}

	EElementType Diagram::CAnimOne::getType() const
	{
		return et_dgm_AnimOne;
	}

	std::wstring Diagram::CAnimOne::toXML() const
	{
		return Diagram::ComplexType<SimpleTypes::CAnimOneStr>::toXML(L"dgm:animOne");
	}

	void Diagram::CAnimOne::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		return Diagram::ComplexType<SimpleTypes::CAnimOneStr>::toXmlWriter(L"dgm:animOne", pWriter);
	}

	void Diagram::CAnimOne::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		m_oVal.Init();
		m_oVal->SetValueFromByte(ReadByteFromPPTY(pReader));
	}

	void Diagram::CAnimOne::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		WriteByteToPPTY(m_oVal.IsInit() ? (BYTE)m_oVal->GetValue() : 0, pWriter);
	}

//------------------------------------------------------------------------------------------------------

	Diagram::CBulletEnabled::CBulletEnabled() {}

	EElementType Diagram::CBulletEnabled::getType() const
	{
		return et_dgm_BulletEnabled;
	}

	std::wstring Diagram::CBulletEnabled::toXML() const
	{
		return Diagram::ComplexType<SimpleTypes::CBool>::toXML(L"dgm:bulletEnabled");
	}

	void Diagram::CBulletEnabled::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		return Diagram::ComplexType<SimpleTypes::CBool>::toXmlWriter(L"dgm:bulletEnabled", pWriter);
	}

	void Diagram::CBulletEnabled::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		m_oVal.Init();
		m_oVal->SetValueFromByte(ReadByteFromPPTY(pReader));
	}

	void Diagram::CBulletEnabled::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		WriteByteToPPTY(m_oVal.IsInit() ? (BYTE)m_oVal->GetValue() : 0, pWriter);
	}

//------------------------------------------------------------------------------------------------------

	Diagram::CChMax::CChMax() {}

	EElementType Diagram::CChMax::getType() const
	{
		return et_dgm_chMax;
	}

	std::wstring Diagram::CChMax::toXML() const
	{
		return Diagram::ComplexType<SimpleTypes::CDecimalNumber>::toXML(L"dgm:chMax");
	}

	void Diagram::CChMax::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		return Diagram::ComplexType<SimpleTypes::CDecimalNumber>::toXmlWriter(L"dgm:chMax", pWriter);
	}

	void Diagram::CChMax::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		m_oVal.Init();
		m_oVal->SetValue(ReadIntFromPPTY(pReader));
	}

	void Diagram::CChMax::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		WriteIntToPPTY(m_oVal.IsInit() ? m_oVal->GetValue() : 0, pWriter);
	}

//------------------------------------------------------------------------------------------------------

	Diagram::CChPref::CChPref() {}

	EElementType Diagram::CChPref::getType() const
	{
		return et_dgm_chPref;
	}

	std::wstring Diagram::CChPref::toXML() const
	{
		return Diagram::ComplexType<SimpleTypes::CDecimalNumber>::toXML(L"dgm:chPref");
	}

	void Diagram::CChPref::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		return Diagram::ComplexType<SimpleTypes::CDecimalNumber>::toXmlWriter(L"dgm:chPref", pWriter);
	}

	void Diagram::CChPref::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		m_oVal.Init();
		m_oVal->SetValue(ReadIntFromPPTY(pReader));
	}

	void Diagram::CChPref::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		WriteIntToPPTY(m_oVal.IsInit() ? m_oVal->GetValue() : 0, pWriter);
	}

//------------------------------------------------------------------------------------------------------

	Diagram::CDirection::CDirection() {}

	EElementType Diagram::CDirection::getType() const
	{
		return et_dgm_Direction;
	}

	std::wstring Diagram::CDirection::toXML() const
	{
		return Diagram::ComplexType<SimpleTypes::CDirectionDraw>::toXML(L"dgm:dir");
	}

	void Diagram::CDirection::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		return Diagram::ComplexType<SimpleTypes::CDirectionDraw>::toXmlWriter(L"dgm:dir", pWriter);
	}

	void Diagram::CDirection::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		m_oVal.Init();
		m_oVal->SetValueFromByte(ReadByteFromPPTY(pReader));
	}

	void Diagram::CDirection::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		WriteByteToPPTY(m_oVal.IsInit() ? (BYTE)m_oVal->GetValue() : 0, pWriter);
	}

//------------------------------------------------------------------------------------------------------

	Diagram::CHierBranch::CHierBranch() {}
	EElementType Diagram::CHierBranch::getType() const
	{
		return et_dgm_HierBranch;
	}

	std::wstring Diagram::CHierBranch::toXML() const
	{
		return Diagram::ComplexType<SimpleTypes::CHierBranch>::toXML(L"dgm:hierBranch");
	}

	void Diagram::CHierBranch::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		return Diagram::ComplexType<SimpleTypes::CHierBranch>::toXmlWriter(L"dgm:hierBranch", pWriter);
	}

	void Diagram::CHierBranch::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		m_oVal.Init();
		m_oVal->SetValueFromByte(ReadByteFromPPTY(pReader));
	}

	void Diagram::CHierBranch::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		WriteByteToPPTY(m_oVal.IsInit() ? (BYTE)m_oVal->GetValue() : 0, pWriter);
	}

//------------------------------------------------------------------------------------------------------

	Diagram::COrgChart::COrgChart() {}

	EElementType Diagram::COrgChart::getType() const
	{
		return et_dgm_OrgChart;
	}

	std::wstring Diagram::COrgChart::toXML() const
	{
		return Diagram::ComplexType<SimpleTypes::CBool>::toXML(L"dgm:orgChart");
	}

	void Diagram::COrgChart::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		return Diagram::ComplexType<SimpleTypes::CBool>::toXmlWriter(L"dgm:orgChart", pWriter);
	}

	void Diagram::COrgChart::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		m_oVal.Init();
		m_oVal->SetValueFromByte(ReadByteFromPPTY(pReader));
	}

	void Diagram::COrgChart::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		WriteByteToPPTY(m_oVal.IsInit() ? (BYTE)m_oVal->GetValue() : 0, pWriter);
	}

//------------------------------------------------------------------------------------------------------

	Diagram::CResizeHandles::CResizeHandles() {}

	EElementType Diagram::CResizeHandles::getType() const
	{
		return et_dgm_ResizeHandles;
	}

	std::wstring Diagram::CResizeHandles::toXML() const
	{
		return Diagram::ComplexType<SimpleTypes::CResizeHandles>::toXML(L"dgm:resizeHandles");
	}

	void Diagram::CResizeHandles::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		return Diagram::ComplexType<SimpleTypes::CResizeHandles>::toXmlWriter(L"dgm:resizeHandles", pWriter);
	}

	void Diagram::CResizeHandles::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		m_oVal.Init();
		m_oVal->SetValueFromByte(ReadByteFromPPTY(pReader));
	}

	void Diagram::CResizeHandles::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		WriteByteToPPTY(m_oVal.IsInit() ? (BYTE)m_oVal->GetValue() : 0, pWriter);
	}

//------------------------------------------------------------------------------------------------------

	Diagram::CVariableList::CVariableList() : node_name(L"dgm:varLst") {}

	Diagram::CVariableList::~CVariableList() {}

	std::wstring Diagram::CVariableList::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}

	void Diagram::CVariableList::fromXML(XmlUtils::CXmlNode& node)
	{
	}

	void Diagram::CVariableList::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:animLvl" == sName)
				m_oAnimLvl = oReader;
			else if (L"dgm:animOne" == sName)
				m_oAnimOne = oReader;
			else if (L"dgm:bulletEnabled" == sName)
				m_oBulletEnabled = oReader;
			else if (L"dgm:chMax" == sName)
				m_oChMax = oReader;
			else if (L"dgm:chPref" == sName)
				m_oChPref = oReader;
			else if (L"dgm:dir" == sName)
				m_oDir = oReader;
			else if (L"dgm:hierBranch" == sName)
				m_oHierBranch = oReader;
			else if (L"dgm:orgChart" == sName)
				m_oOrgChart = oReader;
			else if (L"dgm:resizeHandles" == sName)
				m_oResizeHandles = oReader;
		}
	}

	EElementType Diagram::CVariableList::getType() const
	{
		return et_dgm_VariableList;
	}

	void Diagram::CVariableList::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				m_oAnimLvl.Init(); 
				m_oAnimLvl->fromPPTY(pReader);
			}break;
			case 1:
			{
				m_oAnimOne.Init();
				m_oAnimOne->fromPPTY(pReader);
			}break;
			case 2:
			{
				m_oBulletEnabled.Init();
				m_oBulletEnabled->fromPPTY(pReader);
			}break;
			case 3:
			{
				m_oChMax.Init();
				m_oChMax->fromPPTY(pReader);
			}break;
			case 4:
			{
				m_oChPref.Init();
				m_oChPref->fromPPTY(pReader);
			}break;
			case 5:
			{
				m_oDir.Init();
				m_oDir->fromPPTY(pReader);
			}break;
			case 6:
			{
				m_oHierBranch.Init();
				m_oHierBranch->fromPPTY(pReader);
			}break;
			case 7:
			{
				m_oOrgChart.Init();
				m_oOrgChart->fromPPTY(pReader);
			}break;
			case 8:
			{
				m_oResizeHandles.Init();
				m_oResizeHandles->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}

	void Diagram::CVariableList::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteRecord2(0, m_oAnimLvl);
		pWriter->WriteRecord2(1, m_oAnimOne);
		pWriter->WriteRecord2(2, m_oBulletEnabled);
		pWriter->WriteRecord2(3, m_oChMax);
		pWriter->WriteRecord2(4, m_oChPref);
		pWriter->WriteRecord2(5, m_oDir);
		pWriter->WriteRecord2(6, m_oHierBranch);
		pWriter->WriteRecord2(7, m_oOrgChart);
		pWriter->WriteRecord2(8, m_oResizeHandles);

	}

	void Diagram::CVariableList::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(node_name);
		pWriter->EndAttributes();
		if (m_oChMax.IsInit())			m_oChMax->toXmlWriter(pWriter);
		if (m_oChPref.IsInit())			m_oChPref->toXmlWriter(pWriter);
		if (m_oDir.IsInit())			m_oDir->toXmlWriter(pWriter);
		if (m_oAnimOne.IsInit())		m_oAnimOne->toXmlWriter(pWriter);
		if (m_oAnimLvl.IsInit())		m_oAnimLvl->toXmlWriter(pWriter);
		if (m_oBulletEnabled.IsInit())	m_oBulletEnabled->toXmlWriter(pWriter);
		if (m_oHierBranch.IsInit())		m_oHierBranch->toXmlWriter(pWriter);
		if (m_oResizeHandles.IsInit())	m_oResizeHandles->toXmlWriter(pWriter);
		pWriter->WriteNodeEnd(node_name);
	}

//----------------------------------------------------------------------------------------------------

	Diagram::CPropertySet::CPropertySet() {}

	Diagram::CPropertySet::~CPropertySet() {}

	std::wstring Diagram::CPropertySet::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}

	void Diagram::CPropertySet::fromXML(XmlUtils::CXmlNode& node)
	{
	}

	void Diagram::CPropertySet::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:presLayoutVars" == sName)
				m_oPresLayoutVars = oReader;
			else if (L"dgm:style" == sName)
				m_oStyle = oReader;
		}
	}

	void Diagram::CPropertySet::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (1 == _at)	m_coherent3DOff = pReader->GetBool();
			else if (2 == _at)	m_csCatId = pReader->GetString2();
			else if (3 == _at)	m_csTypeId = pReader->GetString2();
			else if (4 == _at)	m_custAng = pReader->GetLong();
			else if (5 == _at)	m_custFlipHor = pReader->GetBool();
			else if (6 == _at)	m_custFlipVert = pReader->GetBool();
			else if (7 == _at)	m_custLinFactNeighborX = pReader->GetLong();
			else if (8 == _at)	m_custLinFactNeighborY = pReader->GetLong();
			else if (9 == _at)	m_custLinFactX = pReader->GetLong();
			else if (10 == _at)	m_custLinFactY = pReader->GetLong();
			else if (11 == _at)	m_custRadScaleInc = pReader->GetLong();
			else if (12 == _at)	m_custRadScaleRad = pReader->GetLong();
			else if (13 == _at)	m_custScaleX = pReader->GetLong();
			else if (14 == _at)	m_custScaleY = pReader->GetLong();
			else if (15 == _at)	m_custSzX = pReader->GetLong();
			else if (16 == _at)	m_custSzY = pReader->GetLong();
			else if (17 == _at)	m_custT = pReader->GetBool();
			else if (18 == _at)	m_loCatId = pReader->GetString2();
			else if (19 == _at)	m_loTypeId = pReader->GetString2();
			else if (20 == _at)	m_phldr = pReader->GetBool();
			else if (21 == _at)	m_phldrT = pReader->GetString2();
			else if (22 == _at)	m_presAssocID = pReader->GetString2();
			else if (23 == _at)	m_presName = pReader->GetString2();
			else if (24 == _at)	m_presStyleCnt = pReader->GetLong();
			else if (25 == _at)	m_presStyleIdx = pReader->GetLong();
			else if (26 == _at)	m_presStyleLbl = pReader->GetString2();
			else if (27 == _at)	m_qsCatId = pReader->GetString2();
			else if (28 == _at)	m_qsTypeId = pReader->GetString2();
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				m_oPresLayoutVars.Init(); m_oPresLayoutVars->node_name = L"dgm:presLayoutVars";
				m_oPresLayoutVars->fromPPTY(pReader);
			}break;
			case 1:
			{
				m_oStyle.Init(); 
				m_oStyle->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}

	EElementType Diagram::CPropertySet::getType() const
	{
		return et_dgm_prSet;
	}

	void Diagram::CPropertySet::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(1, m_coherent3DOff);
			pWriter->WriteString2(2, m_csCatId);
			pWriter->WriteString2(3, m_csTypeId);
			pWriter->WriteInt2(4, m_custAng);
			pWriter->WriteBool2(5, m_custFlipHor);
			pWriter->WriteBool2(6, m_custFlipVert);
			pWriter->WriteInt2(7, m_custLinFactNeighborX);
			pWriter->WriteInt2(8, m_custLinFactNeighborY);
			pWriter->WriteInt2(9, m_custLinFactX);
			pWriter->WriteInt2(10, m_custLinFactY);
			pWriter->WriteInt2(11, m_custRadScaleInc);
			pWriter->WriteInt2(12, m_custRadScaleRad);
			pWriter->WriteInt2(13, m_custScaleX);
			pWriter->WriteInt2(14, m_custScaleY);
			pWriter->WriteInt2(15, m_custSzX);
			pWriter->WriteInt2(16, m_custSzY);
			pWriter->WriteBool2(17, m_custT);
			pWriter->WriteString2(18, m_loCatId);
			pWriter->WriteString2(19, m_loTypeId);
			pWriter->WriteBool2(20, m_phldr);
			pWriter->WriteString2(21, m_phldrT);
			pWriter->WriteString2(22, m_presAssocID);
			pWriter->WriteString2(23, m_presName);
			pWriter->WriteInt2(24, m_presStyleCnt);
			pWriter->WriteInt2(25, m_presStyleIdx);
			pWriter->WriteString2(26, m_presStyleLbl);
			pWriter->WriteString2(27, m_qsCatId);
			pWriter->WriteString2(28, m_qsTypeId);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, m_oPresLayoutVars);
		pWriter->WriteRecord2(1, m_oStyle);
	}

	void Diagram::CPropertySet::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:prSet");

		pWriter->StartAttributes();
			pWriter->WriteAttribute(L"loTypeId", m_loTypeId);
			pWriter->WriteAttribute(L"loCatId", m_loCatId);
			pWriter->WriteAttribute(L"qsTypeId", m_qsTypeId);
			pWriter->WriteAttribute(L"qsCatId", m_qsCatId);
			pWriter->WriteAttribute(L"coherent3DOff", m_coherent3DOff);
			pWriter->WriteAttribute(L"csTypeId", m_csTypeId);
			pWriter->WriteAttribute(L"csCatId", m_csCatId);
			pWriter->WriteAttribute(L"phldr", m_phldr);
			pWriter->WriteAttribute(L"phldrT", m_phldrT);
			pWriter->WriteAttribute(L"custAng", m_custAng);
			pWriter->WriteAttribute(L"custFlipHor", m_custFlipHor);
			pWriter->WriteAttribute(L"custFlipVert", m_custFlipVert);
			pWriter->WriteAttribute(L"custLinFactNeighborX", m_custLinFactNeighborX);
			pWriter->WriteAttribute(L"custLinFactNeighborY", m_custLinFactNeighborY);
			pWriter->WriteAttribute(L"custLinFactX", m_custLinFactX);
			pWriter->WriteAttribute(L"custLinFactY", m_custLinFactY);
			pWriter->WriteAttribute(L"custRadScaleInc", m_custRadScaleInc);
			pWriter->WriteAttribute(L"custRadScaleRad", m_custRadScaleRad);
			pWriter->WriteAttribute(L"custScaleX", m_custScaleX);
			pWriter->WriteAttribute(L"custScaleY", m_custScaleY);
			pWriter->WriteAttribute(L"custSzX", m_custSzX);
			pWriter->WriteAttribute(L"custSzY", m_custSzY);
			pWriter->WriteAttribute(L"custT", m_custT);
			pWriter->WriteAttribute(L"presAssocID", m_presAssocID);
			pWriter->WriteAttribute(L"presName", m_presName);
			pWriter->WriteAttribute(L"presStyleLbl", m_presStyleLbl);
			pWriter->WriteAttribute(L"presStyleIdx", m_presStyleIdx);
			pWriter->WriteAttribute(L"presStyleCnt", m_presStyleCnt);
		pWriter->EndAttributes();
		
		if (m_oPresLayoutVars.IsInit()) m_oPresLayoutVars->toXmlWriter(pWriter);
		if (m_oStyle.IsInit()) m_oStyle->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dgm:prSet");
	}

	void Diagram::CPropertySet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"coherent3DOff", m_coherent3DOff)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"csCatId", m_csCatId)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"csTypeId", m_csTypeId)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"custAng", m_custAng)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"custFlipHor", m_custFlipHor)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"custFlipVert", m_custFlipVert)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"custLinFactNeighborX", m_custLinFactNeighborX)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"custLinFactNeighborY", m_custLinFactNeighborY)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"custLinFactX", m_custLinFactX)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"custLinFactY", m_custLinFactY)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"custRadScaleInc", m_custRadScaleInc)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"custRadScaleRad", m_custRadScaleRad)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"custScaleX", m_custScaleX)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"custScaleY", m_custScaleY)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"custSzX", m_custSzX)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"custSzY", m_custSzY)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"custT", m_custT)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"loCatId", m_loCatId)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"loTypeId", m_loTypeId)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"phldr", m_phldr)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"phldrT", m_phldrT)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"presAssocID", m_presAssocID)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"presName", m_presName)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"presStyleCnt", m_presStyleCnt)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"presStyleIdx", m_presStyleIdx)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"presStyleLbl", m_presStyleLbl)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"qsCatId", m_qsCatId)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"qsTypeId", m_qsTypeId)
		WritingElement_ReadAttributes_End(oReader)
	}

//----------------------------------------------------------------------------------------------------

	Diagram::CPt::CPt() {}

	Diagram::CPt::~CPt() {}

	std::wstring Diagram::CPt::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}

	void Diagram::CPt::fromXML(XmlUtils::CXmlNode& node)
	{
	}

	void Diagram::CPt::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:spPr" == sName)
				m_oSpPr = oReader;
			else if (L"dgm:t" == sName)
				m_oTxBody = oReader;
			else if (L"dgm:prSet" == sName)
				m_oPrSet = oReader;
		}
	}

	EElementType Diagram::CPt::getType() const
	{
		return et_dgm_pt;
	}

	void Diagram::CPt::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_sModelId = pReader->GetString2();
			else if (1 == _at)
			{
				m_oType.Init();
				m_oType->SetValueFromByte(pReader->GetUChar());
			}
			else if (2 == _at)	m_sCxnId = pReader->GetString2();
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					m_oSpPr.Init(); m_oSpPr->m_namespace = L"dgm";
					m_oSpPr->fromPPTY(pReader);
				}break;
				case 1:
				{
					m_oTxBody.Init(); m_oTxBody->m_name = L"dgm:t";
					m_oTxBody->fromPPTY(pReader);
				}break;
				case 2:
				{
					m_oPrSet.Init();
					m_oPrSet->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
			}
		}
		pReader->Seek(end);
	}

	void Diagram::CPt::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sModelId);
			if (m_oType.IsInit())
				pWriter->WriteByte1(1, m_oType->GetValue());
			pWriter->WriteString2(2, m_sCxnId);			
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, m_oSpPr);
		pWriter->WriteRecord2(1, m_oTxBody);
		pWriter->WriteRecord2(2, m_oPrSet);

	}

	void Diagram::CPt::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:pt");
			pWriter->WriteAttribute(L"modelId", m_sModelId);
			if (m_oType.IsInit())
				pWriter->WriteAttribute2(L"type", m_oType->ToString());
			pWriter->WriteAttribute(L"cxnId", m_sCxnId);
		pWriter->EndAttributes();

		if (m_oPrSet.IsInit())
			m_oPrSet->toXmlWriter(pWriter);
		
		if (m_oSpPr.IsInit())
			m_oSpPr->toXmlWriter(pWriter);
		
		if (m_oTxBody.IsInit())
			m_oTxBody->toXmlWriter(pWriter);		

		pWriter->WriteNodeEnd(L"dgm:pt");
	}

	void Diagram::CPt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"modelId", m_sModelId)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"type", m_oType)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"cxnId", m_sCxnId)
		WritingElement_ReadAttributes_End(oReader)
	}

//------------------------------------------------------------------------------------------------

	Diagram::CPtLst::CPtLst()
	{
	}

	Diagram::CPtLst::~CPtLst()
	{
	}

	void Diagram::CPtLst::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}

	void Diagram::CPtLst::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:pt" == sName)
			{
				m_arrItems.push_back(new Diagram::CPt(oReader));
			}
			else if (L"mc:AlternateContent" == sName)
			{
				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					std::wstring strName = oReader.GetName();

					if (oReader.IsEmptyNode())
						continue;

					if (strName == L"mc:Choice")
					{
						fromXML(oReader);
						break;
					}
					else if (strName == L"mc:Fallback")
					{
						fromXML(oReader);
					}
				}
			}
		}
	}

	EElementType Diagram::CPtLst::getType() const
	{
		return et_dgm_ptLst;
	}

	std::wstring Diagram::CPtLst::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}

	void Diagram::CPtLst::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					m_arrItems.push_back(new Diagram::CPt());
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

	void Diagram::CPtLst::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}

	void Diagram::CPtLst::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:ptLst");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dgm:ptLst");
	}
//------------------------------------------------------------------------------------------------

	Diagram::CCxn::CCxn() {}

	Diagram::CCxn::~CCxn() {}

	std::wstring Diagram::CCxn::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}

	void Diagram::CCxn::fromXML(XmlUtils::CXmlNode& node)
	{
	}

	void Diagram::CCxn::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
	}

	EElementType Diagram::CCxn::getType() const
	{
		return et_dgm_cxn;
	}

	void Diagram::CCxn::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_sModelId = pReader->GetString2();
			else if (1 == _at)	m_sType = pReader->GetString2();
			else if (2 == _at)	m_sDestId = pReader->GetString2();
			else if (3 == _at)	m_sDestOrd = pReader->GetString2();
			else if (4 == _at)	m_sSrcId = pReader->GetString2();
			else if (5 == _at)	m_sSrcOrd = pReader->GetString2();
			else if (6 == _at)	m_sParTransId = pReader->GetString2();
			else if (7 == _at)	m_sSibTransId = pReader->GetString2();
			else if (8 == _at)	m_sPresId = pReader->GetString2();
		}
		pReader->Seek(end);
	}

	void Diagram::CCxn::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sModelId);
			pWriter->WriteString2(1, m_sType);
			pWriter->WriteString2(2, m_sDestId);
			pWriter->WriteString2(3, m_sDestOrd);
			pWriter->WriteString2(4, m_sSrcId);
			pWriter->WriteString2(5, m_sSrcOrd);
			pWriter->WriteString2(6, m_sParTransId);
			pWriter->WriteString2(7, m_sSibTransId);
			pWriter->WriteString2(8, m_sPresId);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}

	void Diagram::CCxn::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:cxn");
		
		pWriter->WriteAttribute(L"modelId", m_sModelId);
		pWriter->WriteAttribute(L"type", m_sType);
		pWriter->WriteAttribute(L"srcId", m_sSrcId);
		pWriter->WriteAttribute(L"destId", m_sDestId);
		pWriter->WriteAttribute(L"srcOrd", m_sSrcOrd);
		pWriter->WriteAttribute(L"destOrd", m_sDestOrd);
		pWriter->WriteAttribute(L"parTransId", m_sParTransId);
		pWriter->WriteAttribute(L"sibTransId", m_sSibTransId);
		pWriter->WriteAttribute(L"presId", m_sPresId);

		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"dgm:cxn");
	}

	void Diagram::CCxn::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"modelId", m_sModelId)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"type", m_sType)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"destId", m_sDestId)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"destOrd", m_sDestOrd)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"srcId", m_sSrcId)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"srcOrd", m_sSrcOrd)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"parTransId", m_sParTransId)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"sibTransId", m_sSibTransId)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"presId", m_sPresId)
		WritingElement_ReadAttributes_End(oReader)
	}

//-------------------------------------------------------------------------------------------

	Diagram::CCxnLst::CCxnLst() {}

	Diagram::CCxnLst::~CCxnLst() {}

	void Diagram::CCxnLst::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}

	void Diagram::CCxnLst::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:cxn" == sName)
				m_arrItems.push_back(new Diagram::CCxn(oReader));
		}
	}

	std::wstring Diagram::CCxnLst::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}

	EElementType Diagram::CCxnLst::getType() const
	{
		return et_dgm_cxnLst;
	}

	void Diagram::CCxnLst::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					m_arrItems.push_back(new Diagram::CCxn());
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

	void Diagram::CCxnLst::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}

	void Diagram::CCxnLst::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:cxnLst");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dgm:cxnLst");
	}

//-------------------------------------------------------------------------------

	Diagram::CWhole::CWhole() {}
	Diagram::CWhole::~CWhole() {}

	std::wstring Diagram::CWhole::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}

	void Diagram::CWhole::fromXML(XmlUtils::CXmlNode& node)
	{
	}

	void Diagram::CWhole::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"effectDag" == sName ||
				L"effectLst" == sName)
			{
				m_oEffectList.fromXML(oReader);
			}
			else if (L"ln" == sName)
			{
				m_oLn = oReader;
			}
		}
	}

	EElementType Diagram::CWhole::getType() const
	{
		return et_dgm_Whole;
	}

	void Diagram::CWhole::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				m_oLn.Init(); m_oLn->m_name = L"dgm:ln";
				m_oLn->fromPPTY(pReader);
			}break;
			case 1:
			{
				m_oEffectList.fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);

	}

	void Diagram::CWhole::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteRecord2(0, m_oLn);
		pWriter->WriteRecord1(1, m_oEffectList);

	}

	void Diagram::CWhole::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:whole");
		pWriter->EndAttributes();
		if (m_oLn.IsInit())
			m_oLn->toXmlWriter(pWriter);
		if (m_oEffectList.is_init())
			m_oEffectList.toXmlWriter(pWriter);
		pWriter->WriteNodeEnd(L"dgm:whole");
	}

//-------------------------------------------------------------------------------

	Diagram::CBg::CBg() {}
	Diagram::CBg::~CBg() {}

	std::wstring Diagram::CBg::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}

	void Diagram::CBg::fromXML(XmlUtils::CXmlNode& node)
	{
	}

	void Diagram::CBg::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"blipFill" == sName ||
				L"gradFill" == sName ||
				L"grpFill" == sName ||
				L"noFill" == sName ||
				L"pattFill" == sName ||
				L"solidFill" == sName)
			{
				m_oFill.fromXML(oReader);
			}
			else if (L"effectDag" == sName ||
				L"effectLst" == sName)
			{
				m_oEffectList.fromXML(oReader);
			}
		}
	}

	EElementType Diagram::CBg::getType() const
	{
		return et_dgm_Bg;
	}

	void Diagram::CBg::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					m_oFill.fromPPTY(pReader);
				}break;
				case 1:
				{
					m_oEffectList.fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
			}
		}
		pReader->Seek(end);
	}

	void Diagram::CBg::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteRecord1(0, m_oFill);
		pWriter->WriteRecord1(1, m_oEffectList);
	}

	void Diagram::CBg::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->WriteString(L"<dgm:bg>");
		
		if (m_oFill.is_init())
			m_oFill.toXmlWriter(pWriter);
		if (m_oEffectList.is_init())
			m_oEffectList.toXmlWriter(pWriter);

		pWriter->WriteString(L"</dgm:bg>");
	}

//-------------------------------------------------------------------------------

	Diagram::CDataModel::CDataModel() {}
	Diagram::CDataModel::~CDataModel() {}

	std::wstring Diagram::CDataModel::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}

	void Diagram::CDataModel::fromXML(XmlUtils::CXmlNode& node)
	{
	}

	void Diagram::CDataModel::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:ptLst" == sName)
			{
				m_oPtLst = oReader;
			}
			else if (L"dgm:cxnLst" == sName)
			{
				m_oCxnLst = oReader;
			}
			else if (L"dgm:bg" == sName)
			{
				m_oBg = oReader;
			}
			else if (L"dgm:whole" == sName)
			{
				m_oWhole = oReader;
			}
			else if (L"dgm:extLst" == sName)
			{
				m_oExtLst = oReader;
			}
		}
	}

	EElementType Diagram::CDataModel::getType() const
	{
		return et_dgm_DataModel;
	}

	void Diagram::CDataModel::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					m_oPtLst.Init();
					m_oPtLst->fromPPTY(pReader);
				}break;
				case 1:
				{
					m_oCxnLst.Init();
					m_oCxnLst->fromPPTY(pReader);
				}break;
				case 2:
				{
					m_oWhole.Init();
					m_oWhole->fromPPTY(pReader);
				}break;
				case 3:
				{
					m_oBg.Init();
					m_oBg->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
			}
		}
		pReader->Seek(end);
	}

	void Diagram::CDataModel::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteRecord2(0, m_oPtLst);
		pWriter->WriteRecord2(1, m_oCxnLst);
		pWriter->WriteRecord2(2, m_oWhole);
		pWriter->WriteRecord2(3, m_oBg);
	}

	void Diagram::CDataModel::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:dataModel");

		pWriter->StartAttributes();
		if (bMain)
		{
			pWriter->WriteAttribute(L"xmlns:dgm", L"http://schemas.openxmlformats.org/drawingml/2006/diagram");
			pWriter->WriteAttribute(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
			pWriter->WriteAttribute(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
		}
		pWriter->EndAttributes();
		
		if (m_oPtLst.IsInit())
			m_oPtLst->toXmlWriter(pWriter);
		if (m_oCxnLst.IsInit())
			m_oCxnLst->toXmlWriter(pWriter);
		if (m_oBg.IsInit())
			m_oBg->toXmlWriter(pWriter);
		if (m_oWhole.IsInit())
			m_oWhole->toXmlWriter(pWriter);
		
		if (m_oExtLst.IsInit())
			m_oExtLst->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dgm:dataModel");
	}

//-------------------------------------------------------------------------------

	Diagram::CCat::CCat() {}
	Diagram::CCat::~CCat() {}

	std::wstring Diagram::CCat::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}

	void Diagram::CCat::fromXML(XmlUtils::CXmlNode& node)
	{
	}

	void Diagram::CCat::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
	}

	EElementType Diagram::CCat::getType() const
	{
		return et_dgm_Cat;
	}

	void Diagram::CCat::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_sType = pReader->GetString2();
			else if (1 == _at)	m_nPri = pReader->GetULong();
		}
		pReader->Seek(end);
	}

	void Diagram::CCat::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sType);
			pWriter->WriteUInt2(1, m_nPri);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}

	void Diagram::CCat::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:cat");
		
		pWriter->StartAttributes();
			pWriter->WriteAttribute(L"type", m_sType);
			pWriter->WriteAttribute2(L"pri", m_nPri);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"dgm:cat");
	}

	void Diagram::CCat::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"type", m_sType)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"pri", m_nPri)
		WritingElement_ReadAttributes_End(oReader)
	}

//-------------------------------------------------------------------------------------------

	Diagram::CCatLst::CCatLst()
	{
	}
	Diagram::CCatLst::~CCatLst()
	{
	}

	void Diagram::CCatLst::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}

	void Diagram::CCatLst::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:cat" == sName)
				m_arrItems.push_back(new Diagram::CCat(oReader));
		}
	}

	EElementType Diagram::CCatLst::getType() const
	{
		return et_dgm_CatLst;
	}

	std::wstring Diagram::CCatLst::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}

	void Diagram::CCatLst::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					m_arrItems.push_back(new Diagram::CCat());
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

	void Diagram::CCatLst::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}

	void Diagram::CCatLst::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:catLst");
		pWriter->EndAttributes();
		
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dgm:catLst");
	}

//-------------------------------------------------------------------------------------------

	void Diagram::CText::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
	}

	void Diagram::CText::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_sLang = pReader->GetString2();
			else if (1 == _at)	m_sVal = pReader->GetString2();
		}
		pReader->Seek(end);
	}

	void Diagram::CText::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sLang);
			pWriter->WriteString2(1, m_sVal);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}

	void Diagram::CText::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(node_name);
		pWriter->WriteAttribute(L"lang", m_sLang);
		pWriter->WriteAttribute(L"val", m_sVal);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(node_name);
	}

	void Diagram::CText::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"lang", m_sLang)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"val", m_sVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//-------------------------------------------------------------------------------------------

	CDiagramData::CDiagramData(OOX::Document* pMain) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
	{
	}

	CDiagramData::CDiagramData(OOX::Document* pMain, const CPath& uri) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
	{
		read(uri.GetDirectory(), uri);
	}

	CDiagramData::CDiagramData(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
	{
		read( oRootPath, oPath );
	}

	CDiagramData::~CDiagramData()
	{
	}

	void CDiagramData::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}

	void CDiagramData::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		IFileContainer::Read(oRootPath, oFilePath);

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		m_strFilename = oFilePath.GetPath();

		std::wstring sName = oReader.GetName();
		if (L"dgm:dataModel" == sName && !oReader.IsEmptyNode())
		{
			m_oDataModel = oReader;
		}
		if (false == id_drawing.IsInit() && m_oDataModel.IsInit())
		{
			for (size_t i = 0; m_oDataModel->m_oExtLst.IsInit() && (i < m_oDataModel->m_oExtLst->m_arrExt.size()); i++)
			{
				if (m_oDataModel->m_oExtLst->m_arrExt[i]->m_oDataModelExt.IsInit())
				{
					id_drawing = m_oDataModel->m_oExtLst->m_arrExt[i]->m_oDataModelExt->m_oRelId;
					break;
				}
			}
		}
	}

	void CDiagramData::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		
		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_DIAGRAM;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
		IFileContainer::Write(oFilePath, oDirectory, oContent);
	}

	const OOX::FileType CDiagramData::type() const
	{
		return FileTypes::DiagramData;
	}

	const CPath CDiagramData::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const CPath CDiagramData::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	void CDiagramData::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					m_oDataModel.Init();
					m_oDataModel->fromPPTY(pReader);

				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
			}
		}
		pReader->Seek(end);
	}

	void CDiagramData::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteRecord2(0, m_oDataModel);
	}

	void CDiagramData::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		if (m_oDataModel.IsInit())
			m_oDataModel->toXmlWriter(pWriter);
	}

} // namespace OOX
