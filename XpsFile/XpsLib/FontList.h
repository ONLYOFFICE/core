#ifndef _XPS_XPSLIB_FONTLIST_H
#define _XPS_XPSLIB_FONTLIST_H

#include <map>
#include <string>
#include <sstream>

#include "../../DesktopEditor/graphics/TemporaryCS.h"
#include "../../DesktopEditor/common/File.h"
#include "Utils.h"

namespace XPS
{
	class CFontList
	{
	public:

		CFontList()
		{
			m_oCS.InitializeCriticalSection();
		}
		~CFontList()
		{
			m_oCS.DeleteCriticalSection();
		}
		void Clear()
		{
			m_mList.clear();
		}
		void Check(const std::wstring& wsName, const std::wstring& wsFontPath)
		{
			m_oCS.Enter();
			if (!Find(wsName))
			{
				Add(wsName);

				unsigned char sKey[16];
				GetFontKey(wsName, sKey);

				NSFile::CFileBinary oFile;
				oFile.OpenFile(wsFontPath, true);

				unsigned char sFontData[32];
				DWORD dwBytesRead;
				oFile.ReadFile(sFontData, 32, dwBytesRead);

				for (int nIndex = 0; nIndex < 32; nIndex++)
					sFontData[nIndex] ^= sKey[nIndex % 16];

				FILE* pFile = oFile.GetFileNative();
				fseek(pFile, 0, SEEK_SET);
				fwrite(sFontData, 1, 32, pFile);
				fclose(pFile);
			}
			m_oCS.Leave();
		}
	private:

		bool Find(const std::wstring& wsName)
		{
			std::map<std::wstring, bool>::iterator oIter = m_mList.find(wsName);
			if (oIter != m_mList.end())
				return oIter->second;

			return false;
		}
		void Add(const std::wstring& wsName)
		{
			m_mList.insert(std::pair<std::wstring, bool>(wsName, true));
		}
		void GetFontKey(const std::wstring& wsName, unsigned char* sKey)
		{
			int k = 0;
			for (int i = wsName.length() - 1; i >= 0; i--)
			{
				if ('-' != wsName[i])
				{
					sKey[k] = (unsigned char)GetIntegerFromHex(wsName.substr(i - 1, 2));
					i--;
					k++;
				}
			}
		}
		int  GetIntegerFromHex(const std::wstring& wsString)
		{
			if (0 == wsString.size())
				return 0;

			std::wistringstream wiss(wsString);

			int nValue = 0;
			wiss >> std::hex >> nValue;
			return nValue;
		}

	private:

		NSCriticalSection::CRITICAL_SECTION m_oCS;
		std::map<std::wstring, bool>        m_mList;
	};
}

#endif // _XPS_XPSLIB_FONTLIST_H