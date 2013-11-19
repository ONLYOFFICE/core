#pragma once

#include "BinaryCodec_RCF.h"
#include "rtl/cipher.h"
#include "rtl/digest.h"

namespace CRYPT
{;

const LONGLONG BIFF_RCF_BLOCKSIZE          = 1024;

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

    /** Decodes nBytes bytes and writes encrypted data into the buffer pnDestData. */
    void decode(BYTE* pnDestData, const BYTE* pnSrcData, const LONGLONG nStreamPos, const WORD nBytes);

private:
    /** Derived classes implement password verification and initialization of
        the decoder. */
	virtual bool implVerify(const std::wstring& rPassword) = 0;

    /** Implementation of decryption of a memory block. */
    virtual void implDecode(BYTE* pnDestData, const BYTE* pnSrcData, const LONGLONG nStreamPos, const WORD nBytes) = 0;

private:
    bool mbValid;        /// True = decoder is correctly initialized.
};

typedef ::boost::shared_ptr<BiffDecoderBase> BiffDecoderRef;


class BiffDecoder_RCF : public BiffDecoderBase
{
public:
	explicit BiffDecoder_RCF(BYTE pnSalt[ 16 ], BYTE pnVerifier[ 16 ], BYTE pnVerifierHash[ 16 ]);

private:

	/** Implements password verification and initialization of the decoder. */
	virtual bool implVerify(const std::wstring& rPassword);

	/** Implementation of decryption of a memory block. */
	virtual void implDecode(BYTE* pnDestData, const BYTE* pnSrcData, const LONGLONG nStreamPos, const WORD nBytes);

private:
	BinaryCodec_RCF maCodec;   /// Cipher algorithm implementation.
	std::vector<WORD> maPassword;
	std::vector<BYTE> maSalt;
	std::vector<BYTE> maVerifier;
	std::vector<BYTE> maVerifierHash;
};


} // namespace CRYPT
