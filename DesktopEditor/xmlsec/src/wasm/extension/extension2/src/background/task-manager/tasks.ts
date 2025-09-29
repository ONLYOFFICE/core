import {openPopup} from "../utils.ts";
import {messageTypes} from "../../common/message-const.ts";
import {sendToPopup} from "../../content/messenger.ts";
import KeyStorage from "../../common/key-storage.ts";
import {base642ui} from "../../common/utils.ts";
import getCrypto from "../../common/crypto.ts";

export const generatePopupKeys = async () => {
    await openPopup();
    await sendToPopup({type: messageTypes.WAIT_ENTER_PASSWORD});
    return {data: true};
}

export const selectSignKeys = async () => {
    await openPopup();
    await sendToPopup({type: messageTypes.WAIT_ENTER_PASSWORD});
    return await sendToPopup({type: messageTypes.SELECT_SIGN_KEYS});
};

export const signData = async (base64Data: string , guid: string) => {
    const keyStorage = new KeyStorage();
    await keyStorage.loadKeysFromStorage();
    const keyPair = keyStorage.getKeyPairByGuid(guid);
    if (!keyPair) {
        throw new Error("Key pair is not found");
    }
    const data = base642ui(base64Data);
    const crypto = getCrypto();
    return crypto.sign(keyPair.privateKey, data);
}