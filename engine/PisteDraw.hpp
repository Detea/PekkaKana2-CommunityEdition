//#########################
//PisteEngine - PisteDraw
//by Janne Kivilahti from Piste Gamez
//#########################

#pragma once

#include <SDL_surface.h>
#include <SDL_render.h>
#include <SDL_pixels.h>
#include <SDL_video.h>
#include <SDL_rect.h>

#include <vector>
#include <string>

#include "PisteFont.hpp"
#include "platform.hpp"

#include <SDL_image.h>

#define		PD_FADE_FAST   5
#define		PD_FADE_NORMAL 2
#define		PD_FADE_SLOW   1

#define		PD_FILTER_NEAREST  "0"
#define		PD_FILTER_BILINEAR "1"

using PD_RECT = SDL_Rect;

/*struct PD_RECT{
	DWORD x, y, w, h;
};*/

bool PisteDraw2_IsFading();
int PisteDraw2_FadeOut(int speed);
int PisteDraw2_FadeIn(int speed);
void PisteDraw2_RotatePalette(BYTE start, BYTE end);

int PisteDraw2_Image_New(int w, int h);
int PisteDraw2_Image_Load(std::string filename, bool getPalette = false);
int PisteDraw2_Image_Copy(int src_i, int dst_i);
int PisteDraw2_Image_Cut(int ImgIndex, int x, int y, int w, int h);
int PisteDraw2_Image_Cut(int ImgIndex, PD_RECT area);
int PisteDraw2_Image_Clip(int index, int x, int y);
int PisteDraw2_Image_ClipTransparent(int index, int x, int y, int alpha);
int PisteDraw2_Image_ClipTransparent(int index, int x, int y, int width, int height, int alpha);
int PisteDraw2_Image_CutClip(int index, int dstx, int dsty, int srcx, int srcy, int oikea, int ala); // Use the function with the SDL_Rect parameters instead
int PisteDraw2_Image_CutClip(int index, int dstx, int dsty, int srcx, int srcy, int oikea, int ala, int alpha);
int PisteDraw2_Image_CutClip(int index, PD_RECT srcrect, PD_RECT dstrect);
int PisteDraw2_Image_CutClipTransparent(int index, PD_RECT srcrect, PD_RECT dstrect, int alpha);
//void PisteDraw2_Image_CutClip(int index, PD_RECT srcRect, PD_RECT dstRect);
int PisteDraw2_Image_CutClipTransparent(int index, PD_RECT srcrect, PD_RECT dstrect, int alpha, int colorsum);
void PisteDraw2_Image_GetSize(int index, int& w, int& h);
int PisteDraw2_Image_FlipHori(int index);
int PisteDraw2_Image_Snapshot(int index);
int PisteDraw2_Image_Delete(int& index);

int PisteDraw2_ImageFill(int index, BYTE color);
int PisteDraw2_ImageFill(int index, int posx, int posy, int oikea, int ala, BYTE color);
int PisteDraw2_ScreenFill(BYTE color);
int PisteDraw2_ScreenFill(int posx, int posy, int width, int height, BYTE color);

void PisteDraw2_Fill_Rect(int posx, int posy, int width, int height, BYTE color);

void PisteDraw2_SetMask(int x, int y, int w, int h);

int PisteDraw2_DrawScreen_Start(BYTE *&pixels, DWORD &pitch);
int PisteDraw2_DrawScreen_End();
int PisteDraw2_DrawImage_Start(int index, BYTE *&pixels, DWORD &pitch);
int PisteDraw2_DrawImage_End(int index);
BYTE PisteDraw2_BlendColors(BYTE color, BYTE colBack,int alpha);

int PisteDraw2_Font_Create(int image, int x, int y, int width, int height, int count);
int PisteDraw2_Font_Create(char* path, char* file);
int PisteDraw2_Font_Write(int font_index, std::string text, int x, int y);
int PisteDraw2_Font_WriteAlpha(int font_index, std::string text, int x, int y, BYTE alpha);

int PisteDraw2_SetFilter(const char* filter);
void PisteDraw2_FullScreen(bool set);
void PisteDraw2_AdjustScreen();
void PisteDraw2_FitScreen(bool fit);
void PisteDraw2_ChangeResolution(int w, int h);

void PisteDraw2_GetWindowPosition(int* x, int* y);
SDL_Window* PisteDraw2_Get_Window();

int PisteDraw2_GetXOffset();
void PisteDraw2_SetXOffset(int x);
int PisteDraw2_Start(int width, int height, const char* name, const char* icon);
void PisteDraw2_Clear_Fonts();
int PisteDraw2_Exit();
void PisteDraw2_Update(bool draw);
void* PisteDraw2_GetRenderer();
