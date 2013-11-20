
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Override.h"


namespace OOX
{
	namespace ContentTypes
	{

		Override::Override(const std::string& type, const boost::filesystem::wpath& path)
			: m_part(path),
				m_type(type)
		{
		}


		Override::~Override()
		{
		}


		Override::Override(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Override& Override::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		
		void Override::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_part	= element.attribute("PartName").value();
			m_type	= element.attribute("ContentType").value();
		}

		
		const XML::XNode Override::toXML() const
		{
			return
				XML::XElement("Override",
					XML::XAttribute("PartName", L"/" + m_part.string()) + 
					XML::XAttribute("ContentType", m_type)
				);
		}


		const std::string Override::type() const
		{
			return m_type;
		}


		const boost::filesystem::wpath Override::filename() const
		{
			return m_part;
		}

	} // namespace ContentTypes
} // namespace OOX