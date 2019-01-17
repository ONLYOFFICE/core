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
#ifndef _PDF_READER_JBIG2STREAM_H
#define _PDF_READER_JBIG2STREAM_H

#include "Object.h"
#include "Stream.h"

namespace PdfReader
{
	class CList;
	class JBIG2Segment;
	class JBIG2Bitmap;
	class JArithmeticDecoder;
	class JArithmeticDecoderStats;
	class JBIG2HuffmanDecoder;
	struct JBIG2HuffmanTable;
	class JBIG2MMRDecoder;

	//------------------------------------------------------------------------

	class JBIG2Stream : public FilterStream
	{
	public:

		JBIG2Stream(Stream *pStream, Object *pGlobalsStream);
		virtual ~JBIG2Stream();
		virtual StreamType GetType()
		{
			return strJBIG2;
		}
		virtual void Reset();
		virtual void Close();
		virtual int GetChar();
		virtual int LookChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		void         ReadSegments();
		bool         ReadSymbolDictSegment(unsigned int unSegNum, unsigned int unLength, unsigned int *punRefSegs, unsigned int unRefSegsCount);
		void         ReadTextRegionSegment(unsigned int unSegNum, bool bImm, bool bLossless, unsigned int unLength, unsigned int *punRefSegs, unsigned int unRefSegsCount);
		JBIG2Bitmap *ReadTextRegion(bool bHuff, bool bRefine, int nW, int nH, unsigned int nNumInstances, unsigned int unLogStrips, int nNumSyms, JBIG2HuffmanTable *pSymCodeTab, unsigned int unSymCodeLen, JBIG2Bitmap **ppSyms, unsigned int unDefPixel, unsigned int unCombOp, unsigned int unTransposed, unsigned int unRefCorner, int nSOffset, JBIG2HuffmanTable *pHuffFSTable, JBIG2HuffmanTable *pHuffDSTable, JBIG2HuffmanTable *pHuffDTTable, JBIG2HuffmanTable *pHuffRDWTable, JBIG2HuffmanTable *pHuffRDHTable, JBIG2HuffmanTable *pHuffRDXTable, JBIG2HuffmanTable *pHuffRDYTable, JBIG2HuffmanTable *pHuffRSizeTable, unsigned int unTempl, int *pnATx, int *pnATy);
		void         ReadPatternDictSegment(unsigned int unSegNum, unsigned int unLength);
		void         ReadHalftoneRegionSegment(unsigned int unSegNum, bool bImm, bool bLossless, unsigned int unLength, unsigned int *punRefSegs, unsigned int unRefSegsCount);
		void         ReadGenericRegionSegment(unsigned int unSegNum, bool bImm, bool bLossless, unsigned int unLength);
		JBIG2Bitmap *ReadGenericBitmap(bool bMMr, int nW, int nH, int unTempl, bool bTpgdOn, bool bUseSkip, JBIG2Bitmap *pSkip, int *pATx, int *pATy, int nMMrDataLength);
		void         ReadGenericRefinementRegionSegment(unsigned int unSegNum, bool bImm, bool bLossless, unsigned int unLength, unsigned int *punRefSegs, unsigned int unRefSegsCount);
		JBIG2Bitmap *ReadGenericRefinementRegion(int nW, int nH, int nTempl, bool bTpgrOn, JBIG2Bitmap *pRefBitmap, int nRefDX, int nRefDY, int *pnAtx, int *pnAty);

		void ReadPageInfoSegment(unsigned int unLength);
		void ReadEndOfStripeSegment(unsigned int unLength);
		void ReadProfilesSegment(unsigned int unLength);
		void ReadCodeTableSegment(unsigned int unSegNum, unsigned int unLength);
		void ReadExtensionSegment(unsigned int unLength);

		JBIG2Segment *FindSegment(unsigned int unSegNum);
		void          DiscardSegment(unsigned int unSegNum);

		void ResetGenericStats(unsigned int unTempl, JArithmeticDecoderStats *pPrevStats);
		void ResetRefinementStats(unsigned int unTempl, JArithmeticDecoderStats *pPrevStats);
		void ResetIntStats(int nSymCodeLen);

		bool ReadUByte(unsigned int *pBuffer);
		bool ReadByte(int *pBuffer);
		bool ReadUWord(unsigned int *pBuffer);
		bool ReadULong(unsigned int *pBuffer);
		bool ReadLong(int *pBuffer);

	private:

		Object         m_oGlobalsStream;
		unsigned int   m_unPageW;
		unsigned int   m_unPageH;
		unsigned int   m_unCurPageH;
		unsigned int   m_unPageDefPixel;
		JBIG2Bitmap   *m_pPageBitmap;
		unsigned int   m_unDefCombOp;
		CList         *m_pSegments;       // [JBIG2Segment]
		CList         *m_pGlobalSegments; // [JBIG2Segment]
		Stream        *m_pCurStream;
		unsigned char *m_pDataPtr;
		unsigned char *m_pDataEnd;

		JArithmeticDecoder      *m_pArithDecoder;
		JArithmeticDecoderStats *m_pGenericRegionStats;
		JArithmeticDecoderStats *m_pRefinementRegionStats;
		JArithmeticDecoderStats *m_pIadhStats;
		JArithmeticDecoderStats *m_pIadwStats;
		JArithmeticDecoderStats *m_pIaexStats;
		JArithmeticDecoderStats *m_pIaaiStats;
		JArithmeticDecoderStats *m_pIadtStats;
		JArithmeticDecoderStats *m_pIaitStats;
		JArithmeticDecoderStats *m_pIafsStats;
		JArithmeticDecoderStats *m_pIadsStats;
		JArithmeticDecoderStats *m_pIardxStats;
		JArithmeticDecoderStats *m_pIardyStats;
		JArithmeticDecoderStats *m_pIardwStats;
		JArithmeticDecoderStats *m_pIardhStats;
		JArithmeticDecoderStats *m_pIariStats;
		JArithmeticDecoderStats *m_pIaidStats;
		JBIG2HuffmanDecoder     *m_pHuffDecoder;
		JBIG2MMRDecoder         *m_pMMrDecoder;
	};
}

#endif // _PDF_READER_JBIG2STREAM_H
