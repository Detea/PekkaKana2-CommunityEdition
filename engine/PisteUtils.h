//#########################
//PisteEngine - PisteUtils
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef PISTELUTILS
#define PISTELUTILS

#include "platform.h"

bool PisteUtils_CheckFile(char *filename);
int PisteUtils_AlphaSort(char *a, char *b);
void  PisteUtils_Lower(char* string);
void  PisteUtils_RemoveSpace(char* string);
char *PisteUtils_FindImage(char *filename);
int   PisteUtils_CreateDir(char *directory);

//type:
// "/" - directory
// ".exe" - *.exe
int  PisteUtils_Scandir(const char* type, char* dir, char (*list)[_MAX_PATH], int length);

#endif
