import browser from "webextension-polyfill";
import {messageTypes} from "../../common/message-const.ts";
import {generatePopupKeys, selectSignKeys, signData, verifyData} from "./tasks.ts";
import {isBackgroundMessageType} from "../../common/message-types.ts";

const initTaskManager = () => {
    browser.runtime.onMessage.addListener((message: unknown) => {
        if (!isBackgroundMessageType(message)) {
            return false;
        }
        const data = message.data;
        switch (data.type) {
            case messageTypes.GENERATE_KEYS: {
                return generatePopupKeys();
            }
            case messageTypes.SELECT_SIGN_KEYS: {
                return selectSignKeys();
            }
            case messageTypes.SIGN_DATA: {
                return signData(data.base64Data, data.guid);
            }
            case messageTypes.VERIFY_DATA: {
                return verifyData(data.base64Data, data.base64Signature, data.guid);
            }
            default:
                return false;
        }
    });
}

export default initTaskManager;