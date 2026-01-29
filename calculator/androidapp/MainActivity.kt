package com.calculator.myapp

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
