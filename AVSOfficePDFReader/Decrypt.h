#ifndef _DECRYPT_H
#define _DECRYPT_H

#include "StringExt.h"
#include "Object.h"
#include "Stream.h"

//-------------------------------------------------------------------------------------------------------------------------------
// Decrypt
//-------------------------------------------------------------------------------------------------------------------------------

class Decrypt 
{
public:

	// —троим FileKey. ѕараметр <sFileKey> должен иметь место как минимум под 16 байт. ѕровер€ем <seOwnerPassword> и <seUserPassword>
	// и возвращаем true, если какой-нибудь корректен. ѕишем <pbOwnerPasswordValide> = true, если <seOwnerPassword> был правильный.  
	// ќдин какой-нибудь или оба парол€ могут быть NULL, что мы будем понимать как пустую строку.
	static BOOL MakeFileKey(int nEncVersion, int nEncRevision, int nKeyLength, StringExt *seOwnerKey, StringExt *seUserKey, int nPermissions, StringExt *seFileID, StringExt *seOwnerPassword, StringExt *seUserPassword, unsigned char *sFileKey, BOOL bEncryptMetadata, BOOL *pbOwnerPasswordValid);

private:

	static BOOL MakeFileKey2(int nEncVersion, int nEncRevision, int nKeyLength, StringExt *seOwnerKey, StringExt *seUserKey, int nPermissions, StringExt *seFileID, StringExt *seUserPassword, unsigned char *sFileKey, BOOL bEncryptMetadata);
};

//-------------------------------------------------------------------------------------------------------------------------------
// DecryptStream
//-------------------------------------------------------------------------------------------------------------------------------

struct DecryptRC4State 
{
	unsigned char sState[256];
	unsigned char unX;
	unsigned char unY;
	int           nBuffer;
};

struct DecryptAESState 
{
	unsigned int  arrW[44];
	unsigned char sState[16];
	unsigned char sCBC[16];
	unsigned char sBuffer[16];
	int           nBufferIndex;
};

class DecryptStream: public FilterStream 
{

public:

	DecryptStream(Stream *pStream, unsigned char *sFileKey, CryptAlgorithm eType, int nKeyLength, int nObjectNum, int nObjectGen);
	virtual ~DecryptStream();
	virtual StreamType GetType() 
	{ 
		return strWeird; 
	}
	virtual void Reset();
	virtual int GetChar();
	virtual int LookChar();
	virtual BOOL IsBinary(BOOL bLast);
  virtual Stream *getUndecodedStream() { return this; }

private:

	CryptAlgorithm m_eCryptType;
	int            m_nObjectKeyLength;
	unsigned char  m_sObjectKey[16 + 9];

	union 
	{
		DecryptRC4State oRC4;
		DecryptAESState oAES;
	} m_oState;
};

#endif /* _DECRYPT_H */
