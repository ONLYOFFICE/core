#ifndef OFFICE_UTILS_H
#define OFFICE_UTILS_H

#include "OfficeUtilsCommon.h"

class COfficeUtils
{
private:
	OnProgressCallback* m_fCallback;
public:
	COfficeUtils(OnProgressCallback* fCallback);

public:

	HRESULT ExtractToDirectory(const std::wstring& zipFile, const std::wstring& unzipDir, wchar_t* password, SHORT extract_without_path);
	HRESULT CompressFileOrDirectory(const std::wstring& name, const std::wstring& outputFile, SHORT level);
	HRESULT Uncompress(BYTE* destBuf, ULONG* destSize, BYTE* sourceBuf, ULONG sourceSize);
	HRESULT Compress(BYTE* destBuf, ULONG* destSize, BYTE* sourceBuf, ULONG sourceSize, SHORT level);

	HRESULT IsArchive(const std::wstring& filename);
	HRESULT IsFileExistInArchive(const std::wstring& zipFile, const std::wstring& filePath);
	HRESULT LoadFileFromArchive(const std::wstring& zipFile, const std::wstring& filePath, BYTE** fileInBytes, ULONG& nFileSize);
	HRESULT ExtractFilesToMemory(const std::wstring& zipFile, const ExtractedFileCallback& data_receiver, void* pParam, bool* result);
	HRESULT CompressFilesFromMemory(const std::wstring& zipFile, const RequestFileCallback& data_source, void* pParam, SHORT compression_level, bool* result);

};

#endif
