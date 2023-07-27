#ifndef _OSIGN_H_
#define _OSIGN_H_

#include <stddef.h>
#include <string>
#include <vector>
#include <map>

#include "../../../../../common/base_export.h"

#ifdef OSIGN_BUILDING_INTERNAL
#define OSIGN_DECL
#else
#ifdef OSIGN_BUILDING
#define OSIGN_DECL Q_DECL_EXPORT
#else
#define OSIGN_DECL Q_DECL_IMPORT
#endif
#endif

namespace OSign
{
	namespace Properties
	{
		static const wchar_t* Email              = L"email";
		static const wchar_t* Phone              = L"phone";
		static const wchar_t* Name               = L"name";
		static const wchar_t* Signature          = L"signature";
		static const wchar_t* DateFrom           = L"date-from";
		static const wchar_t* DateTo             = L"date-to";
		static const wchar_t* CommonName         = L"common-name";
		static const wchar_t* OrganizationName   = L"organization";
		static const wchar_t* Country            = L"country";
	}

	class CStorageBuffer_private;
	class OSIGN_DECL CStorageBuffer
	{
	public:
		CStorageBuffer();
		CStorageBuffer(const CStorageBuffer& src);
		~CStorageBuffer();

	public:
		unsigned char* GetData();
		unsigned char* GetData() const;
		size_t GetLength() const;

		unsigned char* GetCurrentData();
		unsigned char* GetCurrentData() const;

		void Alloc(const size_t& size);
		void CheckAlloc(const size_t& size);
		void Free();
		void FreeNoAttack();

		bool Save(const std::wstring& filePath);
		bool Load(const std::wstring& filePath);

		std::string ToBase64();
		bool FromBase64(const std::string& base64);

		// write
		void AddInt(const size_t& size);
		void Add(const unsigned char* data, const size_t& size);
		void Add(const CStorageBuffer* buffer);
		void AddSkip(const size_t& size);

		// read
		void Seek(const size_t& pos);
		void Skip(const size_t& size);
		unsigned char* ReadData(const size_t& size);
		int ReadInt();

		void LoadExternal(unsigned char* data, size_t len);

	private:
		CStorageBuffer_private* m_internal;
	};

	class CCertificate_private;
	class OSIGN_DECL CCertificate
	{
	public:
		CCertificate();
		~CCertificate();
		void Generate(const std::map<std::wstring, std::wstring>& props = std::map<std::wstring, std::wstring>());

	public:
		std::map<std::wstring, std::wstring> GetProperties();

		bool Load(CStorageBuffer* buffer);
		bool Save(CStorageBuffer* buffer);

		bool LoadKey(CStorageBuffer* buffer);
		bool SaveKey(CStorageBuffer* buffer);

		bool LoadCert(CStorageBuffer* buffer);
		bool SaveCert(CStorageBuffer* buffer);

		CStorageBuffer Sign(const CStorageBuffer& data);
		bool Verify(const CStorageBuffer& data, const CStorageBuffer& signature);

	private:
		CCertificate_private* m_internal;
	};

	class CStorage_private;
	class OSIGN_DECL CStorage
	{
	public:
		CStorage();
		~CStorage();

	public:
		int GetCount();
		CCertificate* Get(const int& index);

		void Add(CCertificate* cert);

		bool Load(CStorageBuffer* buffer);
		bool Save(CStorageBuffer* buffer);

	private:
		CStorage_private* m_internal;
	};

	namespace Crypt
	{
		OSIGN_DECL CStorageBuffer GeneratePassword(const size_t& size = 32);

		// version;salt;encoded_buffer
		OSIGN_DECL CStorageBuffer Encrypt(CStorageBuffer& buffer, CStorageBuffer& password);
		OSIGN_DECL CStorageBuffer Decrypt(CStorageBuffer& buffer, CStorageBuffer& password);
	}

	OSIGN_DECL std::string GetLastError();
	OSIGN_DECL void LogLastError();
}

#endif // _XMLSIGNER_OFORM_H_
