#pragma once
#ifndef PPTX_LOGIC_CMD_INCLUDE_H_
#define PPTX_LOGIC_CMD_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CBhvr.h"
#include "./../../Limit/TLCommandType.h"

namespace PPTX
{
	namespace Logic
	{
		class Cmd : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Cmd)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"type", type);
				node.ReadAttributeBase(L"cmd", cmd);

				cBhvr	= node.ReadNode(_T("p:cBhvr"));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("type"), type);
				oAttr.Write(_T("cmd"), cmd);

				return XmlUtils::CreateNode(_T("p:cmd"), oAttr, cBhvr.toXML());
			}

		public:
			CBhvr									cBhvr;

			nullable_limit<Limit::TLCommandType>	type;
			nullable_string							cmd;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cBhvr.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CMD_INCLUDE_H