
#pragma once

namespace ImageStudio
{
	template<typename T>
	inline T ClampT( const T Value, const T Min, const T Max )
	{
		if( Value < Min ) return Min;
		if( Value > Max ) return Max;

		return Value;
	}

	
	template<typename T>
	inline void Exchange( T* pV1, T* pV2 )
	{
		T temp = *pV1;
		*pV1 = *pV2;
		*pV2 = temp;
	}


	inline float Random()
	{
		return ::rand() * (1.0f / RAND_MAX);
	}

	inline float Random( float fMin, float fMax )
	{
		return Random() * (fMax - fMin) + fMin;
	}

	inline double Random( double dMin, double dMax )
	{
		return Random() * (dMax - dMin) + dMin;
	}

	inline double Interpolate( double dMin, double dMax, double dTime )
	{
		return dTime * (dMax - dMin) + dMin;
	}


	
	inline int Floor( double value )
	{
		int result = int(value);
		
		return (value < result) ? result - 1 : result;
	}
	inline int Ceil( double value )
	{
		int result = int(value);

		return (value > result) ? result + 1 : result;
	}
	inline int Round( double value )
	{
		if( value < 0 )
			value -= 0.5;
		else
			value += 0.5;

		return int(value);
	}
	inline int Div255( int value )
	{
		// value [0..65535]
		return DWORD(value + 127) * 0x8081U >> 23;
	}

	inline int IntToByte( int value )
	{
		if( value < 0 )   value = 0; else
		if( value > 255 ) value = 255;
		
		return value;
	}

	inline int RealToInt( const double value )
	{
		// return int(dReal + 0.5);
		
		const double d = value + 6755399441055744.0;
		return *((int *)(&d));
	}

	inline int RealToByte( double value )
	{
		return IntToByte( RealToInt( value ) );
	}

	int ColorBlend( int dst, int src, double alpha )
	{
		return int(src * alpha + dst * (1 - alpha) + 0.5);
	}
	Gdiplus::Color GetColor( int color, int alpha )
	{
		return Gdiplus::Color( IntToByte(alpha), GetRValue(color), GetGValue(color), GetBValue(color) );
	}

	
	class ImageData
	{
	public:
		BYTE* pPixels;
		int   nStride;

	public:
		ImageData()
		{
			pPixels = NULL;
			nStride = 0;
		}

		ImageData( BYTE* pixels, int stride )
		{
			pPixels = pixels;
			nStride = stride;
		}

		void CopyTo( const ImageData& oResult, int nRowLength, int nCountRows ) const
		{
			const BYTE* src = pPixels;
			const BYTE* dst = oResult.pPixels;

			if( nStride == oResult.nStride && abs(nStride) == nRowLength )
			{
				::memcpy( (void*)dst, src, nCountRows * nRowLength );
			}
			else
			{
				int nSrcStride = nStride - nRowLength;
				int nDstStride = oResult.nStride - nRowLength;

				for( int i = 0; i < nCountRows; ++i, src += nSrcStride, dst += nDstStride )
				{
					::memcpy( (void*)dst, src, nRowLength );
				}
			}
		}

		void MirrowY( int nRowLength, int nCountRows )
		{
			BYTE* src = pPixels;
			BYTE* dst = pPixels + (nCountRows - 1) * nStride;
			
			int nSrcStride =  nStride - nRowLength;
			int nDstStride = -nStride - nRowLength;
			
			int nEnding = nRowLength & 3;

			nCountRows /= 2;
			nRowLength /= 4;

			for( int y = 0; y < nCountRows; ++y, src += nSrcStride, dst += nDstStride )
			{
				for( int x = 0; x < nRowLength; ++x, src += 4, dst += 4 )
				{
					Exchange( (int*)src, (int*)dst );
				}

				for( int i = 0; i < nEnding; i++ )
				{
					Exchange( src + i, dst + i );
				}

				src += nEnding;
				dst += nEnding;
			}
		}

		void Flip( int nCountRows )
		{
			pPixels += nCountRows * nStride;
			nStride = -nStride;
		}

		void Normalize( int nCountRows )
		{
			if( nStride < 0 )
				Flip( nCountRows );
		}

		void PackDims4( int& nCountPixelsInRow, int& nCountRows ) const
 		{
			if( abs(nStride) == nCountPixelsInRow * 4 )
			{
				nCountPixelsInRow *= nCountRows;
				nCountRows = 1;
			}
		}
	};

	
	BOOL MemCopy( const BYTE* pSrc, BYTE* pDst, int nSrcStride, int nDstStride, DWORD nCountRows, DWORD nRowLength )
	{
		// копирует построчно память между неперекрывающимися блоками
		
		try
		{
			if( nSrcStride == nDstStride && abs(nSrcStride) == nRowLength )
			{
				// оба блока памяти сплошные
				DWORD nSize = nRowLength * nCountRows;

				if( nSrcStride < 0 )
				{
					pSrc -= nSize;
					pDst -= nSize;
				}

				::memcpy( pDst, pSrc, nSize );
			}
			else
			{
				for( DWORD i = 0; i < nCountRows; ++i, pSrc += nSrcStride, pDst += nDstStride )
				{
					::memcpy( pDst, pSrc, nRowLength );
				}
			}

			return TRUE;
		}
		catch(...)
		{
			ATLTRACE(_T("Exception! Can`t copy memory!"));
			return FALSE;
		}
	}
	
	template<typename T>
	class CBuffer
	{
	protected:
		T* m_pBuffer;

	public:
		CBuffer() 
			: m_pBuffer( 0 )
		{
		}

		CBuffer( size_t size ) 
			: m_pBuffer( 0 )
		{
			Allocate( size );
		}

		~CBuffer()
		{
			Release();
		}
		
		T* Create( size_t size )
		{
			Release();
			
			Allocate( size );

			return m_pBuffer;
		}
		void Release()
		{
			if( m_pBuffer )
			{
				delete [] m_pBuffer;
				m_pBuffer = 0;
			}
		}

		bool IsEmpty() const
		{
			return 0 == m_pBuffer;
		}
		
		T* GetPtr() const
		{
			return m_pBuffer;
		}
		T* GetPtr( int offset ) const
		{
			return m_pBuffer + offset;
		}
		
		const T& operator[]( int index ) const
		{
			return m_pBuffer[index];
		}
		T& operator[]( int index )
		{
			return m_pBuffer[index];
		}

		operator const T*() const
		{
			return m_pBuffer;
		}
		operator T*() const
		{
			return const_cast<T*>(m_pBuffer);
		}

		operator bool () const
		{
			return m_pBuffer != NULL;
		}

		
		void Fill( const T* pSrcItems, size_t nCountItems )
		{
			::memcpy( GetPtr(), pSrcItems, nCountItems * sizeof(T) );
		}

		void Fill( int nInitValue, size_t nCountItems )
		{
			::memset( GetPtr(), nInitValue, nCountItems * sizeof(T) );
		}
	
	protected:
		void Allocate( size_t size )
		{
			if( size > 0 )
				m_pBuffer = new T[size];
		}
	};

	
	template<typename T>
	class RefArray
	{
		// это враппер над ATL массивами для простой работы с ними
	public:
		T*  ptr;
		int size;

		RefArray() 
			: ptr( 0 ), size( 0 )
		{
		}
		RefArray( T* pData, int nSize )
			: ptr( pData ), size( nSize )
		{
		}
		RefArray( ATL::CSimpleArray< T >& oArray )
			: ptr( oArray.GetData() ), size( oArray.GetSize() )
		{
		}
		RefArray( ATL::CAtlArray< T >& oArray )
			: ptr( oArray.GetData() ), size((int)oArray.GetCount() )
		{
		}
		RefArray( ATL::CSimpleArray< T >& oArray, int nOffset, int nSize )
			: ptr( oArray.GetData() + nOffset ), size( nSize )
		{
		}
		RefArray( ATL::CAtlArray< T >& oArray, int nOffset, int nSize )
			: ptr( oArray.GetData() + nOffset ), size( nSize )
		{
		}

		
		T& operator [](int index)
		{
			return ptr[index];
		}
		const T& operator [](int index) const
		{
			return ptr[index];
		}
		T& operator [](size_t index)
		{
			return ptr[index];
		}
		const T& operator [](size_t index) const
		{
			return ptr[index];
		}
	};

	
	class CRandom
	{
		// аналог сишного random()
	protected:
		unsigned long m_nRnd;

	public:
		CRandom( unsigned long init = 0 )
		{
			NewRnd( init );
		}

		void Init( unsigned long init )
		{
			NewRnd( init );
		}

		int Rand()
		{
			NewRnd( m_nRnd );
			return (m_nRnd >> 16) & 0x7fff;
		}

		int Rand( int nMaxLimit )
		{
			NewRnd( m_nRnd );
			return ((m_nRnd >> 17) * (unsigned long)nMaxLimit) >> 15;
		}

		float RandF()
		{
			return Rand() * (1.0f / 32767);
		}

		float RandF( float fMin, float fMax )
		{
			return RandF() * (fMax - fMin) + fMin;
		}
		double RandF( double dMin, double dMax )
		{
			return RandF() * (dMax - dMin) + dMin;
		}
	
	protected:
		void NewRnd( unsigned long init )
		{
			m_nRnd = init * 214013 + 2531011;
		}

	};

	class CAlphaTable
	{
	protected:
		BYTE m_oTable[512];

	public:
		CAlphaTable()
		{
			m_oTable[0] = 255;
		}

		int Calculate( double alpha )
		{
			const double c_dMinAlpha = 0.00196; // минимальное значение для альфы [0..1], до которого смешивание целых чисел [0..255] не происходит
			const double c_dMaxAlpha = 0.99804; // максимальное значение для альфы [0..1], после которого смешивание целых чисел [0..255] не происходит

			if( alpha < c_dMinAlpha )
			{
				m_oTable[0] = 1;
				return -1;
			}

			if( alpha > c_dMaxAlpha )
			{
				m_oTable[0] = 2;
				return 1;
			}

			CalculateEx( alpha );

			return 0;
		}

		int Calculate( BYTE alpha )
		{
			int nAlpha = alpha;

			if( 0 == nAlpha )
			{
				m_oTable[0] = 1;
				return -1;
			}

			if( 255 == nAlpha )
			{
				m_oTable[0] = 2;
				return 1;
			}

			CalculateEx( alpha );

			return 0;
		}

		int Blend( int dst, int src ) const
		{
			return m_oTable[src - dst + 256] + dst;
		}

		
		void CalculateEx( double alpha )
		{
			//double t = sin( alpha * IPP_PI2 );
			//alpha = t * t;

			m_oTable[0] = 0;

			for( int i = -255; i <= 255; i++ )
			{
				m_oTable[i + 256] = Floor(i * alpha + 0.5);
			}
		}

		void CalculateEx( BYTE alpha )
		{
			//CalculateEx( alpha / 255.0 );

			DWORD nAlpha = alpha;

			m_oTable[0] = 0;
			for( int i = 255; i > 0; i-- )
			{
				int a = DWORD(i * nAlpha + 127) * 0x8081U >> (7 + 16);
				m_oTable[i + 256] = a;
				m_oTable[256 - i] = -a;
			}
			m_oTable[256] = 0;
		}
		
		bool IsUsing() const
		{
			return (0 == m_oTable[0]);
		}

		bool IsEmpty() const
		{
			return (0 != m_oTable[0]);
		}

		bool IsZeroAlpha() const
		{
			return (1 == m_oTable[0]);
		}

		bool IsMaxAlpha() const
		{
			return (2 == m_oTable[0]);
		}

		bool IsCalculate() const
		{
			return (255 != m_oTable[0]);
		}
	};
	
	class CFileTimer
	{
	public:
		CFileTimer()
		{
			QueryPerformanceFrequency( &m_liStart );
			m_dFreq = 1000.0 / m_liStart.LowPart;

			QueryPerformanceCounter( &m_liStart );
		}


		void Reset()
		{
			QueryPerformanceCounter(&m_liStart);
		}
		void CheckTime( DWORD nCountItems = 0, LPCCH szFilePath = NULL )
		{
			LARGE_INTEGER liFinish;
			QueryPerformanceCounter( &liFinish );
			
			FILE* pFile = ::fopen( szFilePath ? szFilePath : "c:\\log.txt", "a" );
			if( pFile )
			{
				//интервал в миллисекундах
				double dTime = double(liFinish.QuadPart - m_liStart.QuadPart);
				
				::fprintf( pFile, "%.6lf", dTime * m_dFreq );
				
				if( nCountItems > 0 )
					::fprintf( pFile, ",  %.2lf times per item", dTime / nCountItems );

				::fprintf( pFile, "\n" );

				::fclose( pFile );
			}
		}

	
	protected:
		LARGE_INTEGER m_liStart;
		double m_dFreq;
	};

	class SimpleTimer
	{
		// простой таймер, аналог FileTimer
		// записывает результаты измерений времени после выхода за область видимости
	public:
		SimpleTimer()
		{
			QueryPerformanceFrequency( &m_liStart );
			m_dFreq = 1000.0 / m_liStart.LowPart;

			QueryPerformanceCounter( &m_liStart );
		}
		~SimpleTimer()
		{
			LARGE_INTEGER liFinish;
			QueryPerformanceCounter( &liFinish );
			
			FILE* pFile = ::fopen( "c:\\log.txt", "a" );
			if( pFile )
			{
				//интервал в миллисекундах
				double dTime = double(liFinish.QuadPart - m_liStart.QuadPart);
				
				::fprintf( pFile, "%.6lf\n", dTime * m_dFreq );
				::fclose( pFile );
			}
		}

	protected:
		LARGE_INTEGER m_liStart;
		double m_dFreq;
	};


	template <typename T>
	void QuickSort( T* base, size_t num )
	{
		if (num < 2)
			return;

		const int StackSize = 8*sizeof(void*) - 2;

		T* lostk[StackSize];
		T* histk[StackSize];

		int stkptr = 0;

		T* lo = base;
		T* hi = base + (num - 1);

	recurse:

		size_t size = (hi - lo) / sizeof(T) + 1;

		if (size <= 8) 
		{
			while (hi > lo) 
			{
				T* max = lo;
				for (T* p = lo + 1; p <= hi; p += 1) 
				{
					if (*p > *max) 
					{
						max = p;
					}
				}

				
				T tmp = *max; *max = *hi; *hi = tmp;

				hi -= 1;
			}
		}
		else 
		{
			T* mid = lo + (size / 2);

			if ( *lo > *mid )
			{
				T tmp = *lo; *lo = *mid; *mid = tmp;
			}
			if ( *lo > *hi )
			{
				T tmp = *lo; *lo = *hi; *hi = tmp;
			}
			if ( *mid > *hi )
			{
				T tmp = *mid; *mid = *hi; *hi = tmp;
			}

			T* loguy = lo;
			T* higuy = hi;

			for (;;) 
			{
				if (mid > loguy) 
				{
					do  
					{
						loguy += 1;
					} while (loguy < mid && *loguy <= *mid);
				}
				if (mid <= loguy) 
				{
					do  
					{
						loguy += 1;
					} while (loguy <= hi && *loguy <= *mid);
				}

				do  
				{
					higuy -= 1;
				} while (higuy > mid && *higuy > *mid);

				if (higuy < loguy)
					break;

				T tmp = *loguy; *loguy = *higuy; *higuy = tmp;

				if (mid == higuy)
					mid = loguy;
			}

			higuy += 1;
			if (mid < higuy) 
			{
				do  
				{
					higuy -= 1;
				} while (higuy > mid && *higuy == *mid);
			}
			if (mid >= higuy) 
			{
				do  
				{
					higuy -= 1;
				} while (higuy > lo && *higuy == *mid );
			}

			if ( higuy - lo >= hi - loguy ) 
			{
				if (lo < higuy) 
				{
					lostk[stkptr] = lo;
					histk[stkptr] = higuy;
					++stkptr;
				}

				if (loguy < hi) 
				{
					lo = loguy;
					goto recurse;
				}
			}
			else 
			{
				if (loguy < hi) 
				{
					lostk[stkptr] = loguy;
					histk[stkptr] = hi;
					++stkptr;
				}

				if (lo < higuy) 
				{
					hi = higuy;
					goto recurse;
				}
			}
		}

		if (--stkptr >= 0) 
		{
			lo = lostk[stkptr];
			hi = histk[stkptr];
			goto recurse;
		}
		else
			return;
	}




#define ALIGN_CODE( x ) __asm{ ALIGN x }
#define ALIGN_DATA( x ) __declspec(align(x))

	BOOL BgraImageToBmpFile( BYTE* pPixels, int nWidth, int nHeight, int nStride, LPCCH szFilePath, BOOL bFlipY = TRUE )
	{
		if( !pPixels || nWidth <= 0 || nHeight <= 0 || abs(nStride) < nWidth * 4 || !szFilePath )
			return FALSE;

		FILE* pFile = ::fopen( szFilePath, "wb" );
		if( !pFile )
			return FALSE;

		int length = (nWidth * 3 + 3) & ~3;

		BITMAPFILEHEADER oFileHeader;
		oFileHeader.bfType      = 'M' * 256 + 'B';
		oFileHeader.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPCOREHEADER) + length * nHeight;
		oFileHeader.bfReserved1 = 0;
		oFileHeader.bfReserved2 = 0;
		oFileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPCOREHEADER);

		BITMAPCOREHEADER oCoreHeader;
		oCoreHeader.bcSize     = sizeof(BITMAPCOREHEADER);
		oCoreHeader.bcWidth    = nWidth;
		oCoreHeader.bcHeight   = nHeight;
		oCoreHeader.bcPlanes   = 1;
		oCoreHeader.bcBitCount = 24;

		::fwrite( &oFileHeader, 1, sizeof(BITMAPFILEHEADER), pFile );
		::fwrite( &oCoreHeader, 1, sizeof(BITMAPCOREHEADER), pFile );

		if( !bFlipY )
		{
			pPixels += nStride * (nHeight - 1);
			nStride = -nStride;
		}
	
		CBuffer<BYTE> row(length);
		if( row.IsEmpty() )
		{
			fclose( pFile );
			return FALSE;
		}

		for( int nRow = nHeight; nRow > 0 ; --nRow, pPixels += nStride )
		{
			for( int x = 0; x < nWidth; x++ )
			{
				row[x*3 + 0] = pPixels[x*4 + 0];
				row[x*3 + 1] = pPixels[x*4 + 1];
				row[x*3 + 2] = pPixels[x*4 + 2];
			}

			::fwrite( row.GetPtr(), 1, length, pFile );
		}

		::fclose( pFile );

		return TRUE;
	}

	BOOL MediaDataToBmpFile( IUnknown* pSource, LPCCH szFilePath )
	{
		if( !pSource )
			return FALSE;

		// вытаскиваем из интерфейса uncompressed frame
		MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
		pSource->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame );
		if( !pFrame )
			return FALSE;

		long lColorSpace = 0; pFrame->get_ColorSpace( &lColorSpace );
		long lWidth      = 0; pFrame->get_Width( &lWidth );
		long lHeight     = 0; pFrame->get_Height( &lHeight );
		long lStride     = 0; pFrame->get_Stride( 0, &lStride );
		long lBufferSize = 0; pFrame->get_BufferSize( &lBufferSize );
		BYTE* pBuffer    = 0; pFrame->get_Buffer( &pBuffer);

		pFrame->Release();

		// проверяем совместимость кадра и формата
		if( 0x40 != (lColorSpace & 0x7fffffff) || lBufferSize < lStride * lHeight )
		{
			return FALSE;
		}

		return BgraImageToBmpFile( pBuffer, lWidth, lHeight, lStride, szFilePath, (lColorSpace & 0x80000000) ? TRUE : FALSE );
	}

	static const float ByteDiv255[256] = {
			 0.f,  1/255.f,  2/255.f,  3/255.f,  4/255.f,  5/255.f,  6/255.f,  7/255.f,  8/255.f,  9/255.f,
		10/255.f, 11/255.f, 12/255.f, 13/255.f, 14/255.f, 15/255.f, 16/255.f, 17/255.f, 18/255.f, 19/255.f,
		20/255.f, 21/255.f, 22/255.f, 23/255.f, 24/255.f, 25/255.f, 26/255.f, 27/255.f, 28/255.f, 29/255.f,
		30/255.f, 31/255.f, 32/255.f, 33/255.f, 34/255.f, 35/255.f, 36/255.f, 37/255.f, 38/255.f, 39/255.f,
		40/255.f, 41/255.f, 42/255.f, 43/255.f, 44/255.f, 45/255.f, 46/255.f, 47/255.f, 48/255.f, 49/255.f,
		50/255.f, 51/255.f, 52/255.f, 53/255.f, 54/255.f, 55/255.f, 56/255.f, 57/255.f, 58/255.f, 59/255.f,
		60/255.f, 61/255.f, 62/255.f, 63/255.f, 64/255.f, 65/255.f, 66/255.f, 67/255.f, 68/255.f, 69/255.f,
		70/255.f, 71/255.f, 72/255.f, 73/255.f, 74/255.f, 75/255.f, 76/255.f, 77/255.f, 78/255.f, 79/255.f,
		80/255.f, 81/255.f, 82/255.f, 83/255.f, 84/255.f, 85/255.f, 86/255.f, 87/255.f, 88/255.f, 89/255.f,
		90/255.f, 91/255.f, 92/255.f, 93/255.f, 94/255.f, 95/255.f, 96/255.f, 97/255.f, 98/255.f, 99/255.f,

		100/255.f, 101/255.f, 102/255.f, 103/255.f, 104/255.f, 105/255.f, 106/255.f, 107/255.f, 108/255.f, 109/255.f,
		110/255.f, 111/255.f, 112/255.f, 113/255.f, 114/255.f, 115/255.f, 116/255.f, 117/255.f, 118/255.f, 119/255.f,
		120/255.f, 121/255.f, 122/255.f, 123/255.f, 124/255.f, 125/255.f, 126/255.f, 127/255.f, 128/255.f, 129/255.f,
		130/255.f, 131/255.f, 132/255.f, 133/255.f, 134/255.f, 135/255.f, 136/255.f, 137/255.f, 138/255.f, 139/255.f,
		140/255.f, 141/255.f, 142/255.f, 143/255.f, 144/255.f, 145/255.f, 146/255.f, 147/255.f, 148/255.f, 149/255.f,
		150/255.f, 151/255.f, 152/255.f, 153/255.f, 154/255.f, 155/255.f, 156/255.f, 157/255.f, 158/255.f, 159/255.f,
		160/255.f, 161/255.f, 162/255.f, 163/255.f, 164/255.f, 165/255.f, 166/255.f, 167/255.f, 168/255.f, 169/255.f,
		170/255.f, 171/255.f, 172/255.f, 173/255.f, 174/255.f, 175/255.f, 176/255.f, 177/255.f, 178/255.f, 179/255.f,
		180/255.f, 181/255.f, 182/255.f, 183/255.f, 184/255.f, 185/255.f, 186/255.f, 187/255.f, 188/255.f, 189/255.f,
		190/255.f, 191/255.f, 192/255.f, 193/255.f, 194/255.f, 195/255.f, 196/255.f, 197/255.f, 198/255.f, 199/255.f,

		200/255.f, 201/255.f, 202/255.f, 203/255.f, 204/255.f, 205/255.f, 206/255.f, 207/255.f, 208/255.f, 209/255.f,
		210/255.f, 211/255.f, 212/255.f, 213/255.f, 214/255.f, 215/255.f, 216/255.f, 217/255.f, 218/255.f, 219/255.f,
		220/255.f, 221/255.f, 222/255.f, 223/255.f, 224/255.f, 225/255.f, 226/255.f, 227/255.f, 228/255.f, 229/255.f,
		230/255.f, 231/255.f, 232/255.f, 233/255.f, 234/255.f, 235/255.f, 236/255.f, 237/255.f, 238/255.f, 239/255.f,
		240/255.f, 241/255.f, 242/255.f, 243/255.f, 244/255.f, 245/255.f, 246/255.f, 247/255.f, 248/255.f, 249/255.f,
		250/255.f, 251/255.f, 252/255.f, 253/255.f, 254/255.f, 1.f
	};
}
