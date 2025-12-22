import {useState, useEffect} from 'react'
import Login from "../login/login.tsx";
import {getStorageMasterPassword, initCheckOpenedPopup, setStorageMasterPassword} from "../../../utils/utils.ts";
import {useTaskManager} from "../../../task-manager/task-manager.ts";
import {Dashboard} from "../dashboard/dashboard.tsx";
// @ts-ignore
import {StorageManager} from "../storage-manager/storage-manager.ts";
import {KeyPair} from "../../../../common/keys/keys.ts";
import {Ed25519KeyGenParams} from "../../../../common/keys/params.ts";
import {ChangePasswordPage} from "../change-password/change-password.tsx";
import {locations} from "../../../utils/locations.ts";
import SelectKeysPage from "../select-keys/select-keys.tsx";
import {messageTypes} from "../../../../common/message-const.ts";
const storageManager = new StorageManager();
const generateKeys = async () => {
    const key = await storageManager.generateKeys(new Ed25519KeyGenParams());
    if (key) {
        await storageManager.addNewKeys([key]);
        return key;
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
           await storageManager.loadKeysFromStorage();
           setKeys(storageManager.getValidKeys());
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
        await storageManager.changeMasterPassword(newMasterPassword);
        setLocalMasterPassword(newMasterPassword);
        setLocation("");
    };

    const handleGenerateKeys = async () => {
      const keyPair = await generateKeys();
      if (keyPair) {
          setKeys(storageManager.getValidKeys());
      }
    };
    const handleExportKeys = () => {
        storageManager.exportKeys();
    }
    const handleImportKeys = async () => {
        storageManager.importKeys(() => {setKeys(storageManager.getValidKeys())});
    }

    const handleDeprecateKey = async (key: KeyPair) => {
        await storageManager.deprecateKey(key);
        setKeys(storageManager.getValidKeys());
    };
    const isLoggedOut = localMasterPassword === null;
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