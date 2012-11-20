package ru.tonybo.pacman.game;

import ru.tonybo.pacman.game.model.IntPosition;

public enum MotionDirection {
	NONE () {
		public IntPosition positionDisplacement () {
			return NONE_POSITION;
		}
		public float angle () {
			return 0.0f;
		}
		public MotionDirection returnDirection () {
			return NONE;
		}
        public int intValue () {
			return 0;
		}
	},
	LEFT () {
		public IntPosition positionDisplacement () {
			return LEFT_POSITION;
		}
		public float angle () {
			return 180.0f;
		}
		public MotionDirection returnDirection () {
			return RIGHT;
		}
        public int intValue () {
			return 1;
		}
	}, RIGHT () {
		public IntPosition positionDisplacement () {
			return RIGHT_POSITION;
		}
		public float angle () {
			return 0.0f;
		}
		public MotionDirection returnDirection () {
			return LEFT;
		}
        public int intValue () {
			return 2;
		}
	}, UP () {
		public IntPosition positionDisplacement () {
			return UP_POSITION;
		}
		public float angle () {
			return 90.0f;
		}
		public MotionDirection returnDirection () {
			return DOWN;
		}
        public int intValue () {
			return 3;
		}
	}, DOWN () {
		public IntPosition positionDisplacement () {
			return DOWN_POSITION;
		}
		public float angle () {
			return 270.0f;
		}
		public MotionDirection returnDirection () {
			return UP;
		}
        public int intValue () {
			return 4;
		}
	};
	
	public abstract IntPosition positionDisplacement ();
	public abstract MotionDirection returnDirection ();
	public abstract float angle();
    public abstract int intValue();
	
	private static final IntPosition NONE_POSITION 	= new IntPosition( 0, 0);
	private static final IntPosition LEFT_POSITION 	= new IntPosition(-1, 0);
	private static final IntPosition RIGHT_POSITION = new IntPosition( 1, 0);
	private static final IntPosition UP_POSITION 	= new IntPosition( 0,-1);
	private static final IntPosition DOWN_POSITION 	= new IntPosition( 0, 1);
}
