#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <chrono>

using namespace std;

chrono::milliseconds elapsedTime;
chrono::milliseconds timer;

void Init();
void InitTimer();
void SetupWindow();
void InitSprites();
void InitGameField();
void Draw();
void DrawGameField();
void Update(int dt);
void MoveSnake();

const int WINDOW_SIZE_IN_TILES = 15;
const int WINDOW_HEIGHT        = 960;
const int WINDOW_WIDTH         = 960;
const int FRAMERATE_LIMIT      = 60;
const int TILE_SIZE            = 64;
const int SNAKE_MOVE_TIME      = 300;

const char SNAKE = 's';
const char TREE  = '#';
const char GRASS = ' ';

// Array with tile codes
char gameField[WINDOW_SIZE_IN_TILES][WINDOW_SIZE_IN_TILES];

int snakeHeadCol = 8;
int snakeHeadRow = 4;


int snakeMoveTimer = SNAKE_MOVE_TIME;

sf::RenderWindow window(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), "Snake");
sf::Sprite treeSprite;
sf::Sprite grassSprite;
sf::Sprite snakeSprite;

sf::Texture textureTerrain;

void SetupWindow()
{
	window.setFramerateLimit(FRAMERATE_LIMIT);
}

void InitSprites()
{
	
	if (!textureTerrain.loadFromFile("snakeset.png")) {
		std::cerr << "Can't load texture" << endl;
		exit(1);
	}

	treeSprite.setTexture(textureTerrain);
	treeSprite.setTextureRect(sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE));

	grassSprite.setTexture(textureTerrain);
	grassSprite.setTextureRect(sf::IntRect(0, TILE_SIZE, TILE_SIZE, TILE_SIZE));

	snakeSprite.setTexture(textureTerrain);
	snakeSprite.setTextureRect(sf::IntRect(TILE_SIZE * 2, TILE_SIZE * 2, TILE_SIZE, TILE_SIZE));
}

void InitGameField()
{
	// Tile codes in gameField array:
	// '#' char means tree
	// 's' char means snake
	// ' ' empty tile, draws it as grass

	for (int row = 0; row < WINDOW_SIZE_IN_TILES; row++)
	{
		for (int col = 0; col < WINDOW_SIZE_IN_TILES; col++)
		{
			if ((row == 0 || row == (WINDOW_SIZE_IN_TILES - 1)) || (col == 0 || col == (WINDOW_SIZE_IN_TILES - 1)))
				gameField[row][col] = TREE;
			else
			{
				gameField[row][col] = GRASS;
			}
		}
	}

	gameField[4][5] = SNAKE;
	gameField[4][6] = SNAKE;
	gameField[4][7] = SNAKE;
	gameField[4][8] = SNAKE;
}

void Init()
{
	SetupWindow();

	InitSprites();

	InitGameField();

	InitTimer();
}

void InitTimer()
{
	timer = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch());
}

int main()
{
	Init();

	bool isRunning = true;

	while (isRunning)
	{
		std::chrono::milliseconds newTime = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch());
		elapsedTime = newTime - timer;
		timer = newTime;

		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		Update(elapsedTime.count());

		Draw();

		window.display();
	}

	return 0;
}

void Update(int dt)
{
	snakeMoveTimer -= dt;

	if (snakeMoveTimer <= 0)
	{
		MoveSnake();
		snakeMoveTimer = SNAKE_MOVE_TIME;
	}
}

void MoveSnake()
{
	// Crash to the tree
	if (gameField[snakeHeadRow][snakeHeadCol + 1] == TREE)
	{
		cout << "You loose!" << endl;
		exit(1);
	}

	gameField[snakeHeadRow][snakeHeadCol - 3] = GRASS;
	snakeHeadCol++;
	gameField[snakeHeadRow][snakeHeadCol] = SNAKE;
}

void Draw()
{
	DrawGameField();
}

void DrawGameField()
{
	for (int row = 0; row < WINDOW_SIZE_IN_TILES; row++)
	{
		for (int col = 0; col < WINDOW_SIZE_IN_TILES; col++)
		{
			char tile = gameField[row][col];

			if (tile == GRASS)
			{
				grassSprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
				window.draw(grassSprite);
			}
			else if (tile == TREE)
			{
				treeSprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
				window.draw(treeSprite);
			}
			else if (tile == SNAKE)
			{
				grassSprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
				window.draw(grassSprite);

				snakeSprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
				window.draw(snakeSprite);
			}
		}
	}
}
