package app.x2t

import android.Manifest
import android.annotation.SuppressLint
import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Toast
import androidx.annotation.RequiresPermission
import androidx.appcompat.app.AppCompatActivity
import app.x2t.utils.AsyncRoutines
import app.x2t.utils.FileUtils
import app.x2t.utils.PathUtils
import app.x2t.utils.PermissionUtils
import kotlinx.android.synthetic.main.activity_converter.*
import kotlinx.coroutines.launch
import lib.x2t.X2t

class ConverterActivity : AppCompatActivity(), View.OnClickListener {

    companion object {
        val TAG = ConverterActivity::class.java.simpleName
        private const val REQUEST_CODE_DOC = 1
        private const val CONVERTER_PREFIX = "converter_"
        private const val PERMISSION_READ_WRITE_STORAGE = 1
        private const val PERMISSION_CHOOSER_READ_STORAGE = 2
    }

    private val mToast: Toast by lazy {
        Toast.makeText(this, "", Toast.LENGTH_LONG)
    }

    private val mAsyncRoutines = AsyncRoutines()
    private var mFilePath: String? = null
    private var mFileName: String? = null
    private var mFolderPath: String? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_converter)
        convertButton.setOnClickListener(this)
        chooseButton.setOnClickListener(this)
    }

    @SuppressLint("MissingPermission")
    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        when (requestCode) {
            PERMISSION_READ_WRITE_STORAGE -> {
                if (PermissionUtils.checkReadWritePermission(this)) {
                    converter()
                }
            }

            PERMISSION_CHOOSER_READ_STORAGE -> {
                if (PermissionUtils.checkReadPermission(this)) {
                    browseDocuments()
                }
            }
        }
    }

    public override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (resultCode == Activity.RESULT_OK) {
            when (requestCode) {
                REQUEST_CODE_DOC -> {
                    if (data != null && data.data != null) {
                        mFilePath = PathUtils.getPath(this, data.data!!)

                        if (mFilePath != null) {
                            mFolderPath = mFilePath!!.substring(0, mFilePath!!.lastIndexOf("/"))
                            mFileName = mFilePath!!.replace(mFolderPath!! + "/", "")
                            logMessage("\n-------------------------------------------------")
                            logMessage("Source file path: " + mFilePath!!)
                            logMessage("Source file name: " + mFileName!!)
                            logMessage("Source folder path: " + mFolderPath!!)
                        } else {
                            showToast("UNKNOWN ERROR!")
                            logMessage("\nUNKNOWN ERROR!\n")
                        }
                    }
                }
            }
        }
    }

    @SuppressLint("MissingPermission")
    override fun onClick(view: View) {
        when (view.id) {
            R.id.convertButton -> converter()
            R.id.chooseButton -> browseDocuments()
        }
    }

    private fun converter() {
        if (PermissionUtils.requestReadWritePermission(this, PERMISSION_READ_WRITE_STORAGE)) {
            if (mFilePath != null && mFileName != null && mFolderPath != null) {
                when (FileUtils.getExtension(mFilePath!!).toUpperCase()) {
                    "DOCX" -> docxConverter()
                    "XLSX" -> xlsxConverter()
                    "PPTX" -> pptxConverter()
                    "CSV" -> csvConverter()
                    "TXT" -> txtConverter()
                    else -> showToast("Unsupported format!")
                }
            } else {
                showToast("Choose documents!")
            }
        }
    }

    @SuppressLint("MissingPermission")
    private fun docxConverter() {
        val to1 = "$mFolderPath/docx_$CONVERTER_PREFIX$mFileName.doct.bin"
        val to2 = "$mFolderPath/docx_$CONVERTER_PREFIX$mFileName.doct"
        val to3 = "$mFolderPath/docx_bin_$CONVERTER_PREFIX$mFileName.docx"
        val to4 = "$mFolderPath/docx_doct_$CONVERTER_PREFIX$mFileName.docx"

        val builder = X2t.builder().setContext(applicationContext)

        logMessage("\nStart DOCX converting...")
        chooseButton.isEnabled = false
        convertButton.isEnabled = false

        mAsyncRoutines.run({ foreground, instance ->
                if (builder.setConvert(X2t.ConvertType.DOCX_DOCTBIN).setFrom(mFilePath!!).setTo(to1).convert()?.isSuccess) {
                    foreground.launch { logMessage("ConvertType PASS: $to1") }

                    if (builder.setConvert(X2t.ConvertType.DOCX_DOCT).setFrom(mFilePath!!).setTo(to2).convert()?.isSuccess) {
                        foreground.launch { logMessage("ConvertType PASS: $to2") }

                        if (builder.setConvert(X2t.ConvertType.DOCTBIN_DOCX).setFrom(to1).setTo(to3).convert()?.isSuccess) {
                            foreground.launch { logMessage("ConvertType PASS: $to3") }

                            if (builder.setConvert(X2t.ConvertType.DOCT_DOCX).setFrom(to2).setTo(to4).convert()?.isSuccess) {
                                foreground.launch { logMessage("ConvertType PASS: $to4") }
                            } else {
                                foreground.launch { logMessage("ConvertType FAILED: $to4") }
                            }
                        } else {
                            foreground.launch { logMessage("ConvertType FAILED: $to3") }
                        }
                    } else {
                        foreground.launch { logMessage("ConvertType FAILED: $to2") }
                    }
                } else {
                    foreground.launch { logMessage("\nConvertType FAILED: $to1") }
                }
            },
            {
                showToast("All DOCX converts PASSED!")
                chooseButton.isEnabled = true
                convertButton.isEnabled = true
            },

            {
                showToast("All DOCX converts FAILED!")
                chooseButton.isEnabled = true
                convertButton.isEnabled = true
            }
        )
    }

    @SuppressLint("MissingPermission")
    private fun xlsxConverter() {
        val to1 = "$mFolderPath/xlsx_$CONVERTER_PREFIX$mFileName.xlst.bin"
        val to2 = "$mFolderPath/xlsx_$CONVERTER_PREFIX$mFileName.xlst"
        val to3 = "$mFolderPath/xlsx_bin_$CONVERTER_PREFIX$mFileName.xlsx"
        val to4 = "$mFolderPath/xlsx_xlst_$CONVERTER_PREFIX$mFileName.xlsx"
        val to5 = "$mFolderPath/xlsx_csv_$CONVERTER_PREFIX$mFileName.csv"

        val builder = X2t.builder().setContext(applicationContext)
        val inputParams = X2t.InputParams()
        inputParams.delimiterChar = ","

        logMessage("\nStart XLSX converting...")
        chooseButton.isEnabled = false
        convertButton.isEnabled = false

        mAsyncRoutines.run({ foreground, instance ->
                if (builder.setConvert(X2t.ConvertType.XLSX_XLSTBIN).setFrom(mFilePath!!).setTo(to1).convert()?.isSuccess) {
                    foreground.launch { logMessage("ConvertType PASS: $to1") }

                    if (builder.setConvert(X2t.ConvertType.XLSX_XLST).setFrom(mFilePath!!).setTo(to2).convert()?.isSuccess) {
                        foreground.launch { logMessage("ConvertType PASS: $to2") }

                        if (builder.setConvert(X2t.ConvertType.XLSTBIN_XLSX).setFrom(to1).setTo(to3).convert()?.isSuccess) {
                            foreground.launch { logMessage("ConvertType PASS: $to3") }

                            if (builder.setConvert(X2t.ConvertType.XLST_XLSX).setFrom(to2).setTo(to4).convert()?.isSuccess) {
                                foreground.launch { logMessage("ConvertType PASS: $to4") }

                                if (builder.setConvert(X2t.ConvertType.XLSX_CSV).setInputParams(inputParams).setFrom(mFilePath!!).setTo(to5).convert()?.isSuccess) {
                                    foreground.launch { logMessage("ConvertType PASS: $to5") }
                                } else {
                                    foreground.launch { logMessage("ConvertType FAILED: $to5") }
                                }
                            } else {
                                foreground.launch { logMessage("ConvertType FAILED: $to4") }
                            }
                        } else {
                            foreground.launch { logMessage("ConvertType FAILED: $to3") }
                        }
                    } else {
                        foreground.launch { logMessage("ConvertType FAILED: $to2") }
                    }
                } else {
                    foreground.launch { logMessage("\nConvertType FAILED: $to1") }
                }
            },

            {
                showToast("All XLSX converts PASSED!")
                chooseButton.isEnabled = true
                convertButton.isEnabled = true
            },

            {
                showToast("All XLSX converts FAILED!")
                chooseButton.isEnabled = true
                convertButton.isEnabled = true
            }
        )
    }

    @SuppressLint("MissingPermission")
    private fun pptxConverter() {
        val to1 = "$mFolderPath/pptx_$CONVERTER_PREFIX$mFileName.pptt.bin"
        val to2 = "$mFolderPath/pptx_$CONVERTER_PREFIX$mFileName.pptt"
        val to3 = "$mFolderPath/pptx_bin_$CONVERTER_PREFIX$mFileName.pptx"
        val to4 = "$mFolderPath/pptx_xlst_$CONVERTER_PREFIX$mFileName.pptx"
        val to5 = "$mFolderPath/pptx_odp_$CONVERTER_PREFIX$mFileName.odp"

        val builder = X2t.builder().setContext(applicationContext)

        logMessage("\nStart PPTX converting...")
        chooseButton.isEnabled = false
        convertButton.isEnabled = false

        mAsyncRoutines.run({ foreground, instance ->
                if (builder.setConvert(X2t.ConvertType.PPTX_PPTTBIN).setFrom(mFilePath!!).setTo(to1).convert()?.isSuccess) {
                    foreground.launch { logMessage("ConvertType PASS: $to1") }

                    if (builder.setConvert(X2t.ConvertType.PPTX_PPTT).setFrom(mFilePath!!).setTo(to2).convert()?.isSuccess) {
                        foreground.launch { logMessage("ConvertType PASS: $to2") }

                        if (builder.setConvert(X2t.ConvertType.PPTTBIN_PPTX).setFrom(to1).setTo(to3).convert()?.isSuccess) {
                            foreground.launch { logMessage("ConvertType PASS: $to3") }

                            if (builder.setConvert(X2t.ConvertType.PPTT_PPTX).setFrom(to2).setTo(to4).convert()?.isSuccess) {
                                foreground.launch { logMessage("ConvertType PASS: $to4") }

                                if (builder.setConvert(X2t.ConvertType.PPTX_ODP).setFrom(to4).setTo(to5).convert()?.isSuccess) {
                                    foreground.launch { logMessage("ConvertType PASS: $to5") }
                                } else {
                                    foreground.launch { logMessage("ConvertType FAILED: $to5") }
                                }
                            } else {
                                foreground.launch { logMessage("ConvertType FAILED: $to4") }
                            }
                        } else {
                            foreground.launch { logMessage("ConvertType FAILED: $to3") }
                        }
                    } else {
                        foreground.launch { logMessage("ConvertType FAILED: $to2") }
                    }
                } else {
                    foreground.launch { logMessage("\nConvertType FAILED: $to1") }
                }
            },

            {
                showToast("All PPTX converts PASSED!")
                chooseButton.isEnabled = true
                convertButton.isEnabled = true
            },

            {
                showToast("All PPTX converts FAILED!")
                chooseButton.isEnabled = true
                convertButton.isEnabled = true
            }
        )
    }

    @SuppressLint("MissingPermission")
    private fun csvConverter() {
        val to1 = "$mFolderPath/csv_$CONVERTER_PREFIX$mFileName.xlst"
        val to2 = "$mFolderPath/csv_$CONVERTER_PREFIX$mFileName.xlsx"
        val to3 = "$mFolderPath/csv_$CONVERTER_PREFIX$mFileName.xlst.bin"
        val to4 = "$mFolderPath/csv_xlst_bin_$CONVERTER_PREFIX$mFileName.xlsx"
        val to5 = "$mFolderPath/csv_xlst_$CONVERTER_PREFIX$mFileName.csv"
        val to6 = "$mFolderPath/csv_xlsx_$CONVERTER_PREFIX$mFileName.csv"

        val builder = X2t.builder().setContext(applicationContext)
        val inputParams = X2t.InputParams()
        inputParams.delimiterChar = ","

        logMessage("\nStart CSV converting...")
        chooseButton.isEnabled = false
        convertButton.isEnabled = false

        mAsyncRoutines.run({ foreground, instance ->
                foreground.launch { logMessage("CSV available encodings:") }
                X2t.getEncodings()?.forEach {
                    foreground.launch { logMessage("    $it") }
                }

                if (builder.setConvert(X2t.ConvertType.CSV_XLST).setInputParams(inputParams).setFrom(mFilePath!!).setTo(to1).convert()?.isSuccess) {
                    foreground.launch { logMessage("ConvertType PASS: $to1") }

                    if (builder.setConvert(X2t.ConvertType.CSV_XLSX).setInputParams(inputParams).setFrom(mFilePath!!).setTo(to2).convert()?.isSuccess) {
                        foreground.launch { logMessage("ConvertType PASS: $to2") }

                        if (builder.setConvert(X2t.ConvertType.CSV_XLSTBIN).setInputParams(inputParams).setFrom(mFilePath!!).setTo(to3).convert()?.isSuccess) {
                            foreground.launch { logMessage("ConvertType PASS: $to3") }

                            if (builder.setConvert(X2t.ConvertType.XLSTBIN_XLSX).setFrom(to3).setTo(to4).convert()?.isSuccess) {
                                foreground.launch { logMessage("ConvertType PASS: $to4") }

                                if (builder.setConvert(X2t.ConvertType.XLST_CSV).setInputParams(inputParams).setFrom(to1).setTo(to5).convert()?.isSuccess) {
                                    foreground.launch { logMessage("ConvertType PASS: $to5") }

                                    if (builder.setConvert(X2t.ConvertType.XLSX_CSV).setInputParams(inputParams).setFrom(to2).setTo(to6).convert()?.isSuccess) {
                                        foreground.launch { logMessage("ConvertType PASS: $to6") }
                                    } else {
                                        foreground.launch { logMessage("ConvertType FAILED: $to6") }
                                    }
                                } else {
                                    foreground.launch { logMessage("ConvertType FAILED: $to5") }
                                }
                            } else {
                                foreground.launch { logMessage("ConvertType FAILED: $to4") }
                            }
                        } else {
                            foreground.launch { logMessage("ConvertType FAILED: $to3") }
                        }
                    } else {
                        foreground.launch { logMessage("ConvertType FAILED: $to2") }
                    }
                } else {
                    foreground.launch { logMessage("\nConvertType FAILED: $to1") }
                }
            },

            {
                showToast("All CSV converts PASSED!")
                chooseButton.isEnabled = true
                convertButton.isEnabled = true
            },

            {
                showToast("All CSV converts FAILED!")
                chooseButton.isEnabled = true
                convertButton.isEnabled = true
            }
        )
    }

    @SuppressLint("MissingPermission")
    private fun txtConverter() {
        val to1 = "$mFolderPath/$CONVERTER_PREFIX$mFileName.doct.bin"
        val to2 = "$mFolderPath/$CONVERTER_PREFIX$mFileName.docx"

        val builder = X2t.builder().setContext(applicationContext)

        logMessage("\nStart TXT converting...")
        chooseButton.isEnabled = false
        convertButton.isEnabled = false

        mAsyncRoutines.run({ foreground, instance ->
                if (builder.setConvert(X2t.ConvertType.TXT_DOCTBIN).setFrom(mFilePath!!).setTo(to1).convert()?.isSuccess) {
                    foreground.launch { logMessage("ConvertType PASS: $to1") }

                    if (builder.setConvert(X2t.ConvertType.DOCTBIN_DOCX).setFrom(to1).setTo(to2).convert()?.isSuccess) {
                        foreground.launch { logMessage("ConvertType PASS: $to2") }
                    } else {
                        foreground.launch { logMessage("ConvertType FAILED: $to2") }
                    }
                } else {
                    foreground.launch { logMessage("ConvertType FAILED: $to1") }
                }
            },

            {
                showToast("All TXT converts PASSED!")
                chooseButton.isEnabled = true
                convertButton.isEnabled = true
            },

            {
                showToast("All TXT converts FAILED!")
                chooseButton.isEnabled = true
                convertButton.isEnabled = true
            }
        )
    }

    private fun showToast(message: String) {
        mToast.setText(message)
        mToast.show()
    }

    private fun logMessage(message: String) {
        Log.d(TAG, message)
        val stringBuffer = StringBuffer()
        stringBuffer.append(loggerText.text)
        stringBuffer.append("\n").append(message)
        loggerText.text = stringBuffer.toString()
        scrollView.scrollTo(0, loggerText.bottom)
    }

    @RequiresPermission(Manifest.permission.READ_EXTERNAL_STORAGE)
    private fun browseDocuments() {
        if (PermissionUtils.requestReadPermission(this, PERMISSION_CHOOSER_READ_STORAGE)) {
            val intent = Intent(Intent.ACTION_GET_CONTENT)
            intent.type = "*/*"
            intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
            startActivityForResult(Intent.createChooser(intent, getString(R.string.button_choose)), REQUEST_CODE_DOC)
        }
    }

}
