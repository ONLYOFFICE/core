#pragma once
#ifndef PPTX_LOGIC_SEQ_INCLUDE_H_
#define PPTX_LOGIC_SEQ_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CTn.h"
#include "CondLst.h"
#include "./../../Limit/TLPrevAc.h"
#include "./../../Limit/TLNextAc.h"

namespace PPTX
{
	namespace Logic
	{
		class Seq : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Seq)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cTn			= node.ReadNode(_T("p:cTn"));
				nextCondLst = node.ReadNode(_T("p:nextCondLst"));
				prevCondLst = node.ReadNode(_T("p:prevCondLst"));

				node.ReadAttributeBase(L"concurrent", concurrent);
				node.ReadAttributeBase(L"nextAc", nextAc);
				node.ReadAttributeBase(L"prevAc", prevAc);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("concurrent"), concurrent);
				oAttr.WriteLimitNullable(_T("prevAc"), prevAc);
				oAttr.WriteLimitNullable(_T("nextAc"), nextAc);

				XmlUtils::CNodeValue oValue;
				oValue.Write(cTn);
				oValue.WriteNullable(nextCondLst);
				oValue.WriteNullable(prevCondLst);

				return XmlUtils::CreateNode(_T("p:seg"), oAttr, oValue);
			}

		public:
			CTn									cTn;
			nullable<CondLst>					nextCondLst;
			nullable<CondLst>					prevCondLst;

			nullable_bool						concurrent;
			nullable_limit<Limit::TLNextAc>		nextAc;
			nullable_limit<Limit::TLPrevAc>		prevAc;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cTn.SetParentPointer(this);
				if(prevCondLst.IsInit())
					prevCondLst->SetParentPointer(this);
				if(nextCondLst.IsInit())
					nextCondLst->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SEQ_INCLUDE_H