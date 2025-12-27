export const c_oAscDigestType = {
	SHA1: 1,
	SHA256: 2,
	SHA384: 3,
	SHA512: 4
};

export const c_oAscCryptoDigestType = {};
c_oAscCryptoDigestType[c_oAscDigestType.SHA1] = "SHA-1";
c_oAscCryptoDigestType[c_oAscDigestType.SHA256] = "SHA-256";
c_oAscCryptoDigestType[c_oAscDigestType.SHA384] = "SHA-384";
c_oAscCryptoDigestType[c_oAscDigestType.SHA512] = "SHA-512";

export const c_oAscExportKeyFormat = {
	pkcs8: "pkcs8",
	spki: "spki",
	raw: "raw"
};

export const c_oAscKeyStorageType = {
	NoType: 0,
	WebSymmetricKey: 1,
	WebSignKeyPair: 2,
	WebEncryptKeyPair: 3,
	Ed25519ImportParams: 6,
	EncryptData: 7,
	RSAOAEPImportParams: 8,
	RSAOAEPKeyGenParams: 9,
	Ed25519KeyGenParams: 10,
	AesGCMCryptoParams: 11,
	AesGCMKeyGenParams: 12,
	PBKDF2Params: 14,
	WebPrivateSignKey: 15,
	WebPrivateEncryptKey: 16,
	WebPublicSignKey: 17,
	WebPublicEncryptKey: 18,
	RsaOAEPCryptoParams: 19
};

export const c_oAscCryptoRsaType = {};
c_oAscCryptoRsaType[c_oAscKeyStorageType.RSAOAEPKeyGenParams] = "RSA-OAEP";

export const c_oAscCryptoAesType = {};
c_oAscCryptoAesType[c_oAscKeyStorageType.AesGCMKeyGenParams] = "AES-GCM";