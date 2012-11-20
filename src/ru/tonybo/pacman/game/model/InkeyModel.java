package ru.tonybo.pacman.game.model;

public class InkeyModel extends GhostModel{

	public InkeyModel(int[][] path, int permissionLevel,
			int defaultHomePermission, PacManModel pacManModel, IntPosition initialPosition, IntPosition scatterPosition, int tabletsToLeftHome, GhostModel blinkyModel) {
		super(path, permissionLevel, defaultHomePermission, pacManModel, initialPosition, scatterPosition, tabletsToLeftHome);
		
		this.blinkyModel = blinkyModel;
	}

	protected IntPosition calculateTargetPosition () {
		IntPosition pacManPosition = pacManModel.getCurrentPosition();
		IntPosition pacManPositionDisplacement = pacManModel.getPositionDisplacement();
		IntPosition blinkyPosition = blinkyModel.getCurrentPosition();
		
		return new IntPosition (
				(pacManPosition.x + 2 * pacManPositionDisplacement.x - blinkyPosition.x) * 2 + blinkyPosition.x,
				(pacManPosition.y + 2 * pacManPositionDisplacement.y - blinkyPosition.y) * 2 + blinkyPosition.y
			);
	}
	
	private GhostModel blinkyModel;
}
