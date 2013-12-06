#pragma once
#ifndef OOX_FONTTABLE_INCLUDE_H_
#define OOX_FONTTABLE_INCLUDE_H_

#include <vector>

#include "File.h"

namespace OOX
{
	class FontTable : public OOX::File
	{
	public:
		class Font
		{
		public:
			Font();
			virtual ~Font();
			explicit Font(XmlUtils::CXmlNode& node);
			const Font& operator =(XmlUtils::CXmlNode& node);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);

		public:
			NSCommon::nullable<std::wstring>	m_name;
			NSCommon::nullable<std::wstring>	m_family;
			NSCommon::nullable<std::wstring>	m_charset;
			NSCommon::nullable<std::wstring>	m_pitch;
			NSCommon::nullable<std::wstring>	m_panose1;
			NSCommon::nullable<std::wstring>	m_usb0;
			NSCommon::nullable<std::wstring>	m_usb1;
			NSCommon::nullable<std::wstring>	m_usb2;
			NSCommon::nullable<std::wstring>	m_usb3;
			NSCommon::nullable<std::wstring>	m_csb0;
			NSCommon::nullable<std::wstring>	m_csb1;
		};

	public:
		FontTable();
		FontTable(const OOX::CPath& filename);
		virtual ~FontTable();

	public:
		virtual void read(const OOX::CPath& filename);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

	public:

		std::vector<Font> m_fonts;
	};
} // namespace OOX

#endif // OOX_FONTTABLE_INCLUDE_H_