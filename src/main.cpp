#include "wex.h"
#include <string>

class TestState : public wex::EngineState
{
private:
	float rot;
	wex::graphics::Quad square;
	wex::graphics::Quad square2;
	wex::graphics::Quad square3;
	wex::graphics::Sprite sprite;
	float pos_x;
	float pos_y;
	public:                         // Clean up any resources here
		void pause(){};                              // Execute when game state pauses.
		void begin()
		{
			rot = 0.0;
		    square = Quad(1,1,ColorRGBA(0,0,0,1));
		    square2 = Quad(1,1,ColorRGBA(0,0,0,1));
		    square3 = Quad(1,1,ColorRGBA(0,0,0,1));
			square.setColor(ColorRGBA(0,1,0,1));
			square2.setColor(ColorRGBA(0,0,1,1));
			square3.setColor(ColorRGBA(1,0,0,1));
		}                                            // Execute when game state starts.
		void end(){};                                // Execute when game state ends.
		void resume(){};                             // Execute when game state resumes.
		void handleEvents(){};                       // Execute before update loop.  Handle your events here.
		void update(double time,double deltatime)
		{
			rot += 10.0 * deltatime;
		}; // Executes between draws
		void draw()
		{
			engine->controller.updateMouse();
			pos_x = engine->controller.getMousePos().x;
			pos_y = engine->controller.getMousePos().y;
			renderer->drawQuad(square,Point3f(pos_x+100,pos_y,0),pos_y*1.0,pos_y*1.0,rot);
			renderer->drawQuad(square2,Point3f(pos_x,pos_y,-.01),pos_y*1.0,pos_y*1.0,rot*.9);	
			renderer->drawQuad(square3,Point3f(pos_x-100,pos_y,-.02),pos_y*1.0,pos_y*1.0,rot*.8);				
			renderer->drawText("default font","Scale 1",Point3f(100,100,0),ColorRGBA(1,1,1,1),8,1.0);
			renderer->drawText("default font","Scale 2",Point3f(100,200,0),ColorRGBA(1,1,1,1),8*2,2.0);
			renderer->drawText("default font","Scale 3",Point3f(100,300,0),ColorRGBA(1,1,1,1),8*3,3.0);
			renderer->drawText("default font",std::to_string(engine->t).c_str(),Point3f(100,400,0),ColorRGBA(1,1,1,1),8*10,10.0);
			renderer->drawBuffer();
		};                              //executes as often as possible
		TestState(){};
};

int main(int argc, char *argv[])
{
    wex::GameEngine* engine = wex::GameEngine::Instance();
	TestState* state = new TestState();
	engine->init();
	wex::graphics::printShaderList();
	wex::graphics::printTextureList();
	engine->pushState(state); 
	engine->run();
}

