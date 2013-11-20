
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Default.h"
#include "Extensiontable.h"


namespace OOX
{
	namespace ContentTypes
	{

		Default::Default(const std::wstring& extension)
			: m_extension(extension)
		{
		}


		Default::Default(const wchar_t* extension)
			: m_extension(extension)
		{
		}


		Default::~Default()
		{
		}


		Default::Default(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Default& Default::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Default::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_extension = element.attribute("Extension").value();
		}


		const XML::XNode Default::toXML() const
		{
			static const ExtensionTable table;
			return
				XML::XElement("Default",
					XML::XAttribute("Extension", m_extension) + 
					XML::XAttribute("ContentType", table[m_extension])
				);
		}


		const bool Default::operator ==(const std::wstring& rhs) const
		{
			return m_extension == rhs;
		}

	} // namespace ContentTypes
} // namespace OOX