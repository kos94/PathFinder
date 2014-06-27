package com.example.wizard1;

import java.util.ArrayList;
import com.example.wizard1.components.Vector4d;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

public class AcceleratorThread extends Thread implements SensorEventListener {
	private Looper mLooper;
	private SensorManager mSensorManager;
    private Sensor mAccelerometer;
	private ArrayList <Vector4d> records;
	
	public AcceleratorThread(SensorManager sm, Sensor s) {
		setName("Accelerator thread");
		mSensorManager = sm;
		mAccelerometer = s;
		records = new ArrayList<Vector4d>();
	}
	
	public void run() {
		Looper.prepare();
		Handler handler = new Handler() {
			public void handleMessage(Message msg) {
				
			}
		};
		mLooper = Looper.myLooper();
		mSensorManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_GAME, handler);
		Looper.loop();
	}
	
	public ArrayList<Vector4d> stopAndGetResult() {
		mSensorManager.unregisterListener(this);
		mLooper.quit();
		return records;
	}
	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// TODO Auto-generated method stub
	}

	@Override
	 public void onSensorChanged(SensorEvent event) {
    	long timeStamp = event.timestamp / 1000000;
    	Vector4d rec = new Vector4d(
				event.values[0], event.values[1], event.values[2], timeStamp);
		records.add(rec);
    }
}