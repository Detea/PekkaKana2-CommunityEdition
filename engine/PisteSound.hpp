//#########################
//PisteEngine - PisteSound
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef P_SOUND
#define P_SOUND

#include "platform.hpp"
#include <string>

int PisteSound_Start();
int PisteSound_Update();
int PisteSound_End();

int PisteSound_LoadSFX(std::string filename);
int PisteSound_PlaySFX(int index);
int PisteSound_PlaySFX(int index, int volume, int panoramic, int freq);
void PisteSound_SetSFXVolume(int volume);
int PisteSound_FreeSFX(int index);
void PisteSound_ResetSFX();

int PisteSound_StartMusic(char* filename);
void PisteSound_SetMusicVolume(int volume);
void PisteSound_StopMusic();

#endif
