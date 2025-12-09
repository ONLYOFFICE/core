import {useState, useEffect} from 'react'
import Login from "../login/login.tsx";
import {getStorageMasterPassword, initCheckOpenedPopup, setStorageMasterPassword} from "../../../utils/utils.ts";
import {useTaskManager} from "../../../task-manager/task-manager.ts";
import {Dashboard} from "../dashboard/dashboard.tsx";
// @ts-ignore
import {KeyStorage} from "../../../../../../key-storage/key-storage.js";
import {KeyPair, KeyUsages} from "../../../../common/keys/keys.ts";
import getCrypto from "../../../../common/crypto.ts";
import {Ed25519KeyGenParams} from "../../../../common/keys/params.ts";
import {ChangePasswordPage} from "../change-password/change-password.tsx";
import {locations} from "../../../utils/locations.ts";
import SelectKeysPage from "../select-keys/select-keys.tsx";
import {messageTypes} from "../../../../common/message-const.ts";
const keyStorage = new KeyStorage();
const generateKeys = async () => {
    const crypto = getCrypto();
    const cryptoPair = await crypto.generateKey(new Ed25519KeyGenParams(), new KeyUsages(false,true));
    if (cryptoPair instanceof KeyPair) {
        await keyStorage.addNewKeys([cryptoPair]);
        return cryptoPair;
    }
};


export default function App() {
    const [localMasterPassword, setLocalMasterPassword] = useState<string | null>(null);
    const [keys, setKeys] = useState<KeyPair[]>([]);
    const {location, setLocation, promiseRef} = useTaskManager();
    useEffect(() => {
       (async () => {
           const storageMasterPassword = await getStorageMasterPassword();
           setLocalMasterPassword(storageMasterPassword);
           await keyStorage.loadKeysFromStorage();
           setKeys(keyStorage.getValidKeys());
        })();
        initCheckOpenedPopup();
    }, []);

    const handleSelectKey = (e: React.MouseEvent<HTMLLIElement>) => {
        if (promiseRef.current) {
            const guid = e.currentTarget.dataset.guid;
            if (promiseRef.current.messageId === messageTypes.SELECT_SIGN_KEYS && guid) {
                promiseRef.current.resolve(guid);
            } else {
                promiseRef.current.reject("Another task was expected to resolve");
            }
        }
        promiseRef.current = null;
        window.close();
    };

    const handleSubmitMasterPassword = (masterPassword: string) => {
        setStorageMasterPassword(masterPassword);
        setLocalMasterPassword(masterPassword);
    };

    const handleSubmitNewMasterPassword = async (newMasterPassword: string) => {
        await keyStorage.changeMasterPassword(newMasterPassword);
        setLocalMasterPassword(newMasterPassword);
        setLocation("");
    };

    const handleGenerateKeys = async () => {
      const keyPair = await generateKeys();
      if (keyPair) {
          setKeys(keyStorage.getValidKeys());
      }
    };
    const handleExportKeys = () => {
        keyStorage.exportKeys();
    }
    const handleImportKeys = async () => {
        keyStorage.importKeys(() => {setKeys(keyStorage.getValidKeys())});
    }

    const handleDeprecateKey = async (key: KeyPair) => {
        await keyStorage.deprecateKey(key);
        setKeys(keyStorage.getValidKeys());
    };
    const isLoggedOut = localMasterPassword === null;
    console.log(location)
    return (
        <>
            {
                isLoggedOut ?
                <Login handleSubmitMasterPassword={handleSubmitMasterPassword} /> :
                location === locations.changeMasterPassword ? <ChangePasswordPage handleSubmitNewMasterPassword={handleSubmitNewMasterPassword} /> :
                location === locations.selectKeys ? <SelectKeysPage keys={keys} handleKey={handleSelectKey}/> :
                <Dashboard handleDeprecateKey={handleDeprecateKey} changeLocation={setLocation} handleImportKeys={handleImportKeys} handleExportKeys={handleExportKeys} handleGenerateSignKeys={handleGenerateKeys} keys={keys} masterPassword={localMasterPassword}/>
            }
        </>
    )
}