package docbuilder;

import docbuilder.utils.NativeLibraryLoader;

public class CDocBuilder {
    public CDocBuilder() {
        c_internal = c_Create();
    }

    protected void finalize() {
        c_Destroy(c_internal);
    }

    public int openFile(String path, String params) {
        return c_OpenFile(c_internal, path, params);
    }

    public boolean createFile(int type) {
        return c_CreateFileByType(c_internal, type);
    }

    public boolean createFile(String extension) {
        return c_CreateFileByExtension(c_internal, extension);
    }

    public void setTmpFolder(String folder) {
        c_SetTmpFolder(c_internal, folder);
    }

    public int saveFile(int type, String path) {
        return c_SaveFileByType(c_internal, type, path);
    }

    public int saveFile(int type, String path, String params) {
        return c_SaveFileByTypeWithParams(c_internal, type, path, params);
    }

    public int saveFile(String extension, String path) {
        return c_SaveFileByExtension(c_internal, extension, path);
    }

    public int saveFile(String extension, String path, String params) {
        return c_SaveFileByExtensionWithParams(c_internal, extension, path, params);
    }

    public void closeFile() {
        c_CloseFile(c_internal);
    }

    public boolean executeCommand(String command) {
        return c_ExecuteCommand(c_internal, command);
    }

    public boolean executeCommand(String command, CDocBuilderValue retValue) {
        return c_ExecuteCommandWithRetValue(c_internal, command, retValue.c_internal);
    }

    public boolean run(String path) {
        return c_Run(c_internal, path);
    }

    public boolean runText(String commands) {
        return c_RunText(c_internal, commands);
    }

    public void setProperty(String param, String value) {
        c_SetProperty(c_internal, param, value);
    }

    public void writeData(String path, String value, boolean append) {
        c_WriteData(c_internal, path, value, append);
    }

    public boolean isSaveWithDoctrendererMode() {
        return c_IsSaveWithDoctrendererMode(c_internal);
    }

    public String getVersion() {
        return c_GetVersion(c_internal);
    }

    public CDocBuilderContext getContext() {
        return new CDocBuilderContext(c_GetContext(c_internal));
    }

    public static void initialize() {
        c_Initialize();
    }

    public static void initialize(String directory) {
        if (directory.isEmpty())
            directory = NativeLibraryLoader.getLibPath().toString();
        c_InitializeWithDirectory(directory);
    }

    public static void dispose() {
        c_Dispose();
    }

    // Native code
    static {
        docbuilder.utils.NativeLibraryLoader.loadLibraries();
    }

    long c_internal = 0;

    private static native long c_Create();
    private static native void c_Destroy(long self);

    private static native int c_OpenFile(long self, String path, String params);
    private static native boolean c_CreateFileByType(long self, int type);
    private static native boolean c_CreateFileByExtension(long self, String extension);

    private static native void c_SetTmpFolder(long self, String folder);

    private static native int c_SaveFileByType(long self, int type, String path);
    private static native int c_SaveFileByTypeWithParams(long self, int type, String path, String params);
    private static native int c_SaveFileByExtension(long self, String extension, String path);
    private static native int c_SaveFileByExtensionWithParams(long self, String extension, String path, String params);

    private static native void c_CloseFile(long self);

    private static native boolean c_ExecuteCommand(long self, String command);
    private static native boolean c_ExecuteCommandWithRetValue(long self, String command, long retValue);

    private static native boolean c_Run(long self, String path);
    private static native boolean c_RunText(long self, String commands);

    private static native void c_SetProperty(long self, String param, String value);

    private static native void c_WriteData(long self, String path, String value, boolean append);

    private static native boolean c_IsSaveWithDoctrendererMode(long self);

    private static native String c_GetVersion(long self);

    private static native long c_GetContext(long self);

    private static native void c_Initialize();
    private static native void c_InitializeWithDirectory(String directory);
    private static native void c_Dispose();
}
