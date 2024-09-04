package com.skysidestudio.skyside3gby;

import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.io.Writer;
import java.lang.Thread.UncaughtExceptionHandler;
import java.lang.reflect.Field;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import android.annotation.SuppressLint;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Build;
import android.os.Environment;
import android.os.Looper;
import android.util.Log;
import android.view.WindowManager;
import android.widget.Toast;

/**
 * UncaughtException处理类,当程序发生Uncaught异常的时候,有该类来接管程序,并记录发送错误报告.
 * 
 * @author user
 * 
 */
public class CrashHandler implements UncaughtExceptionHandler {
	
        public static String SDPATH = Environment.getExternalStorageDirectory().getPath() + "/skyside3gby3/";
	public static final String TAG = "CrashHandler";
	
	//系统默认的UncaughtException处理类 
	private Thread.UncaughtExceptionHandler mDefaultHandler;
	//CrashHandler实例
	private static CrashHandler INSTANCE = new CrashHandler();
	//程序的Context对象
	private Context mContext;
	//用来存储设备信息和异常信息
	private Map<String, String> infos = new HashMap<String, String>();

	//用于格式化日期,作为日志文件名的一部分
	private DateFormat formatter = new SimpleDateFormat("yyyy-MM-dd-HH-mm-ss");

	/** 保证只有一个CrashHandler实例 */
	private CrashHandler() {
	}

	/** 获取CrashHandler实例 ,单例模式 */
	public static CrashHandler getInstance() {
		return INSTANCE;
	}

	/**
	 * 初始化
	 * 
	 * @param context
	 */
	public void init(Context context) {
		mContext = context;
		//获取系统默认的UncaughtException处理器
		mDefaultHandler = Thread.getDefaultUncaughtExceptionHandler();
		//设置该CrashHandler为程序的默认处理器
		Thread.setDefaultUncaughtExceptionHandler(this);
	}

	/**
	 * 当UncaughtException发生时会转入该函数来处理
	 */
	@Override
	public void uncaughtException(Thread thread, Throwable ex) {
		if (!handleException(ex) && mDefaultHandler != null) {
			//如果用户没有处理则让系统默认的异常处理器来处理
			mDefaultHandler.uncaughtException(thread, ex);
		} else {
			try {
				Thread.sleep(30000);
			} catch (InterruptedException e) {
				Log.e(TAG, "error : ", e);
			}
			//退出程序
			int currentVersion = android.os.Build.VERSION.SDK_INT;  
            if (currentVersion > android.os.Build.VERSION_CODES.ECLAIR_MR1) {  
                Intent startMain = new Intent(Intent.ACTION_MAIN);  
                startMain.addCategory(Intent.CATEGORY_HOME);  
                startMain.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);  
                mContext.startActivity(startMain);  
                ActivityManager manager = (ActivityManager) mContext.getSystemService(Context.ACTIVITY_SERVICE);      
                                manager.killBackgroundProcesses("com.skysidestudio.skyside3gby");
				System.exit(0);  
            } else {// android2.1  
            	ActivityManager am = (ActivityManager) mContext.getSystemService(Context.ACTIVITY_SERVICE);  
                am.restartPackage("com.skysidestudio.skyside3gby");
            }  
		}
	}

	/**
	 * 自定义错误处理,收集错误信息 发送错误报告等操作均在此完成.
	 * 
	 * @param ex
	 * @return true:如果处理了该异常信息;否则返回false.
	 */
	private boolean handleException(final Throwable ex) {
		if (ex == null) {
			return false;
		}

		//使用Toast来显示异常信息
		new Thread() {
			@Override
			public void run() {
				Looper.prepare();
				
				//存崩溃存档
				//Public.getGameView().game.newgame.who.gamemap.ChangeDataToSave();	
				//Public.getGameView().game.newgame.who.gamemap.save.Write(9);
				
				Toast.makeText(mContext, "很抱歉,程序出现异常,即将退出.", Toast.LENGTH_LONG).show();
				collectDeviceInfo(mContext);
				//保存日志文件 
				final String crashReport = saveCrashInfo2File(ex);
				//开启发送错误报告界面
				sendAppCrashReport(mContext, crashReport);
				Looper.loop();
			}
		}.start();
		//收集设备参数信息 
		
		//android.os.Process.killProcess(android.os.Process.myPid());
		//System.exit(1);
		return true;
	}
	
	private void sendAppCrashReport(final Context context,
			final String crashReport ) {
		AlertDialog.Builder builder = new AlertDialog.Builder(context)
		.setTitle("错误报告")
		.setMessage("天际边三国霸业 遇到问题需要关闭，我们已经产生了一个相关的错误报告，希望您能打开网络连接并发送此错误报告给我们以便更好的完善程序")
		.setPositiveButton("发送",
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) 
					{
						SimpleDateFormat format1 = new SimpleDateFormat("yyyyMMddHHmmss");  
						SimpleDateFormat format2 = new SimpleDateFormat("yyyyMMdd");  
					    String t=format1.format(new java.util.Date());  
					    String day=format2.format(new java.util.Date());  
					  //ftp上传
					    try 
						{
							Public.ftpUpload("ftp.example.com", "21", "feedback","123456","/Upload3gN/" + t, SDPATH +"Crash/",crashReport) ;

						} catch (Exception e) {
							Log.i("ftpUpload","error:" + e.getMessage());
						} 
					    
						try 
						{
							Public.ftpUpload("ftp.example.com", "21", "feedback","123456","/Upload3gN/" + t, SDPATH +"Log/",day + ".log") ;

						} catch (Exception e) {
							Log.i("ftpUpload","error:" + e.getMessage());
						} 
						
						try 
						{
							Public.ftpUpload("ftp.example.com", "21", "feedback","123456","/Upload3gN/" + t, SDPATH ,"save0.ssv") ;

						} catch (Exception e) {
							Log.i("ftpUpload","error:" + e.getMessage());
						} 
						
						try 
						{
							Public.ftpUpload("ftp.example.com", "21", "feedback","123456","/Upload3gN/" + t, SDPATH ,"save1.ssv") ;

						} catch (Exception e) {
							Log.i("ftpUpload","error:" + e.getMessage());
						} 
						
						try 
						{
							Public.ftpUpload("ftp.example.com", "21", "feedback","123456","/Upload3gN/" + t, SDPATH ,"save2.ssv") ;

						} catch (Exception e) {
							Log.i("ftpUpload","error:" + e.getMessage());
						} 
						try 
						{
							Public.ftpUpload("ftp.example.com", "21", "feedback","123456","/Upload3gN/" + t, SDPATH ,"save9.ssv") ;

						} catch (Exception e) {
							Log.i("ftpUpload","error:" + e.getMessage());
						} 
						finally 
						{
							dialog.dismiss();
							// 退出
							//android.os.Process.killProcess(android.os.Process.myPid());
							//System.exit(1);
							
							int currentVersion = android.os.Build.VERSION.SDK_INT;  
				            if (currentVersion > android.os.Build.VERSION_CODES.ECLAIR_MR1) {  
				                Intent startMain = new Intent(Intent.ACTION_MAIN);  
				                startMain.addCategory(Intent.CATEGORY_HOME);  
				                startMain.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);  
				                context.startActivity(startMain);  
				                ActivityManager manager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);      
                                                                manager.killBackgroundProcesses("com.skysidestudio.skyside3gby");
								System.exit(0);  
				            } else {// android2.1  
				            	ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);  
                                                am.restartPackage("com.skysidestudio.skyside3gby");
				            }  
				            
							//ActivityManager manager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);      
                                                        //manager.killBackgroundProcesses("com.skysidestudio.skyside3gby");

						}
					}
				})
		.setNegativeButton(android.R.string.cancel,
				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						dialog.dismiss();
						// 退出
						//android.os.Process.killProcess(android.os.Process.myPid());
						//System.exit(1);
						int currentVersion = android.os.Build.VERSION.SDK_INT;  
			            if (currentVersion > android.os.Build.VERSION_CODES.ECLAIR_MR1) {  
			                Intent startMain = new Intent(Intent.ACTION_MAIN);  
			                startMain.addCategory(Intent.CATEGORY_HOME);  
			                startMain.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);  
			                context.startActivity(startMain);  
			                ActivityManager manager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);      
                                                        manager.killBackgroundProcesses("com.skysidestudio.skyside3gby");
							System.exit(0);  
			            } else {// android2.1  
			            	ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);  
                                        am.restartPackage("com.skysidestudio.skyside3gby");
			            }  
					}
				});
		
		AlertDialog dialog = builder.create();
		//需要的窗口句柄方式，没有这句会报错的
        dialog.getWindow().setType(WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
		dialog.show();
	}
	
	/**
	 * 收集设备参数信息
	 * @param ctx
	 */
	public void collectDeviceInfo(Context ctx) {
		try {
			PackageManager pm = ctx.getPackageManager();
			PackageInfo pi = pm.getPackageInfo(ctx.getPackageName(), PackageManager.GET_ACTIVITIES);
			if (pi != null) {
				String versionName = pi.versionName == null ? "null" : pi.versionName;
				String versionCode = pi.versionCode + "";
				infos.put("versionName", versionName);
				infos.put("versionCode", versionCode);
			}
		} catch (NameNotFoundException e) {
			Log.e(TAG, "an error occured when collect package info", e);
		}
		Field[] fields = Build.class.getDeclaredFields();
		for (Field field : fields) {
			try {
				field.setAccessible(true);
				infos.put(field.getName(), field.get(null).toString());
				Log.d(TAG, field.getName() + " : " + field.get(null));
			} catch (Exception e) {
				Log.e(TAG, "an error occured when collect crash info", e);
			}
		}
	}

	/**
	 * 保存错误信息到文件中
	 * 
	 * @param ex
	 * @return	返回文件名称,便于将文件传送到服务器
	 */
	private String saveCrashInfo2File(Throwable ex) {
		
		StringBuffer sb = new StringBuffer();
		for (Map.Entry<String, String> entry : infos.entrySet()) {
			String key = entry.getKey();
			String value = entry.getValue();
			sb.append(key + "=" + value + "\n");
		}
		
		Writer writer = new StringWriter();
		PrintWriter printWriter = new PrintWriter(writer);
		ex.printStackTrace(printWriter);
		Throwable cause = ex.getCause();
		while (cause != null) {
			cause.printStackTrace(printWriter);
			cause = cause.getCause();
		}
		printWriter.close();
		String result = writer.toString();
		sb.append(result);
		try {
			long timestamp = System.currentTimeMillis();
			String time = formatter.format(new Date());
			String fileName = "crash-" + time + "-" + timestamp + ".log";
			if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
				String path = SDPATH + "Crash/";
				File dir = new File(path);
				if (!dir.exists()) {
					dir.mkdirs();
				}
				FileOutputStream fos = new FileOutputStream(path + fileName);
				fos.write(sb.toString().getBytes());
				fos.close();
			}
			return fileName;
		} catch (Exception e) {
			Log.e(TAG, "an error occured while writing file...", e);
		}
		return null;
	}
}
