import {useState, useEffect} from 'react'
import Login from "../login/login.tsx";
import {getStorageMasterPassword, initCheckOpenedPopup, setStorageMasterPassword} from "../../../utils/utils.ts";
import initTaskManager from "../../../task-manager/task-manager.ts";
import {Dashboard} from "../dashboard/dashboard.tsx";
import KeyStorage from "../../../../common/key-storage.ts";
import {KeyPair, KeyUsages} from "../../../../common/keys/keys.ts";
import getCrypto from "../../../../common/crypto.ts";
import {Ed25519KeyGenParams} from "../../../../common/keys/params.ts";
import {ChangePasswordPage} from "../change-password/change-password.tsx";
import {locations} from "../../../utils/locations.ts";
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
    const [location, setLocation] = useState("");
    const [localMasterPassword, setLocalMasterPassword] = useState<string | null>(null);
    const [keys, setKeys] = useState<KeyPair[]>([]);
    useEffect(() => {
       (async () => {
           const storageMasterPassword = await getStorageMasterPassword();
           setLocalMasterPassword(storageMasterPassword);
           await keyStorage.loadKeysFromStorage();
           setKeys(keyStorage.getValidKeys());
        })();
        initCheckOpenedPopup();
        initTaskManager();
    }, []);

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
    return (
        <>
            {
                isLoggedOut ?
                <Login handleSubmitMasterPassword={handleSubmitMasterPassword} /> :
                location === locations.changeMasterPassword ?
                <ChangePasswordPage handleSubmitNewMasterPassword={handleSubmitNewMasterPassword} /> :
                <Dashboard handleDeprecateKey={handleDeprecateKey} changeLocation={setLocation} handleImportKeys={handleImportKeys} handleExportKeys={handleExportKeys} handleGenerateSignKeys={handleGenerateKeys} keys={keys} masterPassword={localMasterPassword}/>
            }
        </>
    )
}