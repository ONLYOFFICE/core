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
         * OfficeFileFormats
         * */
        @JvmStatic
        external fun getOfficeFileFormats(): HashMap<String, String>

        /*
                * OfficeFileFormats
                * */
        @JvmStatic
        external fun setFonts(fontsPaths: Array<String>, cacheFontsPath: String)

        @RequiresPermission(allOf = [READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE])
        @JvmStatic
        external fun convertFile(pathXml: String): Int

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
    private var mContext: Context? = null

    /*
    * ConvertType type
    * */
    private var mConvertType: String? = null

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
        var theme: String? = null
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

        fun setConvertType(value: String?): Builder {
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

            if (mConvertType == null) {
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
            mInputParams.xml = mInputParams.xml ?: "${cache.temp}/param.xml"

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

            mInputParams.key = UUID.randomUUID().toString()
            with(mInputParams)
            {
                createXmlFileTransform(xml, key, mConvertType, from, to, temp, fontsJs, theme, password,
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

            if (mIsClearTemp) {
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
            xmlSerializer.attribute(null,"xmlns:xsi",""""http://www.w3.org/2001/XMLSchema-instance"""")
            xmlSerializer.attribute(null,"xmlns:xsd", """"http://www.w3.org/2001/XMLSchema"""")

            insertParam(xmlSerializer,"m_sKey", key )
            insertParam(xmlSerializer,"m_nFormatTo", format )
            insertParam(xmlSerializer,"m_sFileFrom", from )
            insertParam(xmlSerializer,"m_sFileTo", to )
            insertParam(xmlSerializer,"m_sTempDir", temp )
            insertParam(xmlSerializer,"m_sFontDir", fonts )
            insertParam(xmlSerializer,"m_sThemeDir", themes )

            password?.let{
                insertParam(xmlSerializer,"m_sPassword", password )
            }

            insertParam(xmlSerializer,"m_bIsPDFA", null, hashMapOf("xsi:nil" to """"true""""))

            delimiter?.let{
                insertParam(xmlSerializer,"m_nCsvDelimiter", delimiter)
            } ?: run {
                insertParam(xmlSerializer,"m_nCsvDelimiter", null, hashMapOf("xsi:nil" to """"false""""))
            }

            delimiterChar?.let{
                insertParam(xmlSerializer,"m_nCsvDelimiterChar", delimiterChar)
            } ?: run {
                insertParam(xmlSerializer,"m_nCsvDelimiterChar", null, hashMapOf("xsi:nil" to """"false""""))
            }

            encoding?.let{
                insertParam(xmlSerializer,"m_nCsvTxtEncoding", encoding)
            } ?: run {
                insertParam(xmlSerializer,"m_nCsvTxtEncoding", null, hashMapOf("xsi:nil" to """"false""""))
            }

            insertParam(xmlSerializer,"m_bPaid", null, hashMapOf("xsi:nil" to """"true""""))
            insertParam(xmlSerializer,"m_bEmbeddedFonts", "false" )
            insertParam(xmlSerializer,"m_bFromChanges", null, hashMapOf("xsi:nil" to """"false""""))
            insertParam(xmlSerializer,"m_nDoctParams", null, hashMapOf("xsi:nil" to """"false""""))
            insertParam(xmlSerializer,"m_bIsNoBase64", "true" )


            xmlSerializer.endTag(null, "TaskQueueDataConvert")
            xmlSerializer.endDocument()
            xmlSerializer.flush()
            val dataWrite: String = writer.toString()
            fileOutputStream.write(dataWrite.toByteArray())
            fileOutputStream.close()
        }
        catch (e: FileNotFoundException) {
            e.printStackTrace();

        } catch (e: IllegalArgumentException) {
            e.printStackTrace();

        } catch (e: IllegalStateException) {
            e.printStackTrace();

        } catch (e: IOException) {
            e.printStackTrace();

        }
    }

}
