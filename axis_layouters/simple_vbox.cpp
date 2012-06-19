#include <fifechan.hpp>
#include <fifechan/sdl/sdlinput.hpp>
#include <fifechan/opengl/openglgraphics.hpp>
#include <fifechan/opengl/openglsdlimageloader.hpp>
#include <SDL/SDL.h>
#include <sstream>
#include <vector>

/**
 * Simple VBox creating code.
 *
 * Test doesn't do any memory management in sake of
 * usability. (and my sanity :) )
 */

class MyListModel : public fcn::ListModel
{
    std::vector<std::string> mOptions;
    
public:
    
    MyListModel()
    {
        mOptions.push_back("Option 1");
        mOptions.push_back("Option 2");
        mOptions.push_back("Option 3");
        mOptions.push_back("Option 4");
    }
    
    ~MyListModel()
    {
    }
    
    virtual int getNumberOfElements()
    {
        return mOptions.size();
    }
    
    virtual std::string getElementAt(int i)
    {
        if(i >= mOptions.size())
            return "";
        
        return mOptions[i];
    }
};

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
    
        fcn::VBox *testVBox = new fcn::VBox(new fcn::FixedSizeConstraint(fcn::Size(0, 0), fcn::Size(400, 400)));

        for(int i = 0; i < 3; ++i)
        {
            std::stringstream s;
            s << "Button " << i;
            
            fcn::Button* btn = new fcn::Button(s.str());
            btn->adjustSize();
            testVBox->add(btn);
        }
        
        fcn::DropDown *dropDown = new fcn::DropDown(new MyListModel());
        dropDown->adjustHeight();
        testVBox->add(dropDown);
        
        for(int i = 3; i < 6; ++i)
        {
            std::stringstream s;
            s << "Button " << i;
            
            fcn::Button* btn = new fcn::Button(s.str());
            btn->adjustSize();
            testVBox->add(btn);
        }
        
        top->add(testVBox);
        
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
                else if(evt.type == SDL_KEYDOWN)
                {
                    if(evt.key.keysym.sym == SDLK_F1)
                    {
                        testVBox->relayout();
                    }
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
