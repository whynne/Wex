#include "entity.h"

enum GuiMessages
{
	mouseover,
	mouseout,
	mousedown,
	mouseup,

};

class Clickable: public Entity
{
private:
	void   (*onMouseOver)();
	void   (*onMouseOut)();
	void   (*onMouseDown)();
	void   (*onMouseUp)();
public:

};

class GuiEvent
{
	int handle;
	int msg;
};