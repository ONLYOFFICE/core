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
#include "Object.h"
#include "Stream.h"
#include "JArithmeticDecoder.h"

namespace PdfReader
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// JArithmeticDecoderStates
	//-------------------------------------------------------------------------------------------------------------------------------

	JArithmeticDecoderStats::JArithmeticDecoderStats(int nContextSize)
	{
		m_nContextSize = nContextSize;
		m_pContextTable = (unsigned char *)MemUtilsMallocArray(m_nContextSize, sizeof(unsigned char));
		Reset();
	}

	JArithmeticDecoderStats::~JArithmeticDecoderStats()
	{
		MemUtilsFree(m_pContextTable);
	}

	JArithmeticDecoderStats *JArithmeticDecoderStats::Copy()
	{
		JArithmeticDecoderStats *pStats = new JArithmeticDecoderStats(m_nContextSize);
		memcpy(pStats->m_pContextTable, m_pContextTable, m_nContextSize);
		return pStats;
	}

	void JArithmeticDecoderStats::Reset()
	{
		memset(m_pContextTable, 0, m_nContextSize);
	}

	void JArithmeticDecoderStats::CopyFrom(JArithmeticDecoderStats *pStats)
	{
		memcpy(m_pContextTable, pStats->m_pContextTable, m_nContextSize);
	}

	void JArithmeticDecoderStats::SetEntry(unsigned int unCx, int nIndex, int nMPS)
	{
		m_pContextTable[unCx] = (nIndex << 1) + nMPS;
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// JArithmeticDecoder
	//-------------------------------------------------------------------------------------------------------------------------------

	unsigned int JArithmeticDecoder::arrunQeTable[47] =
	{
		0x56010000, 0x34010000, 0x18010000, 0x0AC10000,
		0x05210000, 0x02210000, 0x56010000, 0x54010000,
		0x48010000, 0x38010000, 0x30010000, 0x24010000,
		0x1C010000, 0x16010000, 0x56010000, 0x54010000,
		0x51010000, 0x48010000, 0x38010000, 0x34010000,
		0x30010000, 0x28010000, 0x24010000, 0x22010000,
		0x1C010000, 0x18010000, 0x16010000, 0x14010000,
		0x12010000, 0x11010000, 0x0AC10000, 0x09C10000,
		0x08A10000, 0x05210000, 0x04410000, 0x02A10000,
		0x02210000, 0x01410000, 0x01110000, 0x00850000,
		0x00490000, 0x00250000, 0x00150000, 0x00090000,
		0x00050000, 0x00010000, 0x56010000
	};

	int JArithmeticDecoder::arrnNMPSTable[47] =
	{
		1, 2, 3, 4, 5, 38, 7, 8, 9, 10, 11, 12, 13, 29, 15, 16,
		17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 45, 46
	};

	int JArithmeticDecoder::arrnNLPSTable[47] =
	{
		1, 6, 9, 12, 29, 33, 6, 14, 14, 14, 17, 18, 20, 21, 14, 14,
		15, 16, 17, 18, 19, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 46
	};

	int JArithmeticDecoder::arrnSwitchTable[47] =
	{
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	JArithmeticDecoder::JArithmeticDecoder()
	{
		m_pStream      = NULL;
		m_nDataSize    = 0;
		m_bLimitStream = false;
	}

	inline unsigned int JArithmeticDecoder::ReadByte()
	{
		if (m_bLimitStream)
		{
			--m_nDataSize;
			if (m_nDataSize < 0)
			{
				return 0xff;
			}
		}
		return (unsigned int)m_pStream->GetChar() & 0xff;
	}

	JArithmeticDecoder::~JArithmeticDecoder()
	{
		Cleanup();
	}

	void JArithmeticDecoder::Start()
	{
		m_unBuffer0 = ReadByte();
		m_unBuffer1 = ReadByte();

		// INITDEC
		m_unC = (m_unBuffer0 ^ 0xff) << 16;
		ByteIn();
		m_unC <<= 7;
		m_nCT -= 7;
		m_unA = 0x80000000;
	}

	void JArithmeticDecoder::Restart(int nDatasize)
	{
		int nOldDataSize = m_nDataSize;
		m_nDataSize = nDatasize;
		if (nOldDataSize == -1)
		{
			m_unBuffer1 = ReadByte();
		}
		else if (nOldDataSize <= -2)
		{
			m_unBuffer0 = ReadByte();
			m_unBuffer1 = ReadByte();
		}
	}

	void JArithmeticDecoder::Cleanup()
	{
		if (m_bLimitStream)
		{
			while (m_nDataSize > 0)
			{
				m_unBuffer0 = m_unBuffer1;
				m_unBuffer1 = ReadByte();
			}
		}
	}

	int JArithmeticDecoder::DecodeBit(unsigned int unContext, JArithmeticDecoderStats *pStats)
	{
		int nBit = 0;

		int nICX   = pStats->m_pContextTable[unContext] >> 1;
		int nMPSCX = pStats->m_pContextTable[unContext] & 1;
		unsigned int unQe = arrunQeTable[nICX];
		m_unA -= unQe;
		if (m_unC < m_unA)
		{
			if (m_unA & 0x80000000)
			{
				nBit = nMPSCX;
			}
			else
			{
				// MPS_EXCHANGE
				if (m_unA < unQe)
				{
					nBit = 1 - nMPSCX;
					if (arrnSwitchTable[nICX])
					{
						pStats->m_pContextTable[unContext] = (arrnNLPSTable[nICX] << 1) | (1 - nMPSCX);
					}
					else
					{
						pStats->m_pContextTable[unContext] = (arrnNLPSTable[nICX] << 1) | nMPSCX;
					}
				}
				else
				{
					nBit = nMPSCX;
					pStats->m_pContextTable[unContext] = (arrnNMPSTable[nICX] << 1) | nMPSCX;
				}
				// RENORMD
				do
				{
					if (m_nCT == 0)
					{
						ByteIn();
					}
					m_unA <<= 1;
					m_unC <<= 1;
					--m_nCT;
				} while (!(m_unA & 0x80000000));
			}
		}
		else
		{
			m_unC -= m_unA;
			// LPS_EXCHANGE
			if (m_unA < unQe)
			{
				nBit = nMPSCX;
				pStats->m_pContextTable[unContext] = (arrnNMPSTable[nICX] << 1) | nMPSCX;
			}
			else
			{
				nBit = 1 - nMPSCX;
				if (arrnSwitchTable[nICX])
				{
					pStats->m_pContextTable[unContext] = (arrnNLPSTable[nICX] << 1) | (1 - nMPSCX);
				}
				else
				{
					pStats->m_pContextTable[unContext] = (arrnNLPSTable[nICX] << 1) | nMPSCX;
				}
			}
			m_unA = unQe;
			// RENORMD
			do
			{
				if (m_nCT == 0)
				{
					ByteIn();
				}
				m_unA <<= 1;
				m_unC <<= 1;
				--m_nCT;
			} while (!(m_unA & 0x80000000));
		}
		return nBit;
	}

	int JArithmeticDecoder::DecodeByte(unsigned int unContext, JArithmeticDecoderStats *pStats)
	{
		int nByte = 0;
		for (int nIndex = 0; nIndex < 8; ++nIndex)
		{
			nByte = (nByte << 1) | DecodeBit(unContext, pStats);
		}
		return nByte;
	}

	bool JArithmeticDecoder::DecodeInt(int *pnValue, JArithmeticDecoderStats *pStats)
	{
		unsigned int unVal = 0;
		m_unPrev = 1;
		int nSign = DecodeIntBit(pStats);
		if (DecodeIntBit(pStats))
		{
			if (DecodeIntBit(pStats))
			{
				if (DecodeIntBit(pStats))
				{
					if (DecodeIntBit(pStats))
					{
						if (DecodeIntBit(pStats))
						{
							unVal = 0;
							for (int nIndex = 0; nIndex < 32; ++nIndex)
							{
								unVal = (unVal << 1) | DecodeIntBit(pStats);
							}
							unVal += 4436;
						}
						else
						{
							unVal = 0;
							for (int nIndex = 0; nIndex < 12; ++nIndex)
							{
								unVal = (unVal << 1) | DecodeIntBit(pStats);
							}
							unVal += 340;
						}
					}
					else
					{
						unVal = 0;
						for (int nIndex = 0; nIndex < 8; ++nIndex)
						{
							unVal = (unVal << 1) | DecodeIntBit(pStats);
						}
						unVal += 84;
					}
				}
				else
				{
					unVal = 0;
					for (int nIndex = 0; nIndex < 6; ++nIndex)
					{
						unVal = (unVal << 1) | DecodeIntBit(pStats);
					}
					unVal += 20;
				}
			}
			else
			{
				unVal = DecodeIntBit(pStats);
				unVal = (unVal << 1) | DecodeIntBit(pStats);
				unVal = (unVal << 1) | DecodeIntBit(pStats);
				unVal = (unVal << 1) | DecodeIntBit(pStats);
				unVal += 4;
			}
		}
		else
		{
			unVal = DecodeIntBit(pStats);
			unVal = (unVal << 1) | DecodeIntBit(pStats);
		}

		if (nSign)
		{
			if (unVal == 0)
			{
				return false;
			}
			*pnValue = -(int)unVal;
		}
		else
		{
			*pnValue = (int)unVal;
		}
		return true;
	}

	int JArithmeticDecoder::DecodeIntBit(JArithmeticDecoderStats *pStats)
	{
		int nBit = DecodeBit(m_unPrev, pStats);
		if (m_unPrev < 0x100)
		{
			m_unPrev = (m_unPrev << 1) | nBit;
		}
		else
		{
			m_unPrev = (((m_unPrev << 1) | nBit) & 0x1ff) | 0x100;
		}
		return nBit;
	}

	unsigned int JArithmeticDecoder::DecodeIAID(unsigned int unCodeLen, JArithmeticDecoderStats *pStats)
	{
		m_unPrev = 1;
		for (unsigned int unIndex = 0; unIndex < unCodeLen; ++unIndex)
		{
			int nBit = DecodeBit(m_unPrev, pStats);
			m_unPrev = (m_unPrev << 1) | nBit;
		}
		return m_unPrev - (1 << unCodeLen);
	}

	void JArithmeticDecoder::ByteIn()
	{
		if (m_unBuffer0 == 0xff)
		{
			if (m_unBuffer1 > 0x8f)
			{
				m_nCT = 8;
			}
			else
			{
				m_unBuffer0 = m_unBuffer1;
				m_unBuffer1 = ReadByte();
				m_unC = m_unC + 0xfe00 - (m_unBuffer0 << 9);
				m_nCT = 7;
			}
		}
		else
		{
			m_unBuffer0 = m_unBuffer1;
			m_unBuffer1 = ReadByte();
			m_unC = m_unC + 0xff00 - (m_unBuffer0 << 8);
			m_nCT = 8;
		}
	}
}