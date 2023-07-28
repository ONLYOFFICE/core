#include "gtest/gtest.h"
#include "../lib/include/osign.h"
#include "../../../../common/File.h"

class COSignTest : public testing::Test
{
public:
	std::wstring WorkiDirectory;
public:
	virtual void SetUp() override
	{
		WorkiDirectory = NSFile::GetProcessDirectory();
	}

	virtual void TearDown() override
	{
	}
};

// aes шифрование
TEST_F(COSignTest, crypt_storage_aes_gcm_random_password)
{
	// создаем случайный пароль длиной 100 символов
	OSign::CStorageBuffer oPassword = OSign::Crypt::GeneratePassword(100);
	EXPECT_EQ(100, oPassword.GetLength());

	// буфер для шифрования
	std::string sDataCrypt = "Hello, world!";
	OSign::CStorageBuffer oBuffer;
	oBuffer.Add((const unsigned char*)sDataCrypt.c_str(), sDataCrypt.length());

	// шифруем буфер, соль генерируется случайным образом и дописывается в результат
	OSign::CStorageBuffer oCryptBuffer = OSign::Crypt::Encrypt(oBuffer, oPassword);

	// дешифруем буфер
	OSign::CStorageBuffer oDecryptBuffer = OSign::Crypt::Decrypt(oCryptBuffer, oPassword);

	// сравниваем с исходником
	std::string sDecryptData((char*)oDecryptBuffer.GetData(), oDecryptBuffer.GetLength());
	EXPECT_EQ(sDecryptData, sDataCrypt);
}

TEST_F(COSignTest, crypt_storage_aes_gcm_string_password)
{
	// создаем случайный пароль из строки
	std::string sPassword = "password";
	OSign::CStorageBuffer oPassword;
	oPassword.Add((unsigned char*)sPassword.c_str(), sPassword.length());

	// буфер для шифрования
	std::string sDataCrypt = "Hello, world!";
	OSign::CStorageBuffer oBuffer;
	oBuffer.Add((const unsigned char*)sDataCrypt.c_str(), sDataCrypt.length());

	// шифруем буфер, соль генерируется случайным образом и дописывается в результат
	OSign::CStorageBuffer oCryptBuffer = OSign::Crypt::Encrypt(oBuffer, oPassword);

	// дешифруем буфер
	OSign::CStorageBuffer oDecryptBuffer = OSign::Crypt::Decrypt(oCryptBuffer, oPassword);

	// сравниваем с исходником
	std::string sDecryptData((char*)oDecryptBuffer.GetData(), oDecryptBuffer.GetLength());
	EXPECT_EQ(sDecryptData, sDataCrypt);
}

// сериализация буфера
TEST_F(COSignTest, serialize_buffer_string)
{
	std::string sBuffer = "Hello, world";
	OSign::CStorageBuffer oBuffer;
	oBuffer.Add((unsigned char*)sBuffer.c_str(), sBuffer.length());

	std::string sBase64 = oBuffer.ToBase64();
	OSign::CStorageBuffer oDecodeBuffer;
	oDecodeBuffer.FromBase64(sBase64);

	std::string sDecodeBuffer((char*)oDecodeBuffer.GetData(), oDecodeBuffer.GetLength());

	EXPECT_EQ(sBuffer, sDecodeBuffer);
}

// подпись
TEST_F(COSignTest, sign_buffer_string)
{
	// генерируем новый сертификат
	OSign::CCertificate* pCert = new OSign::CCertificate();
	pCert->Generate();

	// буфер для подписи
	std::string sDataBuffer = "Hello, world!";
	OSign::CStorageBuffer oBuffer;
	oBuffer.Add((const unsigned char*)sDataBuffer.c_str(), sDataBuffer.length());

	// подписываем
	OSign::CStorageBuffer oBufferSign = pCert->Sign(oBuffer);
	// проверяем
	bool bIsValid = pCert->Verify(oBuffer, oBufferSign);
	delete pCert;

	EXPECT_EQ(bIsValid, true);
}

// тест сериализации стораджа через подпись/верификацию
TEST_F(COSignTest, serialize_storage_by_sign)
{
	// генерируем новый сертификат
	OSign::CCertificate* pCert = new OSign::CCertificate();
	pCert->Generate();

	// создаем сторадж и добавлем сертификат (сторадж сам следит за удалением сертификата)
	OSign::CStorage oStorage;
	oStorage.Add(pCert);

	// сохраняем сторадж
	OSign::CStorageBuffer oStorageBuffer;
	oStorage.Save(&oStorageBuffer);

	OSign::CStorage oStorageLoad;
	oStorageLoad.Load(&oStorageBuffer);

	// буфер для подписи
	std::string sDataBuffer = "Hello, world!";
	OSign::CStorageBuffer oBuffer;
	oBuffer.Add((const unsigned char*)sDataBuffer.c_str(), sDataBuffer.length());
	// подписываем
	OSign::CStorageBuffer oBufferSign = pCert->Sign(oBuffer);

	OSign::CCertificate* pCert2 = oStorage.Get(0);
	bool bIsValid = pCert2->Verify(oBuffer, oBufferSign);

	EXPECT_EQ(bIsValid, true);
}

// тест сериализации стораджа через подпись/верификацию
TEST_F(COSignTest, serialize_storage_by_property)
{
	// генерируем новый сертификат с настройками
	std::map<std::wstring, std::wstring> properties;
	properties.insert(std::make_pair(OSign::Properties::Email, L"sign@onlyoffice.com"));
	properties.insert(std::make_pair(OSign::Properties::Phone, L"+00000000000"));
	std::wstring sNameTest = L"NameTest";
	std::wstring sValueTest = L"ValueTest";
	properties.insert(std::make_pair(sNameTest, sValueTest));

	OSign::CCertificate* pCert = new OSign::CCertificate();
	pCert->Generate(properties);

	// создаем сторадж и добавлем сертификат (сторадж сам следит за удалением сертификата)
	OSign::CStorage oStorage;
	oStorage.Add(pCert);

	// сохраняем сторадж
	OSign::CStorageBuffer oStorageBuffer;
	oStorage.Save(&oStorageBuffer);

	OSign::CStorage oStorageLoad;
	oStorageLoad.Load(&oStorageBuffer);

	std::wstring sValue = L"";
	OSign::CCertificate* pCert2 = oStorage.Get(0);

	std::map<std::wstring, std::wstring> mapProps = pCert2->GetProperties();
	std::map<std::wstring, std::wstring>::const_iterator iterFind = mapProps.find(sNameTest);
	if (iterFind != mapProps.end())
		sValue = iterFind->second;

	EXPECT_EQ(sValueTest, sValue);
}
