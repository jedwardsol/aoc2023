#pragma once
#include <SDL2/SDL.h>
#include "thrower.h"

#include <string>
using namespace std::literals;


#if _DEBUG
#pragma comment(lib,"SDL2d")
#pragma comment(lib,"SDL2maind")
#else
#pragma comment(lib,"SDL2")
#pragma comment(lib,"SDL2main")
#endif

class Uncopyable
{
  public: 
    Uncopyable (const Uncopyable &) = delete;
    Uncopyable & operator = (const Uncopyable &) = delete;

  protected:
    Uncopyable () = default;
    ~Uncopyable () = default; 
};

// RAII wrappers,  that's all


struct SDL : Uncopyable
{
    SDL()
    {
        auto err = SDL_Init( SDL_INIT_VIDEO ) ;

        if(err < 0)
        {
            throw_runtime_error("SDL_Init : "s + SDL_GetError());
        }
    }

    ~SDL()
    {
        SDL_Quit();
    }
};         


struct SDLWindow : Uncopyable
{
    SDLWindow(std::string   const&title, int width, int height)
    {
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE |SDL_WINDOW_SHOWN);

        if( window == nullptr )
        {
            throw_runtime_error("SDL_CreateWindow : "s + SDL_GetError());
        }
    }

    ~SDLWindow()
    {
        SDL_DestroyWindow  (window);
    }

    operator SDL_Window *()
    {
        return window;
    }

private:
    SDL_Window *window{};
};

struct SDLRenderer : Uncopyable
{

    SDLRenderer(SDLWindow &window)
    {
        renderer   = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED  | SDL_RENDERER_PRESENTVSYNC );

        if( renderer == nullptr )
        {
            throw_runtime_error("SDL_CreateRenderer : "s + SDL_GetError());
        }
    }

    ~SDLRenderer()
    {
    	SDL_DestroyRenderer(renderer);
    }

    
    operator SDL_Renderer *()
    {
        return renderer;
    }

private:
    SDL_Renderer    *renderer{};
};



struct SDLTexture : Uncopyable
{

    SDLTexture(SDLRenderer &renderer,int width, int height)
    {
        texture    = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STREAMING,width,height);

        if (texture == nullptr) 
        {
            throw_runtime_error("SDL_CreateTexture : "s + SDL_GetError());
        }
    }

    ~SDLTexture()
    {
    	SDL_DestroyTexture(texture);
    }

    
    operator SDL_Texture *()
    {
        return texture;
    }

private:
    SDL_Texture    *texture{};
};


