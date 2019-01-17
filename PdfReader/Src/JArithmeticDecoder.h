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
