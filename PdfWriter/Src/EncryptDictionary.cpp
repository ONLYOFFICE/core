#include "EncryptDictionary.h"
#include "Encrypt.h"
#include "Info.h"

#include <ctime>

namespace PdfWriter
{
	//----------------------------------------------------------------------------------------
	// CEncryptDict
	//----------------------------------------------------------------------------------------
	CEncryptDict::CEncryptDict(CXref* pXref)
	{
		m_pEncrypt = new CEncrypt();
		if (m_pEncrypt)
			m_pEncrypt->Init();

		pXref->Add(this);
	}
	CEncryptDict::~CEncryptDict()
	{
		if (m_pEncrypt)
			delete m_pEncrypt;
	}
	void CEncryptDict::CreateId(CInfoDict* pInfo, CXref* pXref)
	{
		CMd5 oMd5;
		oMd5.Init();

		std::time_t oTime = time(0);
		oMd5.Update((BYTE*)&oTime, sizeof(oTime));

		// Создаем идентификатор файла по элементам библиотеки Info.
		if (pInfo)
		{
			const char *sTemp = NULL;
			unsigned int nLen = 0;

			// Author
			sTemp = pInfo->GetInfo(InfoAuthor);
			if ((nLen = StrLen(sTemp, -1)) > 0)
				oMd5.Update((const BYTE *)sTemp, nLen);

			// Creator
			sTemp = pInfo->GetInfo(InfoCreator);
			if ((nLen = StrLen(sTemp, -1)) > 0)
				oMd5.Update((const BYTE *)sTemp, nLen);

			// Producer   
			sTemp = pInfo->GetInfo(InfoProducer);
			if ((nLen = StrLen(sTemp, -1)) > 0)
				oMd5.Update((const BYTE *)sTemp, nLen);

			// Title   
			sTemp = pInfo->GetInfo(InfoTitle);
			if ((nLen = StrLen(sTemp, -1)) > 0)
				oMd5.Update((const BYTE *)sTemp, nLen);

			// Subject   
			sTemp = pInfo->GetInfo(InfoSubject);
			if ((nLen = StrLen(sTemp, -1)) > 0)
				oMd5.Update((const BYTE *)sTemp, nLen);

			// Keywords   
			sTemp = pInfo->GetInfo(InfoKeyWords);
			if ((nLen = StrLen(sTemp, -1)) > 0)
				oMd5.Update((const BYTE *)sTemp, nLen);

			int nXrefEntriesCount = pXref->GetCount();
			oMd5.Update((const BYTE *)&nXrefEntriesCount, sizeof(unsigned int));

		}
		oMd5.Final(m_pEncrypt->m_anEncryptID);
	}
	void CEncryptDict::SetPassword(const char* sOwnerPassword, const char* sUserPassword)
	{
		if (0 == StrLen(sOwnerPassword, 2))
			return;

		if (sOwnerPassword && sUserPassword && 0 == StrCmp(sOwnerPassword, sUserPassword))
			return;

		PadOrTrancatePassword(sOwnerPassword, m_pEncrypt->m_anOwnerPassword);
		PadOrTrancatePassword(sUserPassword, m_pEncrypt->m_anUserPassword);
	}
	void CEncryptDict::Prepare(CInfoDict* pInfo, CXref* pXref)
	{
		CreateId(pInfo, pXref);
		
		m_pEncrypt->CreateOwnerKey();
		m_pEncrypt->CreateEncryptionKey();
		m_pEncrypt->CreateUserKey();

		CBinaryObject* pOwnerKey = new CBinaryObject(m_pEncrypt->m_anOwnerKey, PASSWD_LEN);
		if (!pOwnerKey)
			return;

		Add("O", pOwnerKey);

		CBinaryObject* pUserKey = new CBinaryObject(m_pEncrypt->m_anUserKey, PASSWD_LEN);
		if (!pUserKey)
			return;

		Add("U", pUserKey);

		Add("Filter", "Standard");
		if (encryptmode_R2 == m_pEncrypt->m_eMode)
		{
			Add("V", 1);
			Add("R", 2);
		}
		else if (encryptmode_R3 == m_pEncrypt->m_eMode)
		{
			Add("V", 2);
			Add("R", 3);
			Add("Length", m_pEncrypt->m_unKeyLen * 8);
		}
		Add("P", m_pEncrypt->m_unPermission);
	}
}