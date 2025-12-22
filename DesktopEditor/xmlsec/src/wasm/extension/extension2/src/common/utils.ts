export function ab2str(buf: ArrayBuffer) {
    return String.fromCharCode.apply(null, buf);
}
export function ab2base64(buf: ArrayBuffer) {
    const str = ab2str(buf);
    return btoa(str);
}
export function base642ui(base64: string) {
    const str = atob(base64);
    return str2ui(str);
}
export function str2ui(str: string) {
    const ui = new Uint8Array(str.length);
    for (let i = 0; i < str.length; i++) {
        ui[i] = str.charCodeAt(i);
    }
    return ui;
}

export const selectBinary = (callback: (file: File) => void) => {
    const input = document.createElement("input");
    input.type = "file";
    input.accept = "application/octet-stream";
    input.addEventListener("change", (e) => {
        const target = e.target as HTMLInputElement;
        const file = target.files?.[0];
        if (file) {
            callback(file);
        }
    });
    input.click();
};

export const downloadBinary = (data: Uint8Array) => {
    const blob = new Blob([data], {type: "application/octet-stream"});
    const url = URL.createObjectURL(blob);
    const link = document.createElement("a");
    link.href = url;
    link.download = `onlyoffice_keychain_${(new Date()).toISOString()}.bin`;
    link.click();
    URL.revokeObjectURL(url);
}