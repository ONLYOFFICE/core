
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Formulas.h"

namespace OOX
{
	namespace Logic
	{
			
		Formulas::Formulas()
		{
		}


		Formulas::~Formulas()
		{
		}


		Formulas::Formulas(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Formulas& Formulas::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Formulas::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Fill(Fs, element, "f");			
		}
		
		const XML::XNode Formulas::toXML() const
		{	
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX