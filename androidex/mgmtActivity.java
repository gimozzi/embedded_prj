package com.example.androidex;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;

public class mgmtActivity extends Activity {

	// main xml Button
	Button mgmt_btn1, mgmt_btn2, mgmt_btn3, mgmt_btn4, mgmt_btn5, mgmt_btn6, mgmt_btn7, mgmt_btn8;
	
	boolean mgmt_flag1;
	int mgmt_value1;
	boolean mgmt_flag2;
	int mgmt_value2;
	// variables
	// static int toilet_status;

	// Thread & Handler
	/*
	 * DataReceiver rcvThread; Handler mHandler = new Handler(){
	 * 
	 * @Override public void handleMessage(Message msg){ //DataReceive.java 의
	 * run에서 set 된 msg.what //msg.what == 1 //거리센서 기준 40 이내면 화장실에 있는것으로 판단
	 * if(msg.what == 0){ toilet_status = 0;
	 * 
	 * 
	 * } else if(msg.what == 1){ if(toilet_status == 0){
	 * 
	 * } } }
	 * 
	 * };
	 */
	// onCreate
	
	JNI_test jni_mgmt = new JNI_test();
	
	LinearLayout linear;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.mgmt);
		linear = (LinearLayout) findViewById(R.id.container);

		// main code start

		// R.java problem: project -> clean , Build automatically
		mgmt_btn1 = (Button) findViewById(R.id.mgmt_btn1);
		mgmt_btn2 = (Button) findViewById(R.id.mgmt_btn2);
		mgmt_btn3 = (Button) findViewById(R.id.mgmt_btn3);
		mgmt_btn4 = (Button) findViewById(R.id.mgmt_btn4);
		mgmt_btn5 = (Button) findViewById(R.id.mgmt_btn5);
		mgmt_btn6 = (Button) findViewById(R.id.mgmt_btn6);
		mgmt_btn7 = (Button) findViewById(R.id.mgmt_btn7);
		mgmt_btn8 = (Button) findViewById(R.id.mgmt_btn8);

		Log.d("MY_TAG", "mgmt activity");

		// receive Thread start
		/*
		 * rcvThread = new DataReceiver(mHandler); rcvThread.setDaemon(true);
		 * rcvThread.start();
		 */

		mgmt_btn1.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// Intent intent_mgmt = new Intent(mgmtActivity.this,
				// MainActivity2.class);
				// startActivity(intent_mgmt);
				Log.d("MY_TAG", "mgmt1Btn");
				//하루 급식량
				jni_mgmt.sendToBoard(1, 0, 1, 0);
			}
		});

		mgmt_btn2.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// Intent intent_mgmt = new Intent(mgmtActivity.this,
				// MainActivity2.class);
				// startActivity(intent_mgmt);
				Log.d("MY_TAG", "mgmt2Btn");
				//누적 급식량
				jni_mgmt.sendToBoard(1, 0, 2, 0);
			}
		});

		mgmt_btn3.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// Intent intent_mgmt = new Intent(mgmtActivity.this,
				// MainActivity2.class);
				// startActivity(intent_mgmt);
				Log.d("MY_TAG", "mgmt3Btn");
				// 먹이 급식 시간 간격 
				jni_mgmt.sendToBoard(1, 0, 3, 0);
			}
		});

		mgmt_btn4.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// Intent intent_mgmt = new Intent(mgmtActivity.this,
				// MainActivity2.class);
				// startActivity(intent_mgmt);
				Log.d("MY_TAG", "mgmt4Btn");
				//하루 화장실 사용 횟수 
				jni_mgmt.sendToBoard(1, 0, 4, 0);
			}
		});

		mgmt_btn5.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// Intent intent_mgmt = new Intent(mgmtActivity.this,
				// MainActivity2.class);
				// startActivity(intent_mgmt);
				Log.d("MY_TAG", "mgmt5Btn");
				//누적 화장실 사용 횟수 
				jni_mgmt.sendToBoard(1, 0, 5, 0);
			}
		});

		mgmt_btn6.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// Intent intent_mgmt = new Intent(mgmtActivity.this,
				// MainActivity2.class);
				// startActivity(intent_mgmt);
				Log.d("MY_TAG", "mgmt6Btn");
				//화장실 사용 시간간격 
				jni_mgmt.sendToBoard(1, 0, 6, 0);
			}
		});

		mgmt_btn7.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// Intent intent_mgmt = new Intent(mgmtActivity.this,
				// MainActivity2.class);
				// startActivity(intent_mgmt);
				Log.d("MY_TAG", "mgmt7Btn");
				EditText mgmt_textinput1;
				mgmt_textinput1 = (EditText) findViewById(R.id.mgmt_text);

				// String mgmt_Edittext;
				String mgmt_text;
				String mgmt_tokens[];
				// mgmt_Edittext = mgmt_text_input.getText().toString();
				mgmt_text = mgmt_textinput1.getText().toString();

				mgmt_flag1 = false;
				mgmt_value1 = -1;
				
				if (mgmt_text.length() != 0) {
					mgmt_tokens = mgmt_text.split(" ");
					if (mgmt_tokens.length == 1) {
						Log.d("MY_TAG", "before_parse");
						try {
							mgmt_value1 = Integer.parseInt(mgmt_tokens[0]);
						} catch (Exception e) {
							mgmt_flag1 = false;
						}
						Log.d("MY_TAG", "integer?");

						if (mgmt_value1 > 0) {
							mgmt_flag1 = true;
						} else {
							mgmt_flag1 = false;
						}
						Log.d("MY_TAG", "integer?" + mgmt_value1);
					} else {
						mgmt_flag1 = false;
					}
				}
				
				//call jni
				if(mgmt_flag1 == true)
					jni_mgmt.sendToBoard(1, mgmt_value1, 7, 0);

			}
		});

		mgmt_btn8.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// Intent intent_mgmt = new Intent(mgmtActivity.this,
				// MainActivity2.class);
				// startActivity(intent_mgmt);
				Log.d("MY_TAG", "mgmt8Btn");
				EditText mgmt_textinput2;
				mgmt_textinput2 = (EditText) findViewById(R.id.mgmt_text);

				// String mgmt_Edittext;
				String mgmt_text2;
				String mgmt_tokens2[];
				// mgmt_Edittext = mgmt_text_input.getText().toString();
				mgmt_text2 = mgmt_textinput2.getText().toString();

				mgmt_flag2 = false;
				mgmt_value2 = -1;
				
				if (mgmt_text2.length() != 0) {
					mgmt_tokens2 = mgmt_text2.split(" ");
					if (mgmt_tokens2.length == 1) {
						Log.d("MY_TAG", "before_parse");
						try {
							mgmt_value2= Integer.parseInt(mgmt_tokens2[0]);
						} catch (Exception e) {
							mgmt_flag2 = false;
						}
						Log.d("MY_TAG", "integer?");

						if (mgmt_value2 > 0) {
							mgmt_flag2 = true;
						} else {
							mgmt_flag2 = false;
						}
						Log.d("MY_TAG", "integer?" + mgmt_value2);
					} else {
						mgmt_flag2 = false;
					}
				}
				
				//jni call
				if(mgmt_flag2 == true)
					jni_mgmt.sendToBoard(1, mgmt_value2, 8, 0);
			}
		});
		
	}

	public void onBackPressed() { // When Back key pressed, parsing stop and go
		// to MainActivity.

		// rcvThread.isRunning = false;
		// rcvThread.interrupt();

		finish();

		super.onBackPressed();
	}

}
