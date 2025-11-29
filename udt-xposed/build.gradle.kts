plugins {
    alias(libs.plugins.android.library)
}

android {
    namespace = "jp.miruku.unitydisplaytweaker.module"
    ndkVersion = "29.0.14206865"
    compileSdk {
        version = release(36)
    }

    defaultConfig {
        minSdk = 24

        consumerProguardFiles("consumer-rules.pro")

        ndk {
            abiFilters.add("armeabi-v7a")
            abiFilters.add("arm64-v8a")
        }
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "4.1.2"
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
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
    compileOnly(libs.xposed.api) { artifact { classifier = "sources" } }
    compileOnly(libs.xposed.api)

    implementation(libs.annotation)
}