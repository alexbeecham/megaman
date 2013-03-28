#include "SDL/SDL.h"
#include <string>

const int Screen_Width = 300;
const int Screen_Height = 100;
const int Screen_BPP = 32;

SDL_Surface *load_image(std::string file) // this function loads a BMP image 
{
  SDL_Surface* loadedImage = NULL;
  SDL_Surface* optimizedImage = NULL;
  loadedImage = SDL_LoadBMP( file.c_str() ); // loads the image 
  if(loadedImage != NULL) // if the image loaded successfully
    {
      optimizedImage = SDL_DisplayFormat(loadedImage); // optimizes image
      SDL_FreeSurface(loadedImage); // frees memory
    }
  return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination) // this function applies the source image to the destination at x,y
{
  SDL_Rect offset; // holds location of image to be placed
  offset.x = x; // set x coordinate
  offset.y = y; // set y coordinate
  SDL_BlitSurface(source, NULL, destination, &offset); // place image
}

int main(int argc, char* args[])
{

  SDL_Surface *outerBox = NULL;
  SDL_Surface *innerBox = NULL;
  SDL_Surface *screen = NULL;
  
  screen = SDL_SetVideoMode(Screen_Width,Screen_Height, Screen_BPP, SDL_SWSURFACE); // creates the screen
  
  // load the outer shell and the inner boxes
  outerBox = load_image( "outerShell.bmp");
  innerBox = load_image( "innerBox.bmp");
  
  apply_surface(0,0, outerBox, screen); // apply the outer shell to the screen
  SDL_Flip(screen); // refresh
  SDL_Delay(1000); // wait 1 sec
  int i;
  for(i=0;i<11;i++) // places 11 solid blue boxes inside the outer shell
    {
      apply_surface(5+i*26,5,innerBox,screen); // apply image
      SDL_Flip(screen); // refresh
      SDL_Delay(500); // wait 1/2 sec
    }
  SDL_Delay(2000); // hold final image for 2 seconds
  SDL_FreeSurface(outerBox); // free all memory
  SDL_FreeSurface(innerBox);
  SDL_Quit();
  
  return 0;
}
