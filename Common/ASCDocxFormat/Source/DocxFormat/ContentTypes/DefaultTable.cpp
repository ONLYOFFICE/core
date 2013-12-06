
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "DefaultTable.h"

namespace OOX
{
	namespace ContentTypes
	{

		DefaultTable::DefaultTable()
		{
			m_items.push_back(L"rels");
			m_items.push_back(L"xml");
		}


		DefaultTable::~DefaultTable()
		{
		}


		DefaultTable::DefaultTable(const XML::XNode& node)
		{
			fromXML(node);
		}


		const DefaultTable& DefaultTable::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void DefaultTable::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_items, element, "Default");
		}

		void DefaultTable::add(const OOX::CPath& path)
		{
			const std::wstring extension = std::wstring(OOX::CSystemUtility::GetFileExtention(path.GetPath()));
			//const std::wstring extension = path.extension().substr(1);
			if (std::find(begin(), end(), extension) == end())
				m_items.push_back(extension);

			////////const std::wstring extension = path.extension().substr(1);
			////////if (std::find(begin(), end(), extension) == end())
			////////	m_items.push_back(extension);
		}

	} // namespace ContentTypes
} // namespace OOX