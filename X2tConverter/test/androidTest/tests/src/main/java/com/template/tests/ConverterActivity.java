package com.template.tests;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ContentUris;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.annotation.RequiresPermission;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.AppCompatButton;
import android.support.v7.widget.AppCompatTextView;
import android.util.Log;
import android.view.View;
import android.widget.ScrollView;
import android.widget.Toast;

import com.onlyoffice.core.Core;

public class ConverterActivity extends AppCompatActivity implements View.OnClickListener {

    public static final String TAG = ConverterActivity.class.getSimpleName();

    private static final int REQUEST_CODE_DOC = 1;

    private static final String CONVERTER_PREFIX = "converter_";

    private static final int PERMISSION_WRITE_STORAGE = 1;
    private static final int PERMISSION_CONVERTER_READ_STORAGE = 2;
    private static final int PERMISSION_CHOOSER_READ_STORAGE = 3;

    private Toast mToast;
    private ScrollView mScrollView;
    private AppCompatButton mConvertButton;
    private AppCompatButton mChooseButton;
    private AppCompatTextView mAppCompatTextView;

    private String mFilePath;
    private String mFileName;
    private String mFolderPath;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mScrollView = findViewById(R.id.scrollView);
        mAppCompatTextView = findViewById(R.id.logger_text);
        mConvertButton = findViewById(R.id.convert_button);
        mChooseButton = findViewById(R.id.choose_button);
        mConvertButton.setOnClickListener(this);
        mChooseButton.setOnClickListener(this);
        mToast = Toast.makeText(this, "", Toast.LENGTH_LONG);
    }

    @SuppressLint("MissingPermission")
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        final boolean isGranted = grantResults.length == 1 && grantResults[0] == PackageManager.PERMISSION_GRANTED;
        switch (requestCode) {
            case PERMISSION_WRITE_STORAGE: {
                if (isGranted) {
                    converter();
                }
                break;
            }

            case PERMISSION_CONVERTER_READ_STORAGE: {
                if (isGranted) {
                    converter();
                }
                break;
            }

            case PERMISSION_CHOOSER_READ_STORAGE: {
                if (isGranted) {
                    browseDocuments();
                }
                break;
            }
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == Activity.RESULT_OK) {
            switch (requestCode) {
                case REQUEST_CODE_DOC: {
                    if (data != null && data.getData() != null) {
                       mFilePath = getFilePath(data.getData());
                       if (mFilePath != null) {
                            mFolderPath = mFilePath.substring(0, mFilePath.lastIndexOf("/"));
                            mFileName = mFilePath.replace(mFolderPath + "/", "");
                            logMessage("\n-------------------------------------------------");
                            logMessage("Source file path: " + mFilePath);
                            logMessage("Source file name: " + mFileName);
                            logMessage("Source folder path: " + mFolderPath);
                       } else {
                           showToast("UNKNOWN ERROR!");
                           logMessage("\nUNKNOWN ERROR!\n");
                       }
                    }
                    break;
                }
            }
        }
    }

    @SuppressLint("MissingPermission")
    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.convert_button:
                converter();
                break;
            case R.id.choose_button:
                browseDocuments();
                break;
        }
    }

    private void converter() {
        if (checkReadPermission(PERMISSION_CONVERTER_READ_STORAGE) && checkWritePermission()) {
            if (mFilePath != null && mFileName != null && mFolderPath != null) {
                switch (getExtension(mFilePath).toUpperCase()) {
                    case "DOCX":
                        docxConverter();
                        break;
                    case "XLSX":
                        xlsxConverter();
                        break;
                    case "PPTX":
                        pptxConverter();
                        break;
                    case "CSV":
                        csvConverter();
                        break;
                    case "TXT":
                        txtConverter();
                        break;
                    default:
                        showToast("Unsupported format!");
                }
            } else {
                showToast("Choose documents!");
            }
        }
    }

    @SuppressLint("MissingPermission")
    private void docxConverter() {
        final String to1 = mFolderPath + "/docx_" + CONVERTER_PREFIX + mFileName + ".doct.bin";
        final String to2 = mFolderPath + "/docx_" + CONVERTER_PREFIX + mFileName + ".doct";
        final String to3 = mFolderPath + "/docx_bin_" + CONVERTER_PREFIX + mFileName + ".docx";
        final String to4 = mFolderPath + "/docx_doct_" + CONVERTER_PREFIX + mFileName + ".docx";

        final AsyncTask<Void, String, Void> asyncTask = new AsyncTask<Void, String, Void>() {

            @Override
            protected void onPreExecute() {
                super.onPreExecute();
                logMessage("\nStart DOCX converting...");
                mChooseButton.setEnabled(false);
                mConvertButton.setEnabled(false);
            }

            @Override
            protected Void doInBackground(Void... voids) {
                if (Core.docx2doctbin(getBaseContext(), mFilePath, to1)) {
                    publishProgress("Convert PASS: " + to1);
                    if (Core.docx2doct(getBaseContext(), mFilePath, to2)) {
                        publishProgress("Convert PASS: " + to2);
                        if (Core.doctbin2docx(getBaseContext(), to1, to3)) {
                            publishProgress("Convert PASS: " + to3);
                            if (Core.doct2docx(getBaseContext(), to2, to4)) {
                                publishProgress("Convert PASS: " + to4);
                            } else {
                                publishProgress("Convert FAILED: " + to4);
                            }
                        } else {
                            publishProgress("Convert FAILED: " + to3);
                        }
                    } else {
                        publishProgress("Convert FAILED: " + to2);
                    }
                } else {
                    publishProgress("\nConvert FAILED: " + to1);
                }
                return null;
            }

            @Override
            protected void onProgressUpdate(String... values) {
                super.onProgressUpdate(values);
                logMessage(values[0]);
            }

            @Override
            protected void onPostExecute(Void aVoid) {
                super.onPostExecute(aVoid);
                showToast("All DOCX converts PASSED!");
                mChooseButton.setEnabled(true);
                mConvertButton.setEnabled(true);
            }
        };
        asyncTask.execute();
    }

    @SuppressLint("MissingPermission")
    private void xlsxConverter() {
        final String to1 = mFolderPath + "/xlsx_" + CONVERTER_PREFIX + mFileName + ".xlst.bin";
        final String to2 = mFolderPath + "/xlsx_" + CONVERTER_PREFIX + mFileName + ".xlst";
        final String to3 = mFolderPath + "/xlsx_bin_" + CONVERTER_PREFIX + mFileName + ".xlsx";
        final String to4 = mFolderPath + "/xlsx_xlst_" + CONVERTER_PREFIX + mFileName + ".xlsx";
        final String to5 = mFolderPath + "/xlsx_csv_" + CONVERTER_PREFIX + mFileName + ".csv";
        
        final AsyncTask<Void, String, Void> asyncTask = new AsyncTask<Void, String, Void>() {

            @Override
            protected void onPreExecute() {
                super.onPreExecute();
                logMessage("\nStart XLSX converting...");
                mChooseButton.setEnabled(false);
                mConvertButton.setEnabled(false);
            }

            @Override
            protected Void doInBackground(Void... voids) {
                if (Core.xlsx2xlstbin(getBaseContext(), mFilePath, to1)) {
                    publishProgress("Convert PASS: " + to1);
                    if (Core.xlsx2xlst(getBaseContext(), mFilePath, to2)) {
                        publishProgress("Convert PASS: " + to2);
                        if (Core.xlstbin2xlsx(getBaseContext(), to1, to3)) {
                            publishProgress("Convert PASS: " + to3);
                            if (Core.xlst2xlsx(getBaseContext(), to2, to4)) {
                                if (Core.xlsx2csv(getBaseContext(), mFilePath, to5, ",")) {
                                    publishProgress("Convert PASS: " + to5);
                                } else {
                                    publishProgress("Convert FAILED: " + to5);
                                }
                            } else {
                                publishProgress("Convert FAILED: " + to4);
                            }
                        } else {
                            publishProgress("Convert FAILED: " + to3);
                        }
                    } else {
                        publishProgress("Convert FAILED: " + to2);
                    }
                } else {
                    publishProgress("\nConvert FAILED: " + to1);
                }
                
                return null;
            }

            @Override
            protected void onProgressUpdate(String... values) {
                super.onProgressUpdate(values);
                logMessage(values[0]);
            }

            @Override
            protected void onPostExecute(Void aVoid) {
                super.onPostExecute(aVoid);
                showToast("All XLSX converts PASSED!");
                mChooseButton.setEnabled(true);
                mConvertButton.setEnabled(true);
            }
        };
        asyncTask.execute();
    }

    @SuppressLint("MissingPermission")
    private void pptxConverter() {
        final String to1 = mFolderPath + "/pptx_" + CONVERTER_PREFIX + mFileName + ".pptt.bin";
        final String to2 = mFolderPath + "/pptx_" + CONVERTER_PREFIX + mFileName + ".pptt";
        final String to3 = mFolderPath + "/pptx_bin_" + CONVERTER_PREFIX + mFileName + ".pptx";
        final String to4 = mFolderPath + "/pptx_xlst_" + CONVERTER_PREFIX + mFileName + ".pptx";

        final AsyncTask<Void, String, Void> asyncTask = new AsyncTask<Void, String, Void>() {

            @Override
            protected void onPreExecute() {
                super.onPreExecute();
                logMessage("\nStart PPTX converting...");
                mChooseButton.setEnabled(false);
                mConvertButton.setEnabled(false);
            }

            @Override
            protected Void doInBackground(Void... voids) {
                if (Core.pptx2ppttbin(getBaseContext(), mFilePath, to1)) {
                    publishProgress("Convert PASS: " + to1);
                    if (Core.pptx2pptt(getBaseContext(), mFilePath, to2)) {
                        publishProgress("Convert PASS: " + to2);
                        if (Core.ppttbin2pptx(getBaseContext(), to1, to3)) {
                            publishProgress("Convert PASS: " + to3);
                            if (Core.pptt2pptx(getBaseContext(), to2, to4)) {
                                publishProgress("Convert PASS: " + to4);
                            } else {
                                publishProgress("Convert FAILED: " + to4);
                            }
                        } else {
                            publishProgress("Convert FAILED: " + to3);
                        }
                    } else {
                        publishProgress("Convert FAILED: " + to2);
                    }
                } else {
                    publishProgress("\nConvert FAILED: " + to1);
                }
                return null;
            }

            @Override
            protected void onProgressUpdate(String... values) {
                super.onProgressUpdate(values);
                logMessage(values[0]);
            }

            @Override
            protected void onPostExecute(Void aVoid) {
                super.onPostExecute(aVoid);
                showToast("All PPTX converts PASSED!");
                mChooseButton.setEnabled(true);
                mConvertButton.setEnabled(true);
            }
        };
        asyncTask.execute();
    }

    @SuppressLint("MissingPermission")
    private void csvConverter() {
        final String to1 = mFolderPath + "/csv_" + CONVERTER_PREFIX + mFileName + ".xlst";
        final String to2 = mFolderPath + "/csv_" + CONVERTER_PREFIX + mFileName + ".xlsx";
        final String to3 = mFolderPath + "/csv_" + CONVERTER_PREFIX + mFileName + ".xlst.bin";
        final String to4 = mFolderPath + "/csv_xlst_bin_" + CONVERTER_PREFIX + mFileName + ".xlsx";
        final String to5 = mFolderPath + "/csv_xlst_" + CONVERTER_PREFIX + mFileName + ".csv";
        final String to6 = mFolderPath + "/csv_xlsx_" + CONVERTER_PREFIX + mFileName + ".csv";

        final AsyncTask<Void, String, Void> asyncTask = new AsyncTask<Void, String, Void>() {

            @Override
            protected void onPreExecute() {
                super.onPreExecute();
                logMessage("\nStart CSV converting...");
                mChooseButton.setEnabled(false);
                mConvertButton.setEnabled(false);
            }

            @Override
            protected Void doInBackground(Void... voids) {
                if (Core.csv2xlst(getBaseContext(), mFilePath, to1, ",")) {
                    publishProgress("Convert PASS: " + to1);
                    if (Core.csv2xlsx(getBaseContext(), mFilePath, to2, ",")) {
//                publishProgress("Convert PASS: " + to2);
//                if (Core.xlsx2csv(getBaseContext(), to2, to6, ",")) {
//                    publishProgress("Convert PASS: " + to6);
//                    showToast("All CSV converts PASSED!");
//                    return;
//                } else {
//                    publishProgress("Convert FAILED: " + to6);
//                }
                        if (Core.csv2xlstbin(getBaseContext(), mFilePath, to3, ",")) {
                            publishProgress("Convert PASS: " + to3);
                            if (Core.xlstbin2xlsx(getBaseContext(), to3, to4)) {
                                publishProgress("Convert PASS: " + to4);
                                if (Core.xlst2csv(getBaseContext(), to1, to5, ",")) {
                                    publishProgress("Convert PASS: " + to5);
                                    if (Core.xlsx2csv(getBaseContext(), to2, to6, ",")) {
                                        publishProgress("Convert PASS: " + to6);
                                    } else {
                                        publishProgress("Convert FAILED: " + to6);
                                    }
                                } else {
                                    publishProgress("Convert FAILED: " + to5);
                                }
                            } else {
                                publishProgress("Convert FAILED: " + to4);
                            }
                        } else {
                            publishProgress("Convert FAILED: " + to3);
                        }
                    } else {
                        publishProgress("Convert FAILED: " + to2);
                    }
                } else {
                    publishProgress("\nConvert FAILED: " + to1);
                }
                return null;
            }

            @Override
            protected void onProgressUpdate(String... values) {
                super.onProgressUpdate(values);
                logMessage(values[0]);
            }

            @Override
            protected void onPostExecute(Void aVoid) {
                super.onPostExecute(aVoid);
                showToast("All CSV converts PASSED!");
                mChooseButton.setEnabled(true);
                mConvertButton.setEnabled(true);
            }
        };
        asyncTask.execute();
    }

    @SuppressLint("MissingPermission")
    private void txtConverter() {
        final String to1 = mFolderPath + "/" + CONVERTER_PREFIX + mFileName + ".doct.bin";
        final String to2 = mFolderPath + "/" + CONVERTER_PREFIX + mFileName + ".docx";

        final AsyncTask<Void, String, Void> asyncTask = new AsyncTask<Void, String, Void>() {

            @Override
            protected void onPreExecute() {
                super.onPreExecute();
                logMessage("\nStart TXT converting...");
                mChooseButton.setEnabled(false);
                mConvertButton.setEnabled(false);
            }

            @Override
            protected Void doInBackground(Void... voids) {
                if (Core.txt2doctbin(getBaseContext(), mFilePath, to1)) {
                    publishProgress("Convert PASS: " + to1);
                    if (Core.doctbin2docx(getBaseContext(), to1, to2)) {
                        publishProgress("Convert PASS: " + to2);
                    } else {
                        publishProgress("Convert FAILED: " + to2);
                    }
                } else {
                    publishProgress("Convert FAILED: " + to1);
                }
                return null;
            }

            @Override
            protected void onProgressUpdate(String... values) {
                super.onProgressUpdate(values);
                logMessage(values[0]);
            }

            @Override
            protected void onPostExecute(Void aVoid) {
                super.onPostExecute(aVoid);
                showToast("All PPTX converts PASSED!");
                mChooseButton.setEnabled(true);
                mConvertButton.setEnabled(true);
            }
        };
        asyncTask.execute();
    }

    private void showToast(@NonNull final String message) {
        mToast.setText(message);
        mToast.show();
    }

    private void logMessage(@NonNull final String message) {
        Log.d(TAG, message);
        final StringBuffer stringBuffer = new StringBuffer();
        stringBuffer.append(mAppCompatTextView.getText());
        stringBuffer.append("\n").append(message);
        mAppCompatTextView.setText(stringBuffer.toString());
        mScrollView.scrollTo(0, mAppCompatTextView.getBottom());
    }

    private boolean checkWritePermission() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{ Manifest.permission.WRITE_EXTERNAL_STORAGE }, PERMISSION_WRITE_STORAGE);
            return false;
        }

        return true;
    }

    private boolean checkReadPermission(final int requestCode) {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{ Manifest.permission.READ_EXTERNAL_STORAGE }, requestCode);
            return false;
        }

        return true;
    }

    @RequiresPermission(Manifest.permission.READ_EXTERNAL_STORAGE)
    private void browseDocuments(){
        if (checkReadPermission(PERMISSION_CHOOSER_READ_STORAGE)) {
            final Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
            intent.setType("*/*");
            intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
            startActivityForResult(Intent.createChooser(intent, getString(R.string.button_choose)), REQUEST_CODE_DOC);
        }
    }

    private static String getExtension(final String path) {
        try {
            return path.substring(path.lastIndexOf(".") + 1);
        } catch (RuntimeException e) {
            return "";
        }
    }

    @SuppressLint("NewApi")
    public String getFilePath(Uri uri) {
        String selection = null;
        String[] selectionArgs = null;

        // Uri is different in versions after KITKAT (Android 4.4), we need to
        if (Build.VERSION.SDK_INT >= 19 && DocumentsContract.isDocumentUri(getBaseContext(), uri)) {
            if (isExternalStorageDocument(uri)) {
                final String docId = DocumentsContract.getDocumentId(uri);
                final String[] split = docId.split(":");
                return Environment.getExternalStorageDirectory() + "/" + split[1];
            } else if (isDownloadsDocument(uri)) {
                final String id = DocumentsContract.getDocumentId(uri);
                if (id.startsWith("raw:")) {
                    return id.replaceFirst("raw:", "");
                }
                uri = ContentUris.withAppendedId(Uri.parse("content://downloads/public_downloads"), Long.valueOf(id));
            } else if (isMediaDocument(uri)) {
                final String docId = DocumentsContract.getDocumentId(uri);
                final String[] split = docId.split(":");
                final String type = split[0];

                if ("image".equals(type)) {
                    uri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
                } else if ("video".equals(type)) {
                    uri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
                } else if ("audio".equals(type)) {
                    uri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
                }

                selection = "_id=?";
                selectionArgs = new String[]{
                        split[1]
                };
            }
        }

        if ("content".equalsIgnoreCase(uri.getScheme())) {
            String[] projection = {
                    MediaStore.Images.Media.DATA
            };
            Cursor cursor = null;
            try {
                cursor = getBaseContext().getContentResolver().query(uri, projection, selection, selectionArgs, null);
                int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
                if (cursor.moveToFirst()) {
                    return cursor.getString(column_index);
                }
            } catch (Exception e) {
            }
        } else if ("file".equalsIgnoreCase(uri.getScheme())) {
            return uri.getPath();
        }

        return null;
    }

    public static boolean isExternalStorageDocument(Uri uri) {
        return "com.android.externalstorage.documents".equals(uri.getAuthority());
    }

    public static boolean isDownloadsDocument(Uri uri) {
        return "com.android.providers.downloads.documents".equals(uri.getAuthority());
    }

    public static boolean isMediaDocument(Uri uri) {
        return "com.android.providers.media.documents".equals(uri.getAuthority());
    }

}
