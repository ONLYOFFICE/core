#pragma once
#ifndef XML_PRIVATE_INCLUDE_H_
#define XML_PRIVATE_INCLUDE_H_

#include <boost/operators.hpp>
#include <string>
#include "property.h"
#include <list>

namespace XML
{
	namespace Private
	{
		class Namespace 
		{
		public:
			Namespace(const std::string& prefix, const std::string& name);
			const bool operator==(const Namespace& rhs) const;
			const bool operator<(const Namespace& rhs) const;

		public:
			virtual const bool exist() const;
			const std::string ToString() const;
			const std::wstring ToWString() const;
			virtual void SaveToStringList(std::list<std::string>& strList)const;
			virtual void SaveToWStringList(std::list<std::wstring>& strList)const;
		private:
			std::string	Prefix;
			std::string	Name;
		public:
			const std::string GetPrefix()const;
			const std::string GetName()const;

		protected:
			Namespace();
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_INCLUDE_H_