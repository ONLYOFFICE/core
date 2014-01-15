
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Styles.h"
#include "FileTypes.h"

namespace OOX
{
	Styles::Styles()
	{

	}

	Styles::Styles(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}

	Styles::~Styles()
	{

	}

	void Styles::read(const boost::filesystem::wpath& filename)
	{
		const XML::XDocument document(filename);
		
		Default	=	document.Root.element("docDefaults");
		Lattent	=	document.Root.element("latentStyles");
		
		Fill(Named, document.Root, "style");
	}

	void Styles::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		XML::XElement(ns.w + "styles",
			XML::Write(Default) +
			XML::Write(Lattent) +
			XML::Write(Named)
			).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
	}

	const FileType Styles::type() const
	{
		return FileTypes::Style;
	}

	const boost::filesystem::wpath Styles::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const boost::filesystem::wpath Styles::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	const OOX::Styles::Style Styles::GetStyleById(const std::string& strStyleId) const
	{
		for (size_t i = 0; i < Named->size(); ++i)
		{
			if ( Named->operator [](i).StyleId == strStyleId)
				return Named->operator [](i);
		}

		return OOX::Styles::Style();
	}

	const OOX::Styles::Style Styles::GetDefaultStyle(const std::string& Type) const
	{
		OOX::Styles::Style defaultStyle;

		BOOST_FOREACH( const OOX::Styles::Style& style, *Named )
		{
			if ( ( style.Type == Type ) && style.Default.is_init() && ( *style.Default == 1 ) )
			{
				defaultStyle = style;   
			}
		}

		return defaultStyle;
	}

	const OOX::Styles::Style Styles::GetStyleWithTypeAndName (const std::string& Type, const std::string& Name) const
	{
		OOX::Styles::Style defaultStyle;
		BOOST_FOREACH( const OOX::Styles::Style& style, *Named )
		{
			if ( (style.Type == Type) && (style.name == Name) )
			{
				defaultStyle = style;   
			}
		}

		return defaultStyle;
	}

} // namespace OOX