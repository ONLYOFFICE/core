import com.android.build.gradle.internal.tasks.factory.dependsOn

plugins {
    id("com.android.library")
    kotlin("android")
}

android {

    buildToolsVersion = AppDependency.BUILD_TOOLS_VERSION
    compileSdk = AppDependency.COMPILE_SDK_VERSION
    ndkVersion = rootProject.extra.get("NDK_VERSION").toString()

    defaultConfig {
        minSdk = AppDependency.MIN_SDK_VERSION
        targetSdk = AppDependency.TARGET_SDK_VERSION


        buildConfigField("String", "LIB_X2T", "\"${extra.get("NAME_LIB")}\"")

        externalNativeBuild {
            cmake {
                arguments(
                    "-DANDROID_TOOLCHAIN=clang",
                    "-DANDROID_STL=c++_static",
                    "-DANDROID_ARM_NEON=TRUE",
                    "-DARG_PATH_LIB_BUILD_TOOLS=${getProjectPath(extra.get("PATH_LIB_BUILD_TOOLS") as String)}",
                    "-DARG_PATH_LIB_DST=${getProjectPath(extra.get("PATH_LIB_DST") as String, true)}",
                    "-DARG_PATH_SRC_CORE=${getProjectPath(extra.get("PATH_SRC_CORE") as String)}",
                    "-DARG_NAME_LIB=${extra.get("NAME_LIB")}"
                )
            }
        }

        ndk {
            abiFilters.addAll(arrayOf("armeabi-v7a", "arm64-v8a", "x86", "x86_64"))
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
        debug {
            isJniDebuggable = true
        }
    }

    sourceSets {
        getByName("main") {
            java.srcDir("src/main/java")
            jni.srcDir("src/main/cpp")
            jniLibs.srcDirs(
                arrayOf(
                    extra.get("PATH_LIB_DST") as String,
                    extra.get("PATH_LIB_BUILD_TOOLS") as String
                )
            )
        }
    }

    externalNativeBuild {
        cmake {
            path("src/main/cpp/CMakeLists.txt")
        }
    }

    compileOptions {
        sourceCompatibility(JavaVersion.VERSION_11)
        targetCompatibility(JavaVersion.VERSION_11)
    }

    kotlinOptions {
        jvmTarget = "11"
    }

    packagingOptions {
        jniLibs.useLegacyPackaging = true
        arrayOf("armeabi-v7a", "x86", "arm64-v8a", "x86_64").forEach { abi ->
            val dh = file("${extra.get("PATH_LIB_BUILD_TOOLS")}/$abi")
            dh.listFiles()?.forEach {
                if (it.name.contains(".so"))
                    jniLibs.pickFirsts.add("lib/$abi/${it.name}")
            }
            jniLibs.pickFirsts.add("lib/$abi/lib${extra.get("NAME_LIB")}.so")
            jniLibs.pickFirsts.add("lib/$abi/lib${extra.get("NAME_LIB_KERNEL_NETWORK")}.so")
        }
    }
}

dependencies {
    implementation(fileTree(mapOf("dir" to "libs", "include" to listOf("*.jar"))))
    implementation("androidx.appcompat:appcompat:1.4.2")
    implementation("org.jetbrains.kotlin:kotlin-stdlib-jdk8:${rootProject.extra.get("kotlin_version")}")
}


/*
* Check the path for existence,
* otherwise get the path relative to the current project,
* otherwise the error is in the path
* */
fun getProjectPath(path: String, isRelativeCreate: Boolean = true): String {
    val absolutePath = file(path)
    val relativePath = file("${file(".").absolutePath}/$path")
    //def relativePath = file("${rootProject.projectDir}/path")

    if (absolutePath.exists() && absolutePath.isDirectory) {
        return absolutePath.toString().replace("\\\\", "/")
    } else if ((relativePath.exists() && relativePath.isDirectory) || (isRelativeCreate && relativePath.mkdirs())) {
        return relativePath.toString().replace("\\\\", "/")
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

        val pathIcuDatFiles = project.extra.get("PATH_SRC_ICU_DAT_FILE") as String
        val pathIcuV8DatFiles = project.extra.get("PATH_SRC_ICU_V8_DAT_FILE") as String

        if (!file(pathIcuDatFiles).exists()) {
            throw GradleException("Path with build_tools repository doesn't exist...")
        }

        if (!file(pathIcuV8DatFiles).exists()) {
            throw GradleException("Path with core repository doesn't exist...")
        }

        copy {
            println("\nCopy dat files...")
            from(pathIcuDatFiles, pathIcuV8DatFiles)
            into(pathAssets)
        }
    }
}

tasks.preBuild.dependsOn("copyIcuDatFiles")