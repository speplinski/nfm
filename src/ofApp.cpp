#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate( 120 );
	ofSetVerticalSync( true );
    
    serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	
	int baud = 57600;
	serial.setup( 0, baud ); //open the first device
	//serial.setup("COM4", baud); // windows
	//serial.setup("/dev/tty.usbserial-A4001JEC", baud); // mac osx
	//serial.setup("/dev/ttyUSB0", baud); //linux
    
    // Trigger
    names.push_back( "sounds/om_trigger.mp3" );
    
    // Sounds
    names.push_back( "sounds/arkadiusz_katny_opening.mp3" );
    names.push_back( "sounds/bednarz_o_u_e.mp3" );
    names.push_back( "sounds/dawid_majewski_g-ch_project.mp3" );
    names.push_back( "sounds/szmigiel_szpakipogodnieodmierzajaczas.mp3" );
    names.push_back( "sounds/tomasz_cuncvir_kedzierski_quotidian_beats.mp3" );
    
    // Voices
    names.push_back( "sounds/ludwig_b.mp3" );
    names.push_back( "sounds/om.mp3" );
    names.push_back( "sounds/owego_roku.mp3" );
    names.push_back( "sounds/tak_wiec_na_poczatku_byl.mp3" );
    names.push_back( "sounds/wokaliza.mp3" );
    
    volume = 1.0;
    
    fadeOutSpeed = 0.005;
    
    lastTime = 0;
    
    allowSerialCommunication = false;
}

//--------------------------------------------------------------
void ofApp::update()
{
	if( serial.available() )
    {
        unsigned int key = 0;
        key = serial.readByte();
            
        if( key >= names.size() ) return;
        
        cout << "byteRead: " << key << endl;
        
        lastAudioId = key;
        
        cout << "lastAudioId: " << lastAudioId << " audioId: " << audioId << " isPlaying: " << isPlaying() << endl;
        
        if( isPlaying() ) return;
        
        lastTime = ofGetUnixTime();
        audioId = key;
        volume = 1.0;
        allowSerialCommunication = true;
        
        player.unloadSound();
        player.loadSound( names.at( audioId ) );
        player.play();
        
        cout << "player.play: " << audioId << endl;
	}
    
    ofSoundUpdate();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    player.setVolume( volume );
    
    if( ofGetUnixTime() - lastTime > 5 && lastAudioId != audioId )
    {
        volume -= fadeOutSpeed;
        
        if( volume <= 0 )
        {
            if( allowSerialCommunication )
            {
                allowSerialCommunication = false;
                
                cout << "player.stop (with fade) " << endl;
                serial.writeByte( 1 );
                
                cout << "play: " << lastAudioId << endl;
                playById( lastAudioId );
            }
        }
    }
    
    if( !player.getIsPlaying() )
    {
        if( allowSerialCommunication )
        {
            allowSerialCommunication = false;
            
            cout << "player.stop " << endl;
            serial.writeByte( 1 );
            
            if( lastAudioId != audioId )
            {
                cout << "play: " << lastAudioId << endl;
                playById( lastAudioId );
            }
        }
    }
}

//--------------------------------------------------------------
bool ofApp::isPlaying()
{
    return player.getIsPlaying();
}

void ofApp::playById( int key )
{
    if( key >= names.size() ) return;
    
    lastAudioId = key;
    audioId = key;
    volume = 1.0;
    allowSerialCommunication = true;
    
    player.unloadSound();
    player.loadSound( names.at( audioId ) );
    player.play();
    
    cout << "player.playById: " << audioId << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed ( int k )
{
    int key = k - 48; // keyCode to charCode
    
    if( key >= names.size() ) return;
    
    lastAudioId = key;
    
    cout << "lastAudioId: " << lastAudioId << " audioId: " << audioId << " isPlaying: " << isPlaying() << endl;
    
    if( isPlaying() ) return;
    
    lastTime = ofGetUnixTime();
    audioId = key;
    volume = 1.0;
    allowSerialCommunication = true;
    
    player.unloadSound();
    player.loadSound( names.at( audioId ) );
    player.play();
    
    cout << "player.play: " << audioId << endl;
}
