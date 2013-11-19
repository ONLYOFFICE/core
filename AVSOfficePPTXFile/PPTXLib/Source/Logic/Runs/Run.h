#pragma once
#ifndef PPTX_LOGIC_RUN_INCLUDE_H_
#define PPTX_LOGIC_RUN_INCLUDE_H_

#include "RunBase.h"
#include "property.h"
#include "./../RunProperties.h"
#include "Encoding.h"

namespace PPTX
{
	namespace Logic
	{
		class Run : public RunBase
		{
		public:
			Run();
			virtual ~Run();	
			explicit Run(const XML::XNode& node);
			const Run& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual std::string GetText()const{return text.get_value_or("");};
			virtual std::wstring GetWText()const{return Encoding::utf82unicode(text.get_value_or(""));};
		public:
			nullable_property<RunProperties> rPr;
		private:
			nullable_property<std::string> text;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_RUN_INCLUDE_H