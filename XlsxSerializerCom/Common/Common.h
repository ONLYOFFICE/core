#ifndef SERIALIZER_COMMON
#define SERIALIZER_COMMON

#if defined(_WIN32) || defined (_WIN64)
    #include <atlbase.h>
    #include <atlstr.h>
#else
    #include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include "../../DesktopEditor/common/File.h"

#include <string>
#include <vector>

#define BUFFER_GROW_SIZE 1 * 1024 * 1024 //1mb

namespace SerializeCommon
{
	CString DownloadImage(const CString& strFile);
	VOID convertBase64ToImage (NSFile::CFileBinary& oFile, CString &pBase64);
	long Round(double val);
	CString changeExtention(const CString& sSourcePath, const CString& sTargetExt);
	class CommentData
	{
	public :
		CString sText;
		CString sTime;
		CString sUserId;
		CString sUserName;
		CString sQuoteText;
		bool Solved;
		bool Document;

		bool bSolved;
		bool bDocument;
		std::vector<CommentData*> aReplies;
		CommentData()
		{
			bSolved = false;
			bDocument = false;
		}
		~CommentData()
		{
			for(int i = 0, length = aReplies.size(); i < length; ++i)
				delete aReplies[i];
			aReplies.clear();
		}
	};
    void ReadFileType(const CString& sXMLOptions, BYTE& result, UINT& nCodePage, WCHAR& wcDelimiter, BYTE& saveFileType);
}

#endif //SERIALIZER_COMMON
