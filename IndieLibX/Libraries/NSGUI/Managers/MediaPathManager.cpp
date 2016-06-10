#include "MediaPathManager.h"



vector<NSString> MediaPathManager::pathStack;



const char* MediaPathManager::lookUpMediaPath(const char *path)

{

  if(!path)

    return 0;



  ifstream test;

  string   buffer;

  std::string fn=stx_convertpath(path);

  //if(!fn.compare("GUILayout.xml"))
    //fn=stx_convertpath("/NatureScene/GUI/GUILayout.xml");

  LOG_PRINT("MediaPathManager::lookUpMediaPath:ifstream=%s\n", fn.c_str());

  

  test.open(fn.c_str());



  if(test.is_open())

  {

    test.close();

    return path;

  }



  size_t count = pathStack.size();

  char   *pathBuffer = new char[256];

  memset(pathBuffer, 0, 256);



  for(size_t i = 0; i < count; i++)

  {

    buffer  = pathStack[i];

    buffer += path;



    test.open(buffer.data());

    if(test.is_open())

    {

      stx_strlcpy(pathBuffer, buffer.data(),buffer.length()+1);

      test.close();

      return pathBuffer;

    }

  }

  deleteArray(pathBuffer);

  return 0;

}



bool MediaPathManager::registerPath( XMLElement *mediaPathNode)

{

  XMLElement *child;



  if(mediaPathNode)

  if(mediaPathNode->getName() == "MediaFolder")

  if(child = mediaPathNode->getChildByName("path"))
  {
	char buf[1024];
	stx_snprintf(buf,1024, "%s",child->getValuec());
    return  registerPath(buf);
  }


  return false;

}



bool MediaPathManager::registerPath(const char *path0)

{
	char path[MAX_PATH];
	std::string f=path0;
	stx_strlcpy(path, f.c_str(), MAX_PATH);
	if(stx_strcmp("Data/", path0)==-1)
	{
		stx_strlcat(path, "/NatureScene/", MAX_PATH);
		stx_strlcat(path, path0+4, MAX_PATH);
	}
	else if(stx_strcmp("/NatureScene/", path0)==-1)
		stx_strlcat(path, "/", MAX_PATH);

  if(!path || !stx_strlen(path))

    {NatureScene::Logger::writeErrorLog(NSString("Failed to register data path -> 0"));return false;}



  for(size_t i = 0; i < pathStack.size(); i++)

    if(pathStack[i] == path)

      return true;
	
  NSString stringBuffer = path;

  NatureScene::Logger::writeInfoLog(NSString("Registering data path -> ") + path);

  pathStack.push_back(stringBuffer);

  return true;

}



int MediaPathManager::getPathCount()

{

  return int(pathStack.size());

}



void  MediaPathManager::printAllPaths()

{

  cout << "List of registred Media Paths: \n";



  for(size_t i = 0; i < pathStack.size(); i++){

    if(pathStack[i]){

      cout << int(i) << "-" << pathStack[i] << endl;

    }

  }



  cout << endl;

}
