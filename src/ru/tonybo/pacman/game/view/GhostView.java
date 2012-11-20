package ru.tonybo.pacman.game.view;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import ru.tonybo.pacman.R;
import ru.tonybo.pacman.game.GLESUtils;
import ru.tonybo.pacman.game.model.GhostModel;
import ru.tonybo.pacman.game.model.IGhostEatenHandler;
import ru.tonybo.pacman.game.model.IntPosition;
import ru.tonybo.pacman.game.model.PacManModel;
import ru.tonybo.pacman.game.view.text.TextString;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.opengl.Matrix;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.SystemClock;
import android.util.Log;

public class GhostView implements IDrawable, IGhostEatenHandler{
	public GhostView (Context context, GhostModel ghostModel, float[] ghostColor) {
		super();
		
		this.ghostModel = ghostModel;
		this.GHOST_COLOR = ghostColor;
		
		ghostEatenTextString = new TextString(context);
		ghostEatenTextString.loadFont("typodermic_foo_regular.ttf", 50, 2, 2);
		ghostEatenTextString.setScale(0.0015f);
		ghostEatenTextString.setColorV(GHOST_EATEN_COLOR);
		
		this.ghostModel.setGhostEatenHandler(this);
		
		mTriangleVertices = ByteBuffer.allocateDirect(mTriangleVerticesData.length
                * FLOAT_SIZE_BYTES).order(ByteOrder.nativeOrder()).asFloatBuffer();
        mTriangleVertices.put(mTriangleVerticesData).position(0);
        
        // Ignore the passed-in GL10 interface, and use the GLES20
        // class's static methods instead.
        mProgram = GLESUtils.createProgram(mVertexShader, mFragmentShader);
        if (mProgram == 0) {
            return;
        }
        maPositionHandle = GLES20.glGetAttribLocation(mProgram, "aPosition");
        GLESUtils. checkGlError("glGetAttribLocation aPosition");
        if (maPositionHandle == -1) {
            throw new RuntimeException("Could not get attrib location for aPosition");
        }
        
        maTextureHandle = GLES20.glGetAttribLocation(mProgram, "aTextureCoord");
        GLESUtils.checkGlError("glGetAttribLocation aTextureCoord");
        if (maTextureHandle == -1) {
            throw new RuntimeException("Could not get attrib location for aTextureCoord");
        }
        
        muMVPMatrixHandle = GLES20.glGetUniformLocation(mProgram, "uMVPMatrix");
        GLESUtils.checkGlError("glGetUniformLocation uMVPMatrix");
        if (muMVPMatrixHandle == -1) {
            throw new RuntimeException("Could not get attrib location for uMVPMatrix");
        }
        
        basicColorHandle = GLES20.glGetUniformLocation(mProgram, "basicColor");
        GLESUtils.checkGlError("glGetUniformLocation basicColor");
        if (basicColorHandle == -1) {
            throw new RuntimeException("Could not get attrib location for basicColor");
        }
        
        targetColorHandle = GLES20.glGetUniformLocation(mProgram, "targetColor");
        GLESUtils.checkGlError("glGetUniformLocation targetColor");
        if (targetColorHandle == -1) {
            throw new RuntimeException("Could not get attrib location for targetColor");
        }

        /*
         * Create our texture. This has to be done each time the
         * surface is created.
         */

        int[] textures = new int[2];
        GLES20.glGenTextures(2, textures, 0);

        mTextureID = textures[0];
        
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureID);

        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER,
                GLES20.GL_NEAREST);
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
                GLES20.GL_TEXTURE_MAG_FILTER,
                GLES20.GL_LINEAR);

        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S,
                GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T,
                GLES20.GL_CLAMP_TO_EDGE);

        InputStream is = context.getResources()
            .openRawResource(R.raw.ghost);
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
        
        eyeTextureID = textures[1];
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, eyeTextureID);

        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER,
                GLES20.GL_NEAREST);
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
                GLES20.GL_TEXTURE_MAG_FILTER,
                GLES20.GL_LINEAR);

        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S,
                GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T,
                GLES20.GL_CLAMP_TO_EDGE);

        is = context.getResources().openRawResource(R.raw.ghost_eye);
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
	
	public void handleGhostEaten (IntPosition ghostPosition, int score) {
		ghostEatenTime = SystemClock.uptimeMillis();
		ghostEatenScore = score;
		ghostEatenPosition = ghostPosition;
	}
	

    public void draw(float[] mProjMatrix, float[] mVMatrix) {
        GLES20.glUseProgram(mProgram);
        GLESUtils.checkGlError("glUseProgram");
        
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureID);

        mTriangleVertices.position(TRIANGLE_VERTICES_DATA_POS_OFFSET);
        GLES20.glVertexAttribPointer(maPositionHandle, 3, GLES20.GL_FLOAT, false,
                TRIANGLE_VERTICES_DATA_STRIDE_BYTES, mTriangleVertices);
        GLESUtils.checkGlError("glVertexAttribPointer maPosition");
        
        mTriangleVertices.position(TRIANGLE_VERTICES_DATA_UV_OFFSET);
        GLES20.glEnableVertexAttribArray(maPositionHandle);
        GLESUtils.checkGlError("glEnableVertexAttribArray maPositionHandle");
        
        GLES20.glVertexAttribPointer(maTextureHandle, 2, GLES20.GL_FLOAT, false,
                TRIANGLE_VERTICES_DATA_STRIDE_BYTES, mTriangleVertices);
        GLESUtils.checkGlError("glVertexAttribPointer maTextureHandle");
        
        GLES20.glEnableVertexAttribArray(maTextureHandle);
        GLESUtils.checkGlError("glEnableVertexAttribArray maTextureHandle");
        
        long time2 = SystemClock.uptimeMillis() % 640L;
        long time = time2 % 160L;
        
        int modeOffset = 0;
        int frameOffset = 0;
        boolean drawEyes = true;
        boolean drawBody = true;
        if (ghostModel.isFrightened()) {
        	long timeLeft = ghostModel.timeInFrightenStateLeft(SystemClock.uptimeMillis());
        	if ((timeLeft > 0) && (timeLeft < FRIGHTEN_BLINK_DURATION) && (time2 < 320)) {
        		modeOffset = 12;
        	} else {
        		modeOffset = 6;
        	}
        	drawEyes = false;
        } else if (ghostModel.isReturning()) {
        	drawBody = false;
        }
        
        if (time > 80) {
        	frameOffset = 3;
        }
        
        float moveAnimationPhase = ghostModel.getMoveAnimationPhase();
        IntPosition currentPathPosition = ghostModel.getCurrentPosition();
        IntPosition positionDisplacement = ghostModel.getPositionDisplacement();
        FloatPosition homeDisp = ghostModel.getHomeDisp();
        
        float positionX = ViewConstants.LEFT_TOP_CORNER.x + ViewConstants.PATH_CELL_SIZE_X * ((float)currentPathPosition.x - 1.0f + homeDisp.x + (float)positionDisplacement.x * moveAnimationPhase);
        float positionY = ViewConstants.LEFT_TOP_CORNER.y - ViewConstants.PATH_CELL_SIZE_Y * ((float)currentPathPosition.y - 1.0f + homeDisp.y + (float)positionDisplacement.y * moveAnimationPhase);
        if (drawBody) {
	        Matrix.setIdentityM(mMMatrix, 0);
	        Matrix.translateM(
	        		mMMatrix,
	        		0,
	        		positionX,
	        		positionY,
	        		0);
	        Matrix.multiplyMM(mMVPMatrix, 0, mVMatrix, 0, mMMatrix, 0);
	        Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);
	        
	        GLES20.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, mMVPMatrix, 0);
	        GLES20.glUniform4f(basicColorHandle, 1.0f, 0.0f, 0.0f, 1.0f);
	        GLES20.glUniform4fv(targetColorHandle, 1, GHOST_COLOR, 0);
	        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, modeOffset + frameOffset, 3);
	        GLESUtils.checkGlError("glDrawArrays");
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        if (drawEyes) {
	        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, eyeTextureID);
	        
	        Matrix.setIdentityM(mMMatrix, 0);
	        Matrix.translateM(
	        		mMMatrix,
	        		0,
	        		positionX + EYE_DISP_X,
	        		positionY + EYE_DISP_Y,
	        		0);
	        Matrix.rotateM(mMMatrix, 0, ghostModel.getAngle(), 0.0f, 0.0f, 1.0f);
	        Matrix.multiplyMM(mMVPMatrix, 0, mVMatrix, 0, mMMatrix, 0);
	        Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);
	        
	        GLES20.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, mMVPMatrix, 0);
	        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 18, 3);
	        GLESUtils.checkGlError("glDrawArrays");
	        /////////////////////////////////////////////////////////////////////////////////////////////////////
	        Matrix.setIdentityM(mMMatrix, 0);
	        Matrix.translateM(
	        		mMMatrix,
	        		0,
	        		positionX - EYE_DISP_X,
	        		positionY + EYE_DISP_Y,
	        		0);
	        Matrix.rotateM(mMMatrix, 0, ghostModel.getAngle(), 0.0f, 0.0f, 1.0f);
	        Matrix.multiplyMM(mMVPMatrix, 0, mVMatrix, 0, mMMatrix, 0);
	        Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);
	        
	        GLES20.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, mMVPMatrix, 0);
	        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 18, 3);
	        GLESUtils.checkGlError("glDrawArrays");
        }
        long delta = SystemClock.uptimeMillis() - ghostEatenTime;
        if (delta < GHOST_EATEN_DISPLAY_TIME) {
        	ghostEatenTextString.draw(
    			"" + ghostEatenScore,
    			ViewConstants.LEFT_TOP_CORNER.x + ViewConstants.PATH_CELL_SIZE_X * ((float)ghostEatenPosition.x - 1.5f),
    			ViewConstants.LEFT_TOP_CORNER.y - ViewConstants.PATH_CELL_SIZE_Y * ((float)ghostEatenPosition.y - 0.5f),
    			mProjMatrix,
    			mVMatrix
        	);
        }
    }

    protected static final int FLOAT_SIZE_BYTES = 4;
    protected static final int TRIANGLE_VERTICES_DATA_STRIDE_BYTES = 5 * FLOAT_SIZE_BYTES;
    protected static final int TRIANGLE_VERTICES_DATA_POS_OFFSET = 0;
    protected static final int TRIANGLE_VERTICES_DATA_UV_OFFSET = 3;
    
    protected static final float BASE_SIZE = 0.06f;
    
    protected static final float BASE_EYE_SIZE = 0.025f;
    protected static final float BASE_EYE_DISP_X = 0.01f;
    
    protected static final float EYE_DISP_X = 0.023f;
    protected static final float EYE_DISP_Y = 0.02f;
    
    protected static final float FRAME_STEP = 1.0f/6.0f;
    
    protected final float[] mTriangleVerticesData = {
	    // X, Y, Z, U, V
    		
    	// normal, frame0
	    -BASE_SIZE, 3 * BASE_SIZE, 0, 0.0f, -1.0f,
	    -BASE_SIZE, -BASE_SIZE, 0, 0.0f, 1.0f,
	    3 * BASE_SIZE, -BASE_SIZE, 0, FRAME_STEP, 1.0f,
	    
	    // normal, frame1
	    -BASE_SIZE, 3 * BASE_SIZE, 0, FRAME_STEP, -1.0f,
	    -BASE_SIZE, -BASE_SIZE, 0, FRAME_STEP, 1.0f,
	    3 * BASE_SIZE, -BASE_SIZE, 0, 2*FRAME_STEP, 1.0f,
	    
		 // frightened, frame0
		-BASE_SIZE, 3 * BASE_SIZE, 0, 2*FRAME_STEP, -1.0f,
	    -BASE_SIZE, -BASE_SIZE, 0, 2*FRAME_STEP, 1.0f,
	    3 * BASE_SIZE, -BASE_SIZE, 0, 3*FRAME_STEP, 1.0f,
	    
	    // frightened, frame1
	    -BASE_SIZE, 3 * BASE_SIZE, 0, 3*FRAME_STEP, -1.0f,
	    -BASE_SIZE, -BASE_SIZE, 0, 3*FRAME_STEP, 1.0f,
	    3 * BASE_SIZE, -BASE_SIZE, 0, 4*FRAME_STEP, 1.0f,
	    
	    // frightened-blink, frame0
		-BASE_SIZE, 3 * BASE_SIZE, 0, 4*FRAME_STEP, -1.0f,
	    -BASE_SIZE, -BASE_SIZE, 0, 4*FRAME_STEP, 1.0f,
	    3 * BASE_SIZE, -BASE_SIZE, 0, 5*FRAME_STEP, 1.0f,
	    
	    // frightened-blink, frame1
	    -BASE_SIZE, 3 * BASE_SIZE, 0, 5*FRAME_STEP, -1.0f,
	    -BASE_SIZE, -BASE_SIZE, 0, 5*FRAME_STEP, 1.0f,
	    3 * BASE_SIZE, -BASE_SIZE, 0, 1.0f, 1.0f,
	    
	    // eye
	    BASE_EYE_DISP_X - BASE_EYE_SIZE, 3 * BASE_EYE_SIZE, 0, 0.0f, -1.0f,
	    BASE_EYE_DISP_X - BASE_EYE_SIZE, -BASE_EYE_SIZE, 0, 0.0f, 1.0f,
	    BASE_EYE_DISP_X + 3 * BASE_EYE_SIZE, -BASE_EYE_SIZE, 0, 2.0f, 1.0f,
    };

    protected FloatBuffer mTriangleVertices;

    protected final String mVertexShader =
        "uniform mat4 uMVPMatrix;\n" +
        "attribute vec4 aPosition;\n" +
        "attribute vec2 aTextureCoord;\n" +
        "varying vec2 vTextureCoord;\n" +
        "void main() {\n" +
        "  gl_Position = uMVPMatrix * aPosition;\n" +
        "  vTextureCoord = aTextureCoord;\n" +
        "}\n";

    protected final String mFragmentShader =
        "precision mediump float;\n" +
        "varying vec2 vTextureCoord;\n" +
        "uniform sampler2D sTexture;\n" +
        "uniform vec4 basicColor;\n" +
        "uniform vec4 targetColor;\n" +
        "void main() {\n" +
        "  vec4 col = texture2D(sTexture, vTextureCoord);\n" +
        "  if (col == basicColor)\n" +
        "    col = targetColor;\n" +
        "  gl_FragColor = col;\n" +
        "}\n";

    protected float[] mMMatrix = new float[16];
    protected float[] mMVPMatrix = new float[16];

    protected int mProgram;
    protected int mTextureID;
    protected int eyeTextureID;
    protected int muMVPMatrixHandle;
    protected int maPositionHandle;
    protected int maTextureHandle;
    protected int basicColorHandle;
    protected int targetColorHandle;
    
    protected GhostModel ghostModel;
    
    protected final float[] GHOST_COLOR;
    

    protected String TAG = "GhostView";
    
    float phase = 0.0f;
    boolean forwardAnimation = true;
    
    protected static final int GHOST_EATEN_DISPLAY_TIME = 5000;
    private final float[] GHOST_EATEN_COLOR = { 1.0f, 1.0f, 0.0f, 1.0f };
    
    private Handler ghostEatenHandler;
    private IntPosition ghostEatenPosition;
    private int ghostEatenScore;
    private long ghostEatenTime;
    
    private TextString ghostEatenTextString;
    
    private final long FRIGHTEN_BLINK_DURATION = 3000L;
}
