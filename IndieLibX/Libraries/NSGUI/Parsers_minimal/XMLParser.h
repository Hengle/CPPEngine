#pragma once

#if 1
class RawData
{
  public:
    bool  skipDelete;
    char *data;
    int   byteCount;
    RawData(){}
    RawData(const RawData &copy){}
    RawData &operator=(const RawData &copy){}
   ~RawData(){}
    void destroy(){}
};
#endif

struct XMLElement
{
  private:
	NSString  name;
    NSString  value;
  public:
	const int      getChildrenCount(){return 0;}
    XMLElement *getChild(size_t index){return 0;}
	NSString  &getName(){return name;}
	NSString  &getValue (){return value;}
    const char*    getValuec(){return 0;}
    const float    getValuef(){return 0.0f;}
    const int      getValuei(){return 0;}
	XMLElement* getChildByName(const char* n){return 0;}
	RawData rawData;
};

struct IOXMLObject
{
	IOXMLObject(const char* n=0){}
	virtual bool loadXMLSettings(const char *path  ){return true;}
	bool    isSuitable(XMLElement *element){return true;}
};
struct XMLStack 
{
	int    loadXMLFile(const char *xmlFilePath){return 0;}
	const int      getChildrenCount(){return 0;}
    XMLElement *getChild(size_t index){return 0;}
};

