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

val keystore = extra.get("getKeystore") as org.codehaus.groovy.runtime.MethodClosure

//publishing {
//    publications {
//        create<MavenPublication>("maven") {
//            groupId = PublishEditors.groupId
//            artifactId = PublishEditors.x2tId
//            version = PublishEditors.version
//            artifact("${layout.buildDirectory}/outputs/aar/lib${artifactId}-release.aar")
//        }
//    }
//    repositories {
//        maven {
//            name = "GitHubPackages"
//            url = uri("${PublishEditors.publishUrl}/")
//            credentials {
//                username = (keystore() as? java.util.Properties)?.getProperty("git_user_name") ?: ""
//                password = (keystore() as? java.util.Properties)?.getProperty("git_token") ?: ""
//            }
//        }
//    }
//}

android {

    namespace = "lib.x2t"
    compileSdk = AppDependency.COMPILE_SDK_VERSION
    ndkVersion = rootProject.extra.get("NDK_VERSION").toString()

    publishing {
        singleVariant("release") {
        }
    }

    defaultConfig {
        minSdk = AppDependency.MIN_SDK_VERSION

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
        sourceCompatibility(JavaVersion.VERSION_17)
        targetCompatibility(JavaVersion.VERSION_17)
    }

    kotlinOptions {
        jvmTarget = "17"
    }

    buildFeatures {
        buildConfig = true
    }

    packaging {
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
    implementation("androidx.appcompat:appcompat:1.6.1")
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