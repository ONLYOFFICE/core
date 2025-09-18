import {useState, useEffect} from 'react'
import Login from "../login/login.tsx";
import {getStorageMasterPassword, initCheckOpenedPopup, setStorageMasterPassword} from "../../../utils/utils.ts";
import initTaskManager from "../../../task-manager/task-manager.ts";

export default function App() {
    const [localMasterPassword, setLocalMasterPassword] = useState<string | null>(null);
    useEffect(() => {
       (async () => {
           const storageMasterPassword = await getStorageMasterPassword();
           setLocalMasterPassword(storageMasterPassword);

        })();
        initCheckOpenedPopup();
        initTaskManager();
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