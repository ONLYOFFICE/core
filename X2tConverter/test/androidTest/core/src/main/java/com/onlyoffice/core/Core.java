package com.onlyoffice.core;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.RequiresPermission;

import java.io.File;

import static android.Manifest.permission.READ_EXTERNAL_STORAGE;
import static android.Manifest.permission.WRITE_EXTERNAL_STORAGE;

public class Core {

    static {
        System.loadLibrary(BuildConfig.LIB_CORE);
    }

    private static class Paths {
        public String from;
        public String to;
        public String cache;
    }

    public static class InputParams {

        public static final int TCSVD_NONE = 0;
        public static final int TCSVD_TAB = 1;
        public static final int TCSVD_SEMICOLON = 2;
        public static final int TCSVD_COLON = 3;
        public static final int TCSVD_COMMA = 4;
        public static final int TCSVD_SPACE = 5;

        private String password;
        private String delimiterChar;
        private int delimiterCode = TCSVD_NONE;
        private int encoding;
        private int formatFrom;
        private int formatTo;
        private int isNoBase64 = 1;
    }

    public static native String test(String str);

    /*
    * Native DOCX
    * */
    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int docx2doctbin(@NonNull String from, @NonNull String to, @NonNull String buf, @Nullable InputParams inputParams);

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int docx2doct(@NonNull String from, @NonNull String to, @NonNull String buf, @Nullable InputParams inputParams);

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int doctbin2docx(@NonNull String from, @NonNull String to, @NonNull String buf, boolean isFromChanges,
                                          @Nullable String theme, @Nullable InputParams inputParams);

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int doct2docx(@NonNull String from, @NonNull String to, @NonNull String buf, boolean isFromChanges,
                                       @Nullable String theme, @Nullable InputParams inputParams);


    /*
    * Native XLSX
    * */
    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int xlsx2xlstbin(@NonNull String from, @NonNull String to, @NonNull String buf, @Nullable InputParams inputParams);

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int xlsx2xlst(@NonNull String from, @NonNull String to, @NonNull String buf, @Nullable InputParams inputParams);

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int xlstbin2xlsx(@NonNull String from, @NonNull String to, @NonNull String buf, boolean isFromChanges,
                                          @Nullable String theme, @Nullable InputParams inputParams);

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int xlst2xlsx(@NonNull String from, @NonNull String to, @NonNull String buf, boolean isFromChanges,
                                       @Nullable String theme, @Nullable InputParams inputParams);


    /*
    * Native PPTX
    * */
    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int pptx2ppttbin(@NonNull String from, @NonNull String to, @NonNull String buf, @Nullable InputParams inputParams);

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int pptx2pptt(@NonNull String from, @NonNull String to, @NonNull String buf, @Nullable InputParams inputParams);

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int ppttbin2pptx(@NonNull String from, @NonNull String to, @NonNull String buf, boolean isFromChanges,
                                          @Nullable String theme, @Nullable InputParams inputParams);

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int pptt2pptx(@NonNull String from, @NonNull String to, @NonNull String buf, boolean isFromChanges,
                                       @Nullable String theme, @Nullable InputParams inputParams);


    /*
    * Native CSV
    * */
    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int csv2xlst(@NonNull String from, @NonNull String to, @NonNull String buf, @Nullable InputParams inputParams);

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int csv2xlsx(@NonNull String from, @NonNull String to, @NonNull String buf, @Nullable InputParams inputParams);

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int csv2xlstbin(@NonNull String from, @NonNull String to, @Nullable InputParams inputParams);

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int xlst2csv(@NonNull String from, @NonNull String to, @NonNull String buf, @Nullable InputParams inputParams);

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int xlsx2csv(@NonNull String from, @NonNull String to, @NonNull String buf, @Nullable InputParams inputParams);


    /*
    * Native TXT
    * */
    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int txt2doctbin(@NonNull String from, @NonNull String to, @NonNull String buf, @Nullable InputParams inputParams);

    /*
    * Native ZIP
    * */
    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int dir2zip(String from, String to);
    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static native int zip2dir(String from, String to);



    private static String deleteExtensionFromPath(final String path) {
        try {
            return path.substring(0, path.lastIndexOf("."));
        } catch (RuntimeException e) {
            return "";
        }
    }

    private static boolean createPathIfNotExist(@NonNull String path) {
        final File file = new File(path);
        return (file.isDirectory() && file.exists()) || file.mkdirs();
    }

    @Nullable
    private static Paths getPaths(@NonNull Context context, @NonNull String from, @Nullable String to,
                                  @NonNull String cacheDir, @NonNull String extension) {
        Paths paths = null;
        final String cache = context.getExternalCacheDir().getAbsolutePath() + "/" + cacheDir;
        if (createPathIfNotExist(cache)) {
            final String newTo = (to == null)? deleteExtensionFromPath(from) + extension : to;
            paths = new Paths();
            paths.from = from;
            paths.to = newTo;
            paths.cache = cache;
        }

        return paths;
    }

    @RequiresPermission(WRITE_EXTERNAL_STORAGE)
    public static void deleteFolder(File fileOrDirectory) {
        if (fileOrDirectory.isDirectory()) {
            for (File child : fileOrDirectory.listFiles()) {
                deleteFolder(child);
            }
        }

        fileOrDirectory.delete();
    }


    /*
    * DOCSs
    * */
    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean docx2doctbin(@NonNull Context context, @NonNull String from, @Nullable String to) {
        final Paths paths = getPaths(context, from, to, "docx2docbin", "doc.bin");
        boolean isSuccess = false;
        if (paths != null) {
            isSuccess = docx2doctbin(paths.from, paths.to, paths.cache, null) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean docx2doct(@NonNull Context context, @NonNull String from, @Nullable String to) {
        final Paths paths = getPaths(context, from, to, "docx2doct", "doct");
        boolean isSuccess = false;
        if (paths != null) {
            isSuccess = docx2doct(paths.from, paths.to, paths.cache, null) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean doctbin2docx(@NonNull Context context, @NonNull String from, @Nullable String to) {
        final Paths paths = getPaths(context, from, to, "doctbin2docx", "docx");
        boolean isSuccess = false;
        if (paths != null) {
            isSuccess = doctbin2docx(paths.from, paths.to, paths.cache, false, "", null) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean doct2docx(@NonNull Context context, @NonNull String from, @Nullable String to) {
        final Paths paths = getPaths(context, from, to, "doct2docx", "dox");
        boolean isSuccess = false;
        if (paths != null) {
            isSuccess = doct2docx(paths.from, paths.to, paths.cache, false, "", null) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }


    /*
     * XLSX
     * */
    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean xlsx2xlstbin(@NonNull Context context, @NonNull String from, @Nullable String to) {
        final Paths paths = getPaths(context, from, to, "xlsx2xlstbin", "xlsx.bin");
        boolean isSuccess = false;
        if (paths != null) {
            isSuccess = xlsx2xlstbin(paths.from, paths.to, paths.cache, null) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean xlsx2xlst(@NonNull Context context, @NonNull String from, @Nullable String to) {
        final Paths paths = getPaths(context, from, to, "xlsx2xlst", "xlst");
        boolean isSuccess = false;
        if (paths != null) {
            isSuccess = xlsx2xlst(paths.from, paths.to, paths.cache, null) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean xlstbin2xlsx(@NonNull Context context, @NonNull String from, @Nullable String to) {
        final Paths paths = getPaths(context, from, to, "xlstbin2xlsx", "xlsx");
        boolean isSuccess = false;
        if (paths != null) {
            isSuccess = xlstbin2xlsx(paths.from, paths.to, paths.cache, false, "", null) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean xlst2xlsx(@NonNull Context context, @NonNull String from, @Nullable String to) {
        final Paths paths = getPaths(context, from, to, "xlst2xlsx", "xlsx");
        boolean isSuccess = false;
        if (paths != null) {
            isSuccess = xlst2xlsx(paths.from, paths.to, paths.cache, false, "", null) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }


    /*
     * PPTX
     * */
    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean pptx2ppttbin(@NonNull Context context, @NonNull String from, @Nullable String to) {
        final Paths paths = getPaths(context, from, to, "pptx2ppttbin", "pptt.bin");
        boolean isSuccess = false;
        if (paths != null) {
            isSuccess = pptx2ppttbin(paths.from, paths.to, paths.cache, null) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean pptx2pptt(@NonNull Context context, @NonNull String from, @Nullable String to) {
        final Paths paths = getPaths(context, from, to, "pptx2pptt", "pptt");
        boolean isSuccess = false;
        if (paths != null) {
            isSuccess = pptx2pptt(paths.from, paths.to, paths.cache, null) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean ppttbin2pptx(@NonNull Context context, @NonNull String from, @Nullable String to) {
        final Paths paths = getPaths(context, from, to, "ppttbin2pptx", "pptx");
        boolean isSuccess = false;
        if (paths != null) {
            isSuccess = ppttbin2pptx(paths.from, paths.to, paths.cache, false, "", null) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean pptt2pptx(@NonNull Context context, @NonNull String from, @Nullable String to) {
        final Paths paths = getPaths(context, from, to, "pptt2pptx", "pptx");
        boolean isSuccess = false;
        if (paths != null) {
            isSuccess = pptt2pptx(paths.from, paths.to, paths.cache, false, "", null) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }


    /*
     * CSV
     * */
    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean csv2xlst(@NonNull Context context, @NonNull String from, @Nullable String to, @NonNull String delimiter) {
        final Paths paths = getPaths(context, from, to, "csv2xlst", "xlst");
        boolean isSuccess = false;
        if (paths != null) {
            final InputParams inputParams = new InputParams();
            inputParams.delimiterChar = delimiter;
            isSuccess = csv2xlst(paths.from, paths.to, paths.cache, inputParams) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean csv2xlsx(@NonNull Context context, @NonNull String from, @Nullable String to, @NonNull String delimiter) {
        final Paths paths = getPaths(context, from, to, "csv2xlsx", "xlsx");
        boolean isSuccess = false;
        if (paths != null) {
            final InputParams inputParams = new InputParams();
            inputParams.delimiterChar = delimiter;
            isSuccess = csv2xlsx(paths.from, paths.to, paths.cache, inputParams) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean csv2xlstbin(@NonNull Context context, @NonNull String from, @Nullable String to, @NonNull String delimiter) {
        final Paths paths = getPaths(context, from, to, "csv2xlstbin", "xlst.bin");
        boolean isSuccess = false;
        if (paths != null) {
            final InputParams inputParams = new InputParams();
            inputParams.delimiterChar = delimiter;
            isSuccess = csv2xlstbin(paths.from, paths.to, inputParams) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean xlst2csv(@NonNull Context context, @NonNull String from, @Nullable String to, @NonNull String delimiter) {
        final Paths paths = getPaths(context, from, to, "xlst2csv", "csv");
        boolean isSuccess = false;
        if (paths != null) {
            final InputParams inputParams = new InputParams();
            inputParams.delimiterChar = delimiter;
            isSuccess = xlst2csv(paths.from, paths.to, paths.cache, inputParams) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }

    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean xlsx2csv(@NonNull Context context, @NonNull String from, @Nullable String to, @NonNull String delimiter) {
        final Paths paths = getPaths(context, from, to, "xlsx2csv", "csv");
        boolean isSuccess = false;
        if (paths != null) {
            final InputParams inputParams = new InputParams();
            inputParams.delimiterChar = delimiter;
            isSuccess = xlsx2csv(paths.from, paths.to, paths.cache, inputParams) == 0;
//            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }


    /*
     * TXT
     * */
    @RequiresPermission(allOf = { READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE })
    public static boolean txt2doctbin(@NonNull Context context, @NonNull String from, @Nullable String to) {
        final Paths paths = getPaths(context, from, to, "txt2doctbin", "doct.bin");
        boolean isSuccess = false;
        if (paths != null) {
            isSuccess = txt2doctbin(paths.from, paths.to, paths.cache, null) == 0;
            deleteFolder(new File(paths.cache));
        }

        return isSuccess;
    }

}
