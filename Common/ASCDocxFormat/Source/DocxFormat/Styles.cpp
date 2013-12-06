
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

	Styles::Styles(const OOX::CPath& filename)
	{
		read(filename);
	}

	Styles::~Styles()
	{

	}

	void Styles::read(const OOX::CPath& filename)
	{
		const XML::XDocument document(filename.GetPath());
		
		Default	=	document.Root.element("docDefaults");
		Lattent	=	document.Root.element("latentStyles");
		
		Fill(Named, document.Root, "style");
	}

	void Styles::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const FileType Styles::type() const
	{
		return FileTypes::Style;
	}

	const OOX::CPath Styles::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath Styles::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	const OOX::Styles::Style Styles::GetStyleById(const std::string& Id) const
	{
		for (size_t i = 0; i < Named->size(); ++i)
		{
			if (Named->operator [](i).StyleId == Id)
				return Named->operator [](i);
		}

		return OOX::Styles::Style();
	}

	const OOX::Styles::Style Styles::GetDefaultStyle(const std::string& Type) const
	{
		OOX::Styles::Style defSt;

		const std::vector<Style>& st = Named.get();
		for (std::vector<Style>::const_iterator iter = st.begin(); iter != st.begin(); ++iter)
		{
			if (((*iter).Type == Type) && (*iter).Default.is_init() && (*(*iter).Default == 1))
			{
				defSt = (*iter);   
			}
		}

		return defSt;
	}

	const OOX::Styles::Style Styles::GetStyleWithTypeAndName (const std::string& Type, const std::string& Name) const
	{
		OOX::Styles::Style defSt;
		
		const std::vector<Style>& st = Named.get();
		for (std::vector<Style>::const_iterator iter = st.begin(); iter != st.begin(); ++iter)
		{
			if (((*iter).Type == Type) && ((*iter).name == Name))
			{
				defSt = (*iter);   
			}
		}

		return defSt;
	}

} // namespace OOX