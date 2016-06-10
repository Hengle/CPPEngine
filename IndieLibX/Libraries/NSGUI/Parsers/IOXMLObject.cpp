#include "IOXMLObject.h"

#include "../Managers/MediaPathManager.h"



IOXMLObject::IOXMLObject(const char *ionameArg)

{

  ioname = ionameArg;

}



bool IOXMLObject::loadXMLSettings(const char* apath)

{

	std::string f1=apath;
	std::string f2=stx_convertpath(f1);
	const char* path=f2.c_str();

  if(!ioname.getLength())

  {

    NatureScene::Logger::writeErrorLog(NSString("Cannot load XML settings -> 0 element name"));

    return false;

  }

  //LOG_PRINT("path    =%s\n",path);

  const char *verified = MediaPathManager::lookUpMediaPath(path);

  //LOG_PRINT("verified:%s\n",verified);

	char path3[1024];
	stx_strlcpy(path3,path,1024);

		int slash1 = int(stx_strrchr(path3, stx_getOsSeparator())-path3);
		int slash2 = int(stx_strrchr(path3, stx_getOsSeparator())-path3);
		int slash=max(slash1,slash2);
		path3[slash+1]='\0';

  MediaPathManager::registerPath(path3);



  if(!verified)

  {

	NatureScene::Logger::writeErrorLog(NSString("Couldn't locate the <") +

                                 ioname + "> XML file at \""       +

                                 path   + "\" even with a look up");

    return false;

  }

  XMLElement  *child  = 0;

  XMLStack     xmlStack;



  if(!xmlStack.loadXMLFile(verified))

  {

	NatureScene::Logger::writeErrorLog(NSString("Invalid XML file -> ") + verified);

    return false;

  }


 //LOG_PRINT("ioname=%s\n",ioname.data);
  if(!(child = xmlStack.getChildByName(ioname)))

  {

	NatureScene::Logger::writeErrorLog(NSString("Need a <") + ioname + "> tag in the XML file");

    return false;

  }



  return loadXMLSettings(child);

}



bool IOXMLObject::exportXMLSettings(const char* xmlPath)

{

  ofstream xmlFile(xmlPath, ios::app | ios::binary);

  bool result = exportXMLSettings(xmlFile);

  xmlFile.close();

  return result;

}



bool IOXMLObject::isSuitable(XMLElement *element)

{

  if(!element)

  {

	NatureScene::Logger::writeErrorLog(NSString("0 <") + ioname +  "> node");

    return false;

  }



  if(element->getName() != ioname)

  {

	NatureScene::Logger::writeErrorLog(NSString("Need a <") + ioname + "> tag in the XML Node");

    return false;

  }



  return true;

}





