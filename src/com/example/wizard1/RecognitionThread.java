package com.example.wizard1;

import java.util.ArrayList;

import com.example.wizard1.WizardFight.AppMessage;
import com.example.wizard1.components.*;
import android.hardware.SensorManager;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;

enum Shape { 
	CIRCLE("circle"), 
	TRIANGLE("triangle"), 
	SQUARE("square"), 
	CLOCK("clock"),
	FAIL("fail");
	
	private final String name;
	
	private Shape(String s) {
		name = s;
	}
	
	@Override
	public String toString() {
		return name;
	}
}

public class RecognitionThread extends Thread {
	private Handler mHandler;
	private ArrayList<Vector4d> records;
	private Shape shape;
	
	public RecognitionThread(Handler mainHandler, ArrayList<Vector4d> recs) {
		setName("Recognition thread");
		mHandler = mainHandler;
		records = recs;
	}
	
	public void run() {
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++ SOME RECOGNITION
		/*try {
            Thread.sleep(3000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }*/
		try {
			ArrayList<Vector2d> projection = PathCalculator.calculateTrajectory(records);
			Shape shape = Recognition.recognize(projection,mHandler);
			mHandler.obtainMessage(AppMessage.MESSAGE_OWN_SPELL.ordinal(), 0, 0, shape)
        	.sendToTarget();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
