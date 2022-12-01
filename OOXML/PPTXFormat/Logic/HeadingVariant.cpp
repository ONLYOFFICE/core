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


#include "HeadingVariant.h"

namespace PPTX
{
	namespace Logic
	{
		void HeadingVariant::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode oNode = node.ReadNodeNoNS(_T("i4"));
			if (oNode.IsValid())
			{
				m_type		= L"i4";
				m_iContent	= oNode.GetTextExt();
			}
			else
			{
				m_type		 = L"lpstr";
				m_strContent =	 node.ReadNodeNoNS(_T("lpstr")).GetTextExt();
			}
		}
		std::wstring HeadingVariant::toXML() const
		{
			if (m_type.IsInit() && (m_type->get() == _T("i4")))
			{
				return _T("<vt:variant><vt:i4>") + std::to_wstring(*m_iContent) + _T("</vt:i4></vt:variant>");
			}
			return _T("<vt:variant><vt:lpstr>") + *m_strContent + _T("</vt:lpstr></vt:variant>");
		}
		void HeadingVariant::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("vt:variant"));
			pWriter->EndAttributes();

			if (m_type.IsInit() && (m_type->get() == _T("i4")))
			{
				pWriter->WriteNodeValue(_T("vt:i4"), *m_iContent);
			}
			else
			{
				pWriter->WriteNodeValue(_T("vt:lpstr"), *m_strContent);
			}

			pWriter->EndNode(_T("vt:variant"));
		}
		void HeadingVariant::FillParentPointersForChilds() {}

		void CVariantVStream::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, _T("version"), m_strVersion);
			m_strContent = node.GetTextExt();
		}
		std::wstring CVariantVStream::toXML() const
		{
			return L"";
		}
		void CVariantVStream::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("vt:vstream"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"version", m_strVersion);
			pWriter->EndAttributes();

			pWriter->WriteStringXML(*m_strContent);
			pWriter->EndNode(_T("vt:vstream"));
		}
		void CVariantVStream::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				case 0: m_strVersion = pReader->GetString2(); break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					m_strContent = pReader->GetString2();
					break;
				}
				default:
				{
					pReader->SkipRecord();
					break;
				}
				}
			}
			pReader->Seek(_end_rec);
		}
		void CVariantVStream::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_strVersion);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteString2(0, m_strContent);
		}

		void CVariant::fromXML(XmlUtils::CXmlNode& node)
		{
			m_eType = CVariant::getTypeByString(XmlUtils::GetNameNoNS(node.GetName()));
			switch(getVariantType())
			{
				case vtEmpty: break;
				case vtNull: break;
				case vtVariant:
					{
						XmlUtils::CXmlNodes oNodes;
						if (node.GetNodes(_T("*"), oNodes))
						{
							int nCount = oNodes.GetCount();
							for (int i = 0; i < nCount; ++i)
							{
								XmlUtils::CXmlNode oNode;
								oNodes.GetAt(i, oNode);

								m_oVariant = oNode;
							}
						}
					break;
					}
				case vtVector: m_oVector.Init();m_oVector->fromXML(node);break;
				case vtArray: m_oArray.Init();m_oArray->fromXML(node);break;
				case vtVStream: m_oVStream.Init();m_oVStream->fromXML(node);break;
				case vtBlob: m_strContent = node.GetTextExt();break;
				case vtOBlob: m_strContent = node.GetTextExt();break;
				case vtI1: m_iContent = node.GetTextExt();break;
				case vtI2: m_iContent = node.GetTextExt();break;
				case vtI4: m_iContent = node.GetTextExt();break;
				case vtI8: m_iContent = node.GetTextExt();break;
				case vtInt: m_iContent = node.GetTextExt();break;
				case vtUi1: m_uContent = node.GetTextExt();break;
				case vtUi2: m_uContent = node.GetTextExt();break;
				case vtUi4: m_uContent = node.GetTextExt();break;
				case vtUi8: m_uContent = node.GetTextExt();break;
				case vtUint: m_uContent = node.GetTextExt();break;
				case vtR4: m_dContent = node.GetTextExt();break;
				case vtR8: m_dContent = node.GetTextExt();break;
				case vtDecimal: m_dContent = node.GetTextExt();break;
				case vtLpstr: m_strContent = node.GetTextExt();break;
				case vtLpwstr: m_strContent = node.GetTextExt();break;
				case vtBstr: m_strContent = node.GetTextExt();break;
				case vtDate: m_strContent = node.GetTextExt();break;
				case vtFiletime: m_strContent = node.GetTextExt();break;
				case vtBool: m_bContent = node.GetTextExt();break;
				case vtCy: m_strContent = node.GetTextExt();break;
				case vtError: m_strContent = node.GetTextExt();break;
				case vtStream: m_strContent = node.GetTextExt();break;
				case vtOStream: m_strContent = node.GetTextExt();break;
				case vtStorage: m_strContent = node.GetTextExt();break;
				case vtOStorage: m_strContent = node.GetTextExt();break;
				case vtClsid: m_strContent = node.GetTextExt();break;
			}
		}
		std::wstring CVariant::toXML() const
		{
			return L"";
		}
		void CVariant::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"vt:variant");
			pWriter->StartAttributes();
			pWriter->EndAttributes();
			toXmlWriterContent(pWriter);
			pWriter->EndNode(L"vt:variant");
		}
		void CVariant::toXmlWriterContent(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			eVariantType eType = getVariantType();
			std::wstring strNodeName = L"vt:" + getStringByType(eType);
			if (vtEmpty == eType || vtNull == eType)
			{
				pWriter->StartNode(strNodeName);
				pWriter->StartAttributes();
				pWriter->EndAttributes();
				pWriter->EndNode(strNodeName);
			}
			pWriter->WriteNodeValue2(strNodeName, m_strContent);
			pWriter->WriteNodeValue(strNodeName, m_iContent);
			pWriter->WriteNodeValue(strNodeName, m_uContent);
			pWriter->WriteNodeValue(strNodeName, m_dContent);
			if (m_bContent.IsInit())
			{
				pWriter->StartNode(strNodeName);
				pWriter->StartAttributes();
				pWriter->EndAttributes();
				if (*m_bContent)
					pWriter->WriteString(NSBinPptxRW::g_bstr_boolean_true);
				else
					pWriter->WriteString(NSBinPptxRW::g_bstr_boolean_false);
				pWriter->EndNode(strNodeName);
			}
			if (m_oVariant.IsInit())
			{
				m_oVariant->toXmlWriter(pWriter);
			}
			if (m_oVector.IsInit())
			{
				m_oVector->toXmlWriter(pWriter);
			}
			if (m_oArray.IsInit())
			{
				m_oArray->toXmlWriter(pWriter);
			}
			if (m_oVStream.IsInit())
			{
				m_oVStream->toXmlWriter(pWriter);
			}
		}
		void CVariant::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				case 0: m_eType = (eVariantType)pReader->GetUChar(); break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0: {m_strContent = pReader->GetString2(); break;}
				case 1: {m_iContent = pReader->GetLong(); break;}
				case 2: {m_uContent = pReader->GetULong(); break;}
				case 3: {m_dContent = pReader->GetDoubleReal(); break;}
				case 4: {m_bContent = pReader->GetBool(); break;}
				case 5: {m_oVariant.Init(); m_oVariant->fromPPTY(pReader); break;}
				case 6: {m_oVector.Init(); m_oVector->fromPPTY(pReader); break;}
				case 7: {m_oArray.Init(); m_oArray->fromPPTY(pReader); break;}
				case 8: {m_oVStream.Init(); m_oVStream->fromPPTY(pReader); break;}
				default:
				{
					pReader->SkipRecord();
					break;
				}
				}
			}
			pReader->Seek(_end_rec);
		}
		void CVariant::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteByte1(0, getVariantType());
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteString2(0, m_strContent);
			pWriter->WriteInt2(1, m_iContent);
			pWriter->WriteUInt2(2, m_uContent);
			pWriter->WriteDoubleReal2(3, m_dContent);
			pWriter->WriteBool2(4, m_bContent);
			pWriter->WriteRecord2(5, m_oVariant);
			pWriter->WriteRecord2(6, m_oVector);
			pWriter->WriteRecord2(7, m_oArray);
			pWriter->WriteRecord2(8, m_oVStream);

		}
		eVariantType CVariant::getVariantType() const
		{
			return m_eType.IsInit() ? m_eType.get() : vtEmpty;
		}
		eVariantType CVariant::getTypeByString(const std::wstring& sName)
		{
			if(L"empty" == sName)
				return vtEmpty;
			else if(L"null" == sName)
				return vtNull;
			else if(L"variant" == sName)
				return vtVariant;
			else if(L"vector" == sName)
				return vtVector;
			else if(L"array" == sName)
				return vtArray;
			else if(L"vstream" == sName)
				return vtVStream;
			else if(L"blob" == sName)
				return vtBlob;
			else if(L"oblob" == sName)
				return vtOBlob;
			else if(L"i1" == sName)
				return vtI1;
			else if(L"i2" == sName)
				return vtI2;
			else if(L"i4" == sName)
				return vtI4;
			else if(L"i8" == sName)
				return vtI8;
			else if(L"int" == sName)
				return vtInt;
			else if(L"ui1" == sName)
				return vtUi1;
			else if(L"ui2" == sName)
				return vtUi2;
			else if(L"ui4" == sName)
				return vtUi4;
			else if(L"ui8" == sName)
				return vtUi8;
			else if(L"uint" == sName)
				return vtUint;
			else if(L"r4" == sName)
				return vtR4;
			else if(L"r8" == sName)
				return vtR8;
			else if(L"decimal" == sName)
				return vtDecimal;
			else if(L"lpstr" == sName)
				return vtLpstr;
			else if(L"lpwstr" == sName)
				return vtLpwstr;
			else if(L"bstr" == sName)
				return vtBstr;
			else if(L"date" == sName)
				return vtDate;
			else if(L"filetime" == sName)
				return vtFiletime;
			else if(L"bool" == sName)
				return vtBool;
			else if(L"cy" == sName)
				return vtCy;
			else if(L"error" == sName)
				return vtError;
			else if(L"stream" == sName)
				return vtStream;
			else if(L"ostream" == sName)
				return vtOStream;
			else if(L"storage" == sName)
				return vtStorage;
			else if(L"ostorage" == sName)
				return vtOStorage;
			else if(L"clsid" == sName)
				return vtClsid;
			return vtEmpty;
		}
		std::wstring CVariant::getStringByType(const eVariantType& eType)
		{
			if(vtEmpty == eType)
				return L"empty";
			else if(vtNull == eType)
				return L"null";
			else if(vtVariant == eType)
				return L"variant";
			else if(vtVector == eType)
				return L"vector";
			else if(vtArray == eType)
				return L"array";
			else if(vtVStream == eType)
				return L"vstream";
			else if(vtBlob == eType)
				return L"blob";
			else if(vtOBlob == eType)
				return L"oblob";
			else if(vtI1 == eType)
				return L"i1";
			else if(vtI2 == eType)
				return L"i2";
			else if(vtI4 == eType)
				return L"i4";
			else if(vtI8 == eType)
				return L"i8";
			else if(vtInt == eType)
				return L"int";
			else if(vtUi1 == eType)
				return L"ui1";
			else if(vtUi2 == eType)
				return L"ui2";
			else if(vtUi4 == eType)
				return L"ui4";
			else if(vtUi8 == eType)
				return L"ui8";
			else if(vtUint == eType)
				return L"uint";
			else if(vtR4 == eType)
				return L"r4";
			else if(vtR8 == eType)
				return L"r8";
			else if(vtDecimal == eType)
				return L"decimal";
			else if(vtLpstr == eType)
				return L"lpstr";
			else if(vtLpwstr == eType)
				return L"lpwstr";
			else if(vtBstr == eType)
				return L"bstr";
			else if(vtDate == eType)
				return L"date";
			else if(vtFiletime == eType)
				return L"filetime";
			else if(vtBool == eType)
				return L"bool";
			else if(vtCy == eType)
				return L"cy";
			else if(vtError == eType)
				return L"error";
			else if(vtStream == eType)
				return L"stream";
			else if(vtOStream == eType)
				return L"ostream";
			else if(vtStorage == eType)
				return L"storage";
			else if(vtOStorage == eType)
				return L"ostorage";
			else if(vtClsid == eType)
				return L"clsid";
			return L"";
		}

		void CVariantVector::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring sBaseType;
			XmlMacroReadAttributeBase(node, _T("baseType"), sBaseType);
			m_eBaseType = CVariant::getTypeByString(sBaseType);
			XmlMacroReadAttributeBase(node, _T("size"), m_nSize);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					arrVariants.emplace_back();
					arrVariants.back() = oNode;
				}
			}
		}
		std::wstring CVariantVector::toXML() const
		{
			return L"";
		}
		void CVariantVector::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("vt:vector"));
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"baseType", CVariant::getStringByType(getVariantType()));
			pWriter->WriteAttribute(L"size", m_nSize);
			pWriter->EndAttributes();

			for(size_t i = 0; i < arrVariants.size(); ++i)
			{
				arrVariants[i].toXmlWriterContent(pWriter);
			}

			pWriter->EndNode(_T("vt:vector"));
		}
		void CVariantVector::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				case 0: m_eBaseType = (eVariantType)pReader->GetUChar(); break;
				case 1: m_nSize = pReader->GetLong(); break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						pReader->Skip(4);
						LONG len = pReader->GetLong();
						for (LONG i = 0; i < len; ++i)
						{
							pReader->Skip(1);
							arrVariants.emplace_back();
							arrVariants.back().fromPPTY(pReader);
						}
						break;
					}
					default:
					{
						pReader->SkipRecord();
						break;
					}
				}
			}
			pReader->Seek(_end_rec);
		}
		void CVariantVector::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteByte1(0, getVariantType());
			pWriter->WriteInt2(1, m_nSize);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecordArray(0, 0, arrVariants);
		}
		eVariantType CVariantVector::getVariantType() const
		{
			return m_eBaseType.IsInit() ? m_eBaseType.get() : vtEmpty;
		}

		void CVariantArray::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring sBaseType;
			XmlMacroReadAttributeBase(node, _T("baseType"), sBaseType);
			m_eBaseType = CVariant::getTypeByString(sBaseType);
			XmlMacroReadAttributeBase(node, _T("lBounds"), m_strLBounds);
			XmlMacroReadAttributeBase(node, _T("uBounds"), m_strUBounds);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					arrVariants.emplace_back();
					arrVariants.back() = oNode;
				}
			}
		}
		std::wstring CVariantArray::toXML() const
		{
			return L"";
		}
		void CVariantArray::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("vt:array"));
			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"lBounds", m_strLBounds);
			pWriter->WriteAttribute2(L"uBounds", m_strUBounds);
			pWriter->WriteAttribute(L"baseType", CVariant::getStringByType(getVariantType()));
			pWriter->EndAttributes();

			for(size_t i = 0; i < arrVariants.size(); ++i)
			{
				arrVariants[i].toXmlWriterContent(pWriter);
			}

			pWriter->EndNode(_T("vt:array"));
		}
		void CVariantArray::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				case 0: m_eBaseType = (eVariantType)pReader->GetUChar(); break;
				case 1: m_strLBounds = pReader->GetString2(); break;
				case 2: m_strUBounds = pReader->GetString2(); break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						pReader->Skip(4);
						LONG len = pReader->GetLong();
						for (LONG i = 0; i < len; ++i)
						{
							pReader->Skip(1);
							arrVariants.emplace_back();
							arrVariants.back().fromPPTY(pReader);
						}
						break;
					}
					default:
					{
						pReader->SkipRecord();
						break;
					}
				}
			}
			pReader->Seek(_end_rec);
		}
		void CVariantArray::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteByte1(0, getVariantType());
			pWriter->WriteString2(1, m_strLBounds);
			pWriter->WriteString2(2, m_strUBounds);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecordArray(0, 0, arrVariants);
		}
		eVariantType CVariantArray::getVariantType() const
		{
			return m_eBaseType.IsInit() ? m_eBaseType.get() : vtEmpty;
		}

		void CustomProperty::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, _T("fmtid"), m_strFmtid);
			XmlMacroReadAttributeBase(node, _T("pid"), m_nPid);
			XmlMacroReadAttributeBase(node, _T("name"), m_strName);
			XmlMacroReadAttributeBase(node, _T("linkTarget"), m_strLinkTarget);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					m_oContent = oNode;
				}
			}
		}
		std::wstring CustomProperty::toXML() const
		{
			return L"";
		}
		void CustomProperty::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("property"));
			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"fmtid", m_strFmtid);
			pWriter->WriteAttribute(L"pid", m_nPid);
			pWriter->WriteAttribute2(L"name", m_strName);
			pWriter->WriteAttribute2(L"linkTarget", m_strLinkTarget);
			pWriter->EndAttributes();

			if(m_oContent.IsInit())
			{
				m_oContent->toXmlWriterContent(pWriter);
			}

			pWriter->EndNode(_T("property"));
		}
		void CustomProperty::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				case 0: m_strFmtid = pReader->GetString2(); break;
				case 1: m_nPid = pReader->GetLong(); break;
				case 2: m_strName = pReader->GetString2(); break;
				case 3: m_strLinkTarget = pReader->GetString2(); break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						m_oContent.Init();
						m_oContent->fromPPTY(pReader);
						break;
					}
					default:
					{
						pReader->SkipRecord();
						break;
					}
				}
			}
			pReader->Seek(_end_rec);
		}
		void CustomProperty::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_strFmtid);
			pWriter->WriteInt2(1, m_nPid);
			pWriter->WriteString2(2, m_strName);
			pWriter->WriteString2(3, m_strLinkTarget);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, m_oContent);
		}
	} // namespace Logic
} // namespace PPTX

namespace PPTX
{
	CustomProperties::CustomProperties(OOX::Document* pMain) : WrapperFile(pMain)
	{
	}
	CustomProperties::CustomProperties(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain)
	{
		read(filename, map);
	}
	CustomProperties::~CustomProperties()
	{
	}
	void CustomProperties::read(const OOX::CPath& filename, FileMap& map)
	{
		XmlUtils::CXmlNode oNode;
		oNode.FromXmlFile(filename.m_strFilename);

		XmlUtils::CXmlNodes oNodes;
		oNode.GetNodes(_T("property"), oNodes);
		int nCount = oNodes.GetCount();
		for (int i = 0; i < nCount; ++i)
		{
			XmlUtils::CXmlNode oProperty;
			oNodes.GetAt(i, oProperty);

			m_arProperties.emplace_back();
			m_arProperties.back().fromXML(oProperty);
		}
	}
	void CustomProperties::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
	{
		WrapperFile::write(filename, directory, content);
	}
	const OOX::FileType CustomProperties::type() const
	{
		return OOX::FileTypes::CustomProperties;
	}
	const OOX::CPath CustomProperties::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath CustomProperties::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CustomProperties::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->StartRecord(NSBinPptxRW::NSMainTables::CustomProperties);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecordArray(0, 0, m_arProperties);

		pWriter->EndRecord();
	}

	void CustomProperties::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(_T("Properties"));

		pWriter->StartAttributes();
		pWriter->WriteAttribute(_T("xmlns"), PPTX::g_Namespaces.cup.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:vt"), PPTX::g_Namespaces.vt.m_strLink);
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arProperties.size(); ++i)
		{
			m_arProperties[i].toXmlWriter(pWriter);
		}

		pWriter->EndNode(_T("Properties"));
	}

	void CustomProperties::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		pReader->Skip(1); // type
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // start attributes

		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _at = pReader->GetUChar();
			switch (_at)
			{
				case 0:
				{
					pReader->Skip(4);
					LONG len = pReader->GetLong();
					for (LONG i = 0; i < len; ++i)
					{
						pReader->Skip(1);
						m_arProperties.emplace_back();
						m_arProperties.back().fromPPTY(pReader);
					}
					break;
				}
				default:
				{
					pReader->SkipRecord();
					break;
				}
			}
		}

		pReader->Seek(_end_rec);
	}
} // namespace PPTX
