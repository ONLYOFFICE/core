
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "ConverterProperties.h"


namespace Docx2Odt
{
	namespace Private
	{
		ConverterProperties::ConverterProperties()
			: addStyleToContent(true), Lines(0), Distance(UniversalUnit()), Bukvitza(""), hasBukvitza(false)
		{
		}


		void ConverterProperties::setDestStyleIsContent(const bool flag)
		{
			addStyleToContent = flag;
		}

		
		const bool ConverterProperties::getDestStyleIsContent() const
		{
			return addStyleToContent;
		}
	} // namespace Private
} // namespace Docx2Odt