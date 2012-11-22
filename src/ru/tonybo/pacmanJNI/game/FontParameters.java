package ru.tonybo.pacmanJNI.game;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.util.Log;

public class FontParameters {
	
	//--Constants--//
	public final static int CHAR_START = 32;           // First Character (ASCII Code)
	public final static int CHAR_END = 126;            // Last Character (ASCII Code)
	public final static int CHAR_CNT = ( ( ( CHAR_END - CHAR_START ) + 1 ) + 1 );  // Character Count (Including Character to use for Unknown)
	
	public final static int CHAR_NONE = 32;            // Character to Use for Unknown (ASCII Code)
	public final static int CHAR_UNKNOWN = ( CHAR_CNT - 1 );  // Index of the Unknown Character
	
	public final static int FONT_SIZE_MIN = 6;         // Minumum Font Size (Pixels)
	public final static int FONT_SIZE_MAX = 180;       // Maximum Font Size (Pixels)

	//--Members--//
	public float fontHeight;                                  	// Font Height (Actual; Pixels)
	public float fontAscent;                                  	// Font Ascent (Above Baseline; Pixels)
	public float fontDescent;                                 	// Font Descent (Below Baseline; Pixels)

	public int textureId;                                     	// Font Texture ID [NOTE: Public for Testing Purposes Only!]
	public int textureSize;                                   	// Texture Size for Font (Square) [NOTE: Public for Testing Purposes Only!]

	public float charWidthMax;                                	// Character Width (Maximum; Pixels)
	public float charHeight;                                  	// Character Height (Maximum; Pixels)
	final float[] charWidths;                         		 	// Width of Each Character (Actual; Pixels)
//	TextureRegion[] charRgn;                        		   	// Region of Each Character (Texture Coordinates)
	public int cellWidth, cellHeight;                         	// Character Cell Width/Height
	int rowCnt, colCnt;                               			// Number of Rows/Columns

	int padX;
	int padY;
	
	Context context;
	   
	public FontParameters (Context context, String fileName, int height, int padX, int padY) {
		Log.d("FontParameters","begin");
		this.context = context;
		
		charWidths = new float[CHAR_CNT];               // Create the Array of Character Widths
//		charRgn = new TextureRegion[CHAR_CNT];          // Create the Array of Character Regions
	
		fontHeight = 0.0f;
		fontAscent = 0.0f;
		fontDescent = 0.0f;
	
		textureId = -1;
		textureSize = 0;
	
		charWidthMax = 0;
		charHeight = 0;
	
		cellWidth = 0;
		cellHeight = 0;
		rowCnt = 0;
		colCnt = 0;
		
		loadFont (fileName, height, padX, padY);
		Log.d("FontParameters","end");
	}
	
	private Boolean loadFont (String fileName, int height, int padX, int padY) {
		this.padX = padX;
		this.padY = padY;
		
		Log.d("FontParameters", "height: " + height);
		
		// load the font and setup paint instance for drawing
		Typeface tf = Typeface.createFromAsset( context.getAssets(), fileName );// Create the Typeface from Font File
		Paint paint = new Paint();// Create Android Paint Instance
		paint.setAntiAlias( true ); // Enable Anti Alias
		paint.setTextSize( height );// Set Text Size
		paint.setColor( 0xffffffff ); // Set ARGB (White, Opaque)
		paint.setTypeface( tf );// Set Typeface
		
		// get font metrics
		Paint.FontMetrics fm = paint.getFontMetrics();// Get Font Metrics
		fontHeight = (float)Math.ceil( Math.abs( fm.bottom ) + Math.abs( fm.top ) );// Calculate Font Height
		fontAscent = (float)Math.ceil( Math.abs( fm.ascent ) );// Save Font Ascent
		fontDescent = (float)Math.ceil( Math.abs( fm.descent ) );// Save Font Descent
		
		// determine the width of each character (including unknown character)
		// also determine the maximum character width
		char[] s = new char[2]; // Create Character Array
		charWidthMax = charHeight = 0;// Reset Character Width/Height Maximums
		float[] w = new float[2]; // Working Width Value
		int cnt = 0;// Array Counter
		for ( char c = CHAR_START; c <= CHAR_END; c++ ){// FOR Each Character
			s[0] = c;// Set Character
			paint.getTextWidths( s, 0, 1, w ); // Get Character Bounds
			charWidths[cnt] = w[0];// Get Width
			if ( charWidths[cnt] > charWidthMax )// IF Width Larger Than Max Width
				charWidthMax = charWidths[cnt]; // Save New Max Width
            cnt++; // Advance Array Counter
        }
        s[0] = CHAR_NONE; // Set Unknown Character
        paint.getTextWidths( s, 0, 1, w );// Get Character Bounds
        charWidths[cnt] = w[0]; // Get Width
        if ( charWidths[cnt] > charWidthMax ) // IF Width Larger Than Max Width
            charWidthMax = charWidths[cnt];// Save New Max Width
    
        // set character height to font height
        charHeight = fontHeight;// Set Character Height
        
        // find the maximum size, validate, and setup cell sizes
        cellWidth = (int)charWidthMax + ( 2 * padX );// Set Cell Width
        cellHeight = (int)charHeight + ( 2 * padY );// Set Cell Height
        int maxSize = cellWidth > cellHeight ? cellWidth : cellHeight;// Save Max Size (Width/Height)
        if ( maxSize < FONT_SIZE_MIN || maxSize > FONT_SIZE_MAX )// IF Maximum Size Outside Valid Bounds
            return false;// Return Error
    
        // set texture size based on max font size (width or height)
        // NOTE: these values are fixed, based on the defined characters. when
        // changing start/end characters (CHAR_START/CHAR_END) this will need adjustment too!
        if ( maxSize <= 24 )// IF Max Size is 18 or Less
            textureSize = 256; // Set 256 Texture Size
        else if ( maxSize <= 40 ) // ELSE IF Max Size is 40 or Less
            textureSize = 512; // Set 512 Texture Size
        else if ( maxSize <= 80 ) // ELSE IF Max Size is 80 or Less
            textureSize = 1024;// Set 1024 Texture Size
        else// ELSE IF Max Size is Larger Than 80 (and Less than FONT_SIZE_MAX)
            textureSize = 2048;// Set 2048 Texture Size
		
		// create an empty bitmap (alpha only)
		Bitmap bitmap = Bitmap.createBitmap( textureSize, textureSize, Bitmap.Config.ALPHA_8 );// Create Bitmap
		Canvas canvas = new Canvas( bitmap ); // Create Canvas for Rendering to Bitmap
		bitmap.eraseColor( 0x00000000 );// Set Transparent Background (ARGB)
		
		// calculate rows/columns
		// NOTE: while not required for anything, these may be useful to have :)
		colCnt = textureSize / cellWidth; // Calculate Number of Columns
		rowCnt = (int)Math.ceil( (float)CHAR_CNT / (float)colCnt );// Calculate Number of Rows
		
		// render each of the characters to the canvas (ie. build the font map)
		float x = padX; // Set Start Position (X)
		float y = ( cellHeight - 1 ) - fontDescent - padY;// Set Start Position (Y)
		for ( char c = CHAR_START; c <= CHAR_END; c++ ){// FOR Each Character
			s[0] = c;// Set Character to Draw
			Log.d("FontParameters ","char \"" + s[0] + "\", x: " + x + ", y: " + y );
			canvas.drawText( s, 0, 1, x, y, paint ); // Draw Character
			x += cellWidth;// Move to Next Character
			if ( ( x + cellWidth - padX ) > textureSize ){// IF End of Line Reached
				x = padX; // Set X for New Row
				y += cellHeight;// Move Down a Row
			}
		}
		s[0] = CHAR_NONE; // Set Character to Use for NONE
		canvas.drawText( s, 0, 1, x, y, paint );// Draw Character
		
		// generate a new texture
		int[] textureIds = new int[1];// Array to Get Texture Id
		GLES20.glGenTextures( 1, textureIds, 0 ); // Generate New Texture
		textureId = textureIds[0];// Save Texture Id
		
		// setup filters for texture
		GLES20.glBindTexture( GLES20.GL_TEXTURE_2D, textureId );// Bind Texture
		GLES20.glTexParameterf( GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST );// Set Minification Filter
		GLES20.glTexParameterf( GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR );// Set Magnification Filter
		GLES20.glTexParameterf( GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE );// Set U Wrapping
		GLES20.glTexParameterf( GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE );// Set V Wrapping
		
		// load the generated bitmap onto the texture
		GLUtils.texImage2D( GLES20.GL_TEXTURE_2D, 0, bitmap, 0 );// Load Bitmap to Texture
		GLES20.glBindTexture( GLES20.GL_TEXTURE_2D, 0 );// Unbind Texture
		
		// release the bitmap
		bitmap.recycle(); // Release the Bitmap
		
		// return success
		return true;// Return Success
	}
}
