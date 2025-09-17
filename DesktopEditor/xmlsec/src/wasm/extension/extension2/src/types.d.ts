export {};

declare global {
    interface WindowEventMap {
        "onlyoffice-sign-extension-channel": CustomEvent;
    }
}