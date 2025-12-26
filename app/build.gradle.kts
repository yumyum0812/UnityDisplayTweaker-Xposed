import android.databinding.tool.ext.capitalizeUS
import java.util.Properties

val localProperties = Properties()
localProperties.load(rootProject.file("local.properties").inputStream())

plugins {
    alias(libs.plugins.android.application)
}

android {
    namespace = "jp.miruku.unitydisplaytweaker"
    compileSdk {
        version = release(36)
    }

    defaultConfig {
        applicationId = "jp.miruku.unitydisplaytweaker"
        minSdk = 24
        targetSdk = 36
        versionCode = 4
        versionName = "0.2.1"

        ndk {
            abiFilters.add("armeabi-v7a")
            abiFilters.add("arm64-v8a")
        }
    }

    signingConfigs.create("basic") {
        enableV1Signing = true
        enableV2Signing = true
        enableV3Signing = true
        enableV4Signing = false

        val sf = localProperties.getProperty("signing.storeFile")
        val sp = localProperties.getProperty("signing.storePassword")
        val ka = localProperties.getProperty("signing.keyAlias")
        val kp = localProperties.getProperty("signing.keyPassword")
        if (sf == null || sp  == null || ka == null || kp == null) {
            logger.warn("[WARNING] Keystore configs not specified. using debug keystore.")
            val d = signingConfigs.getByName("debug")
            storeFile = d.storeFile
            storePassword = d.storePassword
            keyAlias = d.keyAlias
            keyPassword = d.keyPassword
        } else {
            storeFile = file(sf)
            storePassword = sp
            keyAlias = ka
            keyPassword = kp
        }
    }

    buildTypes {
        all {
            signingConfig = signingConfigs.getByName("basic")
        }
        debug {
            versionNameSuffix = "-debug"
            isDebuggable = true
        }
        release {
            isMinifyEnabled = true
            isShrinkResources = true
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
}

dependencies {
    // AndroidX
    implementation(libs.androidx.preference)
    implementation(libs.androidx.appcompat)
    implementation(libs.androidx.activity)
    implementation(libs.androidx.constraintlayout)

    // Google
    implementation(libs.google.material)

    // Project Modules
    implementation(project(":material-preference"))
    implementation(project(":udt-xposed"))
}

android.applicationVariants.all {
    val variant = this
    val variantNameCapped = variant.name.capitalizeUS()

    tasks.register<Exec>("install${variantNameCapped}ForUser") {
        group = "installation"
        description = "Install APK for specific user"
        dependsOn("assemble${variantNameCapped}")

        doFirst {
            val uidStr = project.findProperty("uid")?.toString() ?: throw IllegalArgumentException("uid not given! add -Puid=<UID> to argument!")
            val apkFile = variant.outputs.first().outputFile
            commandLine(android.adbExecutable.absolutePath, "install", "-r", "--user", uidStr, apkFile.absolutePath)
        }
    }
}
