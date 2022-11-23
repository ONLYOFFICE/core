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

#include "Decryptor.h"
#include "rtl/cipher.h"
#include "rtl/digest.h"

#include <string.h>

namespace CRYPT
{

/** Encodes and decodes data from protected MSO 97+ documents.

This is a wrapper class around low level cryptographic functions from RTL.
Implementation is based on the wvDecrypt package by Caolan McNamara:
http://www.csn.ul.ie/~caolan/docs/wvDecrypt.html
*/
class BinaryCodec_RCF
{
public:
	/** Default constructor.

	Two-step construction in conjunction with the initKey() and verifyKey()
	functions allows to try to initialize with different passwords (e.g.
	built-in default password used for Excel workbook protection).
	*/
	explicit BinaryCodec_RCF();

	~BinaryCodec_RCF();

	/** Initializes the algorithm with the specified password and document ID.

	@param pnPassData
	Unicode character array containing the password. Must be zero
	terminated, which results in a maximum length of 15 characters.
	@param pnSalt
	Random salt data block read from or written to the file.
	*/
	void initKey(const unsigned short pnPassData[ 16 ], const unsigned char pnSalt[ 16 ] );

	/** Verifies the validity of the password using the passed salt data.

	@precond
	The codec must be initialized with the initKey() function before
	this function can be used.

	@param pnVerifier
	Verifier block read from the file.
	@param pnVerifierHash
	Verifier hash read from the file.

	@return
	True = test was successful.
	*/
	bool verifyKey(const unsigned char pnVerifier[ 16 ], const unsigned char pnVerifierHash[ 16 ] );

	/** Rekeys the codec using the specified counter.

	After reading a specific amount of data the cipher algorithm needs to
	be rekeyed using a counter that counts the data blocks.

	The block size is for example 512 unsigned chars for MS Word files and 1024
	unsigned chars for MS Excel files.

	@precond
	The codec must be initialized with the initKey() function before
	this function can be used.

	@param nCounter
	Block counter used to rekey the cipher.
	*/
	bool startBlock( size_t nCounter );

	/** Decodes a block of memory.

	@see rtl_cipher_decode()

	@precond
	The codec must be initialized with the initKey() function before
	this function can be used.

	@param pnDestData
	Destination buffer. Will contain the decrypted data afterwards.
	@param pnSrcData
	Encrypted data block.
	@param nBytes
	Size of the passed data blocks. pnDestData and pnSrcData must be of
	this size.

	@return
	True = decoding was successful (no error occured).
	*/
	bool decode(unsigned char* pnDestData, const unsigned char* pnSrcData, size_t nBytes );

	/** Lets the cipher skip a specific amount of unsigned chars.

	This function sets the cipher to the same state as if the specified
	amount of data has been decoded with one or more calls of decode().

	@precond
	The codec must be initialized with the initKey() function before
	this function can be used.

	@param nBytes
	Number of unsigned chars to be skipped (cipher "seeks" forward).

	@return
	True = skip was successful (no error occured).
	*/
	bool skip( size_t nBytes );

private:
	CipherARCFOUR		mhCipher;
	DigestMD5           mhDigest;
	unsigned char		mpnDigestValue[DigestMD5::RTL_DIGEST_LENGTH_MD5];
};


} // namespace CRYPT
