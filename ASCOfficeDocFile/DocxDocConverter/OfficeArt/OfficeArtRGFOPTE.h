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

#include "ComplexOfficeArtProperty.h"

namespace OfficeArt
{
	class OfficeArtRGFOPTE
	{
	public:

		OfficeArtRGFOPTE () : bytes(NULL), size(0)
		{

		}

		OfficeArtRGFOPTE (const OfficeArtRGFOPTE& prop) :  complexOfficeArtProperties(prop.complexOfficeArtProperties), bytes(NULL), size(prop.size)
		{
			Initialize();
		}

		inline unsigned int PushComplexProperty (const ComplexOfficeArtProperty& complex)
		{
			complexOfficeArtProperties.push_back(complex);

			Initialize();

			return (unsigned int)complexOfficeArtProperties.size();
		}

		inline unsigned int Clear()
		{
			complexOfficeArtProperties.clear();

			Initialize();

			return (unsigned int)complexOfficeArtProperties.size();
		}

		inline unsigned int Count() const
		{
			return (unsigned int)this->complexOfficeArtProperties.size();
		}

		operator unsigned char*() const
		{
			return this->bytes;
		}

		operator const unsigned char*() const
		{
			return (const unsigned char*)(bytes);
		}

		inline unsigned int Size() const
		{
			return size;
		}

		~OfficeArtRGFOPTE()
		{
			RELEASEARRAYOBJECTS (bytes);
		}

	private:

		void Initialize()
		{
			RELEASEARRAYOBJECTS (bytes);

			size = 0;

			for ( std::list<ComplexOfficeArtProperty>::const_iterator iter = complexOfficeArtProperties.begin(); iter != complexOfficeArtProperties.end(); ++iter)
			{
				size += iter->officeArtProperty.Size();

				if ( iter->officeArtProperty.GetOpid().GetFComplex() && (NULL != iter->m_pComplexData))
				{
					size += iter->officeArtProperty.GetValue() + sizeof(unsigned short) * 3;
				}
			}

			if ( size != 0 )
			{
				bytes = new unsigned char[size];

				if (bytes)
				{
					memset (bytes, 0, size);

					unsigned int offset = 0;

					for ( std::list<ComplexOfficeArtProperty>::const_iterator iter = complexOfficeArtProperties.begin(); iter != complexOfficeArtProperties.end(); ++iter)
					{
						memcpy( ( bytes + offset ), (unsigned char*)iter->officeArtProperty, iter->officeArtProperty.Size() );
						offset += iter->officeArtProperty.Size();
					}

					for ( std::list<ComplexOfficeArtProperty>::const_iterator iter = complexOfficeArtProperties.begin(); iter != complexOfficeArtProperties.end(); ++iter)
					{
						if ( iter->officeArtProperty.GetOpid().GetFComplex() && (NULL != iter->m_pComplexData))
						{
							unsigned int buffSize	=	iter->officeArtProperty.GetValue() + sizeof(unsigned short) * 3;

							memcpy ((bytes + offset), iter->m_pComplexData, buffSize);

							offset					+=	buffSize;
						}
					}
				}
			}
		}

	private:

		std::list<ComplexOfficeArtProperty> complexOfficeArtProperties;
		unsigned char* bytes;
		unsigned int size;
	};
}