
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XDeclaration.h"
#include "Encoding.h"


namespace XML
{
	namespace Private
	{

		XDeclaration::XDeclaration()
			: Version("1.0"),
			  Encoding("UTF-8"),
			  Standalone("yes")
		{
		}


		void XDeclaration::fromSource(boost::shared_ptr<XSingleSource> source)
		{
			source->find('?');
			source->next();
			source->skipSpace();

			while (source->get() != '?')
			{
				const std::pair<const std::string, const std::string> pair = source->getPair();
				setValue(pair.first, pair.second);
				source->skipSpace();
			}
			source->find('>');
			source->next();
		}


		void XDeclaration::fromSource(boost::shared_ptr<XWideSource> source)
		{
			source->find(L'?');
			source->next();
			source->skipSpace();

			while (source->get() != L'?')
			{
				const std::pair<const std::wstring, const std::wstring> pair = source->getPair();
				setValue(Encoding::unicode2utf8(pair.first), Encoding::unicode2utf8(pair.second));
				source->skipSpace();
			}
			source->find(L'>');
			source->next();
		}


		const std::string XDeclaration::ToString() const
		{
			return "<?xml version=\"" + Version.ToString() + "\" encoding=\"" + Encoding.ToString() + 
				(Standalone.is_init() ? ("\" standalone=\"" + Standalone.ToString()) : "") + "\" ?>"; 
		}


		const std::wstring XDeclaration::ToWString() const
		{
			return L"<?xml version=\"" + Encoding::utf82unicode(Version.ToString()) + L"\" encoding=\"" + L"Unicode" + 
				(Standalone.is_init() ? (L"\" standalone=\"" + Encoding::utf82unicode(Standalone.ToString())) : L"") + L"\" ?>"; 
		}


		void XDeclaration::setValue(const std::string& name, const std::string& value)
		{
			if (name == "version")
				Version = value;
			else if (name == "encoding")
				Encoding = value;
			else if (name == "standalone")
				Standalone = value;
		}

	} // namespace Private
} // namespace XML