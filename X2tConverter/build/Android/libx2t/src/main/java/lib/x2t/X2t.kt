package lib.x2t

import android.Manifest.permission.READ_EXTERNAL_STORAGE
import android.Manifest.permission.WRITE_EXTERNAL_STORAGE
import android.annotation.SuppressLint
import android.content.Context
import androidx.annotation.RequiresPermission
import lib.x2t.data.Encoding
import lib.x2t.utils.FileUtils
import java.io.File

class X2t private constructor() {

    companion object {

        const val CONVERTER_CODE_FAIL = -1
        const val CONVERTER_CODE_SUCCESS = 0
        const val CONVERTER_CODE_EXIST = 1
        const val CONVERTED_CODE_PASSWORD = -2147216550
        const val CONVERTER_CODE_INVALID_PASSWORD = -2147216549

        init {
            System.loadLibrary(BuildConfig.LIB_X2T)
        }


        /*
         * Encoding
         * */
        @JvmStatic
        external fun getEncodings(): Array<Encoding>?


        /*
         * Native ODF
         * */
        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun odf2oox(inputParams: InputParams): Int

        /*
         * Native DOCX
         * */
        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun docx2doctbin(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun docx2doct(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun docx2odt(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun docxdir2txt(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun docx2rtf(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun doctbin2docx(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun doct2docx(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun rtf2docx(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun txt2docx(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun rtf2doctbin(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun doc2doctbin(inputParams: InputParams): Int


        /*
         * Native XLSX
         * */
        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun xlsx2xlstbin(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun xlsx2xlst(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun xlsx2ods(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun xlstbin2xlsx(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun xlst2xlsx(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun xls2xlstbin(inputParams: InputParams): Int


        /*
         * Native PPTX
         * */
        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun pptx2ppttbin(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun pptx2pptt(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun ppttbin2pptx(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun pptt2pptx(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun pptx2odp(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun pptx2DirOdp(inputParams: InputParams, boolean: Boolean): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun ppt2ppttbin(inputParams: InputParams): Int


        /*
         * Native CSV
         * */
        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun csv2xlst(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun csv2xlsx(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun csv2xlstbin(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun xlst2csv(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun xlsx2csv(inputParams: InputParams): Int


        /*
         * Native TXT
         * */
        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun txt2doctbin(inputParams: InputParams): Int

        /*
         * Native ZIP
         * */
        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun dir2zip(inputParams: InputParams): Int

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun zip2dir(inputParams: InputParams): Int

        /*
         * Native BIN
         * */
        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun bin2pdf(inputParams: InputParams): Int

        /*
         * Builder for converter
         * */
        @JvmStatic
        fun builder(): Builder {
            return X2t().Builder()
        }
    }


    /*
    * Context mTo form paths
    * */
    private var mContext: Context? = null

    /*
    * ConvertType type
    * */
    private var mConvertType = ConvertType.NONE

    /*
    * Input params for native converter (passed mTo x2t.jni).
    * */
    private var mInputParams = InputParams()

    /*
    * Clear temp data after converting
    * */
    private var mIsClearTemp = false

    /*
    * Cleat temp data after converting
    * */
    private var mIsOverwrite = false

    /*
    * File name to converting
    * */
    private var mConvertFileName: String = "Editor.bin"

    /*
    * Folder name to converting
    * */
    private var mConvertFolderName: String? = null


    /*
    * Helpers for convert
    * Do not modify fields names, else JNI will not work!
    * */
    class InputParams {

        companion object {
            const val ENCODING_CODE_DEFAULT = 65001

            const val DELIMITER_CODE_NONE = 0
            const val DELIMITER_CODE_TAB = 1
            const val DELIMITER_CODE_SEMICOLON = 2
            const val DELIMITER_CODE_COLON = 3
            const val DELIMITER_CODE_COMMA = 4
            const val DELIMITER_CODE_SPACE = 5

            const val DELIMITER_CHAR_TAB = "\t"
            const val DELIMITER_CHAR_SEMICOLON = ";"
            const val DELIMITER_CHAR_COLON = ":"
            const val DELIMITER_CHAR_COMMA = ","
            const val DELIMITER_CHAR_SPACE = " "
        }

        var from: String? = null
        var to: String? = null
        var temp: String? = null
        var fontsDir: String? = null
        var fontsJs: String? = null
        var theme: String? = null
        var password: String? = null
        var delimiterChar: String? = null

        var delimiterCode: Int = DELIMITER_CODE_NONE
        var encoding: Int = 0
        var formatFrom: Int = 0
        var formatTo: Int = 0

        var isNoBase64:Boolean = false
        var isFromChange:Boolean = false
        var isPaid:Boolean = false
        var isTemplate:Boolean = false
    }


    class ConvertResult {
        var to: String? = null
        var root: String? = null
        var code = CONVERTER_CODE_FAIL
        val isSuccess: Boolean
            get() = code == CONVERTER_CODE_SUCCESS
    }


    enum class ConvertType {
        NONE,

        DOCX_DOCTBIN,
        DOCX_DOCT,
        DOCX_ODT,
        DOCXDIR_TXT,
        DOCX_RTF,
        DOCTBIN_DOCX,
        DOCTBIN_ODT,
        DOCTBIN_RTF,
        DOCT_DOCX,
        RTF_DOCX,
        TXT_DOCX,
        RTF_DOCTBIN,
        ODT_DOCX,
        ODT_DOCTBIN,
        DOC_DOCTBIN,

        XLSX_XLSTBIN,
        XLSX_XLST,
        XLSX_ODS,
        XLSTBIN_XLSX,
        XLSTBIN_ODS,
        XLSTBIN_CSV,
        XLST_XLSX,
        ODS_XLSX,
        ODS_XLSTBIN,
        XLS_XLSTBIN,

        PPTX_PPTTBIN,
        PPTX_PPTT,
        PPTX_ODP,
        PPTTBIN_PPTX,
        PPTT_PPTX,
        PPTTBIN_ODP,
        PPTX_DIR2ODP,
        ODP_PPTX,
        ODP_PPTTBIN,
        PPT_PPTTBIN,

        CSV_XLST,
        CSV_XLSX,
        CSV_XLSTBIN,
        XLST_CSV,
        XLSX_CSV,
        TXT_DOCTBIN,

        DIR_ZIP,
        ZIP_DIR,

        BIN_PDF
    }

    inner class Builder {

        fun setContext(value: Context): Builder {
            mContext = value
            return this
        }

        fun setIsClearTemp(value: Boolean): Builder {
            mIsClearTemp = value
            return this
        }

        fun setIsOverwrite(value: Boolean): Builder {
            mIsOverwrite = value
            return this
        }

        fun setConvertType(value: ConvertType): Builder {
            mConvertType = value
            return this
        }

        fun setFromPath(value: String): Builder {
            mInputParams.from = value
            return this
        }

        fun setToPath(value: String): Builder {
            mInputParams.to = value
            return this
        }

        fun setToName(value: String = "Editor.bin"): Builder {
            mConvertFileName = value
            return this
        }

        fun setToFolder(value: String? = null): Builder {
            mConvertFolderName = value
            return this
        }

        fun setInputParams(value: InputParams): Builder {
            mInputParams = value
            return this
        }

        fun getInputParams() = mInputParams


        @SuppressLint("MissingPermission")
        fun convert(): ConvertResult {
            if (mContext == null) {
                throw RuntimeException("Context for convertation must be set!")
            }

            if (mConvertType == ConvertType.NONE) {
                throw RuntimeException("Converter type must be set!")
            }

            if (mInputParams.from == null) {
                throw RuntimeException("Converted file must be set!")
            }

            return this@X2t.convert(mContext!!)
        }

    }


    @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
    fun convert(context: Context): ConvertResult {
        val result = ConvertResult()

        FileUtils.getCache(context, mConvertFileName, mConvertFolderName)?.let { cache ->
            result.to = mInputParams.to ?: cache.to
            result.root = File(result.to).parent

            mInputParams.to = mInputParams.to ?: cache.to
            mInputParams.temp = mInputParams.temp ?: cache.temp
            mInputParams.theme = mInputParams.theme ?: "${cache.root}/theme"

            if (File(mInputParams.to).exists()) {
                if (mIsOverwrite) {
                    FileUtils.deletePath(result.to!!)
                } else {
                    result.code = CONVERTER_CODE_EXIST
                    return@let
                }
            }

            FileUtils.createPath(result.root!!)
            FileUtils.createPath(mInputParams.temp!!)

            mInputParams.theme?.let { theme ->
                FileUtils.createPath(theme)
            }

            when (mConvertType) {
                ConvertType.DOCX_DOCTBIN -> result.code = docx2doctbin(mInputParams)
                ConvertType.DOCX_DOCT -> result.code = docx2doct(mInputParams)
                ConvertType.DOCX_ODT -> result.code = docx2odt(mInputParams)
                ConvertType.DOCX_RTF -> result.code = docx2rtf(mInputParams)
                ConvertType.DOCXDIR_TXT -> result.code = docxdir2txt(mInputParams)
                ConvertType.DOCTBIN_DOCX -> result.code = doctbin2docx(mInputParams)
                ConvertType.DOCT_DOCX -> result.code = doct2docx(mInputParams)
                ConvertType.DOC_DOCTBIN -> result.code = doc2doctbin(mInputParams)
                ConvertType.RTF_DOCX -> result.code = rtf2docx(mInputParams)
                ConvertType.TXT_DOCX -> result.code = txt2docx(mInputParams)
                ConvertType.ODT_DOCX -> result.code = odf2oox(mInputParams)
                ConvertType.RTF_DOCTBIN -> result.code = rtf2doctbin(mInputParams)
                ConvertType.DOCTBIN_ODT -> result.code = doubleStepConvert(context, mInputParams, {
                    doctbin2docx(it)
                }, {
                    docx2odt(it)
                })

                ConvertType.DOCTBIN_RTF -> result.code = doubleStepConvert(context, mInputParams, {
                    doctbin2docx(it)
                }, {
                    docx2rtf(it)
                })

                ConvertType.ODT_DOCTBIN -> result.code = doubleStepConvert(context, mInputParams, {
                    odf2oox(it)
                }, {
                    docx2doctbin(it)
                })


                ConvertType.XLSX_XLSTBIN -> result.code = xlsx2xlstbin(mInputParams)
                ConvertType.XLSX_XLST -> result.code = xlsx2xlst(mInputParams)
                ConvertType.XLSX_ODS -> result.code = xlsx2ods(mInputParams)
                ConvertType.XLSTBIN_XLSX -> result.code = xlstbin2xlsx(mInputParams)
                ConvertType.XLST_XLSX -> result.code = xlst2xlsx(mInputParams)
                ConvertType.XLS_XLSTBIN -> result.code = xls2xlstbin(mInputParams)
                ConvertType.ODS_XLSX -> result.code = odf2oox(mInputParams)
                ConvertType.XLSTBIN_ODS -> result.code = doubleStepConvert(context, mInputParams, {
                    xlstbin2xlsx(it)
                }, {
                    xlsx2ods(it)
                })

                ConvertType.XLSTBIN_CSV -> result.code = doubleStepConvert(context, mInputParams, {
                    xlstbin2xlsx(it)
                }, {
                    it.delimiterCode = InputParams.DELIMITER_CODE_COMMA
                    xlsx2csv(it)
                })

                ConvertType.ODS_XLSTBIN -> result.code = doubleStepConvert(context, mInputParams, {
                    odf2oox(it)
                }, {
                    xlsx2xlstbin(it)
                })


                ConvertType.PPTX_PPTTBIN -> result.code = pptx2ppttbin(mInputParams)
                ConvertType.PPTX_PPTT -> result.code = pptx2pptt(mInputParams)
                ConvertType.PPTX_ODP -> result.code = pptx2odp(mInputParams)
                ConvertType.PPTTBIN_PPTX -> result.code = ppttbin2pptx(mInputParams)
                ConvertType.PPTT_PPTX -> result.code = pptt2pptx(mInputParams)
                ConvertType.PPTX_DIR2ODP-> result.code = pptx2DirOdp(mInputParams, mInputParams.isTemplate)
                ConvertType.PPT_PPTTBIN -> result.code = ppt2ppttbin(mInputParams)
                ConvertType.ODP_PPTX -> result.code = odf2oox(mInputParams)
                ConvertType.PPTTBIN_ODP -> result.code = doubleStepConvert(context, mInputParams, {
                    ppttbin2pptx(it)
                }, {
                    pptx2odp(it)
                })

                ConvertType.ODP_PPTTBIN -> result.code = doubleStepConvert(context, mInputParams, {
                    odf2oox(it)
                }, {
                    pptx2ppttbin(it)
                })


                ConvertType.CSV_XLST -> result.code = csv2xlst(mInputParams)
                ConvertType.CSV_XLSX -> result.code = csv2xlsx(mInputParams)
                ConvertType.CSV_XLSTBIN -> result.code = csv2xlstbin(mInputParams)
                ConvertType.XLST_CSV -> result.code = xlst2csv(mInputParams)
                ConvertType.XLSX_CSV -> result.code = xlsx2csv(mInputParams)
                ConvertType.TXT_DOCTBIN -> result.code = txt2doctbin(mInputParams)
                ConvertType.DIR_ZIP -> result.code = dir2zip(mInputParams)
                ConvertType.ZIP_DIR -> result.code = zip2dir(mInputParams)
                ConvertType.BIN_PDF -> result.code = bin2pdf(mInputParams)
            }

            FileUtils.deletePath(cache.temp!!)

            if (mIsClearTemp) {
                FileUtils.deletePath(cache.root!!)
            }
        }

        return result
    }

    @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
    private fun doubleStepConvert(context: Context, inputParams: InputParams, first: (InputParams) -> Int, second: (InputParams) -> Int): Int {
        return FileUtils.getCache(context)?.let { cache1 ->
            val inputTemp = InputParams().apply {
                this.from = inputParams.from
                this.to = cache1.to
                this.temp = cache1.temp
                this.theme = inputParams.theme
                this.password = inputParams.password
            }

            var code = first.invoke(inputTemp)
            if (code == CONVERTER_CODE_SUCCESS) {
                code = FileUtils.getCache(context)?.let { cache2 ->
                    val inputResult = InputParams().apply {
                        this.from = cache1.to
                        this.to = inputParams.to
                        this.temp = cache2.temp
                        this.theme = "${cache2.root}/theme"
                        this.password = inputParams.password
                    }

                    if (!FileUtils.createPath(inputResult.theme!!)) {
                        return CONVERTER_CODE_FAIL
                    }

                    val code = second.invoke(inputResult)
                    FileUtils.deletePath(cache2.root!!)
                    code
                } ?: CONVERTER_CODE_FAIL
            }

            FileUtils.deletePath(cache1.root!!)
            return code

        } ?: CONVERTER_CODE_FAIL
    }

}
