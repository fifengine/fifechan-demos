#include <fifechan.hpp>
#include <fifechan/sdl/sdlinput.hpp>
#include <fifechan/opengl/openglgraphics.hpp>
#include <fifechan/opengl/openglsdlimageloader.hpp>
#include <SDL/SDL.h>
#include <sstream>

/**
 * Simple circular container creating code.
 *
 * Test doesn't do any memory management in sake of
 * usability. (and my sanity :) )
 */

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char **argv)
{
    SDL_Surface *screen = NULL;
    fcn::SDLInput* input;
    fcn::OpenGLGraphics* graphics;
    fcn::OpenGLSDLImageLoader* imageLoader;
    fcn::ImageFont* font;
    fcn::Gui* gui;
    fcn::Container* top;
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);    
    
    screen = SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);
   
    try
    {
        imageLoader = new fcn::OpenGLSDLImageLoader;
        fcn::Image::setImageLoader(imageLoader);
        
        graphics = new fcn::OpenGLGraphics(SCREEN_WIDTH, SCREEN_HEIGHT);
        input = new fcn::SDLInput;
        
        font = new fcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        fcn::Widget::setGlobalFont(font);
        
        gui = new fcn::Gui;
        gui->setGraphics(graphics);
        gui->setInput(input);
        
        top = new fcn::Container;
        top->setDimension(fcn::Rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
        top->setOpaque(false);
        gui->setTop(top);
    
       
        fcn::CircularContainer* c = new fcn::CircularContainer(150, fcn::CircularContainer::PI / 5.0);
        c->setPosition((SCREEN_WIDTH / 2) - (c->getWidth() / 2), (SCREEN_HEIGHT / 2) - (c->getHeight() / 2));
 
        for(int i = 0; i < 20; i++)
        {
            std::stringstream s;
            s << "Button " << i;
            
            fcn::Button *btn = new fcn::Button(s.str());
            btn->setSize(50, 25);
            c->add(btn);
        }
        
        top->add(c);
        
        bool running = true;
        SDL_Event evt;
        while(running)
        {
            glClear(GL_COLOR_BUFFER_BIT);

            while(SDL_PollEvent(&evt))
            {
                if(evt.type == SDL_QUIT)
                {
                    running = false;
                }
                
                input->pushInput(evt);
            }
            
            gui->logic();
            gui->draw();
            SDL_GL_SwapBuffers();
        }
    }
    catch(const fcn::Exception& exc)
    {
        std::cerr << exc.getMessage() << "\n";
    }
    
    return 0;
}
