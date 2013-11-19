#ifndef _SECURITY_HANDLER_H
#define _SECURITY_HANDLER_H

#include "Object.h"

class StringExt;
class PDFDoc;

//-------------------------------------------------------------------------------------------------------------------------------
// SecurityHandler
//-------------------------------------------------------------------------------------------------------------------------------

class SecurityHandler 
{
public:

	static SecurityHandler *Make(PDFDoc *pDocument, Object *pEncryptDict);

	SecurityHandler(PDFDoc *pDocument);
	virtual ~SecurityHandler();

	// Проверяем шифрованный ли документ. Если документ шифрованный, тогда сначала продуем использовать <seOwnerPassword> и 
	// <seUserPassword>. Если оба пароля не подходят, тогда запрашиваем пароль у пользователя (не более 3-х раз). 
	// Возвращаемые значения: 
	// True, если документ может быть открыт( либо он вообще не зашифрован, либо пароль верный)
	// False, в противном случае ( документ зашрифован, а пароль неверный).
	BOOL CheckEncryption(StringExt *seOwnerPassword, StringExt *seUserPassword);

	// Для "Batch mode", т.е. когда пароли заданы и проверяются без пользователя. 
	virtual void *MakeAuthData(StringExt *ownerPassword, StringExt *userPassword) = 0;

	// Для запроса пароля от пользователя.
	virtual void *GetAuthData() = 0;

	// Освобождаем память выделенную функциями MakeAuthData или GetAuthData.
	virtual void FreeAuthData(void *pAuthData) = 0;

	// Пытаемся авторизоваться.
	virtual BOOL Authorize(void *pAuthData) = 0;

	// Считываем различные параметры.
	virtual int            GetPermissionFlags()   = 0;
	virtual BOOL           GetOwnerPasswordValid()= 0;
	virtual unsigned char *GetFileKey()           = 0;
	virtual int            GetFileKeyLength()     = 0;
	virtual int            GetEncodingVersion()   = 0;
	virtual CryptAlgorithm GetEncodingAlgorithm() = 0;

protected:

	PDFDoc *m_pDocument;
};

//-------------------------------------------------------------------------------------------------------------------------------
// StandardSecurityHandler
//-------------------------------------------------------------------------------------------------------------------------------

class StandardSecurityHandler: public SecurityHandler 
{
public:

	StandardSecurityHandler(PDFDoc *pDocument, Object *pEncryptDict);
	virtual ~StandardSecurityHandler();

	virtual void *MakeAuthData(StringExt *seOwnerPassword, StringExt *seUserPassword);
	virtual void *GetAuthData();
	virtual void FreeAuthData(void *pAuthData);
	virtual BOOL Authorize(void *pAuthData);
	virtual int GetPermissionFlags() 
	{ 
		return m_nPermissionFlags; 
	}
	virtual BOOL GetOwnerPasswordValid() 
	{ 
		return m_bOwnerPasswordValid; 
	}
	virtual unsigned char *GetFileKey() 
	{
		return m_sFileKey; 
	}
	virtual int GetFileKeyLength() 
	{ 
		return m_nFileKeyLength; 
	}
	virtual int GetEncodingVersion() 
	{ 
		return m_nEncryptVersion; 
	}
	virtual CryptAlgorithm GetEncodingAlgorithm() 
	{ 
		return m_eCryptType; 
	}

private:

	int            m_nPermissionFlags;
	BOOL           m_bOwnerPasswordValid;
	unsigned char  m_sFileKey[16];
	int            m_nFileKeyLength;
	int            m_nEncryptVersion;
	int            m_nEncryptRevision;
	CryptAlgorithm m_eCryptType;
	BOOL           m_bEncryptMetadata;

	StringExt     *m_seOwnerKey;
	StringExt     *m_seUserKey;
	StringExt     *m_seFileID;

	BOOL          m_bValid;
};

#endif /* _SECURITY_HANDLER_H */
