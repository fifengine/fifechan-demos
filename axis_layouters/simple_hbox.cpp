#include <fifechan.hpp>
#include <fifechan/sdl/sdlinput.hpp>
#include <fifechan/opengl/openglgraphics.hpp>
#include <fifechan/opengl/openglsdlimageloader.hpp>
#include <SDL/SDL.h>
#include <sstream>

/**
 * Simple hbox creating code.
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
    
        fcn::HBox *testHBox = new fcn::HBox(new fcn::FixedSizeConstraint(fcn::Size(0, 0), fcn::Size(400, 100)));

        fcn::Button* btn = new fcn::Button("Button 1");
        btn->adjustSize();
        testHBox->add(btn);
        
        testHBox->add(new fcn::Spacer(new fcn::FixedSizeConstraint(fcn::Size(5, 0))));
        
        fcn::Label* lbl = new fcn::Label("Label");
        lbl->adjustSize();
        testHBox->add(lbl);
        
        testHBox->add(new fcn::Spacer(new fcn::FixedSizeConstraint(fcn::Size(5, 0))));
        
        btn = new fcn::Button("Button 2");
        btn->adjustSize();
        testHBox->add(btn);
        
        testHBox->add(new fcn::Spacer(new fcn::FixedSizeConstraint(fcn::Size(5, 0))));
        
        fcn::TextBox* txtBox = new fcn::TextBox;
        txtBox->setSizeConstraint(new fcn::FixedSizeConstraint(fcn::Size(100, 20), fcn::Size(400, 100)));
        testHBox->add(txtBox);
        
        top->add(testHBox);
        
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
