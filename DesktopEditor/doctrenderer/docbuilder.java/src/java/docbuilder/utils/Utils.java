package docbuilder.utils;

public class Utils {
    static {
        System.loadLibrary("docbuilder.jni_utils");
    }

    public static native void addToPath(String directory);
}
