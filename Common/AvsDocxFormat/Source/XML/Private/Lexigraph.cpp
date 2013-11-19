
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "Lexigraph.h"
//#include <cctype>
//#include <boost/algorithm/string/replace.hpp>


namespace XML
{
	namespace Private
	{

		const std::string Lexigraph::fromSource(const std::string& input)
		{
			std::string output = input;

			size_t pos = output.find("&lt;");
			while(pos != output.npos)
			{
				output.replace(pos, 4, "<");
				pos = output.find("&lt;", pos + 1);
			}

			pos = output.find("&gt;");
			while(pos != output.npos)
			{
				output.replace(pos, 4, ">");
				pos = output.find("&gt;", pos + 1);
			}

			pos = output.find("&apos;");
			while(pos != output.npos)
			{
				output.replace(pos, 6, "\'");
				pos = output.find("&apos;", pos + 1);
			}

			pos = output.find("&quot;");
			while(pos != output.npos)
			{
				output.replace(pos, 6, "\"");
				pos = output.find("&quot;", pos + 1);
			}

			pos = output.find("&amp;");
			while(pos != output.npos)
			{
				output.replace(pos, 5, "&");
				pos = output.find("&amp;", pos + 1);
			}

			//boost::replace_all(output, "&amp;", "&");
			//boost::replace_all(output, "&lt;", "<");
			//boost::replace_all(output, "&gt;", ">");
			//boost::replace_all(output, "&apos;", "'");
			//boost::replace_all(output, "&quot;", "\"");
			return output;
		}


		const std::string Lexigraph::toSource(const std::string& input)
		{
			std::string output = input;
			size_t pos = output.find('&');
			while(pos != output.npos)
			{
				output.replace(pos, 1, "&amp;");
				pos = output.find('&', pos + 1);
			}

			pos = output.find('<');
			while(pos != output.npos)
			{
				output.replace(pos, 1, "&lt;");
				pos = output.find('<', pos + 1);
			}

			pos = output.find('>');
			while(pos != output.npos)
			{
				output.replace(pos, 1, "&gt;");
				pos = output.find('>', pos + 1);
			}

			pos = output.find('\'');
			while(pos != output.npos)
			{
				output.replace(pos, 1, "&apos;");
				pos = output.find('\'', pos + 1);
			}

			pos = output.find('\"');
			while(pos != output.npos)
			{
				output.replace(pos, 1, "&quot;");
				pos = output.find('\"', pos + 1);
			}

			//boost::replace_all(output, "&", "&amp;");
			//boost::replace_all(output, "<", "&lt;");
			//boost::replace_all(output, ">", "&gt;");
			//boost::replace_all(output, "'", "&apos;");
			//boost::replace_all(output, "\"", "&quot;");
			return output;
		}


		const std::wstring Lexigraph::fromSource(const std::wstring& input)
		{
			std::wstring output = input;

			size_t pos = output.find(L"&lt;");
			while(pos != output.npos)
			{
				output.replace(pos, 4, L"<");
				pos = output.find(L"&lt;", pos + 1);
			}

			pos = output.find(L"&gt;");
			while(pos != output.npos)
			{
				output.replace(pos, 4, L">");
				pos = output.find(L"&gt;", pos + 1);
			}

			pos = output.find(L"&apos;");
			while(pos != output.npos)
			{
				output.replace(pos, 6, L"\'");
				pos = output.find(L"&apos;", pos + 1);
			}

			pos = output.find(L"&quot;");
			while(pos != output.npos)
			{
				output.replace(pos, 6, L"\"");
				pos = output.find(L"&quot;", pos + 1);
			}

			pos = output.find(L"&amp;");
			while(pos != output.npos)
			{
				output.replace(pos, 5, L"&");
				pos = output.find(L"&amp;", pos + 1);
			}

			//boost::replace_all(output, "&amp;", "&");
			//boost::replace_all(output, "&lt;", "<");
			//boost::replace_all(output, "&gt;", ">");
			//boost::replace_all(output, "&apos;", "'");
			//boost::replace_all(output, "&quot;", "\"");
			return output;
		}


		const std::wstring Lexigraph::toSource(const std::wstring& input)
		{
			std::wstring output = input;
			size_t pos = output.find(L'&');
			while(pos != output.npos)
			{
				output.replace(pos, 1, L"&amp;");
				pos = output.find(L'&', pos + 1);
			}

			pos = output.find(L'<');
			while(pos != output.npos)
			{
				output.replace(pos, 1, L"&lt;");
				pos = output.find(L'<', pos + 1);
			}

			pos = output.find(L'>');
			while(pos != output.npos)
			{
				output.replace(pos, 1, L"&gt;");
				pos = output.find(L'>', pos + 1);
			}

			pos = output.find(L'\'');
			while(pos != output.npos)
			{
				output.replace(pos, 1, L"&apos;");
				pos = output.find(L'\'', pos + 1);
			}

			pos = output.find(L'\"');
			while(pos != output.npos)
			{
				output.replace(pos, 1, L"&quot;");
				pos = output.find(L'\"', pos + 1);
			}

			//boost::replace_all(output, "&", "&amp;");
			//boost::replace_all(output, "<", "&lt;");
			//boost::replace_all(output, ">", "&gt;");
			//boost::replace_all(output, "'", "&apos;");
			//boost::replace_all(output, "\"", "&quot;");
			return output;
		}

	} // namespace Private
} // namespace XML