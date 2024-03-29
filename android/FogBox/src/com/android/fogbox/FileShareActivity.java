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

import java.io.IOException;
import java.io.InputStream;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnMultiChoiceClickListener;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;



public class FileShareActivity extends Activity {
    
	private final int DIALOG_PROGRESS_ID =1;
	private Button mShareButton;
	private Button mCancelButton;
	private ImageView mImgview;
	private TextView mTextView;
	//private final String serverIP = "192.168.72.167";
	//private final String portNo="4001";
	private final String serverIP = "10.10.26.1";
	private final String portNo="4001";
	private String Filename = null;
    private String TAG ="FileShareActivity";
 
    private static final int SELECT_PICTURE = 1;

   // private String selectedImagePath = "/mnt/sdcard/DCIM/Camera/";
    private String selectedImagePath = "/mnt/sdcard/";


	
    @Override
    public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.image_view);
    
  
    AssetManager assetManager = getBaseContext().getAssets();
    InputStream istr;
    Bitmap bitmap = null;

    mShareButton = (Button) findViewById(R.id.buttonShare);
 	mCancelButton = (Button) findViewById(R.id.buttonCancel);
 	mImgview	=(ImageView) findViewById(R.id.imageView);
 	mTextView =(TextView) findViewById(R.id.filenameTextView);
 	
   	 Bundle bundle = getIntent().getExtras();

   	 final Handler handler=new Handler();
     int groupID = bundle.getInt("groupID");
     Filename = bundle.getString("file_name");
     //Toast.makeText(this,"Group ID is "+groupID, Toast.LENGTH_SHORT).show();
     /*Intent intent = new Intent();
     intent.setType("image/*");
     intent.setAction(Intent.ACTION_GET_CONTENT);
     startActivityForResult(Intent.createChooser(intent,
             "Select Picture"), SELECT_PICTURE);*/
     selectedImagePath += Filename;
     Log.d(TAG,"The image path is "+selectedImagePath);
     
     try {
         istr = assetManager.open(Filename);
         bitmap = BitmapFactory.decodeFile(selectedImagePath);
  
     } catch (IOException e) {
         return ;
     }
     mTextView.setText(Filename);
     mImgview.setImageBitmap(bitmap);
     
     mShareButton.setOnClickListener(new OnClickListener() {
         public void onClick(View v) {
        
        	 AlertDialog.Builder builder = new AlertDialog.Builder(FileShareActivity.this);
        	 builder.setMessage(R.string.ConfirmFileShare)
        	        .setCancelable(false)
        	        .setPositiveButton("Yes", new DialogInterface.OnClickListener() {
        	            public void onClick(DialogInterface dialog, int id) {
        	            	final String [] items = new String[]{"Group 1", "Group 2", "Group 3","Group 4",
        	            			"Group 5","Group 6","Group 7","Group 8","Group 9","Group 10"};
        	            	
        	     
        	            	AlertDialog.Builder builder = new AlertDialog.Builder(FileShareActivity.this);
        	            	builder.setTitle(R.string.SelectGroups);
							builder.setMultiChoiceItems(items, null, new OnMultiChoiceClickListener() {
								public void onClick(DialogInterface dialog,
										int which, boolean ischecked) {
									// TODO Auto-generated method stub
									
								}
        	            	});
							builder.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
		        	            public void onClick(DialogInterface dialog, int id) {
		        	            	showDialog(DIALOG_PROGRESS_ID);
		        	            	Toast.makeText(FileShareActivity.this,"File is being sent", Toast.LENGTH_SHORT).show();
		        	            	new Thread(new Runnable() {
		        	            	    public void run() {
		        	            	    	FileList.getNativeLibInstance().sendFile(serverIP,portNo,Filename);	
		        	            	    }
		        	            	  }).start();
		        	            	
		        	            	handler.postDelayed(new Runnable()
		        	            	{		        	        
										public void run() 
		        	            	    {		       
				        	            	removeDialog(DIALOG_PROGRESS_ID); 
		        	            	    }
		        	            	}, 5000);
		        	            }
		        	        });
        	            	AlertDialog alert = builder.show();
        	            }
        	        })
        	        .setNegativeButton("No", new DialogInterface.OnClickListener() {
        	            public void onClick(DialogInterface dialog, int id) {
        	                 dialog.cancel();
        	            }
        	        });
        	 AlertDialog alert = builder.show();
         }
     });
 	 
   	 mCancelButton.setOnClickListener(new OnClickListener() {
         public void onClick(View v) {
        	 //Toast.makeText(FileShareActivity.this,R.string.Exiting, Toast.LENGTH_SHORT).show();
             finish();
             return;
         }
     });
    }

    protected Dialog onCreateDialog(int id) {
        Dialog dialog;
        switch(id) {
        case DIALOG_PROGRESS_ID:
        	 dialog = ProgressDialog.show(this, "", 
                    "Sharing the file...", true);
            break;
        default:
            dialog = null;
        }
        return dialog;
    }
 
    public void appupdate(int count){
    	Toast.makeText(this,"Count is " +count, Toast.LENGTH_SHORT).show();
    }
    
   /* public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode == RESULT_OK) {
            if (requestCode == SELECT_PICTURE) {
                Uri selectedImageUri = data.getData();
                selectedImagePath = getPath(selectedImageUri);
                Log.d(TAG,"The image path is "+selectedImagePath);
            }
        }
    }
    public String getPath(Uri uri) {
        String[] projection = { MediaStore.Images.Media.DATA };
        Cursor cursor = managedQuery(uri, projection, null, null, null);
        int column_index = cursor
                .getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
        cursor.moveToFirst();
        return cursor.getString(column_index);
    }*/


}