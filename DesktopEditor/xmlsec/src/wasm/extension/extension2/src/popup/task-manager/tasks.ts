import {checkIsStorageLogged, getChangedDefaultGuid} from "../utils/utils.ts";

export const isStorageLogged = async () => {
    await checkIsStorageLogged();
    return true;
};

export const selectSignKeys = async () => {
    return getChangedDefaultGuid();
}