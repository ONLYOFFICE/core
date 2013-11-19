
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XSingleSource.h"
#include "Lexigraph.h"
#include "Exception\not_implement.h"


namespace XML
{
	namespace Private
	{

		XSingleSource::~XSingleSource()
		{
		}


		void XSingleSource::next()
		{
			++m_iterator;
		}


		void XSingleSource::skipSpace()
		{
			for (; isSpace() ; ++m_iterator);
		}


		void XSingleSource::find(const char value)
		{
			//if (NULL == m_iterator._Mycont)
			//	return;
			for (; *m_iterator != value; ++m_iterator);
		}


		void XSingleSource::find(const char value1, const char value2)
		{
			for (; ((*m_iterator != value1) && (*m_iterator != value2)); ++m_iterator);
		}


		void XSingleSource::findAndSkip(const char value)
		{
			find(value);
			next();
		}


		const char XSingleSource::get() const
		{
			//if (NULL == m_iterator._Mycont)
			//	return 0;
			return *m_iterator;
		}


		const std::string XSingleSource::getString(const char separator)
		{
			std::string result;
			while (*m_iterator != separator)
				result += *m_iterator++;
			return Lexigraph::fromSource(result);
		}


		const std::string XSingleSource::getString(const char separator1, const char separator2)
		{
			std::string result;
			while ((*m_iterator != separator1) && (*m_iterator != separator2))
				result += *m_iterator++;
			return Lexigraph::fromSource(result);
		}


		const std::pair<const std::string, const std::string> XSingleSource::getName()
		{
			std::string prefix = getToSeparator(':');
			std::string name;
			if (get() == ':')
			{
				next();
				name = getToSeparator();
			}
			else
			{
				std::swap(prefix, name);
			}
			return std::make_pair(prefix, name);
		}


		const std::pair<const std::string, const std::string> XSingleSource::getPair()
		{
			const std::string name = get(' ', '=');
			const std::string value = getValue();
			return std::make_pair(name, value);
		}


		const std::pair<const std::pair<const std::string, const std::string>, const std::string> XSingleSource::getAttribute()
		{
			const std::pair<const std::string, const std::string> name = getName();
			const std::string value = getValue();
			return std::make_pair(name, value);
		}


		const bool XSingleSource::isSpace() const
		{
			if (*m_iterator == ' ' || *m_iterator == '\n' || *m_iterator == '\r' || *m_iterator == '\t')
				return true;
			return false;
		}


		const bool XSingleSource::isSeparator() const
		{
			if (*m_iterator == '<' || *m_iterator == '>' || *m_iterator == '=' || *m_iterator == '/')
				return true;
			return false;
		}


		const std::string XSingleSource::get(const char separator1, const char separator2)
		{
			skipSpace();
			std::string result;
			while (*m_iterator != separator1 && *m_iterator != separator2)
				result += *m_iterator++;
			return result;
		}


		const std::string XSingleSource::getToSeparator()
		{
			skipSpace();
			std::string result;
			while (!isSeparator() && !isSpace())
				result += *m_iterator++;
			return result;
		}


		const std::string XSingleSource::getToSeparator(const char separator)
		{
			skipSpace();
			std::string result;
			while (!isSeparator() && !isSpace() && *m_iterator != separator)
				result += *m_iterator++;
			return result;
		}


		const std::string XSingleSource::getValue()
		{
			find('"', '\'');
			char EndChar = *m_iterator;
			next();
			const std::string value = getString(EndChar);//Значение аттрибута - это то, что заключено между _одинаковыми_ кавычками
			next();
			return value;
		}

	} // namespace Private
} // namespace XML