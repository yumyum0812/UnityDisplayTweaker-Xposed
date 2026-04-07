import com.android.build.api.artifact.SingleArtifact
import com.android.build.api.dsl.ApplicationExtension
import java.util.Properties

val localProperties = Properties()
localProperties.load(rootProject.file("local.properties").inputStream())

plugins {
    alias(libs.plugins.android.application)
}

extensions.configure<ApplicationExtension> {
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

androidComponents.onVariants { variant ->
    val name = variant.name
    val capped = name.replaceFirstChar { it.uppercase() }
    val adbExeProv = androidComponents.sdkComponents.adb
    val apkDirProv = variant.artifacts.get(SingleArtifact.APK)

    tasks.register<Exec>("install${capped}ForUser") {
        group = "installation"
        description = "Install APK for specific user"
        dependsOn("assemble${capped}")

        doFirst {
            val uidStr = project.findProperty("uid")?.toString()
                ?: throw IllegalArgumentException("uid not given! add -Puid=<UID> to argument!")
            val adbExe = adbExeProv.get().asFile
            val apkDir = apkDirProv.get().asFile
            val apkFiles = apkDir.walkTopDown().filter { it.extension == "apk" }

            val args = mutableListOf("install-multiple", "-r", "--user", uidStr)
            args += apkFiles.map { it.absolutePath }

            logger.debug("adb executable: ${adbExe.absolutePath}")
            logger.debug("arguments: ${args.joinToString(" ")}")
            commandLine(adbExe.absolutePath, *args.toTypedArray())
        }
    }
}