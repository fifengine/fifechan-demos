#include <fifechan.hpp>
#include <fifechan/sdl/sdlinput.hpp>
#include <fifechan/opengl/openglgraphics.hpp>
#include <fifechan/opengl/openglsdlimageloader.hpp>
#include <SDL/SDL.h>
#include <sstream>

/**
 * See how an hbox is arranged into a vbox.
 *
 * Test doesn't do any memory management in sake of
 * usability. (and my sanity :) )
 */

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
        
        graphics = new fcn::OpenGLGraphics(800, 600);
        input = new fcn::SDLInput;
        
        font = new fcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        fcn::Widget::setGlobalFont(font);
        
        gui = new fcn::Gui;
        gui->setGraphics(graphics);
        gui->setInput(input);
        
        top = new fcn::Container;
        top->setDimension(fcn::Rectangle(0, 0, 800, 600));
        top->setOpaque(false);
        gui->setTop(top);
    
        fcn::VBox *testVBox = new fcn::VBox(new fcn::FixedSizeConstraint(fcn::Size(0, 0), fcn::Size(400, 400)));
        fcn::HBox *testHBox = new fcn::HBox(new fcn::FixedSizeConstraint(fcn::Size(0, 0), fcn::Size(400, 100)));
        
        fcn::Button *btn;

        for(int i = 0; i < 2; ++i)
        {
            std::stringstream s;
            s << "Button " << i;

            btn = new fcn::Button(s.str());
            btn->adjustSize();
            testHBox->add(btn);
        }

        testVBox->add(testHBox);
        
        top->add(testVBox);
    

        fcn::Label *lbl;
        for(int i = 0; i < 5; ++i)
        {
            std::stringstream s;
            s << "Label " << i;

            lbl = new fcn::Label(s.str());
            lbl->adjustSize();
            testVBox->add(lbl);
        }
        
        btn = new fcn::Button("Another Button");
        btn->adjustSize();
        testVBox->add(btn);
        
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
