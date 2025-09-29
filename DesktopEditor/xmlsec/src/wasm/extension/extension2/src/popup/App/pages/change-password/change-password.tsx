import PasswordInput from "../../components/password-input/password-input.tsx";
import {type FormEvent, useState} from "react";
import {compareWithOldMasterPassword} from "../../../utils/utils.ts";

type ChangePasswordPageProps = {
    handleSubmitNewMasterPassword: (newMasterPassword: string) => void;
};
export function ChangePasswordPage({handleSubmitNewMasterPassword}: ChangePasswordPageProps) {
    const [oldMasterPassword, setOldMasterPassword] = useState("");
    const [newMasterPassword, setNewMasterPassword] = useState("");
    const [confirmMasterPassword, setConfirmMasterPassword] = useState("");
    const [error, setError] = useState("");
    const onSubmit = async (e: FormEvent<HTMLFormElement>) => {
        e.preventDefault();
        const isEqualsOldPassword = await compareWithOldMasterPassword(oldMasterPassword);
        if (!isEqualsOldPassword) {
            setError("Check if you entered your old password correctly.");
        } else if (newMasterPassword !== confirmMasterPassword) {
            setError("The new passwords do not match.");
        } else {
            handleSubmitNewMasterPassword(newMasterPassword);
        }
    };
    return <form onSubmit={onSubmit}>
        <PasswordInput name={"old-password"} labelText={"Enter old master password"} onChange={setOldMasterPassword}/>
        <PasswordInput name={"new-password"} labelText={"Enter new master password"} onChange={setNewMasterPassword}/>
        <PasswordInput name={"confirm-new-password"} labelText={"Confirm new master password"} onChange={setConfirmMasterPassword}/>
        {error && <div>{error}</div>}
        <button type={"submit"}>Confirm</button>
    </form>
}