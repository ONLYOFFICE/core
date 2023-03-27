package lib.x2t

import android.Manifest.permission.READ_EXTERNAL_STORAGE
import android.Manifest.permission.WRITE_EXTERNAL_STORAGE
import android.annotation.SuppressLint
import android.content.Context
import android.util.Xml
import androidx.annotation.RequiresPermission
import lib.x2t.data.Encoding
import lib.x2t.utils.FileUtils
import org.xmlpull.v1.XmlSerializer
import java.io.*
import java.util.*


class X2t private constructor() {

    companion object {

        const val CONVERTER_CODE_FAIL = -1
        const val CONVERTER_CODE_SUCCESS = 0
        const val CONVERTER_CODE_EXIST = 1
        const val CONVERTER_CODE_ERROR_XML = 88
        const val CONVERTED_CODE_PASSWORD = 90
        const val CONVERTER_CODE_INVALID_PASSWORD = 91

        init {
            System.loadLibrary(BuildConfig.LIB_X2T)
        }


        /*
         * Encoding
         * */
        @JvmStatic
        external fun getEncodings(): Array<Encoding>?

        /*
         * OfficeFileFormats
         * */
        @JvmStatic
        external fun getOfficeFileFormats(): HashMap<String, String>

        /*
        * SetFonts
        * */
        @JvmStatic
        external fun setFonts(fontsPaths: Array<String?>, cacheFontsPath: String?)

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun convertFile(pathXml: String): Int

        /*
         * SetIcuDataPath
         * */
        @JvmStatic
        external fun setIcuDataPath(icuDataPath: String?)

        /*
         * Builder for converter
         * */
        @JvmStatic
        fun builder(): Builder {
            return X2t().Builder()
        }

        val ConvertType: HashMap<String, String> by lazy {
            getOfficeFileFormats()
        }
    }


    /*
    * Context mTo form paths
    * */
    private var context: Context? = null

    /*
    * ConvertType type
    * */
    private var convertType: String? = null

    /*
    * Input params for native converter (passed mTo x2t.jni).
    * */
    private var inputParams = InputParams()

    /*
    * Clear temp data after converting
    * */
    private var isClearTemp = false

    /*
    * Cleat temp data after converting
    * */
    private var isOverwrite = false

    /*
    * File name to converting
    * */
    private var convertFileName: String = "Editor.bin"

    /*
    * Folder name to converting
    * */
    private var convertFolderName: String? = null

    private var isSave: Boolean = false;

    /*
    * Helpers for convert
    * Do not modify fields names, else JNI will not work!
    * */
    class InputParams {

        companion object {
            const val ENCODING_INDEX_DEFAULT = 46

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

        var key: String? = null
        var from: String? = null
        var to: String? = null
        var temp: String? = null
        var fontsDir: String? = null
        var fontsJs: String? = null
        var themes: String? = null
        var icu: String? = null
        var password: String? = null
        var xml: String? = null

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


    inner class Builder {

        fun setContext(value: Context): Builder {
            context = value
            return this
        }

        fun setIsClearTemp(value: Boolean): Builder {
            isClearTemp = value
            return this
        }

        fun setIsOverwrite(value: Boolean): Builder {
            isOverwrite = value
            return this
        }

        fun setConvertType(value: String?): Builder {
            convertType = value
            return this
        }

        fun setFromPath(value: String): Builder {
            inputParams.from = value
            return this
        }

        fun setToPath(value: String): Builder {
            inputParams.to = value
            return this
        }

        fun setToName(value: String = "Editor.bin"): Builder {
            convertFileName = value
            return this
        }

        fun setToFolder(value: String? = null): Builder {
            convertFolderName = value
            return this
        }

        fun setInputParams(value: InputParams): Builder {
            inputParams = value
            return this
        }

        fun setSave(isSave: Boolean): Builder {
            this@X2t.isSave = isSave
            return this
        }


        fun getInputParams() = inputParams


        @SuppressLint("MissingPermission")
        fun convert(): ConvertResult {
            if (context == null) {
                throw RuntimeException("Context for convertation must be set!")
            }

            if (convertType == null) {
                throw RuntimeException("Converter type must be set!")
            }

            if (inputParams.from == null) {
                throw RuntimeException("Converted file must be set!")
            }

            return this@X2t.convert(context!!)
        }

    }


    @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
    fun convert(context: Context): ConvertResult {
        val result = ConvertResult()

        FileUtils.getCache(context, convertFileName, convertFolderName)?.let { cache ->
            result.to = inputParams.to ?: cache.to
            result.root = File(result.to ?: "").parent

            inputParams.to = inputParams.to ?: cache.to
            inputParams.temp = inputParams.temp ?: cache.temp
            inputParams.themes = inputParams.themes ?: "${cache.root}/themes"
            inputParams.xml = inputParams.xml ?: "${cache.temp}/param.xml"

            if (File(inputParams.to ?: "").exists()) {
                if (isOverwrite) {
                    FileUtils.deletePath(result.to!!)
                } else {
                    result.code = CONVERTER_CODE_EXIST
                    return@let
                }
            }

            FileUtils.createPath(result.root!!)
            FileUtils.createPath(inputParams.temp!!)

            inputParams.themes?.let { theme ->
                FileUtils.createPath(theme)
            }

            inputParams.key = UUID.randomUUID().toString()
            with(inputParams)
            {
                setIcuDataPath(icu)
                setFonts(arrayOf(fontsJs), fontsDir)
                createXmlFileTransform(xml, key, convertType, from, to, temp, fontsDir, themes, password,
                    delimiterCode.equals(
                        InputParams.DELIMITER_CODE_NONE
                    ).let {
                        if(it)
                            null
                        else
                            delimiterCode.toString()
                    }, delimiterChar,
                    encoding.equals(
                        0
                    ).let {
                        if(it)
                            null
                        else
                            encoding.toString()
                    } )
                result.code = convertFile(xml!!)
            }

            FileUtils.deletePath(cache.temp!!)

            if (isClearTemp) {
                FileUtils.deletePath(cache.root!!)
            }
        }

        return result
    }

    @Throws(IOException::class)
    private fun insertParam(xmlSerializer: XmlSerializer, startTag: String, text: String?, atrr: HashMap<String, String>? = null, endTag: String = startTag)
    {
        xmlSerializer.startTag(null, startTag)
        atrr?.let {
            it.forEach {
                    (key, value) -> xmlSerializer.attribute(null, key, value)
            }
        }
        text?.let {
            xmlSerializer.text(it)
        }
        xmlSerializer.endTag(null, endTag)
    }

    private fun createXmlFileTransform(xmlDirectory: String?,
                                        key: String?,
                                        format: String?,
                                        from: String?,
                                        to: String?,
                                        temp: String?,
                                        fonts: String?,
                                        themes: String?,
                                        password: String?,
                                        delimiter: String?,
                                        delimiterChar: String?,
                                        encoding: String?)
    {
        try {
            val fileOutputStream = FileOutputStream(xmlDirectory)
            val xmlSerializer: XmlSerializer = Xml.newSerializer()
            val writer = StringWriter()

            xmlSerializer.setOutput(writer)
            xmlSerializer.startDocument("UTF-8", null)
            xmlSerializer.startTag(null, "TaskQueueDataConvert")
            xmlSerializer.attribute(null,"xmlns:xsi","""http://www.w3.org/2001/XMLSchema-instance""")
            xmlSerializer.attribute(null,"xmlns:xsd", """http://www.w3.org/2001/XMLSchema""")

            insertParam(xmlSerializer,"m_sKey", key )
            insertParam(xmlSerializer,"m_nFormatTo", format )
            insertParam(xmlSerializer,"m_sFileFrom", from )
            insertParam(xmlSerializer,"m_sFileTo", to )
            insertParam(xmlSerializer,"m_sTempDir", temp )
            insertParam(xmlSerializer,"m_sFontDir", fonts )
            insertParam(xmlSerializer,"m_sThemeDir", themes )

            password?.let{
                insertParam(xmlSerializer,"m_sPassword", password )
                if (isSave && password.isNotEmpty()) {
                    insertParam(xmlSerializer,"m_sSavePassword", password)
                }
            }

            insertParam(xmlSerializer,"m_bIsPDFA", null, hashMapOf("xsi:nil" to """true"""))

            delimiter?.let{
                insertParam(xmlSerializer,"m_nCsvDelimiter", delimiter)
            } ?: run {
                insertParam(xmlSerializer,"m_nCsvDelimiter", null, hashMapOf("xsi:nil" to """false"""))
            }

            delimiterChar?.let{
                insertParam(xmlSerializer,"m_nCsvDelimiterChar", delimiterChar)
            } ?: run {
                insertParam(xmlSerializer,"m_nCsvDelimiterChar", null, hashMapOf("xsi:nil" to """false"""))
            }

            encoding?.let{
                insertParam(xmlSerializer,"m_nCsvTxtEncoding", encoding)
            } ?: run {
                insertParam(xmlSerializer,"m_nCsvTxtEncoding", null, hashMapOf("xsi:nil" to """false"""))
            }

            insertParam(xmlSerializer,"m_bPaid", null, hashMapOf("xsi:nil" to """true"""))
            insertParam(xmlSerializer,"m_bEmbeddedFonts", "false")
            insertParam(xmlSerializer,"m_bFromChanges", null, hashMapOf("xsi:nil" to """false"""))
            insertParam(xmlSerializer,"m_nDoctParams", null, hashMapOf("xsi:nil" to """false"""))
            insertParam(xmlSerializer,"m_bIsNoBase64", "true" )


            xmlSerializer.endTag(null, "TaskQueueDataConvert")
            xmlSerializer.endDocument()
            xmlSerializer.flush()
            val dataWrite: String = writer.toString()
            fileOutputStream.write(dataWrite.toByteArray())
            fileOutputStream.close()
        }
        catch (e: FileNotFoundException) {
            e.printStackTrace()

        } catch (e: IllegalArgumentException) {
            e.printStackTrace()

        } catch (e: IllegalStateException) {
            e.printStackTrace()

        } catch (e: IOException) {
            e.printStackTrace()

        }
    }

}
