/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "./../source/ECMACryptFile.h"
#include "./../../DesktopEditor/common/File.h"
#include "./../../Common/3dParty/openssl/common/common_openssl.h"
#include <iostream>
#include <map>
#include <vector>

void string_replace(std::wstring& text, const std::wstring& replaceFrom, const std::wstring& replaceTo)
{
	size_t posn = 0;
	while (std::wstring::npos != (posn = text.find(replaceFrom, posn)))
	{
		text.replace(posn, replaceFrom.length(), replaceTo);
		posn += replaceTo.length();
	}
}
void string_replaceA(std::string& text, const std::string& replaceFrom, const std::string& replaceTo)
{
	size_t posn = 0;
	while (std::string::npos != (posn = text.find(replaceFrom, posn)))
	{
		text.replace(posn, replaceFrom.length(), replaceTo);
		posn += replaceTo.length();
	}
}

enum RecordType
{
	rtNone     = 0,
	rtAdd      = 1,
	rtRemove   = 3,
	rtEncrypt  = 4,
	rtDecrypt  = 5,
	rtPrint    = 6,
	rtMaster   = 7
};

class CRecord
{
public:
	RecordType Type = rtNone;
	std::map<std::wstring, std::wstring> Items;

public:
	CRecord(RecordType type = rtNone)
	{
		Type = type;
	}

	CRecord(const CRecord& rec)
	{
		Type = rec.Type;
		Items = rec.Items;
	}
	CRecord& operator=(const CRecord& rec)
	{
		Type = rec.Type;
		Items = rec.Items;
		return *this;
	}

	static RecordType getType(const std::wstring& key)
	{
		if (key == L"--add")
			return rtAdd;
		if (key == L"--remove")
			return rtRemove;
		if (key == L"--encrypt")
			return rtEncrypt;
		if (key == L"--decrypt")
			return rtDecrypt;
		if (key == L"--print")
			return rtPrint;
		if (key == L"--master")
			return rtMaster;
		return rtNone;
	}

	std::wstring getValue(const std::wstring& key)
	{
		std::map<std::wstring, std::wstring>::iterator iter = Items.find(key);
		if (iter == Items.end())
			return L"";
		return iter->second;
	}

	void addItem(std::wstring& key)
	{
		if (0 == key.find(L"--"))
			key = key.substr(2);

		std::wstring::size_type pos = key.find(L"=");
		if (std::wstring::npos != pos)
		{
			Items.insert(std::pair<std::wstring, std::wstring>(key.substr(0, pos), key.substr(pos + 1)));
		}
	}

	bool isValid()
	{
		switch (Type)
		{
		case rtAdd:
		{
			if (!getValue(L"user").empty() && (!getValue(L"data").empty() || !getValue(L"key").empty()))
				return true;
			break;
		}
		case rtRemove:
		{
			if (!getValue(L"user").empty())
				return true;
			break;
		}
		case rtEncrypt:
		{
			return true;
		}
		case rtDecrypt:
		{
			return true;
		}
		case rtPrint:
		{
			return true;
		}
		case rtMaster:
		{
			if (!getValue(L"user").empty() && !getValue(L"key").empty())
				return true;
			return true;
		}
		default:
			break;
		}

		std::string sCommandName = U_TO_UTF8(getName());
		if (sCommandName.empty())
		{
			std::cout << "unknown command" << std::endl;
		}
		else
		{
			std::cout << "bad command: " << sCommandName << std::endl;
		}

		return false;
	}

	std::wstring getName()
	{
		switch (Type)
		{
		case rtAdd:
		{
			return L"add";
		}
		case rtRemove:
		{
			return L"remove";
		}
		case rtEncrypt:
		{
			return L"encrypt";
		}
		case rtDecrypt:
		{
			return L"decrypt";
		}
		case rtPrint:
		{
			return L"printinfo";
		}
		case rtMaster:
		{
			return L"master";
		}
		default:
			break;
		}
		return L"";
	}
};

#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
	if (argc <= 0)
		return 0;

	std::wstring file_path;
	std::wstring file_password;

	CRecord EncryptRecord;
	CRecord DecryptRecord;
	CRecord PrintRecord;
	CRecord MasterRecord;
	std::vector<CRecord> Records;

	CRecord CurrentRecord;

	for (int i = 0; i < argc; ++i)
	{
		std::string param;
#ifdef WIN32
		param = U_TO_UTF8(std::wstring(argv[i]));
#else
		param = std::string(argv[i]);
#endif

		if (param == "--help")
		{
			std::cout << "1) encrypt/decrypt" << std::endl;
			std::cout << "decrypt command removes all user info" << std::endl;
			std::cout << "ooxml_crypt --file=path_to_document --encrypt --password=password" << std::endl;
			std::cout << "ooxml_crypt --file=path_to_document --decrypt --password=password" << std::endl;
			std::cout << std::endl;

			std::cout << "2) print info" << std::endl;
			std::cout << "ooxml_crypt --print" << std::endl;
			std::cout << std::endl;

			std::cout << "3) add/remove records" << std::endl;
			std::cout << "ooxml_crypt --file=path_to_document --add --user=user --data=data" << std::endl;
			std::cout << "ooxml_crypt --file=path_to_document --remove --user=user" << std::endl;
			std::cout << std::endl;

			std::cout << "4) generate record" << std::endl;
			std::cout << "ooxml_crypt --file=path_to_document --add --user=user --key=pem_file_public_key --password=password" << std::endl;
			std::cout << std::endl;

			std::cout << "5) work without password" << std::endl;
			std::cout << "ooxml_crypt --file=path_to_document --add --user=user --key=pem_file --master --user=user --key=pem_file_private_key" << std::endl;
			std::cout << std::endl;

			return 0;
		}
	}

	for (int i = 0; i <= argc; ++i)
	{
		// чтобы не дублировать код
		std::wstring param = L"--print";

		if (i < argc)
		{
#ifdef WIN32
			param = std::wstring(argv[i]);
#else
			std::string paramA(argv[i]);
			param = UTF8_TO_U(paramA);
#endif
		}

		std::wstring::size_type len = param.length();
		if (2 > len)
			continue;

		if (0 == param.find(L"--file="))
		{
			file_path = param.substr(7);
			continue;
		}
		if (0 == param.find(L"--password="))
		{
			file_password = param.substr(11);
			continue;
		}

		RecordType rtCurrent = CRecord::getType(param);
		if (rtNone != rtCurrent)
		{
			if (CurrentRecord.Type != rtNone &&	CurrentRecord.isValid())
			{
				switch (CurrentRecord.Type)
				{
				case rtDecrypt:
				{
					DecryptRecord = CurrentRecord;
					break;
				}
				case rtEncrypt:
				{
					EncryptRecord = CurrentRecord;
					break;
				}
				case rtPrint:
				{
					PrintRecord = CurrentRecord;
					break;
				}
				case rtAdd:
				case rtRemove:
				{
					Records.push_back(CurrentRecord);
					break;
				}
				case rtMaster:
				{
					MasterRecord = CurrentRecord;
				}
				default:
					break;
				}
			}

			CurrentRecord.Type = rtCurrent;
			CurrentRecord.Items.clear();
		}
		else if (CurrentRecord.Type != rtNone)
		{
			CurrentRecord.addItem(param);
		}
	}

	if (file_path.empty() || !NSFile::CFileBinary::Exists(file_path))
	{
		std::cout << "error: file not exist" << std::endl;
		return 1;
	}

	if (DecryptRecord.Type == rtDecrypt)
	{
		ECMACryptFile file;
		bool bDataIntegrity = false;
		bool result = file.DecryptOfficeFile(file_path, file_path, file_password, bDataIntegrity);
		if (!result)
		{
			std::cout << "error: file is not decrypted" << std::endl;
			return 1;
		}
		return 0;
	}

	if (EncryptRecord.Type == rtEncrypt)
	{
		ECMACryptFile file;
		bool result = file.EncryptOfficeFile(file_path, file_path, file_password, L"ONLYOFFICE CryptoEngine (Version 1)\n\n");
		if (!result)
		{
			std::cout << "error: file is not encrypted" << std::endl;
			return 1;
		}
	}

	ECMACryptFile file;
	std::string docinfo = file.ReadAdditional(file_path, L"DocumentID");
	std::string docinfoOld = docinfo;

	// декодируем пароль (если надо)
	if (MasterRecord.Type == rtMaster && file_password.empty())
	{
		std::string user = U_TO_UTF8(MasterRecord.getValue(L"user"));
		std::wstring keyW = MasterRecord.getValue(L"key");

		std::string::size_type pos = docinfo.find(user);
		while (pos != std::string::npos)
		{
			std::string::size_type posEndUser = docinfo.find("\n", pos);
			if (posEndUser == std::string::npos)
				break;

			std::string userCur = docinfo.substr(pos, posEndUser - pos);
			if (user == userCur)
			{
				std::string::size_type posEndUser2 = docinfo.find("\n", posEndUser + 1);
				if (posEndUser2 == std::string::npos)
					break;

				std::string data = docinfo.substr(posEndUser + 1, posEndUser2 - posEndUser - 1);

				std::string private_key_content;
				NSFile::CFileBinary::ReadAllTextUtf8A(keyW, private_key_content);

				std::string password;
				if (NSOpenSSL::RSA_DecryptPrivate_desktop((unsigned char*)private_key_content.c_str(), data, password))
				{
					file_password = UTF8_TO_U(password);
					break;
				}
			}

			pos = docinfo.find(user);
		}
	}

	// сначала удаляем
	for (std::vector<CRecord>::iterator iter = Records.begin(); iter != Records.end(); iter++)
	{
		CRecord& rec = *iter;
		if (rec.Type != rtRemove)
			continue;

		std::string user = U_TO_UTF8(rec.getValue(L"user"));

		std::string::size_type pos = docinfo.find(user);
		while (pos != std::string::npos)
		{
			std::string::size_type posEnd = docinfo.find("\n\n", pos);
			if (posEnd == std::string::npos)
				break;

			docinfo = docinfo.substr(0, pos) + docinfo.substr(posEnd + 2);
			pos = docinfo.find(user);
		}
	}

	// теперь добавляем
	for (std::vector<CRecord>::iterator iter = Records.begin(); iter != Records.end(); iter++)
	{
		CRecord& rec = *iter;
		if (rec.Type != rtAdd)
			continue;

		std::string user = U_TO_UTF8(rec.getValue(L"user"));
		if (user.empty())
			continue;

		std::string data = U_TO_UTF8(rec.getValue(L"data"));

		if (!data.empty())
		{
			// записи не дублируем
			std::string::size_type pos = docinfo.find(user);
			while (pos != std::string::npos)
			{
				std::string::size_type posEnd = docinfo.find("\n\n", pos);
				if (posEnd == std::string::npos)
					break;

				std::string user_record = docinfo.substr(pos, posEnd - pos + 2);
				if (std::string::npos != user_record.find(data))
				{
					data = "";
					break;
				}

				pos = docinfo.find(user);
			}
		}

		if (!data.empty())
		{
			docinfo += user;
			docinfo += "\n";
			docinfo += data;
			docinfo += "\n\n";

			continue;
		}

		std::wstring keyW = rec.getValue(L"key");
		if (NSFile::CFileBinary::Exists(keyW))
		{
			std::string public_key_content;
			NSFile::CFileBinary::ReadAllTextUtf8A(keyW, public_key_content);

			if (NSOpenSSL::RSA_EncryptPublic_desktop((unsigned char*)public_key_content.c_str(), U_TO_UTF8(file_password), data))
			{
				docinfo += user;
				docinfo += "\n";
				docinfo += data;
				docinfo += "\n\n";
			}
		}
	}

	if (docinfo != docinfoOld)
	{
		bool result = file.WriteAdditional(file_path, L"DocumentID", docinfo);
		if (!result)
		{
			std::cout << "error: docinfo not writed" << std::endl;
			return 1;
		}
	}

	if (PrintRecord.Type == rtPrint)
	{
		ECMACryptFile file;
		std::string docinfo = file.ReadAdditional(file_path, L"DocumentID");

		std::cout << docinfo << std::endl;
	}

	return 0;
}
