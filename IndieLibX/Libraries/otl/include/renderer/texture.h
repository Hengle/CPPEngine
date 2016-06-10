/*
  Copyright (c) 2004 Pascal Kirchdorfer, Seth Yastrov

  This software is provided 'as-is', without any express or implied warranty. In
  no event will the authors be held liable for any damages arising from the use
  of this software.

  Permission is granted to anyone to use this software for any purpose, including
  commercial applications, and to alter it and redistribute it freely, subject to
  the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim
       that you wrote the original software. If you use this software in a product,
       an acknowledgment in the product documentation would be appreciated but is
       not required.

    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _FOREST_TEXTURE_H_
#define _FOREST_TEXTURE_H_

/// \addtogroup Forest
/// @{

class TextureOTL
{
private:

public:
  char* name;
  char* name2;
  int width, height;
  int id;

  TextureOTL() : name(0), name2(0) {setName2("TexDiffuse");}
  ~TextureOTL() { delete [] name; delete [] name2;}

  const char* getName()
  {
    return name;
  }

  void setName(const char* name)
  {
    this->name = new char[strlen(name)+1];
    strcpy(this->name,name);
  }



  const char* getName2()
  {
    return name2;
  }

  void setName2(const char* name)
  {
    this->name2 = new char[strlen(name)+1];
    strcpy(this->name2,name);
  }
};

/// }@

#endif // _FOREST_TEXTURE_H_
