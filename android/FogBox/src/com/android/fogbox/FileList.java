/*
 * Copyright (c) 2007-2020 Texas A&M University System
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the copyright holders nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package com.android.fogbox;

import java.io.File;
import java.io.FileDescriptor;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

import android.app.Activity;
import android.app.ExpandableListActivity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetFileDescriptor;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.ExpandableListAdapter;
import android.widget.ExpandableListView;
import android.widget.SimpleExpandableListAdapter;
import android.widget.Toast;

public class FileList extends ExpandableListActivity {
	
	 private ExpandableListAdapter mAdapter;
	 private static final String NAME = "NAME";
	 private static final String DETAILS = "DETAILS";
	 private static String filename = null;
	 private static NativeLib nativelib = new NativeLib();
	 private String TAG ="FileListActivity";
	 PackageInfo info = null; 
	 private int imageCount = 0;
	 private static ArrayList<String> imageList = new ArrayList<String>();
	 private static ArrayList<String> datecreatedList = new ArrayList<String>();
	 private static Set<String> set;
	 private static List<String> list;
	 private static final int ShareStatus = 1;
	 private static int shareFilePosition = -1;
	 public static final String PREFS_NAME = "MyPrefsFile";

	 
	    @Override
	    public void onCreate(Bundle savedInstanceState) {
	        super.onCreate(savedInstanceState);

	        /*try {
	      	   info = getBaseContext().getPackageManager().getPackageInfo("com.android.fogbox", 0);
	      	   Log.e( TAG, info.applicationInfo.sourceDir);
	      	} catch( NameNotFoundException e ) {
	      	  
	     		Log.e( TAG, e.toString() );
	      	   return;
	      	}
	        nativelib.setAppName(info.applicationInfo.sourceDir);*/
	        
	     // which image properties are we querying
	         SharedPreferences settings = getSharedPreferences(PREFS_NAME, 0);
		   	 SharedPreferences.Editor editor = settings.edit();
		   	 list = new ArrayList<String>();
		   	
	
	        String[] projection = new String[]{	    
	                MediaStore.Images.Media.BUCKET_DISPLAY_NAME,
	                MediaStore.Images.Media.DATE_TAKEN,
	                MediaStore.Images.Media.DISPLAY_NAME
	        };

	        // Get the base URI for the People table in the Contacts content provider.
	        Uri images = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;

	        // Make the query.
	        Cursor cur = managedQuery(images,
	                projection, // Which columns to return
	                "",         // Which rows to return (all rows)
	                null,       // Selection arguments (none)
	                ""          // Ordering
	                );

	        Log.i(TAG,"query count="+cur.getCount());
	        imageCount = cur.getCount();
	        if (cur.moveToFirst()) {
	            String bucket;
	            String date;
	            String name;
	            int bucketColumn = cur.getColumnIndex(
	                MediaStore.Images.Media.BUCKET_DISPLAY_NAME);

	            int dateColumn = cur.getColumnIndex(
	                MediaStore.Images.Media.DATE_TAKEN);
	            
	            int nameColumn = cur.getColumnIndex(
		                MediaStore.Images.Media.DISPLAY_NAME);

	            do {
	                // Get the field values
	                bucket = cur.getString(bucketColumn);
	                date = cur.getString(dateColumn);
	                name = cur.getString(nameColumn);

	                imageList.add(name);
	                datecreatedList.add(date);
	                list.add("Unshared");
	              
	    	        //String status = settings.getString("shareStatus", "Unshared");
	                //sharestatusList.add(status);
	            } while (cur.moveToNext());

	        }   
	        set = new HashSet<String>(list);
	        //list = new ArrayList<String>(set);
	        
		   	//editor.putStringSet("key",set);
		   	//editor.commit();
	    }
 public void onResume(){
	    	super.onResume();
	    	Log.d(TAG,"OnResume called ");
	    	
	    	SharedPreferences settings = getSharedPreferences(PREFS_NAME, 0);
	        //set = settings.getStringSet("key", null);
	       // ArrayList<String> list = new ArrayList<String>(set);
	        
	        List<Map<String, String>> groupData = new ArrayList<Map<String, String>>();
	        List<List<Map<String, String>>> childData = new ArrayList<List<Map<String, String>>>();
	        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm", Locale.US);
	        
	        for (int i = 0; i < imageCount; i++) {
	            Map<String, String> curGroupMap = new HashMap<String, String>();
	            groupData.add(curGroupMap);
	            //curGroupMap.put(NAME, "File " + i);
	            curGroupMap.put(NAME, imageList.get(i));
	            long milliseconds = Long.valueOf(datecreatedList.get(i));
	            Date datecreated = new Date(milliseconds);
	            //Log.d(TAG,"Date created is "+datecreated.toString());
	            //sdf.format(new Date(milliseconds),StringBuffer stringbuf,null);
	            List<Map<String, String>> children = new ArrayList<Map<String, String>>();
	            for (int j = 0; j < 4; j++) {
	                Map<String, String> curChildMap = new HashMap<String, String>();
	                children.add(curChildMap);
	                switch(j)
	                {
		                case 0:
		                	
		                curChildMap.put(NAME, "Date Created ");
		                curChildMap.put(DETAILS,"Date Created is " +datecreated.toString());
		                break;
		                case 1:
		                curChildMap.put(NAME, "Date Modified ");
		                curChildMap.put(DETAILS,"Last Date Modified is "+datecreated.toString());
		                break;
		                case 2:
		                curChildMap.put(NAME, "Shared Status ");
		                curChildMap.put(DETAILS,"File Shared Status is "+list.get(i));
		    
		                break;
		                case 3:
	                	curChildMap.put(NAME, "Share");
		                curChildMap.put(DETAILS,"Share the file");
		                break;	
	                }
	            }
	            childData.add(children);
	            
	        }
	        
	        // Set up our adapter
	        mAdapter = new SimpleExpandableListAdapter(
	                this,
	                groupData,
	                android.R.layout.simple_expandable_list_item_1,
	                new String[] { NAME, DETAILS },
	                new int[] { android.R.id.text1, android.R.id.text2 },
	                childData,
	                R.layout.expandable_list_item,
	                new String[] { NAME, DETAILS },
	                new int[] { android.R.id.text1, android.R.id.text2 }
	                );
	        setListAdapter(mAdapter);
	    }
 
	    public boolean onChildClick(ExpandableListView expandableListView, View view, int groupPosition, int childPosition, long id) {
	    	//Toast.makeText(FileList.this,"Entered and group position is"+groupPosition+"childposition is"+childPosition, Toast.LENGTH_SHORT).show();
	    	if(childPosition == 3)
	    	{
	    		shareFilePosition = groupPosition;
	    		filename = imageList.get(groupPosition);
	    		Intent shareIntent = new Intent(FileList.this, FileShareActivity.class);
	    		shareIntent.putExtra("groupID", groupPosition);
	    		shareIntent.putExtra("file_name",filename);
       		 	startActivityForResult(shareIntent, ShareStatus);
	    	}
			return true;
	    }
	    @Override
	    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
	    	// TODO Auto-generated method stub
	    	Log.d(TAG,"Onactivityresult called " + "requestCode = "+requestCode+"resultcode = "+resultCode);
	    	//if (resultCode == RESULT_OK) {
	    		// Log.d(TAG,"resultcode is Ok ");
	    		
	           if (requestCode == ShareStatus) {
	        	   Log.d(TAG,"requestcode called ");
	        	   list.set(shareFilePosition,"Shared");
	            }
	        //}
	    }
	    public static NativeLib getNativeLibInstance(){
	    	
			return nativelib;
	    	
	    }
	    public void onDestroy(){
	    	 super.onDestroy();
	    	 Log.d( TAG, "OnDestroy called");
	    	 SharedPreferences settings = getSharedPreferences(PREFS_NAME, 0);
	    	 SharedPreferences.Editor editor = settings.edit();
	    	 //editor.putStringSet("key", set);
	    	 //editor.commit();

	    	//nativelib.closezip();
	    }

}
