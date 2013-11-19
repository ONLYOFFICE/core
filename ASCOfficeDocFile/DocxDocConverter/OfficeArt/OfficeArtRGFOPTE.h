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

		operator byte*() const
		{
			return this->bytes;
		}

		operator const byte*() const
		{
			return (const byte*)(bytes);
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

			for ( list<ComplexOfficeArtProperty>::const_iterator iter = complexOfficeArtProperties.begin(); iter != complexOfficeArtProperties.end(); ++iter)
			{
				size += iter->officeArtProperty.Size();

				if ( iter->officeArtProperty.GetOpid().GetFComplex() && (NULL != iter->m_pComplexData))
				{
					size += iter->officeArtProperty.GetValue() + sizeof(unsigned short) * 3;
				}
			}

			if ( size != 0 )
			{
				bytes = new byte[size];

				if (bytes)
				{
					memset (bytes, 0, size);

					unsigned int offset = 0;

					for ( list<ComplexOfficeArtProperty>::const_iterator iter = complexOfficeArtProperties.begin(); iter != complexOfficeArtProperties.end(); ++iter)
					{
						memcpy( ( bytes + offset ), (byte*)iter->officeArtProperty, iter->officeArtProperty.Size() );
						offset += iter->officeArtProperty.Size();
					}

					for ( list<ComplexOfficeArtProperty>::const_iterator iter = complexOfficeArtProperties.begin(); iter != complexOfficeArtProperties.end(); ++iter)
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

		list<ComplexOfficeArtProperty> complexOfficeArtProperties;
		byte* bytes;
		unsigned int size;
	};
}