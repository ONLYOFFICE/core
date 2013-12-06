
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
		return XML::XElement();
		}


		const std::string Insert::toTxt() const
		{
			std::string text;
			
			const std::vector<Run>& runs = Runs.get();

			for (std::vector<Run>::const_iterator iter = runs.begin(); iter != runs.end(); ++iter)
				text += (*iter).toTxt();
		
			return text;
		}

	} // namespace Logic
} // namespace OOX