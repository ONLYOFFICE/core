/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#ifndef _ZIPFOLDER_H_
#define _ZIPFOLDER_H_

#include "ZipBuffer.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/Path.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"
#include <iostream>
#include <ostream>

class IFolder
{
public:
	enum IFolderType
	{
		iftFolder = 0,
		iftZip = 1
	};

public:
	class CBuffer
	{
	public:
		BYTE* Buffer;
		DWORD Size;

	private:
		bool m_bIsDestroy;
		CBuffer(BYTE* data = NULL, DWORD size = 0, bool destroy = false)
		{
			Buffer = data;
			Size = size;
			m_bIsDestroy = destroy;
		}
	public:
		~CBuffer()
		{
			if (m_bIsDestroy && Buffer != NULL)
				delete [] Buffer;
		}

		void UnsetDestroy()
		{
			m_bIsDestroy = false;
		}

		friend class IFolder;
		friend class CFolderSystem;
		friend class CZipFolderMemory;
	};


public:
	virtual ~IFolder() {}

	virtual IFolderType getType() = 0;
	// full path from local path
	virtual std::wstring getFullFilePath(const std::wstring& path) = 0;
	// local path from full path (without the leading '/')
	virtual std::wstring getLocalFilePath(const std::wstring& path) = 0;
	// read file into buffer. intermediate class is needed so that
	// one implementation can return memory directly, while another allocates and doesn't store it
	virtual bool read(const std::wstring& path, CBuffer*& buffer) = 0;
	// write data to file
	virtual void write(const std::wstring& path, BYTE* data, DWORD length) = 0;
	// file operations
	virtual void move(const std::wstring& src, const std::wstring& dst) = 0;
	virtual bool exists(const std::wstring& path) = 0;
	virtual void remove(const std::wstring& path) = 0;
	// directory operations
	virtual void createDirectory(const std::wstring& path) = 0;
	virtual void removeDirectory(const std::wstring& path) = 0;
	virtual std::vector<std::wstring> getFiles(const std::wstring& path, bool recursion) = 0;
	// finalization
	virtual CBuffer* finalize() { return NULL; }
	// read node
	virtual XmlUtils::CXmlNode getNodeFromFile(const std::wstring& path) = 0;
	virtual bool getReaderFromFile(const std::wstring& path, XmlUtils::CXmlLiteReader& oReader) = 0;

	// helper functions
	void writeXml(const std::wstring& path, const std::wstring& xml)
	{
		std::string sXmlUtf8 = U_TO_UTF8(xml);
		write(path, (BYTE*)sXmlUtf8.c_str(), (DWORD)sXmlUtf8.length());
	}
	void writeXmlA(const std::wstring& path, const std::string& xml)
	{
		write(path, (BYTE*)xml.c_str(), (DWORD)xml.length());
	}
	bool existsXml(const std::wstring& path)
	{
		if (exists(path))
			return true;

		std::vector<std::wstring> arPieces = getFiles(path, false);
		if (0 < arPieces.size())
		{
			std::vector<std::wstring>::iterator iter = arPieces.begin();
			while (iter != arPieces.end())
			{
				std::wstring::size_type len = iter->length();
				std::wstring::size_type pos = iter->rfind(L".piece");
				if (std::wstring::npos != pos && ((pos + 6) == len))
				{
					return true;
				}
				iter++;
			}
		}

		return false;
	}
	std::string readXml(const std::wstring& path)
	{
		CBuffer* buffer = NULL;
		if (!read(path, buffer))
		{
			std::vector<std::wstring> arPieces = getFiles(path, false);
			if (0 < arPieces.size())
			{
				std::sort(arPieces.begin(), arPieces.end(), compareAsXmlPiece);
				std::vector<std::wstring>::iterator iter = arPieces.begin();
				while (iter != arPieces.end())
				{
					std::wstring::size_type len = iter->length();
					std::wstring::size_type pos = iter->rfind(L".piece");
					if (std::wstring::npos != pos && ((pos + 6) == len))
					{
						iter++;
						continue;
					}
					else
					{
						iter = arPieces.erase(iter);
					}
				}
			}
			if (0 < arPieces.size())
			{
				std::string sResult;
				for (std::vector<std::wstring>::iterator iter = arPieces.begin(); iter != arPieces.end(); iter++)
				{
					CBuffer* bufferPiece = NULL;
					if (read(*iter, bufferPiece))
					{
						sResult += std::string((char*)bufferPiece->Buffer, (size_t)bufferPiece->Size);
					}
					delete bufferPiece;
				}
				return sResult;
			}

			return "";
		}
		std::string sXmlUtf8 = XmlUtils::GetUtf8FromFileContent(buffer->Buffer, (unsigned int)buffer->Size);
		delete buffer;
		return sXmlUtf8;
	}
	bool readFileWithChunks(const std::wstring& path, CBuffer*& buffer)
	{
		if (this->exists(path))
			return this->read(path, buffer);

		std::vector<std::wstring> arPieces = getFiles(path, false);
		if (0 < arPieces.size())
		{
			std::sort(arPieces.begin(), arPieces.end(), compareAsXmlPiece);
			std::vector<std::wstring>::iterator iter = arPieces.begin();
			while (iter != arPieces.end())
			{
				std::wstring::size_type len = iter->length();
				std::wstring::size_type pos = iter->rfind(L".piece");
				if (std::wstring::npos != pos && ((pos + 6) == len))
				{
					iter++;
					continue;
				}
				else
				{
					iter = arPieces.erase(iter);
				}
			}
		}
		if (0 == arPieces.size())
			return false;

		std::vector<CBuffer*> arBuffers;
		DWORD dwSizeFull = 0;
		for (std::vector<std::wstring>::iterator iter = arPieces.begin(); iter != arPieces.end(); iter++)
		{
			CBuffer* bufferPiece = NULL;
			if (read(*iter, bufferPiece))
			{
				arBuffers.push_back(bufferPiece);
				dwSizeFull += bufferPiece->Size;
			}
		}

		if (0 == dwSizeFull)
			return false;

		BYTE* pData = new BYTE[dwSizeFull];
		DWORD dwPos = 0;
		for (std::vector<CBuffer*>::iterator iter = arBuffers.begin(); iter != arBuffers.end(); iter++)
		{
			CBuffer* bufferPiece = *iter;
			DWORD dwSizeChunk = bufferPiece->Size;
			if (dwSizeChunk != 0)
			{
				memcpy(pData + dwPos, bufferPiece->Buffer, dwSizeChunk);
				dwPos += dwSizeChunk;
			}
			delete bufferPiece;
		}
		arBuffers.clear();

		this->removeDirectory(path);
		this->write(path, pData, dwSizeFull);

		buffer = new CBuffer(pData, dwSizeFull, true);
		return true;
	}
	std::string getFileBase64(const std::wstring& path)
	{
		CBuffer* buffer = NULL;
		if (!read(path, buffer))
			return "";

		char* pData = NULL;
		int nLen = 0;
		NSFile::CBase64Converter::Encode(buffer->Buffer, (int)buffer->Size, pData, nLen, NSBase64::B64_BASE64_FLAG_NOCRLF);

		std::string sRet(pData, (size_t)nLen);

		RELEASEARRAYOBJECTS(pData);
		delete buffer;

		return sRet;
	}

private:
	static bool compareAsXmlPiece(const std::wstring& a, const std::wstring& b)
	{
		size_t aLen = a.length();
		size_t bLen = b.length();

		size_t posA = 0;
		size_t posB = 0;

		int nPartA = 0;
		int nPartB = 0;

		size_t len = (aLen < bLen) ? aLen : bLen;
		if (2 > len)
			goto error;

		while (posA < len)
		{
			if (a[posA] != b[posA])
				break;
			++posA;
		}

		if (0 == posA)
			goto error;

		posB = posA;

		// don't search for '['. just the first non-equal
		//if ('[' != a[posA - 1] || '[' != b[posB - 1])
		//    goto error;

		while (posA < aLen)
		{
			if (a[posA] < '0' || a[posA] > '9')
				break;
			nPartA = 10 * nPartA + (a[posA] - '0');
			++posA;
		}
		if (posA == aLen || a[posA] != ']')
			goto error;

		while (posB < bLen)
		{
			if (b[posB] < '0' || b[posB] > '9')
				break;
			nPartB = 10 * nPartB + (b[posB] - '0');
			++posB;
		}
		if (posB == bLen || b[posB] != ']')
			goto error;

		return nPartA < nPartB;

error:
		return a < b;
	}
};

class CFolderSystem : public IFolder
{
	std::wstring m_sFolder;

	void correct_folder(std::wstring& folder)
	{
		if (!folder.empty() && folder[folder.length() - 1] == '/')
			folder.erase(folder.length() - 1, 1);
	}

public:
	CFolderSystem(const std::wstring& folder)
	{
		m_sFolder = folder;
#ifdef _WIN32
		NSStringUtils::string_replace(m_sFolder, L"\\", L"/");
#endif
		correct_folder(m_sFolder);
	}
	virtual IFolderType getType()
	{
		return iftFolder;
	}

	virtual std::wstring getFullFilePath(const std::wstring& path)
	{
		std::wstring full_path = path;
#ifdef _WIN32
		NSStringUtils::string_replace(full_path, L"\\", L"/");
#endif
		if (0 == full_path.find(m_sFolder))
			return full_path;
		if (full_path.empty())
			return m_sFolder;
		if (full_path[0] == L'/')
			return m_sFolder + full_path;
		return m_sFolder + L"/" + full_path;
	}
	virtual std::wstring getLocalFilePath(const std::wstring& path)
	{
		std::wstring local_path = path;
#ifdef _WIN32
		NSStringUtils::string_replace(local_path, L"\\", L"/");
#endif
		if (0 == local_path.find(m_sFolder))
			return local_path.substr(m_sFolder.length() + 1);
		if (!local_path.empty() && local_path[0] == L'/')
			return local_path.substr(1);
		return local_path;
	}

	virtual bool read(const std::wstring& path, CBuffer*& buffer)
	{
		buffer = NULL;
		std::wstring sPath = getFullFilePath(path);
		if (NSFile::CFileBinary::Exists(sPath))
		{
			BYTE* pData = NULL;
			DWORD nSize = 0;
			if (NSFile::CFileBinary::ReadAllBytes(sPath, &pData, nSize))
			{
				buffer = new CBuffer(pData, nSize, true);
				return true;
			}
		}
		return false;
	}
	virtual void write(const std::wstring& path, BYTE* data, DWORD length)
	{
		std::wstring sPath = getFullFilePath(path);
		NSFile::CFileBinary::Remove(sPath);
		NSFile::CFileBinary oFile;
		oFile.CreateFileW(sPath);
		oFile.WriteFile(data, length);
		oFile.CloseFile();
	}
	virtual void move(const std::wstring& sSrc,   const std::wstring& sDst)
	{
		NSFile::CFileBinary::Move(getFullFilePath(sSrc), getFullFilePath(sDst));
	}
	virtual bool exists(const std::wstring& path)
	{
		std::wstring full = getFullFilePath(path);
		return NSFile::CFileBinary::Exists(full) && !NSDirectory::Exists(full);
	}
	virtual void remove(const std::wstring& path)
	{
		NSFile::CFileBinary::Remove(getFullFilePath(path));
	}
	virtual void createDirectory(const std::wstring& path)
	{
		std::wstring sPath = getFullFilePath(path);
		if (!NSDirectory::Exists(sPath))
			NSDirectory::CreateDirectory(sPath);
	}
	virtual void removeDirectory(const std::wstring& path)
	{
		std::wstring sPath = getFullFilePath(path);
		if (NSDirectory::Exists(sPath))
			NSDirectory::DeleteDirectory(sPath);
	}
	virtual std::vector<std::wstring> getFiles(const std::wstring& path, bool bIsRecursion)
	{
		std::wstring folder = getFullFilePath(path);
		correct_folder(folder);
		std::vector<std::wstring> files = NSDirectory::GetFiles(folder, bIsRecursion);
#ifdef _WIN32
		for (std::vector<std::wstring>::iterator i = files.begin(); i != files.end(); i++)
		{
			NSStringUtils::string_replace(*i, L"\\", L"/");
		}
#endif
		return files;
	}
	virtual XmlUtils::CXmlNode getNodeFromFile(const std::wstring& path)
	{
		XmlUtils::CXmlNode node;
		node.FromXmlFile(getFullFilePath(path));
		return node;
	}
	virtual bool getReaderFromFile(const std::wstring& path, XmlUtils::CXmlLiteReader& oReader)
	{
		return oReader.FromFile(getFullFilePath(path));
	}
};

// Works with an archive in memory
class CZipFolderMemory : public IFolder
{
	CZipBuffer* m_zlib;

protected:

	// Converts wstring -> string and removes '/' at the beginning, since paths are relative to the archive
	std::string getLocalFilePathA(const std::wstring& path)
	{
		std::string sPath = U_TO_UTF8(path);
		if (!sPath.empty() && sPath[0] == '/')
			return NSSystemPath::NormalizePath(sPath.substr(1), true);
		return NSSystemPath::NormalizePath(sPath, true);
	}

public:
	// Opens the archive, the passed data must be freed after class usage
	CZipFolderMemory()
	{
		m_zlib = new CZipBuffer();
	}
	CZipFolderMemory(BYTE* data, DWORD length)
	{
		m_zlib = new CZipBuffer(data, length);
	}
	// Closes the archive and frees memory
	~CZipFolderMemory()
	{
		delete m_zlib;
	}

	virtual IFolderType getType()
	{
		return iftZip;
	}

	// Relative path to the file in the archive
	virtual std::wstring getFullFilePath(const std::wstring& path)
	{
		return path;
	}
	// Relative path to the file in the archive without '/' at the beginning
	virtual std::wstring getLocalFilePath(const std::wstring& path)
	{
		if (!path.empty() && path[0] == L'/')
			return path.substr(1);
		return path;
	}

	// Reads a file by relative path in the archive, the returned data must be freed
	virtual bool read(const std::wstring& path, CBuffer*& buffer)
	{
		buffer = NULL;
		std::string sPath = getLocalFilePathA(path);
		BYTE* pData = NULL;
		DWORD nSize = 0;
		m_zlib->getFile(sPath, pData, nSize);
		if (nSize)
		{
			buffer = new CBuffer(pData, nSize, false);
			return true;
		}
		return false;
	}
	// Writes a file by relative path in the archive, the passed data must be freed
	virtual void write(const std::wstring& path, BYTE* data, DWORD length)
	{
		std::string sPath = getLocalFilePathA(path);
		m_zlib->addFile(sPath, data, length);
	}
	// Moves a file within the archive
	virtual void move(const std::wstring& sSrc, const std::wstring& sDst)
	{
		m_zlib->move(getLocalFilePathA(sSrc), getLocalFilePathA(sDst));
	}
	// Checks if the file exists in the archive
	virtual bool exists(const std::wstring& path)
	{
		std::string sPath = getLocalFilePathA(path);
		return std::find_if(m_zlib->m_arrFiles.begin(), m_zlib->m_arrFiles.end(), [sPath](const CZipBuffer::CFile& file){ return file.m_sPath == sPath; }) != m_zlib->m_arrFiles.end();
	}
	// Removes a file by relative path in the archive
	virtual void remove(const std::wstring& path)
	{
		std::string sPath = getLocalFilePathA(path);
		m_zlib->removeFile(sPath);
	}
	// Creating a directory in the archive is not required
	virtual void createDirectory(const std::wstring& path)
	{
	}
	virtual void removeDirectory(const std::wstring& path)
	{
		std::vector<std::wstring> arFiles = getFiles(path, true);
		for (std::vector<std::wstring>::iterator i = arFiles.begin(); i != arFiles.end(); i++)
			remove(*i);
	}
	// Returns a vector of paths located in the folder
	virtual std::vector<std::wstring> getFiles(const std::wstring& path, bool bIsRecursion)
	{
		std::string sPath = getLocalFilePathA(path);
		std::vector<std::wstring> sRes;

		for (const CZipBuffer::CFile& i : m_zlib->m_arrFiles)
		{
			if (bIsRecursion)
			{
				if (i.m_sPath.find(sPath) == 0)
					sRes.push_back(L'/' + UTF8_TO_U(i.m_sPath));
			}
			else
			{
				size_t nFindDirectory = i.m_sPath.find(sPath);
				if (nFindDirectory == 0)
				{
					nFindDirectory = i.m_sPath.find_first_of("\\/", sPath.length());
					if (nFindDirectory != std::wstring::npos && i.m_sPath.find_first_of("\\/", nFindDirectory + 1) == std::wstring::npos)
						sRes.push_back(L'/' + UTF8_TO_U(i.m_sPath));
				}
			}
		}
		return sRes;
	}
	// Returns the archived data and closes the archive, the returned data must be freed
	virtual CBuffer* finalize()
	{
		BYTE* data = NULL;
		DWORD length = 0;
		m_zlib->save(data, length);
		m_zlib->close();
		return new CBuffer(data, length, true);
	}
	// Reads a file by relative path in the archive and creates a CXmlNode from it
	virtual XmlUtils::CXmlNode getNodeFromFile(const std::wstring& path)
	{
		CBuffer* buffer = NULL;
		XmlUtils::CXmlNode node;
		if (!read(path, buffer))
			return node;

		std::string sUtf8((char*)buffer->Buffer, (size_t)buffer->Size);
		node.FromXmlStringA(sUtf8);
		delete buffer;
		return node;
	}
	virtual bool getReaderFromFile(const std::wstring& path, XmlUtils::CXmlLiteReader& oReader)
	{
		CBuffer* buffer = NULL;
		if (!read(path, buffer))
			return false;

		const std::string sUtf8((char*)buffer->Buffer, (size_t)buffer->Size);
		delete buffer;

		return oReader.FromStringA(sUtf8);
	}
};

#endif //_ZIPFOLDER_H_
