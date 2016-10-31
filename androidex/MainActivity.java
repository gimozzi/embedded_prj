package com.example.androidex;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;



public class MainActivity extends Activity{
	
	// main xml Button
	Button btn_puzzle_start, bluetoothBtn,mgmtBtn,feedBtn,cleanBtn  ;	

	private static final boolean D = true;

	//private static final int REQUEST_CONNECT_DEVICE = 1;
	private static final int REQUEST_ENABLE_BT = 2;
	

	static int toilet_status;
	
	DataReceiver rcvThread;
	
	JNI_test jni_main = new JNI_test();	//jni obj
	
	/*
	 * bluetooth 통신 위하여
	 * 1. 기기의 블루투스 설정
	 * 2. 사용할 장치 정보 등록(페어링)
	 * 3. xml 퍼미션 설정
	 * 4. 애플리케이션 내에서 통신
	 *
	*/
	
	//Data Stream
	private static InputStream mInputStream;
	private static OutputStream mOutputStream;
	private static DataInputStream DatainputStream;
	private static DataOutputStream DataoutputStream;	
	public static DataInputStream getInputdataStream() {
		return DatainputStream;
	}
	public static DataOutputStream getOutputdataStream() {
		return DataoutputStream;
	}
	
	
	//bluetooth	
	private BluetoothAdapter bluetoothAdapter = null;
	public static BluetoothAdapter localAdapter;
	public static BluetoothSocket socket;
	
	private static final UUID MY_UUID = UUID
			.fromString("00001101-0000-1000-8000-00805F9B34FB");

	private static String HC06Address = "20:15:08:10:63:21";
	
	boolean success = false;
	
	private static boolean isConnected = false;
	public static boolean getIsConnected() {
		return isConnected;
	}
	

	

	// Enables Bluetooth if not enabled
	public void enableBT() {
		localAdapter = BluetoothAdapter.getDefaultAdapter();
		// If Bluetooth not enable then do it
		Log.d("MY_TAG", "try to connect BT");
		if (localAdapter.isEnabled() == false) {
			localAdapter.enable();
			while (!(localAdapter.isEnabled())) {
			}
		}
	}
	
	// connect to arduino
	public boolean connectToHC06() {
		// get the BluetoothDevice of the arduino
		BluetoothDevice hc06Device = localAdapter.getRemoteDevice(HC06Address);
		Log.d("MY_TAG", " @ConnectToHC06_start");

		// try to connect to the arduino
		try {
			Log.d("MY_TAG", " @ConnectToHC06_try");
			socket = hc06Device.createRfcommSocketToServiceRecord(UUID
					.fromString("00001101-0000-1000-8000-00805F9B34FB"));

			socket.connect();

			//open stream
			mInputStream = socket.getInputStream();
			mOutputStream = socket.getOutputStream();
			
			DatainputStream = new DataInputStream(mInputStream);
			DataoutputStream = new DataOutputStream(mOutputStream);
			//receive data
			
			success = true;
			Log.d("MY_TAG", "connect to HC06");

		} catch (IOException e) {
			if (D)
				Log.d("MY_TAG", "cannot connect");
			success = false;
		}
		if (D){
			if (success){
				Log.d("MY_TAG", "Connected");
			}
		}
		return success;
	}

	
	Handler mHandler = new Handler(){		
		@Override
		public void handleMessage(Message msg){
			//DataReceive.java 의 run에서 set 된 msg.what
			//msg.what == 1 	//거리센서 기준  40 이내면 화장실에 있는것으로 판단 
			
			if(msg.what == 0){
				Log.d("RCV", "## Not in toilet");
				toilet_status = 0;				
			
			}
			else if(msg.what == 1){
				Log.d("RCV", "@@ in toilet");
				toilet_status = 1;
			}
			
			//Log.d("BT", "main send attempt"	);
			//call jni func(send bluetooth data to ACHRO4210 board)
			//jni_main.sendToBoard(0, 0, 0, toilet_status);
			//Log.d("BT", "main send success"	);
		}	
	};
	
	//onCreate
	LinearLayout linear;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		linear = (LinearLayout)findViewById(R.id.container);
		
		// main code start
		btn_puzzle_start = (Button)findViewById(R.id.id_puzzle_start);
		// R.java problem: project -> clean , Build automatically
		bluetoothBtn = (Button)findViewById(R.id.bluetoothBtn);
		mgmtBtn = (Button)findViewById(R.id.mgmtBtn);
		feedBtn = (Button)findViewById(R.id.feedBtn);
		cleanBtn = (Button)findViewById(R.id.cleanBtn);
		
		
		
		//load library
		System.loadLibrary("jni");
		
		
		// Get local Bluetooth adapter
		bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
		// If the adapter is null, then Bluetooth is not supported
		if (bluetoothAdapter == null) {	
			finish();
			return;
		}
		
		


		
		
		
		Log.d("MY_TAG","program start");
		
		OnClickListener listener = new OnClickListener() {
			@Override
			public void onClick(View v){
				Intent intent = new Intent(MainActivity.this, MainActivity2.class);
				startActivity(intent);
				Log.d("MY_TAG","start activity");
			}
		};
		btn_puzzle_start.setOnClickListener(listener);
		
		
		bluetoothBtn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v){
				//Intent intent = new Intent(MainActivity.this, MainActivity2.class);
				//startActivity(intent);
				
				
				if (D)
					Log.d("MY_TAG","@bluetoothBtn");
				// If BT is not on, request that it be enabled.
				if (!bluetoothAdapter.isEnabled()) {
					Log.d("MY_TAG", " @BT_01");
					Intent enableBTIntent = new Intent(
							BluetoothAdapter.ACTION_REQUEST_ENABLE);
					Log.d("MY_TAG", " @BT_02");

					startActivityForResult(enableBTIntent, REQUEST_ENABLE_BT);
					Log.d("MY_TAG", " @BT_03");

				} else {
					Log.d("MY_TAG", "try to enable BT...");
					try {
						enableBT();
						Log.d("MY_TAG", " @BT_try");

						if (connectToHC06()) {
							isConnected = true;
							
							Log.d("MY_TAG", "bluetooth connected");

						}

					} catch (Exception e) {
						Log.d("MY_TAG", " @BT_exception");

						e.printStackTrace();
					}

				}
				
				Log.d("RCV", "rcvThread start");
				rcvThread = new DataReceiver(mHandler);
				rcvThread.setDaemon(true);
				rcvThread.start();
				Log.d("RCV", "rcvThread start success");
			}
		});
		
		mgmtBtn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v){
				Intent intent_mgmt = new Intent(MainActivity.this, mgmtActivity.class);
				startActivity(intent_mgmt);
				Log.d("MY_TAG","mgmtBtn");
			}
		});
		
		feedBtn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v){
				//Intent intent = new Intent(MainActivity.this, MainActivity2.class);
				//startActivity(intent);
				Log.d("MY_TAG","feedBtn");
				jni_main.sendToBoard(0, 0, 9, 0);
			}
		});
		
		
		cleanBtn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v){
				//Intent intent = new Intent(MainActivity.this, MainActivity2.class);
				//startActivity(intent);
				Log.d("MY_TAG","cleanBtn");
				jni_main.sendToBoard(0, 0, 10, 0);
			}
		});
		
	}	
	
	public void onBackPressed() { // End program.


		rcvThread.isRunning = false;
		rcvThread.interrupt();
		moveTaskToBack(true);

		finish(); // this Activity end.
		android.os.Process.killProcess(android.os.Process.myPid());
	}
	
	protected void onDestroy() {
		try {
			mInputStream.close();
			mOutputStream.close();
			socket.close();
		} catch (Exception e) {
		}
		super.onDestroy();
	}

}


