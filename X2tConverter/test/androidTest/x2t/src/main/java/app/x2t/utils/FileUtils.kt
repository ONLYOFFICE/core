package app.x2t.utils

object FileUtils {

    @JvmStatic
    fun getExtension(path: String): String {
        var extension = ""
        val index = path.lastIndexOf(".") + 1
        if (index >= 0 && index < path.length) {
            extension = path.substring(index)
        }
        return extension
    }

}