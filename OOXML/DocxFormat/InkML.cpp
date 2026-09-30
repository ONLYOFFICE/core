/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "InkML.h"
#include "../Binary/Presentation/BinaryFileReaderWriter.h"
#include "../Binary/Presentation/XmlWriter.h"

namespace SimpleTypes
{
namespace Ink
{
	EChannelType CChannelType::FromString(const std::wstring& sValue)
	{
		if (L"integer" == sValue) this->m_eValue = channeltypeInteger;
		else if (L"double" == sValue) this->m_eValue = channeltypeDouble;
		else if (L"boolean" == sValue) this->m_eValue = channeltypeBoolean;
		else this->m_eValue = channeltypeDecimal;

		return this->m_eValue;
	}
	std::wstring CChannelType::ToString() const
	{
		switch (this->m_eValue)
		{
		case channeltypeInteger: return L"integer";
		case channeltypeDouble: return L"double";
		case channeltypeBoolean: return L"boolean";
		default: return L"decimal";
		}
	}
	EChannelOrientation CChannelOrientation::FromString(const std::wstring& sValue)
	{
		if (L"-ve" == sValue) this->m_eValue = orientationNegative;
		else this->m_eValue = orientationPositive;

		return this->m_eValue;
	}
	std::wstring CChannelOrientation::ToString() const
	{
		switch (this->m_eValue)
		{
		case orientationNegative: return L"-ve";
		default: return L"+ve";
		}
	}
	ETraceType CTraceType::FromString(const std::wstring& sValue)
	{
		if (L"penUp" == sValue) this->m_eValue = tracetypePenUp;
		else if (L"indeterminate" == sValue) this->m_eValue = tracetypeIndeterminate;
		else this->m_eValue = tracetypePenDown;

		return this->m_eValue;
	}
	std::wstring CTraceType::ToString() const
	{
		switch (this->m_eValue)
		{
		case tracetypePenUp: return L"penUp";
		case tracetypeIndeterminate: return L"indeterminate";
		default: return L"penDown";
		}
	}
	ETraceContinuation CTraceContinuation::FromString(const std::wstring& sValue)
	{
		if (L"end" == sValue) this->m_eValue = continuationEnd;
		else if (L"middle" == sValue) this->m_eValue = continuationMiddle;
		else this->m_eValue = continuationBegin;

		return this->m_eValue;
	}
	std::wstring CTraceContinuation::ToString() const
	{
		switch (this->m_eValue)
		{
		case continuationEnd: return L"end";
		case continuationMiddle: return L"middle";
		default: return L"begin";
		}
	}
	EMappingType CMappingType::FromString(const std::wstring& sValue)
	{
		if (L"identity" == sValue) this->m_eValue = mappingtypeIdentity;
		else if (L"lookup" == sValue) this->m_eValue = mappingtypeLookup;
		else if (L"affine" == sValue) this->m_eValue = mappingtypeAffine;
		else if (L"mathml" == sValue) this->m_eValue = mappingtypeMathML;
		else if (L"product" == sValue) this->m_eValue = mappingtypeProduct;
		else this->m_eValue = mappingtypeUnknown;

		return this->m_eValue;
	}
	std::wstring CMappingType::ToString() const
	{
		switch (this->m_eValue)
		{
		case mappingtypeIdentity: return L"identity";
		case mappingtypeLookup: return L"lookup";
		case mappingtypeAffine: return L"affine";
		case mappingtypeMathML: return L"mathml";
		case mappingtypeProduct: return L"product";
		default: return L"unknown";
		}
	}
	ETableApply CTableApply::FromString(const std::wstring& sValue)
	{
		if (L"relative" == sValue) this->m_eValue = applyRelative;
		else this->m_eValue = applyAbsolute;

		return this->m_eValue;
	}
	std::wstring CTableApply::ToString() const
	{
		switch (this->m_eValue)
		{
		case applyRelative: return L"relative";
		default: return L"absolute";
		}
	}
	ETableInterpolation CTableInterpolation::FromString(const std::wstring& sValue)
	{
		if (L"middle" == sValue) this->m_eValue = interpolationMiddle;
		else if (L"ceiling" == sValue) this->m_eValue = interpolationCeiling;
		else if (L"linear" == sValue) this->m_eValue = interpolationLinear;
		else if (L"cubic" == sValue) this->m_eValue = interpolationCubic;
		else this->m_eValue = interpolationFloor;

		return this->m_eValue;
	}
	std::wstring CTableInterpolation::ToString() const
	{
		switch (this->m_eValue)
		{
		case interpolationMiddle: return L"middle";
		case interpolationCeiling: return L"ceiling";
		case interpolationLinear: return L"linear";
		case interpolationCubic: return L"cubic";
		default: return L"floor";
		}
	}
}// Ink
} // SimpleTypes

namespace OOX
{
	WritingElement* CreateInkElement(const std::wstring& sName)
	{
		if (L"annotation" == sName) return new CInkAnnotation();
		else if (L"annotationXML" == sName) return new CInkAnnotationXML();
		else if (L"brush" == sName) return new CInkBrush();
		else if (L"brushProperty" == sName) return new CInkBrushProperty();
		else if (L"canvas" == sName) return new CInkCanvas();
		else if (L"canvasTransform" == sName) return new CInkCanvasTransform();
		else if (L"context" == sName) return new CInkContext();
		else if (L"inkSource" == sName) return new CInkSource();
		else if (L"mapping" == sName) return new CInkMapping();
		else if (L"timestamp" == sName) return new CInkTimestamp();
		else if (L"trace" == sName) return new CInkTrace();
		else if (L"traceFormat" == sName) return new CInkTraceFormat();
		else if (L"traceGroup" == sName) return new CInkTraceGroup();
		else if (L"traceView" == sName) return new CInkTraceView();
		else if (L"bind" == sName) return new CInkBind();
		else if (L"table" == sName) return new CInkTable();
		else if (L"matrix" == sName) return new CInkMatrix();

		return NULL;
	}
	WritingElement* CreateInkElement(BYTE nType)
	{
		switch (nType)
		{
		case 0: return new CInkAnnotation();
		case 1: return new CInkAnnotationXML();
		case 2: return new CInkBrush();
		case 3: return new CInkBrushProperty();
		case 4: return new CInkCanvas();
		case 5: return new CInkCanvasTransform();
		case 6: return new CInkContext();
		case 7: return new CInkSource();
		case 8: return new CInkMapping();
		case 9: return new CInkTimestamp();
		case 10: return new CInkTrace();
		case 11: return new CInkTraceFormat();
		case 12: return new CInkTraceGroup();
		case 13: return new CInkTraceView();
		case 14: return new CInkBind();
		case 15: return new CInkTable();
		case 16: return new CInkMatrix();
		}
		return NULL;
	}
	BYTE GetInkRecordType(EElementType eType)
	{
		switch (eType)
		{
		case et_inkml_annotation: return 0;
		case et_inkml_annotationXML: return 1;
		case et_inkml_brush: return 2;
		case et_inkml_brushProperty: return 3;
		case et_inkml_canvas: return 4;
		case et_inkml_canvasTransform: return 5;
		case et_inkml_context: return 6;
		case et_inkml_inkSource: return 7;
		case et_inkml_mapping: return 8;
		case et_inkml_timestamp: return 9;
		case et_inkml_trace: return 10;
		case et_inkml_traceFormat: return 11;
		case et_inkml_traceGroup: return 12;
		case et_inkml_traceView: return 13;
		case et_inkml_bind: return 14;
		case et_inkml_table: return 15;
		case et_inkml_matrix: return 16;
		case et_inkml_definitions: return 17;
		default: return 0xff;
		}
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkAnnotation::getType() const
	{
		return et_inkml_annotation;
	}
	void CInkAnnotation::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "type", m_sType)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "encoding", m_sEncoding)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkAnnotation::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		m_sText = oReader.GetText2();
	}
	void CInkAnnotation::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:annotation");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"type", m_sType);
		pWriter->WriteAttribute2(L"encoding", m_sEncoding);
		pWriter->EndAttributes();

		pWriter->WriteStringXML(m_sText);

		pWriter->WriteNodeEnd(L"inkml:annotation");
	}
	void CInkAnnotation::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sType);
		pWriter->WriteString2(1, m_sEncoding);
		if (false == m_sText.empty())
			pWriter->WriteString1(2, m_sText);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CInkAnnotation::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sType = pReader->GetString2();
			}break;
			case 1:
			{
				m_sEncoding = pReader->GetString2();
			}break;
			case 2:
			{
				m_sText = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkAnnotationXML::getType() const
	{
		return et_inkml_annotationXML;
	}
	void CInkAnnotationXML::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "type", m_sType)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "encoding", m_sEncoding)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "href", m_sHref)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkAnnotationXML::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		m_sContent = oReader.GetInnerXml();
	}
	void CInkAnnotationXML::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:annotationXML");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"type", m_sType);
		pWriter->WriteAttribute2(L"encoding", m_sEncoding);
		pWriter->WriteAttribute2(L"href", m_sHref);

		if (m_sContent.empty())
		{
			pWriter->WriteNodeEnd(L"inkml:annotationXML", true);
			return;
		}
		pWriter->EndAttributes();

		pWriter->WriteString(m_sContent);

		pWriter->WriteNodeEnd(L"inkml:annotationXML");
	}
	void CInkAnnotationXML::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sType);
		pWriter->WriteString2(1, m_sEncoding);
		pWriter->WriteString2(2, m_sHref);
		if (false == m_sContent.empty())
			pWriter->WriteString1(3, m_sContent);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CInkAnnotationXML::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sType = pReader->GetString2();
			}break;
			case 1:
			{
				m_sEncoding = pReader->GetString2();
			}break;
			case 2:
			{
				m_sHref = pReader->GetString2();
			}break;
			case 3:
			{
				m_sContent = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkBind::getType() const
	{
		return et_inkml_bind;
	}
	void CInkBind::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "source", m_sSource)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "target", m_sTarget)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "column", m_sColumn)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "variable", m_sVariable)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkBind::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
	}
	void CInkBind::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:bind");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"source", m_sSource);
		pWriter->WriteAttribute2(L"target", m_sTarget);
		pWriter->WriteAttribute2(L"column", m_sColumn);
		pWriter->WriteAttribute2(L"variable", m_sVariable);
		pWriter->WriteNodeEnd(L"inkml:bind", true);
	}
	void CInkBind::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sSource);
		pWriter->WriteString2(1, m_sTarget);
		pWriter->WriteString2(2, m_sColumn);
		pWriter->WriteString2(3, m_sVariable);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CInkBind::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sSource = pReader->GetString2();
			}break;
			case 1:
			{
				m_sTarget = pReader->GetString2();
			}break;
			case 2:
			{
				m_sColumn = pReader->GetString2();
			}break;
			case 3:
			{
				m_sVariable = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkTable::getType() const
	{
		return et_inkml_table;
	}
	void CInkTable::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "id", m_sId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "apply", m_oApply)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "interpolation", m_oInterpolation)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkTable::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		m_sData = oReader.GetText2();
	}
	void CInkTable::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:table");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xml:id", m_sId);
		if (m_oApply.IsInit()) pWriter->WriteAttribute(L"apply", m_oApply->ToString());
		if (m_oInterpolation.IsInit()) pWriter->WriteAttribute(L"interpolation", m_oInterpolation->ToString());
		pWriter->EndAttributes();

		pWriter->WriteString(m_sData);

		pWriter->WriteNodeEnd(L"inkml:table");
	}
	void CInkTable::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sId);
		if (m_oApply.IsInit()) pWriter->WriteByte1(1, m_oApply->GetValue());
		if (m_oInterpolation.IsInit()) pWriter->WriteByte1(2, m_oInterpolation->GetValue());
		if (false == m_sData.empty())
			pWriter->WriteString1(3, m_sData);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CInkTable::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sId = pReader->GetString2();
			}break;
			case 1:
			{
				m_oApply.Init();
				m_oApply->SetValueFromByte(pReader->GetUChar());
			}break;
			case 2:
			{
				m_oInterpolation.Init();
				m_oInterpolation->SetValueFromByte(pReader->GetUChar());
			}break;
			case 3:
			{
				m_sData = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkMatrix::getType() const
	{
		return et_inkml_matrix;
	}
	void CInkMatrix::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "id", m_sId)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkMatrix::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		m_sData = oReader.GetText2();
	}
	void CInkMatrix::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:matrix");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xml:id", m_sId);
		pWriter->EndAttributes();

		pWriter->WriteString(m_sData);

		pWriter->WriteNodeEnd(L"inkml:matrix");
	}
	void CInkMatrix::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sId);
		if (false == m_sData.empty())
			pWriter->WriteString1(1, m_sData);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CInkMatrix::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sId = pReader->GetString2();
			}break;
			case 1:
			{
				m_sData = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkMapping::getType() const
	{
		return et_inkml_mapping;
	}
	void CInkMapping::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "id", m_sId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "type", m_oType)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "mappingRef", m_sMappingRef)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkMapping::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"math" == sName)
			{
				m_sMathML = oReader.GetOuterXml();
				continue;
			}
			WritingElement* pItem = CreateInkElement(sName);
			if (pItem)
			{
				pItem->fromXML(oReader);
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CInkMapping::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:mapping");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xml:id", m_sId);
		if (m_oType.IsInit()) pWriter->WriteAttribute(L"type", m_oType->ToString());
		pWriter->WriteAttribute2(L"mappingRef", m_sMappingRef);

		if (m_arrItems.empty() && m_sMathML.empty())
		{
			pWriter->WriteNodeEnd(L"inkml:mapping", true);
			return;
		}
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteString(m_sMathML);

		pWriter->WriteNodeEnd(L"inkml:mapping");
	}
	void CInkMapping::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sId);
		if (m_oType.IsInit()) pWriter->WriteByte1(1, m_oType->GetValue());
		pWriter->WriteString2(2, m_sMappingRef);
		if (false == m_sMathML.empty())
			pWriter->WriteString1(3, m_sMathML);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			BYTE type = GetInkRecordType(m_arrItems[i]->getType());
			if (type != 0xff)
				pWriter->WriteRecord2(type, m_arrItems[i]);
		}
	}
	void CInkMapping::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sId = pReader->GetString2();
			}break;
			case 1:
			{
				m_oType.Init();
				m_oType->SetValueFromByte(pReader->GetUChar());
			}break;
			case 2:
			{
				m_sMappingRef = pReader->GetString2();
			}break;
			case 3:
			{
				m_sMathML = pReader->GetString2();
			}break;
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();

			WritingElement* pItem = CreateInkElement(_rec);
			if (pItem)
			{
				pItem->fromPPTY(pReader);
				m_arrItems.push_back(pItem);
			}
			else
			{
				pReader->SkipRecord();
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkChannel::getType() const
	{
		return et_inkml_channel;
	}
	void CInkChannel::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "id", m_sId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "name", m_sName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "type", m_oType)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "default", m_sDefault)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "min", m_sMin)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "max", m_sMax)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "orientation", m_oOrientation)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "respectTo", m_sRespectTo)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "units", m_sUnits)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkChannel::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"mapping" == sName)
				m_oMapping = oReader;
		}
	}
	void CInkChannel::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:channel");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xml:id", m_sId);
		pWriter->WriteAttribute2(L"name", m_sName);
		if (m_oType.IsInit()) pWriter->WriteAttribute(L"type", m_oType->ToString());
		pWriter->WriteAttribute2(L"default", m_sDefault);
		pWriter->WriteAttribute2(L"min", m_sMin);
		pWriter->WriteAttribute2(L"max", m_sMax);
		if (m_oOrientation.IsInit()) pWriter->WriteAttribute(L"orientation", m_oOrientation->ToString());
		pWriter->WriteAttribute2(L"respectTo", m_sRespectTo);
		pWriter->WriteAttribute2(L"units", m_sUnits);

		if (false == m_oMapping.IsInit())
		{
			pWriter->WriteNodeEnd(L"inkml:channel", true);
			return;
		}
		pWriter->EndAttributes();

		m_oMapping->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"inkml:channel");
	}
	void CInkChannel::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sId);
		pWriter->WriteString2(1, m_sName);
		if (m_oType.IsInit()) pWriter->WriteByte1(2, m_oType->GetValue());
		pWriter->WriteString2(3, m_sDefault);
		pWriter->WriteString2(4, m_sMin);
		pWriter->WriteString2(5, m_sMax);
		if (m_oOrientation.IsInit()) pWriter->WriteByte1(6, m_oOrientation->GetValue());
		pWriter->WriteString2(7, m_sRespectTo);
		pWriter->WriteString2(8, m_sUnits);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, m_oMapping);
	}
	void CInkChannel::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sId = pReader->GetString2();
			}break;
			case 1:
			{
				m_sName = pReader->GetString2();
			}break;
			case 2:
			{
				m_oType.Init();
				m_oType->SetValueFromByte(pReader->GetUChar());
			}break;
			case 3:
			{
				m_sDefault = pReader->GetString2();
			}break;
			case 4:
			{
				m_sMin = pReader->GetString2();
			}break;
			case 5:
			{
				m_sMax = pReader->GetString2();
			}break;
			case 6:
			{
				m_oOrientation.Init();
				m_oOrientation->SetValueFromByte(pReader->GetUChar());
			}break;
			case 7:
			{
				m_sRespectTo = pReader->GetString2();
			}break;
			case 8:
			{
				m_sUnits = pReader->GetString2();
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
				m_oMapping.Init();
				m_oMapping->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkIntermittentChannels::getType() const
	{
		return et_inkml_intermittentChannels;
	}
	void CInkIntermittentChannels::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"channel" == sName)
			{
				CInkChannel* pItem = new CInkChannel();
				*pItem = oReader;
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CInkIntermittentChannels::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:intermittentChannels");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"inkml:intermittentChannels");
	}
	void CInkIntermittentChannels::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CInkIntermittentChannels::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CInkChannel());
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
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkTraceFormat::getType() const
	{
		return et_inkml_traceFormat;
	}
	void CInkTraceFormat::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "id", m_sId)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkTraceFormat::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"channel" == sName)
			{
				CInkChannel* pItem = new CInkChannel();
				*pItem = oReader;
				m_arrItems.push_back(pItem);
			}
			else if (L"intermittentChannels" == sName)
			{
				m_oIntermittentChannels = oReader;
			}
		}
	}
	void CInkTraceFormat::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:traceFormat");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xml:id", m_sId);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		if (m_oIntermittentChannels.IsInit())
			m_oIntermittentChannels->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"inkml:traceFormat");
	}
	void CInkTraceFormat::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sId);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));

		pWriter->WriteRecord2(1, m_oIntermittentChannels);
	}
	void CInkTraceFormat::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sId = pReader->GetString2();
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
				m_arrItems.push_back(new CInkChannel());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			case 1:
			{
				m_oIntermittentChannels.Init();
				m_oIntermittentChannels->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkChannelProperty::getType() const
	{
		return et_inkml_channelProperty;
	}
	void CInkChannelProperty::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "channel", m_sChannel)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "name", m_sName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "value", m_sValue)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "units", m_sUnits)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkChannelProperty::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
	}
	void CInkChannelProperty::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:channelProperty");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"channel", m_sChannel);
		pWriter->WriteAttribute2(L"name", m_sName);
		pWriter->WriteAttribute2(L"value", m_sValue);
		pWriter->WriteAttribute2(L"units", m_sUnits);
		pWriter->WriteNodeEnd(L"inkml:channelProperty", true);
	}
	void CInkChannelProperty::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sChannel);
		pWriter->WriteString2(1, m_sName);
		pWriter->WriteString2(2, m_sValue);
		pWriter->WriteString2(3, m_sUnits);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CInkChannelProperty::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sChannel = pReader->GetString2();
			}break;
			case 1:
			{
				m_sName = pReader->GetString2();
			}break;
			case 2:
			{
				m_sValue = pReader->GetString2();
			}break;
			case 3:
			{
				m_sUnits = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkChannelProperties::getType() const
	{
		return et_inkml_channelProperties;
	}
	void CInkChannelProperties::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"channelProperty" == sName)
			{
				CInkChannelProperty* pItem = new CInkChannelProperty();
				*pItem = oReader;
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CInkChannelProperties::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:channelProperties");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"inkml:channelProperties");
	}
	void CInkChannelProperties::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CInkChannelProperties::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CInkChannelProperty());
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
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkSampleRate::getType() const
	{
		return et_inkml_sampleRate;
	}
	void CInkSampleRate::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "uniform", m_bUniform)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "value", m_dValue)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkSampleRate::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
	}
	void CInkSampleRate::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:sampleRate");
		pWriter->StartAttributes();
		pWriter->WriteAttribute(L"uniform", m_bUniform);
		pWriter->WriteAttribute(L"value", m_dValue);
		pWriter->WriteNodeEnd(L"inkml:sampleRate", true);
	}
	void CInkSampleRate::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteBool2(0, m_bUniform);
		pWriter->WriteDoubleReal2(1, m_dValue);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CInkSampleRate::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_bUniform = pReader->GetBool();
			}break;
			case 1:
			{
				m_dValue = pReader->GetDoubleReal();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkLatency::getType() const
	{
		return et_inkml_latency;
	}
	void CInkLatency::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "value", m_dValue)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkLatency::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
	}
	void CInkLatency::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:latency");
		pWriter->StartAttributes();
		pWriter->WriteAttribute(L"value", m_dValue);
		pWriter->WriteNodeEnd(L"inkml:latency", true);
	}
	void CInkLatency::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteDoubleReal2(0, m_dValue);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CInkLatency::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_dValue = pReader->GetDoubleReal();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkActiveArea::getType() const
	{
		return et_inkml_activeArea;
	}
	void CInkActiveArea::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "size", m_sSize)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "height", m_dHeight)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "width", m_dWidth)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "units", m_sUnits)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkActiveArea::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
	}
	void CInkActiveArea::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:activeArea");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"size", m_sSize);
		pWriter->WriteAttribute(L"height", m_dHeight);
		pWriter->WriteAttribute(L"width", m_dWidth);
		pWriter->WriteAttribute2(L"units", m_sUnits);
		pWriter->WriteNodeEnd(L"inkml:activeArea", true);
	}
	void CInkActiveArea::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sSize);
		pWriter->WriteDoubleReal2(1, m_dHeight);
		pWriter->WriteDoubleReal2(2, m_dWidth);
		pWriter->WriteString2(3, m_sUnits);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CInkActiveArea::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sSize = pReader->GetString2();
			}break;
			case 1:
			{
				m_dHeight = pReader->GetDoubleReal();
			}break;
			case 2:
			{
				m_dWidth = pReader->GetDoubleReal();
			}break;
			case 3:
			{
				m_sUnits = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkSrcProperty::getType() const
	{
		return et_inkml_srcProperty;
	}
	void CInkSrcProperty::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "name", m_sName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "value", m_sValue)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "units", m_sUnits)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkSrcProperty::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
	}
	void CInkSrcProperty::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:srcProperty");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"name", m_sName);
		pWriter->WriteAttribute2(L"value", m_sValue);
		pWriter->WriteAttribute2(L"units", m_sUnits);
		pWriter->WriteNodeEnd(L"inkml:srcProperty", true);
	}
	void CInkSrcProperty::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sName);
		pWriter->WriteString2(1, m_sValue);
		pWriter->WriteString2(2, m_sUnits);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CInkSrcProperty::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sName = pReader->GetString2();
			}break;
			case 1:
			{
				m_sValue = pReader->GetString2();
			}break;
			case 2:
			{
				m_sUnits = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkSource::getType() const
	{
		return et_inkml_inkSource;
	}
	void CInkSource::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "id", m_sId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "manufacturer", m_sManufacturer)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "model", m_sModel)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "serialNo", m_sSerialNo)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "specificationRef", m_sSpecificationRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "description", m_sDescription)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkSource::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"traceFormat" == sName)
				m_oTraceFormat = oReader;
			else if (L"sampleRate" == sName)
				m_oSampleRate = oReader;
			else if (L"latency" == sName)
				m_oLatency = oReader;
			else if (L"activeArea" == sName)
				m_oActiveArea = oReader;
			else if (L"channelProperties" == sName)
				m_oChannelProperties = oReader;
			else if (L"srcProperty" == sName)
			{
				CInkSrcProperty* pItem = new CInkSrcProperty();
				*pItem = oReader;
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CInkSource::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:inkSource");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xml:id", m_sId);
		pWriter->WriteAttribute2(L"manufacturer", m_sManufacturer);
		pWriter->WriteAttribute2(L"model", m_sModel);
		pWriter->WriteAttribute2(L"serialNo", m_sSerialNo);
		pWriter->WriteAttribute2(L"specificationRef", m_sSpecificationRef);
		pWriter->WriteAttribute2(L"description", m_sDescription);
		pWriter->EndAttributes();

		if (m_oTraceFormat.IsInit())
			m_oTraceFormat->toXmlWriter(pWriter);
		if (m_oSampleRate.IsInit())
			m_oSampleRate->toXmlWriter(pWriter);
		if (m_oLatency.IsInit())
			m_oLatency->toXmlWriter(pWriter);
		if (m_oActiveArea.IsInit())
			m_oActiveArea->toXmlWriter(pWriter);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		if (m_oChannelProperties.IsInit())
			m_oChannelProperties->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"inkml:inkSource");
	}
	void CInkSource::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sId);
		pWriter->WriteString2(1, m_sManufacturer);
		pWriter->WriteString2(2, m_sModel);
		pWriter->WriteString2(3, m_sSerialNo);
		pWriter->WriteString2(4, m_sSpecificationRef);
		pWriter->WriteString2(5, m_sDescription);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, m_oTraceFormat);
		pWriter->WriteRecord2(1, m_oSampleRate);
		pWriter->WriteRecord2(2, m_oLatency);
		pWriter->WriteRecord2(3, m_oActiveArea);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(4, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));

		pWriter->WriteRecord2(5, m_oChannelProperties);
	}
	void CInkSource::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sId = pReader->GetString2();
			}break;
			case 1:
			{
				m_sManufacturer = pReader->GetString2();
			}break;
			case 2:
			{
				m_sModel = pReader->GetString2();
			}break;
			case 3:
			{
				m_sSerialNo = pReader->GetString2();
			}break;
			case 4:
			{
				m_sSpecificationRef = pReader->GetString2();
			}break;
			case 5:
			{
				m_sDescription = pReader->GetString2();
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
				m_oTraceFormat.Init();
				m_oTraceFormat->fromPPTY(pReader);
			}break;
			case 1:
			{
				m_oSampleRate.Init();
				m_oSampleRate->fromPPTY(pReader);
			}break;
			case 2:
			{
				m_oLatency.Init();
				m_oLatency->fromPPTY(pReader);
			}break;
			case 3:
			{
				m_oActiveArea.Init();
				m_oActiveArea->fromPPTY(pReader);
			}break;
			case 4:
			{
				m_arrItems.push_back(new CInkSrcProperty());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			case 5:
			{
				m_oChannelProperties.Init();
				m_oChannelProperties->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkBrushProperty::getType() const
	{
		return et_inkml_brushProperty;
	}
	void CInkBrushProperty::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "name", m_sName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "value", m_sValue)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "units", m_sUnits)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkBrushProperty::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			WritingElement* pItem = CreateInkElement(XmlUtils::GetNameNoNS(oReader.GetName()));
			if (pItem)
			{
				pItem->fromXML(oReader);
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CInkBrushProperty::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:brushProperty");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"name", m_sName);
		pWriter->WriteAttribute2(L"value", m_sValue);
		pWriter->WriteAttribute2(L"units", m_sUnits);

		if (m_arrItems.empty())
		{
			pWriter->WriteNodeEnd(L"inkml:brushProperty", true);
			return;
		}
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"inkml:brushProperty");
	}
	void CInkBrushProperty::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sName);
		pWriter->WriteString2(1, m_sValue);
		pWriter->WriteString2(2, m_sUnits);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			BYTE type = GetInkRecordType(m_arrItems[i]->getType());
			if (type != 0xff)
				pWriter->WriteRecord2(type, m_arrItems[i]);
		}
	}
	void CInkBrushProperty::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sName = pReader->GetString2();
			}break;
			case 1:
			{
				m_sValue = pReader->GetString2();
			}break;
			case 2:
			{
				m_sUnits = pReader->GetString2();
			}break;
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();

			WritingElement* pItem = CreateInkElement(_rec);
			if (pItem)
			{
				pItem->fromPPTY(pReader);
				m_arrItems.push_back(pItem);
			}
			else
			{
				pReader->SkipRecord();
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkBrush::getType() const
	{
		return et_inkml_brush;
	}
	void CInkBrush::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "id", m_sId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "brushRef", m_sBrushRef)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkBrush::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			WritingElement* pItem = CreateInkElement(XmlUtils::GetNameNoNS(oReader.GetName()));
			if (pItem)
			{
				pItem->fromXML(oReader);
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CInkBrush::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:brush");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xml:id", m_sId);
		pWriter->WriteAttribute2(L"brushRef", m_sBrushRef);

		if (m_arrItems.empty())
		{
			pWriter->WriteNodeEnd(L"inkml:brush", true);
			return;
		}
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"inkml:brush");
	}
	void CInkBrush::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sId);
		pWriter->WriteString2(1, m_sBrushRef);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			BYTE type = GetInkRecordType(m_arrItems[i]->getType());
			if (type != 0xff)
				pWriter->WriteRecord2(type, m_arrItems[i]);
		}
	}
	void CInkBrush::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sId = pReader->GetString2();
			}break;
			case 1:
			{
				m_sBrushRef = pReader->GetString2();
			}break;
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();

			WritingElement* pItem = CreateInkElement(_rec);
			if (pItem)
			{
				pItem->fromPPTY(pReader);
				m_arrItems.push_back(pItem);
			}
			else
			{
				pReader->SkipRecord();
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkTimestamp::getType() const
	{
		return et_inkml_timestamp;
	}
	void CInkTimestamp::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "id", m_sId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "time", m_dTime)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "timestampRef", m_sTimestampRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "timeString", m_sTimeString)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "timeOffset", m_dTimeOffset)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkTimestamp::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);
	}
	void CInkTimestamp::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:timestamp");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xml:id", m_sId);
		pWriter->WriteAttribute(L"time", m_dTime);
		pWriter->WriteAttribute2(L"timestampRef", m_sTimestampRef);
		pWriter->WriteAttribute2(L"timeString", m_sTimeString);
		pWriter->WriteAttribute(L"timeOffset", m_dTimeOffset);
		pWriter->WriteNodeEnd(L"inkml:timestamp", true);
	}
	void CInkTimestamp::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sId);
		pWriter->WriteDoubleReal2(1, m_dTime);
		pWriter->WriteString2(2, m_sTimestampRef);
		pWriter->WriteString2(3, m_sTimeString);
		pWriter->WriteDoubleReal2(4, m_dTimeOffset);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CInkTimestamp::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sId = pReader->GetString2();
			}break;
			case 1:
			{
				m_dTime = pReader->GetDoubleReal();
			}break;
			case 2:
			{
				m_sTimestampRef = pReader->GetString2();
			}break;
			case 3:
			{
				m_sTimeString = pReader->GetString2();
			}break;
			case 4:
			{
				m_dTimeOffset = pReader->GetDoubleReal();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkCanvas::getType() const
	{
		return et_inkml_canvas;
	}
	void CInkCanvas::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "id", m_sId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "traceFormatRef", m_sTraceFormatRef)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkCanvas::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"traceFormat" == sName)
				m_oTraceFormat = oReader;
		}
	}
	void CInkCanvas::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:canvas");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xml:id", m_sId);
		pWriter->WriteAttribute2(L"traceFormatRef", m_sTraceFormatRef);

		if (false == m_oTraceFormat.IsInit())
		{
			pWriter->WriteNodeEnd(L"inkml:canvas", true);
			return;
		}
		pWriter->EndAttributes();

		m_oTraceFormat->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"inkml:canvas");
	}
	void CInkCanvas::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sId);
		pWriter->WriteString2(1, m_sTraceFormatRef);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, m_oTraceFormat);
	}
	void CInkCanvas::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sId = pReader->GetString2();
			}break;
			case 1:
			{
				m_sTraceFormatRef = pReader->GetString2();
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
				m_oTraceFormat.Init();
				m_oTraceFormat->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkCanvasTransform::getType() const
	{
		return et_inkml_canvasTransform;
	}
	void CInkCanvasTransform::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "id", m_sId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "invertible", m_bInvertible)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkCanvasTransform::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"mapping" == sName)
			{
				CInkMapping* pItem = new CInkMapping();
				*pItem = oReader;
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CInkCanvasTransform::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:canvasTransform");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xml:id", m_sId);
		pWriter->WriteAttribute(L"invertible", m_bInvertible);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"inkml:canvasTransform");
	}
	void CInkCanvasTransform::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sId);
		pWriter->WriteBool2(1, m_bInvertible);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CInkCanvasTransform::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sId = pReader->GetString2();
			}break;
			case 1:
			{
				m_bInvertible = pReader->GetBool();
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
				m_arrItems.push_back(new CInkMapping());
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
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkContext::getType() const
	{
		return et_inkml_context;
	}
	void CInkContext::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "id", m_sId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "contextRef", m_sContextRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "canvasRef", m_sCanvasRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "canvasTransformRef", m_sCanvasTransformRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "traceFormatRef", m_sTraceFormatRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "inkSourceRef", m_sInkSourceRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "brushRef", m_sBrushRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "timestampRef", m_sTimestampRef)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkContext::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"canvas" == sName)
				m_oCanvas = oReader;
			else if (L"canvasTransform" == sName)
				m_oCanvasTransform = oReader;
			else if (L"traceFormat" == sName)
				m_oTraceFormat = oReader;
			else if (L"inkSource" == sName)
				m_oInkSource = oReader;
			else if (L"brush" == sName)
				m_oBrush = oReader;
			else if (L"timestamp" == sName)
				m_oTimestamp = oReader;
		}
	}
	void CInkContext::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:context");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xml:id", m_sId);
		pWriter->WriteAttribute2(L"contextRef", m_sContextRef);
		pWriter->WriteAttribute2(L"canvasRef", m_sCanvasRef);
		pWriter->WriteAttribute2(L"canvasTransformRef", m_sCanvasTransformRef);
		pWriter->WriteAttribute2(L"traceFormatRef", m_sTraceFormatRef);
		pWriter->WriteAttribute2(L"inkSourceRef", m_sInkSourceRef);
		pWriter->WriteAttribute2(L"brushRef", m_sBrushRef);
		pWriter->WriteAttribute2(L"timestampRef", m_sTimestampRef);

		if (false == m_oCanvas.IsInit() && false == m_oCanvasTransform.IsInit() && false == m_oTraceFormat.IsInit() &&
			false == m_oInkSource.IsInit() && false == m_oBrush.IsInit() && false == m_oTimestamp.IsInit())
		{
			pWriter->WriteNodeEnd(L"inkml:context", true);
			return;
		}
		pWriter->EndAttributes();

		if (m_oCanvas.IsInit())
			m_oCanvas->toXmlWriter(pWriter);
		if (m_oCanvasTransform.IsInit())
			m_oCanvasTransform->toXmlWriter(pWriter);
		if (m_oTraceFormat.IsInit())
			m_oTraceFormat->toXmlWriter(pWriter);
		if (m_oInkSource.IsInit())
			m_oInkSource->toXmlWriter(pWriter);
		if (m_oBrush.IsInit())
			m_oBrush->toXmlWriter(pWriter);
		if (m_oTimestamp.IsInit())
			m_oTimestamp->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"inkml:context");
	}
	void CInkContext::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sId);
		pWriter->WriteString2(1, m_sContextRef);
		pWriter->WriteString2(2, m_sCanvasRef);
		pWriter->WriteString2(3, m_sCanvasTransformRef);
		pWriter->WriteString2(4, m_sTraceFormatRef);
		pWriter->WriteString2(5, m_sInkSourceRef);
		pWriter->WriteString2(6, m_sBrushRef);
		pWriter->WriteString2(7, m_sTimestampRef);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, m_oCanvas);
		pWriter->WriteRecord2(1, m_oCanvasTransform);
		pWriter->WriteRecord2(2, m_oTraceFormat);
		pWriter->WriteRecord2(3, m_oInkSource);
		pWriter->WriteRecord2(4, m_oBrush);
		pWriter->WriteRecord2(5, m_oTimestamp);
	}
	void CInkContext::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sId = pReader->GetString2();
			}break;
			case 1:
			{
				m_sContextRef = pReader->GetString2();
			}break;
			case 2:
			{
				m_sCanvasRef = pReader->GetString2();
			}break;
			case 3:
			{
				m_sCanvasTransformRef = pReader->GetString2();
			}break;
			case 4:
			{
				m_sTraceFormatRef = pReader->GetString2();
			}break;
			case 5:
			{
				m_sInkSourceRef = pReader->GetString2();
			}break;
			case 6:
			{
				m_sBrushRef = pReader->GetString2();
			}break;
			case 7:
			{
				m_sTimestampRef = pReader->GetString2();
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
				m_oCanvas.Init();
				m_oCanvas->fromPPTY(pReader);
			}break;
			case 1:
			{
				m_oCanvasTransform.Init();
				m_oCanvasTransform->fromPPTY(pReader);
			}break;
			case 2:
			{
				m_oTraceFormat.Init();
				m_oTraceFormat->fromPPTY(pReader);
			}break;
			case 3:
			{
				m_oInkSource.Init();
				m_oInkSource->fromPPTY(pReader);
			}break;
			case 4:
			{
				m_oBrush.Init();
				m_oBrush->fromPPTY(pReader);
			}break;
			case 5:
			{
				m_oTimestamp.Init();
				m_oTimestamp->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkTrace::getType() const
	{
		return et_inkml_trace;
	}
	void CInkTrace::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "id", m_sId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "type", m_oType)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "continuation", m_oContinuation)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "priorRef", m_sPriorRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "contextRef", m_sContextRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "brushRef", m_sBrushRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "duration", m_dDuration)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "timeOffset", m_dTimeOffset)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkTrace::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		m_sData = oReader.GetText2();
	}
	void CInkTrace::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:trace");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xml:id", m_sId);
		if (m_oType.IsInit()) pWriter->WriteAttribute(L"type", m_oType->ToString());
		if (m_oContinuation.IsInit()) pWriter->WriteAttribute(L"continuation", m_oContinuation->ToString());
		pWriter->WriteAttribute2(L"priorRef", m_sPriorRef);
		pWriter->WriteAttribute(L"timeOffset", m_dTimeOffset);
		pWriter->WriteAttribute(L"duration", m_dDuration);
		pWriter->WriteAttribute2(L"contextRef", m_sContextRef);
		pWriter->WriteAttribute2(L"brushRef", m_sBrushRef);
		pWriter->EndAttributes();

		pWriter->WriteString(m_sData); // trace grammar - digits, signs, '"!?*, no xml special chars

		pWriter->WriteNodeEnd(L"inkml:trace");
	}
	void CInkTrace::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sId);
		if (m_oType.IsInit()) pWriter->WriteByte1(1, m_oType->GetValue());
		if (m_oContinuation.IsInit()) pWriter->WriteByte1(2, m_oContinuation->GetValue());
		pWriter->WriteString2(3, m_sPriorRef);
		pWriter->WriteString2(4, m_sContextRef);
		pWriter->WriteString2(5, m_sBrushRef);
		pWriter->WriteDoubleReal2(6, m_dDuration);
		pWriter->WriteDoubleReal2(7, m_dTimeOffset);
		if (false == m_sData.empty())
			pWriter->WriteString1(8, m_sData);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CInkTrace::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sId = pReader->GetString2();
			}break;
			case 1:
			{
				m_oType.Init();
				m_oType->SetValueFromByte(pReader->GetUChar());
			}break;
			case 2:
			{
				m_oContinuation.Init();
				m_oContinuation->SetValueFromByte(pReader->GetUChar());
			}break;
			case 3:
			{
				m_sPriorRef = pReader->GetString2();
			}break;
			case 4:
			{
				m_sContextRef = pReader->GetString2();
			}break;
			case 5:
			{
				m_sBrushRef = pReader->GetString2();
			}break;
			case 6:
			{
				m_dDuration = pReader->GetDoubleReal();
			}break;
			case 7:
			{
				m_dTimeOffset = pReader->GetDoubleReal();
			}break;
			case 8:
			{
				m_sData = pReader->GetString2();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkTraceGroup::getType() const
	{
		return et_inkml_traceGroup;
	}
	void CInkTraceGroup::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "id", m_sId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "contextRef", m_sContextRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "brushRef", m_sBrushRef)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkTraceGroup::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			WritingElement* pItem = CreateInkElement(XmlUtils::GetNameNoNS(oReader.GetName()));
			if (pItem)
			{
				pItem->fromXML(oReader);
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CInkTraceGroup::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:traceGroup");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xml:id", m_sId);
		pWriter->WriteAttribute2(L"contextRef", m_sContextRef);
		pWriter->WriteAttribute2(L"brushRef", m_sBrushRef);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"inkml:traceGroup");
	}
	void CInkTraceGroup::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sId);
		pWriter->WriteString2(1, m_sContextRef);
		pWriter->WriteString2(2, m_sBrushRef);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			BYTE type = GetInkRecordType(m_arrItems[i]->getType());
			if (type != 0xff)
				pWriter->WriteRecord2(type, m_arrItems[i]);
		}
	}
	void CInkTraceGroup::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sId = pReader->GetString2();
			}break;
			case 1:
			{
				m_sContextRef = pReader->GetString2();
			}break;
			case 2:
			{
				m_sBrushRef = pReader->GetString2();
			}break;
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();

			WritingElement* pItem = CreateInkElement(_rec);
			if (pItem)
			{
				pItem->fromPPTY(pReader);
				m_arrItems.push_back(pItem);
			}
			else
			{
				pReader->SkipRecord();
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkTraceView::getType() const
	{
		return et_inkml_traceView;
	}
	void CInkTraceView::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "id", m_sId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "contextRef", m_sContextRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "traceDataRef", m_sTraceDataRef)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "from", m_sFrom)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "to", m_sTo)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CInkTraceView::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			WritingElement* pItem = CreateInkElement(XmlUtils::GetNameNoNS(oReader.GetName()));
			if (pItem)
			{
				pItem->fromXML(oReader);
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CInkTraceView::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:traceView");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xml:id", m_sId);
		pWriter->WriteAttribute2(L"contextRef", m_sContextRef);
		pWriter->WriteAttribute2(L"traceDataRef", m_sTraceDataRef);
		pWriter->WriteAttribute2(L"from", m_sFrom);
		pWriter->WriteAttribute2(L"to", m_sTo);

		if (m_arrItems.empty())
		{
			pWriter->WriteNodeEnd(L"inkml:traceView", true);
			return;
		}
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"inkml:traceView");
	}
	void CInkTraceView::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, m_sId);
		pWriter->WriteString2(1, m_sContextRef);
		pWriter->WriteString2(2, m_sTraceDataRef);
		pWriter->WriteString2(3, m_sFrom);
		pWriter->WriteString2(4, m_sTo);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			BYTE type = GetInkRecordType(m_arrItems[i]->getType());
			if (type != 0xff)
				pWriter->WriteRecord2(type, m_arrItems[i]);
		}
	}
	void CInkTraceView::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

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
				m_sId = pReader->GetString2();
			}break;
			case 1:
			{
				m_sContextRef = pReader->GetString2();
			}break;
			case 2:
			{
				m_sTraceDataRef = pReader->GetString2();
			}break;
			case 3:
			{
				m_sFrom = pReader->GetString2();
			}break;
			case 4:
			{
				m_sTo = pReader->GetString2();
			}break;
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();

			WritingElement* pItem = CreateInkElement(_rec);
			if (pItem)
			{
				pItem->fromPPTY(pReader);
				m_arrItems.push_back(pItem);
			}
			else
			{
				pReader->SkipRecord();
			}
		}
		pReader->Seek(_end_rec);
	}
//-----------------------------------------------------------------------------------------------------------------
	EElementType CInkDefinitions::getType() const
	{
		return et_inkml_definitions;
	}
	void CInkDefinitions::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			WritingElement* pItem = CreateInkElement(XmlUtils::GetNameNoNS(oReader.GetName()));
			if (pItem)
			{
				pItem->fromXML(oReader);
				m_arrItems.push_back(pItem);
			}
		}
	}
	void CInkDefinitions::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"inkml:definitions");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"inkml:definitions");
	}
	void CInkDefinitions::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			BYTE type = GetInkRecordType(m_arrItems[i]->getType());
			if (type != 0xff)
				pWriter->WriteRecord2(type, m_arrItems[i]);
		}
	}
	void CInkDefinitions::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();

			WritingElement* pItem = CreateInkElement(_rec);
			if (pItem)
			{
				pItem->fromPPTY(pReader);
				m_arrItems.push_back(pItem);
			}
			else
			{
				pReader->SkipRecord();
			}
		}
		pReader->Seek(_end_rec);
	}
} // namespace OOX
