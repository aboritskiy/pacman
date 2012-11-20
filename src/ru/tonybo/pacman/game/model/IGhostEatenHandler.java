package ru.tonybo.pacman.game.model;

public interface IGhostEatenHandler {
	void handleGhostEaten (IntPosition ghostPosition, int score);
}
