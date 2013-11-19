
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "OleObjectItem.h"
#include "TypeTable.h"
#include "exception\not_implement.h"


namespace Odt
{
	namespace Manifest
	{

		OleObjectItem::OleObjectItem()
		{
		}


		OleObjectItem::OleObjectItem(const boost::filesystem::wpath& filename)
			: m_filename(filename)
		{
		}


		OleObjectItem::~OleObjectItem()
		{
		}


		OleObjectItem::OleObjectItem(const XML::XNode& node)
		{
			fromXML(node);
		}


		const OleObjectItem& OleObjectItem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void OleObjectItem::fromXML(const XML::XNode& node)
		{
			throw not_implement("OleObjectItem fromXML");
		}


		const XML::XNode OleObjectItem::toXML() const
		{
			throw not_implement("OleObjectItem toXML");
		}


		const XML::XNode OleObjectItem::toXML(const boost::filesystem::wpath& path) const
		{
			if (!boost::filesystem::exists(path))
				boost::filesystem::create_directories(path);

			copy(path);

			static const TypeTable table;
			return
				XML::XElement(ns.manifest + "file-entry",
					XML::XAttribute(ns.manifest + "media-type", "application/vnd.sun.star.oleobject") + 
					XML::XAttribute(ns.manifest + "full-path", m_filename.filename())
				);
		}


		const boost::filesystem::wpath OleObjectItem::path() const
		{
			return m_filename;
		}


		const std::wstring OleObjectItem::filename() const
		{
			return m_filename.filename();
		}


		void OleObjectItem::copy(const boost::filesystem::wpath& path) const
		{
			const std::wstring filename = m_filename.filename();
			if (!boost::filesystem::exists(path/filename))
				boost::filesystem::copy_file(m_filename, path/filename);
		}

	} // namespace Manifest
} // namespace Odt