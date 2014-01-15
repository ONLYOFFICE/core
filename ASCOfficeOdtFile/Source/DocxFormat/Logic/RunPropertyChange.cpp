
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "RunPropertyChange.h"


namespace OOX
{
	namespace Logic
	{

		RunPropertyChange::RunPropertyChange()
		{
		}


		RunPropertyChange::~RunPropertyChange()
		{
		}
	

		RunPropertyChange::RunPropertyChange(const XML::XNode& node)
		{
			fromXML(node);
		}


		const RunPropertyChange& RunPropertyChange::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void RunPropertyChange::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Id          = element.attribute("id").value();
			Author      = element.attribute("author").value();
			Date        = element.attribute("date").value();
		}


		const XML::XNode RunPropertyChange::toXML() const
		{
			return 
				XML::XElement(ns.w + "rPrChange",
				    XML::XAttribute(ns.w + "id", Id) +
					XML::XAttribute(ns.w + "author", Author) +
					XML::XAttribute(ns.w + "date", Date) 
				);
		}

	} // namespace Logic
} // namespace OOX