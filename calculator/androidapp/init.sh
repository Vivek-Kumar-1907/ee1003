#!/bin/bash

echo "Interactive android project generator on termux"
echo "Script by Jnanesh and Vivek. Inspired by u/AddressSpiritual9574."
echo ""

# Enter the project and package name
read -p "Enter project name (Example: Calculator): " PROJECT_NAME
PROJECT_NAME=${PROJECT_NAME:-Calculator}

read -p "Enter package name (Example: com.calculator.app): " PACKAGE_NAME
PACKAGE_NAME=${PACKAGE_NAME:-com.calculator.app}

echo ""
echo "Setting up directory structure"
echo ""

# Convert package name to directory path. By replacing the dots with slashes.
PACKAGE_PATH=$(echo $PACKAGE_NAME | tr '.' '/')

# Create project structure
mkdir -p $PROJECT_NAME/app/src/main/java/$PACKAGE_PATH
mkdir -p $PROJECT_NAME/app/src/main/res
mkdir -p $PROJECT_NAME/app/src/main/assets
mkdir -p $PROJECT_NAME/app/build
mkdir -p $PROJECT_NAME/app/libs
mkdir -p $PROJECT_NAME/gradle/wrapper

# This is an elementary app with a webview component that loads the contents from a html file in assets.
echo "Creating MainActivity.kt"
cat > $PROJECT_NAME/app/src/main/java/$PACKAGE_PATH/MainActivity.kt <<EOL
package $PACKAGE_NAME

import android.os.Bundle
import android.webkit.WebSettings
import android.webkit.WebView
import android.webkit.WebViewClient
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.runtime.Composable
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.viewinterop.AndroidView

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            // This is the correct way to start a Compose UI
            MyWebViewScreen()
        }
    }
}

@Composable
fun MyWebViewScreen() {
    val context = LocalContext.current
    val htmlString:String = context.assets.open("index.html").bufferedReader().use{
		it.readText()
    }
    AndroidView(factory = {
        WebView(context).apply {
            // Standard WebView setup
            webViewClient = WebViewClient()
            settings.javaScriptEnabled = true
	    //settings.mixedContentMode = WebSettings.MIXED_CONTENT_ALWAYS_ALLOW
	    settings.domStorageEnabled = true
            loadDataWithBaseURL(
	    	"http://localhost/",
		htmlString,
		"text/html",
		"UTF-8",
		null
	    )
	}
    })
}
EOL

# Create AndroidManifest.xml WITHOUT package attribute. The script also requests for Internet permissions and enables clear text traffic which makes communication with the localhost http server possible.
echo "Creating AndroidManifest.xml"
cat > $PROJECT_NAME/app/src/main/AndroidManifest.xml <<EOL
<manifest xmlns:android="http://schemas.android.com/apk/res/android">
    <uses-permission android:name="android.permission.INTERNET" />
    <application android:label="Calculator" android:usesCleartextTraffic="true">
        <activity android:name=".MainActivity"
            android:exported="true">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
    	</activity>
    </application>
</manifest>
EOL

#Create index.html file which is used by the app. It is stored in the assets folder.
echo "Creating index.html"
cat > $PROJECT_NAME/app/src/main/assets/index.html$ << EOL
<!DOCTYPE html>
<html>
<head>
<title>Calculator</title>
</head>
<body>
Calculator boilerplate app. For the full html file please follow the instructions given in the repository https://github.com/Vivek-Kumar-1907/ee1003/calculator.
</body>
</html>
EOL


# Create Gradle build files with Kotlin DSL
echo "Creating Gradle build files"

# App-level build.gradle.kts. Change the sdk versions around if needed. We have so far tested with sdk version 34 which works seamlessly.
cat > $PROJECT_NAME/app/build.gradle.kts <<EOL
plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
}

android {
    namespace = "com.calculator.myapp"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.calculator.myapp"
        minSdk = 21
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }

    kotlinOptions {
        jvmTarget = "17"
    }

    buildFeatures {
        compose = true
    }

    composeOptions {
        kotlinCompilerExtensionVersion = "1.5.3"
    }
}

dependencies {
    val composeBom = platform("androidx.compose:compose-bom:2024.01.00")
    implementation(composeBom)

    // Jetpack Compose
    implementation("androidx.activity:activity-compose:1.8.2")
    implementation("androidx.compose.ui:ui")
    implementation("androidx.compose.material3:material3")
    implementation("androidx.compose.ui:ui-tooling-preview")

    // Kotlin Standard Library
    implementation("org.jetbrains.kotlin:kotlin-stdlib:1.9.10")

    // Include webview component which needs to be used for it to run the webpage natively on the app
    implementation("androidx.webkit:webkit:1.8.0")
}
EOL

# Root settings.gradle.kts
cat > $PROJECT_NAME/settings.gradle.kts <<EOL
rootProject.name = "$PROJECT_NAME"
include(":app")
EOL

# Root build.gradle.kts
cat > $PROJECT_NAME/build.gradle.kts <<EOL
buildscript {
    repositories {
        google()
        mavenCentral()
    }
    dependencies {
        classpath("com.android.tools.build:gradle:8.8.0")
        classpath("org.jetbrains.kotlin:kotlin-gradle-plugin:1.9.10")
    }
}

allprojects {
    repositories {
        google()
        mavenCentral()
    }
}
EOL

# Gradle Wrapper properties. Feel free to change the version around
mkdir -p $PROJECT_NAME/gradle/wrapper
cat > $PROJECT_NAME/gradle/wrapper/gradle-wrapper.properties <<EOL
distributionUrl=https\://services.gradle.org/distributions/gradle-8.12.1-all.zip
EOL

# Gradle Wrapper script
cat > $PROJECT_NAME/gradlew <<EOL
#!/bin/bash
gradle wrapper --gradle-version 8.12.1
EOL
chmod +x $PROJECT_NAME/gradlew

# Gradle properties. Xmx flag is set to 2gigs, which we found most optimal during testing.
cat > $PROJECT_NAME/gradle.properties <<EOL
org.gradle.jvmargs=-Xmx2g
android.useAndroidX=true
EOL

# Set Android SDK Path in local.properties
echo "Setting up SDK path in local.properties..."
echo "sdk.dir=$ANDROID_HOME" > "$PROJECT_NAME/local.properties"

# Find Termux AAPT2 path and override it in gradle.properties
AAPT2_PATH=$(which aapt2)
if [ -n "$AAPT2_PATH" ]; then
    echo "Using Termux AAPT2: $AAPT2_PATH"
    echo "android.aapt2FromMavenOverride=$AAPT2_PATH" >> "$PROJECT_NAME/gradle.properties"
else
    echo "AAPT2 not found in Termux! Please follow the previous instructions given to install aapt2"
fi

# Change into the project directory by default
cd "$PROJECT_NAME"

echo "---------------------------------"
echo "Android project initisialisation ready. Follow more steps to make a calculator app or use this template to make your own app."
echo "Clean with: ./gradlew clean"
echo "Build with: ./gradlew assembleDebug"
echo "Install the apk which is present in app/build/outputs/apk/debug/app-debug.apk."
echo "You are ready to go."
