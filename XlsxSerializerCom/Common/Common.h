#ifndef SERIALIZER_COMMON
#define SERIALIZER_COMMON

#ifdef _WIN32
#include <atlbase.h>
#include <atlstr.h>
#else
#include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include <string>
#include <vector>

#define BUFFER_GROW_SIZE 1 * 1024 * 1024 //1mb

namespace SerializeCommon
{
	bool IsUnicodeSymbol( WCHAR symbol );
	void CorrectString(CString& strValue);
	CString DownloadImage(const CString& strFile);
	VOID convertBase64ToImage (CString sImage, CString &pBase64);
	long Round(double val);
	CString changeExtention(CString& sSourcePath, CString& sTargetExt);
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
	void ReadFileType(CString& sXMLOptions, BYTE& result, UINT& nCodePage, WCHAR& wcDelimiter);
}

#endif //SERIALIZER_COMMON