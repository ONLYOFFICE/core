package docbuilder;

public class CDocBuilder {

    private long c_internal = 0;

    // NATIVE METHODS:
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
