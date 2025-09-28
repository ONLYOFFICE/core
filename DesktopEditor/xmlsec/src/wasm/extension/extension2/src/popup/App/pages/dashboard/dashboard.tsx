import type {KeyPair} from "../../../../common/keys/keys.ts";
import {ab2base64} from "../../../../common/utils.ts";

type DashboardProps = {
    masterPassword: string;
    keys: KeyPair[];
    handleGenerateSignKeys: () => Promise<void>
    handleReloadStorage: () => void
};

export function Dashboard({keys, masterPassword, handleReloadStorage, handleGenerateSignKeys}: DashboardProps) {
    return <div>
        <div>Hello, your master password: {masterPassword}</div>
        <button onClick={handleGenerateSignKeys}>Generate sign keys</button>
        <button onClick={handleReloadStorage}>Reload keys</button>
        <div>Generated sign keys</div>
        {keys.map((key) => <div>{ab2base64(key.privateKey.key)} : {ab2base64(key.publicKey.key)}</div>)}
    </div>
}