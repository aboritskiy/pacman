package ru.tonybo.pacman.game.model;

public class PinkyModel extends GhostModel{

	public PinkyModel(int[][] path, int permissionLevel,
			int defaultHomePermission, PacManModel pacManModel, IntPosition initialPosition, IntPosition scatterPosition, int tabletsToLeftHome) {
		super(path, permissionLevel, defaultHomePermission, pacManModel, initialPosition, scatterPosition, tabletsToLeftHome);
		// TODO Auto-generated constructor stub
	}

	protected IntPosition calculateTargetPosition () {
		IntPosition pacManPosition = pacManModel.getCurrentPosition();
		IntPosition pacManPositionDisplacement = pacManModel.getPositionDisplacement();
		return new IntPosition (pacManPosition.x + 4 * pacManPositionDisplacement.y, pacManPosition.y + 4 * pacManPositionDisplacement.y);
	}
}
