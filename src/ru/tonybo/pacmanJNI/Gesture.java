package ru.tonybo.pacmanJNI;

import ru.tonybo.pacmanJNI.game.MotionDirection;

public class Gesture {
	@SuppressWarnings("unused")
	private final float startX;
	@SuppressWarnings("unused")
	private final float startY;
	
	private volatile float currentX;
	private volatile float currentY;
	
	private volatile float baseX;
	private volatile float baseY;
	
	private volatile float deltaX;
	private volatile float deltaY;

	public Gesture(float x, float y) {
		startX = x;
		startY = y;
		
		baseX = x;
		baseY = y;
	}
	
	public void setPosition(float x, float y) {
		float newDeltaX = x - currentX;
		float newDeltaY = y - currentY;
		
		if (((newDeltaX >= 0) && (deltaX < 0)) || ((newDeltaX < 0) && (deltaX >= 0))) {
			baseX = currentX;
		}
		
		if (((newDeltaY >= 0) && (deltaY < 0)) || ((newDeltaY < 0) && (deltaY >= 0))) {
			baseY = currentY;
		}
		
		currentX = x;
		currentY = y;
		
		deltaX = x - baseX;
		deltaY = y - baseY;
	}
	
	public MotionDirection getMotionDirection () {
		if ((deltaX >= 0) && (deltaY >= 0)) {
			if (deltaX >= deltaY) {
				return MotionDirection.RIGHT;
			} else {
				return MotionDirection.DOWN;
			}
		} else if ((deltaX >= 0) && (deltaY < 0)) {
			if (deltaX >= -deltaY) {
				return MotionDirection.RIGHT;
			} else {
				return MotionDirection.UP;
			}
		} else if ((deltaX < 0) && (deltaY >= 0)) {
			if (-deltaX >= deltaY) {
				return MotionDirection.LEFT;
			} else {
				return MotionDirection.DOWN;
			}
		} else if ((deltaX < 0) && (deltaY < 0)) {
			if (deltaX >= deltaY) {
				return MotionDirection.UP;
			} else {
				return MotionDirection.LEFT;
			}
		} else {
			return MotionDirection.NONE;
		}
	}
}
