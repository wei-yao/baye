package com.skysidestudio.skyside3gby;

//import android.app.Application;
import org.qtproject.qt5.android.bindings.QtApplication;
public class ContextUtil extends QtApplication {
    private static ContextUtil instance;

    public static ContextUtil getInstance() {
        return instance;
    }

    @Override
    public void onCreate() {
        // TODO Auto-generated method stub
        super.onCreate();
        instance = this;
        CrashHandler crashHandler = CrashHandler.getInstance();  
        crashHandler.init(getApplicationContext());  
    }
}
