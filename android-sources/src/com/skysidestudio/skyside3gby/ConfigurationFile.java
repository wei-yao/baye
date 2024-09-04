package com.skysidestudio.skyside3gby;

/*
* ConfigurationFile.java
*
* Created on 2009年4月15日, 下午1:36
*
* To change this template, choose Tools | Template Manager
* and open the template in the editor.
*/

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
/**
* 这是个配置文档操作类，用来读取和配置ini配置文档
* @author 由月
* @version 2004-08-18
* @修改 2008-05-22
*/
public final class ConfigurationFile 
{
    /**
     * 从ini配置文档中读取变量的值
     * @param file 配置文档的路径
     * @param section 要获取的变量所在段名称
     * @param variable 要获取的变量名称
     * @param defaultValue 变量名称不存在时的默认值
     * @return 变量的值
     * @throws IOException 抛出文档操作可能出现的io异常
     */
	
	
	public static int InitPrivateProfile(String file)
	{
		String strLine, value = "";
		File file1 = new File(Public.SDPATH + file);
        
        if(!file1.exists())
        {
        	try
			{
				Public.copyIniToSD(file);
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        }
        BufferedReader bufferedReader = null;
		try
		{
			bufferedReader = new BufferedReader(new FileReader(Public.SDPATH + file));
		}
		catch (FileNotFoundException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
			return -1;
		}
        boolean isInSection = false;
        int ii = Public.configStoredMax;
        String tmpsection = "";
        try {
            try
			{
				while ((strLine = bufferedReader.readLine()) != null) 
				{
				    strLine = strLine.trim();
				    if(strLine.indexOf("[")!=-1)
				    {
				    	tmpsection=strLine.replace("[","").replace("]",""); 
				    }
				    else
				    {
				        String[] strArray = strLine.split("=");
				        if (strArray.length == 1) 
				        {
				            value = strArray[0].trim();
				            Public.configStored[ii][0] = file;
				            Public.configStored[ii][1] = tmpsection;
				            Public.configStored[ii][2] = value;
				            Public.configStored[ii][3] = "";
				        } 
				        else if (strArray.length == 2) 
				        {
				            value = strArray[0].trim();
				            Public.configStored[ii][0] = file;
				            Public.configStored[ii][1] = tmpsection;
				            Public.configStored[ii][2] = value;
				            value = strArray[1].trim();
				            Public.configStored[ii][3] = value;
				        } 
				        else if (strArray.length > 2) 
				        {
				            value = strArray[0].trim();
				            Public.configStored[ii][0] = file;
				            Public.configStored[ii][1] = tmpsection;
				            Public.configStored[ii][2] = value;
				            value = strLine.substring(strLine.indexOf("=") + 1).trim();
				            Public.configStored[ii][3] = value;
				            
				        }
				        ii ++;
				        if(ii >= 100)ii = 0;
				    }
				}
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        } finally {
            try
			{
				bufferedReader.close();
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        }
        Public.configStoredMax = ii;
		
		return 0;
	}
	
    public static String GetPrivateProfileString(
    		String section,
            String variable,
            String defaultValue,
            String file){
        /*String strLine, value = "";
        File file1 = new File(Public.SDPATH + file);
        
        if(!file1.exists())
        {
        	try
			{
				Public.copyIniToSD(file);
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        }
        BufferedReader bufferedReader = null;
		try
		{
			bufferedReader = new BufferedReader(new FileReader(Public.SDPATH + file));
		}
		catch (FileNotFoundException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
			return defaultValue;
		}
        boolean isInSection = false;
        try {
            try
			{
				while ((strLine = bufferedReader.readLine()) != null) {
				    strLine = strLine.trim();
				    //strLine = strLine.split("[;]")[0];
				    Pattern p;
				    Matcher m;
				    p = Pattern.compile("\\["+section+"\\]");
				    m = p.matcher((strLine));
				    if (m.matches()) {
				        p = Pattern.compile("\\["+section+"\\]");
				        m = p.matcher(strLine);
				        if (m.matches()) {
				            isInSection = true;
				        } else {
				            isInSection = false;
				        }
				    }
				    if (isInSection == true) {
				        strLine = strLine.trim();
				        String[] strArray = strLine.split("=");
				        if (strArray.length == 1) {
				            value = strArray[0].trim();
				            if (value.equalsIgnoreCase(variable)) {
				                value = "";
				                return value;
				            }
				        } else if (strArray.length == 2) {
				            value = strArray[0].trim();
				            if (value.equalsIgnoreCase(variable)) {
				                value = strArray[1].trim();
				                return value;
				            }
				        } else if (strArray.length > 2) {
				            value = strArray[0].trim();
				            if (value.equalsIgnoreCase(variable)) {
				                value = strLine.substring(strLine.indexOf("=") + 1).trim();
				                return value;
				            }
				        }
				    }
				}
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        } finally {
            try
			{
				bufferedReader.close();
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        }*/
    	for(int i = 0 ; i < Public.configStoredMax ; i ++)
    	{
    		if(Public.configStored[i][0].equals(file) && Public.configStored[i][1].equals(section) && Public.configStored[i][2].equals(variable))
    		{
    			return Public.configStored[i][3];
    		}
    	}
        return defaultValue;
    }
    
    public static int GetPrivateProfileInt(
            String section,
            String variable,
            int defaultValue,
            String file){
    	return Integer.parseInt(GetPrivateProfileString(section,variable,defaultValue+"",file));
    }
    /**
     * 修改ini配置文档中变量的值
     * @param file 配置文档的路径
     * @param section 要修改的变量所在段名称
     * @param variable 要修改的变量名称
     * @param value 变量的新值
     * @throws IOException 抛出文档操作可能出现的io异常
     */
    public static boolean WritePrivateProfileString(
            String section,
            String variable,
            String value,
            String file) {
        String fileContent, allLine,strLine, newLine, remarkStr;
        String getValue;
        BufferedReader bufferedReader =null;
		try
		{
			bufferedReader = new BufferedReader(new FileReader(Public.SDPATH + file));
		}
		catch (FileNotFoundException e1)
		{
			// TODO Auto-generated catch block
			e1.printStackTrace();
			return false;
		}
        boolean isInSection = false;
        fileContent = "";
        try {
            
            while ((allLine = bufferedReader.readLine()) != null) {
                allLine = allLine.trim();
                System.out.println("allLine == "+allLine);
                strLine = allLine;
                Pattern p;
                Matcher m;
                p = Pattern.compile("\\["+section+"\\]");
                m = p.matcher((strLine));
                //System.out.println("+++++++ ");
                if (m.matches()) {
                    System.out.println("+++++++ ");
                    p = Pattern.compile("\\["+section+"\\]");
                    m = p.matcher(strLine);
                    if (m.matches()) {
                        System.out.println("true ");
                        isInSection = true;
                    } else {
                        isInSection = false;
                        System.out.println("+++++++ ");
                    }
                }
                
                if (isInSection == true) {
                    
                    strLine = strLine.trim();
                    String[] strArray = strLine.split("=");
                    getValue = strArray[0].trim();
                    
                    if (getValue.equalsIgnoreCase(variable)) {
                        // newLine = getValue + " = " + value + " " + remarkStr;
                        
                        newLine = getValue + " = " + value + " ";
                        fileContent += newLine + "\r\n";
                        while ((allLine = bufferedReader.readLine()) != null) {
                            fileContent += allLine + "\r\n";
                        }
                        bufferedReader.close();
                        BufferedWriter bufferedWriter =
                                new BufferedWriter(new FileWriter(file, false));
                        bufferedWriter.write(fileContent);
                        bufferedWriter.flush();
                        bufferedWriter.close();
                        
                        return true;
                    }
                }
                fileContent += allLine + "\r\n";
            }
        }catch(IOException ex){
            
        } finally {
            try
			{
				bufferedReader.close();
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        }
        return false;
    }
}
