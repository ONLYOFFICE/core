import browser from "webextension-polyfill";
import {isPopupMessageType} from "../../common/message-types.ts";
import {messageTypes} from "../../common/message-const.ts";
import {isStorageLogged, selectSignKeys} from "./tasks.ts";
import {useEffect, useRef, useState} from "react";

type TSelectKeysPromise = {
  resolve: (guid: string) => void;
  messageId: typeof messageTypes.SELECT_SIGN_KEYS
};
type TPromiseArgs = {
    reject: (error: string) => void;
} & (TSelectKeysPromise);
export type TPromiseRef = (newResolve: TPromiseArgs["resolve"], newReject: TPromiseArgs["reject"], id: TPromiseArgs["messageId"]) => void;
export const useTaskManager = () => {
    const [location, setLocation] = useState("");
    const promiseRef = useRef<TPromiseArgs | null>(null);
    const setPromiseRef: TPromiseRef = (newResolve, newReject, id) => {
        if (promiseRef.current) {
            promiseRef.current.reject("Another task has been selected");
        }
        promiseRef.current = {resolve: newResolve, reject: newReject, messageId: id};
    }
    useEffect(() => {
        const listener = (message: unknown) => {
            if (!isPopupMessageType(message)) {
                return false;
            }
            const data = message.data;
            switch (data.type) {
                case messageTypes.WAIT_ENTER_PASSWORD: {
                    return isStorageLogged();
                }
                case messageTypes.SELECT_SIGN_KEYS: {
                    return selectSignKeys(setLocation, setPromiseRef);
                }
            }
            return false;
        };
        browser.runtime.onMessage.addListener(listener);
        return () => {
            browser.runtime.onMessage.removeListener(listener);
        }
    }, []);
    return {location, setLocation, promiseRef};
};