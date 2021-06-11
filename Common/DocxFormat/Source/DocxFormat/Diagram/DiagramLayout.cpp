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
#include "../../../../../ASCOfficePPTXFile/Editor/BinaryFileReaderWriter.h"

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
	void Diagram::CDiferentData::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

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
		pWriter->WriteRecord2(0, m_oDataModel);
	}
	void Diagram::CDiferentData::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(node_name);
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
	void Diagram::CLayoutNode::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		Diagram_Layout_Read_Xml()
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
	void Diagram::CForEach::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		Diagram_Layout_Read_Xml()
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
			else if (1 == _at)	m_sRef = pReader->GetString2();
			else if (2 == _at)	m_sSt = pReader->GetString2();
			else if (3 == _at)	m_sStep = pReader->GetString2();
			else if (4 == _at)	m_bHideLastTrans = pReader->GetBool();
			else if (5 == _at)	m_nCnt = pReader->GetULong();
			else if (6 == _at)
			{
				m_oAxis.Init();
				m_oAxis->SetValueFromByte(pReader->GetUChar());
			}
			else if (7 == _at)
			{
				m_oPtType.Init();
				m_oPtType->SetValueFromByte(pReader->GetUChar());
			}
		}
		
		Diagram_Layout_From_PPTY()

		pReader->Seek(end);
	}
	void Diagram::CForEach::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sName);
			pWriter->WriteString2(1, m_sRef);
			pWriter->WriteString2(2, m_sSt);
			pWriter->WriteString2(3, m_sStep);
			pWriter->WriteBool2(4, m_bHideLastTrans);
			pWriter->WriteUInt2(5, m_nCnt);
			if (m_oAxis.IsInit()) pWriter->WriteByte1(6, m_oAxis->GetValue());
			if (m_oPtType.IsInit()) pWriter->WriteByte1(7, m_oPtType->GetValue());
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		Diagram_Layout_To_PPTY()
	}
	void Diagram::CForEach::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:forEach");
			pWriter->WriteAttribute(L"name", m_sName);
			if (m_oAxis.IsInit()) pWriter->WriteAttribute(L"axis", m_oAxis->ToString());
			pWriter->WriteAttribute2(L"cnt", m_nCnt);
			if (m_oPtType.IsInit()) pWriter->WriteAttribute(L"ptType", m_oPtType->ToString());
			pWriter->WriteAttribute(L"hideLastTrans", m_bHideLastTrans);
			pWriter->WriteAttribute(L"ref", m_sRef);
			pWriter->WriteAttribute(L"st", m_sSt);
			pWriter->WriteAttribute(L"step", m_sStep);
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
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"name", m_sName)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"axis", m_oAxis)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"cnt", m_nCnt)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"hideLastTrans", m_bHideLastTrans)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"ptType", m_oPtType)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"ref", m_sRef)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"st", m_sSt)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"step", m_sStep)
		WritingElement_ReadAttributes_End(oReader)
	}
//-------------------------------------------------------------------------------------------
	void Diagram::CIf::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		Diagram_Layout_Read_Xml()
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
			else if (1 == _at)	m_sRef = pReader->GetString2();
			else if (2 == _at)	m_sSt = pReader->GetString2();
			else if (3 == _at)	m_sStep = pReader->GetString2();
			else if (4 == _at)	m_bHideLastTrans = pReader->GetBool();
			else if (5 == _at)	m_nCnt = pReader->GetULong();
			else if (6 == _at)
			{
				m_oAxis.Init();
				m_oAxis->SetValueFromByte(pReader->GetUChar());
			}
			else if (7 == _at)
			{
				m_oPtType.Init();
				m_oPtType->SetValueFromByte(pReader->GetUChar());
			}
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
			pWriter->WriteString2(1, m_sRef);
			pWriter->WriteString2(2, m_sSt);
			pWriter->WriteString2(3, m_sStep);
			pWriter->WriteBool2(4, m_bHideLastTrans);
			pWriter->WriteUInt2(5, m_nCnt);
			if (m_oAxis.IsInit()) pWriter->WriteByte1(6, m_oAxis->GetValue());
			if (m_oPtType.IsInit()) pWriter->WriteByte1(7, m_oPtType->GetValue());
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
			if (m_oFunc.IsInit()) pWriter->WriteAttribute(L"func", m_oFunc->ToString());
			pWriter->WriteAttribute(L"arg", m_sArg);
			if (m_oOp.IsInit()) pWriter->WriteAttribute(L"op", m_oOp->ToString());
			if (m_oAxis.IsInit()) pWriter->WriteAttribute(L"axis", m_oAxis->ToString());
			pWriter->WriteAttribute2(L"cnt", m_nCnt);
			if (m_oPtType.IsInit()) pWriter->WriteAttribute(L"ptType", m_oPtType->ToString());
			pWriter->WriteAttribute(L"hideLastTrans", m_bHideLastTrans);
			pWriter->WriteAttribute(L"ref", m_sRef);
			pWriter->WriteAttribute(L"st", m_sSt);
			pWriter->WriteAttribute(L"step", m_sStep);
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
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"name", m_sName)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"axis", m_oAxis)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"cnt", m_nCnt)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"hideLastTrans", m_bHideLastTrans)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"ptType", m_oPtType)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"ref", m_sRef)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"st", m_sSt)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"step", m_sStep)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"arg", m_sArg)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"func", m_oFunc)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"op", m_oOp)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"val", m_sVal)
		WritingElement_ReadAttributes_End(oReader)
	}
//-------------------------------------------------------------------------------------------
	void Diagram::CElse::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		Diagram_Layout_Read_Xml()
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
				m_oIf = oReader;
			}
			else if (L"dgm:else" == sName)
			{
				m_oElse = oReader;
			}
		}
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
					m_oIf.Init();
					m_oIf->fromPPTY(pReader);
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

		pWriter->WriteRecord2(0, m_oIf);
		pWriter->WriteRecord2(1, m_oElse);
	}
	void Diagram::CChoose::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:choose");
		pWriter->WriteAttribute(L"name", m_sName);
		pWriter->EndAttributes();

		if (m_oIf.IsInit()) m_oIf->toXmlWriter(pWriter);
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
			else if (10 == _at)	m_oVal = pReader->GetDoubleReal();
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
			pWriter->WriteDoubleReal2(10, m_oVal);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void Diagram::CConstraint::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:constr");

		pWriter->StartAttributes();
			if (m_oType.IsInit()) pWriter->WriteAttribute(L"type", m_oType->ToString());
			if (m_oFor.IsInit()) pWriter->WriteAttribute(L"for", m_oFor->ToString());
			pWriter->WriteAttribute(L"forName", m_oForName);
			if (m_oRefType.IsInit()) pWriter->WriteAttribute(L"refType", m_oRefType->ToString());
			if (m_oRefFor.IsInit()) pWriter->WriteAttribute(L"refFor", m_oRefFor->ToString());
			pWriter->WriteAttribute(L"refForName", m_oRefForName);
			if (m_oRefPtType.IsInit()) pWriter->WriteAttribute(L"refPtType", m_oRefPtType->ToString());
			if (m_oOp.IsInit()) pWriter->WriteAttribute(L"op", m_oOp->ToString());
			if (m_oPtType.IsInit()) pWriter->WriteAttribute(L"ptType", m_oPtType->ToString());
			pWriter->WriteAttribute(L"fact", m_oFact);
			pWriter->WriteAttribute(L"val", m_oVal);
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

			else if (0 == _at)	m_oFact = pReader->GetDoubleReal();
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
			else if (5 == _at)	m_oVal = pReader->GetDoubleReal();
			else if (6 == _at)	m_oMax = pReader->GetDoubleReal();
		}
		pReader->Seek(end);
	}
	void Diagram::CRule::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteDoubleReal2(0, m_oFact);
			if (m_oFor.IsInit()) pWriter->WriteByte1(1, m_oFor->GetValue());
			pWriter->WriteString2(2, m_oForName);
			if (m_oPtType.IsInit()) pWriter->WriteByte1(3, m_oPtType->GetValue());
			if (m_oType.IsInit()) pWriter->WriteByte1(4, m_oType->GetValue());
			pWriter->WriteDoubleReal2(5, m_oVal);
			pWriter->WriteDoubleReal2(6, m_oMax);
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
			pWriter->WriteAttribute(L"val", m_oVal);
			pWriter->WriteAttribute(L"fact", m_oFact);
			pWriter->WriteAttribute(L"max", m_oMax);
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
	void Diagram::CParam::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
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
	void Diagram::CPresOf::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			else if (0 == _at)	m_sName = pReader->GetString2();
			else if (1 == _at)	m_sSt = pReader->GetString2();
			else if (2 == _at)	m_sStep = pReader->GetString2();
			else if (3 == _at)	m_bHideLastTrans = pReader->GetBool();
			else if (4 == _at)	m_nCnt = pReader->GetULong();
			else if (5 == _at)
			{
				m_oAxis.Init();
				m_oAxis->SetValueFromByte(pReader->GetUChar());
			}
			else if (6 == _at)
			{
				m_oPtType.Init();
				m_oPtType->SetValueFromByte(pReader->GetUChar());
			}
		}
		pReader->Seek(end);
	}
	void Diagram::CPresOf::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sName);
			pWriter->WriteString2(1, m_sSt);
			pWriter->WriteString2(2, m_sStep);
			pWriter->WriteBool2(3, m_bHideLastTrans);
			pWriter->WriteUInt2(4, m_nCnt);
			if (m_oAxis.IsInit()) pWriter->WriteByte1(5, m_oAxis->GetValue());
			if (m_oPtType.IsInit()) pWriter->WriteByte1(6, m_oPtType->GetValue());
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void Diagram::CPresOf::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"dgm:presOf");
			pWriter->WriteAttribute(L"name", m_sName);
			if (m_oAxis.IsInit()) pWriter->WriteAttribute(L"axis", m_oAxis->ToString());
			if (m_oPtType.IsInit()) pWriter->WriteAttribute(L"ptType", m_oPtType->ToString());
			pWriter->WriteAttribute2(L"cnt", m_nCnt);
			pWriter->WriteAttribute(L"st", m_sSt);
			pWriter->WriteAttribute(L"step", m_sStep);
			pWriter->WriteAttribute(L"hideLastTrans", m_bHideLastTrans);
		pWriter->EndAttributes();
		pWriter->WriteNodeEnd(L"dgm:presOf");
	}
	void Diagram::CPresOf::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start(oReader)
			WritingElement_ReadAttributes_Read_if(oReader, L"axis", m_oAxis)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"cnt", m_nCnt)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"hideLastTrans", m_bHideLastTrans)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"ptType", m_oPtType)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"name", m_sName)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"st", m_sSt)
			WritingElement_ReadAttributes_Read_else_if(oReader, L"step", m_sStep)
		WritingElement_ReadAttributes_End(oReader)
	}
//-------------------------------------------------------------------------------------------
	void Diagram::CShapeAdjust::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;
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
		pWriter->WriteAttribute(L"blip", m_sBlip);
			pWriter->WriteAttribute(L"blipPhldr", m_bBlipPhldr);
			pWriter->WriteAttribute(L"hideGeom", m_bHideGeom);
			pWriter->WriteAttribute(L"lkTxEntry", m_bLkTxEntry);
			pWriter->WriteAttribute(L"rot", m_dRot);
			pWriter->WriteAttribute(L"zOrderOff", m_nZOrderOff);
			pWriter->WriteAttribute(L"type", m_oType);
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
