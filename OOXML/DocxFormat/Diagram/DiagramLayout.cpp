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

#include "DiagramLayout.h"
#include "../../Binary/Presentation/BinaryFileReaderWriter.h"

#define Diagram_Layout_Read_Xml() \
{\
ReadAttributes(oReader);\
if (oReader.IsEmptyNode())\
return;\
int nParentDepth = oReader.GetDepth();\
while (oReader.ReadNextSiblingNode(nParentDepth))\
{\
	std::wstring sName = oReader.GetName();\
	WritingElement *pItem = NULL;\
	if (L"dgm:alg" == sName)\
		pItem = new CAlg(oReader);\
	else if (L"dgm:choose" == sName)\
		pItem = new CChoose(oReader);\
	else if (L"dgm:constrLst" == sName)\
		pItem = new CConstrLst(oReader);\
	else if (L"dgm:forEach" == sName)\
		pItem = new CForEach(oReader);\
	else if (L"dgm:layoutNode" == sName)\
		pItem = new CLayoutNode(oReader);\
	else if (L"dgm:presOf" == sName)\
		pItem = new CPresOf(oReader);\
	else if (L"dgm:ruleLst" == sName)\
		pItem = new CRuleLst(oReader);\
	else if (L"dgm:shape" == sName)\
		pItem = new CShape(oReader);\
	else if (L"dgm:varLst" == sName)\
		pItem = new CVariableList(oReader);\
	else if (L"dgm:extLst" == sName)\
		pItem = new OOX::Drawing::COfficeArtExtensionList(oReader);\
	if (pItem)\
		m_arrItems.push_back(pItem);\
}}
//---------------------------------------------------------------------------------
#define Diagram_Layout_From_PPTY() \
while (pReader->GetPos() < end)\
{\
	BYTE _rec = pReader->GetUChar();\
	OOX::WritingElement *pItem = NULL;\
	switch (_rec)\
	{\
	case DIAGRAM_LAYOUT_TYPE_ALG:			pItem = new OOX::Diagram::CAlg();			break;\
	case DIAGRAM_LAYOUT_TYPE_CHOOSE:		pItem = new OOX::Diagram::CChoose();		break;\
	case DIAGRAM_LAYOUT_TYPE_CONSTRLST:		pItem = new OOX::Diagram::CConstrLst();		break;\
	case DIAGRAM_LAYOUT_TYPE_FOREACH:		pItem = new OOX::Diagram::CForEach();		break;\
	case DIAGRAM_LAYOUT_TYPE_LAYOUTNODE:	pItem = new OOX::Diagram::CLayoutNode();	break;\
	case DIAGRAM_LAYOUT_TYPE_PRESOF:		pItem = new OOX::Diagram::CPresOf();		break;\
	case DIAGRAM_LAYOUT_TYPE_RULELST:		pItem = new OOX::Diagram::CRuleLst();		break;\
	case DIAGRAM_LAYOUT_TYPE_SHAPE:			pItem = new OOX::Diagram::CShape();			break;\
	case DIAGRAM_LAYOUT_TYPE_VARIABLELIST:	pItem = new OOX::Diagram::CVariableList();	break;\
	default:\
	{\
		pReader->SkipRecord();\
	}break;\
	}\
	if (pItem)\
	{\
		pItem->fromPPTY(pReader);\
		m_arrItems.push_back(pItem);\
	}\
}
//-----------------------------------------------------------------------------------------------------
#define Diagram_Layout_To_PPTY() \
for (size_t i = 0; i < m_arrItems.size(); ++i)\
{\
	if (!m_arrItems[i]) continue;\
	switch (m_arrItems[i]->getType())\
	{\
	case OOX::et_dgm_Alg:			pWriter->WriteRecord2(DIAGRAM_LAYOUT_TYPE_ALG, m_arrItems[i]); break;\
	case OOX::et_dgm_Choose:		pWriter->WriteRecord2(DIAGRAM_LAYOUT_TYPE_CHOOSE, m_arrItems[i]);	break;\
	case OOX::et_dgm_ConstrLst:		pWriter->WriteRecord2(DIAGRAM_LAYOUT_TYPE_CONSTRLST, m_arrItems[i]); break;\
	case OOX::et_dgm_ForEach:		pWriter->WriteRecord2(DIAGRAM_LAYOUT_TYPE_FOREACH, m_arrItems[i]);	break;\
	case OOX::et_dgm_LayoutNode:	pWriter->WriteRecord2(DIAGRAM_LAYOUT_TYPE_LAYOUTNODE, m_arrItems[i]); break;\
	case OOX::et_dgm_PresOf:		pWriter->WriteRecord2(DIAGRAM_LAYOUT_TYPE_PRESOF, m_arrItems[i]); break;\
	case OOX::et_dgm_RuleLst:		pWriter->WriteRecord2(DIAGRAM_LAYOUT_TYPE_RULELST, m_arrItems[i]); break;\
	case OOX::et_dgm_Shape:			pWriter->WriteRecord2(DIAGRAM_LAYOUT_TYPE_SHAPE, m_arrItems[i]); break;\
	case OOX::et_dgm_VariableList:	pWriter->WriteRecord2(DIAGRAM_LAYOUT_TYPE_VARIABLELIST, m_arrItems[i]); break;\
	default:\
		break;\
	}\
}

//------------------------------------------------------------------------------------------------------------------
namespace OOX
{
	CDiagramLayout::CDiagramLayout(OOX::Document* pMain) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
	{
	}
	CDiagramLayout::CDiagramLayout(OOX::Document* pMain, const CPath& uri) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
	{
		read(uri.GetDirectory(), uri);
	}
	CDiagramLayout::CDiagramLayout(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::IFileContainer(pMain), OOX::FileGlobalEnumerated(pMain)
	{
		read(oRootPath, oPath);
	}
	CDiagramLayout::~CDiagramLayout()
	{
	}
	void CDiagramLayout::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void CDiagramLayout::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		IFileContainer::Read(oRootPath, oFilePath);

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		std::wstring sName = oReader.GetName();
		if (L"dgm:layoutDef" == sName)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
			{
				int nNumberingDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nNumberingDepth))
				{
					sName = oReader.GetName();
					nullable_string m_sUniqueId;
					nullable_string m_sMinVer;

					if (L"dgm:catLst" == sName)
					{
						m_oCatLst = oReader;
					}
					else if (L"dgm:desc" == sName)
					{
						m_oDesc = oReader;
					}
					else if (L"dgm:title" == sName)
					{
						m_oTitle = oReader;
					}
					else if (L"dgm:clrData" == sName)
					{
						m_oClrData = oReader;
					}
					else if (L"dgm:layoutNode" == sName)
					{
						m_oLayoutNode = oReader;
					}
					else if (L"dgm:sampData" == sName)
					{
						m_oSampData = oReader;
					}
					else if (L"dgm:styleData" == sName)
					{
						m_oStyleData = oReader;
					}
					else if (L"dgm:extLst" == sName)
					{
						m_oExtLst = oReader;
					}
				}
			}
		}
	}
	void CDiagramLayout::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"uniqueId", m_sUniqueId)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"minVer", m_sMinVer)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"defStyle", m_sDefStyle)
		WritingElement_ReadAttributes_End(oReader)
	}
	void CDiagramLayout::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		
		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_DIAGRAM;
		toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
		IFileContainer::Write(oFilePath, oDirectory, oContent);
	}
	const OOX::FileType CDiagramLayout::type() const
	{
		return FileTypes::DiagramLayout;
	}
	const CPath CDiagramLayout::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CDiagramLayout::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CDiagramLayout::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_sUniqueId = pReader->GetString2();
			else if (1 == _at)	m_sMinVer = pReader->GetString2();
			else if (2 == _at)	m_sDefStyle = pReader->GetString2();
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					m_oTitle.Init(); 
					m_oTitle->node_name = L"dgm:title";
					m_oTitle->fromPPTY(pReader);
				}break;
				case 1:
				{
					m_oDesc.Init();
					m_oDesc->node_name = L"dgm:desc";
					m_oDesc->fromPPTY(pReader);
				}break;
				case 2:
				{
					m_oCatLst.Init();
					m_oCatLst->fromPPTY(pReader);
				}break;
				case 3:
				{
					m_oSampData.Init(); m_oSampData->node_name = L"dgm:sampData";
					m_oSampData->fromPPTY(pReader);
				}break;
				case 4:
				{
					m_oStyleData.Init(); m_oStyleData->node_name = L"dgm:styleData";
					m_oStyleData->fromPPTY(pReader);
				}break;
				case 5:
				{
					m_oClrData.Init(); m_oClrData->node_name = L"dgm:clrData";
					m_oClrData->fromPPTY(pReader);
				}break;
				case DIAGRAM_LAYOUT_TYPE_LAYOUTNODE:
				{
					m_oLayoutNode.Init();
					m_oLayoutNode->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
			}
		}
		pReader->Seek(end);
	}
	void CDiagramLayout::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sUniqueId);
			pWriter->WriteString2(1, m_sMinVer);
			pWriter->WriteString2(2, m_sDefStyle);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, m_oTitle);
		pWriter->WriteRecord2(1, m_oDesc);
		pWriter->WriteRecord2(2, m_oCatLst);
		pWriter->WriteRecord2(3, m_oSampData);
		pWriter->WriteRecord2(4, m_oStyleData);
		pWriter->WriteRecord2(5, m_oClrData);
		pWriter->WriteRecord2(DIAGRAM_LAYOUT_TYPE_LAYOUTNODE, m_oLayoutNode);
	}
	void CDiagramLayout::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:layoutDef");

		pWriter->StartAttributes();
			pWriter->WriteAttribute(L"xmlns:dgm", L"http://schemas.openxmlformats.org/drawingml/2006/diagram");
			pWriter->WriteAttribute(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
			pWriter->WriteAttribute(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");

			pWriter->WriteAttribute(L"uniqueId", m_sUniqueId);
			pWriter->WriteAttribute(L"minVer", m_sMinVer);
			pWriter->WriteAttribute(L"defStyle", m_sDefStyle);
		pWriter->EndAttributes();

		if (m_oTitle.IsInit()) m_oTitle->toXmlWriter(pWriter);
		if (m_oDesc.IsInit()) m_oDesc->toXmlWriter(pWriter);
		if (m_oCatLst.IsInit()) m_oCatLst->toXmlWriter(pWriter);
		if (m_oSampData.IsInit()) m_oSampData->toXmlWriter(pWriter);
		if (m_oStyleData.IsInit()) m_oStyleData->toXmlWriter(pWriter);
		if (m_oClrData.IsInit()) m_oClrData->toXmlWriter(pWriter);
		if (m_oLayoutNode.IsInit()) m_oLayoutNode->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dgm:layoutDef");
	}

//------------------------------------------------------------------------------------------------

	Diagram::CDiferentData::CDiferentData() {}
	Diagram::CDiferentData::~CDiferentData() {}
	std::wstring Diagram::CDiferentData::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CDiferentData::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CDiferentData::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:dataModel" == sName)
			{
				m_oDataModel = oReader;
			}
		}

	}
	EElementType Diagram::CDiferentData::getType() const
	{
		return et_dgm_DiferentData;
	}
	void Diagram::CDiferentData::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_bUseDef = pReader->GetBool();
		}

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				m_oDataModel.Init(); m_oDataModel->bMain = false;
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
	void Diagram::CDiferentData::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, m_bUseDef);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, m_oDataModel);
	}
	void Diagram::CDiferentData::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(node_name);
			pWriter->WriteAttribute(L"useDef", m_bUseDef);
		pWriter->EndAttributes();
		if (m_oDataModel.IsInit())
			m_oDataModel->toXmlWriter(pWriter);
		pWriter->WriteNodeEnd(node_name);
	}
	void Diagram::CDiferentData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"useDef", m_bUseDef)
		WritingElement_ReadAttributes_End(oReader)
	}

//-------------------------------------------------------------------------------------------

	Diagram::CLayoutNode::CLayoutNode() {}
	Diagram::CLayoutNode::~CLayoutNode() {}
	std::wstring Diagram::CLayoutNode::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CLayoutNode::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CLayoutNode::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		Diagram_Layout_Read_Xml()
	}
	EElementType Diagram::CLayoutNode::getType() const
	{
		return et_dgm_LayoutNode;
	}
	void Diagram::CLayoutNode::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_sName = pReader->GetString2();
			else if (1 == _at)	m_sStyleLbl = pReader->GetString2();
			else if (2 == _at)	m_sMoveWith = pReader->GetString2();
			else if (3 == _at)
			{
				m_oChOrder.Init();
				m_oChOrder->SetValueFromByte(pReader->GetUChar());
			}
		}
		Diagram_Layout_From_PPTY()

		pReader->Seek(end);
	}
	void Diagram::CLayoutNode::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sName);
			pWriter->WriteString2(1, m_sStyleLbl);
			pWriter->WriteString2(2, m_sMoveWith);
			if (m_oChOrder.IsInit()) pWriter->WriteByte1(3, m_oChOrder->GetValue());
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		Diagram_Layout_To_PPTY()
	}
	void Diagram::CLayoutNode::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:layoutNode");
			pWriter->WriteAttribute(L"name", m_sName);
			pWriter->WriteAttribute(L"styleLbl", m_sStyleLbl);
			pWriter->WriteAttribute(L"moveWith", m_sMoveWith);
			if (m_oChOrder.IsInit()) pWriter->WriteAttribute(L"chOrder", m_oChOrder->ToString());
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (!m_arrItems[i]) continue;

			m_arrItems[i]->toXmlWriter(pWriter);
		}

		pWriter->WriteNodeEnd(L"dgm:layoutNode");
	}
	void Diagram::CLayoutNode::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"name", m_sName)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"styleLbl", m_sStyleLbl)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"moveWith", m_sMoveWith)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"chOrder", m_oChOrder)
		WritingElement_ReadAttributes_End(oReader)
	}

//-------------------------------------------------------------------------------------------

	Diagram::CForEach::CForEach() {}
	Diagram::CForEach::~CForEach() {}
	std::wstring Diagram::CForEach::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CForEach::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CForEach::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		Diagram_Layout_Read_Xml()
	}
	EElementType Diagram::CForEach::getType() const
	{
		return et_dgm_ForEach;
	}
	void Diagram::CForEach::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_sName = pReader->GetString2();
			else if (1 == _at)	m_arSt.push_back(pReader->GetLong());
			else if (2 == _at)	m_arStep.push_back(pReader->GetLong());
			else if (3 == _at)	m_arHideLastTrans.push_back(pReader->GetBool());
			else if (4 == _at)	m_arCnt.push_back(pReader->GetLong());
			else if (5 == _at)
			{
				SimpleTypes::CAxisTypes axis; axis.SetValueFromByte(pReader->GetUChar());
				m_arAxis.push_back(axis);
			}
			else if (6 == _at)
			{
				SimpleTypes::CElementTypes ptType; ptType.SetValueFromByte(pReader->GetUChar());
				m_arPtType.push_back(ptType);
			}
			else if (7 == _at)	m_sRef = pReader->GetString2();
		}
		
		Diagram_Layout_From_PPTY()

		pReader->Seek(end);
	}
	void Diagram::CForEach::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sName);
			for (size_t i = 0; i < m_arSt.size(); ++i)
				pWriter->WriteInt1(1, m_arSt[i]);
			for (size_t i = 0; i < m_arStep.size(); ++i)
				pWriter->WriteInt1(2, m_arStep[i]);
			for (size_t i = 0; i < m_arHideLastTrans.size(); ++i)
				pWriter->WriteBool1(3, m_arHideLastTrans[i]);
			for (size_t i = 0; i < m_arCnt.size(); ++i)
				pWriter->WriteInt1(4, m_arCnt[i]);
			for (size_t i = 0; i < m_arAxis.size(); ++i)
				pWriter->WriteByte1(5, m_arAxis[i].GetValue());
			for (size_t i = 0; i < m_arPtType.size(); ++i)
				pWriter->WriteByte1(6, m_arPtType[i].GetValue());
			pWriter->WriteString2(7, m_sRef);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		Diagram_Layout_To_PPTY()
	}
	void Diagram::CForEach::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:forEach");
			pWriter->WriteAttribute(L"name", m_sName);
			if (false == m_arAxis.empty())
			{
				std::wstring sAxis;
				for (size_t i = 0; i < m_arAxis.size(); ++i) sAxis += L" " + m_arAxis[i].ToString();
				pWriter->WriteAttribute(L"axis", sAxis.substr(1));
			}
			if (false == m_arPtType.empty())
			{
				std::wstring sPtType;
				for (size_t i = 0; i < m_arPtType.size(); ++i) sPtType += L" " + m_arPtType[i].ToString();
				pWriter->WriteAttribute(L"ptType", sPtType.substr(1));
			}
			if (false == m_arSt.empty())
			{
				std::wstring sRes;
				for (size_t i = 0; i < m_arSt.size(); ++i) sRes += L" " + std::to_wstring(m_arSt[i]);
				pWriter->WriteAttribute(L"st", sRes.substr(1));
			}
			if (false == m_arCnt.empty())
			{
				std::wstring sRes;
				for (size_t i = 0; i < m_arCnt.size(); ++i) sRes += L" " + std::to_wstring(m_arCnt[i]);
				pWriter->WriteAttribute(L"cnt", sRes.substr(1));
			}
			if (false == m_arStep.empty())
			{
				std::wstring sRes;
				for (size_t i = 0; i < m_arStep.size(); ++i) sRes += L" " + std::to_wstring(m_arStep[i]);
				pWriter->WriteAttribute(L"step", sRes.substr(1));
			}
			if (false == m_arHideLastTrans.empty())
			{
				std::wstring sRes;
				for (size_t i = 0; i < m_arHideLastTrans.size(); ++i) sRes += (m_arHideLastTrans[i] ? L" 1" : L" 0");
				pWriter->WriteAttribute(L"hideLastTrans", sRes.substr(1));
			}
			pWriter->WriteAttribute(L"ref", m_sRef);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (!m_arrItems[i]) continue;

			m_arrItems[i]->toXmlWriter(pWriter);
		}

		pWriter->WriteNodeEnd(L"dgm:forEach");
	}
	void Diagram::CForEach::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		nullable_string sPtTypes, sAxis, sCnt, sStep, sHideLastTrans, sSt;
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"name", m_sName)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"axis", sAxis)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"cnt", sCnt)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"hideLastTrans", sHideLastTrans)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"ptType", sPtTypes)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"ref", m_sRef)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"st", sSt)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"step", sStep)
		WritingElement_ReadAttributes_End(oReader)

		if (sAxis.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sAxis, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arAxis.push_back(SimpleTypes::CAxisTypes(arStr[i]));
		}
		if (sPtTypes.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sPtTypes, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arPtType.push_back(SimpleTypes::CElementTypes(arStr[i]));
		}
		if (sSt.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sSt, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arSt.push_back(XmlUtils::GetInteger(arStr[i]));
		}
		if (sCnt.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sCnt, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arCnt.push_back(XmlUtils::GetInteger(arStr[i]));
		}
		if (sStep.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sStep, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arStep.push_back(XmlUtils::GetInteger(arStr[i]));
		}
		if (sHideLastTrans.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sHideLastTrans, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arHideLastTrans.push_back(SimpleTypes::COnOff(arStr[i]).ToBool());
		}
	}

//-------------------------------------------------------------------------------------------

	Diagram::CIf::CIf() {}
	Diagram::CIf::~CIf() {}
	std::wstring Diagram::CIf::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CIf::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CIf::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		Diagram_Layout_Read_Xml()
	}
	EElementType Diagram::CIf::getType() const
	{
		return et_dgm_If;
	}
	void Diagram::CIf::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_sName = pReader->GetString2();
			else if (1 == _at)	m_arSt.push_back(pReader->GetLong());
			else if (2 == _at)	m_arStep.push_back(pReader->GetLong());
			else if (3 == _at)	m_arHideLastTrans.push_back(pReader->GetBool());
			else if (4 == _at)	m_arCnt.push_back(pReader->GetLong());
			else if (5 == _at)
			{
				SimpleTypes::CAxisTypes axis; axis.SetValueFromByte(pReader->GetUChar());
				m_arAxis.push_back(axis);
			}
			else if (6 == _at)
			{
				SimpleTypes::CElementTypes ptType; ptType.SetValueFromByte(pReader->GetUChar());
				m_arPtType.push_back(ptType);
			}
			else if (7 == _at)	m_sRef = pReader->GetString2();
			else if (8 == _at)
			{
				m_oOp.Init();
				m_oOp->SetValueFromByte(pReader->GetUChar());
			}
			else if (9 == _at)
			{
				m_oFunc.Init();
				m_oFunc->SetValueFromByte(pReader->GetUChar());
			}
			else if (10 == _at)	m_sVal = pReader->GetString2();
			else if (11 == _at)	m_sArg = pReader->GetString2();
		}
		Diagram_Layout_From_PPTY()

		pReader->Seek(end);
	}
	void Diagram::CIf::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sName);
			for (size_t i = 0; i < m_arSt.size(); ++i)
				pWriter->WriteInt1(1, m_arSt[i]);
			for (size_t i = 0; i < m_arStep.size(); ++i)
				pWriter->WriteInt1(2, m_arStep[i]);
			for (size_t i = 0; i < m_arHideLastTrans.size(); ++i)
				pWriter->WriteBool1(3, m_arHideLastTrans[i]);
			for (size_t i = 0; i < m_arCnt.size(); ++i)
				pWriter->WriteInt1(4, m_arCnt[i]);
			for (size_t i = 0; i < m_arAxis.size(); ++i)
				pWriter->WriteByte1(5, m_arAxis[i].GetValue());
			for (size_t i = 0; i < m_arPtType.size(); ++i)
				pWriter->WriteByte1(6, m_arPtType[i].GetValue());
			pWriter->WriteString2(7, m_sRef);
			if (m_oOp.IsInit()) pWriter->WriteByte1(8, m_oOp->GetValue());
			if (m_oFunc.IsInit()) pWriter->WriteByte1(9, m_oFunc->GetValue());
			pWriter->WriteString2(10, m_sVal);
			pWriter->WriteString2(11, m_sArg);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		Diagram_Layout_To_PPTY()
	}
	void Diagram::CIf::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:if");
			pWriter->WriteAttribute(L"name", m_sName);
			if (false == m_arAxis.empty())
			{
				std::wstring sAxis;
				for (size_t i = 0; i < m_arAxis.size(); ++i) sAxis += L" " + m_arAxis[i].ToString();
				pWriter->WriteAttribute(L"axis", sAxis.substr(1));
			}
			if (false == m_arPtType.empty())
			{
				std::wstring sPtType;
				for (size_t i = 0; i < m_arPtType.size(); ++i) sPtType += L" " + m_arPtType[i].ToString();
				pWriter->WriteAttribute(L"ptType", sPtType.substr(1));
			}
			if (false == m_arSt.empty())
			{
				std::wstring sRes;
				for (size_t i = 0; i < m_arSt.size(); ++i) sRes += L" " + std::to_wstring(m_arSt[i]);
				pWriter->WriteAttribute(L"st", sRes.substr(1));
			}
			if (false == m_arCnt.empty())
			{
				std::wstring sRes;
				for (size_t i = 0; i < m_arCnt.size(); ++i) sRes += L" " + std::to_wstring(m_arCnt[i]);
				pWriter->WriteAttribute(L"cnt", sRes.substr(1));
			}
			if (m_oFunc.IsInit()) pWriter->WriteAttribute(L"func", m_oFunc->ToString());
			pWriter->WriteAttribute(L"arg", m_sArg);
			if (false == m_arStep.empty())
			{
				std::wstring sRes;
				for (size_t i = 0; i < m_arStep.size(); ++i) sRes += L" " + std::to_wstring(m_arStep[i]);
				pWriter->WriteAttribute(L"step", sRes.substr(1));
			}
			if (false == m_arHideLastTrans.empty())
			{
				std::wstring sRes;
				for (size_t i = 0; i < m_arHideLastTrans.size(); ++i) sRes += (m_arHideLastTrans[i] ? L" 1" : L" 0");
				pWriter->WriteAttribute(L"hideLastTrans", sRes.substr(1));
			}

			if (m_oOp.IsInit()) pWriter->WriteAttribute(L"op", m_oOp->ToString());
			pWriter->WriteAttribute(L"ref", m_sRef);
			pWriter->WriteAttribute(L"val", m_sVal);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (!m_arrItems[i]) continue;

			m_arrItems[i]->toXmlWriter(pWriter);
		}

		pWriter->WriteNodeEnd(L"dgm:if");
	}
	void Diagram::CIf::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		nullable_string sPtTypes, sAxis, sCnt, sStep, sHideLastTrans, sSt;
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"name", m_sName)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"axis", sAxis)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"cnt", sCnt)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"hideLastTrans", sHideLastTrans)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"ptType", sPtTypes)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"ref", m_sRef)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"st", sSt)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"step", sStep)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"arg", m_sArg)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"func", m_oFunc)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"op", m_oOp)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"val", m_sVal)
		WritingElement_ReadAttributes_End(oReader)
		if (sAxis.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sAxis, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arAxis.push_back(SimpleTypes::CAxisTypes(arStr[i]));
		}
		if (sPtTypes.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sPtTypes, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arPtType.push_back(SimpleTypes::CElementTypes(arStr[i]));
		}
		if (sSt.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sSt, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arSt.push_back(XmlUtils::GetInteger(arStr[i]));
		}
		if (sCnt.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sCnt, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arCnt.push_back(XmlUtils::GetInteger(arStr[i]));
		}
		if (sStep.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sStep, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arStep.push_back(XmlUtils::GetInteger(arStr[i]));
		}
		if (sHideLastTrans.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sHideLastTrans, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arHideLastTrans.push_back(SimpleTypes::COnOff(arStr[i]).ToBool());
		}
	}

//-------------------------------------------------------------------------------------------

	Diagram::CElse::CElse() {}
	Diagram::CElse::~CElse() {}
	std::wstring Diagram::CElse::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CElse::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CElse::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		Diagram_Layout_Read_Xml()
	}
	EElementType Diagram::CElse::getType() const
	{
		return et_dgm_Else;
	}
	void Diagram::CElse::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_sName = pReader->GetString2();
		}
		Diagram_Layout_From_PPTY()

		pReader->Seek(end);
	}
	void Diagram::CElse::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sName);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		Diagram_Layout_To_PPTY()
	}
	void Diagram::CElse::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:else");
			pWriter->WriteAttribute(L"name", m_sName);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (!m_arrItems[i]) continue;

			m_arrItems[i]->toXmlWriter(pWriter);
		}

		pWriter->WriteNodeEnd(L"dgm:else");
	}
	void Diagram::CElse::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"name", m_sName)
		WritingElement_ReadAttributes_End(oReader)
	}

//-------------------------------------------------------------------------------------------

	Diagram::CChoose::CChoose() {}
	Diagram::CChoose::~CChoose() {}
	std::wstring Diagram::CChoose::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CChoose::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CChoose::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();
			
			if (L"dgm:if" == sName)
			{
				OOX::Diagram::CIf *pItem = new Diagram::CIf(oReader);
				if (pItem)
					m_arrItems.push_back(pItem); 
			}
			else if (L"dgm:else" == sName)
			{
				m_oElse = oReader;
			}
		}
	}
	EElementType Diagram::CChoose::getType() const
	{
		return et_dgm_Choose;
	}
	void Diagram::CChoose::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_sName = pReader->GetString2();
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					m_arrItems.push_back(new Diagram::CIf());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				case 1:
				{
					m_oElse.Init();
					m_oElse->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
			}
		}
		pReader->Seek(end);
	}
	void Diagram::CChoose::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sName);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));

		pWriter->WriteRecord2(1, m_oElse);
	}
	void Diagram::CChoose::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:choose");
		pWriter->WriteAttribute(L"name", m_sName);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		if (m_oElse.IsInit()) m_oElse->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dgm:choose");
	}
	void Diagram::CChoose::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"name", m_sName)
		WritingElement_ReadAttributes_End(oReader)
	}

//-------------------------------------------------------------------------------------------

	Diagram::CAlg::CAlg() {}
	Diagram::CAlg::~CAlg() {}
	std::wstring Diagram::CAlg::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CAlg::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CAlg::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:param" == sName)
			{
				CParam* pItem = new CParam(oReader);
				if (pItem)
					m_arrItems.push_back(pItem);
			}
			else if (L"dgm:extLst" == sName)
				m_oExtLst = oReader;
		}

	}
	EElementType Diagram::CAlg::getType() const
	{
		return et_dgm_Alg;
	}
	void Diagram::CAlg::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_nRev = pReader->GetULong();
			else if (1 == _at)
			{
				m_oType.Init();
				m_oType->SetValueFromByte(pReader->GetUChar());
			}
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
				case 0:
				{
					m_arrItems.push_back(new Diagram::CParam());
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
	void Diagram::CAlg::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, m_nRev);
			if (m_oType.IsInit()) pWriter->WriteByte1(1, m_oType->GetValue());
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void Diagram::CAlg::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:alg");
		if (m_oType.IsInit()) pWriter->WriteAttribute(L"type", m_oType->ToString());
		pWriter->WriteAttribute2(L"rev", m_nRev);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dgm:alg");
	}
	void Diagram::CAlg::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"rev", m_nRev)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"type", m_oType)
		WritingElement_ReadAttributes_End(oReader)
	}

//-------------------------------------------------------------------------------------------

	Diagram::CConstrLst::CConstrLst() {}
	Diagram::CConstrLst::~CConstrLst() {}
	std::wstring Diagram::CConstrLst::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CConstrLst::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CConstrLst::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:constr" == sName)
			{
				CConstraint* pItem = new CConstraint(oReader);
				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	EElementType Diagram::CConstrLst::getType() const
	{
		return et_dgm_ConstrLst;
	}
	void Diagram::CConstrLst::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
				case 0:
				{
					m_arrItems.push_back(new Diagram::CConstraint());
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
	void Diagram::CConstrLst::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void Diagram::CConstrLst::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:constrLst");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dgm:constrLst");
	}

//-------------------------------------------------------------------------------------------

	Diagram::CRuleLst::CRuleLst() {}
	Diagram::CRuleLst::~CRuleLst() {}
	std::wstring Diagram::CRuleLst::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CRuleLst::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CRuleLst::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:rule" == sName)
			{
				CRule* pItem = new CRule(oReader);
				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	EElementType Diagram::CRuleLst::getType() const
	{
		return et_dgm_RuleLst;
	}
	void Diagram::CRuleLst::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
				case 0:
				{
					m_arrItems.push_back(new Diagram::CRule());
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
	void Diagram::CRuleLst::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void Diagram::CRuleLst::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:ruleLst");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dgm:ruleLst");
	}

//-------------------------------------------------------------------------------------------

	Diagram::CAdjLst::CAdjLst() {}
	Diagram::CAdjLst::~CAdjLst() {}
	std::wstring Diagram::CAdjLst::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CAdjLst::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CAdjLst::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:adj" == sName)
			{
				CShapeAdjust* pItem = new CShapeAdjust(oReader);
				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	EElementType Diagram::CAdjLst::getType() const
	{
		return et_dgm_AdjLst;
	}
	void Diagram::CAdjLst::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
				case 0:
				{
					m_arrItems.push_back(new Diagram::CShapeAdjust());
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
	void Diagram::CAdjLst::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void Diagram::CAdjLst::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:adjLst");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dgm:adjLst");
	}

//-------------------------------------------------------------------------------------------

	Diagram::CConstraint::CConstraint() {}
	Diagram::CConstraint::~CConstraint() {}
	std::wstring Diagram::CConstraint::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CConstraint::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CConstraint::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:extLst" == sName)
				m_oExtLst = oReader;
		}

	}
	EElementType Diagram::CConstraint::getType() const
	{
		return et_dgm_Constraint;
	}
	void Diagram::CConstraint::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_oFact = pReader->GetDoubleReal();
			else if (1 == _at)
			{
				m_oFor.Init();
				m_oFor->SetValueFromByte(pReader->GetUChar());
			}
			else if (2 == _at)	m_oForName = pReader->GetString2();
			else if (3 == _at)
			{
				m_oOp.Init();
				m_oOp->SetValueFromByte(pReader->GetUChar());
			}
			else if (4 == _at)
			{
				m_oPtType.Init();
				m_oPtType->SetValueFromByte(pReader->GetUChar());
			}
			else if (5 == _at)
			{
				m_oRefFor.Init();
				m_oRefFor->SetValueFromByte(pReader->GetUChar());
			}
			else if (6 == _at)	m_oRefForName = pReader->GetString2();
			else if (7 == _at)
			{
				m_oRefPtType.Init();
				m_oRefPtType->SetValueFromByte(pReader->GetUChar());
			}
			else if (8 == _at)
			{
				m_oRefType.Init();
				m_oRefType->SetValueFromByte(pReader->GetUChar());
			}
			else if (9 == _at)
			{
				m_oType.Init();
				m_oType->SetValueFromByte(pReader->GetUChar());
			}
			else if (10 == _at)
			{
				m_oVal.Init();
				m_oVal->SetValue(pReader->GetDoubleReal());
			}
		}
		pReader->Seek(end);
	}
	void Diagram::CConstraint::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteDoubleReal2(0, m_oFact);
			if (m_oFor.IsInit()) pWriter->WriteByte1(1, m_oFor->GetValue());
			pWriter->WriteString2(2, m_oForName);
			if (m_oOp.IsInit()) pWriter->WriteByte1(3, m_oOp->GetValue());
			if (m_oPtType.IsInit()) pWriter->WriteByte1(4, m_oPtType->GetValue());
			if (m_oRefFor.IsInit()) pWriter->WriteByte1(5, m_oRefFor->GetValue());
			pWriter->WriteString2(6, m_oRefForName);
			if (m_oRefPtType.IsInit()) pWriter->WriteByte1(7, m_oRefPtType->GetValue());
			if (m_oRefType.IsInit()) pWriter->WriteByte1(8, m_oRefType->GetValue());
			if (m_oType.IsInit()) pWriter->WriteByte1(9, m_oType->GetValue());
			if (m_oVal.IsInit()) pWriter->WriteDoubleReal1(10, m_oVal->GetValue());
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void Diagram::CConstraint::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:constr");

		pWriter->StartAttributes();
			if (m_oType.IsInit()) pWriter->WriteAttribute(L"type", m_oType->ToString());
			if (m_oFor.IsInit()) pWriter->WriteAttribute(L"for", m_oFor->ToString());
			pWriter->WriteAttribute(L"forName", m_oForName);
			if (m_oPtType.IsInit()) pWriter->WriteAttribute(L"ptType", m_oPtType->ToString());
			if (m_oRefPtType.IsInit()) pWriter->WriteAttribute(L"refPtType", m_oRefPtType->ToString());
			if (m_oRefType.IsInit()) pWriter->WriteAttribute(L"refType", m_oRefType->ToString());
			if (m_oRefFor.IsInit()) pWriter->WriteAttribute(L"refFor", m_oRefFor->ToString());
			pWriter->WriteAttribute(L"refForName", m_oRefForName);
			if (m_oOp.IsInit()) pWriter->WriteAttribute(L"op", m_oOp->ToString());
			pWriter->WriteAttribute(L"fact", m_oFact);
			if (m_oVal.IsInit()) pWriter->WriteAttribute(L"val", m_oVal->ToString());
			pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"dgm:constr");
	}
	void Diagram::CConstraint::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"fact", m_oFact)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"for", m_oFor)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"op", m_oOp)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"refFor", m_oRefFor)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"forName", m_oForName)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"refForName", m_oRefForName)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"refType", m_oRefType)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"refPtType", m_oRefPtType)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"ptType", m_oPtType)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"type", m_oType)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//-------------------------------------------------------------------------------------------

	Diagram::CRule::CRule() {}
	Diagram::CRule::~CRule() {}
	std::wstring Diagram::CRule::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CRule::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CRule::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:extLst" == sName)
				m_oExtLst = oReader;
		}
	}
	void Diagram::CRule::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)
			{
				m_oFact.Init();
				m_oFact->SetValue(pReader->GetDoubleReal());
			}
			else if (1 == _at)
			{
				m_oFor.Init();
				m_oFor->SetValueFromByte(pReader->GetUChar());
			}
			else if (2 == _at)	m_oForName = pReader->GetString2();
			else if (3 == _at)
			{
				m_oPtType.Init();
				m_oPtType->SetValueFromByte(pReader->GetUChar());
			}
			else if (4 == _at)
			{
				m_oType.Init();
				m_oType->SetValueFromByte(pReader->GetUChar());
			}
			else if (5 == _at)
			{
				m_oVal.Init();
				m_oVal->SetValue(pReader->GetDoubleReal());
			}
			else if (6 == _at)
			{
				m_oMax.Init();
				m_oMax->SetValue(pReader->GetDoubleReal());
			}
		}
		pReader->Seek(end);
	}
	EElementType Diagram::CRule::getType() const
	{
		return et_dgm_Rule;
	}
	void Diagram::CRule::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			if (m_oFact.IsInit())pWriter->WriteDoubleReal1(0, m_oFact->GetValue());
			if (m_oFor.IsInit()) pWriter->WriteByte1(1, m_oFor->GetValue());
			pWriter->WriteString2(2, m_oForName);
			if (m_oPtType.IsInit()) pWriter->WriteByte1(3, m_oPtType->GetValue());
			if (m_oType.IsInit()) pWriter->WriteByte1(4, m_oType->GetValue());
			if (m_oVal.IsInit()) pWriter->WriteDoubleReal1(5, m_oVal->GetValue());
			if (m_oMax.IsInit()) pWriter->WriteDoubleReal1(6, m_oMax->GetValue());
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void Diagram::CRule::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:rule");

		pWriter->StartAttributes();
			if (m_oType.IsInit()) pWriter->WriteAttribute(L"type", m_oType->ToString());
			if (m_oFor.IsInit()) pWriter->WriteAttribute(L"for", m_oFor->ToString());
			pWriter->WriteAttribute(L"forName", m_oForName);
			if (m_oPtType.IsInit()) pWriter->WriteAttribute(L"ptType", m_oPtType->ToString());
			if (m_oVal.IsInit()) pWriter->WriteAttribute(L"val", m_oVal->ToString());
			if (m_oFact.IsInit()) pWriter->WriteAttribute(L"fact", m_oFact->ToString());
			if (m_oMax.IsInit()) pWriter->WriteAttribute(L"max", m_oMax->ToString());
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"dgm:rule");
	}
	void Diagram::CRule::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"fact", m_oFact)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"for", m_oFor)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"forName", m_oForName)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"ptType", m_oPtType)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"type", m_oType)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"val", m_oVal)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"max", m_oMax)
		WritingElement_ReadAttributes_End(oReader)
	}

//-------------------------------------------------------------------------------------------

	Diagram::CParam::CParam() {}
	Diagram::CParam::~CParam() {}
	std::wstring Diagram::CParam::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CParam::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CParam::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
	}
	EElementType Diagram::CParam::getType() const
	{
		return et_dgm_Param;
	}
	void Diagram::CParam::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_oVal = pReader->GetString2();
			else if (1 == _at)
			{
				m_oType.Init();
				m_oType->SetValueFromByte(pReader->GetUChar());
			}

		}
		pReader->Seek(end);
	}
	void Diagram::CParam::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_oVal);
			if (m_oType.IsInit()) pWriter->WriteByte1(1, m_oType->GetValue());
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void Diagram::CParam::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:param");

		pWriter->StartAttributes();
		if (m_oType.IsInit()) pWriter->WriteAttribute(L"type", m_oType->ToString());
		pWriter->WriteAttribute(L"val", m_oVal);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"dgm:param");
	}
	void Diagram::CParam::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"type", m_oType)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//-------------------------------------------------------------------------------------------

	Diagram::CPresOf::CPresOf() {}
	Diagram::CPresOf::~CPresOf() {}
	std::wstring Diagram::CPresOf::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CPresOf::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CPresOf::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:extLst" == sName)
				m_oExtLst = oReader;
		}

	}
	EElementType Diagram::CPresOf::getType() const
	{
		return et_dgm_PresOf;
	}
	void Diagram::CPresOf::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (1 == _at)	m_arSt.push_back(pReader->GetLong());
			else if (2 == _at)	m_arStep.push_back(pReader->GetLong());
			else if (3 == _at)	m_arHideLastTrans.push_back(pReader->GetBool());
			else if (4 == _at)	m_arCnt.push_back(pReader->GetLong());
			else if (5 == _at)
			{
				SimpleTypes::CAxisTypes axis; axis.SetValueFromByte(pReader->GetUChar());
				m_arAxis.push_back(axis);
			}
			else if (6 == _at)
			{
				SimpleTypes::CElementTypes ptType; ptType.SetValueFromByte(pReader->GetUChar());
				m_arPtType.push_back(ptType);
			}
		}
		pReader->Seek(end);
	}
	void Diagram::CPresOf::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			for (size_t i = 0; i < m_arSt.size(); ++i)
				pWriter->WriteInt1(1, m_arSt[i]);
			for (size_t i = 0; i < m_arStep.size(); ++i)
				pWriter->WriteInt1(2, m_arStep[i]);
			for (size_t i = 0; i < m_arHideLastTrans.size(); ++i)
				pWriter->WriteBool1(3, m_arHideLastTrans[i]);
			for (size_t i = 0; i < m_arCnt.size(); ++i)
				pWriter->WriteInt1(4, m_arCnt[i]);
			for (size_t i = 0; i < m_arAxis.size(); ++i)
				pWriter->WriteByte1(5, m_arAxis[i].GetValue());
			for (size_t i = 0; i < m_arPtType.size(); ++i)
				pWriter->WriteByte1(6, m_arPtType[i].GetValue());
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void Diagram::CPresOf::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:presOf");
			if (false == m_arAxis.empty())
			{
				std::wstring sAxis;
				for (size_t i = 0; i < m_arAxis.size(); ++i) sAxis += L" " + m_arAxis[i].ToString();
				pWriter->WriteAttribute(L"axis", sAxis.substr(1));
			}
			if (false == m_arPtType.empty())
			{
				std::wstring sPtType;
				for (size_t i = 0; i < m_arPtType.size(); ++i) sPtType += L" " + m_arPtType[i].ToString();
				pWriter->WriteAttribute(L"ptType", sPtType.substr(1));
			}
			if (false == m_arSt.empty())
			{
				std::wstring sRes;
				for (size_t i = 0; i < m_arSt.size(); ++i) sRes += L" " + std::to_wstring(m_arSt[i]);
				pWriter->WriteAttribute(L"st", sRes.substr(1));
			}
			if (false == m_arHideLastTrans.empty())
			{
				std::wstring sRes;
				for (size_t i = 0; i < m_arHideLastTrans.size(); ++i) sRes += (m_arHideLastTrans[i] ? L" 1" : L" 0");
				pWriter->WriteAttribute(L"hideLastTrans", sRes.substr(1));
			}
			if (false == m_arCnt.empty())
			{
				std::wstring sRes;
				for (size_t i = 0; i < m_arCnt.size(); ++i) sRes += L" " + std::to_wstring(m_arCnt[i]);
				pWriter->WriteAttribute(L"cnt", sRes.substr(1));
			}
			if (false == m_arStep.empty())
			{
				std::wstring sRes;
				for (size_t i = 0; i < m_arStep.size(); ++i) sRes += L" " + std::to_wstring(m_arStep[i]);
				pWriter->WriteAttribute(L"step", sRes.substr(1));
			}
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"dgm:presOf");
	}
	void Diagram::CPresOf::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		nullable_string sPtTypes, sAxis, sCnt, sStep, sHideLastTrans, sSt;
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"axis", sAxis)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"cnt", sCnt)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"hideLastTrans", sHideLastTrans)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"ptType", sPtTypes)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"st", sSt)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"step", sStep)
		WritingElement_ReadAttributes_End(oReader)

		if (sAxis.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sAxis, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arAxis.push_back(SimpleTypes::CAxisTypes(arStr[i]));
		}
		if (sPtTypes.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sPtTypes, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arPtType.push_back(SimpleTypes::CElementTypes(arStr[i]));
		}
		if (sSt.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sSt, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arSt.push_back(XmlUtils::GetInteger(arStr[i]));
		}
		if (sCnt.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sCnt, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arCnt.push_back(XmlUtils::GetInteger(arStr[i]));
		}
		if (sStep.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sStep, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arStep.push_back(XmlUtils::GetInteger(arStr[i]));
		}
		if (sHideLastTrans.IsInit())
		{
			std::vector<std::wstring> arStr;
			boost::algorithm::split(arStr, *sHideLastTrans, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < arStr.size(); ++i)
				m_arHideLastTrans.push_back(SimpleTypes::COnOff(arStr[i]).ToBool());
		}
	}

//-------------------------------------------------------------------------------------------

	Diagram::CShapeAdjust::CShapeAdjust() {}
	Diagram::CShapeAdjust::~CShapeAdjust() {}
	std::wstring Diagram::CShapeAdjust::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CShapeAdjust::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CShapeAdjust::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
	}
	EElementType Diagram::CShapeAdjust::getType() const
	{
		return et_dgm_ShapeAdjust;
	}
	void Diagram::CShapeAdjust::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_oIdx = pReader->GetULong();
			else if (1 == _at)	m_oVal = pReader->GetDoubleReal();
		}
		pReader->Seek(end);
	}
	void Diagram::CShapeAdjust::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, m_oIdx);
			pWriter->WriteDoubleReal2(1, m_oVal);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void Diagram::CShapeAdjust::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:adj");

		pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"idx", m_oIdx);
			pWriter->WriteAttribute(L"val", m_oVal);
		pWriter->EndAttributes();

		pWriter->WriteNodeEnd(L"dgm:adj");
	}
	void Diagram::CShapeAdjust::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"idx", m_oIdx)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"val", m_oVal)
		WritingElement_ReadAttributes_End(oReader)
	}

//-------------------------------------------------------------------------------------------

	Diagram::CShape::CShape() {}
	Diagram::CShape::~CShape() {}
	std::wstring Diagram::CShape::toXML() const
	{
		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		return oWriter.GetXmlString();
	}
	void Diagram::CShape::fromXML(XmlUtils::CXmlNode& node)
	{
	}
	void Diagram::CShape::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetName();

			if (L"dgm:adjLst" == sName)
				m_oAdjLst = oReader;
			else if (L"dgm:extLst" == sName)
				m_oExtLst = oReader;
		}
	}
	EElementType Diagram::CShape::getType() const
	{
		return et_dgm_Shape;
	}
	void Diagram::CShape::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_sBlip = pReader->GetString2();
			else if (1 == _at)	m_bBlipPhldr = pReader->GetBool();
			else if (2 == _at)	m_bHideGeom = pReader->GetBool();
			else if (3 == _at)	m_bLkTxEntry = pReader->GetBool();
			else if (4 == _at)	m_dRot = pReader->GetDoubleReal();
			else if (5 == _at)	m_nZOrderOff = pReader->GetLong();
			else if (6 == _at)	m_oType = pReader->GetString2();
		}
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					m_oAdjLst.Init();
					m_oAdjLst->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
			}
		}
		pReader->Seek(end);
	}
	void Diagram::CShape::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sBlip);
			pWriter->WriteBool2(1, m_bBlipPhldr);
			pWriter->WriteBool2(2, m_bHideGeom);
			pWriter->WriteBool2(3, m_bLkTxEntry);
			pWriter->WriteDoubleReal2(4, m_dRot);			
			pWriter->WriteInt2(5, m_nZOrderOff);
			pWriter->WriteString2(6, m_oType);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, m_oAdjLst);
	}
	void Diagram::CShape::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:shape");
			pWriter->WriteAttribute(L"rot", m_dRot);
			pWriter->WriteAttribute(L"type", m_oType);
			pWriter->WriteAttribute(L"r:blip", m_sBlip.get_value_or(L""));
			pWriter->WriteAttribute(L"blipPhldr", m_bBlipPhldr);
			pWriter->WriteAttribute(L"lkTxEntry", m_bLkTxEntry);
			pWriter->WriteAttribute(L"zOrderOff", m_nZOrderOff);
			pWriter->WriteAttribute(L"hideGeom", m_bHideGeom);
		pWriter->EndAttributes();

		if (m_oAdjLst.IsInit()) m_oAdjLst->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"dgm:shape");
	}
	void Diagram::CShape::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"blip", m_sBlip)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"blipPhldr", m_bBlipPhldr)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"hideGeom", m_bHideGeom)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"lkTxEntry", m_bLkTxEntry)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"rot", m_dRot)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"zOrderOff", m_nZOrderOff)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"type", m_oType)
		WritingElement_ReadAttributes_End(oReader)
	}

//-------------------------------------------------------------------------------------------
} // namespace OOX
