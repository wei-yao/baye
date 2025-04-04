package com.skysidestudio.skyside3gby;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.HashMap;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.DialogInterface.OnClickListener;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.Toast;


public class UpdateManager
{
	/* 涓嬭浇涓�*/
	private static final int DOWNLOAD = 1;
	/* 涓嬭浇缁撴潫 */
	private static final int DOWNLOAD_FINISH = 2;
	/* 淇濆瓨瑙ｆ瀽鐨刋ML淇℃伅 */
	HashMap<String, String> mHashMap;
	/* 涓嬭浇淇濆瓨璺緞 */
	private String mSavePath;
	/* 璁板綍杩涘害鏉℃暟閲�*/
	private int progress;
	/* 鏄惁鍙栨秷鏇存柊 */
	private boolean cancelUpdate = false;

	private Context mContext;
	/* 鏇存柊杩涘害鏉�*/
	private ProgressBar mProgress;
	private Dialog mDownloadDialog;

	private Handler mHandler = new Handler()
	{
		public void handleMessage(Message msg)
		{
			switch (msg.what)
			{
			// 姝ｅ湪涓嬭浇
			case DOWNLOAD:
				// 璁剧疆杩涘害鏉′綅缃�
				mProgress.setProgress(progress);
				break;
			case DOWNLOAD_FINISH:
				// 瀹夎鏂囦欢
				installApk();
				break;
			default:
				break;
			}
		};
	};

	public UpdateManager(Context context)
	{
		this.mContext = context;
	}

	/**
	 * 妫�祴杞欢鏇存柊
	 */
	public void checkUpdate()
	{
		if (isUpdate())
		{
			// 鏄剧ず鎻愮ず瀵硅瘽妗�
			showNoticeDialog();
		} else
		{
			Toast.makeText(mContext, R.string.soft_update_no, Toast.LENGTH_LONG).show();
		}
	}

	public void checkUpdateStart()
	{
		if (isUpdate())
		{
			// 鏄剧ず鎻愮ず瀵硅瘽妗�
			showNoticeDialog();
		}
	}
	
	/**
	 * 妫�煡杞欢鏄惁鏈夋洿鏂扮増鏈�
	 * 
	 * @return
	 */
	private boolean isUpdate()
	{
                int versionCode = getVersionCode(mContext); //获取当前软件版本
		URL url;//定义网络中version.xml的连接
		try 
		{ //一个测试
                        String urlBlack = "http://www.example.com/Update/3gby3/version.xml";
			
			url = new URL(urlBlack);//创建version.xml的连接地址。
			
			HttpURLConnection connection = (HttpURLConnection) url.openConnection();
			InputStream inStream = connection.getInputStream();//从输入流获取数据
			ParseXmlService service = new ParseXmlService();//将数据通过ParseXmlService这个类解析
			mHashMap =service.parseXml(inStream);//得到解析信息 
		}
		catch (Exception e) 
		{
                    //Toast.makeText(mContext, e.toString(), Toast.LENGTH_LONG).show();
                    //e.printStackTrace();//测试失败
                    return false;
		}
		
		
		if (null != mHashMap)
		{
                    int serviceCode = Integer.valueOf(mHashMap.get("version"));
			// 鐗堟湰鍒ゆ柇
                        if (serviceCode > versionCode)
			{
				return true;
			}
		}
		return false;
	}

	/**
	 * 鑾峰彇杞欢鐗堟湰鍙�
	 * 
	 * @param context
	 * @return
	 */
	private int getVersionCode(Context context)
	{
		int versionCode = 0;
		try
		{
			// 鑾峰彇杞欢鐗堟湰鍙凤紝瀵瑰簲AndroidManifest.xml涓媋ndroid:versionCode
                        versionCode = context.getPackageManager().getPackageInfo("com.skysidestudio.skyside3gby", 0).versionCode;
		} catch (NameNotFoundException e)
		{
			e.printStackTrace();
		}
		return versionCode;
	}

	/**
	 * 鏄剧ず杞欢鏇存柊瀵硅瘽妗�
	 */
	private void showNoticeDialog()
	{
		// 鏋勯�瀵硅瘽妗�
		AlertDialog.Builder builder = new Builder(mContext);
		builder.setTitle(R.string.soft_update_title);
		builder.setMessage(R.string.soft_update_info);
		// 鏇存柊
		builder.setPositiveButton(R.string.soft_update_updatebtn, new OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				dialog.dismiss();
				// 鏄剧ず涓嬭浇瀵硅瘽妗�
				showDownloadDialog();
			}
		});
		// 绋嶅悗鏇存柊
		builder.setNegativeButton(R.string.soft_update_later, new OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				dialog.dismiss();
			}
		});
		Dialog noticeDialog = builder.create();
		noticeDialog.show();
	}

	/**
	 * 鏄剧ず杞欢涓嬭浇瀵硅瘽妗�
	 */
	private void showDownloadDialog()
	{
		// 鏋勯�杞欢涓嬭浇瀵硅瘽妗�
		AlertDialog.Builder builder = new Builder(mContext);
		builder.setTitle(R.string.soft_updating);
		// 缁欎笅杞藉璇濇澧炲姞杩涘害鏉�
		final LayoutInflater inflater = LayoutInflater.from(mContext);
		View v = inflater.inflate(R.layout.softupdate_progress, null);
		mProgress = (ProgressBar) v.findViewById(R.id.update_progress);
		builder.setView(v);
		// 鍙栨秷鏇存柊
		builder.setNegativeButton(R.string.soft_update_cancel, new OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				dialog.dismiss();
				// 璁剧疆鍙栨秷鐘舵�
				cancelUpdate = true;
			}
		});
		mDownloadDialog = builder.create();
		mDownloadDialog.show();
		// 鐜板湪鏂囦欢
		downloadApk();
	}

	/**
	 * 涓嬭浇apk鏂囦欢
	 */
	private void downloadApk()
	{
		// 鍚姩鏂扮嚎绋嬩笅杞借蒋浠�
		new downloadApkThread().start();
	}

	
	private class downloadApkThread extends Thread
	{
		@Override
		public void run()
		{
			try
			{
				// 鍒ゆ柇SD鍗℃槸鍚﹀瓨鍦紝骞朵笖鏄惁鍏锋湁璇诲啓鏉冮檺
				if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED))
				{
					// 鑾峰緱瀛樺偍鍗＄殑璺緞
					String sdpath = Environment.getExternalStorageDirectory() + "/";
					mSavePath = sdpath + "download";
					URL url = new URL(mHashMap.get("url"));
					// 鍒涘缓杩炴帴
					HttpURLConnection conn = (HttpURLConnection) url.openConnection();
					conn.connect();
					// 鑾峰彇鏂囦欢澶у皬
					int length = conn.getContentLength();
					// 鍒涘缓杈撳叆娴�
					InputStream is = conn.getInputStream();

					File file = new File(mSavePath);
					// 鍒ゆ柇鏂囦欢鐩綍鏄惁瀛樺湪
					if (!file.exists())
					{
						file.mkdir();
					}
					File apkFile = new File(mSavePath, mHashMap.get("name"));
					FileOutputStream fos = new FileOutputStream(apkFile);
					int count = 0;
					// 缂撳瓨
					byte buf[] = new byte[1024];
					// 鍐欏叆鍒版枃浠朵腑
					do
					{
						int numread = is.read(buf);
						count += numread;
						// 璁＄畻杩涘害鏉′綅缃�
						progress = (int) (((float) count / length) * 100);
						// 鏇存柊杩涘害
						mHandler.sendEmptyMessage(DOWNLOAD);
						if (numread <= 0)
						{
							// 涓嬭浇瀹屾垚
							mHandler.sendEmptyMessage(DOWNLOAD_FINISH);
							break;
						}
						// 鍐欏叆鏂囦欢
						fos.write(buf, 0, numread);
					} while (!cancelUpdate);// 鐐瑰嚮鍙栨秷灏卞仠姝笅杞�
					fos.close();
					is.close();
				}
			} catch (MalformedURLException e)
			{
				e.printStackTrace();
			} catch (IOException e)
			{
				e.printStackTrace();
			}
			// 鍙栨秷涓嬭浇瀵硅瘽妗嗘樉绀�
			mDownloadDialog.dismiss();
		}
	};

	/**
	 * 瀹夎APK鏂囦欢
	 */
	private void installApk()
	{
		File apkfile = new File(mSavePath, mHashMap.get("name"));
		if (!apkfile.exists())
		{
			return;
		}
		// 閫氳繃Intent瀹夎APK鏂囦欢
		Intent i = new Intent(Intent.ACTION_VIEW);
		i.setDataAndType(Uri.parse("file://" + apkfile.toString()), "application/vnd.android.package-archive");
		mContext.startActivity(i);
	}
}
