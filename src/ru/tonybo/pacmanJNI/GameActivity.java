package ru.tonybo.pacmanJNI;

import ru.tonybo.pacmanJNI.R;
import android.app.Activity;
import android.opengl.*;
import android.os.*;
import android.view.View;

public class GameActivity extends Activity {
	
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
		setContentView(menuViewID); 		// after this gameView freezes
		gameView.setVisibility(View.GONE);	// after this application throw an error: android.view.ViewRootImpl$CalledFromWrongThreadException: Only the original thread that created a view hierarchy can touch its views.
	}
}