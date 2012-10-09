#include "blockstate.h"

#define RENDERER graphics::Renderer::Instance()


bool O_pattern [1][2][2] = {1,1,
                            1,1};

bool L_pattern [4][3][3] = {0,1,0,
                            0,1,0,
					    	0,1,1,
							
                            0,0,0,     
							1,1,1,  
							1,0,0,
							
                            1,1,0,
							0,1,0,
							0,1,0,
							
							0,0,1,
							1,1,1,
							0,0,0};

bool J_pattern [4][3][3] = {0,1,0,
                            0,1,0,
					    	1,1,0,

                            1,0,0,     
							1,1,1,
							0,0,0,

                            0,1,1,
							0,1,0,
							0,1,0,

							0,0,0,
							1,1,1,
							0,0,1};

bool S_pattern [2][3][3] = {0,0,0,
                            0,1,1,
							1,1,0,
							
							0,1,0,
							0,1,1,
							0,0,1};

bool Z_pattern [2][3][3] = {0,0,0,
                            1,1,0,
                            0,1,1,

							 0,1,0,
							 1,1,0,
							 1,0,0};

bool I_pattern [2][4][4] = {0,0,1,0,
                             0,0,1,0,
                             0,0,1,0,
						     0,0,1,0,
						    
						     0,0,0,0,
						     0,0,0,0,
						     1,1,1,1,
						     0,0,0,0};

bool T_pattern [4][3][3] = {0,0,0,
							 1,1,1,
							 0,1,0,
							 
							 0,1,0,
							 1,1,0,
							 0,1,0,
							 
							 0,1,0,
							 1,1,1,
							 0,0,0,
							 
							 0,1,0,
							 0,1,1,
							 0,1,0};



void BlockState::init(Controller &usercontroller)
{
	gameover = false;
	score = 0;
	shadermult = 1;
	defaultticktime = 1.0;
	ticktime = defaultticktime;
	_maincontrol = &usercontroller;
	normalshader = RENDERER->loadShader("normal.vert","normal.frag");
	wavyshader = RENDERER->loadShader("normal.vert","wavy.frag");
	cout << "normal shader: " << normalshader << endl;
	cout << "wavy shader: " << wavyshader << endl;


	cout << "Made new piece" << endl;
	nextpiece = 0;
	currentpiece = 0;
	accumulatedtime = 0;

	switch(rand()%7+1)
	{
	case 1:
		currentpiece = new Piece(piece_O,&O_pattern[0][0][0]);
	    break;
	case 2:
		currentpiece = new Piece(piece_I,&I_pattern[0][0][0]);
		break;
	case 3:
		currentpiece = new Piece(piece_J,&J_pattern[0][0][0]);
		break;
	case 4:
		currentpiece = new Piece(piece_T,&T_pattern[0][0][0]);
		break;
	case 5:
		currentpiece = new Piece(piece_L,&L_pattern[0][0][0]);
		break;
	case 6:
		currentpiece = new Piece(piece_S,&S_pattern[0][0][0]);
		break;
	case 7:
		currentpiece = new Piece(piece_Z,&Z_pattern[0][0][0]);
		break;
	}

	switch(rand()%7+1)
	{
	case 1:
		nextpiece = new Piece(piece_O,&O_pattern[0][0][0]);
	    break;
	case 2:
		nextpiece = new Piece(piece_I,&I_pattern[0][0][0]);
		break;
	case 3:
		nextpiece = new Piece(piece_J,&J_pattern[0][0][0]);
		break;
	case 4:
		nextpiece = new Piece(piece_T,&T_pattern[0][0][0]);
		break;
	case 5:
		nextpiece = new Piece(piece_L,&L_pattern[0][0][0]);
		break;
	case 6:
		nextpiece = new Piece(piece_S,&S_pattern[0][0][0]);
		break;
	case 7:
		nextpiece = new Piece(piece_Z,&Z_pattern[0][0][0]);
		break;
	
	}

	currentpiece->position = (Point2i(5,0));

	for(int y = 0; y < PLAYFIELD_HEIGHT;y++)
	{
		for(int x = 0; x < PLAYFIELD_WIDTH; x++)
		{
			playfield[y][x] = Block();
		}
	}
}

void BlockState::cleanup()
{

}

void BlockState::pause()
{

}

void BlockState::resume()
{

}

void BlockState::handleEvents()
{
	if(_maincontrol->keyPressed(SDLK_RIGHT))
	{
	    movePiece(Vec2<int>(currentpiece->position._x+1,currentpiece->position._y));
	}
	if(_maincontrol->keyPressed(SDLK_LEFT))
	{
	    movePiece(Vec2<int>(currentpiece->position._x-1,currentpiece->position._y));
	}
	if(_maincontrol->keyPressed(SDLK_UP) || _maincontrol->keyPressed(SDLK_SPACE))
	{
		if(canRotatePiece())
		    currentpiece->rotate();
	}
	if(_maincontrol->keyPressed(SDLK_DOWN))
	{
		ticktime = .1;
	}
	if(_maincontrol->keyReleased(SDLK_DOWN))
	{
		ticktime = defaultticktime;
	}
}

void BlockState::update(double t,double dt)
{
	

	glUseProgramObjectARB(wavyshader);
    int intensity = glGetUniformLocationARB(wavyshader,"intensity");
    glUniform1fARB(intensity, (float)(t)/shadermult);

	if(gameover == false)
	{
	   accumulatedtime += dt;
	}
	else
	{
		accumulatedtime = 0;
	}

	if (accumulatedtime >= ticktime)
	{
		if(movePiece(Vec2<int>(currentpiece->position._x,currentpiece->position._y+1))==false)
		{
            placePiece();
			delete currentpiece;
			currentpiece = nextpiece;
			nextpiece = 0;
			clearAllRows();
			currentpiece->position = (Point2i(5,0));
		}

		if(movePiece(Point2i(currentpiece->position._x,currentpiece->position._y)) == false)
		{
			cout << "game over" << endl;
			gameover = true;
		}


		if(nextpiece==0)
		{
			switch(rand()%7+1)
			{
			case 1:
				nextpiece = new Piece(piece_O,&O_pattern[0][0][0]);
			    break;
			case 2:
				nextpiece = new Piece(piece_I,&I_pattern[0][0][0]);
				break;
			case 3:
				nextpiece = new Piece(piece_J,&J_pattern[0][0][0]);
				break;
			case 4:
				nextpiece = new Piece(piece_T,&T_pattern[0][0][0]);
				break;
			case 5:
				nextpiece = new Piece(piece_L,&L_pattern[0][0][0]);
				break;
			case 6:
				nextpiece = new Piece(piece_S,&S_pattern[0][0][0]);
				break;
			case 7:
				nextpiece = new Piece(piece_Z,&Z_pattern[0][0][0]);
				break;

			}

			nextpiece->position = (Point2i(5,0));


		}
		accumulatedtime = 0;
	}
	
}

void BlockState::draw()
{	
	glUseProgram(wavyshader);
	glBindTexture(GL_TEXTURE_2D,1);
	RENDERER->addFixedStaticSpriteToBuffer(graphics::BACKGROUND_L1,512,512,Point3d(0,0,0));
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	RENDERER->drawBuffer();

	glUseProgram(normalshader);
	glBindTexture(GL_TEXTURE_2D,2);
	RENDERER->addFixedStaticSpriteToBuffer(graphics::BACKGROUND_L2,512,512,Point3d(0,0,0));
	RENDERER->drawBuffer();

	//Draw the blocks on the playfield first

	glBindTexture(GL_TEXTURE_2D,3);

	for(int y = 0; y < 20;y++)
	{
		for(int x = 0; x < 10; x++)
		{
			playfield[y][x].draw(Point2i(PLAYSCREEN_LEFT+(x*BLOCK_SIZE),PLAYSCREEN_TOP+(y*BLOCK_SIZE)));
		}
	}

    //Then draw the blocks from the piece we're currently moving
	currentpiece->draw();

	//Draw the next piece
	nextpiece->drawPreview(Point2i(350,16));
	

	RENDERER->drawBuffer();
	
	//Draw the score

	stringstream scorestream;
	string scorestring;
	scorestream << score;
	scorestream >> scorestring;
	glBindTexture(GL_TEXTURE_2D,4);
	RENDERER->addTextToBuffer("Score ",Point3d(340,115,0),10,ColorRGBA(1,1,1,1));
	RENDERER->addTextToBuffer(scorestring,Point3d(375,130,0),10,ColorRGBA(1,1,1,1));

	// the following code draws a triangle

	//If we gameovered, let the player know.
	if(gameover)
	RENDERER->addTextToBuffer("GAME OVER",Point3d(95,256,0),10,ColorRGBA(1,1,1,1));
	
	RENDERER->drawBuffer();
}


void Piece::rotate()
{
	rotation = nextRotate();
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			blocks[(y*height)+x].solid = blockpattern[((rotation*height)+y) * (height) + x];
		}
	}
}



Piece::Piece(enum piecetypes type, bool* pattern)
{
    colors color;
	blockpattern = pattern;
	rotation = 0;

	switch(type)
	{
	case piece_O:
		width  = 2;
		height = 2;
		color = red;
		maxrotations = 1;
		break;
	case piece_T:
		width  = 3;
		height = 3;
		color = blue;
		maxrotations = 4;
		break;
	case piece_J:
		width  = 3;
		height = 3;
		color = magenta;
		maxrotations = 4;
		break;
	case piece_L:
		width  = 3;
		height = 3;
		color = yellow;
		maxrotations = 4;
	    break;
	case piece_S:
		width  = 3;
		height = 3;
		color = green;
		maxrotations = 2;
		break;
	case piece_Z:
		width  = 3;
		height = 3;
		color = cyan;
		maxrotations = 2;
	    break;
	case piece_I:
		width  = 4;
		height = 4;
		color = orange;
		maxrotations = 2;
	}

	blocks = new Block[height*width];
	
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			blocks[(y*height)+x] = Block(color,blockpattern[(y*height)+x]);

		}
	}
}

Piece::Piece()
{
}


bool BlockState::movePiece(Vec2<int> newposition)
{
	if(canMovePiece(newposition))
	{
		currentpiece->position = newposition;
		return true;
	}
	else
	{
		cout << "collision detected" << endl;
		return false;
	}
}

bool BlockState::canRotatePiece()
{
	for(int y = 0; y < currentpiece->height; y++)
	{
		for(int x = 0; x < currentpiece->width; x++)
		{
			if(!(currentpiece->blockpattern[((currentpiece->nextRotate()*currentpiece->height)+y) * (currentpiece->height) + x]))
				continue;
			else
			{
				if(currentpiece->position._y+y > 19 || currentpiece->position._y+y < 0 || 
					currentpiece->position._x+x > 9 || currentpiece->position._x+x < 0)
					return false;
			}
		
			
			if((playfield[currentpiece->position._y + y][currentpiece->position._x + x].solid) 
				&& (currentpiece->blockpattern[((currentpiece->nextRotate()*currentpiece->height)+y) * (currentpiece->height) + x]))
                return false;
			
		}
	}
	return true;
}



void BlockState::clearLines()
{
}

bool BlockState::canMovePiece(Vec2<int> newposition)
{
	for(int y = 0; y < currentpiece->height; y++)
	{
		for(int x = 0; x < currentpiece->width; x++)
		{
			if(newposition._x+x > 9 || newposition._x+x < 0)
			{
				if(currentpiece->blocks[(y*currentpiece->height)+x].solid)
				{
					return false;
				}
				else
			        continue;
			}
			if(newposition._y+y > 19 || newposition._y+y < 0)
			{
				if(currentpiece->blocks[(y*currentpiece->height)+x].solid)
				{
					return false;
				}
				else
			        continue;
			}
			
			
			if(playfield[newposition._y+y][newposition._x+x].solid)
			{
				 if (currentpiece->blocks[(y*currentpiece->height)+x].solid)
				 {
					 return false;
				 }
			}
			
		}
	}
	return true;
}

BlockState::BlockState()
{
	for(int y = 0;y>20;y++)
	{
		for(int x = 0;x>10;x++)
		{
			playfield[y][x].solid = false;
		}
	}
}

int Piece::nextRotate()
{
	if(rotation+1 == maxrotations)
		return 0;
	else
		return rotation+1;
}

Piece::~Piece()
{
	//delete any blocks we might have allocated
	if(blocks != 0)
	{
		delete [] blocks;
		cout << "Deleted current piece." << endl;
	}
}
void BlockState::placePiece()
{
	cout << "Placing piece." << endl;
	for(int y = 0; y < currentpiece->height; y++)
	{
		for(int x = 0; x < currentpiece->width; x++)
		{
			if(currentpiece->blocks[(currentpiece->height*y)+x].solid)
                playfield[currentpiece->position._y+y][currentpiece->position._x+x] = currentpiece->blocks[(y*currentpiece->height)+x];
		}
	}
}


Block::Block(enum colors color, bool solid)
{
	animation = Animation(BLOCK_SIZE,BLOCK_SIZE);
	this->solid = solid;
	switch(color)
	{
	case red:
		animation.changeSprite(RENDERER->getSprite(RED_BLOCK));
		break;
	case blue:
		animation.changeSprite(RENDERER->getSprite(BLUE_BLOCK));
		break;
	case magenta:
		animation.changeSprite(RENDERER->getSprite(MAGENTA_BLOCK));
		break;
	case orange:
		animation.changeSprite(RENDERER->getSprite(ORANGE_BLOCK));
		break;
	case cyan:
		animation.changeSprite(RENDERER->getSprite(CYAN_BLOCK));
		break;
	case green:
		animation.changeSprite(RENDERER->getSprite(GREEN_BLOCK));
		break;
	case yellow:
		animation.changeSprite(RENDERER->getSprite(YELLOW_BLOCK));
		break;
	}
}

Block::Block()
{
	animation = Animation(BLOCK_SIZE,BLOCK_SIZE);
	animation.changeSprite(RENDERER->getSprite(BLUE_BLOCK));
	solid = false;
	color = red;
}

void Block::draw(Point2i position)
{
	if(solid)
	{
		RENDERER->addSpriteToBuffer(&animation,Point3d(position._x,position._y,0.0));
	}
	else
	{
		return;
	}
}

bool BlockState::isRowFull(int row)
{
	if(row > PLAYFIELD_HEIGHT-1 || row < 0)
		return false;
	else
	{
		for(int x = 0;x < PLAYFIELD_WIDTH;x++)
		{
			if(!playfield[row][x].solid)
			{
				return false;
			}
		}
	}
	return true;
}

bool BlockState::isRowCleared(int row)
{
	if(row > PLAYFIELD_HEIGHT-1 || row < 0)
		return false;
	else
	{
		for(int x = 0;x < PLAYFIELD_WIDTH;x++)
		{
			if(playfield[row][x].solid)
			{
				return false;
			}
		}
	}
	return true;
}

//Clears all completed rows and shifts them down

void BlockState::clearAllRows()
{
	int deletedrows = 0;
	for(int row = 0;row < PLAYFIELD_HEIGHT;row++)
	{
		if(isRowFull(row))
		{
			deleteRow(row);
			deletedrows +=1;
		}
	}

	switch(deletedrows)
	{
	case 1:
		score += 200;
		break;
	case 2:
		score += 500;
		break;
	case 3:
		score += 800;
		break;
	case 4:
		score += 1000;
		break;
	}

	if(deletedrows == 0)
	{
		return;
	}

	int shift = 0;

	for(int row = PLAYFIELD_HEIGHT-1; row >= 0;row--)
	{
		if(isRowCleared(row) == true)
		{
			shift++;
		}

		if(isRowCleared(row) == false)
		{
			moveRow(row,row+shift);
		}
	}
}

void BlockState::deleteRow(int row)
{
	if(row > PLAYFIELD_HEIGHT-1 || row < 0)
	{
		cout << "Warning: Attempting to delete invalid row." << endl;
		return;
	}
	
	for (int x = 0; x < PLAYFIELD_WIDTH;x++)
	{
		playfield[row][x] = Block();
	}
}

void BlockState::moveRow(int src,int dst)
{
	if(src == dst)
		return;
	if(src > PLAYFIELD_HEIGHT-1 || src < 0)
	{
		cout << "Warning: Invalid source row." << endl;
		return;
	}

	if(dst > PLAYFIELD_HEIGHT-1 || dst < 0)
	{
		cout << "Warning: Invalid destination row." << endl;
		return;
	}

	for (int x = 0; x < PLAYFIELD_WIDTH;x++)
	{
		playfield[dst][x] = playfield[src][x];
	}

	deleteRow(src);
}

void Piece::draw()
{
	
	for(int y = 0; y < height;y++)
	{
		for(int x = 0; x < width;x++)
		{
			if(blocks[(height*y)+x].solid)
			{
			    blocks[(y*height)+x].draw(Point2i(PLAYSCREEN_LEFT+((position._x+x)*BLOCK_SIZE),PLAYSCREEN_TOP+((position._y+y)*BLOCK_SIZE)));
			}
		}
	}
}

void Piece::drawPreview(Point2i position)
{
	for(int y = 0; y < height;y++)
	{
		for(int x = 0; x < width;x++)
		{
			if(blocks[(height*y)+x].solid)
			{
				blocks[(height*y)+x].animation.setClipDimensions(NEXT_SIZE,NEXT_SIZE);
				RENDERER->addSpriteToBuffer(&blocks[(height*y)+x].animation,Point3d(position._x+(x*NEXT_SIZE),position._y+(y*NEXT_SIZE),0));
				blocks[(height*y)+x].animation.setClipDimensions(BLOCK_SIZE,BLOCK_SIZE);
			}
		}
	}
}