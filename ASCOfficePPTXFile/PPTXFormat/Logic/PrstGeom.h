#pragma once
#ifndef PPTX_LOGIC_PRSTGEOM_INCLUDE_H_
#define PPTX_LOGIC_PRSTGEOM_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/ShapeType.h"
#include "Gd.h"

namespace PPTX
{
	namespace Logic
	{
		class PrstGeom : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(PrstGeom)

			PrstGeom& operator=(const PrstGeom& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				prst = oSrc.prst;
				avLst = oSrc.avLst;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"prst", prst);
				node.ReadNode(_T("a:avLst")).LoadArray(_T("a:gd"), avLst);

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("prst"), prst.get());

				XmlUtils::CNodeValue oValue;
				oValue.WriteArray(_T("a:avLst"), avLst);

				return XmlUtils::CreateNode(_T("a:prstGeom"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:prstGeom"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("prst"), prst.get());
				pWriter->EndAttributes();
				
				if (avLst.size() == 0)
					pWriter->WriteString(_T("<a:avLst/>"));
				else
					pWriter->WriteArray(_T("a:avLst"), avLst);

				pWriter->EndNode(_T("a:prstGeom"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(GEOMETRY_TYPE_PRST);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, prst.get());
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecordArray(0, 1, avLst);

				pWriter->EndRecord();
			}

		public:
			Limit::ShapeType	prst;
			std::vector<Gd>		avLst;
		protected:
			virtual void FillParentPointersForChilds()
			{
				size_t count = avLst.size();
				for (size_t i = 0; i < count; ++i)
					avLst[i].SetParentPointer(this);
			}
		public:
			virtual CString GetODString()const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("prst"), prst.get());

				CString strXml = _T("");
				size_t nCount = avLst.size();
				for (size_t i = 0; i < nCount; ++i)
					strXml += avLst[i].GetODString();

				return XmlUtils::CreateNode(_T("prstGeom"), oAttr, strXml);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PRSTGEOM_INCLUDE_H