import {useState, useEffect} from 'react'
import browser from "webextension-polyfill";
import Login from "./Login.tsx";
import {isMessageType} from "../../common/message-types.ts";
import {messageListeners, messageTypes} from "../../common/message-const.ts";

const getStorageMasterPassword = async () => {
    const masterPassword = await browser.storage.local.get('masterPassword');
    if (masterPassword && typeof masterPassword.masterPassword === 'string') {
        return masterPassword.masterPassword;
    }
    return null;
}
const setStorageMasterPassword = (masterPassword: string) => {
    browser.storage.local.set({masterPassword});
}
const checkIsStorageLogged = async () => {
    const masterPassword = await getStorageMasterPassword();
    if (masterPassword) {
        return true;
    }
    const masterPasswordPromise = new Promise((resolve) => {
        const onChangedHandler = (change: Record<string, unknown>) => {
            if (change.masterPassword) {
                resolve(true);
            }
        }
        browser.storage.local.onChanged.addListener(onChangedHandler);
    });
    return await masterPasswordPromise;
};
export default function App() {
    const [localMasterPassword, setLocalMasterPassword] = useState<string | null>(null);
    useEffect(() => {
       (async () => {
           const storageMasterPassword = await getStorageMasterPassword();
           setLocalMasterPassword(storageMasterPassword);

        })();
        const port = browser.runtime.connect({ name: "popup" });
        port.postMessage({ opened: true });
        browser.runtime.onMessage.addListener((message: unknown) => {
            if (!isMessageType(message) || message.listener !== messageListeners.popup) {
                return true;
            }
            switch (message.type) {
                case messageTypes.GENERATE_KEYS: {
                    return (async () => {
                        await checkIsStorageLogged();
                        return true;
                    })();
                }
            }
            return true;
        });
    }, []);

    const handleSubmitMasterPassword = (e: React.FormEvent, masterPassword: string) => {
        e.preventDefault();
        setStorageMasterPassword(masterPassword);
        setLocalMasterPassword(masterPassword);
    }
    const isLoggedIn = localMasterPassword !== null;
    return (
        <>
            {isLoggedIn ? <div>Hello world {localMasterPassword}</div> : <Login handleSubmitMasterPassword={handleSubmitMasterPassword} />}
        </>
    )
}