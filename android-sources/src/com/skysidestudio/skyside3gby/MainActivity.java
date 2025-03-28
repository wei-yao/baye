package com.skysidestudio.skyside3gby;




import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;


import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.StrictMode;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.Toast;
import android.os.Environment;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

import org.qtproject.qt5.android.bindings.QtActivity;
import android.support.v4.content.ContextCompat;
import android.support.v4.app.ActivityCompat;
import android.content.pm.PackageManager;
import android.Manifest;

public class MainActivity extends QtActivity {

    private static MainActivity m_instance;

    public MainActivity()
      {
        //构造函数必须
        m_instance = this;
      }

    private static Handler m_handler = new Handler()
    {
          @Override
          public void handleMessage(Message msg)
          {
              switch (msg.what)
              {
              case 0:
              {
                  new AlertDialog.Builder(m_instance)
                  .setTitle("提示")
          .setMessage("是否退出？")
          .setPositiveButton("确定", new DialogInterface.OnClickListener()
          {

              @Override
              public void onClick(DialogInterface dialog, int which)
              {
               // TODO Auto-generated method stub
                  m_instance.finish();
                  System.exit(0);
              }
          })
          .setNegativeButton("取消", null)
          .show();
                  break;
              }
              case 1:
                  Toast toast = Toast.makeText(m_instance,(String)msg.obj, Toast.LENGTH_SHORT);
                  toast.show();
                  //m_instance.openOptionsMenu();
                  break;
              case 2:
                  UpdateManager manager = new UpdateManager(m_instance);
                  // 检查软件更新
                  manager.checkUpdateStart();
                      break;
              case 3:
              {
                  String SDPATH = Environment.getExternalStorageDirectory().getPath() + "/skyside3gby3/";

                  SimpleDateFormat format1 = new SimpleDateFormat("yyyyMMddHHmmss");
                  SimpleDateFormat format2 = new SimpleDateFormat("yyyyMMdd");
                  String t=format1.format(new java.util.Date());
                  String day=format2.format(new java.util.Date());
                  //ftp上传

                  try
                  {
                          Public.ftpUpload("ftp.example.com", "21", "feedback","123456","/Upload3gN/" + t, SDPATH ,"sango0.sav") ;

                  } catch (Exception e) {
                          Log.i("ftpUpload","error:" + e.getMessage());
                  }

                  try
                  {
                          Public.ftpUpload("ftp.example.com", "21", "feedback","123456","/Upload3gN/" + t, SDPATH ,"sango1.sav") ;

                  } catch (Exception e) {
                          Log.i("ftpUpload","error:" + e.getMessage());
                  }

                  try
                  {
                          Public.ftpUpload("ftp.example.com", "21", "feedback","123456","/Upload3gN/" + t, SDPATH ,"sango2.sav") ;

                  } catch (Exception e) {
                          Log.i("ftpUpload","error:" + e.getMessage());
                  }
                  try
                  {
                          Public.ftpUpload("ftp.example.com", "21", "feedback","123456","/Upload3gN/" + t, SDPATH ,"sango3.sav") ;

                  } catch (Exception e) {
                          Log.i("ftpUpload","error:" + e.getMessage());
                  }

                  try
                  {
                          Public.ftpUpload("ftp.example.com", "21", "feedback","123456","/Upload3gN/" + t, SDPATH ,"sango4.sav") ;

                  } catch (Exception e) {
                          Log.i("ftpUpload","error:" + e.getMessage());
                  }

                  try
                  {
                          Public.ftpUpload("ftp.example.com", "21", "feedback","123456","/Upload3gN/" + t, SDPATH ,"sango5.sav") ;

                  } catch (Exception e) {
                          Log.i("ftpUpload","error:" + e.getMessage());
                  }


                  Toast toast1 = Toast.makeText(m_instance,(String)msg.obj, Toast.LENGTH_SHORT);
                  toast1.show();
                  //m_instance.openOptionsMenu();
                  break;
              }
              case 4:
              {
                            Toast toast1 = Toast.makeText(m_instance,"隐藏状态栏成功", Toast.LENGTH_SHORT);
                            toast1.show();
                            WindowManager.LayoutParams lp = m_instance.getWindow().getAttributes();
                            lp.flags |= WindowManager.LayoutParams.FLAG_FULLSCREEN;
                            m_instance.getWindow().setAttributes(lp);
                            break;
              }
              case 5:
              {
                            Toast toast1 = Toast.makeText(m_instance,"显示状态栏成功", Toast.LENGTH_SHORT);
                            toast1.show();
                            WindowManager.LayoutParams lp = m_instance.getWindow().getAttributes();
                            lp.flags &= (~WindowManager.LayoutParams.FLAG_FULLSCREEN);
                            m_instance.getWindow().setAttributes(lp);
                            break;
              }
              };
          }
      };



	@Override
	@TargetApi(16)
        public void onCreate(Bundle savedInstanceState) {
		
                ConfigurationFile.InitPrivateProfile(Public.absolutepath);
		ConfigurationFile.InitPrivateProfile(Public.versionpath);
		
                /*Public.showWay=ConfigurationFile.GetPrivateProfileInt("Startup", "ShowWay", 0, Public.absolutepath );
		Public.color=ConfigurationFile.GetPrivateProfileInt("Startup", "Color", Public.color, Public.absolutepath );
		
		Public.menushow1=ConfigurationFile.GetPrivateProfileInt("Startup", "Menushow", 0, Public.absolutepath );
		Public.menumove1=ConfigurationFile.GetPrivateProfileInt("Startup", "Menumove", 0, Public.absolutepath );
		Public.menuspeed1=ConfigurationFile.GetPrivateProfileInt("Startup", "Menuspeed", 0, Public.absolutepath );
		Public.KeyEnable = ConfigurationFile.GetPrivateProfileInt("Startup", "UseKey", 1, Public.absolutepath );
		Public.TouchEnable = ConfigurationFile.GetPrivateProfileInt("Startup", "UseTouch", 1, Public.absolutepath );
		*/

                getWindow().setFlags(WindowManager.LayoutParams. FLAG_FULLSCREEN ,WindowManager.LayoutParams. FLAG_FULLSCREEN);


		
                int permission_write=ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.WRITE_EXTERNAL_STORAGE);
                int permission_read=ContextCompat.checkSelfPermission(MainActivity.this,Manifest.permission.READ_EXTERNAL_STORAGE);
                int permission_mont=ContextCompat.checkSelfPermission(MainActivity.this,Manifest.permission.MOUNT_UNMOUNT_FILESYSTEMS);
                if(permission_write!= PackageManager.PERMISSION_GRANTED|| permission_mont!=PackageManager.PERMISSION_GRANTED|| permission_read!=PackageManager.PERMISSION_GRANTED)
                {
                     Toast.makeText(m_instance, "正在请求权限", Toast.LENGTH_SHORT).show();
                     //申请权限，特征码自定义为1，可在回调时进行相关判断
                     ActivityCompat.requestPermissions(MainActivity.this,new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE,Manifest.permission.READ_EXTERNAL_STORAGE,Manifest.permission.MOUNT_UNMOUNT_FILESYSTEMS},1);
                }

                /*int permission3=ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.INTERNET);
                int permission4=ContextCompat.checkSelfPermission(MainActivity.this,Manifest.permission.SYSTEM_ALERT_WINDOW);
                int permission5=ContextCompat.checkSelfPermission(MainActivity.this,Manifest.permission.KILL_BACKGROUND_PROCESSES);
                if(permission3!= PackageManager.PERMISSION_GRANTED|| permission4!=PackageManager.PERMISSION_GRANTED|| permission5!=PackageManager.PERMISSION_GRANTED)
                {
                     Toast.makeText(m_instance, "正在请求权限2", Toast.LENGTH_SHORT).show();
                     //申请权限，特征码自定义为1，可在回调时进行相关判断
                     ActivityCompat.requestPermissions(MainActivity.this,new String[]{Manifest.permission.INTERNET,Manifest.permission.SYSTEM_ALERT_WINDOW,Manifest.permission.KILL_BACKGROUND_PROCESSES},1);

                }*/


		super.onCreate(savedInstanceState);

                if (android.os.Build.VERSION.SDK_INT > 9) {
                    StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
                    StrictMode.setThreadPolicy(policy);
                }
		
		
		UpdateManager manager = new UpdateManager(MainActivity.this);
		// 检查软件更新
		manager.checkUpdateStart();
		
		
		Public.checkNewVersion(MainActivity.this);
		
		

		
		

		
		 
	}
	
    @Override
        public void onRequestPermissionsResult(int requestCode,  String[] permissions,  int[] grantResults) {
            switch (requestCode){
                case 1:
                    if(grantResults.length>0 && grantResults[0]==PackageManager.PERMISSION_GRANTED){
                        //权限已成功申请
                    }else{
                        //用户拒绝授权
                        //Toast.makeText(this, "无法获取权限", Toast.LENGTH_SHORT).show();
                        //finish();
                    }
                    break;
                default:
                    super.onRequestPermissionsResult(requestCode, permissions, grantResults);
            }
        }

	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		super.onCreateOptionsMenu(menu);
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}
	

    
       /* 		new AlertDialog.Builder(MainActivity.this)
    			.setTitle("提示")  
    	        .setMessage("是否退出？")  
    	        .setPositiveButton("确定", new DialogInterface.OnClickListener() {
    	            
    	            @Override
    	            public void onClick(DialogInterface dialog, int which) {
    	             // TODO Auto-generated method stub
    	            	MainActivity.this.finish(); 
    	            	System.exit(0);  
    	            }
    	           })
    	        .setNegativeButton("取消", null)  
    	        .show(); */
        	
    
    private Button.OnClickListener mClickListenerSpace = new Button.OnClickListener() 
	{   
        @Override  
        public void onClick(View arg0) 
        {   
        	

        	if(Public.warinflag == 0)
        	{
        		openOptionsMenu();  
        	}
        }          
    }; 
    
    
    
    
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case R.id.menu_settings:
            //设置
        	menu_set();
            return true;
        case R.id.menu_whatsnew:
            //更新记录
        	menu_new();
            return true;
        /*case R.id.menu_faceback:
        	
            //反馈
        	menu_faceback();
            return true;*/
        case R.id.menu_update:
            //反馈
        	menu_update();
            return true;
        case R.id.menu_about:
            //关于
        	menu_abt();
            return true;
        }
        return false; //should never happen
    }
    
    private void menu_update() {
		// TODO Auto-generated method stub
    	UpdateManager manager = new UpdateManager(MainActivity.this);
		// 检查软件更新
		manager.checkUpdate();
	}


    public static void makeToast(String s){
            m_handler.sendMessage(m_handler.obtainMessage(1, s));
        }

    public static void exitApp(String s){
            m_handler.sendMessage(m_handler.obtainMessage(0, s));
        }

    public static void checkUpdate(String s){
            m_handler.sendMessage(m_handler.obtainMessage(2, s));
        }

    public static void uploadSaves(String s){
            m_handler.sendMessage(m_handler.obtainMessage(3, s));
        }

     public static void statusBar(String s){
                m_handler.sendMessage(m_handler.obtainMessage(4, s));
            }

     public static void statusBarAdd(String s){
                m_handler.sendMessage(m_handler.obtainMessage(5, s));
            }

    private void menu_set()
    {
		try
		{
    	new AlertDialog.Builder(MainActivity.this)
    	.setTitle("设置")  
    	.setItems(new String[] { "1/6设置横屏显示","2/6重置竖屏显示",
    			"3/6开启实时存档功能","4/6关闭实时存档功能","5/6开启触屏操作","6/6关闭触屏操作"}, new DialogInterface.OnClickListener(){                                        
    		@Override
    		public void onClick(DialogInterface dialog, int which)
    		{
    			
    			if(which==0)
    			{
    				Toast.makeText(MainActivity.this, "如果改变了朝向，请重新开始游戏", 1).show();
    				Public.showWay=1;
                                ConfigurationFile.WritePrivateProfileString( "Startup", "ShowWay", Public.showWay+"",Public.SDPATH + "version.ini");
    				
    				MainActivity.this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    				
    			}
    			else if(which==1)
    			{
    				Toast.makeText(MainActivity.this, "如果改变了朝向，请重新开始游戏", 1).show();
    				Public.showWay=0;
                                ConfigurationFile.WritePrivateProfileString( "Startup", "ShowWay", Public.showWay+"",Public.SDPATH + "version.ini");
    				
    				MainActivity.this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    			}
    			
    			else if(which == 2)//实时存档开启
    			{
    				Toast.makeText(MainActivity.this, "实时存档开启成功！实时存档默认覆盖第一存档位置", 1).show();
    				Public.alwaysSave = 1;
    			}
    			else if(which ==3)//关闭实时存档
    			{
    				Toast.makeText(MainActivity.this, "实时存档关闭成功！", 1).show();
    				Public.alwaysSave = 0;
    			}
    			else if(which==4)//触屏允许
    			{
    				Toast.makeText(MainActivity.this, "暂不支持！", 1).show();
    				Public.TouchEnable=1;
    				
    			}
    			else if(which==5)
    			{
    				Toast.makeText(MainActivity.this, "设置成功！", 1).show();
    				Public.TouchEnable=0;
    				
    			}
    			
    		}
    	})
    	.setPositiveButton("确定", null)
    	.show(); 
		}
		catch(Exception e)
		{
			return;
		}
    	
    }
    
    private void menu_new()
    {
    	new AlertDialog.Builder(MainActivity.this)
		.setTitle("更新记录")  
        .setMessage(Public.getWhatsNewText())
        .setPositiveButton("确定", null)
        .show(); 
    }
    
    private void menu_abt()
    {
    	int v1 = Public.version_now%1000/100;
    	int v2 = Public.version_now - Public.version_now/100*100;
    	new AlertDialog.Builder(MainActivity.this)
		.setTitle("帮助信息")  
        .setMessage("基本按键如下：\r\n 方向键——移动\r\n Enter（回车）——确认 \r\n Esc——退出、返回或菜单\r\n Space（空格）——战争界面可查看武将信息\r\n Home——地图界面显示城市连接信息，战争界面显示当前战场情况\r\n 注意事项：\r\n 如遇白屏，请按任意键继续\r\n 版权信息： \r\n 天际边工作室  \r\n http://www.skysidestudio.com \r\n 版本信息： \r\n ver"+v1+"."+v2+" \r\n 官方网站：\r\n http://3gby.skysidestudio.com")  
        .setPositiveButton("确定", null)
        .show(); 
    }
    
    
    
}
