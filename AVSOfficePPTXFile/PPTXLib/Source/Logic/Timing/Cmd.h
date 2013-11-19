#pragma once
#ifndef PPTX_LOGIC_CMD_INCLUDE_H_
#define PPTX_LOGIC_CMD_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CBhvr.h"
#include "./../../Limit/TLCommandType.h"

namespace PPTX
{
	namespace Logic
	{
		class Cmd : public WrapperWritingElement
		{
		public:
			Cmd();
			virtual ~Cmd();			
			explicit Cmd(const XML::XNode& node);
			const Cmd& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CBhvr> cBhvr;

			nullable_property<std::string, Limit::TLCommandType> type;
			nullable_property<std::string> cmd;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CMD_INCLUDE_H