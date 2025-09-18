import {useState} from "react";
import PasswordInput from "../../components/password-input/password-input.tsx";

type LoginProps = {
    handleSubmitMasterPassword: (e: React.FormEvent, password: string) => void;
};
export default function Login({handleSubmitMasterPassword}: LoginProps) {
    const [masterPassword, setMasterPassword] = useState("");
    return <form onSubmit={(e) => {
        handleSubmitMasterPassword(e, masterPassword)
    }}>
        <PasswordInput name={"login-password"} labelText={"Enter master password"} onChange={(e) => {
            setMasterPassword(e.target.value);
        }}/>
    </form>
};