import {type FormEvent, useState} from "react";
import PasswordInput from "../../components/password-input/password-input.tsx";

type LoginProps = {
    handleSubmitMasterPassword: (password: string) => void;
};
export default function Login({handleSubmitMasterPassword}: LoginProps) {
    const [masterPassword, setMasterPassword] = useState("");
    const [confirmMasterPassword, setConfirmMasterPassword] = useState("");
    const [error, setError] = useState("");
    const handleSubmit = (e: FormEvent<HTMLFormElement>) => {
        e.preventDefault();
        if (masterPassword === confirmMasterPassword) {
            handleSubmitMasterPassword(masterPassword);
        } else {
            setError("The passwords don't match");
        }
    };
    return <form onSubmit={handleSubmit}>
        <PasswordInput name={"login-password"} labelText={"Enter new master password"} onChange={setMasterPassword}/>
        <PasswordInput name={"login-confirm-password"} labelText={"Confirm new master password"} onChange={setConfirmMasterPassword}/>
        {error && <div>{error}</div>}
        <button type={"submit"}>Confirm</button>
    </form>
};