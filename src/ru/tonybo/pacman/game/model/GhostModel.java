package ru.tonybo.pacman.game.model;

import java.util.HashMap;

import android.os.Handler;
import android.os.SystemClock;
import android.util.Log;

import ru.tonybo.pacman.game.MotionDirection;
import ru.tonybo.pacman.game.TileType;
import ru.tonybo.pacman.game.view.FloatPosition;

public class GhostModel extends PersonModel{

	public GhostModel(
			int[][] path,
			int permissionLevel,
			int defaultHomePermission,
			PacManModel pacManModel,
			IntPosition initialPosition,
			IntPosition scatterPosition,
			int tabletsToLeftHome
		) {
		super(path, permissionLevel);
		
		this.defaultHomePermission = defaultHomePermission;
		this.homePermission = defaultHomePermission;
		this.pacManModel = pacManModel;
		
		INITIAL_POSITION = initialPosition;
		SCATTER_POSITION = scatterPosition;
		LEAVE_HOME_POSITION = new IntPosition(14,11);
		GET_BODY_POSITION = new IntPosition(14,14);
		
		this.tabletsToLeftHome = tabletsToLeftHome;
		reset(0L);
	}
	
	public void reset (long time) {
		currentPosition = INITIAL_POSITION;
		nextPosition = currentPosition;
		isTrapped = (path[INITIAL_POSITION.y][INITIAL_POSITION.x] & TileType.GHOST_TRAP) == TileType.GHOST_TRAP;
		atHome = (path[INITIAL_POSITION.y][INITIAL_POSITION.x] & TileType.GHOST_HOME) == TileType.GHOST_HOME;
		if (atHome)
			homeOffset = HOME_OFFSET;
		else
			homeOffset = new FloatPosition(HOME_OFFSET.x, 0.0f);
		isChasing = false;
	    isFrightened = false;
	    isReturning = false;
	    
		lastChaseModeChangeAt = time;
		lastFrightenModeChangeAt = time;
		
		currentMotionDirection = MotionDirection.NONE;
		desiredMotionDirection = MotionDirection.NONE;
		
		lastTick = time;
		moveAnimationPhase = 0.0f;
		lastTabletEatenAt = time;
	}
	
	protected void forceDirectionChange () {
		if (isFrightened || isReturning)
			return;
		needChangeDirection = true;
	}
	
	protected void processTime (long time) {
		long currentWaveDuration = time - lastChaseModeChangeAt;
		if ((waveLengthMap[currentLevel][currentModeWave] > 0) && (currentWaveDuration > waveLengthMap[currentLevel][currentModeWave])) {
			lastChaseModeChangeAt = time;
			isChasing = !isChasing;
			currentModeWave++;
			forceDirectionChange();
		}
		/**
		 * TODO: add frighten length change with level
		 */
		if (isFrightened) {
			long currentFrightDuration = time - lastFrightenModeChangeAt;
			if (currentFrightDuration > FRIGHT_DURATION) {
				lastFrightenModeChangeAt = time;
				isFrightened = false;
				forceDirectionChange();
			}
		}
		
		if (isTrapped && ((time - tabletsTimeToLeftHome) > lastTabletEatenAt)) {
			isTrapped = false;
		}
	}
	
	public FloatPosition getHomeDisp () {
		return homeOffset;
	}
	
	public Boolean isChasing () {
		return isChasing;
	}
	
	public Boolean isFrightened () {
		return isFrightened;
	}
	
	public Boolean isReturning () {
		return isReturning;
	}
	
	public long timeInFrightenStateLeft (long time) {
		return (FRIGHT_DURATION - time + lastFrightenModeChangeAt);
	}
	
	public void setGhostEatenHandler(IGhostEatenHandler value) {
		synchronized (ghostEatenHandlerMonitor) {
			ghostEatenHandler = value;
		}
	}
	
	protected IntPosition calculateTargetPosition () {
		IntPosition pacManPosition = pacManModel.getCurrentPosition();
		IntPosition pacManPositionDisplacement = pacManModel.getPositionDisplacement();
		return new IntPosition (pacManPosition.x + pacManPositionDisplacement.y, pacManPosition.y + pacManPositionDisplacement.y);
	}
	
	protected int estimateDistanceToTargetPoint (IntPosition disp) {
		int dX = targetPosition.x - currentPosition.x - disp.x;
		int dY = targetPosition.y - currentPosition.y - disp.y;
		
		return (dX * dX + dY * dY); 
	}
	
	public void frighten () {
		if (isReturning)
			return;
		isFrightened = true;
		/**
		 * TODO: very very bad! Ghost should't get time by himself
		 */
		lastFrightenModeChangeAt = SystemClock.uptimeMillis();
	}
	
	public void eaten (int score) {
		isFrightened = false;
		isReturning = true;
		homePermission = homePermission | TileType.GHOST_HOME;
		
		synchronized (ghostEatenHandlerMonitor) {
			if (ghostEatenHandler != null)
				ghostEatenHandler.handleGhostEaten(currentPosition, score);
		}
	}
	
	public void step(long time) {
		processTime (time);
		
        long delta = time - lastTick;
        moveAnimationPhase = (float)delta / (float)animationDuration;
//        Log.d("outX", "3333 " + lastTick + " " + time + " " + delta + " " + moveAnimationPhase);
        if (delta > animationDuration) {
        	moveAnimationPhase = 0.0f;
        	lastTick = time;
        	animationDuration = DEFAULT_ANIMATION_DURATION;
        	currentPosition = nextPosition;
        	
        	if (isReturning) {
        		if (currentPosition.equals(GET_BODY_POSITION)) {
        			isReturning = false;
        		}
        		homePermission = homePermission | TileType.GHOST_HOME;
        		atHome = (path[currentPosition.y][currentPosition.x] & homePermission) != 0;
        	} else {
        		homePermission = (path[currentPosition.y][currentPosition.x] & homePermission);
            	atHome = (homePermission & TileType.GHOST_HOME) == TileType.GHOST_HOME;
        	}
        	
        	if ((currentPosition.x == 1) && (currentMotionDirection == MotionDirection.LEFT)) {
        		currentPosition = new IntPosition(path[currentPosition.y].length - 2, currentPosition.y);
        	} else if ((currentPosition.x == (path[currentPosition.y].length - 2)) && (currentMotionDirection == MotionDirection.RIGHT)){
        		currentPosition = new IntPosition(1, currentPosition.y);
        	}
        	
        	availableExits.clear();
        	/**
        	 * TODO: fix forced turn around
        	 */
        	if (false/*needChangeDirection*/) {
        		needChangeDirection = false;
        		currentMotionDirection = currentMotionDirection.returnDirection();
        	} else {
        		currentMotionDirection = MotionDirection.NONE;
	        	for (int i = 0; i < possibleMotionDirections.length; i++) {
	        		int currentX = currentPosition.x + possibleMotionDirections[i].positionDisplacement().x;
	        		int currentY = currentPosition.y + possibleMotionDirections[i].positionDisplacement().y;
	        		int currentPermission = path[currentY][currentX];
	        		if (
	        				(atHome || (possibleMotionDirections[i] != motionDirectionOfReturn))
	        				&& (
	        					(isTrapped && ((currentPermission & TileType.GHOST_TRAP) != 0))
	        					|| (!isTrapped && ((currentPermission & permissionLevel) != 0))
	        					|| (!isTrapped && ((currentPermission & homePermission) != 0))
	        				)
	        			) {
	        			availableExits.put(possibleMotionDirections[i], 0.0f);
	        		}
	        	}
        	
	        	if (availableExits.size() == 0) {
        			currentMotionDirection = motionDirectionOfReturn;
	        	} else if (availableExits.size() == 1) {
	        		for (MotionDirection md : availableExits.keySet()) {
	        			currentMotionDirection = md;
	        		}
	        	} else {
	        		int minDistance = 10000;
	        		if (isReturning) {
	        			targetPosition = GET_BODY_POSITION;
        				for (MotionDirection md : availableExits.keySet()) {
    	        			int newDistance = estimateDistanceToTargetPoint(md.positionDisplacement());
    	        		    if (minDistance > newDistance) {
    	        		    	minDistance = newDistance;
    	        		    	currentMotionDirection = md;
    	        		    }
    	        		}
	        		} else if (atHome) {
	        			targetPosition = LEAVE_HOME_POSITION;
		        		for (MotionDirection md : availableExits.keySet()) {
		        			int newDistance = estimateDistanceToTargetPoint(md.positionDisplacement());
		        		    if (minDistance > newDistance) {
		        		    	minDistance = newDistance;
		        		    	currentMotionDirection = md;
		        		    }
		        		}
	        		} else if (isFrightened) {
	        			availableExits.size();
        				long exitNumber = Math.round(Math.random() * availableExits.size());
    	        		for (MotionDirection md : availableExits.keySet()) {
    	        		    if (--exitNumber <= 0) {
    	        		    	currentMotionDirection = md;
    	        		    }
    	        		}
	        		} else if (isChasing) {
	        			targetPosition = calculateTargetPosition();
        				for (MotionDirection md : availableExits.keySet()) {
    	        			int newDistance = estimateDistanceToTargetPoint(md.positionDisplacement());
    	        		    if (minDistance > newDistance) {
    	        		    	minDistance = newDistance;
    	        		    	currentMotionDirection = md;
    	        		    }
    	        		}
	        		} else {
	        			targetPosition = SCATTER_POSITION;
    	        		for (MotionDirection md : availableExits.keySet()) {
    	        			int newDistance = estimateDistanceToTargetPoint(md.positionDisplacement());
    	        		    if (minDistance > newDistance) {
    	        		    	minDistance = newDistance;
    	        		    	currentMotionDirection = md;
    	        		    }
    	        		}
	        		}
	        	}
        	}
        	
        	motionDirectionOfReturn = currentMotionDirection.returnDirection();
        	
        	nextPosition = new IntPosition(currentPosition.x + currentMotionDirection.positionDisplacement().x, currentPosition.y + currentMotionDirection.positionDisplacement().y);
        	if ((homeOffset.x != 0.0f) && ((path[nextPosition.y][nextPosition.x] & TileType.GHOST_HOME) == 0)) {
        		if (currentMotionDirection.positionDisplacement().x > 0) {
	        		animationDuration = animationDuration * (1 + homeOffset.x);
	        		lastTick = lastTick - (long)(animationDuration * homeOffset.x);
	        		moveAnimationPhase = homeOffset.x;
	        		homeOffset = new FloatPosition(0.0f, homeOffset.y);
        		} else if (currentMotionDirection.positionDisplacement().x < 0) {
        			animationDuration = animationDuration * (1 + homeOffset.x);
	        		lastTick = lastTick + (long)(animationDuration * homeOffset.x);
	        		moveAnimationPhase = -homeOffset.x;
	        		homeOffset = new FloatPosition(0.0f, homeOffset.y);
        		}
        		
        	} else if ((homeOffset.x == 0.0f) && (currentMotionDirection.positionDisplacement().x != 0) && ((path[nextPosition.y][nextPosition.x] & TileType.GHOST_HOME) == TileType.GHOST_HOME)) {
        		if (currentMotionDirection.positionDisplacement().x > 0) {
//	        		Log.d("outX", "11 " + animationDuration + " " + lastTick + " " + moveAnimationPhase + " " + homeOffset.x);
	        		homeOffset = new FloatPosition(HOME_OFFSET.x, homeOffset.y);
	        		animationDuration = animationDuration * (1 + homeOffset.x);
	        		lastTick =  lastTick - (long)(animationDuration * homeOffset.x);
	        		moveAnimationPhase = homeOffset.x;
//	        		Log.d("outX", "12 " + animationDuration + " " + lastTick + " " + moveAnimationPhase + " " + homeOffset.x);
        		} else if (currentMotionDirection.positionDisplacement().x < 0) {
//        			Log.d("outX", "21 " + animationDuration + " " + lastTick + " " + moveAnimationPhase + " " + homeOffset.x);
	        		homeOffset = new FloatPosition(HOME_OFFSET.x, homeOffset.y);
	        		animationDuration = animationDuration * (1 + homeOffset.x);
	        		lastTick =  lastTick - (long)(animationDuration * homeOffset.x);
	        		moveAnimationPhase = homeOffset.x;
//	        		Log.d("outX", "22 " + animationDuration + " " + lastTick + " " + moveAnimationPhase + " " + homeOffset.x);
        		}
        	}
        	
        	if ((homeOffset.y != 0.0f) && (currentMotionDirection.positionDisplacement().y != 0) && ((path[nextPosition.y][nextPosition.x] & TileType.GHOST_HOME) == 0)) {
        		if (currentMotionDirection.positionDisplacement().y < 0) {
	        		animationDuration = animationDuration * (1 + homeOffset.y);
	        		lastTick =  lastTick + (long)(animationDuration * homeOffset.y);
	        		moveAnimationPhase = -homeOffset.y;
	        		homeOffset = new FloatPosition(homeOffset.x, 0.0f);
        		} else if (currentMotionDirection.positionDisplacement().y > 0) {
        			animationDuration = animationDuration * (1 - homeOffset.y);
	        		lastTick =  lastTick - (long)(animationDuration * homeOffset.y);
	        		moveAnimationPhase = homeOffset.y;
	        		homeOffset = new FloatPosition(homeOffset.x, 0.0f);
        		}
        	} else if ((homeOffset.y == 0.0f) && (currentMotionDirection.positionDisplacement().y != 0) && ((path[nextPosition.y][nextPosition.x] & TileType.GHOST_HOME) == TileType.GHOST_HOME)) {
        		if (currentMotionDirection.positionDisplacement().y < 0) {
	        		homeOffset = new FloatPosition(homeOffset.x, HOME_OFFSET.y);
	        		animationDuration = animationDuration * (1 + homeOffset.y);
	        		lastTick =  lastTick - (long)(animationDuration * homeOffset.y);
	        		moveAnimationPhase = homeOffset.y;
        		} else if (currentMotionDirection.positionDisplacement().y > 0) {
        			/** 
        			 * this case is not properly tested as it is not possible with the current map configuration
        			 */
        			homeOffset = new FloatPosition(homeOffset.x, HOME_OFFSET.y);
	        		animationDuration = animationDuration * (1 - homeOffset.y);
	        		lastTick =  lastTick + (long)(animationDuration * homeOffset.y);
	        		moveAnimationPhase = -homeOffset.y;
        		}
        	}
    	}
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
		return currentMotionDirection.angle();
	}
	
	public void tabletEaten (long time, int tabletsEaten) {
		lastTabletEatenAt = time;
		if (isTrapped && (tabletsEaten > tabletsToLeftHome))
			isTrapped = false;
	}
   
	private final IntPosition INITIAL_POSITION;
	private final IntPosition SCATTER_POSITION;
	private final IntPosition LEAVE_HOME_POSITION;
	private final IntPosition GET_BODY_POSITION;
    
	private final float DEFAULT_ANIMATION_DURATION = 200.0f;
	private float animationDuration = DEFAULT_ANIMATION_DURATION;
	private volatile long lastTick = 0;
	private volatile float moveAnimationPhase = 0.0f;
    
    private final int defaultHomePermission;
    private int homePermission;
    
    
    protected PacManModel pacManModel;
    
    private static final MotionDirection[] possibleMotionDirections = {MotionDirection.UP, MotionDirection.LEFT, MotionDirection.DOWN, MotionDirection.RIGHT};
    private MotionDirection motionDirectionOfReturn = MotionDirection.NONE;
    private HashMap <MotionDirection, Float> availableExits = new HashMap<MotionDirection, Float>(); 
    
    private IntPosition targetPosition;
    
    private static final FloatPosition DEFAULT_OFFSET = new FloatPosition(0.0f, 0.0f);
    private static final FloatPosition HOME_OFFSET = new FloatPosition(0.5f, 0.5f);
    private FloatPosition homeOffset = DEFAULT_OFFSET;
    
    private Boolean isTrapped = true;
    private Boolean atHome = true;
    private Boolean isChasing = false;
    private Boolean isFrightened = false;
    private Boolean isReturning = false;
    
    private boolean needChangeDirection = false;
    private long lastChaseModeChangeAt = 0;
    private int currentModeWave = 0;
    private int currentLevel = 0;
    
    private long lastFrightenModeChangeAt = 0;
    
    private long lastFrightenBegunAt = 0;
    private static final long[][] waveLengthMap = {
    	{7000, 20000, 7000, 20000, 5000, 20000, 5000, -1},
    	{7000, 20000, 7000, 20000, 5000, 1033000, 1/60, -1},
    	{7000, 20000, 7000, 20000, 5000, 1033000, 1/60, -1},
    	{7000, 20000, 7000, 20000, 5000, 1033000, 1/60, -1},
    	{5000, 20000, 5000, 20000, 5000, 1037000, 1/60, -1}
    };
    
    private static long FRIGHT_DURATION = 10000L; 
    
    private String TAG = "GhostModel";
    
    private IGhostEatenHandler ghostEatenHandler;
    private Object ghostEatenHandlerMonitor = new Object();
    
    private int tabletsToLeftHome = 1;
    private int tabletsTimeToLeftHome = 10000;
    private long lastTabletEatenAt = 0;
}
