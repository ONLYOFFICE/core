/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "Crypt.h"
#include "../Logic/Biff_structures/RC4EncryptionHeader.h"
#include "BiffDecoder_RCF.h"

namespace CRYPT
{

class XORCrypt : public Crypt
{
public:
	XORCrypt(int type, unsigned short key, unsigned short hash, std::wstring password);

	virtual void Init(const unsigned long val);

	virtual void Decrypt(char* data, const size_t size, const unsigned long stream_pos, const size_t block_size);
	virtual void Decrypt(char* data, const size_t size, const unsigned long block_index);

	virtual bool IsVerify();

private:
	void Skip( size_t size );

	unsigned short m_nKey;
	unsigned short m_nHash;

	std::string m_sPassword;
	bool m_VerifyPassword;

    unsigned char	m_pnKey[ 16 ];   // Encryption key.
    size_t			m_nOffset;       // Key offset.
    int				m_nRotateDistance;
};


} // namespace CRYPT
