package ru.tonybo.pacmanJNI;

import java.io.IOException;
import java.io.InputStream;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import ru.tonybo.pacmanJNI.game.FontParameters;
import ru.tonybo.pacmanJNI.game.MotionDirection;

import android.content.Context;
import android.content.res.Resources.NotFoundException;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Handler;
import android.util.Log;

public class NativeRenderer implements Renderer {
	static {
		System.loadLibrary("gnustl_shared");
        System.loadLibrary("NativeRenderer");
    }
    
    public NativeRenderer (Context context, Handler gameOverHandler) {
		super();
		
		this.context = context;
		this.gameOverHandler = gameOverHandler;
        gamePtr = create();
	}
    
    public void onSurfaceCreated(GL10 unused, EGLConfig config) {
        onSurfaceCreated(gamePtr);
    }
    
    public void onDrawFrame(GL10 unused) {
        onDrawFrame(gamePtr);
    }
    
    public void onSurfaceChanged(GL10 glUnused, int width, int height) {
        onSurfaceChanged(gamePtr, width, height);
    }
    
    public void setMotionDirection ( MotionDirection md ) {
    	setMotionDirection(gamePtr, md.intValue());
    }
    
    public void loadBitmapResource(int id, int[] pixels) {
        Bitmap bitmap = null;
        InputStream stream = null;
        try {
        	stream = context.getResources().openRawResource(id);
            bitmap = BitmapFactory.decodeStream(stream);
            bitmap.getPixels(pixels, 0, bitmap.getWidth(), 0, 0, bitmap.getHeight(), bitmap.getWidth());
        } catch (NotFoundException e) {
            Log.e("loadRawResource ERROR ", "raw resource not found.");
        } finally {
        	if (bitmap != null) 
        		bitmap.recycle();
        	try {
        		if (stream != null) 
        			stream.close();
			} catch (IOException e1) {
			}
        }
    }
    
    public void createGLTextureFromResource(int resourceId, int textureID) {
    	GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureID);

        InputStream is = context.getResources().openRawResource(resourceId);
        Bitmap bitmap;
        try {
            bitmap = BitmapFactory.decodeStream(is);
        } finally {
            try {
                is.close();
            } catch(IOException e) {
                // Ignore.
            }
        }

        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);
        bitmap.recycle();
    }
    
    public FontParameters loadFont(String fileName, int height, int padX, int padY) {
    	return new FontParameters(context, fileName, height, padX, padY);
    }
    
    public void handleGameOver() {
    	gameOverHandler.obtainMessage().sendToTarget();
    }
    
	@Override
    protected void finalize() throws Throwable {
        if (gamePtr != 0) {
            destroy(gamePtr);
        }
        super.finalize();
    }
	
	private Context context;
	private Handler gameOverHandler;

	private native long create();
    private native void destroy(long gamePtr);    
   
    private native void onSurfaceCreated(long gamePtr);
    private native void onDrawFrame(long gamePtr);
    private native void onSurfaceChanged(long gamePtr, int width, int height);
    private native void setMotionDirection(long gamePtr, int md);

    private long gamePtr = 0;
    
    private long lastTime = 0L;
}