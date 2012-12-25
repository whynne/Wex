#include "mainstate.h"
#include "mainstate.h"
#define vpos(h) h*SCREEN_HEIGHT
#define hpos(v) v*SCREEN_WIDTH

///////////////////////////////////////////////
//
//  MAIN MENU STATE - Pushed onto stack when 
//                    game first starts
//
///////////////////////////////////////////////

void MainMenuState::init(Controller &maincontrol)
{
  glClearColor(0,0,0,1);
  renderer->setDefaultRendering(true); 
}

void MainMenuState::cleanup()
{

}

void MainMenuState::pause()
{

}

void MainMenuState::resume()
{

}

void MainMenuState::handleEvents()
{

}

void MainMenuState::update(double t,double dt)
{
	
}

void MainMenuState::draw()
{
	renderer->changeTexture("uifont");
	renderer->drawText("uifont","Oh, yes, that's mature.  Blame me, why don't you.",Point3f(vpos(.5),hpos(.5),0),ColorRGBA(.9,.9,.9,1),8.0);
	renderer->drawBuffer();
}

MainMenuState::MainMenuState()
{

}

///////////////////////////////////////////////
//
//  MAIN GAME STATE - Pushed onto stack when 
//                    game is started/loaded
//
///////////////////////////////////////////////


void MainGameState::init(Controller &maincontrol)
{
  console.setColumns(95);
  console.setRows(34);
  console.print("Dragon dildos were first made available to the public on the adult sex toy website Bad-Dragon.[1] On December 7th, 2007, Bad-Dragon co-founder Varka announced plans to create a business involving the sale and manufacture of dragon-like sex toys on his website Herpy.net.[2] On June 26th, 2008, a sex shop based in Phoenix, Arizona launched the online shopping site Bad-Dragon.com, featuring a variety of sex toys including phalluses shaped to resemble the male genitals of dragons, horses, dolphins, orcas, canines wallabies and cephalopods.");
  console.setPosition(Point3f(170,170,0));
  glClearColor(0,0,0,1);
  renderer->setDefaultRendering(true); 
}

void MainGameState::cleanup()
{

}

void MainGameState::pause()
{

}

void MainGameState::resume()
{

}

void MainGameState::handleEvents()
{

}

void MainGameState::update(double t,double dt)
{
	
}

void MainGameState::draw()
{
	drawWindow(Point3f(10,10,0),150,150);
	drawWindow(Point3f(10,170,0),525,150);
	renderer->changeTexture("uifont");
	renderer->drawText("uifont","STAT: 120",Point3f(30,190,0),ColorRGBA(1,1,1,1),8.0);
	renderer->drawText("uifont","STAT: 120",Point3f(30,210,0),ColorRGBA(1,1,1,1),8.0);
	renderer->drawText("uifont","STAT: 120",Point3f(30,230,0),ColorRGBA(1,1,1,1),8.0);
	renderer->drawText("uifont","STAT: 120",Point3f(30,250,0),ColorRGBA(1,1,1,1),8.0);

	renderer->drawBuffer();
	console.draw();
}

MainGameState::MainGameState()
{

}

///////////////////////////////////////////////
//
//  BATTLE STATE - Pushed onto stack whenever a
//                 battle begins
//
///////////////////////////////////////////////


void BattleState::init(Controller &maincontrol)
{
  renderer->setDefaultRendering(true); 
}

void BattleState::cleanup()
{

}

void BattleState::pause()
{

}

void BattleState::resume()
{

}

void BattleState::handleEvents()
{

}

void BattleState::update(double t,double dt)
{
	
}

void BattleState::draw()
{

}

BattleState::BattleState()
{

}
