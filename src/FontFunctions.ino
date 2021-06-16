#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/varela_regular10pt7b.h>
#include <Fonts/varela_regular20pt7b.h>
#include <Fonts/Futuri_Condensed_Light_Oblique40pt7b.h>
#include <Fonts/Futuri_Condensed_Light_Oblique60pt7b.h>

#define NUMFONTS 6
typedef struct _FontMap
{
  char *name;
  GFXfont *font;
} FontMap;

static FontMap fmap[NUMFONTS] = {
  {"FreeMono",(GFXfont *)(&FreeMono9pt7b)},
  {"FreeSans",(GFXfont *)(&FreeSans9pt7b)},
  {"Varela10",(GFXfont *)(&varela_regular10pt7b)},
  {"Varela20",(GFXfont *)(&varela_regular20pt7b)},
  {"Futuri60",(GFXfont *)(&Futuri_Condensed_Light_Oblique60pt7b)},
  {"Futuri40",(GFXfont *)(&Futuri_Condensed_Light_Oblique40pt7b)}
  };

bool CompareFontNames(char *name, char *compare)
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
  
  for (int i = 0 ; i < NUMFONTS ; i++)
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
