#pragma once

#include "Prl.h"

namespace ASCDocFileFormat
{
	class GrpPrlAndIstd: public IStreamWriter
	{
	public:
		GrpPrlAndIstd() :  istd(0), grpprlSize(0)
		{
		}

		GrpPrlAndIstd( short _istd, const vector<Prl>& _grpprl ) : istd(_istd), grpprlSize(0)
		{
			for ( unsigned int i = 0; i < _grpprl.size(); i++ )
			{
				this->grpprlSize += (unsigned short)_grpprl[i].Size();
				this->grpprl.push_back( _grpprl[i] );
			}
		}

		GrpPrlAndIstd( const GrpPrlAndIstd& gpai ) : istd(0), grpprlSize(0)
		{
			this->istd = gpai.istd;
			this->grpprlSize = gpai.grpprlSize;

			for ( unsigned int i = 0; i < gpai.grpprl.size(); i++ )
			{
				this->grpprl.push_back( gpai.grpprl[i] );
			}
		}

		virtual ~GrpPrlAndIstd()
		{
		}

		inline bool operator == ( const GrpPrlAndIstd& gpai )
		{
			if ( ( this->istd != gpai.istd ) || ( this->grpprlSize != gpai.grpprlSize ) || ( this->grpprl.size() != gpai.grpprl.size() ) )
			{
				return false;
			}
			else
			{
				unsigned int minPrlCount = min( this->grpprl.size(), gpai.grpprl.size() );

				for ( unsigned int i = 0; i < minPrlCount; i++ )
				{
					if ( this->grpprl[i] != gpai.grpprl[i] )
					{
						return false;
					}
				}
			}

			return true;
		}

		inline bool operator != ( const GrpPrlAndIstd& gpai )
		{
			return !( *this == gpai );
		}

		inline GrpPrlAndIstd& operator = ( const GrpPrlAndIstd& gpai )
		{
			if ( *this != gpai )
			{
				this->istd = gpai.istd;
				this->grpprlSize = gpai.grpprlSize;

				this->grpprl.clear();

				for ( unsigned int i = 0; i < gpai.grpprl.size(); i++ )
				{
					this->grpprl.push_back( gpai.grpprl[i] );
				}
			}

			return *this;
		}

		inline unsigned long Size() const
		{
			return ( sizeof(this->istd) + this->grpprlSize );
		}

		inline Prl operator[] ( unsigned int index ) const
		{
			return this->grpprl[index];
		}

		inline unsigned int GetPrlCount() const
		{
			return (unsigned int)this->grpprl.size();
		}

		inline vector<Prl> GetPrls() const
		{
			return this->grpprl;
		}

		inline short GetIstd() const
		{
			return istd;
		}

		// IStreamWriter
		virtual byte* GetBytes(unsigned long* size) const
		{
			byte* bytes = NULL;

			if ( size != NULL )
			{
				*size = sizeof(this->istd) + this->grpprlSize;
				bytes = new byte[*size];

				if ( bytes != NULL )
				{
					memset( bytes, 0, *size );
					FormatUtils::SetBytes( bytes, this->istd );

					byte* prlBytes = NULL;
					unsigned long prlSize = 0;
					unsigned int prlPrevSize = 0; 

					for ( unsigned int i = 0; i < this->grpprl.size(); i++ )
					{
						prlBytes = this->grpprl[i].GetBytes( &prlSize );

						if ( prlBytes != NULL )
						{
							memcpy( ( bytes + sizeof(this->istd) + prlPrevSize ), prlBytes, prlSize );
							prlPrevSize += prlSize;

							RELEASEARRAYOBJECTS (prlBytes);
						}
					}
				}
			}

			return bytes;
		}

	private:

		short			istd; //The style applied to this paragraph, cell marker or table row marker.
		vector<Prl>		grpprl;
		unsigned short	grpprlSize;
	};
}