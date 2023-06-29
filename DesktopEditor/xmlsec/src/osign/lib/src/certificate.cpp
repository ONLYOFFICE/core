#include "../include/osign.h"
#include "./utils.h"

#include "../../../../../../Common/3dParty/openssl/common/common_openssl.h"
#include <openssl/rand.h>

namespace OSign
{
	class CCertificate_private
	{
	public:
		CCertificate_private()
		{
		}
		~CCertificate_private()
		{
		}
	};

	CCertificate::CCertificate()
	{
		m_internal = new CCertificate_private();
	}
	CCertificate::CCertificate(const std::map<std::wstring, std::wstring>& props)
	{
		m_internal = new CCertificate_private();
	}
	CCertificate::~CCertificate()
	{
		delete m_internal;
	}

	std::map<std::wstring, std::wstring> CCertificate::GetProperties()
	{
		std::map<std::wstring, std::wstring> props;
		return props;
	}

	bool CCertificate::Load(CStorageBuffer* buffer)
	{
		return true;
	}
	bool CCertificate::Save(CStorageBuffer* buffer)
	{
		return true;
	}

	CStorageBuffer CCertificate::Sign(const CStorageBuffer& data)
	{
		CStorageBuffer buffer;
		return buffer;
	}
	bool CCertificate::Verify(const CStorageBuffer& data, const CStorageBuffer& signature)
	{
		return false;
	}
}

namespace OSign
{
	namespace Crypt
	{
		CStorageBuffer GeneratePassword(const size_t& size)
		{
			CStorageBuffer buffer;
			buffer.Alloc(size);
			RAND_priv_bytes(buffer.GetData(), (int)size);
			buffer.Skip(size);
			return buffer;
		}

		// version;salt;encoded_buffer
		CStorageBuffer Encrypt(CStorageBuffer& buffer, CStorageBuffer& password)
		{
			const int salt_len = 32;
			unsigned char salt[salt_len];
			RAND_bytes(salt, salt_len);

			unsigned char* key = NSOpenSSL::PBKDF2((const char*)password.GetData(), (int)password.GetLength(), salt, salt_len, OPENSSL_HASH_ALG_SHA256, 32);

			NSOpenSSL::CMemoryData oBufferData;
			AES_Encrypt_desktop_GCM(key, buffer.GetData(), (int)buffer.GetLength(), oBufferData);

			CStorageBuffer result;
			result.CheckAlloc(4 + 4 + salt_len + 4 + oBufferData.Size);
			result.AddInt(OSIGN_SHARE_VERSION);
			result.AddInt(salt_len);
			result.Add(salt, salt_len);
			result.AddInt(oBufferData.Size);
			result.Add(oBufferData.Data, oBufferData.Size);
			result.Seek(0);

			NSOpenSSL::openssl_free(key);

			return result;
		}
		CStorageBuffer Decrypt(CStorageBuffer& buffer, CStorageBuffer& password)
		{
			int nVersion = buffer.ReadInt();
			OSIGN_UNUSED(nVersion);

			int salt_len = buffer.ReadInt();
			unsigned char* salt = buffer.ReadData(salt_len);
			int data_len = buffer.ReadInt();

			unsigned char* key = NSOpenSSL::PBKDF2((const char*)password.GetData(), (int)password.GetLength(), salt, salt_len, OPENSSL_HASH_ALG_SHA256, 32);

			NSOpenSSL::CMemoryData oBufferData;
			NSOpenSSL::AES_Decrypt_desktop_GCM(key, buffer.ReadData(data_len), data_len, oBufferData);

			CStorageBuffer result;
			result.Alloc(oBufferData.Size);
			result.Add(oBufferData.Data, oBufferData.Size);

			NSOpenSSL::openssl_free(key);

			return result;
		}
	}
}
