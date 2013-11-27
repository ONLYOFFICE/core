#pragma once
#include "stdafx.h"
#include "../Writers/Writer.h"
#include "../ASCOfficeUtils/ZLIB/zlib-1.2.3/contrib/minizip/zip.h"

namespace NSMobi
{
	class CStream
	{
	protected:
		BYTE* m_pBuffer;
		BYTE* m_pBufferMem;
		
		DWORD m_lSize;

	public:

		CStream()
		{
			m_pBuffer = 0;

			m_pBufferMem	= m_pBuffer;
			m_lSize			= 0;
		}
		~CStream()
		{
			RELEASEARRAYOBJECTS(m_pBuffer);
		}
		AVSINLINE void DestroyNoAttack()
		{
			m_pBuffer = 0;

			m_pBufferMem	= m_pBuffer;
			m_lSize			= 0;
		}

		AVSINLINE void FromFile(CFile* pFile)
		{
			m_lSize		= (DWORD)pFile->GetFileSize();
			m_pBuffer	= new BYTE[m_lSize];

			m_pBufferMem = m_pBuffer;
			pFile->ReadFile(m_pBuffer, m_lSize);
		}

		AVSINLINE void FromBuffer(BYTE* pData, DWORD size)
		{
			m_lSize		= size;
			m_pBuffer	= pData;

			m_pBufferMem = m_pBuffer;
		}
		AVSINLINE void CreateFromBuffer(DWORD size)
		{
			m_lSize		= size;
			m_pBuffer	= new BYTE[m_lSize];

			m_pBufferMem = m_pBuffer;
		}

		AVSINLINE DWORD GetSize()
		{
			return m_lSize;
		}
		
		AVSINLINE BOOL IsValid() const
		{
			if (m_pBuffer)
				return TRUE;

			return FALSE;
		}
		AVSINLINE BYTE* GetBuffer()
		{
			return m_pBuffer;
		}
		
		AVSINLINE void Seek(int nOrigin = 0)
		{
			m_pBufferMem = m_pBuffer + nOrigin;
		}
		AVSINLINE void Skip(int nSkip = 0)
		{
			m_pBufferMem += nSkip;
		}

		AVSINLINE int GetPosition()
		{
			return (int)(m_pBufferMem - m_pBuffer);
		}
		AVSINLINE BYTE* GetData()
		{
			return m_pBuffer;
		}
		AVSINLINE BYTE* GetCurrentData()
		{
			return m_pBufferMem;
		}
		
		AVSINLINE BYTE ReadByte()
		{
			BYTE value = *m_pBufferMem;
			m_pBufferMem += sizeof(BYTE);
			return value;
		}
		AVSINLINE short ReadShort()
		{
			return (short)((ReadByte() << 8) | ReadByte());
		}
		AVSINLINE USHORT ReadUShort()
		{
			return (USHORT)((ReadByte() << 8) | ReadByte());
		}
		AVSINLINE long ReadLong()
		{
			return (long)((ReadByte() << 24) | (ReadByte() << 16) | (ReadByte() << 8) | ReadByte());
		}
		AVSINLINE DWORD ReadDWORD()
		{
			return (DWORD)((ReadByte() << 24) | (ReadByte() << 16) | (ReadByte() << 8) | ReadByte());
		}
		AVSINLINE ULONG64 ReadULONG64()
		{
			return (ULONG64)((ReadDWORD() * 0x0100000000) | ReadDWORD());
		}
		AVSINLINE double ReadDouble()
		{
			double value = *(double*)(m_pBufferMem);
			m_pBufferMem += sizeof(double);
			return value;
		}
		AVSINLINE float ReadFloat()
		{
			float value = *(float*)(m_pBufferMem);
			m_pBufferMem += sizeof(float);
			return value;
		}
		AVSINLINE CString ReadStringW(int nLen = -1)
		{
			if (-1 != nLen)
			{
				CString strRet((WCHAR*)m_pBufferMem, nLen);
				m_pBufferMem += nLen * sizeof(WCHAR);
				return strRet;
			}
			return _T("");
		}
		AVSINLINE CString ReadStringA(int nLen = -1)
		{
			if (-1 != nLen)
			{
				CString strRet((CHAR*)m_pBufferMem, nLen);
				m_pBufferMem += nLen * sizeof(CHAR);
				return strRet;
			}
			return _T("");
		}
		AVSINLINE CString ReadStringUTF8(int nLen = -1)
		{
			if (-1 != nLen)
			{
				CString strRet = CFile::GetCStringFromUTF8(m_pBufferMem, nLen);
				m_pBufferMem += nLen;
				return strRet;
			}
			return _T("");
		}

		AVSINLINE void WriteByte(const BYTE& val)
		{
			*m_pBufferMem = val;
			++m_pBufferMem;
		}
		AVSINLINE void WriteUShort(const USHORT& val)
		{
			*m_pBufferMem = (BYTE)(val >> 8);
			++m_pBufferMem;
			*m_pBufferMem = (BYTE)val;
			++m_pBufferMem;
		}
		AVSINLINE void WriteLong(const LONG& val)
		{
			*m_pBufferMem = (BYTE)(val >> 24);
			++m_pBufferMem;
			*m_pBufferMem = (BYTE)(val >> 16);
			++m_pBufferMem;
			*m_pBufferMem = (BYTE)(val >> 8);
			++m_pBufferMem;
			*m_pBufferMem = (BYTE)val;
			++m_pBufferMem;
		}
		AVSINLINE void WriteDWORD(DWORD val)
		{
			*m_pBufferMem = (BYTE)(val >> 24);
			++m_pBufferMem;
			*m_pBufferMem = (BYTE)(val >> 16);
			++m_pBufferMem;
			*m_pBufferMem = (BYTE)(val >> 8);
			++m_pBufferMem;
			*m_pBufferMem = (BYTE)val;
			++m_pBufferMem;
		}
		AVSINLINE void WriteSet(BYTE val, LONG lSize)
		{
			memset(m_pBufferMem, val, lSize);
			m_pBufferMem += lSize;
		}
		AVSINLINE void WriteData(BYTE* pData, LONG lSize)
		{
			memcpy(m_pBufferMem, pData, lSize);
			m_pBufferMem += lSize;
		}
		AVSINLINE void Write(CStream* pStream)
		{
			memcpy(m_pBufferMem, pStream->GetData(), pStream->GetPosition());
			m_pBufferMem += pStream->GetPosition();
		}
		AVSINLINE void WriteCStringA(const CStringA& str)
		{
			CStringA* pStr = const_cast<CStringA*>(&str);
			WriteData((BYTE*)pStr->GetBuffer(), str.GetLength());
		}

		AVSINLINE void DumpToFile(CFile& oFile)
		{
			oFile.WriteFile((void*)m_pBuffer, GetPosition());
		}
	};

	class CTextDecoder
	{
	public:
		static void PalmDecode(BYTE* pBufferSrc, LONG lSrcLen, BYTE*& pBufferDst, LONG& lDstLen)
		{
			BYTE* input = (BYTE*)pBufferSrc;
			/*BYTE* input = new BYTE[lSrcLen];
			for (LONG j = 0; j < lSrcLen; ++j) 
			{
				CHAR charMem = (CHAR)pBufferSrc[j];
				input[j] = (charMem < 0) ? (_Byte)(charMem + 256) : (_Byte)charMem;
			}*/
			
			int nCurIndexSrc	= 0;
			int nCurIndexDst	= 0;
			pBufferDst = new BYTE[max(6000, 8 * lSrcLen)];
			BYTE* output = pBufferDst;

			BYTE c = 0;
			while (nCurIndexSrc < lSrcLen)
			{
				c = input[nCurIndexSrc++];
				if (c >= 1 && c <= 8)  // copy 'c' bytes
				{
					while (c--) 
						output[nCurIndexDst++] = input[nCurIndexSrc++];
				}
				else if (c <= 0x7F)  
				{
					// 0, 09-7F = self
					output[nCurIndexDst++] = c;
				}
				else if (c >= 0xC0) 
				{ 
					// space + ASCII char
					output[nCurIndexDst++] = ' ';
					output[nCurIndexDst++] = c ^ 0x80;
				}
				else 
				{ 
					// 80-BF repeat sequences
					USHORT _c = (c << 8) + input[nCurIndexSrc++];
					USHORT di = (_c & 0x3FFF) >> 3;
					for ( int n = (_c & 7) + 3; ((n--) > 0); ++nCurIndexDst ) 
						output[nCurIndexDst] = output[nCurIndexDst - di];
				}
			}
			//output[nCurIndexDst++] = 0;
			lDstLen = nCurIndexDst;
			//RELEASEARRAYOBJECTS(input);
		}

		static bool PalmDoc_memcmp(BYTE* a, BYTE* b, LONG len)
		{
			/*for (LONG i = 0; i < len; ++i)
			{
				if (a[i] != b[i])
					return false;
			}
			return true;*/
			return (0 == memcmp(a, b, len)) ? true : false;
		}
		static AVSINLINE LONG PalmDoc_find(BYTE* data, const BYTE& cur, const LONG& pos, const LONG& chunk_len)
		{
			for (LONG i = pos - chunk_len; i > -1; --i)
			{
				//if (PalmDoc_memcmp(data + i, data + pos, chunk_len))
				//	return i;
				if (data[i] == cur)
				{				
					BYTE* a = data + i + 1;
					BYTE* b = data + pos + 1;

					int j = 1;
					for (; j < chunk_len; ++j, ++a, ++b)
					{
						if (*a != *b)
							break;
					}
					if (j == chunk_len)
						return i;
				}
				
				//if (0 == memcmp(data + i, data + pos, chunk_len))
				//	return i;
			}
			return pos;
		}

		static void PalmEncode(BYTE* pBufferSrc, LONG lSrcLen, BYTE*& pBufferDst, LONG& lDstLen, LONG lLenAll)
		{
			pBufferDst = new BYTE[2 * lSrcLen + lLenAll - lSrcLen + 2];
			BYTE* output = pBufferDst;

			LONG i = 0, j = 0, chunk_len = 0, dist = 0;
			DWORD compound = 0;
			BYTE c = 0, n = 0;
			bool found;
			
			BYTE temp[8];
			LONG temp_len = 0;

			while (i < lSrcLen) 
			{
				c = pBufferSrc[i];
				//do repeats
				if ( i > 10 && (lSrcLen - i) > 10) 
				{
					found = false;
					for (chunk_len = 10; chunk_len > 2; --chunk_len) 
					{
						j = PalmDoc_find(pBufferSrc, c, i, chunk_len);
						dist = i - j;
						if (j < i && dist <= 2047) 
						{
							found = true;
							compound = (dist << 3) + chunk_len - 3;
							*(output++) = BYTE(0x80 + (compound >> 8));
							*(output++) = BYTE(compound & 0xFF);
							i += chunk_len;
							break;
						}
					}
					if (found) 
						continue;
				}

				//write single character
				++i;
				if (c == 32 && i < lSrcLen) 
				{
					n = pBufferSrc[i];
					if ( n >= 0x40 && n <= 0x7F) 
					{
						*(output++) = BYTE(n^0x80); 
						++i; 
						continue;
					}
				}
				if (c == 0 || (c > 8 && c < 0x80))
					*(output++) = BYTE(c);
				else 
				{ 
					// Write binary data
					j = i;
					temp[0] = c; 
					temp_len = 1;
					while (j < lSrcLen && temp_len < 8) 
					{
						c = pBufferSrc[j];
						if (c == 0 || (c > 8 && c < 0x80)) 
							break;
						temp[temp_len++] = c; 
						++j;
					}
					i += temp_len - 1;
					*(output++) = (BYTE)temp_len;
					for (j = 0; j < temp_len; ++j) 
						*(output++) = (BYTE)temp[j];
				}
			}
			lDstLen = (LONG)(output - pBufferDst);

			LONG lExtraLen = (lLenAll - lSrcLen);
			if (lExtraLen > 0)
			{
				memcpy(output, pBufferSrc + lSrcLen, lExtraLen);
				lDstLen += lExtraLen;
				output += lExtraLen;
			}

			if (TRUE)
			{
				*output = (BYTE)lExtraLen;
				++lDstLen;
			}
		}


		
	public:
		static void EncInt(int nValue, bool bForward, int& nLen, BYTE*& pBytes)
		{
			CAtlArray<BYTE> oArray;
			while (true)
			{
				BYTE b = nValue & 0x7F;
				nValue >>= 7;	// shift value to the right by 7 bits

				oArray.Add(b);
				if (nValue == 0)
					break;
			}
			nLen = (int)oArray.GetCount();
			if (0 != nLen)
			{
				if (bForward)
					oArray[0] |= 0x80;
				else
					oArray[nLen - 1] |= 0x80;
			}
			pBytes = new BYTE[nLen];
			for (int i = 0; i < nLen; ++i)
			{
				pBytes[i] = oArray[nLen - i - 1];
			}
		}
		static void DecInt(int& nValue, int& nLen, bool bForward, BYTE* pBytes, int nSrcLen)
		{
			nValue = 0;
			CAtlArray<BYTE> oArray;
			if (bForward)
			{
				for (int i = 0; i < nSrcLen; ++i)
				{
					oArray.Add(pBytes[i] & 0x7F);
					if (pBytes[i] & 0x80)
						break;
				}
				nLen = (int)oArray.GetCount();
				for (int i = 0; i < nLen; ++i)
				{
					nValue <<= 7;
					nValue |= oArray[i];
				}
			}
			else
			{
				for (int i = nSrcLen - 1; i >= 0; --i)
				{
					oArray.Add(pBytes[i] & 0x7F);
					if (pBytes[i] & 0x80)
						break;
				}
				nLen = (int)oArray.GetCount();
				for (int i = nLen - 1; i >= 0; --i)
				{
					nValue <<= 7;
					nValue |= oArray[i];
				}
			}
		}
		static void GetTralingData(BYTE* pRecord, DWORD lExtraDataFlag, int& nStart, int& nEnd)
		{
			DWORD lFlags = lExtraDataFlag >> 1;
			
			int num = 0;
			while (lFlags)
			{
				num += 1;
				if (lFlags & 0x01)
				{
					int sz			= 0;
					int consumed	= 0;
					DecInt(sz, consumed, false, pRecord + nStart, nEnd - nStart + 1); 
					
					if (sz > consumed)
					{
						//data[num] = record[-sz:-consumed]
					}
					
					nEnd = nEnd - sz;
				}
				lFlags >>= 1;
			}
			// Read multibyte chars if any
			if (lExtraDataFlag & 0x01)
			{
				// Only the first two bits are used for the size since there can
				// never be more than 3 trailing multibyte chars
				int	sz			= (pRecord[nEnd] & 0x03) + 1;
				int consumed	= 1;
				if (sz > consumed)
				{
					//data[0] = record[-sz:-consumed];
				}
				nEnd = nEnd - sz;
			}
		}
	};

	class CMobiReader
	{
	public:
		class CPDBHeader
		{
		public:
			CString		Name;
			USHORT		Attributes;
			USHORT		Version;

			DWORD		CreationDate;
			DWORD		ModificationDate;
			DWORD		LastBackupDate;
			DWORD		ModificationNumber;

			DWORD		AppInfoID;
			DWORD		SortInfoID;

			CString		Type;

			DWORD		UniqueIdSeed;
			DWORD		NextRecordListID;

			USHORT		NumberOfRecords;

		public:
			CPDBHeader()
			{
				Name				= _T("");
				Attributes			= 0;
				Version				= 0;

				CreationDate		= 0;
				ModificationDate	= 0;
				LastBackupDate		= 0;
				ModificationNumber	= 0;

				AppInfoID			= 0;
				SortInfoID			= 0;

				Type				= _T("BOOK");

				UniqueIdSeed		= 0;
				NextRecordListID	= 0;

				NumberOfRecords		= 0;
			}
			~CPDBHeader()
			{
			}

		public:
			void ReadFromStream(CStream* pStream)
			{
				Name				= pStream->ReadStringA(32);
				Attributes			= pStream->ReadUShort();
				Version				= pStream->ReadUShort();

				CreationDate		= pStream->ReadDWORD();
				ModificationDate	= pStream->ReadDWORD();
				LastBackupDate		= pStream->ReadDWORD();
				ModificationNumber	= pStream->ReadDWORD();

				AppInfoID			= pStream->ReadDWORD();
				SortInfoID			= pStream->ReadDWORD();

				Type				= pStream->ReadStringA(8);

				UniqueIdSeed		= pStream->ReadDWORD();
				NextRecordListID	= pStream->ReadDWORD();

				NumberOfRecords		= pStream->ReadUShort();
			}
			void WriteToStream(CStream* pStream, CString& strNameBook, LONG nRecords)
			{
				// name
				CStringA strName = (CStringA)strNameBook;
				strName.Replace(' ', '_');
				int len = strName.GetLength();
				if (31 < len)
				{
					strName = strName.Mid(0, 31);
					len = 31;
				}

				pStream->WriteCStringA(strName);
				for (int i = len; i < 32; ++i)
					pStream->WriteByte(0);

				//
				DWORD dwTime = GetCurrentTime();
				pStream->WriteUShort(0);
				pStream->WriteUShort(0);
				pStream->WriteDWORD(dwTime);
				pStream->WriteDWORD(dwTime);

				pStream->WriteDWORD(0);
				pStream->WriteDWORD(0);
				pStream->WriteDWORD(0);
				pStream->WriteDWORD(0);

				CStringA strType("BOOKMOBI");
				pStream->WriteCStringA(strType);

				pStream->WriteDWORD((DWORD)(2 * nRecords - 1));
				pStream->WriteDWORD(0);
				pStream->WriteUShort((USHORT)nRecords);
			}
		};

		class CSectionHeader
		{
		public:
			DWORD	Offset;
			DWORD	Flags;
			DWORD	Value;

		public:
			CSectionHeader()
			{
				Offset	= 0;
				Flags	= 0;
				Value	= 0;
			}
			~CSectionHeader()
			{
			}
			AVSINLINE void Read(CStream* pStream)
			{
				Offset	= pStream->ReadDWORD();
				Flags	= pStream->ReadByte();

				BYTE b1	= pStream->ReadByte();
				BYTE b2 = pStream->ReadByte();
				BYTE b3	= pStream->ReadByte();

				Value	= b1 << 16 | b2 << 8 | b3;
			}
		};

		class CSection
		{
		public:
			CSectionHeader	m_oHeader;
			DWORD			Length;

		public:
			CSection()
			{
				Length = 0;
			}
			~CSection()
			{
			}
		};

		class CHuffmanReader
		{
		public:
			class CDictInfo1
			{
			public:
				BYTE codelen;
				BYTE term;
				DWORD maxcode;

			public:
				CDictInfo1()
				{
					codelen		= 0;
					term		= 0;
					maxcode		= 0;
				}
				~CDictInfo1()
				{
				}
				CDictInfo1(const CDictInfo1& oSrc)
				{
					*this = oSrc;
				}
				CDictInfo1& operator=(const CDictInfo1& oSrc)
				{
					codelen		= oSrc.codelen;
					term		= oSrc.term;
					maxcode		= oSrc.maxcode;

					return *this;
				}
			};
			class CDictInfo2
			{
			public:
				USHORT blen;
				BYTE* buffer;

			public:
				CDictInfo2()
				{
					blen		= 0;
					buffer		= NULL;
				}
				~CDictInfo2()
				{
					RELEASEARRAYOBJECTS(buffer);
				}
				CDictInfo2(const CDictInfo2& oSrc)
				{
					*this = oSrc;
				}
				CDictInfo2& operator=(const CDictInfo2& oSrc)
				{
					blen		= oSrc.blen;
					buffer		= NULL;

					if (0 != blen)
					{
						buffer = new BYTE[blen];
						memcpy(buffer, oSrc.buffer, blen);
					}
					return *this;
				}
			};

		private:
			CAtlMap<DWORD, CDictInfo1> m_dict1;
			CAtlArray<DWORD>	m_mincode;
			CAtlArray<DWORD>	m_maxcode;
			CAtlMap<DWORD, CDictInfo2> m_dict;

		public:
			CHuffmanReader()
			{
			}
			~CHuffmanReader()
			{
			}

		public:
			void LoadTables(CStream* pStream, CSection* pSections, LONG lCountSects)
			{
				m_dict1.RemoveAll();
				m_dict.RemoveAll();
				m_mincode.RemoveAll();
				m_maxcode.RemoveAll();
				
				if (0 != lCountSects)
					LoadHuff(pStream, pSections[0]);
				for (LONG i = 1; i < lCountSects; ++i)
					LoadCdic(pStream, pSections[i]);
			}

			void LoadHuff(CStream* pStream, CSection& oSect)
			{
				pStream->Seek(oSect.m_oHeader.Offset);

				CString  strId = pStream->ReadStringA(4);
				if (_T("HUFF") != strId)
					return;

				pStream->Skip(4);
				DWORD offset1 = pStream->ReadDWORD();
				DWORD offset2 = pStream->ReadDWORD();

				pStream->Seek(oSect.m_oHeader.Offset + offset1);
				for (int i = 0; i < 256; ++i)
				{
					DWORD dwVal = pStream->ReadDWORD();
					CDictInfo1 info;
					info.codelen	= (BYTE)(dwVal & 0x1F);
					info.term		= (BYTE)(dwVal & 0x80);
					info.maxcode	= dwVal >> 8;
					info.maxcode	= ((info.maxcode + 1) << (32 - info.codelen)) - 1;
					m_dict1.SetAt(dwVal, info);
				}

				pStream->Seek(oSect.m_oHeader.Offset + offset2);

				DWORD dict2[64];
				for (int i = 0; i < 64; ++i)
					dict2[i] = pStream->ReadDWORD();

				m_mincode.Add(0);
				m_maxcode.Add(0xFFFFFFFF);

				for (int i = 0; i < 64; i += 2)
				{
					DWORD value = dict2[i] << (32 - i / 2);
					m_mincode.Add(value);
				}
				for (int i = 1; i < 64; i += 2)
				{
					DWORD value = dict2[i] << (32 - i / 2);
					m_maxcode.Add(value);
				}				
			}
			void LoadCdic(CStream* pStream, CSection& oSect)
			{
				pStream->Seek(oSect.m_oHeader.Offset);

				CString strId = pStream->ReadStringA(4);
				if (_T("CDIC") != strId)
					return;

				pStream->Skip(4);
				DWORD phrases	= pStream->ReadDWORD();
				DWORD bits		= pStream->ReadDWORD();

				int n = min((int)1 << bits, (int)(phrases - m_dict.GetCount()));

				for (int i = 0; i < n; ++i)
				{
					USHORT value = pStream->ReadUShort();

					int pos = pStream->GetPosition();
					CDictInfo2 info;

					pStream->Seek(16 + value);
					info.blen = pStream->ReadUShort();
					info.blen &= 0x8000;

					info.buffer = new BYTE[info.blen];
					memcpy(info.buffer, pStream->GetCurrentData(), info.blen);
					pStream->Seek(pos);

					m_dict.SetAt(value, info);
				}
			}

			void Decode(BYTE* pBufferSrc, LONG lCountSrc, BYTE*& pBufferDst, LONG& lCountDst)
			{
				LONG bitsleft = lCountSrc * 8;
				BYTE* data = new BYTE[lCountSrc + 8];
				memcpy(data, pBufferSrc, lCountSrc);
				memset(data + lCountSrc, 0x00, 8);

				CStream oStream;
				oStream.FromBuffer(data, lCountSrc + 8);

				CBufferedText oBufferedText;

				int pos = 0;
				ULONG64 x = oStream.ReadDWORD();
				int n = 32;

				while (true)
				{
					if (n <= 0)
					{
						pos += 4;
						oStream.Seek(pos);
						x = oStream.ReadULONG64();
						n += 32;
					}

					DWORD code = (DWORD)((x >> n) & 0xFFFFFFFF);

					CAtlMap<DWORD, CDictInfo1>::CPair* pPair1 = m_dict1.Lookup(code >> 24);
					if (NULL == pPair1)
						break;

					CDictInfo1 info1 = pPair1->m_value;
					DWORD max_code = 0;

					if (!info1.term)
					{
						while (code < m_mincode[info1.codelen])
							info1.codelen += 1;

						max_code = m_maxcode[info1.codelen];
					}

					n -= info1.codelen;
					bitsleft -= info1.codelen;

					if (bitsleft < 0)
						break;

					DWORD r = (max_code - code) >> (32 - info1.codelen);

					CAtlMap<DWORD, CDictInfo2>::CPair* pPair2 = m_dict.Lookup(r);
					if (NULL == pPair2)
						break;

					if (!pPair2->m_value.blen)
					{
						pPair2->m_value.blen = 1;
					}
					oBufferedText.WriteData(pPair2->m_value.buffer, pPair2->m_value.blen);
				}

				oStream.DestroyNoAttack();

				lCountDst = (LONG)oBufferedText.GetCurSize();
				pBufferDst = new BYTE[lCountDst];
				memcpy(pBufferDst, oBufferedText.GetData(), lCountDst);
			}
		};

		class CEXTHRecord
		{
		public:
			enum E_EXTHRecord
			{
				exthDRMServerId			= 1,
                exthDRMCommerceID		= 2,
                exthDRMEbookID			= 3,
                exthAuthor				= 100,
                exthPublisher			= 101,
                exthImpirit				= 102,
                exthDescription			= 103,
                exthIsbn				= 104,
                exthSubject				= 105,
                exthPublishingDate		= 106,
                exthReview				= 107,
                exthContributor			= 108,
                exthRights				= 109,
                exthSubjectCode			= 110,
                exthType				= 111,
                exthSource				= 112,
                exthAsin				= 113,
                exthVersionNumber		= 114,
                exthSample				= 115,
                exthStartReading		= 116,
                exthAdult				= 117,
                exthRetailPrice			= 118,
                exthRetailPriceCurrency	= 119,
				exthDictShortName		= 200,
                exthCoverOffset			= 201,
                exthThumbOffset			= 202,
                exthHasFakeCover		= 203,
                exthCreatorSoftware		= 204,
                exthCreatorMajorVersion	= 205,
                exthCreatorMinorVersion	= 206,
                exthCreatorBuildNumber	= 207,
                exthWatermark			= 208,
                exthTamperProofKeys		= 209,
                exthFontSignature		= 300,
                exthClippingLimit		= 401,
                exthPublisherLimit		= 402,
                exthTTS					= 404,
                exthCdEtype				= 501,
                exthLastUpdateTime		= 502,
                exthUpdateTitle			= 503,

				exthUnknown				= 1000
			};

			E_EXTHRecord		m_eType;
			DWORD				m_dwValue;
			CString				m_strValue;

		public:
			CEXTHRecord()
			{
				m_eType			= exthUnknown;
				m_dwValue		= 0;
				m_strValue		= _T("");
			}
			~CEXTHRecord()
			{
			}

			AVSINLINE void Read(CStream* pStream)
			{
				m_eType			= (E_EXTHRecord)pStream->ReadDWORD();
				DWORD dwLength	= pStream->ReadDWORD();

				if (12 == dwLength)
				{
					m_dwValue = pStream->ReadDWORD();
				}
				else
				{
					m_strValue = pStream->ReadStringUTF8(dwLength - 8);
				}
			}
		};

		class CEXTHHeader
		{
		public:
			CString					m_strType;
			CAtlArray<CEXTHRecord>	m_arRecords;

		public:
			CEXTHHeader() : m_arRecords()
			{
				m_strType		= _T("");
			}
			~CEXTHHeader()
			{
			}

			AVSINLINE void Read(CStream* pStream)
			{
				int pos1 = pStream->GetPosition();

				m_strType		= pStream->ReadStringA(4);
				DWORD dwSize	= pStream->ReadDWORD();
				DWORD dwLen		= pStream->ReadDWORD();

				m_arRecords.RemoveAll();
				for (DWORD i = 0; i < dwLen; ++i)
				{
					m_arRecords.Add();
					m_arRecords[i].Read(pStream);
				}

				int pos2 = pStream->GetPosition();
				int posE = pos1 + (int)dwSize;

				if (pos2 < posE)
				{
					pStream->Skip(posE - pos2);
				}
				else
				{
					int mod = (pos2 - pos1) % 4;
					if (0 != mod)
						pStream->Skip(4 - mod);
				}
			}

			static void WriteRec(CStream& oStream, DWORD dwType, DWORD dwCommand)
			{
				oStream.WriteDWORD(dwType);
				oStream.WriteDWORD(12);
				oStream.WriteDWORD(dwCommand);
			}
			static void WriteRec(CStream& oStream, DWORD dwType, CStringA strCommand)
			{
				oStream.WriteDWORD(dwType);
				oStream.WriteDWORD((DWORD)strCommand.GetLength() + 8);
				oStream.WriteCStringA(strCommand);
			}

			AVSINLINE int GetCoverOffset()
			{
				size_t nCount = m_arRecords.GetCount();
				for (size_t i = 0; i < nCount; ++i)
				{
					if (CEXTHRecord::exthCoverOffset == m_arRecords[i].m_eType)
					{
						return (int)m_arRecords[i].m_dwValue;
					}
				}
				return -1;
			}
		};

		class CMobiHeader
		{
		public:
			USHORT			Compression;
			DWORD			TextLength;
			USHORT			RecordCount;
			USHORT			RecordSize;
			DWORD			CurrentPosition;

			USHORT			EncryptionType;

			CString			Type;
			DWORD			HeaderLength;
			DWORD			MobiType;
			DWORD			TextEncoding;

			DWORD			UID;
			DWORD			Version;
			
			DWORD			OrtographicIndex;
			DWORD			InflectionIndex;
			DWORD			IndexNames;
			DWORD			IndexKeys;

			DWORD			ExtraIndex0;
			DWORD			ExtraIndex1;
			DWORD			ExtraIndex2;
			DWORD			ExtraIndex3;
			DWORD			ExtraIndex4;
			DWORD			ExtraIndex5;

			DWORD			FirstNonBookIndex;
			DWORD			FullNameOffset;
			DWORD			FullNameLength;
			DWORD			Locale;
			DWORD			InputLanguage;
			DWORD			OutputLanguage;
			DWORD			MinVersion;
			DWORD			FirstImageIndex;

			DWORD			HuffmanRecordOffset;
			DWORD			HuffmanRecordCount;
			DWORD			HuffmanTableOffset;
			DWORD			HuffmanTableLength;

			DWORD			ExthFlags;

			bool			IsDRM;

			DWORD			DRMOffset;
			DWORD			DRMCount;
			DWORD			DRMSize;
			DWORD			DRMFlags;

			bool			IsExtra;

			DWORD			FirstContentRecord;
			DWORD			LastContentRecord;

			DWORD			FCIS_Number;
			DWORD			FCIS_Count;
			DWORD			FLIS_Number;
			DWORD			FLIS_Count;

			DWORD			SrcsRecordIndex;
			DWORD			NumSrcsRecords;

			DWORD			ExtraDataFlags;

			bool			IsMultibytes;
			bool			IsIndexingBytes;
			bool			IsUncrossableBreaks;

			DWORD			PrimaryIndexRecord;

			CEXTHHeader		m_oEXTH;

		public:
			CMobiHeader()
			{
				Compression		= 0;
				TextLength		= 0;
				RecordCount		= 0;
				RecordSize		= 0;
				CurrentPosition = 0;

				EncryptionType	= 0;

				Type			= _T("");
				HeaderLength	= 0;
				MobiType		= 0;
				TextEncoding	= 0;

				UID				= 0;
				Version			= 0;
				
				OrtographicIndex	= 0;
				InflectionIndex		= 0;
				IndexNames			= 0;
				IndexKeys			= 0;

				ExtraIndex0			= 0;
				ExtraIndex1			= 0;
				ExtraIndex2			= 0;
				ExtraIndex3			= 0;
				ExtraIndex4			= 0;
				ExtraIndex5			= 0;

				FirstNonBookIndex	= 0;
				FullNameOffset		= 0;
				FullNameLength		= 0;
				Locale				= 0;
				InputLanguage		= 0;
				OutputLanguage		= 0;
				MinVersion			= 0;
				FirstImageIndex		= 0;

				HuffmanRecordOffset = 0;
				HuffmanRecordCount	= 0;
				HuffmanTableOffset	= 0;
				HuffmanTableLength	= 0;

				ExthFlags			= 0;

				IsDRM				= false;

				DRMOffset			= 0;
				DRMCount			= 0;
				DRMSize				= 0;
				DRMFlags			= 0;

				IsExtra				= false;

				FirstContentRecord	= 0;
				LastContentRecord	= 0;

				FCIS_Number			= 0;
				FCIS_Count			= 0;
				FLIS_Number			= 0;
				FLIS_Count			= 0;

				SrcsRecordIndex		= 0;
				NumSrcsRecords		= 0;

				ExtraDataFlags		= 0;

				IsMultibytes		= false;
				IsIndexingBytes		= false;
				IsUncrossableBreaks = false;

				PrimaryIndexRecord	= 0;
			}
			~CMobiHeader()
			{
			}

			void Read(CStream* pStream, CSection* pRecord, bool bIsMobi = true)
			{
				pStream->Seek(pRecord->m_oHeader.Offset);

				Compression		= pStream->ReadUShort();

				pStream->Skip(2);

				TextLength		= pStream->ReadDWORD();
				RecordCount		= pStream->ReadUShort();
				RecordSize		= pStream->ReadUShort();
				CurrentPosition = 0;//pStream->ReadDWORD();

				if (!bIsMobi)
					return;

				EncryptionType	= pStream->ReadUShort();

				pStream->Skip(2);

				Type			= pStream->ReadStringA(4);
				HeaderLength	= pStream->ReadDWORD();
				MobiType		= pStream->ReadDWORD();
				TextEncoding	= pStream->ReadDWORD();

				UID				= pStream->ReadDWORD();
				Version			= pStream->ReadDWORD();
				
				OrtographicIndex	= pStream->ReadDWORD();
				InflectionIndex		= pStream->ReadDWORD();
				IndexNames			= pStream->ReadDWORD();
				IndexKeys			= pStream->ReadDWORD();

				ExtraIndex0			= pStream->ReadDWORD();
				ExtraIndex1			= pStream->ReadDWORD();
				ExtraIndex2			= pStream->ReadDWORD();
				ExtraIndex3			= pStream->ReadDWORD();
				ExtraIndex4			= pStream->ReadDWORD();
				ExtraIndex5			= pStream->ReadDWORD();

				FirstNonBookIndex	= pStream->ReadDWORD();
				FullNameOffset		= pStream->ReadDWORD();
				FullNameLength		= pStream->ReadDWORD();
				Locale				= pStream->ReadDWORD();
				InputLanguage		= pStream->ReadDWORD();
				OutputLanguage		= pStream->ReadDWORD();
				MinVersion			= pStream->ReadDWORD();
				FirstImageIndex		= pStream->ReadDWORD();

				HuffmanRecordOffset = pStream->ReadDWORD();
				HuffmanRecordCount	= pStream->ReadDWORD();
				HuffmanTableOffset	= pStream->ReadDWORD();
				HuffmanTableLength	= pStream->ReadDWORD();

				ExthFlags			= pStream->ReadDWORD();

				IsDRM				= (HeaderLength > 174) ? true : false;

				if (IsDRM)
				{
					pStream->Skip(32);
					DRMOffset			= pStream->ReadDWORD();
					DRMCount			= pStream->ReadDWORD();
					DRMSize				= pStream->ReadDWORD();
					DRMFlags			= pStream->ReadDWORD();
				}

				IsExtra				= (HeaderLength >= 232) ? true : false;

				if (IsExtra)
				{
					pStream->Skip(12);
					FirstContentRecord	= pStream->ReadUShort();
					LastContentRecord	= pStream->ReadUShort();

					pStream->Skip(4);

					FCIS_Number			= pStream->ReadDWORD();
					FCIS_Count			= pStream->ReadDWORD();
					FLIS_Number			= pStream->ReadDWORD();
					FLIS_Count			= pStream->ReadDWORD();

					pStream->Skip(8);

					SrcsRecordIndex		= pStream->ReadDWORD();
					NumSrcsRecords		= pStream->ReadDWORD();

					pStream->Skip(8);

					ExtraDataFlags		= pStream->ReadDWORD();

					IsMultibytes		= ((ExtraDataFlags & 0xb1)		!= 0x00);
					IsIndexingBytes		= ((ExtraDataFlags & 0x10)		!= 0x00);
					IsUncrossableBreaks = ((ExtraDataFlags & 0xb100)	!= 0x00);

					PrimaryIndexRecord	= pStream->ReadDWORD();
				}

				if ((ExthFlags & 0x40) != 0x00)
				{
					pStream->Seek(pRecord->m_oHeader.Offset + 16 + HeaderLength);
					m_oEXTH.Read(pStream);
				}
			}

		};


	public:
		NSEBook::CTempManager m_oTempManager;
		bool m_bIsCorrectHtml;
		CString m_strMeta;

	public:
		CMobiReader()
		{
			m_oTempManager.m_strTempDirectory		= _T("C:\\PPTMemory\\MOBI");
			m_oTempManager.m_strTempImagesDirectory	= m_oTempManager.m_strTempDirectory + _T("\\media");

			m_bIsCorrectHtml = false;
		}

		bool OpenFile(CString strFileName, bool& bIsDRM)
		{
			m_oTempManager.InitTempDirectory();

			CFile oFile;
			oFile.OpenFile(strFileName);

			CStream oStream;
			oStream.FromFile(&oFile);

			CPDBHeader oPDBHeader;
			oPDBHeader.ReadFromStream(&oStream);

			int nCountRecords = (int)oPDBHeader.NumberOfRecords;
			CSection* pSections = new CSection[nCountRecords];
			
			for (int i = 0; i < nCountRecords; ++i)
				pSections[i].m_oHeader.Read(&oStream);

			for (int i = 0; i < nCountRecords; ++i)
			{
				if (i == (nCountRecords - 1))
					pSections[i].Length = oStream.GetSize() - pSections[i].m_oHeader.Offset;
				else
					pSections[i].Length = pSections[i + 1].m_oHeader.Offset - pSections[i].m_oHeader.Offset;
			}

			m_strMeta = _T("<meta>");

			bool bIsMobi = (_T("BOOKMOBI") == oPDBHeader.Type) ? TRUE : FALSE;

			CMobiHeader oHeader;
			oHeader.Read(&oStream, &pSections[0], _T("BOOKMOBI") == oPDBHeader.Type);

			/*
			if (oHeader.IsDRM)
			{
				// нельзя так делать. везде почему-то стоит такой флаг
				bIsDRM = true;
				oFile.CloseFile();

				RELEASEARRAYOBJECTS(pSections);
				return false;
			}
			*/

			CAtlArray<NSMobi::CMobiReader::CEXTHRecord>* pArrayRecs = &oHeader.m_oEXTH.m_arRecords;
			size_t nCountRecsCount = pArrayRecs->GetCount();
			for (size_t i = 0; i < nCountRecsCount; ++i)
			{
				if (CEXTHRecord::exthAuthor == pArrayRecs->GetAt(i).m_eType)
				{
					m_strMeta += (_T("<author>") + CorrectString2(pArrayRecs->GetAt(i).m_strValue) + _T("</author>"));
				}
				else if (CEXTHRecord::exthDescription == pArrayRecs->GetAt(i).m_eType)
				{
					m_strMeta += (_T("<annotation>") + CorrectString2(pArrayRecs->GetAt(i).m_strValue) + _T("</annotation>"));
				}
				else if (CEXTHRecord::exthPublisher == pArrayRecs->GetAt(i).m_eType)
				{
					m_strMeta += (_T("<publisher>") + CorrectString2(pArrayRecs->GetAt(i).m_strValue) + _T("</publisher>"));
				}
			}

			LONG lPos2 = pSections[1].m_oHeader.Offset;
			LONG lPos1 = oStream.GetPosition();
			if (lPos2 > lPos1)
			{
				m_strMeta += (_T("<name>") + CorrectString2(oStream.ReadStringUTF8(lPos2 - lPos1)) + _T("</name>"));
			}

			int nCoverOffset = oHeader.m_oEXTH.GetCoverOffset();
			if (-1 != nCoverOffset)
				m_strMeta += _T("<coverpage>1</coverpage>");

			m_strMeta += _T("</meta>");

			int nNumberOfTextRecords	= oHeader.RecordCount;
			int nFirstNonBook			= oHeader.FirstNonBookIndex;
			int nFirstImage				= oHeader.FirstImageIndex;

			if (!bIsMobi)
			{
				nFirstImage				= nCountRecords;
			}

			CHuffmanReader oHuffmanDecoder;
			if ((1 != oHeader.Compression) && (2 != oHeader.Compression))
			{
				int nHuffOffset = (int)oHeader.HuffmanRecordOffset;
				int nCountHuff	= (int)oHeader.HuffmanRecordCount;

				oHuffmanDecoder.LoadTables(&oStream, pSections + nHuffOffset, nCountHuff);
			}

			int nStart = 1;
			int nEnd = min(nCountRecords, nNumberOfTextRecords + 1);

			CBufferedText oBuffText;
			for (int nIndexTextRec = nStart; nIndexTextRec < nEnd; ++nIndexTextRec)
			{
				oStream.Seek((int)pSections[nIndexTextRec].m_oHeader.Offset);
				
				if (2 == oHeader.Compression)
				{
					BYTE* pUncompress = NULL;
					LONG lUncompressLen = 0;

					int _nStart	= 0;
					int _nEnd	= (int)pSections[nIndexTextRec].Length - 1;
					CTextDecoder::GetTralingData(oStream.GetCurrentData(), oHeader.ExtraDataFlags, _nStart, _nEnd);
					CTextDecoder::PalmDecode(oStream.GetCurrentData() + _nStart, _nEnd - _nStart + 1, pUncompress, lUncompressLen);

					oBuffText.WriteData(pUncompress, min(4096, lUncompressLen));
					//CString strMem = CFile::GetCStringFromUTF8(pUncompress, min(4096, lUncompressLen));
					RELEASEARRAYOBJECTS(pUncompress);
				}
				else if (1 == oHeader.Compression)
				{
					oBuffText.WriteData(oStream.GetCurrentData(), pSections[nIndexTextRec].Length);
				}
				else
				{
					BYTE* pUncompress = NULL;
					LONG lUncompressLen = 0;

					int _nStart	= 0;
					int _nEnd	= (int)pSections[nIndexTextRec].Length - 1;
					CTextDecoder::GetTralingData(oStream.GetCurrentData(), oHeader.ExtraDataFlags, _nStart, _nEnd);
					
					oHuffmanDecoder.Decode(oStream.GetCurrentData() + _nStart, _nEnd - _nStart + 1, pUncompress, lUncompressLen);
					
					oBuffText.WriteData(pUncompress, min(4096, lUncompressLen));
					//CString strMem = CFile::GetCStringFromUTF8(pUncompress, min(4096, lUncompressLen));
					RELEASEARRAYOBJECTS(pUncompress);
				}
			}

			CString strAllText = _T("");
			
			if (!bIsMobi)
			{
				strAllText = CString((char*)oBuffText.GetData(), (int)oBuffText.GetCurSize());
			}
			else
			{
				strAllText = CFile::GetCStringFromUTF8(oBuffText.GetData(), (LONG)oBuffText.GetCurSize());
			}

			if (_T("") == strAllText)
			{
				// вот такая мега проверка на дрм
				bIsDRM = true;
				oFile.CloseFile();

				RELEASEARRAYOBJECTS(pSections);
				return false;
			}

			CFile oFileHtml;
			oFileHtml.CreateFile(m_oTempManager.m_strTempDirectory + _T("\\index.html"));
			
			int nIndexSavedImage = 1;
			for (int nIndexImageRecord = nFirstImage; nIndexImageRecord < nCountRecords; ++nIndexImageRecord)
			{
				oStream.Seek((int)pSections[nIndexImageRecord].m_oHeader.Offset);
				CString strType = oStream.ReadStringA(4);

				if ((_T("FLIS") == strType) || (_T("FCIS") == strType) || (_T("SRCS") == strType))
				{
					continue;
				}

				oStream.Skip(-4);

				CFile oFileImage;
				CString strNumImage = _T("");
				strNumImage.Format(_T("%d.jpg"), nIndexSavedImage++);
				oFileImage.CreateFile(m_oTempManager.m_strTempImagesDirectory + _T("\\image") + strNumImage);
				oFileImage.WriteFile((void*)oStream.GetCurrentData(), pSections[nIndexImageRecord].Length);
				oFileImage.CloseFile();
			}

			if (m_bIsCorrectHtml && bIsMobi)
			{
				int nLen = min(200, strAllText.GetLength());
				CString strStart = strAllText.Mid(0, nLen);
				strStart.MakeLower();
				if (-1 == strStart.Find(_T("utf-8")))
				{
					strAllText.Replace(_T("<head>"), _T("<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>"));
				}

				if (-1 != nCoverOffset)
				{
					int nBody1 = strAllText.Find(_T("<body"));
					int nBody2 = strAllText.Find(_T("<BODY"));
					int nBody = max(nBody1, nBody2);

					if (-1 != nBody)
					{
						int n__ = strAllText.Find(TCHAR('>'), nBody);
						++n__;
						if (0 != n__)
						{
							CString str = _T("");
							str.Format(_T("%d"), nCoverOffset + 1);
							strAllText.Insert(n__, _T("<div style=\"text-align:center;\"><img style=\"center;width:100%;height:100%;\" src=\"media/image") + str + _T(".jpg\"/></div>"));
						}
					}
				}

				// теперь нужно прописать пути к картинкам, убрав ссылки recindex!
				for (int indexImage = 1; indexImage < nIndexSavedImage; ++indexImage)
				{
					CString strNum = _T("");
					strNum.Format(_T("%d"), indexImage);
					int nLenNum = strNum.GetLength();

					for (int i = nLenNum; i <= 5; ++i)
					{
						CString strOld = _T("recindex=\"") + strNum + _T("\"");
						CString strNew = _T("");
						strNew.Format(_T("src=\"media/image%d.jpg\""), indexImage);

						strAllText.Replace(strOld, strNew);
						strNum = _T("0") + strNum;
					}
				}

				strAllText.Replace(_T("<mbp:pagebreak/>"), _T("<p style=\"page-break-after:always\"/>"));
				strAllText.Replace(_T("<mbp:pagebreak>"), _T("<p style=\"page-break-after:always\"/>"));
			}

			if (!bIsMobi)
			{
				BYTE utf8_mem[3];
				utf8_mem[0]	= 0xEF;
				utf8_mem[1]	= 0xBB;
				utf8_mem[2]	= 0xBF;
				
				oFileHtml.WriteFile((void*)utf8_mem, 3);
			}

			oFileHtml.WriteStringUTF8(strAllText);
			oFileHtml.CloseFile();

			oFile.CloseFile();

			RELEASEARRAYOBJECTS(pSections);

			return bIsMobi;
		}
		void CloseFile()
		{
			m_oTempManager.RemoveTempDirectory();
		}
	};
}