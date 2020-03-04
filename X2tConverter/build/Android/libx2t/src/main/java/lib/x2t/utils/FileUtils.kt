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
        return UUID.nameUUIDFromBytes(value.toByteArray()).toString()
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

    @JvmStatic
    @JvmOverloads
    fun getCacheDir(context: Context, isExternal: Boolean = true): File {
        return if (isExternal) context.externalCacheDir!! else context.cacheDir
    }

    @JvmStatic
    @JvmOverloads
    fun getCachePath(context: Context, isExternal: Boolean = true): String {
        return getCacheDir(context, isExternal).absolutePath
    }

    @JvmStatic
    @JvmOverloads
    fun getCache(context: Context, name: String? = null, folder: String? = null, isExternal: Boolean = true): Cache? {
        val newFolder = folder ?: getUuid(System.currentTimeMillis().toString())
        val newName = name ?: getUuid(System.currentTimeMillis().toString())

        val rootDir = "${getCachePath(context, isExternal)}/data/$newFolder"
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