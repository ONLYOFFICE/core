package lib.x2t.utils

import android.Manifest.permission.WRITE_EXTERNAL_STORAGE
import android.content.Context
import androidx.annotation.RequiresPermission
import java.io.File
import java.util.*


object FileUtils {

    class Cache {
        @JvmField
        var to: String? = null
        @JvmField
        var temp: String? = null
        @JvmField
        var root: String? = null
    }

    @JvmStatic
    fun getUuid(value: String = ""): String {
        return UUID.nameUUIDFromBytes(("$value${System.currentTimeMillis()}").toByteArray()).toString()
    }

    @JvmStatic
    fun createPath(path: String): Boolean {
        return File(path).let { it.isDirectory && it.exists() || it.mkdirs() }
    }

    @RequiresPermission(WRITE_EXTERNAL_STORAGE)
    @JvmStatic
    fun deletePath(path: File): Int {
        var count = 0

        if (path.isDirectory) {
            path.listFiles()?.forEach {
                count += deletePath(it)
            }
        }

        count += if (path.delete()) 1 else 0
        return count
    }

    @RequiresPermission(WRITE_EXTERNAL_STORAGE)
    @JvmStatic
    fun deletePath(path: String): Int {
        return deletePath(File(path))
    }

    @RequiresPermission(WRITE_EXTERNAL_STORAGE)
    @JvmStatic
    fun getCache(context: Context, name: String? = null): Cache? {
        val fileName = name ?: System.currentTimeMillis().toString()
        val uuidName = getUuid(fileName)
        val newName = if (name != null) fileName else uuidName

        val rootDir = "${context.externalCacheDir!!.absolutePath}/data/$uuidName"
        val toFile = "$rootDir/$newName"
        val tempDir = "$rootDir/temp"

        if (createPath(tempDir)) {
            return Cache().apply {
                this.to = toFile
                this.temp = tempDir
                this.root = rootDir
            }
        }

        return null
    }
}