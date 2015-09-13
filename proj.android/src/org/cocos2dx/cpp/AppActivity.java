package org.cocos2dx.cpp;

import java.util.Arrays;

import org.cocos2d.layers.CCScene;
import org.cocos2d.nodes.CCDirector;
import org.cocos2d.opengl.CCGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.ContentResolver;
import android.content.Intent;
import android.content.IntentSender.SendIntentException;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

import com.facebook.AppEventsLogger;
import com.facebook.FacebookAuthorizationException;
import com.facebook.FacebookOperationCanceledException;
import com.facebook.Session;
import com.facebook.SessionState;
import com.facebook.UiLifecycleHelper;
import com.facebook.model.GraphUser;
import com.facebook.widget.FacebookDialog;
import com.facebook.widget.LoginButton;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.GoogleApiClient.ConnectionCallbacks;
import com.google.android.gms.common.api.GoogleApiClient.OnConnectionFailedListener;
import com.google.android.gms.plus.Plus;
import com.google.android.gms.plus.PlusShare;
import com.google.android.gms.plus.Plus.PlusOptions;
import com.google.android.gms.plus.model.people.Person;
import com.project.scrabble.R;

public class AppActivity extends Cocos2dxActivity //implements OnClickListener,
//ConnectionCallbacks, OnConnectionFailedListener
{
//	protected CCGLSurfaceView _glSurfaceView;
	//public static native void CallCocosSessionExist();

	//FACEBOOK
//    private final String PENDING_ACTION_BUNDLE_KEY = "org.cocos2dx.cpp.AppActivity:PendingAction";
//	private LoginButton loginButton;
//    //private ProfilePictureView profilePictureView;
//    //private TextView greeting;
//    private GraphUser user;
//    //GOOGLE PLUS
//    private static final int PICK_MEDIA_REQUEST_CODE = 8;
//	   private static final int SHARE_MEDIA_REQUEST_CODE = 9;
//	   private static final int SIGN_IN_REQUEST_CODE = 10;
//	   private static final int ERROR_DIALOG_REQUEST_CODE = 11;
//    // For communicating with Google APIs
//	   private GoogleApiClient mGoogleApiClient;
//	   private boolean mSignInClicked;
//	   private boolean mIntentInProgress;
//	   // contains all possible error codes for when a client fails to connect to
//	   // Google Play services
//	   private ConnectionResult mConnectionResult;
//	
//	private UiLifecycleHelper uiHelper;

//    private Session.StatusCallback callback = new Session.StatusCallback()
//    {
//        @Override
//        public void call(Session session, SessionState state, Exception exception) 
//        {
//       //     onSessionStateChange(session, state, exception);
//        }
//    };
    
//    private void onSessionStateChange(Session session, SessionState state, Exception exception) 
//    {
////        if ((exception instanceof FacebookOperationCanceledException ||
////                exception instanceof FacebookAuthorizationException)) 
////        {
////                new AlertDialog.Builder(AppActivity.this)
////                    .setTitle(R.string.cancelled)
////                    .setMessage(R.string.permission_not_granted)
////                    .setPositiveButton(R.string.ok, null)
////                    .show();
////                
////        } 
////        else if (state == SessionState.OPENED_TOKEN_UPDATED) 
////        {
////        	
////        }
////        updateUI();
//    }
    
//    private void updateUI() 
//    {
//        Session session = Session.getActiveSession();
//
//        boolean enableButtons = (session != null && session.isOpened());
//
//        if (enableButtons && user != null) 
//        {
//            Log.d("FACEBOOK","USERNAME:"+user.getFirstName()+" "+user.getLastName());
//            Log.d("FACEBOOK","FB ID :"+user.getId());
//            Log.d("FACEBOOK","EMAIL ID:"+user.asMap().get("email"));
//            Log.d("FACEBOOK","IMAGE URL"+"http://graph.facebook.com/"+user.getId()+"/picture?type=large");
//        } 
//        else
//        {
//            
//        }
//    }
    
//    private FacebookDialog.Callback dialogCallback = new FacebookDialog.Callback() {
////        @Override
////        public void onError(FacebookDialog.PendingCall pendingCall, Exception error, Bundle data) {
////            Log.d("HelloFacebook", String.format("Error: %s", error.toString()));
////        }
////
////        @Override
////        public void onComplete(FacebookDialog.PendingCall pendingCall, Bundle data) {
////            Log.d("HelloFacebook", "Success!");
////        }
//    };
    
//    private GoogleApiClient buildGoogleAPIClient()
//	   {
//	      return new GoogleApiClient.Builder(this).addConnectionCallbacks(this)
//	            .addOnConnectionFailedListener(this)
//	            .addApi(Plus.API, PlusOptions.builder().build())
//	            .addScope(Plus.SCOPE_PLUS_LOGIN).build();
//	   }
    
	   // * API to handler sign in of user If error occurs while connecting process
	   // * it in processSignInError() api
	    
//	   private void processSignIn() {
	 
//	      if (!mGoogleApiClient.isConnecting()) {
//	         processSignInError();
//	         mSignInClicked = true;
//	      }
	 
//	   }
	 
	   
	   // API to process sign in error Handle error based on ConnectionResult
	    
//	   private void processSignInError() {
//	      if (mConnectionResult != null && mConnectionResult.hasResolution()) {
//	         try {
//	            mIntentInProgress = true;
//	            mConnectionResult.startResolutionForResult(this,
//	                  SIGN_IN_REQUEST_CODE);
//	         } catch (SendIntentException e) {
//	            mIntentInProgress = false;
//	            mGoogleApiClient.connect();
//	         }
//	      }
//	   }
	 
	 @Override
	    public void onCreate(Bundle savedInstanceState)
	    {
	        super.onCreate(savedInstanceState);   
	      
//	        _glSurfaceView = new CCGLSurfaceView(this);
//	        setContentView(_glSurfaceView);
	        
//	        setContentView(R.layout.my_main_activity);
//	        findViewById(R.id.sign_in_button).setOnClickListener(this);
	       // uiHelper = new UiLifecycleHelper(this, callback);
//	        uiHelper.onCreate(savedInstanceState);
//
//	        if (savedInstanceState != null)
//	        {
//	        	
//	        }
//	        Session session_exist_bool = Session.getActiveSession();
//            
//	        if(session_exist_bool!= null)
//	        {
//	        	ScrabbleJNIHelper.CallCocosSessionExist();
//	        }
//	       
//	        loginButton = (LoginButton) findViewById(R.id.login_button);
//	        loginButton.setReadPermissions(Arrays.asList("email","public_profile"));
//	        loginButton.setUserInfoChangedCallback(new LoginButton.UserInfoChangedCallback() 
//	        {
//	            @Override
//	            public void onUserInfoFetched(GraphUser user) 
//	            {
//	                AppActivity.this.user = user;
//	                updateUI();
//	                // It's possible that we were waiting for this.user to be populated in order to post a
//	                // status update.
//	            }
//	        });
//	        
//		      // Initializing google plus api client
//		      mGoogleApiClient = buildGoogleAPIClient();  
	    }	

	  @Override
	  protected void onStart() 
	   {
	      super.onStart();
//	      CCDirector.sharedDirector().attachInView(_glSurfaceView);
//	      
//	      CCDirector.sharedDirector().setDisplayFPS(true);
//	   
//	      CCDirector.sharedDirector().setAnimationInterval(1.0f / 60.0f);
	      // make sure to initiate connection
	     	      
//	      mGoogleApiClient.connect();
	   }
	 
	   @Override
	   protected void onStop() 
	   {
	      super.onStop();
//	      CCDirector.sharedDirector().end();
	      // disconnect api if it is connected
//	      if (mGoogleApiClient.isConnected())
//	         mGoogleApiClient.disconnect();    
	   } 
	 
	 @Override
	    protected void onResume() 
	    {
	        super.onResume();
//	        CCDirector.sharedDirector().resume();
//	        uiHelper.onResume();
//
//	        // Call the 'activateApp' method to log an app event for use in analytics and advertising reporting.  Do so in
//	        // the onResume methods of the primary Activities that an app may be launched into.
//	        AppEventsLogger.activateApp(this);
//	        updateUI();   

	    }

	    @Override
	    protected void onSaveInstanceState(Bundle outState) 
	    {
	        super.onSaveInstanceState(outState);
	        //uiHelper.onSaveInstanceState(outState);
	    }

//	    @Override
//	    protected void onActivityResult(int requestCode, int resultCode, Intent data) 
//	    {
//	        super.onActivityResult(requestCode, resultCode, data);
//	        uiHelper.onActivityResult(requestCode, resultCode, data, dialogCallback);
//	        if (requestCode == SIGN_IN_REQUEST_CODE) 
//		      {
//		         if (resultCode != RESULT_OK)
//		         {
//		            mSignInClicked = false;
//		         }
//		 
//		         mIntentInProgress = false;
//		 
//		         if (!mGoogleApiClient.isConnecting())
//		         {
//		            mGoogleApiClient.connect();
//		         }
//		      } 
//		      else if (requestCode == PICK_MEDIA_REQUEST_CODE) 
//		      {
//		         // If picking media is success, create share post using
//		         // PlusShare.Builder
//		         if (resultCode == RESULT_OK)
//		         {
//		            Uri selectedImage = data.getData();
//		            ContentResolver cr = this.getContentResolver();
//		            String mime = cr.getType(selectedImage);
//		 
//		            PlusShare.Builder share = new PlusShare.Builder(this);
//		            share.setText("Hello from AndroidSRC.net");
//		            share.addStream(selectedImage);
//		            share.setType(mime);
//		            startActivityForResult(share.getIntent(),
//		                  SHARE_MEDIA_REQUEST_CODE);
//		         }
//		      }   
//	    }

	    @Override
	    public void onPause() 
	    {
	        super.onPause();
//	        CCDirector.sharedDirector().pause();
//	        uiHelper.onPause();
//	        // Call the 'deactivateApp' method to log an app event for use in analytics and advertising
//	        // reporting.  Do so in the onPause methods of the primary Activities that an app may be launched into.
//	        AppEventsLogger.deactivateApp(this);   
	    }

	    @Override
	    public void onDestroy() 
	    {
	        super.onDestroy();
	       // uiHelper.onDestroy();
	    }
	    
//		@Override
//		public void onConnectionFailed(ConnectionResult result) 
//		{
//			 if (!result.hasResolution()) {
//		         GooglePlayServicesUtil.getErrorDialog(result.getErrorCode(), this,
//		               ERROR_DIALOG_REQUEST_CODE).show();
//		         return;
//		      }
//		      if (!mIntentInProgress) {
//		         mConnectionResult = result;
//		 
//		         if (mSignInClicked) {
//		            processSignInError();
//		         }
//		      }			
//		}
//		@Override
//		public void onConnected(Bundle connectionHint)
//		{
//			 mSignInClicked = false;
//		      processUserInfo();
//		      Toast.makeText(getApplicationContext(), "Signed In Successfully",
//		            Toast.LENGTH_LONG).show();			
//		}
		
//		 private void processUserInfo()
//		 {
//		      Person signedInUser = Plus.PeopleApi.getCurrentPerson(mGoogleApiClient);
//		      if (signedInUser != null) 
//		      {
//		    	  String userID = signedInUser.getId();
//		    	  Log.d("GOOGLE PLUS","USER ID: " + userID);
//		         if (signedInUser.hasDisplayName()) {
//		            String userName = signedInUser.getDisplayName();
//		            Log.d("GOOGLE PLUS","Name: " + userName);
//		         } 
//		         if (signedInUser.hasImage()) {
//		            String userProfilePicUrl = signedInUser.getImage().getUrl();
//		            Log.d("GOOGLE PLUS","IMAGE URL: "+userProfilePicUrl);
//		         } 
//		      }
//		   }
		 
//		@Override
//		public void onConnectionSuspended(int cause)
//		{
////		      mGoogleApiClient.connect();	
//		}
		
//		@Override
//		public void onClick(View view)
//		{
////		     processSignIn();	
//		}  
		
		static {
	        System.loadLibrary("cocos2dcpp");
	    } 

}
