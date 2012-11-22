package ru.tonybo.pacman.game.view;

import android.content.Context;
import ru.tonybo.pacman.game.model.GameModel;
import ru.tonybo.pacman.game.view.text.TextString;

public class ScoreView implements IDrawable{
	private final float[] LABEL_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
	private final float[] VALUE_COLOR = { 1.0f, 1.0f, 0, 1.0f };

	private GameModel gameModel;
	private TextString textString;
	private Context context;
	
	public ScoreView (Context context, GameModel gameModel) {
		this.context = context;
		this.gameModel = gameModel;
				
		/**
		 * TODO: cache fonts )
		 */
		textString = new TextString(context);
		textString.loadFont("typodermic_foo_regular.ttf", 50, 2, 2);
		textString.setScale(0.003f);
	}

	public void draw(float[] mProjMatrix, float[] mVMatrix) {
		textString.setColorV(LABEL_COLOR);
		textString.draw("Score", -1.0f, -1.2f, mProjMatrix, mVMatrix);
		textString.setColorV(VALUE_COLOR);
		textString.draw("" + gameModel.getScore(), -0.55f, -1.2f, mProjMatrix, mVMatrix);
	}
}
