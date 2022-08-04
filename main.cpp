/*
Basic Principle (Game Loop)

While the game is running:
	handle user input
	update all objects
	render changes to the display

*/


#include "SDL.h"
#include <iostream>
#include "SDL_image.h"

#include "levels.cpp"

//Code block below limits the frame rate to 60 fps. But the correct way would be to use !!!delta time!!!
int cnt = 0;
const int FPS = 60;
const int frameDelay = 1000 / FPS;
Uint32 frameStart;
int frameTime;

bool running = false;
SDL_Window* window;
SDL_Renderer* renderer = nullptr;

SDL_Texture* loadTexture(const char* texture) {
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

void drawTexture(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest) {
	SDL_RenderCopy(renderer, tex, &src, &dest);


}

struct gameObject {

	SDL_Texture* objTexture;
	SDL_Renderer* re;
	int xpos;
	int ypos;


	SDL_Rect srcR, destR; //has width, height, x y positions

	gameObject(const char* textureSheet,int x, int y) { //This is a constructor
		objTexture = loadTexture(textureSheet);
		xpos = x;
		ypos = y;
	}
	~gameObject(); //a deconstructor
	

	void Update() {
		xpos++;
		ypos++;
		srcR.h = 500; //Image resolution. 32x32 will only take the first 32x32 square (on higher resolution, wont see whole image)
		srcR.w = 500;
		srcR.x = 0;
		srcR.y = 0;

		destR.x = xpos;
		destR.y = ypos;
		destR.h = 100;
		destR.w = 100;
	};

	void Render() {
		SDL_RenderCopy(renderer, objTexture, &srcR, &destR);
	};
};



int lvl1[25][25] = {

	{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
}; //We can later parse an XML file and extract information from it

struct Map {

	SDL_Rect srcR, destR;
	SDL_Texture* dirt;
	SDL_Texture* grass;
	SDL_Texture* water;

	int map[25][25];

	Map() {
		dirt = loadTexture("assets/dirt.png");
		grass = loadTexture("assets/grass.png");
		water = loadTexture("assets/water.png");
	
		loadMap(lvl1); //(temporary)

		srcR.y = srcR.x = 0;
		srcR.w = srcR.h = 300;
		destR.w = destR.h = 64;
		destR.x = destR.y = 0;
	}
	~Map();


	//srcR.y = 0;  //Hey ! I wanted to write this. But insider a class/struct, declarations are only possible inside methods. But initialization is possible

	void loadMap(int arr[25][25]) { //take in the level
		for (int r = 0;r < 25;r++) {

			for (int c = 0;c < 25;c++) {
				map[r][c] = arr[r][c];
			}
		}
	}

	void drawMap() { //find the type and draw correct tile to correct position
		int type = 0;
		for (int r = 0;r < 25;r++) {

			for (int c = 0;c < 25;c++) {
				type = map[r][c];

				destR.x = c * 32; //The drawing rectangle will only be on multiple of 32 pixels
				destR.y = r * 32;

				switch (type) {

					case 0:
						drawTexture(water, srcR, destR);
						break;

					case 1:
						drawTexture(grass, srcR, destR);
						break;

					case 2:
						drawTexture(dirt, srcR, destR);
						break;
					}
				}
			}
		}

	


};

//------------------------------Initialize instances here-----------------------------------------------------------------------------------------
gameObject* player;
gameObject* enemy;
Map* map;

void handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		running = false;
		break;

	default:
		break;
	}
}

void updateObjects() {
	player->Update();
	enemy->Update();
}

void renderChanges() { //Things load in layer by layer
	SDL_RenderClear(renderer);

	map->drawMap();
	player->Render();
	enemy->Render();

	SDL_RenderPresent(renderer);

}

void clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	//Game Cleaned
}



int main(int argc, char* argv[]) {

	//Have a check for the 3 functions below (should return 0. else there is an error)
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 640, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

	running = true;


	//Create instances here and declare the values
	player = new gameObject("assets/player.png",0,0);
	enemy = new gameObject("assets/player.png", 100, 100);
	map = new Map(); //pass the map (in the future. default is lvl1)


	while (running) { //game loop

		frameStart = SDL_GetTicks();

		handleEvents();
		updateObjects();
		renderChanges();

		frameTime = SDL_GetTicks() - frameStart; //how long it took for one frame

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
			
		}

	}

	clean();

	return 0;
}