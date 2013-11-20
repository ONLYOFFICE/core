#pragma once
#ifndef OOX_FONTTABLE_INCLUDE_H_
#define OOX_FONTTABLE_INCLUDE_H_

#include "File.h"
#include <vector>
#include "property.h"
#include "nullable_property.h"
#include "Limit/FontName.h"
#include "Limit/FontFamily.h"
#include "Limit/FontCharset.h"
#include "Limit/FontPitch.h" 

namespace OOX
{
	class FontTable : public OOX::File
	{
	public:
		class Font : public WritingElement
		{
		public:
			Font();
			virtual ~Font();
			explicit Font(const XML::XNode& node);
			const Font& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>		                Name;
			property<std::string, Limit::FontFamily>	Family;
			nullable_property<std::string, Limit::FontCharset>	Charset;
			property<std::string, Limit::FontPitch>		Pitch;
			nullable_property<std::string>				Panose1;
			nullable_property<std::string>				Usb0;
			nullable_property<std::string>				Usb1;
			nullable_property<std::string>				Usb2;
			nullable_property<std::string>				Usb3;
			nullable_property<std::string>				Csb0;
			nullable_property<std::string>				Csb1;
		};

	public:

		FontTable();
		FontTable(const boost::filesystem::wpath& filename);
		virtual ~FontTable();

	public:
		virtual void read(const boost::filesystem::wpath& filename);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		property<std::vector<Font> > Fonts;
	};
} // namespace OOX

#endif // OOX_FONTTABLE_INCLUDE_H_