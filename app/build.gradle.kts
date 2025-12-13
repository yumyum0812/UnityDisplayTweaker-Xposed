import android.databinding.tool.ext.capitalizeUS

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
        versionCode = 2
        versionName = "0.1.1"

        ndk {
            abiFilters.add("armeabi-v7a")
            abiFilters.add("arm64-v8a")
        }
    }

    buildTypes {
        debug {
            versionNameSuffix = "-debug"
        }
        release {
            signingConfig = signingConfigs.getByName("debug")
            isMinifyEnabled = true
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
