package com.skysidestudio.skyside3gby;

import static com.skysidestudio.skyside3gby.Public.Delay;
import static com.skysidestudio.skyside3gby.Public.GetFileText;

import static com.skysidestudio.skyside3gby.Public.OUTDATA;

import static com.skysidestudio.skyside3gby.Public.version_now;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.text.SimpleDateFormat;

import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPReply;
import org.apache.http.util.EncodingUtils;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
//import android.graphics.PixelXorXfermode;
import android.graphics.Typeface;
import android.os.Environment;
import android.os.Message;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.Toast;

public class Public
{
	public static String DIRPATH = Environment.getExternalStorageDirectory()
                        .getPath() + "/skyside3gby3";
	public static String SDPATH = Environment.getExternalStorageDirectory()
                        .getPath() + "/skyside3gby3/";
	public static String absolutepath = "startup.ini";
	public static String versionpath = "version.ini";
	public static int testNum = 0;//
	public static int highAtk = 0;// 高攻击
	public static Config picStyle = Bitmap.Config.ARGB_8888;// 图像制式
	public static boolean picCycle = false;
	// main
	public static int version_now = 171230170;//。ini
	// save
	public static int version_save = 0;

	public static int color = 0;// 彩色版-1或黑白版-0

	public static int showWay = 0;// 显示方向 0-纵向 1-横版

	public static int flag = 0;

	public static int KeyCodeFlag = 0;
	public static int KeyInvalidateFlag = 1;
	public static int KeyInvalidateFlag2 = 1;// 20170210用于屏幕触屏事件更新界面控制
	public static int DoTouchFlag = 0;
	public static int state = 0;
        public static final int KeyReturn = 8;
        public static final int KeyLeft0 = 1;
        public static final int KeyRight0 = 2;
        public static final int KeyUp0 = 3;
        public static final int KeyDown0 = 4;
        public static final int KeyEsc = 5;
        public static final int KeySpace = 6;
        public static final int KeyHome = 7;
        public static final int ENDL = -30317;
        public static final int SPACE = -30521;
        public static final int OUTDATA = -30001;
        public static final int ARRAYMAX = 65535;
        public static final int TEXTMAX = 1000;

        public static final int DSTINVERT = -1;
        public static final int SRCCOPY = 1;
        public static final int SRCAND = 2;
        public static final int SRCPAINT = 3;
        public static final int TILE = 9;
	// public static final int SPR=-1;

	public static int screenWidth;

	public static String[][] storetext = new String[22][TEXTMAX];
	public static String[] texta = new String[TEXTMAX];
	public static String[][] configStored = new String[100][4];//file/section/name/value
	public static int configStoredMax = 0;
	public static int id = 0;// 用于管理infile的值，配合numarray使用
	public static int[][] storenumarray = new int[39][ARRAYMAX];
	public static int[] numarray = new int[ARRAYMAX];
	public static double[] numarraydouble = new double[ARRAYMAX];
	// fight
	public static int useableskillnum2 = 0;

	public static int menushow1;
	public static int menumove1;
	public static int menuspeed1;

	
	public static int KeyEnable;
	public static int TouchEnable;
	
        public static int atnowhero = 0;
        public static int atnowheroat = 0;
        public static int tmpposx = 0;
        public static int tmpposy = 0;
        public static int tmparrowx = 0;
        public static int tmparrowy = 0;
        public static int tmpstartx = 0;
        public static int tmpstarty = 0;

        public static int flagunable = 0;
        public static int flagshowsj = 0;
        public static int flagshowsw = 0;

        public static int flagshowhurt = 0;
        public static int flagshowskillhurt = 0;
	public static int ex = 0;
	public static int aimat = 0;
	public static int hurtmax = 0;
	public static int nouse_new_flag1 = 0;
	public static int new_flag1 = 0;

	public static String[] textb = new String[TEXTMAX];// 用于技能存储
	public static int skillFlag = 0;

	// gamemap
	public static int touxiangflag = 0;// 投降了
	public static int swFlag = 0;// 君主死亡标志
	public static int zaoxiangflag = 0;
	public static int quanxiangflag = 0;
	public static int lijianflag = 0;
	public static int daojuflag = 0;
	public static int citychooseflag = 0;
	public static int operateflag = 0;
	public static int outflag = 0;
	public static int zujiang = 0;
	public static int my_zujiang = 0;
	public static int tasknum = 0;
	public static int[] tmp0 = new int[4];
	public static int fgttmpzj = 0;
	public static int fg = 0;
	public static int[] warherono = new int[10];
	public static int warherononum = 0;
	public static int[] asdsad = new int[10];
	public static int adasdsadd = 0;
	public static int warinflag = 0;// 进入战争的flag,用于确认有几次war
	public static int reqcitynum = 0, reqcityto = 0, reqresult = 0;
	public static int captiveflag = 0;
	public static int fighttodeathflag=0;
	public static int endthisflag = 0;// 君主遭劫
	public static int newownerflag = 0;
	public static int warlistflag = 0;// 防守时选择武将标志
	public static int tmpflag = 0;// 防守战已触发。
	public static int my_unx;
	public static int my_uny;
	public static int new_flag = 0;
	public static int alwaysSave = 0;//170512
	public static int saveTemp = 0;
	public static int tempFlagForCap = 0;// 用于测试武将俘虏问题
	public static int tempFlagForWar = 0;// 用于测试战斗错误问题

	public static int keydownEnable = 0;// 初始界面控制按键使能
	public static int keydownMainEnable = 1;
	public static int tempvar = 0;

	public static String[] filename_existed = new String[22];
	public static int filename_current_max = 0;
	public static int[] filename_existed_maxlen = new int[22];

	public static String[] sdtfilename_existed = new String[39];
	public static int sdtfilename_current_max = 0;
	public static int[] sdtfilename_existed_maxlen = new int[39];

	public static int arg0 = 0;
	public static int arg1 = 0;
	public static int arg2 = 0;
	public static int arg3 = 0;

	public static int timeTemp = 0;// 显示用变量20170205
	public static int touchH = 0;//垂直移动距离20170902
	public static int touchV = 0;//水平移动距离20170902

	public static int MouseStartX = 0;// 20170205
	public static int MouseStartY = 0;// 20170205
	public static int MouseEndX = 0;// 20170205
	public static int MouseEndY = 0;// 20170205
	public static int MouseDown = 0;// 左键已经按下
	public static int MouseWheelLen = 0;// 滚轮距离

	public static Typeface  g_typeface;
	public static final int MAINLEFTARROWX = 0;
	public static final int MAINLEFTARROWY = 224;
	public static final int MAINRIGHTARROWX = 640;
	public static final int MAINRIGHTARROWY = 224;
	public static final int MAINUPARROWX = 320;
	public static final int MAINUPARROWY = 0;
	public static final int MAINDOWNARROWX = 320;
	public static final int MAINDOWNARROWY = 448;
	public static final int MAINMENUX = 690;
	public static final int MAINMENUY = 420;

	public static final int FIGHTLEFTARROWX = 0;
	public static final int FIGHTLEFTARROWY = 185;
	public static final int FIGHTRIGHTARROWX = 760;
	public static final int FIGHTRIGHTARROWY = 185;
	public static final int FIGHTUPARROWX = 380;
	public static final int FIGHTUPARROWY = 0;
	public static final int FIGHTDOWNARROWX = 380;
	public static final int FIGHTDOWNARROWY = 370;
	public static final int FIGHTMENUX = 690;
	public static final int FIGHTMENUY = 420;
	public static final int FIGHTBACKX = 690;
	public static final int FIGHTBACKY = 420;

	// 24指令
	public static final int RECLAIM = 1;
	public static final int INVESTMENT = 2;
	public static final int SEARCH = 3;
	public static final int GOVERN = 4;
	public static final int TOUR = 5;
	public static final int SURRENDER = 6;
	public static final int BEHEAD = 7;
	public static final int EXILE = 8;
	public static final int AWARD = 9;
	public static final int CONFISCATE = 10;
	public static final int TRADE = 11;
	public static final int BANQUET = 12;
	public static final int TRANSPORT = 13;
	public static final int MOVE = 14;
	public static final int ALIENATE = 15;
	public static final int SOLICIT = 16;
	public static final int INSTIGATION = 17;
	public static final int ANTI = 18;
	public static final int CAPITULATE = 19;
	public static final int SCOUT = 20;
	public static final int CONSCRIPTION = 21;
	public static final int ALLOCATION = 22;
	public static final int PLUNDER = 23;
	public static final int EXPEDITION = 24;

	public static final int NEWEMPEROR = 25;// 新君主选择武将
	public static final int DEFENSE = 26;// 被攻打选择武将

	// 4时期
	public static final int DZNQ = 0;
	public static final int CCJQ = 1;
	public static final int CBZZ = 2;
	public static final int SGDL = 3;

	// tasklist
	public static final int SEARCHTASK = 1;
	public static final int SURRENDERTASK = 2;
	public static final int TRANSPORTTASK = 3;
	public static final int SOLICITTASK = 5;
	public static final int INSTIGATIONTASK = 6;
	public static final int CAPITULATETASK = 7;
	public static final int EXPEDITIONTASK = 8;
	public static final int PLUNDERTASK = 9;
	public static final int DEFENCETASK = 18;// 防守战
	public static final int WARTASK = -8;// 完成武将选择

	// 战斗消息显示定义
	public static final int WARINTELLIGENCE = 0;
	public static final int HEROLEVELUP = 1;
	public static final int HERODEATH = 2;
	public static final int ENEMYLEVELUP = 3;
	public static final int ENEMYDEATH = 4;

	// 战斗技能定义
	public static final int FIREATTACK = 0;
	public static final int LOGROLLING = 1;
	public static final int ROCKFALL = 2;
	public static final int FIGHTBRAVELY = 3;
	public static final int FLYARROW = 4;
	public static final int ARROWRAIN = 5;
	public static final int FIREARROW = 6;
	public static final int TRAMPLE = 7;
	public static final int CHARGE = 8;
	public static final int ASSAULT = 9;
	public static final int FLOODING = 10;
	public static final int IMPACT = 11;
	public static final int CURSESEAL = 12;
	public static final int IMMOBILIZED = 13;
	public static final int GOSSIP = 14;
	public static final int REINFORCEMENTS = 15;
	public static final int LARGEFIRE = 16;
	public static final int TSUNAMI = 17;
	public static final int INVISIBLEMAGIC = 18;
	public static final int LARGEREINFORCEMENTS = 19;
	public static final int SIEGE = 20;
	public static final int SUDDENSTRIKE = 21;
	public static final int WEATHERCHANGE = 22;
	public static final int STONEHENGE = 23;
	public static final int SPY = 24;
	// 战斗状态定义

	public static final int NORMALSTATUS = 0;
	public static final int CHAOSSTATUS = 1;
	public static final int CURSESEALSTATUS = 2;
	public static final int IMMOBILIZEDSTATUS = 3;
	public static final int INVISIBLEMAGICSTATUS = 4;
	public static final int RUNAWAYMAGICSTATUS = 5;
	public static final int STONEHENGESTATUS = 6;
	public static final int INVISIBLESTATUS = 7;

	// 兵种定义
	public static final int CAVALRY = 0;
	public static final int INFANTRY = 1;
	public static final int ARCHER = 2;
	public static final int SAILOR = 3;
	public static final int GREATHPARM = 4;
	public static final int GREATMPARM = 5;

	// 地形定义
	public static final int PLAIN = 0;
	public static final int GRASS = 1;
	public static final int MOUNTAIN = 2;
	public static final int FOREST = 3;
	public static final int VILLAGE = 4;
	public static final int CITY = 5;
	public static final int CAMP = 6;
	public static final int RIVER = 7;

	// 战斗定义
	public static final int ATTACK = 0;
	public static final int SKILL = 1;
	public static final int STATUS = 2;
	public static final int WAIT = 3;

	// 天气定义
	public static final int SUNNY = 1;
	public static final int CLOUDY = 2;
	public static final int WIND = 3;
	public static final int RAIN = 4;
	public static final int ICE = 5;

	// 城市状态定义
	public static final int NORMAL = 0;
	public static final int FAMINE = 1;
	public static final int DROUGHT = 2;
	public static final int FLOOD = 3;
	public static final int RIOT = 4;

	// 各时期起始年定义
	public static final int DZNQYEAR = 190;
	public static final int CCJQYEAR = 198;
	public static final int CBZZYEAR = 208;
	public static final int SGDLYEAR = 225;

	// map
	public static final int MAPMAX = 32;

	// hero
	public static final int HEROMAX = 200;

	// city
	public static final int CITYMAX = 50;

	// GOOD
	public static final int GOODMAX = 50;

	// TASKLIST
	public static final int TASKLISTMAX = 255;

	public static final int TEXTAMAX = 255;

	public static final int NORMALSTRLEN = 50;

	// 武将列表属性定义
	public static final int HEROOFTHISCITY = 0; // 自己城市武将队列
	public static final int CAPTIVEOFTHISCITYADD = 1; // （自己城市武将队列后显示）自己城市俘虏队列
	public static final int HEROANDCAPTIVEOFMYCITY = 2; // 自己城市武将+俘虏队列
	public static final int HEROOFOTHEREMPEROR = 3; // （自己城市武将队列后显示）其他君主其他城市武将队列（不含君主）
	public static final int HEROANDEMPEROROFOTHER = 4; // （自己城市武将队列后显示）其他君主队列
	public static final int CAPTIVEOFTHISCITY = 5; // 自己城市俘虏队列
	public static final int HEROOFMYALLCITY = 6; // 自己所有武将队列
	public static final int HEROlISTTYPEMAX = 7;

	public static final int LANDSTYLEMAX = 8;
	public static final int HEROTYPEMAX = 6;
	public static final int TILESTYLEMAX = 50;
	public static final int DIRCTIONMAX = 8;
	public static final int FIGHTHEROMAX = 10;
	public static final int DEGREEMAX = 20;
	public static final int MAPSUM = 7; // 地图数量上限
	public static final int SPECIALHEROMAX = 10;// 特殊技能武将数量
	public static final int HEROSKILLMAX = 20;// 武将拥有的技能上限
	public static final int STRMAX = 1000;
	public static final int LINEMAX = 80;
	public static final int HERONAMEMAXLEN = 10;
	public static final int SKILLMAX = 50;
	public static final int SKILLNAMEMAXLEN = 5;
	public static final int GOODNAMEMAXLEN = 10;
	public static final int CITYNAMEMAXLEN = 10;
	public static final int TILEMAX = 50;// 地图tile片的上限
	public static final int SKILLLEN = 32;

	public static final int BLACKVERSION = 0;
	public static final int COLORVERSION = 1;
	public static final int USEHDC = 0;
	public static final int USEHDCTMP = 1;
	public static final int FONTA = 0;
	public static final int FONTB = 1;
	
	public static final int PERIODMAX = 8;
	public static final int PERIODLINEMAX = 2;

	public static int[][] arrow = new int[MAPMAX][MAPMAX];// 移动
	public static int[][] atarr = new int[MAPMAX][MAPMAX];// 普通攻击
	public static int[][] skillarr = new int[MAPMAX][MAPMAX];// 技能

	public static String[][] strText = new String [STRMAX][2];
	public static String t_str;
	public static String[] t_heroName = new String [HEROMAX];
	public static String[] t_skillName = new String [SKILLMAX];
	public static String[] t_goodName = new String [GOODMAX];
	public static String[] t_cityName = new String [CITYMAX];
	public static int[][][] t_tile = new int[MAPSUM][MAPMAX][MAPMAX];
	public static int[] t_citymapnum = new int[CITYMAX];
	public static int[] t_tilestyle = new int[TILEMAX];
	
	public static int[][] periodinfo = new int[PERIODMAX][PERIODLINEMAX];
	public static int periodsum = 4;
	
	//public static GameView gameview;

	public static void Delay(int time)
	{
		try
		{
			Thread.sleep(time);
		} catch (InterruptedException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	/***********************************************************************
	 * 说明: 获取文本函数 输入参数: char *filename——文件名称 返回值 : 无 功能 : 获取文本内容 修改历史: 姓名 日期 说明
	 * ------ ---------- ------------- 吴亭杰 2012-10-5 完成基本功能
	 ***********************************************************************/
	public static int GetFileText(String filename)
	{
		int tmpvar = 0;
		for (int i = 0; i < 22; i++)
		{
			if (filename_existed[i] == filename)
			{
				tmpvar = i;// 内存已经有数据
				break;
			}
		}
		if (tmpvar == 0)
		{
			// 20140615增加判断是否在sd卡中存在，若存在，则sd卡中读取，反之，asset读取后，写入sd卡
			File path = new File(DIRPATH);
			if (!path.exists())
			{
				path.mkdirs();
			}
			File sdfilename = new File(SDPATH + filename);
			if (!sdfilename.exists())// asset读取后，写入sd卡
			{
				Context context = ContextUtil.getInstance();
				Log.v("filename", filename);
				Log.v("sdfilename", "" + sdfilename);
				String res = "";
				if (filename_current_max > 21)
					filename_current_max = 0;
				filename_existed[filename_current_max] = filename;
				try
				{
					InputStream in = context.getResources().getAssets()
							.open(filename);
					int length = in.available();
					byte[] buffer = new byte[length];
					in.read(buffer);
					res = EncodingUtils.getString(buffer, "GB2312");
					String[] ary = res.toString().split(" |\r\n");
					// Log.v("res_name",res);
					for (int i = 0; i < ary.length; i++)
					{
						texta[i] = ary[i];
						storetext[filename_current_max][i] = ary[i];
						Log.v("texta", storetext[filename_current_max][i]);
					}
					filename_existed_maxlen[filename_current_max] = ary.length;
					filename_current_max++;
					in.close();

					// 写入sd卡
					FileOutputStream fout = new FileOutputStream(sdfilename);
					// byte [] bytes = res.getBytes();
					fout.write(buffer);
					fout.close();
					return ary.length;
				} catch (Exception e)
				{

					e.printStackTrace();
				}
			} else
			// sd卡中读取
			{
				String res = "";
				if (filename_current_max > 21)
					filename_current_max = 0;
				filename_existed[filename_current_max] = filename;
				try
				{

					FileInputStream fin = new FileInputStream(sdfilename);
					int length = fin.available();
					byte[] buffer = new byte[length];
					fin.read(buffer);
					res = EncodingUtils.getString(buffer, "GB2312");
					String[] ary = res.toString().split(" |\r\n");
					// Log.v("res_name",res);
					for (int i = 0; i < ary.length; i++)
					{
						texta[i] = ary[i];
						storetext[filename_current_max][i] = ary[i];
						Log.v("texta", storetext[filename_current_max][i]);
					}
					filename_existed_maxlen[filename_current_max] = ary.length;
					filename_current_max++;
					fin.close();
					return ary.length;
				} catch (Exception e)
				{
					e.printStackTrace();
				}

			}

		} else
		{
			for (int i = 0; i < filename_existed_maxlen[tmpvar]; i++)
			{
				texta[i] = storetext[tmpvar][i];
				// Log.v("texta",texta[i]+" a "+tmpvar);
			}
			return filename_existed_maxlen[tmpvar];

		}
		return 0;
		
	}
	
	

	public static int PlcExtract(int num)// 求平方根
	{
		int gen, step;
		int tcnt;

		gen = 0;
		step = 10;
		while (true)
		{
			if (step == 0)
				break;
			tcnt = gen + step;
			if ((int) tcnt * tcnt < num)
				gen = tcnt;
			else
				step -= 1;
		}
		return gen;
	}

	

	public static String getWhatsNewText()
	{
		Context context = ContextUtil.getInstance();
		String res = "";

		try
		{
			InputStream in = context.getResources().getAssets()
					.open("whatsnew.txt");
			int length = in.available();
			byte[] buffer = new byte[length];
			in.read(buffer);
			res = EncodingUtils.getString(buffer, "GB2312");
		} catch (Exception e)
		{
			res = "";
		}

		return res;
	}

	public static void setAssetText()
	{
		Context context = ContextUtil.getInstance();
		String res = "";

		try
		{
			InputStream in = context.getResources().getAssets()
					.open("whatsnew.txt");
			int length = in.available();
			byte[] buffer = new byte[length];
			in.read(buffer);
			res = EncodingUtils.getString(buffer, "GB2312");
		} catch (Exception e)
		{
			res = "";
		}

		return;
	}

	

	public static String getMethodName()
	{
		StackTraceElement[] stacktrace = Thread.currentThread().getStackTrace();
		StackTraceElement e = stacktrace[3];
		String methodName = e.getMethodName();
		String fileName = e.getFileName();
		return fileName + "::" + methodName;
	}

	public static void DeleteLog()
	{
		File path = new File(SDPATH + "Log/");
		if (!path.exists())
		{
			path.mkdirs();
		}
		SimpleDateFormat format1 = new SimpleDateFormat("yyyyMMdd");
		String t = format1.format(new java.util.Date());
		File sdfilename = new File(SDPATH + "Log/" + t + ".log");
		if (sdfilename.exists())
		{
			sdfilename.delete();
		}

		return;

	}

	/**
	 * 通过ftp上传文件
	 * 
	 * @param url
	 *            ftp服务器地址 如： 192.168.1.110
	 * @param port
	 *            端口如 ： 21
	 * @param username
	 *            登录名
	 * @param password
	 *            密码
	 * @param remotePath
	 *            上到ftp服务器的磁盘路径
	 * @param fileNamePath
	 *            要上传的文件路径
	 * @param fileName
	 *            要上传的文件名
	 * @return
	 */
	public static String ftpUpload(String url, String port, String username,
			String password, String remotePath, String fileNamePath,
			String fileName)
	{
		FTPClient ftpClient = new FTPClient();
		FileInputStream fis = null;
		String returnMessage = "0";
		try
		{
			ftpClient.connect(url, Integer.parseInt(port));
			boolean loginResult = ftpClient.login(username, password);
			int returnCode = ftpClient.getReplyCode();
			if (loginResult && FTPReply.isPositiveCompletion(returnCode))
			{// 如果登录成功
				ftpClient.makeDirectory(remotePath);
				// 设置上传目录
				ftpClient.changeWorkingDirectory(remotePath);
				ftpClient.setBufferSize(1024);
				ftpClient.setFileType(FTPClient.BINARY_FILE_TYPE);
				// ftpClient.setControlEncoding("UTF-8");
				ftpClient.enterLocalPassiveMode();
				fis = new FileInputStream(fileNamePath + fileName);
				ftpClient.storeFile(fileName, fis);

				returnMessage = "1"; // 上传成功
			} else
			{// 如果登录失败
				returnMessage = "0";
			}

		} catch (IOException e)
		{
			e.printStackTrace();
			throw new RuntimeException("FTP客户端出错！", e);
		} finally
		{
			// IOUtils.closeQuietly(fis);
			try
			{
				ftpClient.disconnect();
			} catch (IOException e)
			{
				e.printStackTrace();
				throw new RuntimeException("关闭FTP连接发生异常！", e);
			}
		}
		return returnMessage;
	}



	public static int checkNewVersion(Context mcontext)
	{
		
		int versionCode = 0;
		try
		{
                        versionCode = mcontext.getPackageManager().getPackageInfo("com.skysidestudio.skyside3gby", 0).versionCode;
		}
		catch (NameNotFoundException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		int assertver = 0;
		File file1 = new File(Public.SDPATH + "version.ini");
		if(!file1.exists())//在文件不存在的情况下直接复制2个ini文件，并读取数据（按新安装处理）//设定为小版本需要更新assert
                {
			try
			{
				copyIniToSD("version.ini");
				copyIniToSD("startup.ini");
				Public.showWay=ConfigurationFile.GetPrivateProfileInt("Startup", "ShowWay", 0, Public.absolutepath );
				Public.color=ConfigurationFile.GetPrivateProfileInt("Startup", "Color", Public.color, Public.absolutepath );
				Public.version_now=ConfigurationFile.GetPrivateProfileInt("Startup", "Version", 130818100, Public.versionpath );//130325021;
				Public.menushow1=ConfigurationFile.GetPrivateProfileInt("Startup", "Menushow", 0, Public.absolutepath );
				Public.menumove1=ConfigurationFile.GetPrivateProfileInt("Startup", "Menumove", 0, Public.absolutepath );
				Public.menuspeed1=ConfigurationFile.GetPrivateProfileInt("Startup", "Menuspeed", 0, Public.absolutepath );
				Public.KeyEnable = ConfigurationFile.GetPrivateProfileInt("Startup", "UseKey", 1, Public.absolutepath );
				Public.TouchEnable = ConfigurationFile.GetPrivateProfileInt("Startup", "UseTouch", 1, Public.absolutepath );
				
			}
			catch (Exception e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			};
			return 0;
                }
		else
		{
		
			assertver =   ConfigurationFile.GetPrivateProfileInt( "Startup", "VerNo", 18,"version.ini");
			

		}
		
                if(assertver < versionCode)//需要更新资源
		{
                    showNoticeDialog(mcontext,versionCode*1000+assertver);
                    return 1;
		}
		
		return 0;
	}
	
	public static void setSDVersion(int versionCode)
	{
		ConfigurationFile.WritePrivateProfileString( "Startup", "VerNo", versionCode+"",SDPATH + "version.ini");
		
	}

	public static void showNoticeDialog(Context mcontext,final int versionCode)
	{
		AlertDialog.Builder builder = new Builder(mcontext);
		builder.setTitle("请更新资源文件");
                builder.setMessage("是否删除SD卡中旧版本资源数据(非存档)?");

		builder.setPositiveButton("确定", new OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				dialog.dismiss();
                                //DeleteAsserts();
				try
				{
					copyIniToSD("version.ini");
					copyIniToSD("startup.ini");
					Public.showWay=ConfigurationFile.GetPrivateProfileInt("Startup", "ShowWay", 0, Public.absolutepath );
					Public.color=ConfigurationFile.GetPrivateProfileInt("Startup", "Color", Public.color, Public.absolutepath );
					Public.version_now=ConfigurationFile.GetPrivateProfileInt("Startup", "Version", 130818100, Public.versionpath );//130325021;
					Public.menushow1=ConfigurationFile.GetPrivateProfileInt("Startup", "Menushow", 0, Public.absolutepath );
					Public.menumove1=ConfigurationFile.GetPrivateProfileInt("Startup", "Menumove", 0, Public.absolutepath );
					Public.menuspeed1=ConfigurationFile.GetPrivateProfileInt("Startup", "Menuspeed", 0, Public.absolutepath );
					Public.KeyEnable = ConfigurationFile.GetPrivateProfileInt("Startup", "UseKey", 1, Public.absolutepath );
					Public.TouchEnable = ConfigurationFile.GetPrivateProfileInt("Startup", "UseTouch", 1, Public.absolutepath );
					
				}
				catch (Exception e)
				{
					// TODO Auto-generated catch block
					e.printStackTrace();
				};
				//setSDVersion(versionCode);
			}
		});
 
		builder.setNegativeButton("取消", new OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				dialog.dismiss();
				try
				{
					copyIniToSD("version.ini");
				}
				catch (Exception e)
				{
					// TODO Auto-generated catch block
					e.printStackTrace();
				};
			}
		});
		Dialog noticeDialog = builder.create();
		noticeDialog.show();
	}
	
	
	public static void copyIniToSD(String filename) throws IOException 
    {  
        InputStream myInput;  
        java.io.OutputStream myOutput = new FileOutputStream(Public.SDPATH + filename);  
        Context context = ContextUtil.getInstance();
        myInput = context.getResources().getAssets().open(filename);  
        byte[] buffer = new byte[1024];  
        int length = myInput.read(buffer);
        while(length > 0)
        {
            myOutput.write(buffer, 0, length); 
            length = myInput.read(buffer);
        }
        
        myOutput.flush();  
        myInput.close();  
        myOutput.close();        
    } 
	
	
	
}
