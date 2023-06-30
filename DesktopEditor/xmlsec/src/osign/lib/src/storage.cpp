#include "../include/osign.h"
#include "./utils.h"

#define OSIGN_STORAGE_VERSION 1

namespace OSign
{
	class CStorage_private
	{
	public:
		std::vector<CCertificate*> m_certs;

	public:
		CStorage_private()
		{

		}
		~CStorage_private()
		{
			Destroy();
		}

	public:
		void Destroy()
		{
			for (std::vector<CCertificate*>::const_iterator iter = m_certs.begin(); iter != m_certs.end(); iter++)
			{
				CCertificate* cert = *iter;
				delete cert;
			}
			m_certs.clear();
		}
	};

	CStorage::CStorage()
	{
		m_internal = new CStorage_private();
	}

	CStorage::~CStorage()
	{
		delete m_internal;
	}

	int CStorage::GetCount()
	{
		return (int)m_internal->m_certs.size();
	}

	CCertificate* CStorage::Get(const int& index)
	{
		if (0 > index || index >= GetCount())
			return nullptr;

		return m_internal->m_certs[index];
	}

	void CStorage::Add(CCertificate* cert)
	{
		m_internal->m_certs.push_back(cert);
	}

	bool CStorage::Load(CStorageBuffer* buffer)
	{
		size_t len = buffer->GetLength();

		if (len < 4)
			return false;

		unsigned int nVersion = buffer->ReadInt();
		OSIGN_UNUSED(nVersion);

		unsigned int nCount = buffer->ReadInt();

		CStorageBuffer oCertBuffer;
		for (unsigned int i = 0; i < nCount; ++i)
		{
			int nLen = buffer->ReadInt();
			oCertBuffer.LoadExternal(buffer->ReadData((size_t)nLen), nLen);

			CCertificate* pCert = new CCertificate();
			if (pCert->Load(&oCertBuffer))
				m_internal->m_certs.push_back(pCert);
			else
				delete pCert;
		}
		return true;
	}

	bool CStorage::Save(CStorageBuffer* buffer)
	{
		buffer->CheckAlloc(1000);
		buffer->AddInt(OSIGN_STORAGE_VERSION);
		size_t nCount = m_internal->m_certs.size();
		buffer->AddInt(nCount);

		if (0 == nCount)
			return true;

		CStorageBuffer oBuffer;
		oBuffer.Alloc(1000);
		for (size_t i = 0; i < nCount; ++i)
		{
			oBuffer.FreeNoAttack();
			if (m_internal->m_certs[i]->Save(&oBuffer))
			{
				buffer->AddInt(oBuffer.GetLength());
				buffer->Add(&oBuffer);
			}
		}
		return true;
	}
}
