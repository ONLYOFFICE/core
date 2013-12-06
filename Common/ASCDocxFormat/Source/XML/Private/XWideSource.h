#pragma once
#ifndef XML_PRIVATE_XWIDE_SOURCE_INCLUDE_H_
#define XML_PRIVATE_XWIDE_SOURCE_INCLUDE_H_

#include <iterator>
#include <string>

namespace XML
{
	namespace Private
	{
		class XWideSource
		{
		public:
			enum eSourceType {estLittleEndian, estBigEndian, estStringSource};
		public:
			XWideSource(eSourceType type);
			~XWideSource();

		public:
			virtual void next();
			virtual void skipSpace();
			virtual	void find(const wchar_t value);
			virtual	void find(const wchar_t value1, const wchar_t value2);
			virtual void findAndSkip(const wchar_t value);

		public:
			virtual const std::pair<const std::wstring, const std::wstring> getName();
			virtual const std::pair<const std::wstring, const std::wstring> getPair();
			virtual const std::pair<const std::pair<const std::wstring, const std::wstring>, const std::wstring> getAttribute();

		public:
			virtual const wchar_t get() const;
			virtual const std::wstring getString(const wchar_t separator);
			virtual const std::wstring getString(const wchar_t separator1, const wchar_t separator2);

		private:
			const bool isSpace() const;
			const bool isSeparator() const;

			const std::wstring get(const wchar_t separator1, const wchar_t separator2);
			const std::wstring getToSeparator();
			const std::wstring getToSeparator(const wchar_t separator);
			const std::wstring getValue();

		protected:
			std::istreambuf_iterator<wchar_t>		m_iterator;
			wchar_t									m_current;
			eSourceType								m_SourceType;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XWIDE_SOURCE_INCLUDE_H_