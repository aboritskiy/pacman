package ru.tonybo.pacman.game.model;

import ru.tonybo.pacman.game.MotionDirection;

public class PersonModel {
	public PersonModel (int[][] path, int permissionLevel) {
		this.path = path;
		this.permissionLevel = permissionLevel;
		
	}
	
	public IntPosition getCurrentPosition () {
		return currentPosition;
	}
	
	public IntPosition getNextPosition () {
		return nextPosition;
	}
	
	public void setMotionDirection ( MotionDirection md ) {
    	desiredMotionDirection = md;
    }
	
	public void step (long time) {
		
		
	}
	
	protected final int[][] path;
	protected final int permissionLevel;
	
	protected volatile IntPosition currentPosition;
	protected volatile IntPosition nextPosition;
	protected volatile IntPosition positionDisplacement = MotionDirection.NONE.positionDisplacement();
	
	protected volatile MotionDirection currentMotionDirection = MotionDirection.NONE;
	protected volatile MotionDirection desiredMotionDirection = MotionDirection.NONE;
}
