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
    if (variant.buildType.name == "debug") {
        val assembleTaskName = "assemble${variant.name.capitalizeUS()}"
        val installTaskName = "install${variant.name.capitalizeUS()}User0"

        tasks.register(installTaskName) {
            group = "installation"
            description = "Install APK for user 0 only"
            dependsOn(assembleTaskName)

            doLast {
                val apkFile = variant.outputs.first().outputFile
                println("Installing ${apkFile.name} for user 0 only")
                project.exec {
                    executable = "adb"
                    args = listOf("install", "-r", "--user", "0", apkFile.absolutePath)
                }
            }
        }
    }
}
