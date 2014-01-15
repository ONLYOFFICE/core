#pragma once
#ifndef XML_PRIVATE_XSINGLE_SOURCE_INCLUDE_H_
#define XML_PRIVATE_XSINGLE_SOURCE_INCLUDE_H_

//#include "XSource.h"
#include <iterator>
#include <string>


namespace XML
{
	namespace Private
	{
		class XSingleSource// : public XSource
		{
		public:
			virtual ~XSingleSource();

		public:
			virtual void next();
			virtual void skipSpace();
			virtual void find(const char value);
			virtual void find(const char value1, const char value2);
			virtual void findAndSkip(const char value);
			virtual const char get() const;
			virtual const std::string getString(const char separator);
			virtual const std::string getString(const char separator1, const char separator2);

		public:
			virtual const std::pair<const std::string, const std::string> getName();
			virtual const std::pair<const std::string, const std::string> getPair();
			virtual const std::pair<const std::pair<const std::string, const std::string>, const std::string> getAttribute();

		private:
			const bool isSpace() const;
			const bool isSeparator() const;

			const std::string get(const char separator1, const char separator2);
			const std::string getToSeparator();
			const std::string getToSeparator(const char separator);
			const std::string getValue();

		protected:
			std::istreambuf_iterator<char>	m_iterator;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XSINGLE_SOURCE_INCLUDE_H_