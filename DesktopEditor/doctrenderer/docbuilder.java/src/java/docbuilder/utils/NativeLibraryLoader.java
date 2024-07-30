package docbuilder.utils;

public class NativeLibraryLoader {
    static {
        System.loadLibrary("icudt58");
        System.loadLibrary("icuuc58");
        System.loadLibrary("UnicodeConverter");
        System.loadLibrary("kernel");
        System.loadLibrary("kernel_network");
        System.loadLibrary("graphics");
        System.loadLibrary("doctrenderer");
        System.loadLibrary("docbuilder.jni");
    }

    public static void loadLibraries() {
        // No-op, just to force the class loading
    }
}
