#pragma once

#include "OfficeArtFOPTE.h"

namespace OfficeArt
{
	struct ComplexOfficeArtProperty
	{
	public:
		ComplexOfficeArtProperty () : officeArtProperty(), m_pComplexData(NULL)
		{
		}

		ComplexOfficeArtProperty (const OfficeArtFOPTE& office, const byte* pComplexData = NULL) : officeArtProperty(office), m_pComplexData(NULL) 
		{
			// [dataSize + 3*short]

			if (pComplexData)
			{
				unsigned int bufSize	=	office.GetValue() + sizeof(unsigned short) * 3;

				m_pComplexData			=	new byte [ bufSize ];
				if (m_pComplexData)
				{
					memset (m_pComplexData, 0, bufSize);
					memcpy (m_pComplexData, pComplexData, bufSize);
				}
			}
		}

		ComplexOfficeArtProperty (const ComplexOfficeArtProperty& prop) : officeArtProperty(prop.officeArtProperty), m_pComplexData(NULL)
		{
			// [dataSize + 3*short]

			if (prop.m_pComplexData)
			{
				unsigned int bufSize	=	officeArtProperty.GetValue() + sizeof(unsigned short) * 3;

				m_pComplexData			=	new byte[bufSize];
				if (m_pComplexData)
				{
					memset (m_pComplexData, 0, bufSize);
					memcpy (m_pComplexData, prop.m_pComplexData, bufSize);
				}
			}
		}

		~ComplexOfficeArtProperty()
		{
			RELEASEARRAYOBJECTS (m_pComplexData);
		}

	public:	

		OfficeArtFOPTE	officeArtProperty;
		byte*			m_pComplexData;
	};
}