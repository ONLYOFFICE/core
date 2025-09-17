import {useState} from "react";

type LoginProps = {
    handleSubmitMasterPassword: (e: React.FormEvent, password: string) => void;
};
export default function Login({handleSubmitMasterPassword}: LoginProps) {
    const [masterPassword, setMasterPassword] = useState("");
    return <form onSubmit={(e) => {handleSubmitMasterPassword(e, masterPassword)}}>
        <label htmlFor="password">input master password</label>
        <input required={true} minLength={8} onChange={(e) => {setMasterPassword(e.target.value)}} type="password" name="password"/>
    </form>
};