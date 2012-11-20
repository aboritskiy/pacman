package ru.tonybo.pacman.game.view;

import ru.tonybo.pacman.game.model.GameModel;
import ru.tonybo.pacman.game.model.GameState;
import ru.tonybo.pacman.game.view.text.TextString;
import android.content.Context;

public class GameStateView {
	private final float[] INTRO_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
	private final float[] VALUE_COLOR = { 1.0f, 1.0f, 0.0f, 1.0f };
	private final float[] GAME_OVER_COLOR = { 1.0f, 0.0f, 0.0f, 1.0f };

	private GameModel gameModel;
	private TextString textString;
	private Context context;
	
	public GameStateView (Context context, GameModel gameModel) {
		this.context = context;
		this.gameModel = gameModel;
				
		/**
		 * TODO: cache fonts )
		 */
		textString = new TextString(context);
		textString.loadFont("typodermic_foo_regular.ttf", 50, 2, 2);
		textString.setScale(0.0025f);
	}


	public void draw(float[] mProjMatrix, float[] mVMatrix) {
		GameState gameState = gameModel.getGameState();
		if (gameState == GameState.GAME) {
			return;			
		} else if (gameState == GameState.LEVEL_INTRO) {
			textString.setColorV(INTRO_COLOR);
			textString.draw("GET READY!", -0.34f, -0.2f, mProjMatrix, mVMatrix);
		} else if (gameState == GameState.LEVEL_COMPLETED) {
			textString.setColorV(INTRO_COLOR);
			textString.draw("Level", -0.25f, -0.2f, mProjMatrix, mVMatrix);
			String levelStr = "" + gameModel.getLevel();
			if (levelStr.length() == 1) {
				levelStr = "0" + levelStr;
			}
			textString.setColorV(VALUE_COLOR);
			textString.draw(levelStr, 0.1f, -0.2f, mProjMatrix, mVMatrix);
		} else if (gameState == GameState.GAME_OVER) {
			textString.setColorV(GAME_OVER_COLOR);
			textString.draw("GAME OVER", -0.34f, -0.2f, mProjMatrix, mVMatrix);
		}
	}
}
