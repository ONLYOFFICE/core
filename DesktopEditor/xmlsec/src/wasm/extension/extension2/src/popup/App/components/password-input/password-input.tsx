type PropsType = {
    name: string;
    labelText: string;
    onChange: (e: string) => void;
}

export default function PasswordInput({onChange, name, labelText}: PropsType) {
    return <>
        <label htmlFor={name}>{labelText}</label>
        <input required={true} minLength={8} onChange={(e) => onChange(e.target.value)} type="password" name={name}/>
    </>;
}