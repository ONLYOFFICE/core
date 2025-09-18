import browser from "webextension-polyfill";
import {isPopupMessageType} from "../../common/message-types.ts";
import {messageTypes} from "../../common/message-const.ts";
import {checkIsStorageLogged} from "../utils/utils.ts";

const initTaskManager = () => {
    browser.runtime.onMessage.addListener((message: unknown) => {
        if (!isPopupMessageType(message)) {
            return false;
        }
        const data = message.data;
        switch (data.type) {
            case messageTypes.WAIT_ENTER_PASSWORD: {
                return (async () => {
                    await checkIsStorageLogged();
                    return true;
                })();
            }
        }
        return false;
    });
}
export default initTaskManager;