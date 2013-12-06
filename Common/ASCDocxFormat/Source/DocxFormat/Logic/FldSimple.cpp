
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FldSimple.h"


namespace OOX
{
	namespace Logic
	{

		FldSimple::FldSimple()
		{
		}


		FldSimple::~FldSimple()
		{
		}


		FldSimple::FldSimple(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FldSimple& FldSimple::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		
		void FldSimple::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Run		= element.element("r");
			Instr = element.attribute("instr").value();
		}


		const XML::XNode FldSimple::toXML() const
		{
			return XML::XElement();

			//return
			//	XML::XElement(ns.w + "fldSimple", 
			//		XML::XAttribute(ns.w + "instr", Instr) +
			//		XML::Write(Run)
			//	);
		}


		const std::string FldSimple::toTxt() const
		{
			return Run->toTxt();
		}

	} // namespace Logic
} // namespace OOX