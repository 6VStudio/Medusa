// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
class TestSettings
{
    uint Version = 1;	//  main=1?free=2
    uint Language = 1;   //  enus=1,zhcn=2
    uint Device = 2;   //  sd=1,hd=2
	uint DebugInfo=0;	//Performance=1,GPU=2,Touch=4
	
    bool IsMockEnabled = false; //mock
    bool IsCheatEnabled = false;    
    bool IsDebugEnabled = true;    
	bool IsFileUpdateEnabled=false;
	bool IsEncryptEnabled=false;	
	bool IsCompressedEnabled=false;	

	uint MessageVersion=1;	
	uint LoginServerId=1;		
	
	HeapString UpdateServerUrl = "http://static.hhyx.wyximg.com:9999/version1/";  
	HeapString UpdateServerConfigName = "Server.bin";   
	
    float MessageTimeout = 100000.0;    
    float AutoSaveDuration = 5.0;   
	
	uint MessageRetryTimes=5;	
	uint MessageRetryInterval=1000;	
	
    uint HTTPConnectTimeoutSeconds = 5;  
    uint HTTPReadTimeoutSeconds = 5;   
    uint HTTPReadLowSpeedLimit = 10;   
    uint HTTPReadLowSpeedTime = 10;    

    float ToastShowTime = 0.5;  

	
    TestSettings()
    {
	
    }
    
   
	
}