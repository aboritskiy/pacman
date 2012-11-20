package ru.tonybo.pacman.game.view;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import ru.tonybo.pacman.R;
import ru.tonybo.pacman.game.GLESUtils;
import ru.tonybo.pacman.game.model.IntPosition;
import ru.tonybo.pacman.game.view.IDrawable;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.opengl.Matrix;
import android.os.SystemClock;

public class EnergizerView implements IDrawable{
	public EnergizerView (Context context, int[][] tabletPath, IntPosition modelPosition) {
		super();
		
		this.tabletPath = tabletPath;
		this.modelPosition = modelPosition;
		
		mProgram = GLESUtils.createProgram(mVertexShader, mFragmentShader);
        if (mProgram == 0) {
            return;
        }
        
        mTriangleVertices = ByteBuffer.allocateDirect(mTriangleVerticesData.length
                * FLOAT_SIZE_BYTES).order(ByteOrder.nativeOrder()).asFloatBuffer();
        mTriangleVertices.put(mTriangleVerticesData).position(0);
        
        
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
		
		/*
         * Create our texture. This has to be done each time the
         * surface is created.
         */

        int[] textures = new int[1];
        GLES20.glGenTextures(1, textures, 0);

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
            .openRawResource(R.raw.tablet);
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

    public void draw(float[] mProjMatrix, float[] mVMatrix) {
    	if (tabletPath[modelPosition.y][modelPosition.x] == 0)
    		return;
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
        
        long time = SystemClock.uptimeMillis() % 400L;
        float newPhase = (float)time / 400.0f;
        float animPhase;
        float cAnimPhase;
        if (newPhase < phase) {
        	forwardAnimation = !forwardAnimation;
        }
        if (forwardAnimation) {
    		animPhase = 1.0f - 0.5f * newPhase;
    	} else {
    		animPhase = 0.5f + 0.5f * newPhase;
    	}
        cAnimPhase = 1.0f - animPhase;
        
        phase = newPhase;
    	
        Matrix.setIdentityM(mMMatrix, 0);
        Matrix.translateM(
        		mMMatrix,
        		0,
        		ViewConstants.LEFT_TOP_CORNER.x + (modelPosition.x -1) * ViewConstants.PATH_CELL_SIZE_X,
        		ViewConstants.LEFT_TOP_CORNER.y - (modelPosition.y-1) * ViewConstants.PATH_CELL_SIZE_Y,
        		0);
        float scaleValue = (MAX_SCALE - 1) * cAnimPhase + 1;
        Matrix.scaleM(mMMatrix, 0, scaleValue, scaleValue, 1);
        Matrix.multiplyMM(mMVPMatrix, 0, mVMatrix, 0, mMMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);
        
        GLES20.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, mMVPMatrix, 0);
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 3);
        GLESUtils.checkGlError("glDrawArrays");
    }
    
    private final int[][] tabletPath;
    
    private final String mVertexShader =
            "uniform mat4 uMVPMatrix;\n" +
            "attribute vec4 aPosition;\n" +
            "attribute vec2 aTextureCoord;\n" +
            "varying vec2 vTextureCoord;\n" +
            "void main() {\n" +
            "  gl_Position = uMVPMatrix * aPosition;\n" +
            "  vTextureCoord = aTextureCoord;\n" +
            "}\n";

    private final String mFragmentShader =
        "precision mediump float;\n" +
        "varying vec2 vTextureCoord;\n" +
        "uniform sampler2D sTexture;\n" +
        "void main() {\n" +
        "  gl_FragColor = texture2D(sTexture, vTextureCoord);\n" +
        "}\n";
    
    
    private static final int FLOAT_SIZE_BYTES = 4;
    private static final int TRIANGLE_VERTICES_DATA_STRIDE_BYTES = 5 * FLOAT_SIZE_BYTES;
    private static final int TRIANGLE_VERTICES_DATA_POS_OFFSET = 0;
    private static final int TRIANGLE_VERTICES_DATA_UV_OFFSET = 3;
    
    private static final float BASE_TABLET_SIZE = 0.02f;
    private static final float TRIANGLE_SIDE = (float)(BASE_TABLET_SIZE * (Math.sqrt(2) + 1));
    private static final float TEXTURE_BASE_DISP = (float)((1/Math.sqrt(2)));
    private static final float MAX_SCALE = 2.0f;
    

    private final float[] mTriangleVerticesData = {
	    // X, Y, Z, U, V
	    -BASE_TABLET_SIZE, TRIANGLE_SIDE, 0, 0.0f, -TEXTURE_BASE_DISP,
	    -BASE_TABLET_SIZE, -BASE_TABLET_SIZE, 0, 0.0f, 1.0f,
	    TRIANGLE_SIDE, -BASE_TABLET_SIZE, 0, TEXTURE_BASE_DISP + 1.0f, 1.0f
    };

    private FloatBuffer mTriangleVertices;

    private float[] mMMatrix = new float[16];
    private float[] mMVPMatrix = new float[16];

    private int mProgram;
    private int mTextureID;
    private int muMVPMatrixHandle;
    private int maPositionHandle;
    private int maTextureHandle;
    
	private IntPosition modelPosition;
	
	float phase = 0.0f;
    boolean forwardAnimation = true;
    
    @SuppressWarnings("unused")
	private static String TAG = "TabletsView";
}
