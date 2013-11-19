
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "OleViewItem.h"
#include "TypeTable.h"
#include "exception\not_implement.h"


namespace Odt
{
	namespace Manifest
	{

		OleViewItem::OleViewItem()
		{
		}


		OleViewItem::OleViewItem(const boost::filesystem::wpath& filename)
			: m_filename(filename)
		{
		}


		OleViewItem::~OleViewItem()
		{
		}


		OleViewItem::OleViewItem(const XML::XNode& node)
		{
			fromXML(node);
		}


		const OleViewItem& OleViewItem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void OleViewItem::fromXML(const XML::XNode& node)
		{
			throw not_implement("OleItem fromXML");
		}


		const XML::XNode OleViewItem::toXML() const
		{
			throw not_implement("OleItem toXML");
		}


		const XML::XNode OleViewItem::toXML(const boost::filesystem::wpath& path) const
		{
			if (!boost::filesystem::exists(path/L"ObjectReplacements"))
				boost::filesystem::create_directories(path/L"ObjectReplacements");

			copy(path/L"ObjectReplacements");

			static const TypeTable table;
			return
				XML::XElement(ns.manifest + "file-entry",
					XML::XAttribute(ns.manifest + "media-type", "application/x-openoffice-wmf;windows_formatname=\"Image WMF\"") + 
					XML::XAttribute(ns.manifest + "full-path", L"ObjectReplacements\\" + m_filename.filename())
				);
		}


		const boost::filesystem::wpath OleViewItem::path() const
		{
			return m_filename;
		}


		const std::wstring OleViewItem::filename() const
		{
			return m_filename.filename();
		}


		void OleViewItem::copy(const boost::filesystem::wpath& path) const
		{
			const std::wstring filename = m_filename.filename();
			if (!boost::filesystem::exists(path/filename))
				boost::filesystem::copy_file(m_filename, path/filename);
		}

	} // namespace Manifest
} // namespace Odt