#pragma once
#ifndef PPTX_LOGIC_PATH_INCLUDE_H_
#define PPTX_LOGIC_PATH_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Rect.h"
#include "./../Limit/FillPath.h"

namespace PPTX
{
	namespace Logic
	{

		class Path : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Path)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"path", path);
				rect = node.ReadNodeNoNS(_T("fillToRect"));
				
				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("path"), path);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(rect);

				return XmlUtils::CreateNode(_T("a:path"), oAttr, oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, path);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, rect);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)
					{
						path = new PPTX::Limit::FillPath();
						path->SetBYTECode(pReader->GetUChar());
					}
					else
						break;
				}

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							rect = new Logic::Rect();
							rect->fromPPTY(pReader);
							rect->m_name = _T("a:fillToRect");
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				CString sNodeNamespace;
				CString sAttrNamespace;
				if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				{
					sNodeNamespace = _T("w14:");
					sAttrNamespace = sNodeNamespace;
				}
				else
					sNodeNamespace = _T("a:");
				pWriter->StartNode(sNodeNamespace + _T("path"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(sAttrNamespace + _T("path"), path);
				pWriter->EndAttributes();

				pWriter->Write(rect);

				pWriter->EndNode(sNodeNamespace + _T("path"));
			}

		public:
			nullable<Rect>						rect;
			nullable_limit<Limit::FillPath>		path;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(rect.IsInit())
					rect->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PATH_INCLUDE_H_