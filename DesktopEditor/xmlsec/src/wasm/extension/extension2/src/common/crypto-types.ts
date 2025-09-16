type CryptoData = BufferSource;

type SignParameters = Parameters<SubtleCrypto["sign"]>
type SignAlgorithm = SignParameters[0];

type DigestParameters = Parameters<SubtleCrypto["digest"]>
type DigestAlgorithm = DigestParameters[0];

type EncryptParameters = Parameters<SubtleCrypto["encrypt"]>
type EncryptAlgorithm = EncryptParameters[0];

export type {CryptoData, SignAlgorithm, DigestAlgorithm, EncryptAlgorithm};