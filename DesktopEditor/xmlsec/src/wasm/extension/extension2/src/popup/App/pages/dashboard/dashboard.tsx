import type {KeyPair} from "../../../../common/keys/keys.ts";
import {locations} from "../../../utils/locations.ts";
import css from "./dashboard.module.css";

type DashboardProps = {
    masterPassword: string;
    keys: KeyPair[];
    handleGenerateSignKeys: () => Promise<void>
    handleImportKeys: () => void;
    handleExportKeys: () => void;
    handleDeprecateKey: (key: KeyPair) => void;
    changeLocation: (location: string) => void
};

export function Dashboard({keys, handleDeprecateKey, masterPassword, handleGenerateSignKeys, handleImportKeys, handleExportKeys, changeLocation}: DashboardProps) {
    return <div>
        <div>Hello, your master password: {masterPassword}</div>
        <button onClick={() => {changeLocation(locations.changeMasterPassword)}}>Change password</button>
        <button onClick={handleExportKeys}>Export keys</button>
        <button onClick={handleImportKeys}>Import keys</button>
        <button onClick={handleGenerateSignKeys}>Generate sign keys</button>
        <div>Generated sign keys</div>
        {keys.map((key, idx) =>
            <div key={idx} className={css.wrapper}>
            <div>{key.guid}</div>
            <div onClick={() => handleDeprecateKey(key)}>&times;</div>
        </div>)}
    </div>
}