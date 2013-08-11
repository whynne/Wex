#include "wex.h"
#include <string>

using wex::graphics::Quad;
using wex::graphics::Sprite;

class TestState : public wex::EngineState
{
private:
	Quad square;
	Sprite sprite;
	float x;
public:             
	void pause()
		{
		} 
	void begin()
		{
		    square = Quad(100,100,ColorRGBA(0,1,1,1));		
			wex::loadSound("b7.ogg");
			wex::playSound("b7.ogg");
		}                                            
	void end()
		{
		}                                
	void resume()
		{
		}                            
	void handleEvents()
		{
		}                      
	void update(double time,double deltatime)
		{
		}
	void draw()
	{
		wex::draw(square,Point3f(100,100,0));
		wex::drawBuffer();
	}
	TestState()
		{
		}
};

int main(int argc, char *argv[])
{
	TestState test;
	wex::init();
	wex::pushState(test);
	wex::run();
}

