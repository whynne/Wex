#include "mainstate.h"

void MainState::init(Controller &maincontrol)
{

  glClearColor(1,1,1,1);
  mainconsole.setPosition(Point3f(20,180,0));
  mainconsole.setRows(15);
  mainconsole.setColumns(58);
  mainconsole.setFont("uifont");
  mainconsole.setSpacing(9.0);

  mainconsole.print("A container called a birds nest is here. On the branch is a small birds nest. A closed unopenable container called a jewel-encrusted egg is in the nest. The initial appearance of the jewel-encrusted egg is. In the bird's nest is a large egg encrusted with precious jewels, apparently scavenged somewhere by a childless songbird. The egg is covered with fine gold inlay and ornamented in lapis lazuli and mother-of-pearl. Unlike most eggs, this one is hinged and has a delicate looking clasp holding it closed. The egg appears extremely fragile. The description of the egg is. The egg is covered with fine gold inlay and ornamented in lapis lazuli and mother-of-pearl. Unlike most eggs, this one is hinged and has a delicate looking clasp holding it closed. The egg appears extremely fragile.");
  
  renderer->setDefaultRendering(true);
  
}

void MainState::cleanup()
{

}

void MainState::pause()
{

}

void MainState::resume()
{

}

void MainState::handleEvents()
{

}

void MainState::update(double t,double dt)
{
	
}

void MainState::draw()
{
	mainconsole.draw();
}

MainState::MainState()
{

}
