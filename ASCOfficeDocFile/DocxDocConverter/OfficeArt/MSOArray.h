/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "Common.h"

#include <string>
#include <vector>

namespace OfficeArt
{
	class CPoint16
	{
	public:
		CPoint16 () : X(0), Y(0)
		{

		}

		CPoint16 (short nX, short nY) : X(nX), Y(nY)
		{

		}

		inline short GetX()
		{
			return X;
		}

		inline short GetY()
		{
			return Y;
		}

	public:

		short X;
		short Y;
	};

	class CPoint32
	{
	public:
		CPoint32 () : X(0), Y(0)
		{

		}

		CPoint32 (int nX, int nY) : X(nX), Y(nY)
		{

		}

		inline int GetX()
		{
			return X;
		}

		inline int GetY()
		{
			return Y;
		}

	public:

		int X;
		int Y;
	};
}

namespace OfficeArt
{
	template <class T> class CMSOArray
	{
	public:
		CMSOArray(bool cb_FFF0	= false) : m_data (NULL), m_cb_FFF0 (cb_FFF0)
		{

		}

		virtual ~CMSOArray()
		{
			RELEASEARRAYOBJECTS(m_data);
		}


		//
		inline void Add(const T& elem)
		{
			m_arr.push_back(elem);
		}

		inline void Add(const std::vector<T>& arr)
		{
			m_arr.clear();

			for (size_t i = 0; i < arr.size(); ++i)
				m_arr.push_back(arr[i]);
		}

		inline const unsigned char* Get()
		{
			if (m_data)
				return m_data;

			unsigned short count	=	(unsigned short)m_arr.size();
			unsigned short cbSize	=	sizeof(T);

			if (m_cb_FFF0)
				cbSize				=	0xFFF0;

			unsigned int offset		=	sizeof(unsigned short) * 3 + m_arr.size() * sizeof(T);
			m_data					=	new unsigned char [offset];

			if (m_data)
			{
				offset = 0;

				memcpy ( (m_data + offset), &count, sizeof(unsigned short));
				offset += sizeof(unsigned short);

				unsigned short	nElemsAlloc	=	count;

				memcpy ( (m_data + offset), &nElemsAlloc, sizeof(unsigned short));
				offset += sizeof(unsigned short);

				memcpy ( (m_data + offset), &cbSize, sizeof(unsigned short));
				offset += sizeof(unsigned short);

				for (size_t i = 0; i < m_arr.size(); ++i)
				{
					memcpy ( (m_data + offset), &m_arr[i], sizeof(T));
					offset +=  sizeof(T);
				}

				return m_data;
			}

			return NULL;
		}

		inline unsigned int GetSize()
		{
			if (m_cb_FFF0)
				return 4 * m_arr.size();

			return	m_arr.size() * sizeof(T);
		}

		inline void Clear()
		{
			m_arr.clear();
		}

		//
		inline static std::vector<CPoint32> GetPoints(const std::string& strPoints)
		{
			std::vector<CPoint32> values;
			if (strPoints.empty())
				return values;

			std::string str = strPoints;
			while(str.length())
			{
				size_t separator = str.find (';');
				if (string::npos != separator)
				{
					std::string point = str.substr (0,separator);
					size_t pt = point.find(',');
					if (string::npos == pt)
						break;

					unsigned int x = atoi (point.substr(0,pt).c_str());
					unsigned int y = atoi (point.substr(pt+1,point.length() - pt - 1).c_str());

					values.push_back (CPoint32(x,y));

					str	= str.substr(separator + 1,str.length() - separator - 1);

					continue;
				}

				size_t from = str.find(',');
				if (0 != from)
				{
					size_t pt = str.find(',');
					if (string::npos == pt)
						break;

					unsigned int x = atoi (str.substr(0,pt).c_str());
					unsigned int y = atoi (str.substr(pt+1,str.length() - pt - 1).c_str());

					values.push_back (CPoint32(x,y));
					break;
				}
			}

			return values;
		}

		inline static std::vector<int> GetArrInt32(const std::string& strValues, std::string sep)
		{
			std::vector<int> values;
			if (strValues.empty())
				return values;

			std::string str = strValues;
			while(str.length())
			{
				size_t separator = str.find(sep);
				if (string::npos != separator)
				{
					int number = atoi(str.substr(0,separator).c_str());
					values.push_back (number);

					str	= str.substr(separator + 1, str.length()-separator-1);
				}
				else
				{
					int number = atoi(str.c_str());
					values.push_back (number);

					break;
				}
			}

			return values;
		}


		inline static std::vector<unsigned int> GetValues(const std::string& strValues)
		{
			std::vector<unsigned int> values;
			if (strValues.empty())
				return values;

			std::string str = strValues;
			while(str.length())
			{
				size_t separator = str.find (',');
				if (string::npos != separator)
				{
					unsigned int number = ToFixed(atof(str.substr(0,separator).c_str()));
					values.push_back (number);

					str	= str.substr(separator + 1, str.length()-separator-1);
				}
				else
				{
					unsigned int number = ToFixed(atof (str.c_str()));
					values.push_back (number);

					break;
				}
			}

			return values;
		}


		inline static std::vector<OfficeArt::RECT> GetRectangles(const std::string& strValues)
		{
			std::vector<OfficeArt::RECT> rectangles;

			std::vector<int> values = CMSOArray<T>::GetArrInt32(strValues, ",");
			if (values.size())
			{
				int count = values.size() / 4;

				for(int i = 0; i < count; ++i)
				{
					rectangles.push_back(OfficeArt::RECT(values[0 + i * 4], values[1 + i*4], values[2 + i*4], values[3 + i*4]));
				}
			}

			return rectangles;
		}

	private:

		static unsigned int ToFixed(double dNumber)
		{
			if (dNumber < 0.0)
			{		
				return (unsigned int)(((long)dNumber - dNumber) * 65536.0) | (0xffff << 16);
			}	

			if (dNumber > 0.0)
			{
				return (unsigned int)((dNumber - (long)dNumber) * 65536.0);
			}

			return 0;
		}

	private:

		std::vector <T>	m_arr;
		unsigned char*			m_data;
		bool			m_cb_FFF0;
	};
}