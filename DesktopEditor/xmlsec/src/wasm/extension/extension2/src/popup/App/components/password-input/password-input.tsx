type PropsType = {
    name: string;
    labelText: string;
    onChange: (e: React.ChangeEvent<HTMLInputElement>) => void;
}

export default function PasswordInput({onChange, name, labelText}: PropsType) {
    return <>
        <label htmlFor={name}>{labelText}</label>
        <input required={true} minLength={8} onChange={onChange} type="password" name={name}/>
    </>;
}