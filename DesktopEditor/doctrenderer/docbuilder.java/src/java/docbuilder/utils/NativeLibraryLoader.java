package docbuilder.utils;

import java.io.File;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.file.Path;

public class NativeLibraryLoader {
    // helper class for retrieving information about current OS
    public static class OSChecker {
        private static String osName = System.getProperty("os.name").toLowerCase();

        public static boolean isWindows() {
            return (osName.indexOf("win") >= 0);
        }

        public static boolean isMac() {
            return (osName.indexOf("mac") >= 0);
        }

        public static boolean isLinux() {
            return (osName.indexOf("nix") >= 0 || osName.indexOf("nux") >= 0 || osName.indexOf("aix") > 0);
        }
    }

    static {
        try {
            Path libDirPath = getLibPath();
            // load icu libraries
            if (OSChecker.isWindows()) {
                System.load(libDirPath.resolve("icudt58.dll").toString());
                System.load(libDirPath.resolve("icuuc58.dll").toString());
            } else if (OSChecker.isMac()) {
                System.load(libDirPath.resolve("libicudata.58.dylib").toString());
                System.load(libDirPath.resolve("libicuuc.58.dylib").toString());
            } else if (OSChecker.isLinux()) {
                System.load(libDirPath.resolve("libicudata.so.58").toString());
                System.load(libDirPath.resolve("libicuuc.so.58").toString());
            } else {
                throw new RuntimeException("Unsupported OS");
            }

            String[] libs = {"UnicodeConverter", "kernel", "kernel_network", "graphics", "PdfFile", "XpsFile", "DjVuFile", "DocxRenderer", "doctrenderer", "docbuilder.jni"};

            String prefix = "";
            if (OSChecker.isMac() || OSChecker.isLinux()) {
                prefix = "lib";
            }

            String extension = "";
            if (OSChecker.isWindows()) {
                extension = ".dll";
            } else if (OSChecker.isMac()) {
                extension = ".dylib";
            } else {
                extension = ".so";
            }

            for (String lib : libs) {
                System.load(libDirPath.resolve(prefix + lib + extension).toString());
            }
        } catch (UnsatisfiedLinkError e) {
            throw new RuntimeException("Cannot load dynamic libraries. Check if JAR file is in the same directory as all docbuilder libraries.");
        }
    }

    // Returns path to the directory containing current JAR
    public static Path getLibPath()
    {
        URL url = NativeLibraryLoader.class.getProtectionDomain().getCodeSource().getLocation();
        try {
            File jarFile = new File(url.toURI());
            Path jarPath = jarFile.toPath();
            Path libDirPath = jarPath.getParent();

            return libDirPath;
        } catch (URISyntaxException exception) {
            // Very unlikely to be happened
            throw new RuntimeException("Cannot convert URI of the NativeLibraryLoader.class to URL");
        }
    }

    public static void loadLibraries() {
        // No-op, just to force the class loading
    }
}
