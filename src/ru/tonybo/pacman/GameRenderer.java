package ru.tonybo.pacman;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import ru.tonybo.pacman.game.MotionDirection;
import ru.tonybo.pacman.game.model.GameModel;
import ru.tonybo.pacman.game.view.GameView;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.Matrix;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Handler;
import android.os.SystemClock;

public class GameRenderer implements Renderer {

	public GameRenderer (Context context, Handler gameOverHandler) {
		super();
		
		this.context = context;
		this.gameOverHandler = gameOverHandler;
	}

	public void onSurfaceCreated(GL10 unused, EGLConfig config) {
		gameModel = new GameModel(gameOverHandler);
		gameView = new GameView (context, gameModel);
       
        Matrix.setLookAtM(mVMatrix, 0, 0, 0, 2.2f, 0f, 0f, 0f, 0f, 1.0f, 0.0f);
        gameModel.resetGame(SystemClock.uptimeMillis());
    }

    public void onDrawFrame(GL10 unused) {
    	GLES20.glEnable (GLES20.GL_BLEND);
    	GLES20.glBlendFunc (GLES20.GL_SRC_ALPHA, GLES20.GL_ONE_MINUS_SRC_ALPHA);
    	
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        GLES20.glClear( GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);
        
        gameModel.step(SystemClock.uptimeMillis());
        gameView.draw(mProjMatrix, mVMatrix);
    }
    
    public void onSurfaceChanged(GL10 glUnused, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
        float ratio = (float) width / height;
       	if (ratio > 1) {
       		Matrix.frustumM(mProjMatrix, 0, -ratio, ratio, -1.1f, 0.9f, 2, 4);
       	} else {
       		Matrix.frustumM(mProjMatrix, 0, -1, 1, -1.1f/ratio, 0.9f/ratio, 2, 4);
       	}
    }
    
    public void setMotionDirection ( MotionDirection md ) {
    	gameModel.setMotionDirection(md);
    }
    
    private float[] mProjMatrix = new float[16];
    private float[] mVMatrix = new float[16];
    
    private GameModel gameModel;
    private GameView gameView;

    private Context context;
    private Handler gameOverHandler;
    
    @SuppressWarnings("unused")
	private static String TAG = "GameRenderer";
}