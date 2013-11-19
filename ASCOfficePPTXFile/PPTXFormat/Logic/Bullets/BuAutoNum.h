#pragma once
#ifndef PPTX_LOGIC_BUAUTONUM_INCLUDE_H_
#define PPTX_LOGIC_BUAUTONUM_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/TextAutonumberScheme.h"

namespace PPTX
{
	namespace Logic
	{
		class BuAutoNum : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BuAutoNum)

			BuAutoNum& operator=(const BuAutoNum& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				type	= oSrc.type;
				startAt = oSrc.startAt;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"type", type);
				node.ReadAttributeBase(L"startAt", startAt);

				Normalize();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("type"), type.get());
				oAttr.Write(_T("startAt"), startAt);

				return XmlUtils::CreateNode(_T("a:buAutoNum"), oAttr);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:buAutoNum"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("type"), type.get());
				pWriter->WriteAttribute(_T("startAt"), startAt);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:buAutoNum"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(BULLET_TYPE_BULLET_AUTONUM);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);				
				pWriter->WriteLimit1(0, type);
				pWriter->WriteInt2(1, startAt);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}
		
		public:
			Limit::TextAutonumberScheme type;
			nullable_int				startAt;

		public:
			AVSINLINE void Normalize()
			{
				startAt.normalize(1, 32767);
			}

		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUAUTONUM_INCLUDE_H