import {checkIsStorageLogged} from "../utils/utils.ts";
import {locations} from "../utils/locations.ts";
import {messageTypes} from "../../common/message-const.ts";
import type {TPromiseRef} from "./task-manager.ts";

export const isStorageLogged = async () => {
    await checkIsStorageLogged();
    return true;
};

export const selectSignKeys = (setNavigation: (location: string) => void, setPromiseRef: TPromiseRef): Promise<string> => {
    setNavigation(locations.selectKeys);
    return new Promise((resolve, reject) => {
       setPromiseRef(resolve, reject, messageTypes.SELECT_SIGN_KEYS);
    });
}