package ru.tonybo.pacman.game.view;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import ru.tonybo.pacman.R;
import ru.tonybo.pacman.game.GLESUtils;
import ru.tonybo.pacman.game.model.IntPosition;
import ru.tonybo.pacman.game.model.PacManModel;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.opengl.Matrix;
import android.os.SystemClock;
import android.util.Log;

public class PacManView implements IDrawable{
	public PacManView (Context context, PacManModel pacManModel) {
		super();
		
		this.pacManModel = pacManModel;
		
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
            .openRawResource(R.raw.pacman);
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

        long time = SystemClock.uptimeMillis() % 100L;
        float newPhase = (float)time / 100.0f;
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
        
        float vX = animPhase * BASE_PACMAN_SIZE;
        float vY = cAnimPhase * BASE_PACMAN_SIZE;
        
        float tX = 0.5f + animPhase * 0.5f;
        float tY1 = 0.5f - cAnimPhase * 0.5f;
        float tY2 = 0.5f + cAnimPhase * 0.5f;
        
        mTriangleVertices.put(30, vX);
        mTriangleVertices.put(31, -vY);
        mTriangleVertices.put(33, tX);
        mTriangleVertices.put(34, tY1);
        mTriangleVertices.put(55, vX);
        mTriangleVertices.put(56, vY);
        mTriangleVertices.put(58, tX);
        mTriangleVertices.put(59, tY2);

        float moveAnimationPhase = pacManModel.getMoveAnimationPhase();
        IntPosition currentPathPosition = pacManModel.getCurrentPosition();
        IntPosition positionDisplacement = pacManModel.getPositionDisplacement();
        
        Matrix.setIdentityM(mMMatrix, 0);
        Matrix.translateM(
        		mMMatrix,
        		0,
        		ViewConstants.LEFT_TOP_CORNER.x + ViewConstants.PATH_CELL_SIZE_X * ((float)currentPathPosition.x - 1.0f + (float)positionDisplacement.x * moveAnimationPhase),
        		ViewConstants.LEFT_TOP_CORNER.y - ViewConstants.PATH_CELL_SIZE_Y * ((float)currentPathPosition.y - 1.0f + (float)positionDisplacement.y * moveAnimationPhase),
        		0);
        Matrix.rotateM(mMMatrix, 0, pacManModel.getAngle(), 0.0f, 0.0f, 1.0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mVMatrix, 0, mMMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);
        
        GLES20.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, mMVPMatrix, 0);
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 3);
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 3, 3);
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 6, 3);
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 9, 3);
        GLESUtils.checkGlError("glDrawArrays");
    }

    private static final int FLOAT_SIZE_BYTES = 4;
    private static final int TRIANGLE_VERTICES_DATA_STRIDE_BYTES = 5 * FLOAT_SIZE_BYTES;
    private static final int TRIANGLE_VERTICES_DATA_POS_OFFSET = 0;
    private static final int TRIANGLE_VERTICES_DATA_UV_OFFSET = 3;
    
    private static final float BASE_PACMAN_SIZE = 0.06f;

    private final float[] mTriangleVerticesData = {
	    // X, Y, Z, U, V
	    0.0f, 0.0f, 0, 0.5f, 0.5f,
	    0.0f, BASE_PACMAN_SIZE, 0, 0.5f, 1.0f,
	    -BASE_PACMAN_SIZE, 0.0f, 0, 0.0f, 0.5f,
	    
	    0.0f, -BASE_PACMAN_SIZE, 0, 0.5f, 0.0f,
	    0.0f, 0.0f, 0, 0.5f, 0.5f,
	    -BASE_PACMAN_SIZE, 0.0f, 0, 0.0f, 0.5f,
	    
	    BASE_PACMAN_SIZE, 0.0f, 0, 1.0f, 0.5f,
	    0.0f, 0.0f, 0, 0.5f, 0.5f,
	    0.0f, -BASE_PACMAN_SIZE, 0, 0.5f, 0.0f,
	    
	    0.0f, BASE_PACMAN_SIZE, 0, 0.5f, 1.0f,
	    0.0f, 0.0f, 0, 0.5f, 0.5f,
	    BASE_PACMAN_SIZE, 0.0f, 0, 1.0f, 0.5f
    };

    private FloatBuffer mTriangleVertices;

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

    private float[] mMMatrix = new float[16];
    private float[] mMVPMatrix = new float[16];

    private int mProgram;
    private int mTextureID;
    private int muMVPMatrixHandle;
    private int maPositionHandle;
    private int maTextureHandle;
    
    private PacManModel pacManModel;
    

//    private static String TAG = "PacMan";
    
    float phase = 0.0f;
    boolean forwardAnimation = true;
}
