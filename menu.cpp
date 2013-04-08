/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <iostream>

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//keeps track of arrow
int arrowPos=0;

//determines which screen is being shown 0 for main menu 1 for options
int screenNum=0;

//The surfaces
SDL_Surface *background = NULL;
SDL_Surface *message = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *menu=NULL;
SDL_Surface *cont=NULL;
SDL_Surface *newGame=NULL;
SDL_Surface *options=NULL;
SDL_Surface *arrow=NULL;
//The event structure
SDL_Event event;

//The font
TTF_Font *font = NULL;

//The color of the font
SDL_Color textColor = { 0, 0, 255 };

//The music that will be played
Mix_Music *music = NULL;

//The sound effects that will be used
Mix_Chunk *scratch = NULL;
Mix_Chunk *jump = NULL;
Mix_Chunk *fire = NULL;
Mix_Chunk *low = NULL;


SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
	  Uint32 colorkey = SDL_MapRGB(optimizedImage->format,0,0xFF,0xFF);
	  SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Monitor Music", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the background image
    background = load_image( "background.png" );

    //Open the font
    font = TTF_OpenFont( "AiracobraItalic.ttf", 30 );

    //If there was a problem in loading the background
    if( background == NULL )
    {
        return false;
    }
    arrow = load_image("arrow.png");
    if(arrow==NULL)
      {
	return false;
      }
    //If there was an error in loading the font
    if( font == NULL )
    {
        return false;
    }

    //Load the music
    music = Mix_LoadMUS( "superstreetfighter2-guilesstage.wav" );

    //If there was a problem loading the music
    if( music == NULL )
    {
        return false;
    }

    //Load the sound effects
    scratch = Mix_LoadWAV( "scratch.wav" );
    jump = Mix_LoadWAV( "high.wav" );
    fire = Mix_LoadWAV( "medium.wav" );
    low = Mix_LoadWAV( "low.wav" );

    //If there was a problem loading the sound effects
    if( ( scratch == NULL ) || ( jump == NULL ) || ( fire == NULL ) || ( low == NULL ) )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( background );
    SDL_FreeSurface(arrow);
    //Free the sound effects
    Mix_FreeChunk( scratch );
    Mix_FreeChunk( jump );
    Mix_FreeChunk( fire );
    Mix_FreeChunk( low );

    //Free the music
    Mix_FreeMusic( music );

    //Close the font
    TTF_CloseFont( font );

    //Quit SDL_mixer
    Mix_CloseAudio();

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

void show_screen()
{
  apply_surface( 0, 0, background, screen );

    //Render the text
    message = TTF_RenderText_Solid( font, "Megaman", textColor );
    menu = TTF_RenderText_Solid(font, "Main Menu",textColor);
    cont=TTF_RenderText_Solid(font,"Continue",textColor);
    options=TTF_RenderText_Solid(font,"Options",textColor);
    newGame=TTF_RenderText_Solid(font,"New Game",textColor);

    //Show the message on the screen
    apply_surface( ( SCREEN_WIDTH - message->w ) / 2, 50, message, screen );
    apply_surface(( SCREEN_WIDTH - message->w)/2, 150,menu,screen);
    apply_surface((SCREEN_WIDTH-message->w)/4,250,cont,screen);
    apply_surface(3*(SCREEN_WIDTH-message->w)/4,250,newGame,screen);
    apply_surface((SCREEN_WIDTH - message->w)/2,350,options,screen);
    if(arrowPos==0) // arrow at continue
      {
	apply_surface((SCREEN_WIDTH-message->w)/2-200,250,arrow,screen);
      }
    else if(arrowPos==1) // arrow at new game
      {
	apply_surface(4*(SCREEN_WIDTH-message->w)/5-100,250,arrow,screen);
      }
    else if(arrowPos==2) //arrow at options
      {
	apply_surface(3*(SCREEN_WIDTH-message->w)/4-200,350,arrow,screen);
      }
    
    //Free the message
    SDL_FreeSurface(message);
    SDL_FreeSurface(menu);
    SDL_FreeSurface(cont);
    SDL_FreeSurface(options);
    SDL_FreeSurface(newGame);

    SDL_Flip(screen);
}

void show_options()
{
  apply_surface( 0, 0, background, screen );

    //Render the text
    message = TTF_RenderText_Solid( font, "Megaman", textColor );
    menu = TTF_RenderText_Solid(font, "Volume",textColor);
    cont=TTF_RenderText_Solid(font,"Upgrade",textColor);
    options=TTF_RenderText_Solid(font,"Options",textColor);
    newGame=TTF_RenderText_Solid(font,"Super Laser",textColor);

    //Show the message on the screen
    apply_surface( ( SCREEN_WIDTH - message->w ) / 2, 0, message, screen );
    apply_surface(( SCREEN_WIDTH - message->w)/2, 50,options,screen);
    apply_surface((SCREEN_WIDTH-message->w)/2,100,menu,screen);
    apply_surface(3*(SCREEN_WIDTH-message->w)/2,150,cont,screen);
    apply_surface((SCREEN_WIDTH - message->w)/4,200,newGame,screen);

    // reuse newGame to show a different message
    newGame=TTF_RenderText_Solid(font,"Double Jump",textColor);
    apply_surface(3*(SCREEN_WIDTH-message->w)/4,200,newGame,screen);

    // reuse newGame to show a third message
    newGame=TTF_RenderText_Solid(font,"Back",textColor);
    apply_surface((SCREEN_WIDTH-message->w)/2,250,newGame,screen);

    if(arrowPos==0)
      {
	apply_surface((SCREEN_WIDTH-message->w)/2-75,100,arrow,screen);
      }
    else if(arrowPos==1)
      {
	apply_surface((SCREEN_WIDTH-message->w)/4-50,200,arrow,screen);
      }
    else if(arrowPos==2)
      {
	apply_surface(3*(SCREEN_WIDTH-message->w)/4-50,200,arrow,screen);
      }
    else if(arrowPos==3)
      {
	apply_surface((SCREEN_WIDTH-message->w)/2-50,250,arrow,screen);
      }
    SDL_FreeSurface(message);
    SDL_FreeSurface(menu);
    SDL_FreeSurface(cont);
    SDL_FreeSurface(options);
    SDL_FreeSurface(newGame);

    SDL_Flip(screen);
}

void show_loading()
{
  apply_surface(0,0,background,screen);
  SDL_Flip(screen);
}

int main( int argc, char* args[] )
{
    //Quit flag
    bool quit = false;
    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    //Apply the background
    show_screen();
    //While the user hasn't quit
    while( quit == false )
    {
      if( Mix_PlayingMusic() == 0 )
	{
	  //Play the music
	  if( Mix_PlayMusic( music, -1 ) == -1 )
	    {
	      return 1;
	    }
	}
        //While there's events to handle
      while( SDL_PollEvent( &event ) )
	{
	  if(event.type==SDL_KEYDOWN)
	    {
	      if(event.key.keysym.sym==SDLK_w)
		{
		  Mix_PlayChannel(-1,jump,0);
		  if(arrowPos==2 && screenNum==0) // main menu is showing
		    {
		      arrowPos=0;
		    }
		  else if((arrowPos==1 || arrowPos==2 || arrowPos==3)&&screenNum==1) // options is showing
		    {
		      if(arrowPos==3)
			{
			  arrowPos-=2;
			}
		      else
			{
			  arrowPos=0;
			}
		    }
		  else
		    {
		      arrowPos=arrowPos;
		    }
		}
	      else if(event.key.keysym.sym==SDLK_d)
		{
		  Mix_PlayChannel(-1,fire,0);
		  if(arrowPos==0 && screenNum==0)
		    {
		      arrowPos=1;
		    }
		  else if(arrowPos==1 && screenNum==1)
		    {
		      arrowPos=2;
		    }
		  else
		    {
		      arrowPos=arrowPos;
		    }
		}
	      else if(event.key.keysym.sym==SDLK_a)
		{
		  if(arrowPos==1 && screenNum==0)
		    {
		      arrowPos=0;
		    }
		  else if(arrowPos==2 && screenNum==1)
		    {
		      arrowPos=1;
		    }
		  else
		    {
		      arrowPos=arrowPos;
		    }
		}
	      else if(event.key.keysym.sym==SDLK_s)
		{
		  if((arrowPos==0 || arrowPos==1) && screenNum==0)
		    {
		      arrowPos=2;
		    }
		  else if(arrowPos<3 && screenNum==1)
		    {
		      if(arrowPos==0)
			{
			  arrowPos++;
			}
		      else
			{
			  arrowPos=3;
			}
		    }
		  else
		    {
		      arrowPos=arrowPos;
		    }
		}
	      else if(event.key.keysym.sym==SDLK_e)
		{
		  if((arrowPos==0 || arrowPos==1) && screenNum==0)
		    {
		      screenNum=2;
		      quit=true;
		    }
		  else if(arrowPos==2 && screenNum==0)
		    {
		      arrowPos=0;
		      screenNum=1;
		    }	  
		  else if(arrowPos==3 && screenNum==1)
		    {
		      screenNum=0;
		      arrowPos=0;
		    }
		}

	    }
	  if(screenNum==0)
	    {
	      show_screen();
	    }
	  else if(screenNum==1)
	    {
	      show_options();
	    }
	  else
	    {
	      show_loading();
	    }
	}
      //If the user has Xed out the window
      if( event.type == SDL_QUIT )
	{
	  //Quit the program
	  quit = true;
	}
    }
    SDL_Delay(500);
    //Free surfaces, fonts and sounds
    //then quit SDL_mixer, SDL_ttf and SDL
    clean_up();

    return 0;
}
