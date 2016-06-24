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
#pragma once

#include "BinaryCodec_RCF.h"
#include "rtl/cipher.h"
#include "rtl/digest.h"

#include <vector>

namespace CRYPT
{

const long BIFF_RCF_BLOCKSIZE          = 1024;

/** Base class for BIFF stream decoders. */
class BiffDecoderBase //: public ::comphelper::IDocPasswordVerifier
{
public:
    explicit BiffDecoderBase();
    virtual ~BiffDecoderBase();

    /** Implementation of the ::comphelper::IDocPasswordVerifier interface,
        calls the new virtual function implVerify(). */
    virtual bool verifyPassword( const std::wstring& rPassword );

    /** Returns true, if the decoder has been initialized correctly. */
    inline bool isValid() const { return mbValid; }

    /** Decodes nBytes unsigned chars and writes encrypted data into the buffer pnDestData. */
    void decode(unsigned char* pnDestData, const unsigned char* pnSrcData, const long nStreamPos, const unsigned short nBytes);

private:
    /** Derived classes implement password verification and initialization of
        the decoder. */
	virtual bool implVerify(const std::wstring& rPassword) = 0;

    /** Implementation of decryption of a memory block. */
    virtual void implDecode(unsigned char* pnDestData, const unsigned char* pnSrcData, const long nStreamPos, const unsigned short nBytes) = 0;

private:
    bool mbValid;        /// True = decoder is correctly initialized.
};

typedef ::boost::shared_ptr<BiffDecoderBase> BiffDecoderRef;


class BiffDecoder_RCF : public BiffDecoderBase
{
public:
	explicit BiffDecoder_RCF(unsigned char pnSalt[ 16 ], unsigned char pnVerifier[ 16 ], unsigned char pnVerifierHash[ 16 ]);

private:

	/** Implements password verification and initialization of the decoder. */
	virtual bool implVerify(const std::wstring& rPassword);

	/** Implementation of decryption of a memory block. */
	virtual void implDecode(unsigned char* pnDestData, const unsigned char* pnSrcData, const long nStreamPos, const unsigned short nBytes);

private:
	BinaryCodec_RCF maCodec;   /// Cipher algorithm implementation.
	std::vector<unsigned short> maPassword;
	std::vector<unsigned char> maSalt;
	std::vector<unsigned char> maVerifier;
	std::vector<unsigned char> maVerifierHash;
};


} // namespace CRYPT
