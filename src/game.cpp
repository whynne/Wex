#include "game.h"

int GameObject::getNumObjects()
{
}

string GameObject::getName()
{
}

int GameObject::getId()
{
}

string GameObject::getDescription()
{
}

GameObject::GameObject()
{
	GameObject::count++;
}

GameObject::~GameObject()
{
	GameObject::count--;
}
