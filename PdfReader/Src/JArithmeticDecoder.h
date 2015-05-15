#ifndef _PDF_READER_JARITHMETICDECODER_H
#define _PDF_READER_JARITHMETICDECODER_H

namespace PdfReader
{
	class Stream;

	//-------------------------------------------------------------------------------------------------------------------------------
	// JArithmeticDecoderStats
	//-------------------------------------------------------------------------------------------------------------------------------

	class JArithmeticDecoderStats
	{
	public:

		JArithmeticDecoderStats(int nContextSize);
		~JArithmeticDecoderStats();
		JArithmeticDecoderStats *Copy();
		void Reset();
		int GetContextSize()
		{
			return m_nContextSize;
		}
		void CopyFrom(JArithmeticDecoderStats *pStats);
		void SetEntry(unsigned int unCx, int nIndex, int nMPS);

	private:

		unsigned char *m_pContextTable; // cxTab[cx] = (i[cx] << 1) + mps[cx]
		int            m_nContextSize;

		friend class JArithmeticDecoder;
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// JArithmeticDecoder
	//-------------------------------------------------------------------------------------------------------------------------------

	class JArithmeticDecoder
	{
	public:

		JArithmeticDecoder();
		~JArithmeticDecoder();

		void SetStream(Stream *pStream)
		{
			m_pStream      = pStream;
			m_nDataSize    = 0;
			m_bLimitStream = false;
		}
		void SetStream(Stream *pStream, int nDataSize)
		{
			m_pStream      = pStream;
			m_nDataSize    = nDataSize;
			m_bLimitStream = true;
		}

		// Начинаем декодировать новый поток. Заполняем буффер и запускаем INITDEC.
		void Start();

		// Заново декодируем прервынный поток. Заново заполняем буффер, но не запускаем INITDEC. (Используется в  потоках JPEG 2000, где
		// данные могут быть разделены на блоки.)
		void Restart(int nDataSize);

		// Читаем оставшиеся данные в потоке.
		void Cleanup();

		// Декодируем один бит.
		int DecodeBit(unsigned int unContext, JArithmeticDecoderStats *pStats);

		// Декодируем один байт.
		int DecodeByte(unsigned int unContext, JArithmeticDecoderStats *pStats);

		bool DecodeInt(int *pnValue, JArithmeticDecoderStats *pStats);

		unsigned int DecodeIAID(unsigned int unCodeLen, JArithmeticDecoderStats *pStats);

	private:

		unsigned int ReadByte();
		int DecodeIntBit(JArithmeticDecoderStats *pStats);
		void ByteIn();

	private:

		static unsigned int arrunQeTable[47];
		static int          arrnNMPSTable[47];
		static int          arrnNLPSTable[47];
		static int          arrnSwitchTable[47];

		unsigned int m_unBuffer0;
		unsigned int m_unBuffer1;
		unsigned int m_unC;
		unsigned int m_unA;
		int          m_nCT;

		unsigned int m_unPrev;

		Stream      *m_pStream;
		int          m_nDataSize;
		bool         m_bLimitStream;
	};
}

#endif // _PDF_READER_JARITHMETICDECODER_H
