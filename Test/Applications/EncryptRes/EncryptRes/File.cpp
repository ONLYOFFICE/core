#include "stdafx.h"
#include "File.h"
#include "AvsDocxFormat\Source\Utility\TxtFile.h"
#include "AvsDocxFormat\Source\Utility\Encoding.h"
#include "RSA\XMLEncoder.h"
#include "AvsDocxFormat\Source\Utility\Utility.h"
#include <string>
#include <vector>



const std::wstring File::readFile(const std::string& filename)
{
	TxtFile file(Encoding::ansi2unicode(filename));

	std::vector<std::wstring> content;

	if (file.isUtf8())
		content = transform(file.readUtf8(), Encoding::utf82unicode);
	else if (file.isUnicode())
		content = file.readUnicode();
	else if (file.isAnsi())
		content = transform(file.readAnsi(), Encoding::ansi2unicode);

	std::wstring fileContent;
	BOOST_FOREACH(const std::wstring& line, content)
	{
		fileContent += line;
	}
	return fileContent;
}


const std::string File::getFileOutName(const std::string& filename)
{
	std::string fileOut = filename;

	std::string::size_type pos = fileOut.find_last_of(".");
	if (pos != std::string::npos)	
		fileOut = fileOut.substr(0, pos);
	fileOut += ".crpt";

	return fileOut;
}


const std::wstring File::cryptText(const std::wstring& content)
{
	CXMLEncoder encoder;
	CString str = content.c_str();
	CStringA strA = encoder.EncryptXML(str);
	CStringW strW;
	strW = strA;
	std::wstring cryptText(strW);

	/*MediaCore::IAVSMediaData* key = encoder.GetBlowfishKey();
	LONG l;
	BYTE* p;
	key->get_DataSize(&l);
	key->get_Buffer(&p);
*/
	return cryptText;
}


void File::writeFile(const std::string& filename, const std::wstring& content)
{
	TxtFile file(Encoding::ansi2unicode(filename));
	std::vector<std::wstring> text;
	text.push_back(content);
	file.writeUtf8(transform(text, Encoding::unicode2utf8));
}