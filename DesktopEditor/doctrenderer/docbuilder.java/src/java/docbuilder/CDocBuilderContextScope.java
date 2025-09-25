package docbuilder;

public class CDocBuilderContextScope {
    public CDocBuilderContextScope() {
        c_internal = c_Create();
    }

    CDocBuilderContextScope(long internal) {
        c_internal = internal;
    }

    public CDocBuilderContextScope(CDocBuilderContextScope other) {
        c_internal = c_Copy(other.c_internal);
    }

    protected void finalize() {
        c_Destroy(c_internal);
    }

    public void close() {
        c_Close(c_internal);
    }

    // Native code
    static {
        docbuilder.utils.NativeLibraryLoader.loadLibraries();
    }

    long c_internal = 0;

    private static native long c_Create();
    private static native long c_Copy(long other);
    private static native void c_Destroy(long self);

    private static native void c_Close(long self);
}
