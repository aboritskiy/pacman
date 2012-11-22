package ru.tonybo.pacmanJNI.game;

public enum MotionDirection {
	NONE () {
        public int intValue () {
			return 0;
		}
	},
	LEFT () {
        public int intValue () {
			return 1;
		}
	}, RIGHT () {
        public int intValue () {
			return 2;
		}
	}, UP () {
        public int intValue () {
			return 3;
		}
	}, DOWN () {
        public int intValue () {
			return 4;
		}
	};
	
    public abstract int intValue();
}
