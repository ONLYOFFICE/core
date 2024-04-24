/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
