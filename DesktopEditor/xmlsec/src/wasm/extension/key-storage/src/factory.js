import {c_oAscKeyStorageType} from "./defines";
import {WebEncryptKeyPair, WebPrivateKey, WebPublicKey, WebSignKeyPair, WebSymmetricKey} from "./keys";

export const c_oAscObjectFactory = {};
c_oAscObjectFactory[c_oAscKeyStorageType.WebPublicKey] = WebPublicKey;
c_oAscObjectFactory[c_oAscKeyStorageType.WebPrivateKey] = WebPrivateKey;
c_oAscObjectFactory[c_oAscKeyStorageType.WebSymmetricKey] = WebSymmetricKey;
c_oAscObjectFactory[c_oAscKeyStorageType.WebSignKeyPair] = WebSignKeyPair;
c_oAscObjectFactory[c_oAscKeyStorageType.WebEncryptKeyPair] = WebEncryptKeyPair;
