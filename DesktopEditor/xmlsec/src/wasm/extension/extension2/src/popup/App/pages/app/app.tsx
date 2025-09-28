import {useState, useEffect} from 'react'
import Login from "../login/login.tsx";
import {getStorageMasterPassword, initCheckOpenedPopup, setStorageMasterPassword} from "../../../utils/utils.ts";
import initTaskManager from "../../../task-manager/task-manager.ts";
import {Dashboard} from "../dashboard/dashboard.tsx";
import KeyStorage from "../../../../background/key-storage.ts";
import {KeyPair, KeyUsages} from "../../../../common/keys/keys.ts";
import getCrypto from "../../../../common/crypto.ts";
import {Ed25519KeyGenParams} from "../../../../common/keys/params.ts";
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
    const reloadStorage = async () => {
        await keyStorage.loadKeysFromStorage();
        setKeys([...keyStorage.keys]);
    }

    useEffect(() => {
       (async () => {
           const storageMasterPassword = await getStorageMasterPassword();
           setLocalMasterPassword(storageMasterPassword);
           await keyStorage.loadKeysFromStorage();
           setKeys([...keyStorage.keys]);
        })();
        initCheckOpenedPopup();
        initTaskManager();
    }, []);

    const handleSubmitMasterPassword = (e: React.FormEvent, masterPassword: string) => {
        e.preventDefault();
        setStorageMasterPassword(masterPassword);
        setLocalMasterPassword(masterPassword);
    };
    const handleGenerateKeys = async () => {
      const keyPair = await generateKeys();
      if (keyPair) {
          setKeys([...keys, keyPair]);
      }
    };
    const handleReloadStorage = () => {
        reloadStorage()
    }
    const isLoggedIn = localMasterPassword !== null;
    return (
        <>
            {isLoggedIn ?
                <Dashboard handleReloadStorage={handleReloadStorage} handleGenerateSignKeys={handleGenerateKeys} keys={keys} masterPassword={localMasterPassword}/> :
                <Login handleSubmitMasterPassword={handleSubmitMasterPassword} />}
        </>
    )
}