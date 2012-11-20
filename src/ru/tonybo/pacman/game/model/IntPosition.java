package ru.tonybo.pacman.game.model;

import android.util.Log;

public class IntPosition {
	public final int x;
	public final int y;
	
	public IntPosition ( int x, int y ) {
		this.x = x;
		this.y = y;
	}
	
	public boolean equals (IntPosition position) {
		return ((this.x == position.x)&&(this.y == position.y));
	}
	
	public String toString () {
		return "[Position x:" + x + ", y:" + y + "]";
	}
}
