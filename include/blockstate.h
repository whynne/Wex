#pragma once

#include "EngineState.h"
#include "console.h"
#include "vec.h"



#define BLOCK_SIZE 24
#define NEXT_SIZE 16

#define PLAYSCREEN_LEFT 16
#define PLAYSCREEN_TOP 16

#define NEXT_PIECE_TOP 16
#define NEXT_PIECE_LEFT 350

#define PLAYFIELD_WIDTH 10
#define PLAYFIELD_HEIGHT 20

struct Block
{
	Animation animation;
	enum colors color;
	bool solid;
	void draw(Point2i position);
	Block(enum colors color, bool solid);
	Block();
};

class Piece
{
public:
	Vec2<int> position;
	enum piecetypes type;
	int  width;
	int  height;
	int  maxrotations;
	int  rotation;
	bool*  blockpattern;
	Block* blocks;
public: 
	void draw();
	void drawPreview(Point2i position);
	int  nextRotate();
	void rotate();
	Piece(enum piecetypes type,bool* pattern);
	Piece();
	~Piece();

};

enum piecetypes
{
	piece_O,
	piece_T,
	piece_J,
	piece_L,
	piece_S,
	piece_Z,
	piece_I
};

enum colors
{
	red,
	blue,
	magenta,
	orange,
	cyan,
	yellow,
	green
};


class BlockState : public EngineState
{
private:
	bool   gameover;
	double shadermult;
	int    wavyshader;
	int    normalshader;

 	Block  playfield[20][10];
	Piece* currentpiece;
	Piece* nextpiece;
	double defaultticktime;
	double ticktime;
	double accumulatedtime;
	int    score;

public:
	void init(Controller &usercontroller);
	void cleanup();
	void pause();
	void resume();
	void handleEvents();
	void update(double t,double dt);
	void draw();

	//game related functions
	void levelUp();
	bool isRowFull(int row);
	bool isRowCleared(int row);
	void clearAllRows();
	void moveRow(int src,int dst);
	void deleteRow(int row);
	void placePiece();
	bool canMovePiece(Vec2<int> newposition);
	bool movePiece(Vec2<int> newposition);
	bool canRotatePiece();
	void clearLines();
	BlockState();
};
