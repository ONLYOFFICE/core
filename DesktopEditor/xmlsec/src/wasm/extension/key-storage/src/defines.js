export const c_oAscAlgorithmType = {
	AES_GCM          : 1,
	ED25519          : 2,
	RSA_OAEP         : 3
};

export const c_oAscDigestType = {
	SHA1: 1,
	SHA256: 2,
	SHA384: 3,
	SHA512: 4
};

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
	WebPublicKey: 4,
	WebPrivateKey: 5,
	Ed25519ImportParams: 6,
};
