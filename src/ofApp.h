#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	
	public:
		void setup();
		void update();
		void draw();
        bool isPlaying();
    
        void keyPressed(int key);
        void playById( int key );
    
        unsigned int lastTime;
        unsigned int audioId;
        unsigned int lastAudioId;
        float fadeOutSpeed;
        bool allowSerialCommunication;
		
		ofSerial	serial;
        ofSoundPlayer player;
    
        vector <string> names;
        float volume;
};

