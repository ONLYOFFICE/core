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

    private static String libPrefix;
    private static String libExtension;

    private static boolean loadIfExist(String libPath) {
        File libFile = new File(libPath);
        if (libFile.exists()) {
            System.load(libPath);
            return true;
        }
        return false;
    }

    private static void load(Path libDirPath, String libName) {
        String libPath = libDirPath.resolve(libPrefix + libName + libExtension).toString();
        if (OSChecker.isMac()) {
            if (!loadIfExist(libPath)) {
                // if dylib does not exist, load framework library
                System.load(libDirPath.resolve(libName + ".framework/" + libName).toString());
            }
        } else {
            System.load(libPath);
        }
    }

    static {
        try {
            Path libDirPath = getLibPath();
            // load icu libraries
            if (OSChecker.isWindows()) {
                System.load(libDirPath.resolve("icudt74.dll").toString());
                System.load(libDirPath.resolve("icuuc74.dll").toString());
            } else if (OSChecker.isMac()) {
                loadIfExist(libDirPath.resolve("libicudata.74.dylib").toString());
                loadIfExist(libDirPath.resolve("libicuuc.74.dylib").toString());
            } else if (OSChecker.isLinux()) {
                System.load(libDirPath.resolve("libicudata.so.74").toString());
                System.load(libDirPath.resolve("libicuuc.so.74").toString());
            } else {
                throw new RuntimeException("Unsupported OS");
            }

            String[] libs = {"UnicodeConverter", "kernel", "kernel_network", "graphics", "PdfFile", "XpsFile", "DjVuFile", "DocxRenderer", "doctrenderer", "docbuilder.jni"};

            libPrefix = "";
            if (OSChecker.isMac() || OSChecker.isLinux()) {
                libPrefix = "lib";
            }

            libExtension = "";
            if (OSChecker.isWindows()) {
                libExtension = ".dll";
            } else if (OSChecker.isMac()) {
                libExtension = ".dylib";
            } else {
                libExtension = ".so";
            }

            for (String lib : libs) {
                load(libDirPath, lib);
            }
        } catch (Exception e) {
            System.out.println(e.getMessage());
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
