package ru.tonybo.pacman.game.model;

public enum GameState {
	
	LEVEL_INTRO () {
		public long duration () {
			return 1000L;
		}
		
		public GameState nextState () {
			return GAME;
		}
	},
	GAME () {
		public long duration () {
			return -1;
		}
		
		public GameState nextState () {
			return GAME;
		}
	},
	LEVEL_COMPLETED () {
		public long duration () {
			return 3000L;
		}
		
		public GameState nextState () {
			return LEVEL_INTRO;
		}
	},
	GAME_OVER () {
		public long duration () {
			return 3000L;
		}
		
		public GameState nextState () {
			return LEVEL_INTRO;
		}
	};
	
	public abstract long duration ();
	public abstract GameState nextState ();
}
