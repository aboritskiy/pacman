package ru.tonybo.pacman.game.view.text;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import ru.tonybo.pacman.game.GLESUtils;
import android.content.Context;
import android.opengl.GLES20;
import android.opengl.Matrix;
import android.util.Log;

public class StringDrawer {

	public StringDrawer (Context context) {
		super();
		
		this.context = context;
		
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
        
        colorHandle = GLES20.glGetUniformLocation(mProgram, "vColor");
        GLESUtils.checkGlError("glGetUniformLocation vColor");
        if (colorHandle == -1) {
            throw new RuntimeException("Could not get attrib location for vColor");
        }
	}
	
	public void setColor (float red, float green, float blue, float alpha) {
		color[0] = red;
		color[1] = green;
		color[2] = blue;
		color[3] = alpha;
	}
	
	public void setColorV (float[] color) {
		this.color[0] = color[0];
		this.color[1] = color[1];
		this.color[2] = color[2];
		this.color[3] = color[3];
	}

    public void draw(float[] mProjMatrix, float[] mVMatrix, float x, float y, float width, float height, TextureRegion textureRegion) {
        GLES20.glUseProgram(mProgram);

        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);

        mTriangleVertices.position(TRIANGLE_VERTICES_DATA_POS_OFFSET);
        GLES20.glVertexAttribPointer(maPositionHandle, 3, GLES20.GL_FLOAT, false,TRIANGLE_VERTICES_DATA_STRIDE_BYTES, mTriangleVertices);
        mTriangleVertices.position(TRIANGLE_VERTICES_DATA_UV_OFFSET);
        GLES20.glEnableVertexAttribArray(maPositionHandle);
        GLES20.glVertexAttribPointer(maTextureHandle, 2, GLES20.GL_FLOAT, false,TRIANGLE_VERTICES_DATA_STRIDE_BYTES, mTriangleVertices);
        GLES20.glEnableVertexAttribArray(maTextureHandle);

        Matrix.setIdentityM(mMMatrix, 0);
//        Matrix.translateM(mMMatrix, 0, x, y, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mVMatrix, 0, mMMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);

        GLES20.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, mMVPMatrix, 0);
        GLES20.glUniform4f(colorHandle, color[0], color[1], color[2], color[3]);
       
        float halfWidth = width / 2.0f;                 // Calculate Half Width
        float halfHeight = height / 2.0f;               // Calculate Half Height
        float x1 = -halfWidth;                       // Calculate Left X
        float y1 = -halfHeight;                      // Calculate Bottom Y
        float x2 = halfWidth;                       // Calculate Right X
        float y2 = halfHeight;                      // Calculate Top Y
        
/*        mTriangleVertices.put(0, x1);
        mTriangleVertices.put(1, y1);
        mTriangleVertices.put(3, textureRegion.u1);
        mTriangleVertices.put(4, textureRegion.v2);
        mTriangleVertices.put(5, x2);
        mTriangleVertices.put(6, y1);
        mTriangleVertices.put(8, textureRegion.u2);
        mTriangleVertices.put(9, textureRegion.v2);
        mTriangleVertices.put(10, x2);
        mTriangleVertices.put(11, y2);
        mTriangleVertices.put(13, textureRegion.u2);
        mTriangleVertices.put(14, textureRegion.v1);
        mTriangleVertices.put(15, x1);
        mTriangleVertices.put(16, y2);
        mTriangleVertices.put(18, textureRegion.u1);
        mTriangleVertices.put(19, textureRegion.v1);
        mTriangleVertices.put(20, x1);
        mTriangleVertices.put(21, y1);
        mTriangleVertices.put(23, textureRegion.u1);
        mTriangleVertices.put(24, textureRegion.v2);*/
        
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 3);
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 2, 3);
    }

    private static final int FLOAT_SIZE_BYTES = 4;
    private static final int TRIANGLE_VERTICES_DATA_STRIDE_BYTES = 5 * FLOAT_SIZE_BYTES;
    private static final int TRIANGLE_VERTICES_DATA_POS_OFFSET = 0;
    private static final int TRIANGLE_VERTICES_DATA_UV_OFFSET = 3;

    private final float[] mTriangleVerticesData = {
		    // X, Y, Z, U, V
		    -1.0f, -1.0f, 0, 0.0f, 1.0f,
		    1.0f, -1.0f, 0, 1.0f, 1.0f,
		    1.0f, 1.0f, 0, 1.0f, 0.0f,
		    -1.0f, 1.0f, 0, 0.0f, 0.0f,
		    -1.0f, -1.0f, 0, 0.0f, 1.0f};

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
        "uniform vec4 vColor;\n" +
        "uniform sampler2D sTexture;\n" +
        "void main() {\n" +
        "  gl_FragColor = vColor;\n" +
        "  gl_FragColor.a = gl_FragColor.a * texture2D(sTexture, vTextureCoord).a;\n" +
        "}\n";

    private float[] mMMatrix = new float[16];
    private float[] mMVPMatrix = new float[16];

    private int mProgram;
    private int muMVPMatrixHandle;
    private int maPositionHandle;
    private int maTextureHandle;
    private int colorHandle;
    
    private float[] color = {1.0f, 1.0f, 1.0f, 1.0f};

    private Context context;
}
