export type CryptoData = BufferSource;

type SignParameters = Parameters<SubtleCrypto["sign"]>
export type SignAlgorithm = SignParameters[0];

type DigestParameters = Parameters<SubtleCrypto["digest"]>
export type DigestAlgorithm = DigestParameters[0];

type EncryptParameters = Parameters<SubtleCrypto["encrypt"]>
export type EncryptAlgorithm = EncryptParameters[0];

type GenerateKeyParameters = Parameters<SubtleCrypto["generateKey"]>
export type GenerateKeyAlgorithm = GenerateKeyParameters[0];
export type GenerateKeyUsages = GenerateKeyParameters[2];