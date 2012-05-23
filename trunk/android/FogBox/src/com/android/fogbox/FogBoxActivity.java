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


import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class FogBoxActivity extends Activity {
    /** Called when the activity is first created. */
	private Button mOkButton;
	private Button mCancelButton;
	private EditText mEditText1;
	private EditText mEditText2;
	public static final String username = "lenss";
	public static final String password = "lenss";
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
     
    }
    public void onResume(){
    	super.onResume();
    	 mOkButton = (Button) findViewById(R.id.buttonOk);
    	 mCancelButton = (Button) findViewById(R.id.buttonCancel);
    	 mEditText1 = (EditText)findViewById(R.id.editText1);
    	 mEditText2 = (EditText)findViewById(R.id.editText2);
    	 mEditText1.setText("");
    	 mEditText2.setText("");
    	 mOkButton.setOnClickListener(new OnClickListener() {
             public void onClick(View v) {
                 // Send a message using content of the edit text widget
            	 //String newString = mEditText1.getText().toString();
            	 //String newString1 = mEditText2.getText().toString();
            	 if((mEditText1.getText().toString().equals(FogBoxActivity.username)) && (mEditText2.getText().toString().equals(FogBoxActivity.password)))
            	 //if(true)
            	 {	 
            		 Intent filelistIntent = new Intent(FogBoxActivity.this, FileList.class);
            		 startActivity(filelistIntent);
            	 }
            	 else
            	 {
            		 Toast.makeText(FogBoxActivity.this,R.string.IncorrectLogin, Toast.LENGTH_SHORT).show();
            		 
            	 }
              
             }
         });
    	 mCancelButton.setOnClickListener(new OnClickListener() {
             public void onClick(View v) {
            	 Toast.makeText(FogBoxActivity.this,R.string.Exiting, Toast.LENGTH_SHORT).show();
                 finish();
                 return;
             }
         });
    }
}