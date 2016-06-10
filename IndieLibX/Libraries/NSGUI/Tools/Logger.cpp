#include "Logger.h"

/*
D3DXFROMWINEMATRIX g_worldViewProjection;
D3DXFROMWINEMATRIX projection;
D3DXFROMWINEMATRIX mv;
*/
std::vector<NSString> NatureScene::Logger::logStrings;

NSString	       NatureScene::Logger::logPath;

void NatureScene::Logger::push_back(const NSString &info)

{

	  if(!info.getLength())return;



	static int c=0;

	c++;

    logStrings.push_back(info);

    flush();

  

	//if(c==22) {DBG_HALT;}

  //DBG_HALT;//???

}



void NatureScene::Logger::initialize(const char* logfilename)

{

  logPath     = !logfilename ? "Log.txt" : logfilename;

  ofstream logFile(logPath);

  logFile.close();

  logStrings.clear();

}



void NatureScene::Logger::flush()

{

  if(!logPath.getLength() || !logStrings.size())

    return;



  ofstream logFile(logPath, ios::app);



  for(size_t t = 0; t < logStrings.size(); t++)

  {

    logFile << logStrings[t];

	//logFile.flush();

  }



  logStrings.clear();

  logFile.close();

}



void NatureScene::Logger::writeImmidiateInfoLog(const NSString &info)

{

  if(info.getLength())

  {

    push_back(NSString("<+>") + info + "\n");

    flush();

  }

      //DBG_HALT;//???

}



void NatureScene::Logger::writeInfoLog(const NSString &info)

{

  push_back(NSString("<+>") + info + "\n");

  //if(logStrings.size() >= 10)

  //  flush();

}



bool NatureScene::Logger::writeErrorLog(const NSString &info)

{

  if(info.getLength())

  {

    push_back(NSString("<!>") + info + "\n");

    flush();

  }

    DBG_HALT;//???

  return false;

}



void NatureScene::Logger::writeFatalErrorLog(const NSString &info)

{

  if(info.getLength())

  {

   push_back(NSString("<X>") + info + "\n");



    flush();

  }

  DBG_HALT;

}

