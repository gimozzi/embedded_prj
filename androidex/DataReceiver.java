package com.example.androidex;

import java.io.IOException;

import android.os.Handler;
import android.os.Message;
import android.util.Log;




public class DataReceiver extends Thread{
	
	//thread control
	boolean isRunning = false;
	
	
	//BT 수신 data 저장할 변수 선언

	Handler mHandler;

	int distance;
	byte input;
	int sdata;
	
	JNI_test jni_rcv = new JNI_test();	//jni obj
	
	public DataReceiver(Handler mHandler){
		this.mHandler = mHandler;
	}
	
	
	public void run(){
		
		//bluetooth data
	/*
		try{
			if(MainActivity.getIsConnected()){
				while(MainActivity.getInputdataStream().available() != 0){
					input = (byte)MainActivity.getInputdataStream().read();
				}
			}
		}
		catch(IOException e){
			e.getMessage();
		}
	 */
		isRunning = true;
		while (isRunning) {
			try{
				if(MainActivity.getIsConnected()){					
					input = (byte)MainActivity.getInputdataStream().read();					
				}
			}
			catch(IOException e){
				e.printStackTrace();
			}
			
			// message set and send
			Message msg = Message.obtain();
			// msg.what //거리센서 기준 40 이내면 화장실에 있는것으로 판단
			Log.d("RCV", "input: " + input);
			if (input < 40) {
				Log.d("RCV", "toilet: occupied");
				msg.what = 1;
				msg.arg1 = (int) input;
				sdata = 1;
			} else {
				Log.d("RCV", "toilet: Not occupied");
				msg.what = 0;
				msg.arg1 = (int) input;
				sdata = 0;
			}

			// msg.arg1
			// msg.arg2
			// msg.obj
			mHandler.sendMessage(msg);
			
			Log.d("BT", "rcv send attempt"	);
			//call jni func(send bluetooth data to ACHRO4210 board)
			try{
					sleep(50);
			}catch(InterruptedException e){
				System.out.println(e.getMessage());
			}
			//Log.d("MY_TAG", "msg.what" + msg.what);
			//jni_rcv.sendToBoard(0, 0, 0, msg.what);
			Log.d("MY_TAG", "sdata " + sdata);
			jni_rcv.sendToBoard(0, 0, 0, sdata);
			Log.d("BT", "rcv send success"	);
			// //////////
		}
	}


}


