
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Insert.h"


namespace OOX
{
	namespace Logic
	{

		Insert::Insert()
		{
		}


		Insert::~Insert()
		{
		}


		Insert::Insert(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Insert& Insert::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		
		void Insert::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Id          = element.attribute("id").value();
			Author      = element.attribute("author").value();
			Date        = element.attribute("date").value();

			XML::Fill(Runs, element, "r");
		}


		const XML::XNode Insert::toXML() const
		{
			return
				XML::XElement(ns.w + "ins",
				    XML::XAttribute(ns.w + "id", Id) +
					XML::XAttribute(ns.w + "author", Author) +
					XML::XAttribute(ns.w + "date", Date) +
					XML::Write(Runs)
				);
		}


		const std::string Insert::toTxt() const
		{
			std::string text;
			BOOST_FOREACH(const Run& run, *Runs)
				text += run.toTxt();
			return text;
		}

	} // namespace Logic
} // namespace OOX