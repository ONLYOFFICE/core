import type {KeyPair} from "../../../../common/keys/keys.ts";
import type {MouseEventHandler} from "react";
import css from "./select-keys.module.css";

type TSelectKeysProps = {
    keys: KeyPair[];
    handleKey: MouseEventHandler<HTMLLIElement>;
};

export default function SelectKeysPage({keys, handleKey}: TSelectKeysProps) {
    return <ul>
        {keys.map((key) => <li className={css.key} key={key.guid} data-guid={key.guid} onClick={handleKey}>
            {key.guid}
        </li>)}
    </ul>
};