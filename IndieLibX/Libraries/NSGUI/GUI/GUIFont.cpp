#include <Framework3/IRenderer.h>
#include <STX/STXCPP.h>
#include <STX/CInput.h>
#include "GUIFont.h"

GUIFont::GUIFont(const char* name) : NamedObject(name)
{

}

bool GUIFont::build()
{
//  if(!getName().getBytes())
  //  return Logger::writeErrorLog("Null font path");

	std::string f2=stx_convertpath(getName().c_str());
	const char* path=f2.c_str();

	  MediaPathManager::registerPath(path);

  return font.load(path);
}

bool GUIFont::loadXMLSettings(XMLElement *element)
{
  //if(!element || element->getName() != "Font")
//    return Logger::writeErrorLog("Need a Font node in the xml file");

  XMLElement *child    = 0;

 if(child = element->getChildByName("path"))
{
    setName(child->getValuec());
}

  return true;
}

CFont *GUIFont::getFontObject()
{
  return &font;
}

bool  GUIFont::operator ==(GUIFont &compare)
{
  return (name   == compare.getName());
}

bool  GUIFont::operator !=(GUIFont &compare)
{
  return !operator ==(compare);
}

GUIFont::~GUIFont()
{
  font.~CFont();
}
