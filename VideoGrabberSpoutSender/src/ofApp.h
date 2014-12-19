/*
Spout OpenFrameworks VideoGrabberSpoutSender example - ofApp.h
Copyright (C) 2014 Lynn Jarvis.
Modified by Bartosh Polonski
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
================================================================================================
*/

#pragma once

#include "ofMain.h"						// The usual
#include "Spout.h"	// Spout SDK

class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();

	// Spout Setup

	SpoutSender *spoutsender;		// A sender object
	char sendername[256];			// Shared memory name
	GLuint sendertexture;			// Local OpenGL texture used for sharing
	bool bInitialized;				// Initialization result
	bool bMemoryShare;				// Texture share compatibility
	ofImage myTextureImage;			// Texture image for the 3D demo
	unsigned int g_Width, g_Height;	// Global width and height

	// Video Setup
	ofVideoGrabber video;

	void ofApp::setupSpout()
	{
		bInitialized	= false;							// Spout sender initialization
		g_Width			= 640;								// set starting global width and height for the sender
		g_Height		= 360;

		strcpy_s(sendername, "OF Spout Sender");			// Set the sender name
		ofSetWindowTitle(sendername);						// show it on the title bar
		ofSetWindowShape(g_Width, g_Height);				// Set the window size

		spoutsender = new SpoutSender;						// Create a new Spout sender
		spoutsender -> SetDX9(true);						// set DirectX9 mode (may solve issues on some gpu (like Intel HD 5000) )

		sendertexture = 0;									// Initially there is no local OpenGL texture ID
		InitGLtexture(sendertexture, g_Width, g_Height);	// Create an OpenGL texture for data transfers

	}

	void ofApp::startSpout()
	{
		// INITIALIZE A SENDER
		if(!bInitialized) {
			bInitialized = spoutsender->CreateSender(sendername, g_Width, g_Height);
			// Detect texture share compatibility (optional)
			bMemoryShare = spoutsender->GetMemoryShareMode();
		}	
	}

	void ofApp::endSpout()
	{
		if(bInitialized) 
		{
			// Grab the screen into the local spout texture
			glBindTexture(GL_TEXTURE_2D, sendertexture); 
			glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_Width, g_Height);
			glBindTexture(GL_TEXTURE_2D, 0); 

			// Send Texture
			spoutsender->SendTexture(sendertexture, GL_TEXTURE_2D, g_Width, g_Height);
		}
	}

	void ofApp::showDebug()
	{
		char str[256];
		ofSetColor(255);

		// Show what it is sending
		ofSetColor(255); 
		if(!bMemoryShare) sprintf_s(str, "Sending as : [%s]", sendername);
		else sprintf_s(str, "Memoryshare sender");
		ofDrawBitmapString(str, 20, 20);

		// Show fps
		sprintf_s(str, "fps: %3.3d", (int)ofGetFrameRate());
		ofDrawBitmapString(str, ofGetWidth()-120, 20);
	}

	bool ofApp::InitGLtexture(GLuint &texID, unsigned int width, unsigned int height)
	{
		if(texID != 0) glDeleteTextures(1, &texID);
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		return true;
	}

	void ofApp::exit() { 

		// Release the sender - this can be used for repeated 
		// Create and Release of a sender within the program
		spoutsender->ReleaseSender(); 

		// At program termination, clean up everything
		delete spoutsender;
		spoutsender = NULL;

	}

	void ofApp::windowResized(int w, int h)
	{

		// Update the global width and height
		g_Width  = w;
		g_Height = h;

		// Update the sender texture to receive the new dimensions
		InitGLtexture(sendertexture, g_Width, g_Height);

		// Any change of the global width and height is handled 
		// within the SendTexture function and the sender information
		// in shared memory is updated for receivers to detect.

	}

};
