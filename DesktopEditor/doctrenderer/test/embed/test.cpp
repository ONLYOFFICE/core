#include "gtest/gtest.h"

#include <sstream>
#include <string>

#include "embed/Default.h"
#include "hash.h"
#include "js_internal/js_base.h"

using namespace NSJSBase;

static std::string typedArrayToString(JSSmart<CJSTypedArray> oData)
{
	std::stringstream ss;
	for (int i = 0; i < oData->getCount(); i++)
	{
		ss << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned>(oData->getData().Data[i]);
	}

	return ss.str();
}

class CHashEmbedTest : public testing::Test
{
public:
	void SetUp() override
	{
		// create context
		m_pContext = new CJSContext();

		m_pContext->Enter();
		// create default embeded objects for context
		CreateDefaults();
	}

	void TearDown() override
	{
		m_pContext->Exit();
	}

	std::string getHash(const std::string& str, CHash::HashAlgs alg)
	{
		std::string sAlg = std::to_string(static_cast<int>(alg));
		JSSmart<CJSValue> oRes1 = m_pContext->runScript(
			"var oHash = CreateEmbedObject('CHashEmbed');\n"
			"var str = '" + str + "';\n"
			"var alg = " + sAlg + ";\n"
			"var hash = oHash.hash(str, str.length, alg);");

		// Print first result
		JSSmart<CJSObject> oGlobal = m_pContext->GetGlobal();
		JSSmart<CJSTypedArray> oHash = oGlobal->get("hash")->toTypedArray();
		return typedArrayToString(oHash);
	}

	std::string getHash2(const std::string& sPassword, const std::string& sSalt, int nSpinCount, CHash::HashAlgs alg)
	{
		std::string sSpinCount = std::to_string(nSpinCount);
		std::string sAlg = std::to_string(static_cast<int>(alg));
		JSSmart<CJSValue> oRes1 = m_pContext->runScript(
			"var oHash = CreateEmbedObject('CHashEmbed');\n"
			"var sPassword = '" + sPassword + "';\n"
			"var sSalt = '" + sSalt + "';\n"
			"var nSpinCount = " + sSpinCount + ";\n"
			"var alg = " + sAlg + ";\n"
			"var hash = oHash.hash2(sPassword, sSalt, nSpinCount, alg);");

		// Print first result
		JSSmart<CJSObject> oGlobal = m_pContext->GetGlobal();
		JSSmart<CJSTypedArray> oHash = oGlobal->get("hash")->toTypedArray();
		return typedArrayToString(oHash);
	}

public:
	JSSmart<CJSContext> m_pContext;
};

// =============== HASH TESTS ===============

// MD2

TEST_F(CHashEmbedTest, hash_MD2_empty)
{
	std::string sRes = getHash("", CHash::haMD2);
	std::string sHashExp = "8350e5a3e24c153df2275c9f80692773";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_MD2_simple)
{
	std::string sRes = getHash("test", CHash::haMD2);
	std::string sHashExp = "dd34716876364a02d0195e2fb9ae2d1b";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_MD2_numbers)
{
	std::string sRes = getHash("1234567890", CHash::haMD2);
	std::string sHashExp = "38e53522a2e67fc5ea57bae1575a3107";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_MD2_long_with_whitespaces)
{
	std::string sRes = getHash("lorem ipsum dolor sit", CHash::haMD2);
	std::string sHashExp = "5083c7cafb55fc975ebd3570a42b17c7";
	EXPECT_EQ(sRes, sHashExp);
}

// MD4

TEST_F(CHashEmbedTest, hash_MD4_empty)
{
	std::string sRes = getHash("", CHash::haMD4);
	std::string sHashExp = "31d6cfe0d16ae931b73c59d7e0c089c0";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_MD4_simple)
{
	std::string sRes = getHash("abc", CHash::haMD4);
	std::string sHashExp = "a448017aaf21d8525fc10ae87aa6729d";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_MD4_mixed_with_numbers)
{
	std::string sRes = getHash("A4B5C6", CHash::haMD4);
	std::string sHashExp = "347e02b911bfd871f3c6dbaa7ec18e8e";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_MD4_long_with_whitespaces)
{
	std::string sRes = getHash("lorem ipsum dolor sit", CHash::haMD4);
	std::string sHashExp = "ce870262b108914603a976ada0190465";
	EXPECT_EQ(sRes, sHashExp);
}

// MD5

TEST_F(CHashEmbedTest, hash_MD5_empty)
{
	std::string sRes = getHash("", CHash::haMD5);
	std::string sHashExp = "d41d8cd98f00b204e9800998ecf8427e";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_MD5_simple)
{
	std::string sRes = getHash("somePasswordPhrase", CHash::haMD5);
	std::string sHashExp = "22f441c16585c77afc197135d9a182d0";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_MD5_special)
{
	std::string sRes = getHash("%!#$@12&0%#!()/", CHash::haMD5);
	std::string sHashExp = "a650f10669ec12b8649171a303c15191";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_MD5_long_with_whitespaces)
{
	std::string sRes = getHash("lorem ipsum dolor sit", CHash::haMD5);
	std::string sHashExp = "4468ef6e8e0cd5c8648c5833d1df035b";
	EXPECT_EQ(sRes, sHashExp);
}

// RMD160

TEST_F(CHashEmbedTest, hash_RMD160_empty)
{
	std::string sRes = getHash("", CHash::haRMD160);
	std::string sHashExp = "9c1185a5c5e9fc54612808977ee8f548b2258d31";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_RMD160_simple)
{
	std::string sRes = getHash("someTestWord", CHash::haRMD160);
	std::string sHashExp = "23a0e1377bf02e69418bd6799fc6aa7d920bc241";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_RMD160_numbers_and_special)
{
	std::string sRes = getHash("100%number#1&^", CHash::haRMD160);
	std::string sHashExp = "532fe6a1af143b7d6a9f0762274d5c370e7e3db7";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_RMD160_long_with_whitespaces)
{
	std::string sRes = getHash("lorem ipsum dolor sit", CHash::haRMD160);
	std::string sHashExp = "90dc47410bbc09c1ad572e0405a65077436f6522";
	EXPECT_EQ(sRes, sHashExp);
}

// SHA1

TEST_F(CHashEmbedTest, hash_SHA1_empty)
{
	std::string sRes = getHash("", CHash::haSHA1);
	std::string sHashExp = "da39a3ee5e6b4b0d3255bfef95601890afd80709";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_SHA1_simple)
{
	std::string sRes = getHash("TEST", CHash::haSHA1);
	std::string sHashExp = "984816fd329622876e14907634264e6f332e9fb3";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_SHA1_numbers_and_special)
{
	std::string sRes = getHash("100%number#1&^", CHash::haSHA1);
	std::string sHashExp = "75f5abe89b9d46fb7c202a71ecb7ad5c22c8f00e";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_SHA1_long_with_whitespaces)
{
	std::string sRes = getHash("lorem ipsum dolor sit", CHash::haSHA1);
	std::string sHashExp = "93b80998357e6ca4b1c0007ba8847810874785ef";
	EXPECT_EQ(sRes, sHashExp);
}

// SHA256

TEST_F(CHashEmbedTest, hash_SHA256_empty)
{
	std::string sRes = getHash("", CHash::haSHA256);
	std::string sHashExp = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_SHA256_simple)
{
	std::string sRes = getHash("thisIsATest", CHash::haSHA256);
	std::string sHashExp = "295c70476a0d7e57cc34f6548d4677bc8b3802cbe5e77d7fb542ab4fe027415a";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_SHA256_numbers_and_special)
{
	std::string sRes = getHash("100%number#1&^//@", CHash::haSHA256);
	std::string sHashExp = "7bdedbcf7ef0edffbc8667beb414a28fcc90e5b02d99689666aa0032ac16cabc";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_SHA256_long_with_whitespaces)
{
	std::string sRes = getHash("lorem ipsum dolor sit", CHash::haSHA256);
	std::string sHashExp = "fbf4333ef3b1c0aeb7e1d3a82978cb5d410ebc13d5512d894a01fdcd623516ad";
	EXPECT_EQ(sRes, sHashExp);
}

// SHA384

TEST_F(CHashEmbedTest, hash_SHA384_empty)
{
	std::string sRes = getHash("", CHash::haSHA384);
	std::string sHashExp = "38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_SHA384_simple)
{
	std::string sRes = getHash("simple", CHash::haSHA384);
	std::string sHashExp = "309435df62b71bc1c9cdce1a865e397dc94fc7cc19ebf03e8c972a01f3106c187b90787861a8fe8ebad632c1ccc5a6b7";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_SHA384_numbers_and_special)
{
	std::string sRes = getHash("100%number#1&^//@", CHash::haSHA384);
	std::string sHashExp = "af3c0c66f44eba9338e7ff0ec53d678aa80fb8ba1677be22cc0a70c896e3f18df5ff1dba925ef9ea94ec5609f438b37b";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_SHA384_long_with_whitespaces)
{
	std::string sRes = getHash("lorem ipsum dolor sit", CHash::haSHA384);
	std::string sHashExp = "cb73fc5737123ec4bfad644b51646600b57c0cfe70fa59bbf2d2d422be4b5abbd8c144aeb54570008c0a38732afaea7e";
	EXPECT_EQ(sRes, sHashExp);
}

// SHA512

TEST_F(CHashEmbedTest, hash_SHA512_empty)
{
	std::string sRes = getHash("", CHash::haSHA512);
	std::string sHashExp = "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_SHA512_simple)
{
	std::string sRes = getHash("bestSha", CHash::haSHA512);
	std::string sHashExp = "76547bbaf925128849fbbbae49d2cc22e7244495c15399fd27534a9ce9bd62f2eaf2f2aa6f6aed8ecd281260739ad755e241860b4adcf3440fabd34dad89de7c";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_SHA512_numbers_and_special)
{
	std::string sRes = getHash("100%number#1&^//@", CHash::haSHA512);
	std::string sHashExp = "c28516318aa24f37b5e6448e49f747d95eaaea726498d1e0a017be8b289c73598363cd622492113b65a0b92b37250482d1511fa7db9e7012243e57950f04dc93";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_SHA512_long_with_whitespaces)
{
	std::string sRes = getHash("lorem ipsum dolor sit", CHash::haSHA512);
	std::string sHashExp = "48a6898c48655bbd42b588c40ec3ef86fd0bf30c981fd543a03e21e176af1cb5ff2c4ae9a012b924a9696b9ac615018c62c73786cbbc9ea5b5154fd9d69134d2";
	EXPECT_EQ(sRes, sHashExp);
}

// WHIRLPOOL

TEST_F(CHashEmbedTest, hash_WHIRLPOOL_empty)
{
	std::string sRes = getHash("", CHash::haWHIRLPOOL);
	std::string sHashExp = "19fa61d75522a4669b44e39c1d2e1726c530232130d407f89afee0964997f7a73e83be698b288febcf88e3e03c4f0757ea8964e59b63d93708b138cc42a66eb3";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_WHIRLPOOL_simple)
{
	std::string sRes = getHash("wHiRlPooL", CHash::haWHIRLPOOL);
	std::string sHashExp = "35f76e47d8ac40a1431c9d26df5c96fba1ae04a2f53d3a8775261f5133b4f68c95f3bd710a5109def44c659f8407b88016ccba43ba4f974b8d003d429298c79d";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_WHIRLPOOL_numbers_and_special)
{
	std::string sRes = getHash("100%number#1&^//@", CHash::haWHIRLPOOL);
	std::string sHashExp = "6b65f878b4b5292253c40544b572bf381b981d9cdc2d4864c23ac706ba8bf5306c78456487ff7c4f3b3d0d91f29ec6599f1f6b578a1007e5b7029d68330ca31b";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash_WHIRLPOOL_long_with_whitespaces)
{
	std::string sRes = getHash("lorem ipsum dolor sit", CHash::haWHIRLPOOL);
	std::string sHashExp = "6bc062ea83003d44c2dbff2f9cbd2385806f6450171410fca4f189ed5c44726c4dc0330030af2c647082975b41f1c83bcc0722576d7e8cb2aabb5ff0ba38aaa9";
	EXPECT_EQ(sRes, sHashExp);
}


// =============== HASH2 TESTS ===============

// MD2

TEST_F(CHashEmbedTest, hash2_MD2_empty_both)
{
	std::string sRes = getHash2("", "", 0, CHash::haMD2);
	std::string sHashExp = "8350e5a3e24c153df2275c9f80692773";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_MD2_empty_salt)
{
	std::string sRes = getHash2("test", "", 0, CHash::haMD2);
	std::string sHashExp = "4b3991628dcc0d99c21a3e16bae371a1";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_MD2_empty_password)
{
	std::string sRes = getHash2("", "test", 0, CHash::haMD2);
	std::string sHashExp = "f8b4e92bbb2cfb8978ee457ff2e1c52c";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_MD2_normal_zero_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw==", 0, CHash::haMD2);
	std::string sHashExp = "4910f7ccbf97e8692cec700c81be028a";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_MD2_normal_one_spin)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw", 1, CHash::haMD2);
	std::string sHashExp = "dad6d472390d1d4b8e24a6ab6e79ee09";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_MD2_normal_large_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw", 100000, CHash::haMD2);
	std::string sHashExp = "9e32a6c3776cbe9649799bbb6ef4a30b";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_MD2_special_salt_symbols)
{
	std::string sRes	 = getHash2("test", "yrGivlyCImiWnryRee1OJw+===.@#!", 42, CHash::haMD2);
	std::string sHashExp = getHash2("test", "yrGivlyCImiWnryRee1OJw+",		  42, CHash::haMD2);
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_MD2_long_password_with_special_symbols)
{
	std::string sRes = getHash2("l#*Rem / .Ip$uM. +dol@r_ S!t", "ZUdHa+D8F/OAKP3I7ssUnQ==", 100, CHash::haMD2);
	std::string sHashExp = "6d5a4f44884908bf330a89c2e0b4dfb1";
	EXPECT_EQ(sRes, sHashExp);
}

// MD4

TEST_F(CHashEmbedTest, hash2_MD4_empty_both)
{
	std::string sRes = getHash2("", "", 0, CHash::haMD4);
	std::string sHashExp = "31d6cfe0d16ae931b73c59d7e0c089c0";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_MD4_normal_zero_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw==", 0, CHash::haMD4);
	std::string sHashExp = "c7604219b6ebceabecc90e812b0a79d5";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_MD4_normal_large_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw", 100000, CHash::haMD4);
	std::string sHashExp = "0f159ffad2f023cfda82806e0cc0f567";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_MD4_long_password_with_special_symbols)
{
	std::string sRes = getHash2("l#*Rem / .Ip$uM. +dol@r_ S!t", "ZUdHa+D8F/OAKP3I7ssUnQ==", 100, CHash::haMD4);
	std::string sHashExp = "294c29798c2b737387b933e93f449bce";
	EXPECT_EQ(sRes, sHashExp);
}

// MD5

TEST_F(CHashEmbedTest, hash2_MD5_empty_both)
{
	std::string sRes = getHash2("", "", 0, CHash::haMD5);
	std::string sHashExp = "d41d8cd98f00b204e9800998ecf8427e";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_MD5_normal_zero_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw==", 0, CHash::haMD5);
	std::string sHashExp = "32549585b3be9154133ffad540c5c64e";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_MD5_normal_large_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw", 100000, CHash::haMD5);
	std::string sHashExp = "e3f91c932542ddc2665e83b084e976d5";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_MD5_long_password_with_special_symbols)
{
	std::string sRes = getHash2("l#*Rem / .Ip$uM. +dol@r_ S!t", "ZUdHa+D8F/OAKP3I7ssUnQ==", 100, CHash::haMD5);
	std::string sHashExp = "eff44f503d10050fa4d3021b7a8bf342";
	EXPECT_EQ(sRes, sHashExp);
}

// RMD160

TEST_F(CHashEmbedTest, hash2_RMD160_empty_both)
{
	std::string sRes = getHash2("", "", 0, CHash::haRMD160);
	std::string sHashExp = "9c1185a5c5e9fc54612808977ee8f548b2258d31";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_RMD160_normal_zero_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw==", 0, CHash::haRMD160);
	std::string sHashExp = "4bf3966337fbff161ff716fc84ec6d4d07b3a97a";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_RMD160_normal_large_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw", 100000, CHash::haRMD160);
	std::string sHashExp = "30c5d71c1b5155828201fb4eacfc55b7b7d4659c";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_RMD160_long_password_with_special_symbols)
{
	std::string sRes = getHash2("l#*Rem / .Ip$uM. +dol@r_ S!t", "ZUdHa+D8F/OAKP3I7ssUnQ==", 100, CHash::haRMD160);
	std::string sHashExp = "16b740428b7b54ac5731e69b935c017dcc46f41e";
	EXPECT_EQ(sRes, sHashExp);
}

// SHA1

TEST_F(CHashEmbedTest, hash2_SHA1_empty_both)
{
	std::string sRes = getHash2("", "", 0, CHash::haSHA1);
	std::string sHashExp = "da39a3ee5e6b4b0d3255bfef95601890afd80709";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_SHA1_normal_zero_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw==", 0, CHash::haSHA1);
	std::string sHashExp = "8ade9b306c150b75355d2c41eccbaaeade367c68";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_SHA1_normal_large_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw", 100000, CHash::haSHA1);
	std::string sHashExp = "f22e440294cf8d3a17762e1b7e3c85da7653bfc0";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_SHA1_long_password_with_special_symbols)
{
	std::string sRes = getHash2("l#*Rem / .Ip$uM. +dol@r_ S!t", "ZUdHa+D8F/OAKP3I7ssUnQ==", 100, CHash::haSHA1);
	std::string sHashExp = "baef83ca25d011bc8abef8db4eeb7797be882dcf";
	EXPECT_EQ(sRes, sHashExp);
}

// SHA256

TEST_F(CHashEmbedTest, hash2_SHA256_empty_both)
{
	std::string sRes = getHash2("", "", 0, CHash::haSHA256);
	std::string sHashExp = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_SHA256_normal_zero_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw==", 0, CHash::haSHA256);
	std::string sHashExp = "49e56631385352e25bd1832c976c1cb2d16fc890c08b0afa5330ac7b357e4dd2";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_SHA256_normal_large_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw", 100000, CHash::haSHA256);
	std::string sHashExp = "dfb1016fc7df9962c139649f274a013fdcb3acfe58e45a0158b343b682c6be66";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_SHA256_long_password_with_special_symbols)
{
	std::string sRes = getHash2("l#*Rem / .Ip$uM. +dol@r_ S!t", "ZUdHa+D8F/OAKP3I7ssUnQ==", 100, CHash::haSHA256);
	std::string sHashExp = "5a2a9b61a11fd847428817c5e3f801f0890ecbba4c7ff30d6c7148dab5350707";
	EXPECT_EQ(sRes, sHashExp);
}

// SHA384

TEST_F(CHashEmbedTest, hash2_SHA384_empty_both)
{
	std::string sRes = getHash2("", "", 0, CHash::haSHA384);
	std::string sHashExp = "38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_SHA384_normal_zero_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw==", 0, CHash::haSHA384);
	std::string sHashExp = "30acca7905d036c16212d93e910677e35abd7075c83686ef068da98cac2c3e7bd53a00f7630c618a8612a71319c70a69";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_SHA384_normal_large_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw", 100000, CHash::haSHA384);
	std::string sHashExp = "b67220f2f7e058067f37e467fd4022fccdcff9068a90721128cff07161be3d22e4b8f7b7f2a92e2799df361d2fad581f";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_SHA384_long_password_with_special_symbols)
{
	std::string sRes = getHash2("l#*Rem / .Ip$uM. +dol@r_ S!t", "ZUdHa+D8F/OAKP3I7ssUnQ==", 100, CHash::haSHA384);
	std::string sHashExp = "e44475f25135de740b04ac297987e58e65aa1eb87dd6a3825636c82e160f9bd504ce59073777b16f152e35be8086f491";
	EXPECT_EQ(sRes, sHashExp);
}

// SHA512

TEST_F(CHashEmbedTest, hash2_SHA512_empty_both)
{
	std::string sRes = getHash2("", "", 0, CHash::haSHA512);
	std::string sHashExp = "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_SHA512_normal_zero_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw==", 0, CHash::haSHA512);
	std::string sHashExp = "4a1e8ca6c732f73083a598d4a125b6b86968b879d67f9220b745f81319649d97bf7ed9560da0f8a84b6950902f1dd8357c215624d0e7681724c81b3d402042eb";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_SHA512_normal_large_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw", 100000, CHash::haSHA512);
	std::string sHashExp = "79d7f29cbe4bf98ab6844bf2f1e1c98a9ac9f402fe97ae19cf625fee2baa8eacf51f790c2b9a6318685e36c8662b9deaca8096715e2b1c7cc746177b1a36da12";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_SHA512_long_password_with_special_symbols)
{
	std::string sRes = getHash2("l#*Rem / .Ip$uM. +dol@r_ S!t", "ZUdHa+D8F/OAKP3I7ssUnQ==", 100, CHash::haSHA512);
	std::string sHashExp = "306fb41c2ac84cbda4a0cd8b504cd6e642ad24219d3abbbb8371c02e34768f893f32030c99ee26d6f3dfffbf644f5cf864d1054d0ff9c4d0fc35b70638561817";
	EXPECT_EQ(sRes, sHashExp);
}

// WHIRLPOOL

TEST_F(CHashEmbedTest, hash2_WHIRLPOOL_empty_both)
{
	std::string sRes = getHash2("", "", 0, CHash::haWHIRLPOOL);
	std::string sHashExp = "19fa61d75522a4669b44e39c1d2e1726c530232130d407f89afee0964997f7a73e83be698b288febcf88e3e03c4f0757ea8964e59b63d93708b138cc42a66eb3";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_WHIRLPOOL_normal_zero_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw==", 0, CHash::haWHIRLPOOL);
	std::string sHashExp = "4579bac0b569f64f8d6c5787b1a3bf4b98175cffce14b12e7571729622845978f5078fb43816f7ea48d1342f1c2440acf8fabe49c98a7e493ceb363a180031e9";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_WHIRLPOOL_normal_large_spin_count)
{
	std::string sRes = getHash2("test", "yrGivlyCImiWnryRee1OJw", 100000, CHash::haWHIRLPOOL);
	std::string sHashExp = "adbe7de286ebede2d81b5c184cb31753de27fd7212895016d9cad0baadfae9f98e2452b29d8425e35d140d33bde2d9c45f7251beddd551bfb381fbced0275908";
	EXPECT_EQ(sRes, sHashExp);
}

TEST_F(CHashEmbedTest, hash2_WHIRLPOOL_long_password_with_special_symbols)
{
	std::string sRes = getHash2("l#*Rem / .Ip$uM. +dol@r_ S!t", "ZUdHa+D8F/OAKP3I7ssUnQ==", 100, CHash::haWHIRLPOOL);
	std::string sHashExp = "eab4cefa2c1479d71b7b8c0416efb2890a592dc293692117270d579f4d72aee4d166d750ab601066f9e88fd78a83d7981057e6b46550bd00bd6afec9ecf413ae";
	EXPECT_EQ(sRes, sHashExp);
}
