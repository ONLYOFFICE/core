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

#ifndef NSHASH_H
#define NSHASH_H

#include <functional>
#include <stddef.h>
#include "./common.h"

// hide algs into lib
namespace NSHash
{
	void BUILDER_DECL hash_internal(const unsigned char* src, unsigned char* dst, size_t size, int alg);
}

class BUILDER_DECL CHash
{	
public:
	enum HashAlgs
	{
		haMD2		= 0,
		haMD4		= 1,
		haMD5		= 2,
		haRMD160	= 3,
		haSHA1		= 4,
		haSHA256	= 5,
		haSHA384	= 6,
		haSHA512	= 7,
		haWHIRLPOOL	= 8
	};

	static int getDigestLength(HashAlgs alg);

public:
	CHash(std::function<void*(size_t)> allocator);
	~CHash();

	unsigned char* hash(const unsigned char* data, int size, int alg);
	unsigned char* hash2(const char* password, const char* salt, int spinCount, int alg);

private:
	std::function<void*(size_t)> m_fAllocator;
};

#endif // NSHASH_H
