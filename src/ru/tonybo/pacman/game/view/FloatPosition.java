package ru.tonybo.pacman.game.view;

public class FloatPosition {
	public final float x;
	public final float y;
	
	public FloatPosition ( float x, float y ) {
		this.x = x;
		this.y = y;
	}
	
	public String toString () {
		return "[Position x:" + x + ", y:" + y + "]";
	}
}
