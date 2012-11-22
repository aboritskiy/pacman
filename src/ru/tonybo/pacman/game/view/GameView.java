package ru.tonybo.pacman.game.view;

import android.content.Context;
import ru.tonybo.pacman.game.model.GameModel;
import ru.tonybo.pacman.game.model.IntPosition;

public class GameView implements IDrawable{

	public GameView (Context context, GameModel gameModel) {
		this.gameModel = gameModel;
		
		fieldView = new FieldView(context);
		tabletsView = new TabletsView(context, gameModel.getTabletPath());
		pacManView = new PacManView(context, gameModel.getPacManModel());
		
		blinkyView = new GhostView(context, gameModel.getBlinkyModel(), BLINKY_COLOR);
		pinkyView = new GhostView(context, gameModel.getPinkyModel(), PINKY_COLOR);
		inkeyView = new GhostView(context, gameModel.getInkeyModel(), INKEY_COLOR);
		clydeView = new GhostView(context, gameModel.getClydeModel(), CLYDE_COLOR);
		
		scoreView = new ScoreView(context, gameModel); 
		livesView = new LivesView(context, gameModel);
		
		gameStateView = new GameStateView(context, gameModel);
		
		energizerViews = new EnergizerView[gameModel.getEnergizerCount()];
		int[][] energizerPath = gameModel.getEnergizerPath();
		int k = 0;
		for (int j = 0; j < energizerPath.length; j++) {
			for (int i = 0; i < energizerPath[j].length; i++) {
				if (energizerPath[j][i] > 0)
					energizerViews[k++] = new EnergizerView(context, energizerPath, new IntPosition(i,j));
			}
		}
		
	}
	
	public void draw (float[] mProjMatrix, float[] mVMatrix) {
		fieldView.draw(mProjMatrix, mVMatrix);
		tabletsView.draw(mProjMatrix, mVMatrix);
		for (int i = 0; i < energizerViews.length; i++) {
			energizerViews[i].draw(mProjMatrix, mVMatrix);
		}
		
		blinkyView.draw(mProjMatrix, mVMatrix);
		pinkyView.draw(mProjMatrix, mVMatrix);
		inkeyView.draw(mProjMatrix, mVMatrix);
		clydeView.draw(mProjMatrix, mVMatrix);
		
		pacManView.draw(mProjMatrix, mVMatrix);
		
		scoreView.draw(mProjMatrix, mVMatrix);
		livesView.draw(mProjMatrix, mVMatrix);
		
		gameStateView.draw(mProjMatrix, mVMatrix);
	}
	
	private GameModel gameModel;
	
	private FieldView fieldView;
	private TabletsView tabletsView; 
	private PacManView pacManView; 
	
	private GhostView blinkyView, pinkyView, inkeyView, clydeView;
	
	private EnergizerView[] energizerViews;
	
	private ScoreView scoreView;
	private LivesView livesView; 
	
	private GameStateView gameStateView;
	
	private final float[] BLINKY_COLOR = { 0.87f, 0.0f, 0.0f, 1.0f };
	private final float[] PINKY_COLOR = { 1.0f, 0.6f, 0.6f, 1.0f };
	private final float[] INKEY_COLOR = { 0.4f, 1.0f, 1.0f, 1.0f };
	private final float[] CLYDE_COLOR = { 1.0f, 0.6f, 0.0f, 1.0f };
}
