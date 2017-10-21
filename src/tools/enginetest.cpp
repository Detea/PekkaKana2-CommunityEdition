#include "PisteEngine.h"

bool running = true;
int buffer, buffer2, image;
float x = 0, y = 30;
int font;

int Main_Loop(){
  int i;
  x++;
  int a = (int)x%100;
  PisteDraw2_Font_Write(font, "pekka kana", 90, 100);
  if(x>400) {PisteDraw2_FadeIn(5); x = 30;}
  if (((int)x/40)%2==1) PisteDraw2_Image_Clip(buffer2, 30, 30);
  else PisteDraw2_Image_Clip(buffer, 30, 30);

  PisteDraw2_Image_ClipTransparent(buffer, x, x, a);


  PD_RECT srcrect, dstrect;
  srcrect.x = 5;
  srcrect.y = 5;
  srcrect.w = 5;
  srcrect.h = 5;

  dstrect.x = x+20;
  dstrect.y = x+20;

  //PisteDraw2_Image_CutClipTransparent(buffer2,srcrect,dstrect,100-a);

}

int main(){
  Piste_Init(600,480,"Test");
  Piste_SetFPS(60);

  image = PisteDraw2_Image_Load("../res/gfx/pk2stuff.bmp", true);

  PD_RECT area = {213, 46, 25, 34};
  buffer = PisteDraw2_Image_Cut(image, area);
  area = {185, 46, 25, 34};
  buffer2 = PisteDraw2_Image_Cut(image, area);
  PisteDraw2_Image_FlipHori(buffer2);
  font = PisteDraw2_Font_Create(image,0,268,15,21,42);

  PisteDraw2_Image_Delete(image);

  Piste_Loop(running, *Main_Loop);

  Piste_Quit();
  return 0;
}
