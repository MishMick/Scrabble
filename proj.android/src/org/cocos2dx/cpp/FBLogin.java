package org.cocos2dx.cpp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;

import com.facebook.Request;
import com.facebook.Response;
import com.facebook.Session;
import com.facebook.Session.StatusCallback;
import com.facebook.SessionState;
import com.facebook.model.GraphObject;
import com.facebook.model.GraphUser;
 
public class FBLogin extends Activity implements Session.StatusCallback {
 
  private static List<String> permissions;
	Session.StatusCallback statusCallback = new FBLogin();
	static ProgressDialog dialog;
	static FBLogin fb;
	private static Session Session;
	private static SessionState SessionState;
	private static Exception Exception;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		
		/***** FB Permissions *****/
		
		permissions = new ArrayList<String>();
		permissions.add("email");
		permissions.add("public_profile");
		/***** End FB Permissions *****/
		
			
			
				// TODO Check if there is any Active Session, otherwise Open New Session
				Session session = Session.getActiveSession();
				
				if(!session.isOpened()) 
				{
					session.openForRead(new Session.OpenRequest(FBLogin.this).setCallback(statusCallback).setPermissions(permissions));
				}
				else
				{
					Session.openActiveSession(FBLogin.this, true, statusCallback);
				}
			
	
		
		if(session == null) {
			if(savedInstanceState != null) {
				session = Session.restoreSession(this, null, statusCallback, savedInstanceState);
			}
			if(session == null) {
				session = new Session(this);
			}
			Session.setActiveSession(session);
			session.addCallback(statusCallback);
			if(session.getState().equals(SessionState.CREATED_TOKEN_LOADED)) {
				session.openForRead(new Session.OpenRequest(this).setCallback(statusCallback).setPermissions(permissions));
			} 
		}
	}
	
	public static void processSessionStatus(Session session, SessionState state, Exception exception) {
	fb.getApplicationContext();
		if(session != null && session.isOpened())
		{
			
			if(session.getPermissions().contains("email")) {
				//Show Progress Dialog 
				dialog = new ProgressDialog(fb.getApplicationContext());
				dialog.setMessage("Loggin in..");
				dialog.show();
				Request.executeMeRequestAsync(session, new Request.GraphUserCallback() {
 
					@Override
					public void onCompleted(GraphUser user, Response response) {
						
 
						if (dialog!=null && dialog.isShowing()) {
							dialog.dismiss();
						}
						if(user != null) {
							Map<String, Object> responseMap = new HashMap<String, Object>();
							GraphObject graphObject = response.getGraphObject();
							responseMap = graphObject.asMap();
							Log.i("FbLogin", "Response Map KeySet - " + responseMap.keySet());
							// TODO : Get Email responseMap.get("email"); 
							
						/*	String fb_id = user.getId();
							String email = null;
							String name = (String) responseMap.get("name");
							if (responseMap.get("email")!=null) 
							{
								email = responseMap.get("email").toString();
								//TODO Login successfull Start your next activity
								startActivity(new Intent(FBLogin.this, FBLogin.class));
							}
							else
							{
								//Clear all session info & ask user to login again
								Session session = Session.getActiveSession();
								if(session != null) {
									session.closeAndClearTokenInformation();
								}
							} */
						}
					}
				});
			} else {
				session.requestNewReadPermissions(new Session.NewPermissionsRequest(fb.getParent(), permissions));
			}
		}
	}
		
	/********** Activity Methods **********/
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		Log.d("FbLogin", "Result Code is - " + resultCode +"");
		Session.getActiveSession().onActivityResult(FBLogin.this, requestCode, resultCode, data);
	}
 
	@Override
	protected void onSaveInstanceState(Bundle outState) {
		// TODO Save current session
		super.onSaveInstanceState(outState);
		Session session = Session.getActiveSession();
		Session.saveSession(session, outState);
	}
	
	@Override
	protected void onStart() {
		// TODO Add status callback
		super.onStart();
		Session.getActiveSession().addCallback(statusCallback);
	}
	
	@Override
	protected void onStop() {
		// TODO Remove callback
		super.onStop();
		Session.getActiveSession().removeCallback(statusCallback);
	}
	
	public static void callSession(Session session, SessionState state, Exception exception)
	{
		processSessionStatus(session, state, exception);		
	}

	@Override
	public void call(Session session, SessionState state, Exception exception) {
		// TODO Auto-generated method stub
		
	}
}