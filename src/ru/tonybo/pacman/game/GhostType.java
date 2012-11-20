package ru.tonybo.pacman.game;

import ru.tonybo.pacman.game.model.IntPosition;

public enum GhostType {
	BLINKY () {
		public IntPosition targetPoint () {
			return BLINKY_DISP;
		}
		public float[] color () {
			return BLINKY_COLOR;
		}
	},
	PINKY () {
		public IntPosition targetPoint () {
			return BLINKY_DISP;
		}
		public float[] color () {
			return PINKY_COLOR;
		}
	},
	INKEY () {
		public IntPosition targetPoint () {
			return BLINKY_DISP;
		}
		public float[] color () {
			return INKEY_COLOR;
		}
	},
	CLYDE () {
		public IntPosition targetPoint () {
			return BLINKY_DISP;
		}
		public float[] color () {
			return CLYDE_COLOR;
		}
	};
	
	public abstract IntPosition targetPoint ();
	public abstract float[] color();
	
	private static final IntPosition BLINKY_DISP 	= new IntPosition( 0, 0);
	
	private static final float[] BLINKY_COLOR 	= { 221/255, 0.0f, 0.0f, 1.0f };
	private static final float[] PINKY_COLOR 	= { 1.0f, 153/255, 153/255, 1.0f };
	private static final float[] INKEY_COLOR 	= { 102/255, 1.0f, 1.0f, 1.0f };
	private static final float[] CLYDE_COLOR 	= { 1.0f, 153/255, 0.0f, 1.0f };
}