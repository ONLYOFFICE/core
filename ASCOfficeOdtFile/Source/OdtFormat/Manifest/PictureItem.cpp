
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "PictureItem.h"
#include "TypeTable.h"
#include "exception\not_implement.h"


namespace Odt
{
	namespace Manifest
	{

		PictureItem::PictureItem()
		{
		}


		PictureItem::PictureItem(const boost::filesystem::wpath& filename)
			: m_filename(filename)
		{
		}


		PictureItem::~PictureItem()
		{
		}


		PictureItem::PictureItem(const XML::XNode& node)
		{
			fromXML(node);
		}


		const PictureItem& PictureItem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void PictureItem::fromXML(const XML::XNode& node)
		{
			throw not_implement("PictureItem fromXML");
		}


		const XML::XNode PictureItem::toXML() const
		{
			throw not_implement("PictureItem toXML");
		}


		const XML::XNode PictureItem::toXML(const boost::filesystem::wpath& path) const
		{
			if (!boost::filesystem::exists(path/L"Pictures"))
				boost::filesystem::create_directories(path/L"Pictures");

			copy(path/L"Pictures");

			static const TypeTable table;
			return
				XML::XElement(ns.manifest + "file-entry",
					XML::XAttribute(ns.manifest + "media-type", table[m_filename.filename()]) + 
					XML::XAttribute(ns.manifest + "full-path", L"Pictures/" + m_filename.filename())
				);		
		}


		const boost::filesystem::wpath PictureItem::path() const
		{
			return m_filename;
		}


		const std::wstring PictureItem::filename() const
		{
			return m_filename.filename();
		}


		void PictureItem::copy(const boost::filesystem::wpath& path) const
		{
			const std::wstring filename = m_filename.filename();

			if ((filename == std::wstring(L"NULL")) || (filename == std::wstring(L"")))
				return;

			try
			{
				if (!boost::filesystem::exists(path/filename))
					boost::filesystem::copy_file(m_filename, path/filename);
			}
			catch(...)
			{
			}
		}

	} // namespace Manifest
} // namespace Odt