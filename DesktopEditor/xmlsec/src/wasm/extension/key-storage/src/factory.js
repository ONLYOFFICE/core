import {c_oAscKeyStorageType} from "./defines";
import {EncryptData, WebEncryptKeyPair, WebPrivateKey, WebPublicKey, WebSignKeyPair, WebSymmetricKey} from "./keys";
import {
	AesGcmCryptoParams, AesGcmKeyGenParams,
	Ed25519ImportParams,
	Ed25519KeyGenParams,
	RsaOAEPImportParams,
	RsaOAEPKeyGenParams
} from "./params";

export const c_oAscObjectFactory = {};
c_oAscObjectFactory[c_oAscKeyStorageType.WebSymmetricKey] = WebSymmetricKey;
c_oAscObjectFactory[c_oAscKeyStorageType.WebSignKeyPair] = WebSignKeyPair;
c_oAscObjectFactory[c_oAscKeyStorageType.WebEncryptKeyPair] = WebEncryptKeyPair;
c_oAscObjectFactory[c_oAscKeyStorageType.WebPublicKey] = WebPublicKey;
c_oAscObjectFactory[c_oAscKeyStorageType.WebPrivateKey] = WebPrivateKey;
c_oAscObjectFactory[c_oAscKeyStorageType.Ed25519ImportParams] = Ed25519ImportParams;
c_oAscObjectFactory[c_oAscKeyStorageType.EncryptData] = EncryptData;
c_oAscObjectFactory[c_oAscKeyStorageType.RSAOAEPImportParams] = RsaOAEPImportParams;
c_oAscObjectFactory[c_oAscKeyStorageType.RSAOAEPKeyGenParams] = RsaOAEPKeyGenParams;
c_oAscObjectFactory[c_oAscKeyStorageType.Ed25519KeyGenParams] = Ed25519KeyGenParams;
c_oAscObjectFactory[c_oAscKeyStorageType.AesGCMCryptoParams] = AesGcmCryptoParams;
c_oAscObjectFactory[c_oAscKeyStorageType.AesGCMKeyGenParams] = AesGcmKeyGenParams;
