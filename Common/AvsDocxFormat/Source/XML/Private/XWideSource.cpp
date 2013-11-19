
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XWideSource.h"
#include "Lexigraph.h"
#include "Encoding.h"
#include "Exception\not_implement.h"


namespace XML
{
	namespace Private
	{
		XWideSource::XWideSource(eSourceType type)
			: m_SourceType(type)
			, m_current(0)
		{
		}


		XWideSource::~XWideSource()
		{
		}


		void XWideSource::next()
		{
			if(m_SourceType == estLittleEndian)
			{
				m_current = *m_iterator;
				++m_iterator;
				m_current = ((*m_iterator)<<8) + m_current;
				++m_iterator;
			}
			else if(m_SourceType == estBigEndian)
			{
				m_current = *m_iterator;
				++m_iterator;
				m_current = (m_current << 8) + (*m_iterator);
				++m_iterator;
			}
			else
			{
				m_current = *m_iterator;
				++m_iterator;
			}
		}


		void XWideSource::skipSpace()
		{
			for (; isSpace() ; next()/*++m_iterator*/);
		}


		void XWideSource::find(const wchar_t value)
		{
			for (; /**m_iterator*/m_current != value; next()/*++m_iterator*/);
		}


		void XWideSource::find(const wchar_t value1, const wchar_t value2)
		{
			for (; ((/**m_iterator*/m_current != value1) && (/**m_iterator*/m_current != value2)); next()/*++m_iterator*/);
		}


		void XWideSource::findAndSkip(const wchar_t value)
		{
			find(value);
			next();
		}


		const std::pair<const std::wstring, const std::wstring> XWideSource::getName()
		{
			std::wstring prefix = getToSeparator(L':');
			std::wstring name;
			if (get() == L':')
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


		const std::pair<const std::wstring, const std::wstring> XWideSource::getPair()
		{
			const std::wstring name = get(L' ', L'=');
			const std::wstring value = getValue();
			return std::make_pair(name, value);
		}


		const std::pair<const std::pair<const std::wstring, const std::wstring>, const std::wstring> XWideSource::getAttribute()
		{
			const std::pair<const std::wstring, const std::wstring> name = getName();
			const std::wstring value = getValue();
			return std::make_pair(name, value);
		}


		const wchar_t XWideSource::get() const
		{
			return m_current;
		}


		const std::wstring XWideSource::getString(const wchar_t separator)
		{
			std::wstring result;
			while (m_current != separator)
				//result += *m_iterator++;
			{
				result += m_current;
				next();
			}
			return Lexigraph::fromSource(result);
		}


		const std::wstring XWideSource::getString(const wchar_t separator1, const wchar_t separator2)
		{
			std::wstring result;
			while ((m_current != separator1) && (m_current != separator2))
			//	result += *m_iterator++;
			{
				result += m_current;
				next();
			}
			return Lexigraph::fromSource(result);
		}


		const bool XWideSource::isSpace() const
		{
			if (/**m_iterator*/m_current == L' ' || /**m_iterator*/m_current == L'\n' || /**m_iterator*/m_current == L'\r' || /**m_iterator*/m_current == L'\t')
				return true;
			return false;
		}


		const bool XWideSource::isSeparator() const
		{
			if (/**m_iterator*/m_current == L'<' || /**m_iterator*/m_current == L'>' || /**m_iterator*/m_current == L'=' || /**m_iterator*/m_current == L'/')
				return true;
			return false;
		}


		const std::wstring XWideSource::get(const wchar_t separator1, const wchar_t separator2)
		{
			skipSpace();
			std::wstring result;
			while (/**m_iterator*/m_current != separator1 && /**m_iterator*/m_current != separator2)
			//	result += *m_iterator++;
			{
				result += m_current;
				next();
			}
			return result;
		}


		const std::wstring XWideSource::getToSeparator()
		{
			skipSpace();
			std::wstring result;
			while (!isSeparator() && !isSpace())
			{
				result += m_current;
				next();
			}
			//	result += *m_iterator++;
			return result;
		}


		const std::wstring XWideSource::getToSeparator(const wchar_t separator)
		{
			skipSpace();
			std::wstring result;
			while (!isSeparator() && !isSpace() && /**m_iterator*/m_current != separator)
			{
				result += m_current;
				next();
				//result += *m_iterator++;
			}
			return result;
		}


		const std::wstring XWideSource::getValue()
		{
			find(L'"', L'\'');
			wchar_t EndChar = get();//*m_iterator;
			next();
			const std::wstring value = getString(EndChar);//Значение аттрибута - это то, что заключено между _одинаковыми_ кавычками
			next();
			return value;
		}

	} // namespace Private
} // namespace XML