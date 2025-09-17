import CWebCrypto from "./crypto.ts";

const generateCipherKeys = async () => {
    const crypto = new CWebCrypto();
    const keyPair = await crypto.generateKey({ name: "X25519" }, true, ["deriveKey"]);
    return keyPair;
};

const encrypt

