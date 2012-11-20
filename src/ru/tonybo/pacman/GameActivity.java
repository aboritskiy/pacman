package ru.tonybo.pacman;

import android.app.Activity;
import android.opengl.*;
import android.os.*;
import android.view.View;

public class GameActivity extends Activity {
	
	private class GameOverHandler implements Runnable {

		@Override
		public void run() {
			handleGameOver();
		}
	}

    private GLSurfaceView gameView;
    private int menuViewID;
    
    private Handler gameOverHandler;
    
    public GameActivity () {
		super();
	}

    @Override
    public void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
    	
    	gameOverHandler = new Handler() {

    	    public void handleMessage (Message msg){
    	    	handleGameOver();
    	    	//runOnUiThread(new GameOverHandler());
    	    }
    	};
    	gameView = new GameView(this, gameOverHandler);
    	menuViewID = R.layout.main;
    	
        setContentView(menuViewID);
    }
    
    /** Called when the user selects the Send button */
    public void startGame(View view) {
    	setContentView(gameView);
    	gameView.setVisibility(View.VISIBLE);
    }

	private void handleGameOver() {
		/**
		 * TODO: switch back to main menu
		 */
		
		setContentView(menuViewID); 		// after this gameView freezes
		gameView.setVisibility(View.GONE);	// after this application throw an error: android.view.ViewRootImpl$CalledFromWrongThreadException: Only the original thread that created a view hierarchy can touch its views.


	}
}