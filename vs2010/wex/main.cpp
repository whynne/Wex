#include "wex.h"
#include <string>

class TestState : public wex::EngineState
{
private:
	wex::graphics::Quad square;
	wex::graphics::Sprite sprite;
	public:                         // Clean up any resources here
		void pause(){};                              // Execute when game state pauses.
		void begin()
		{
		    square = Quad(100,100,ColorRGBA(0,1,1,1));
			square.setHeight(100);
			square.setWidth(100);
			square.setColor(ColorRGBA(0,0,1,1));
			soundmanager->loadSound("b7.ogg");
			soundmanager->playSound("b7.ogg");
		}                                            // Execute when game state starts.
		void end(){};                                // Execute when game state ends.
		void resume(){};                             // Execute when game state resumes.
		void handleEvents(){};                       // Execute before update loop.  Handle your events here.
		void update(double time,double deltatime){}; // Executes between draws
		void draw()
		{
			renderer->drawQuad(square,Point3f(100,100,0),1,1,0);			
			renderer->drawText("default font","helloooooooooooooooooo OOOOOOOOOOOOOOOO ASDASD",Point3f(100,100,0),ColorRGBA(1,1,1,1),10);
			renderer->drawBuffer();
		};                              //executes as often as possible
		TestState(){};
};

int main(int argc, char *argv[])
{
    wex::GameEngine engine;
	TestState* state = new TestState();
	engine.init();
	wex::graphics::printShaderList();
	wex::graphics::printTextureList();
	engine.pushState(state); 
	engine.run();
}

