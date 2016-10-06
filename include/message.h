
#pragma once

#include "actor.h"

enum MessageTypes
{
	LOOK,
	EXAMINE,

};

class Message
{
private:
	Actor* sender;
private:
	virtual void operate(Actor* actor);
};

class AttackMessage
{
};

class SpellMessage
{
};

class TalkMessage
{
};