#pragma once
#ifndef ODT_MANIFEST_BASE_TABLE_INCLUDE_H_
#define ODT_MANIFEST_BASE_TABLE_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>


namespace Odt
{
	namespace Manifest
	{
		template<class T>
		class BaseTable : public WritingElement
		{
		public:
			void clear()
			{
				m_table.clear();
			}

			void push_back(const T& value)
			{
				m_table.push_back(value);
			}

		protected:
			std::vector<T> m_table;
		};
	} // namespace Manifest
} // namespace Odt

#endif // ODT_MANIFEST_BASE_TABLE_INCLUDE_H_