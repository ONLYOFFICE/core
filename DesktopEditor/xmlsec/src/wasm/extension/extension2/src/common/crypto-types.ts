type CryptoData = BufferSource;

type SignParameters = Parameters<SubtleCrypto["sign"]>
type SignAlgorithm = SignParameters[0];

type DigestParameters = Parameters<SubtleCrypto["digest"]>
type DigestAlgorithm = DigestParameters[0];

type EncryptParameters = Parameters<SubtleCrypto["encrypt"]>
type EncryptAlgorithm = EncryptParameters[0];

type GenerateKeyParameters = Parameters<SubtleCrypto["generateKey"]>
type GenerateKeyAlgorithm = GenerateKeyParameters[0];
type GenerateKeyUsages = GenerateKeyParameters[2];

export type {CryptoData, SignAlgorithm, DigestAlgorithm, EncryptAlgorithm, GenerateKeyAlgorithm, GenerateKeyUsages};