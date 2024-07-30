package docbuilder;

public class CDocBuilderContext {
    public CDocBuilderContext() {
        c_internal = c_Create();
    }

    CDocBuilderContext(long internal) {
        c_internal = internal;
    }

    public CDocBuilderContext(CDocBuilderContext other) {
        c_internal = c_Copy(other.c_internal);
    }

    protected void finalize() {
        c_Destroy(c_internal);
    }

    public CDocBuilderValue createUndefined() {
        return new CDocBuilderValue(c_CreateUndefined(c_internal));
    }

    public CDocBuilderValue createNull() {
        return new CDocBuilderValue(c_CreateNull(c_internal));
    }

    public CDocBuilderValue createObject() {
        return new CDocBuilderValue(c_CreateObject(c_internal));
    }

    public CDocBuilderValue createArray(int length) {
        return new CDocBuilderValue(c_CreateArray(c_internal, length));
    }

    public CDocBuilderValue getGlobal() {
        return new CDocBuilderValue(c_GetGlobal(c_internal));
    }

    public CDocBuilderContextScope createScope() {
        return new CDocBuilderContextScope(c_CreateScope(c_internal));
    }

    public boolean isError() {
        return c_IsError(c_internal);
    }

    // Native code
    static {
        docbuilder.utils.NativeLibraryLoader.loadLibraries();
    }

    long c_internal = 0;

    private static native long c_Create();
    private static native long c_Copy(long other);
    private static native void c_Destroy(long self);

    private static native long c_CreateUndefined(long self);
    private static native long c_CreateNull(long self);
    private static native long c_CreateObject(long self);
    private static native long c_CreateArray(long self, int length);

    private static native long c_GetGlobal(long self);

    private static native long c_CreateScope(long self);

    private static native boolean c_IsError(long self);
}
