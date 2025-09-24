#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include "fonts/futuricondensedlight60pt7b.h"
#include "fonts/futuricondensedlight40pt7b.h"
#include "fonts/Futuri_Condensed_Regular80pt7b.h"
#include "fonts/Varela_Regular10pt7b.h"
#include "fonts/Varela_Regular20pt7b.h"
#include <fonts/FreeMono24pt7b.h>

typedef struct _FontMap
{
  const char *name;
  GFXfont *font;
} FontMap;

static FontMap fmap[] = {
  {"FreeMono",(GFXfont *)(&FreeMono9pt7b)},
  {"FreeMono24",(GFXfont *)(&FreeMono24pt7b)},
  {"FreeSans",(GFXfont *)(&FreeSans9pt7b)},
  {"Varela10",(GFXfont *)(&Varela_Regular10pt7b)},
  {"Varela20",(GFXfont *)(&Varela_Regular20pt7b)},
  {"Futuri80",(GFXfont *)(&Futuri_Condensed_Regular80pt7b)},
  {"Futuri60",(GFXfont *)(&futuricondensedlight60pt7b)},
  {"Futuri40",(GFXfont *)(&futuricondensedlight40pt7b)}
  };

bool CompareFontNames(const char *name, char *compare)
{
  int namelen = strlen(name);
  int comparelen = strlen(compare);
  if (namelen < comparelen)
  {
    return false;
  }
  
  for (int i = 0; i < namelen; i++)
  {
    if (compare[i] != name[i]) return false;
  }

  return true;
}
GFXfont *GetFont(char *name)
{
  DEBUG_PRINT ("Looking for font named ");
  DEBUG_PRINTLN(name);

  int numfonts = sizeof(fmap) / sizeof(FontMap);
  
  for (int i = 0 ; i < numfonts ; i++)
  {
    DEBUG_PRINTLN (fmap[i].name);
    
    if (CompareFontNames(fmap[i].name,name) == true)
    {
      return fmap[i].font;  
    }
  }

  DEBUG_PRINTLN("Not found");
  return NULL;
}
