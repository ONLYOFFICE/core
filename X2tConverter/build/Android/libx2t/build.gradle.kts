@file:Suppress("UnstableApiUsage")

import com.android.build.gradle.internal.tasks.factory.dependsOn
import org.apache.tools.ant.taskdefs.condition.Os

plugins {
    id("com.android.library")
    kotlin("android")
}

apply {
    from("../extras/gradle/common.gradle")
}

android {

    namespace = "lib.x2t"
    compileSdk = libs.versions.compileSdk.get().toInt()
    ndkVersion = rootProject.extra.get("NDK_VERSION").toString()

    publishing {
        singleVariant("release") {
        }
    }

    defaultConfig {
        minSdk = libs.versions.minSdk.get().toInt()

        buildConfigField("String", "LIB_X2T", "\"${extra.get("NAME_LIB")}\"")

        externalNativeBuild {
            cmake {
                arguments(
                    "-DANDROID_TOOLCHAIN=clang",
                    "-DANDROID_STL=c++_shared",
                    "-DANDROID_ARM_NEON=TRUE",
                    "-DARG_PATH_LIB_BUILD_TOOLS=${getProjectPath(extra.get("PATH_LIB_BUILD_TOOLS") as String)}",
                    "-DARG_PATH_SRC_CORE=${getProjectPath(extra.get("PATH_SRC_CORE") as String)}",
                    "-DARG_NAME_LIB=${extra.get("NAME_LIB")}"
                )
            }
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
            ndk {
                abiFilters.addAll(arrayOf("armeabi-v7a", "arm64-v8a", "x86", "x86_64"))
            }
        }
        debug {
            isJniDebuggable = true
            ndk {
                if (System.getProperty("os.arch") == "aarch64") {
                    abiFilters.add("arm64-v8a")
                } else {
                    abiFilters.addAll(arrayOf("armeabi-v7a", "arm64-v8a", "x86", "x86_64"))
                }
            }
        }
    }

    externalNativeBuild {
        cmake {
            path("src/main/cpp/CMakeLists.txt")
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }

    kotlinOptions {
        jvmTarget = "17"
    }

    buildFeatures {
        buildConfig = true
    }

    packaging {
        jniLibs {
            useLegacyPackaging = true
            pickFirsts.add("lib/armeabi-v7a/libc++_shared.so")
            pickFirsts.add("lib/x86/libc++_shared.so")
            pickFirsts.add("lib/arm64-v8a/libc++_shared.so")
            pickFirsts.add("lib/x86_64/libc++_shared.so")
        }
    }
}

dependencies {
    implementation(fileTree(mapOf("dir" to "libs", "include" to listOf("*.jar"))))
}


/*
* Check the path for existence,
* otherwise get the path relative to the current project,
* otherwise the error is in the path
* */
fun getProjectPath(path: String, isRelativeCreate: Boolean = true): String {
    val absolutePath = file(path)
    val relativePath = file("${file(".").absolutePath}/$path")

    val replaced = if (Os.isFamily(Os.FAMILY_WINDOWS)) {
       "\\"
    } else {
        "\\\\"
    }

    if (absolutePath.exists() && absolutePath.isDirectory) {
        return absolutePath.toString().replace(replaced, "/")
    } else if ((relativePath.exists() && relativePath.isDirectory) || (isRelativeCreate && relativePath.mkdirs())) {
        return relativePath.toString().replace(replaced, "/")
    }

    throw GradleException("getProjectPath($path) - path doesn't exist...")
}

tasks.create("copyIcuDatFiles") {
    doLast {

        println()
        println("------------------------- ICUDATFILES (task copyIcuDatFiles()) -------------------------")

        val pathAssets = "${file(".").absolutePath}/src/main/assets/icu"
        if (file(pathAssets).exists()) {
            println("IcuDatFiles already exist.")
            println("Delete $pathAssets folder for update icuDatFiles.")
        }

        if (!project.hasProperty("PATH_SRC_ICU_DAT_FILE")) {
            throw GradleException("Property PATH_SRC_ICU_DAT_FILE with core repository path doesn't exist...")
        }

        if (!project.hasProperty("PATH_SRC_ICU_V8_DAT_FILE")) {
            throw GradleException("Property PATH_SRC_ICU_V8_DAT_FILE with core repository path doesn't exist...")
        }

        if (!project.hasProperty("PATH_SRC_ICU_V8_EXTRA_DAT_FILE")) {
            throw GradleException("Property PATH_SRC_ICU_V8_EXTRA_DAT_FILE with core repository path doesn't exist...")
        }

        val pathIcuDatFiles = project.extra.get("PATH_SRC_ICU_DAT_FILE") as String
        val pathIcuV8DatFiles = project.extra.get("PATH_SRC_ICU_V8_DAT_FILE") as String
        val pathIcuV8DatExtraFiles = project.extra.get("PATH_SRC_ICU_V8_EXTRA_DAT_FILE") as String

        if (!file(pathIcuDatFiles).exists()) {
            throw GradleException("Path with build_tools repository doesn't exist...")
        }

        if (!file(pathIcuV8DatFiles).exists()) {
            throw GradleException("Path with core repository doesn't exist...")
        }

        if (!file(pathIcuV8DatExtraFiles).exists()) {
            throw GradleException("Path with core repository doesn't exist...")
        }
        
        copy {
            println("\nCopy dat files...")
            from(pathIcuDatFiles, pathIcuV8DatFiles, pathIcuV8DatExtraFiles)
            into(pathAssets)
        }
    }
}

tasks.preBuild.dependsOn("copyIcuDatFiles")