#include <fifechan.hpp>
#include <fifechan/sdl/sdlinput.hpp>
#include <fifechan/opengl/openglgraphics.hpp>
#include <fifechan/opengl/openglsdlimageloader.hpp>
#include <SDL/SDL.h>
#include <sstream>

/**
 * Click the buttons on the containers' header in order
 * to remove them and see how the layout adapts iself.
 *
 * Test doesn't do any memory management in sake of
 * usability. (and my sanity :) )
 */

class MyActionListener : public fcn::ActionListener
{
    fcn::Container* mParent;
    
public:
    MyActionListener(fcn::Container *parent)
    { 
        mParent = parent; 
    }
    
    ~MyActionListener(){}
    
    virtual void action(const fcn::ActionEvent& e)
    {
        fcn::Widget* w = e.getSource();
        
        if(w->getParent() == mParent)
        {
            mParent->remove(w);
        }
        
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
    
        fcn::VBox *testVBox = new fcn::VBox(new fcn::FixedSizeConstraint(fcn::Size(0, 0), fcn::Size(1000, 400)));
        fcn::HBox *testHBox = new fcn::HBox(new fcn::FixedSizeConstraint(fcn::Size(0, 0), fcn::Size(1000, 100)));
        
        fcn::Button *btn;
        for(int i = 0; i < 4; ++i)
        {
            std::stringstream s;
            s << "Button " << i;

            btn = new fcn::Button(s.str());
            btn->adjustSize();
            btn->addActionListener(new MyActionListener(testHBox));
            btn->setSizeConstraint(new fcn::FixedSizeConstraint(fcn::Size(0, 0), fcn::Size(100, 100)));
            testHBox->add(btn);
        }

        testVBox->add(testHBox);
        
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
    
        fcn::Window *window = new fcn::Window("Window");
        window->add(testVBox);
        window->resizeToContent();
    
        top->add(window);
    
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
    catch(const fcn::Exception& e)
    {
        std::cerr << e.getMessage() << "\n";
    }
    
    return 0;
}
