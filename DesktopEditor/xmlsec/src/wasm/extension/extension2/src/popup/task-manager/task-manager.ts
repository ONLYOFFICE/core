import browser from "webextension-polyfill";
import {isPopupMessageType} from "../../common/message-types.ts";
import {messageTypes} from "../../common/message-const.ts";
import {isStorageLogged, selectSignKeys} from "./tasks.ts";

const initTaskManager = () => {
    browser.runtime.onMessage.addListener((message: unknown) => {
        if (!isPopupMessageType(message)) {
            return false;
        }
        const data = message.data;
        switch (data.type) {
            case messageTypes.WAIT_ENTER_PASSWORD: {
                return isStorageLogged();
            }
            case messageTypes.SELECT_SIGN_KEYS: {
                return selectSignKeys();
            }
        }
        return false;
    });
}
export default initTaskManager;