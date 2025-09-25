#include <vector>
#include <cctype>

#include "../../../../DesktopEditor/common/Directory.h"
#include "../../../../DesktopEditor/fontengine/TextHyphen.h"
#include "../../../../DesktopEditor/common/StringUTF32.h"

#define USE_DICTIONARIES_FROM_MEMORY

std::vector<std::wstring> ReadWords(const std::wstring& file)
{
	std::vector<std::wstring> words;

	NSFile::CFileBinary oFile;
	if (!oFile.OpenFile(file))
		return words;

	int nInputDataLen = (int)oFile.GetFileSize();
	char* pInputData = new char[nInputDataLen];

	oFile.ReadFile((BYTE*)pInputData, (DWORD)nInputDataLen);

	int nPos = 0;
	if (nInputDataLen > 3 &&
		((BYTE)pInputData[0] == 0xEF) && ((BYTE)pInputData[1] == 0xBB) && ((BYTE)pInputData[2]) == 0xBF)
	{
		nPos = 3;
	}

	while (nPos < nInputDataLen)
	{
		while (nPos < nInputDataLen && (pInputData[nPos] == '\n' || pInputData[nPos] == '\r'))
			++nPos;

		int nStart = nPos;

		while (nPos < nInputDataLen && (pInputData[nPos] != '\n' && pInputData[nPos] != '\r'))
			++nPos;

		if (nPos > nStart)
		{
			std::string s(pInputData + nStart, nPos - nStart);
			words.push_back(UTF8_TO_U(s));
		}
	}

	oFile.CloseFile();
	RELEASEARRAYOBJECTS(pInputData);
	return words;
}

std::wstring HyphenWord(NSHyphen::CEngine& engine, const int& lang, const std::wstring& word)
{
	std::string worda = U_TO_UTF8(word);
	char* result = engine.Process(lang, worda.c_str(), (int)worda.length());

	NSStringUtils::CStringUTF32 oInput = word;
	NSStringUtils::CStringUTF32 oOutput;

	int nCurrentIndex = 0;
	int nPosHyphen = 0;
	while (result[nPosHyphen] != 0)
	{
		if (1 == (result[nPosHyphen] & 1))
		{
			int nLenChunk = nPosHyphen - nCurrentIndex + 1;
			oOutput += oInput.substr(nCurrentIndex, nLenChunk);
			oOutput += '=';
			nCurrentIndex += nLenChunk;
		}
		++nPosHyphen;
	}

	if (nCurrentIndex < oInput.length())
		oOutput += oInput.substr(nCurrentIndex, oInput.length() - nCurrentIndex);

	return oOutput.ToStdWString();
}

int GetLanguage(const std::wstring& sLanguage)
{
	std::string sLang = U_TO_UTF8(sLanguage);
	int nLang = 0;
	for (int j = 0; j < NSTextLanguages::DictionaryRec_count; ++j)
	{
		if (std::string(NSTextLanguages::Dictionaries[j].m_name) == sLang)
		{
			nLang = NSTextLanguages::Dictionaries[j].m_lang;
			break;
		}
	}
	return nLang;
}

int main(int argc, char *argv[])
{
	NSHyphen::CEngine engine;
	std::wstring dictionaries_dir = NSFile::GetProcessDirectory() + L"/../../../../../../dictionaries";
	engine.Init(dictionaries_dir);

#if 0
	std::wstring sOneWord = HyphenWord(engine, 1033, L"expedition");
#endif

	std::wstring input_dir = NSFile::GetProcessDirectory() + L"/input";
	std::wstring output_dir = NSFile::GetProcessDirectory() + L"/output";
	std::vector<std::wstring> langs = NSDirectory::GetFiles(input_dir);

	for (std::wstring& lang : langs)
	{
		std::wstring sLang = NSFile::GetFileName(lang);
		int nLang = GetLanguage(sLang);
		if (nLang == 0)
			continue;

		std::vector<std::wstring> words = ReadWords(lang);
		if (words.empty())
			continue;

#ifdef USE_DICTIONARIES_FROM_MEMORY
		BYTE* pData = NULL;
		DWORD dwDataLen = 0;
		std::wstring sFileDict = dictionaries_dir + L"/" + sLang + L"/hyph_" + sLang + L".dic";
		if (NSFile::CFileBinary::ReadAllBytes(sFileDict, &pData, dwDataLen))
		{
			engine.LoadDictionary(nLang, pData, (unsigned int)dwDataLen);
			RELEASEARRAYOBJECTS(pData);
		}
#endif

		std::wstring sResult;
		for (std::wstring& word : words)
		{
			sResult += HyphenWord(engine, nLang, word);
			sResult += L"\r\n";
		}

		std::wstring sOutputFile = output_dir + L"/" + sLang;

		if (NSFile::CFileBinary::Exists(sOutputFile))
			NSFile::CFileBinary::Remove(sOutputFile);

		NSFile::CFileBinary::SaveToFile(sOutputFile, sResult, true);
	}

	return 0;
}







