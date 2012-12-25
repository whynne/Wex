#pragma once

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

namespace game
{
	class Verb
	{
	};

	class GameObject;
	//Contains all active game objects
	map<int,GameObject*> world_objects;
	
	class GameObject
	{
	private:
		static int count;
		int id;
		string name;
		vector<int> owned_objects;
	public:
		void doVerb();
		int getId();
		int getNumObjects();
		string getName();
	
		GameObject();
		~GameObject();
	};

	class Creature : public GameObject
	{

	};

	class NPC : public Creature
	{
	};

	class PC : public Creature
	{
	};
};