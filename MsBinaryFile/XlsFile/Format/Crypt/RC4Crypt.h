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
#pragma once

#include "Crypt.h"
#include "../Logic/Biff_structures/RC4EncryptionHeader.h"
#include "BiffDecoder_RCF.h"

namespace CRYPT
{

class RC4Crypt : public Crypt
{
public:
	RC4Crypt(CRYPT::_rc4CryptData & data, std::wstring password);

	virtual void Init(const unsigned long val){}

	virtual void Decrypt(char* data, const size_t size, const unsigned long stream_pos, const size_t block_size);
	virtual void Decrypt(char* data, const size_t size, const unsigned long block_index);

	virtual bool IsVerify();

private:
	void CopyDWORDs2Bytes(const unsigned int b1, const unsigned int b2, const unsigned int b3, const unsigned int b4, unsigned char* byte_array);

	unsigned char pnSalt[16];
	unsigned char pnVerifier[16];
	unsigned char pnVerifierHash[16];

	BiffDecoderRef mxDecoder;
	
	bool m_VerifyPassword;
};


} // namespace CRYPT
