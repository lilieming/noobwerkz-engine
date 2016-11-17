package net.noobwerkz.sampleapp;

import net.noobwerkz.sampleapp.GameView;
import android.view.WindowManager;
import android.view.Surface;
import android.os.Bundle;
import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Environment;
import android.util.Log;
import java.io.*;
import java.util.Arrays;

public class MainActivity extends Activity
{
	GameView mView;
	@Override protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		mView = new GameView(getApplication());
		// getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON | WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		copyAssets();
		
		setContentView(mView);
	}

	@Override protected void onPause() 
	{
		super.onPause();
		mView.onPause();
	}

	@Override protected void onResume() 
	{
		super.onResume();
		mView.onResume();
	}

	@Override protected void onDestroy() 
	{
		super.onDestroy();
		mView.onDestroy();
	}
	/*
	   private void copyAssets()
	   {
	   AssetManager assetManager = getAssets();
	   String[] files = null;
	   try
	   {
	   files = assetManager.list("");
	   String allFiles = new String("");
	   for (String s: files)
	   {
	   allFiles += s;
	   allFiles += " ";
	   }
	   JNILib.Log("(Java) MainActivity - Files found in APK: " + allFiles);
	   String files_path = getExternalFilesDir(new String()).getPath();
	   JNILib.SetupArchiveDir(files_path);
	   }
	   catch (IOException e)
	   {
	   Log.e("tag", "Failed to get asset file list.", e);
	   JNILib.Log(" (Java) MainActivity - Failed to get asset file list..");
	   }
	   for(String filename : files)
	   {
	   InputStream in = null;
	   OutputStream out = null;
	   try
	   {
	   in = assetManager.open(filename);
	   File outFile = new File(getExternalFilesDir(null), filename);
	   if (outFile.isDirectory())
	   {
	   i
	   }
	   JNILib.Log("(Java) MainActivity - Copying " + outFile.getPath());
	   out = new FileOutputStream(outFile);
	   copyFile(in, out);
	   }
	   catch(IOException e)
	   {
	   Log.e("tag", "Failed to copy asset file: " + filename, e);
	   JNILib.Log("(Java) MainActivity - Failed to copy asset file: " + filename);
	   }     
	   finally
	   {
	   if (in != null)
	   {
	   try
	   {
	   in.close();
	   }
	   catch (IOException e)
	   {
	// NOOP
	}
	}
	if (out != null)
	{
	tryshaders
	{
	out.close();
	}
	catch (IOException e)
	{
	// NOOP
	}
	}
	}  
	}
	}

	private void copyFile(InputStream in, OutputStream out) throws IOException
	{
		byte[] buffer = new byte[1024];
		int read;
		while((read = in.read(buffer)) != -1)
		{
			out.write(buffer, 0, read);
		}
	}

	public static void copyDirectoryOneLocationToAnotherLocation(File sourceLocation, File targetLocation)
		throws IOException {

			if (sourceLocation.isDirectory()) {
				if (!targetLocation.exists()) {
					targetLocation.mkdir();
				}

				String[] children = sourceLocation.list();
				for (int i = 0; i < sourceLocation.listFiles().length; i++) {

					copyDirectoryOneLocationToAnotherLocation(new File(sourceLocation, children[i]),
							new File(targetLocation, children[i]));
				}
			} else {

				InputStream in = new FileInputStream(sourceLocation);

				OutputStream out = new FileOutputStream(targetLocation);

				// Copy the bits from instream to outstream
				byte[] buf = new byte[1024];
				int len;
				while ((len = in.read(buf)) > 0) {
					out.write(buf, 0, len);
				}
				in.close();
				out.close();
			}

		}
	*/
		private void copyAssets()
		{
			AssetManager assetManager = getAssets();
			String toPath = getExternalFilesDir(new String()).getPath();
			JNILib.SetupArchiveDir(toPath);
			copyAssetFolder(assetManager, "sounds", toPath + "/sounds");
			copyAssetFolder(assetManager, "font", toPath + "/font");
		}

	private static boolean copyAssetFolder(AssetManager assetManager, String fromAssetPath, String toPath)
	{
		try
		{
			String[] files = assetManager.list(fromAssetPath);
			new File(toPath).mkdirs();
			boolean res = true;
			for (String file : files)
			{
				String ss = new String("(Java) MainActivity - Copying " + file + " to " + toPath + "/" + file);
				JNILib.Log(ss);
				if (file.contains("."))
					res &= copyAsset(assetManager, fromAssetPath + "/" + file, toPath + "/" + file);
				else 
					res &= copyAssetFolder(assetManager, fromAssetPath + "/" + file, toPath + "/" + file);
			}
			return res;
		}
		catch (Exception e)
		{
			// JNILib.Log("MainActivity - Could not copy: " + e.toString());
			e.printStackTrace();
			return false;
		}
	}

	private static boolean copyAsset(AssetManager assetManager, String fromAssetPath, String toPath)
	{
		InputStream in = null;
		OutputStream out = null;
		try {
			in = assetManager.open(fromAssetPath);
			new File(toPath).createNewFile();
			out = new FileOutputStream(toPath);
			copyFile(in, out);
			in.close();
			in = null;
			out.flush();
			out.close();
			out = null;
			return true;
		}
		catch(Exception e)
		{
			e.printStackTrace();
			return false;
		}
	}

	private static void copyFile(InputStream in, OutputStream out) throws IOException
	{
		byte[] buffer = new byte[1024];
		int read;
		while((read = in.read(buffer)) != -1)
		{
			out.write(buffer, 0, read);
		}
	}
}
