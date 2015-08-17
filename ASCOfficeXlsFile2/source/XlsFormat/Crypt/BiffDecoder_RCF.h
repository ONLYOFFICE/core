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
