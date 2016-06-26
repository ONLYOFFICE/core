/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "EncryptionHeader.h"

#define GETBIT(from, num) ((from & (1 << num)) != 0)

namespace DocFileFormat
{
	EncryptionHeader::EncryptionHeader( FileInformationBlock* fib, POLE::Stream* tableStream )
	{
		VirtualStreamReader tStream( tableStream, 0, fib->m_bOlderVersion);

		if (fib->m_FibBase.fObfuscation)
		{
			//xor
			return;
		}
		unsigned short vMajor = tStream.ReadUInt16();
		unsigned short vMinor = tStream.ReadUInt16();

		if(0x0001 == vMajor) // RC4 encryption header structure
		{// fib->m_FibBase.lKey == 52 
			crypt_data.Salt.b1  = tStream.ReadUInt32();
			crypt_data.Salt.b2  = tStream.ReadUInt32();
			crypt_data.Salt.b3  = tStream.ReadUInt32();
			crypt_data.Salt.b4  = tStream.ReadUInt32();
			
			crypt_data.EncryptedVerifier.b1  = tStream.ReadUInt32();
			crypt_data.EncryptedVerifier.b2  = tStream.ReadUInt32();
			crypt_data.EncryptedVerifier.b3  = tStream.ReadUInt32();
			crypt_data.EncryptedVerifier.b4  = tStream.ReadUInt32();
			
			crypt_data.EncryptedVerifierHash.b1  = tStream.ReadUInt32();
			crypt_data.EncryptedVerifierHash.b2  = tStream.ReadUInt32();
			crypt_data.EncryptedVerifierHash.b3  = tStream.ReadUInt32();
			crypt_data.EncryptedVerifierHash.b4  = tStream.ReadUInt32();
		}
		else
		{
		//	unsigned short flags = tStream.ReadUInt16();

		//	bool fCryptoAPI	= GETBIT(flags, 2);
		//	bool fDocProps	= GETBIT(flags, 3);
		//	bool fExternal	= GETBIT(flags, 4);
		//	bool fAES		= GETBIT(flags, 5);

		//	_UINT32 EncryptionHeaderSize = tStream.ReadUInt32();
		////header
		//	
		//	tStream.ReadUInt32(); // reserved (SizeExtra)
		//	_INT32	AlgID			= tStream.ReadInt32();
		//	_INT32	AlgIDHash		= tStream.ReadInt32();
		//	_UINT32	KeySize			= tStream.ReadUInt32();
		//	_UINT32	ProviderType	= tStream.ReadUInt32();

		//	tStream.ReadUInt32();
		//	tStream.ReadUInt32(); // reserved
		//	
		//	//std::wstring	CSPName;
		//	//record >> CSPName;

		//// + crypt_data
		}

		//RELEASEARRAYOBJECTS( bytes );
	}

}
