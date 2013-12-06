
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Delete.h"


namespace OOX
{
	namespace Logic
	{

		Delete::Delete()
		{
		}


		Delete::~Delete()
		{
		}


		Delete::Delete(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Delete& Delete::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		
		void Delete::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Id          = element.attribute("id").value();
			Author      = element.attribute("author").value();
			Date        = element.attribute("date").value();

			XML::Fill(Runs, element, "r");
		}


		const XML::XNode Delete::toXML() const
		{
		return XML::XElement();
		}


		const std::string Delete::toTxt() const
		{
			std::string text;

			const std::vector<Run>& runs = Runs.get();

			for (std::vector<Run>::const_iterator iter = runs.begin(); iter != runs.end(); ++iter)
				text += (*iter).toTxt();
	
			return text;
		}

	} // namespace Logic
} // namespace OOX