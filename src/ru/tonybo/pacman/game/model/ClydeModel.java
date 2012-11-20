package ru.tonybo.pacman.game.model;

public class ClydeModel extends GhostModel{

	public ClydeModel(int[][] path, int permissionLevel,
			int defaultHomePermission, PacManModel pacManModel, IntPosition initialPosition, IntPosition scatterPosition, int tabletsToLeftHome) {
		super(path, permissionLevel, defaultHomePermission, pacManModel, initialPosition, scatterPosition, tabletsToLeftHome);
		// TODO Auto-generated constructor stub
	}

	protected IntPosition calculateTargetPosition () {
		IntPosition pacManPosition = pacManModel.getCurrentPosition();
		IntPosition pacManPositionDisplacement = pacManModel.getPositionDisplacement();
		return new IntPosition (pacManPosition.x + pacManPositionDisplacement.y, pacManPosition.y + pacManPositionDisplacement.y);
		/** 
		 * TODO: calculate distance to pacman if greater than eight - use default get away target point else use pacman as target
		 */
	}
}
