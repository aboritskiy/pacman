package ru.tonybo.pacmanJNI;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.view.MotionEvent;

public class GameView extends GLSurfaceView {
	private NativeRenderer gameRenderer;
	
	private Gesture[] gestures = new Gesture[1];

    public GameView(Context context, Handler gameOverHandler){
        super(context);
        setEGLContextClientVersion(2);
        gameRenderer = new NativeRenderer(context, gameOverHandler);
        setRenderer(gameRenderer);
    }

	public boolean onTouchEvent(MotionEvent e) {
		/**
		 * TODO:
		 * add correct multi-touch processing
		 */
		int actionType = e.getAction();
		int historySize = e.getHistorySize();
		switch (actionType) {
		case MotionEvent.ACTION_DOWN : 
			gestures[0] = new Gesture(e.getX(), e.getY());
			break;
		case MotionEvent.ACTION_MOVE :
			for (int i = 0; i < historySize; i++) {
				gestures[0].setPosition (e.getHistoricalX(i), e.getHistoricalY(i));
			}
			gestures[0].setPosition (e.getX(), e.getY());
			break;
		case MotionEvent.ACTION_UP : 
			break;
		}
		
		gameRenderer.setMotionDirection(gestures[0].getMotionDirection());
		return true; 
	}
}