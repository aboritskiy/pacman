package ru.tonybo.pacman.game.model;

import ru.tonybo.pacman.game.MotionDirection;

public class PacManModel extends PersonModel{

	public PacManModel(int[][] path, int permissionLevel) {
		super(path, permissionLevel);
		
		reset(0L);
	}
	
	public void reset(long time) {
		currentMotionDirection = MotionDirection.NONE;
		desiredMotionDirection = MotionDirection.NONE;
		
		currentPosition = INITIAL_POSITION;
		nextPosition = currentPosition;
		
		moveAnimationPhase = 0.0f;
	}
	
	public void step(long time) {
		//Log.v("PacMan", " dsmd " + desiredMotionDirection);
        time = time % 200L;
        float newPhase = (float)time / 200.0f;
        
        if (newPhase < moveAnimationPhase) {
        	currentPosition = nextPosition;
        	
        	if ((currentPosition.x == 1) && (currentMotionDirection == MotionDirection.LEFT)) {
        		currentPosition = new IntPosition(path[currentPosition.y].length - 2, currentPosition.y);
        	} else if ((currentPosition.x == (path[currentPosition.y].length - 2)) && (currentMotionDirection == MotionDirection.RIGHT)){
        		currentPosition = new IntPosition(1, currentPosition.y);
        	}
        	
        	int desiredX = currentPosition.x + desiredMotionDirection.positionDisplacement().x;
        	int desiredY = currentPosition.y + desiredMotionDirection.positionDisplacement().y;
        	int newX = currentPosition.x + currentMotionDirection.positionDisplacement().x;
        	int newY = currentPosition.y + currentMotionDirection.positionDisplacement().y;
        	if ((path[desiredY][desiredX] & permissionLevel) != 0 ) {
        		nextPosition = new IntPosition(desiredX, desiredY);
        		angle = desiredMotionDirection.angle();
        		currentMotionDirection = desiredMotionDirection;
        	} else if ((path[newY][newX] & permissionLevel) != 0 )  {
        		nextPosition = new IntPosition(newX, newY);
        	} else {
        		currentMotionDirection = MotionDirection.NONE;
        	}
        }
        
        moveAnimationPhase = newPhase;
    }
	
	public IntPosition getCurrentPosition () {
		return currentPosition;
	}
	
	public IntPosition getPositionDisplacement () {
		return currentMotionDirection.positionDisplacement();
	}
	
	public float getMoveAnimationPhase () {
		return moveAnimationPhase;
	}

	public float getAngle () {
		return angle;
	}
   
    private final IntPosition INITIAL_POSITION = new IntPosition (14,23);
    
    private float moveAnimationPhase = 0.0f;
    private float angle = 0.0f;
    
    //private static String TAG = "PacManModel";
}
