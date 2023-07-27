#include "../include/osign.h"
#include "./utils.h"
#include <memory>

namespace OSign
{
	class CMemory
	{
	public:
		unsigned char* m_data     = nullptr;
		size_t         m_max_size = 0;
		size_t         m_size     = 0;
		bool           m_external = false;

	public:
		CMemory()
		{
		}

		~CMemory()
		{
			Destroy();
		}

		void Clear()
		{
			m_data = nullptr;
			m_max_size = 0;
			m_size = 0;
			m_external = false;
		}

		void Destroy()
		{
			if (nullptr != m_data && !m_external)
				free(m_data);
			Clear();
		}

		void Attach(unsigned char* data, const size_t& size)
		{
			m_data = data;
			m_max_size = size;
			m_size = size;
			m_external = true;
		}

		void AddSize(size_t size)
		{
			if (m_external)
				return;

			if (nullptr == m_data)
			{
				m_max_size = (std::max)((int)size, 1000);
				m_data = (unsigned char*)malloc(m_max_size * sizeof(unsigned char));
				m_size = 0;
				return;
			}

			if ((m_size + size) > m_max_size)
			{
				while ((m_size + size) > m_max_size)
				{
					if (m_max_size > 10485760/*10 * 1024 * 1024*/)
					{
						m_max_size += (std::max)((int)size * 10, 1048576/*1024 * 1024*/);
					}
					else
					{
						m_max_size *= 2;
					}
				}

				unsigned char* pRealloc = (unsigned char*)realloc(m_data, m_max_size * sizeof(unsigned char));
				if (NULL != pRealloc)
				{
					// реаллок сработал
					m_data = pRealloc;
				}
				else
				{
					unsigned char* pMalloc = (unsigned char*)malloc(m_max_size * sizeof(unsigned char));
					memcpy(pMalloc, m_data, m_size * sizeof(char));

					free(m_data);
					m_data = pMalloc;
				}
			}
		}
	};

	class CStorageBuffer_private
	{
	public:
		std::shared_ptr<CMemory> m_data;
		size_t m_pos = 0;

	public:
		CStorageBuffer_private()
		{
		}

		~CStorageBuffer_private()
		{
			Destroy();
		}

		void Destroy()
		{
			m_data.reset();
		}

		void AddSize(size_t size)
		{
			if (!m_data)
				m_data = std::make_shared<CMemory>();

			m_data->AddSize(size);
		}
	};

	CStorageBuffer::CStorageBuffer()
	{
		m_internal = new CStorageBuffer_private();
	}

	CStorageBuffer::CStorageBuffer(const CStorageBuffer& src)
	{
		m_internal = new CStorageBuffer_private();
		m_internal->m_data = src.m_internal->m_data;
		// всегда сначала
		m_internal->m_pos = 0;
	}

	CStorageBuffer::~CStorageBuffer()
	{
		delete m_internal;
	}

	unsigned char* CStorageBuffer::GetData()
	{
		if (m_internal->m_data)
			return m_internal->m_data->m_data;
		return nullptr;
	}

	unsigned char* CStorageBuffer::GetData() const
	{
		if (m_internal->m_data)
			return m_internal->m_data->m_data;
		return nullptr;
	}

	unsigned char* CStorageBuffer::GetCurrentData()
	{
		if (m_internal->m_data)
			return m_internal->m_data->m_data + m_internal->m_data->m_size;
		return nullptr;
	}

	unsigned char* CStorageBuffer::GetCurrentData() const
	{
		if (m_internal->m_data)
			return m_internal->m_data->m_data + m_internal->m_data->m_size;
		return nullptr;
	}

	size_t CStorageBuffer::GetLength() const
	{
		if (m_internal->m_data)
			return m_internal->m_data->m_size;
		return 0;
	}

	void CStorageBuffer::Alloc(const size_t& size)
	{
		m_internal->Destroy();
		m_internal->AddSize(size);
	}
	void CStorageBuffer::CheckAlloc(const size_t& size)
	{
		m_internal->AddSize(size);
	}
	void CStorageBuffer::Free()
	{
		m_internal->Destroy();
	}
	void CStorageBuffer::FreeNoAttack()
	{
		if (m_internal->m_data)
			m_internal->m_data->m_size = 0;
	}

	bool CStorageBuffer::Save(const std::wstring& filePath)
	{
		if (!m_internal->m_data && 0 == m_internal->m_data->m_size)
			return false;

		NSFile::CFileBinary oFile;
		if (!oFile.CreateFile(filePath))
			return false;

		bool bRes = oFile.WriteFile(m_internal->m_data->m_data, (DWORD)m_internal->m_data->m_size);
		oFile.CloseFile();

		return bRes;
	}

	bool CStorageBuffer::Load(const std::wstring& filePath)
	{
		m_internal->Destroy();

		NSFile::CFileBinary oFile;
		if (!oFile.OpenFile(filePath))
			return false;

		size_t size = (size_t)oFile.GetFileSize();
		m_internal->AddSize(size);
		bool bRes = oFile.ReadFile(m_internal->m_data->m_data, (DWORD)size);

		if (!bRes)
			m_internal->Destroy();
		else
			m_internal->m_data->m_size = size;

		return bRes;
	}

	std::string CStorageBuffer::ToBase64()
	{
		if (!m_internal->m_data && 0 == m_internal->m_data->m_size)
			return "";

		int nLenDst = NSBase64::Base64EncodeGetRequiredLength((int)m_internal->m_data->m_size, NSBase64::B64_BASE64_FLAG_NOCRLF);
		char* pDataDst = new char[nLenDst];

		if (FALSE == NSBase64::Base64Encode(m_internal->m_data->m_data, (int)m_internal->m_data->m_size, (BYTE*)pDataDst, &nLenDst, NSBase64::B64_BASE64_FLAG_NOCRLF))
		{
			RELEASEARRAYOBJECTS(pDataDst);
			return "";
		}

		std::string sRes(pDataDst, nLenDst);
		RELEASEARRAYOBJECTS(pDataDst);
		return sRes;
	}

	bool CStorageBuffer::FromBase64(const std::string& base64)
	{
		int nLenDst = NSBase64::Base64DecodeGetRequiredLength((int)base64.length());

		m_internal->Destroy();
		m_internal->AddSize((size_t)nLenDst);

		if (FALSE == NSBase64::Base64Decode(base64.c_str(), (int)base64.length(), m_internal->m_data->m_data, &nLenDst))
		{
			m_internal->Destroy();
			return false;
		}
		m_internal->m_data->m_size = (size_t)nLenDst;
		return true;
	}

	// WRITE
	void CStorageBuffer::AddInt(const size_t& size)
	{
		m_internal->AddSize(4);

		unsigned char* output = GetCurrentData();
		output[0] = size & 0xFF;
		output[1] = (size >> 8) & 0xFF;
		output[2] = (size >> 16) & 0xFF;
		output[3] = (size >> 24) & 0xFF;

		m_internal->m_data->m_size += 4;
	}

	void CStorageBuffer::Add(const unsigned char* data, const size_t& size)
	{
		if (size == 0)
			return;

		m_internal->AddSize(size);
		memcpy(GetCurrentData(), data, size);
		m_internal->m_data->m_size += size;
	}

	void CStorageBuffer::Add(const CStorageBuffer* buffer)
	{
		if (buffer->GetLength() == 0)
			return;

		Add(buffer->m_internal->m_data->m_data, buffer->m_internal->m_data->m_size);
	}

	void CStorageBuffer::AddSkip(const size_t& size)
	{
		m_internal->m_data->m_size += size;
	}

	// READ
	void CStorageBuffer::Skip(const size_t& size)
	{
		m_internal->m_pos += size;
	}

	void CStorageBuffer::Seek(const size_t& pos)
	{
		m_internal->m_pos = pos;
	}

	unsigned char* CStorageBuffer::ReadData(const size_t& size)
	{
		unsigned char* data = m_internal->m_data->m_data + m_internal->m_pos;
		m_internal->m_pos += size;
		return data;
	}

	int CStorageBuffer::ReadInt()
	{
		const unsigned char* data = m_internal->m_data->m_data + m_internal->m_pos;
		m_internal->m_pos += 4;
		return data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
	}

	void CStorageBuffer::LoadExternal(unsigned char* data, size_t len)
	{
		m_internal->Destroy();
		m_internal->m_data = std::make_shared<CMemory>();
		m_internal->m_data->Attach(data, len);
		m_internal->m_pos = 0;
	}
}
