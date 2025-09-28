export function ab2str(buf: ArrayBuffer) {
    return String.fromCharCode(...new Uint8Array(buf));
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