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
        versionCode = 1
        versionName = "1.0"

        ndk {
            abiFilters.add("armeabi-v7a")
            abiFilters.add("arm64-v8a")
        }
    }

    buildTypes {
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
    // Google
    implementation(libs.material)

    // AndroidX
    implementation(libs.preference)
    implementation(libs.appcompat)
    implementation(libs.activity)
    implementation(libs.constraintlayout)

    implementation(project(":udt-xposed"))
    implementation(project(":material-preference"))
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
