import {openPopup} from "../utils.ts";
import {messageTypes} from "../../common/message-const.ts";
import {sendToPopup} from "../../content/messenger.ts";

export const generatePopupKeys = async () => {
    await openPopup();
    await sendToPopup({type: messageTypes.WAIT_ENTER_PASSWORD});
    return {data: true};
}