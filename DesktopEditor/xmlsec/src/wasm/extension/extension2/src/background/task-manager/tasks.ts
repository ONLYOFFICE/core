import {openPopup} from "../utils.ts";
import {messageTypes} from "../../common/message-const.ts";
import {sendToPopup} from "../../content/messenger.ts";
// @ts-ignore
import {StorageManager} from "../../common/storage.ts";
import {ab2base64, base642ui} from "../../common/utils.ts";
import getCrypto from "../../common/crypto.ts";

export const generatePopupKeys = async () => {
    await openPopup();
    await sendToPopup({type: messageTypes.WAIT_ENTER_PASSWORD});
    return true;
}

export const selectSignKeys = async () => {
    await openPopup();
    await sendToPopup({type: messageTypes.WAIT_ENTER_PASSWORD});
    return await sendToPopup({type: messageTypes.SELECT_SIGN_KEYS});
};

export const signData = async (base64Data: string , guid: string) => {
    const keyStorage = new StorageManager();
    await keyStorage.loadKeysFromStorage();
    const keyPair = keyStorage.getKeyByGuid(guid);
    if (!keyPair) {
        throw new Error("Key pair is not found");
    }
    const data = base642ui(base64Data);
    const signData = await keyPair.sign(data);
    return ab2base64(signData);
}

export const verifyData = async (base64Data: string, base64Signature: string, guid: string) => {
    const keyStorage = new StorageManager();
    await keyStorage.loadKeysFromStorage();
    const keyPair = keyStorage.getKeyByGuid(guid);
    if (!keyPair) {
        throw new Error("Key pair is not found");
    }
    const data = base642ui(base64Data);
    const signature = base642ui(base64Signature);
    return await keyPair.verify(signature, data);
}