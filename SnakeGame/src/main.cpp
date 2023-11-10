#include <iostream>
#include <conio.h>
#include <Windows.h>

enum snakeDirection { stop = 0, left, right, up, down };

struct playArea {
	const int width = 80;
	const int height = 20;
	int foodCordX, foodCordY;
} playArea;

struct player {
	int x, y;
	int score;
	int snakeTailX[100], snakeTailY[100];
	int snakeTailLen;
	snakeDirection snakeDir;
} player;

bool isGameOver;

void gameInit();

void gameRender(std::string);

void gameUpdate();

void userInput();

void gameLoop(std::string);

bool deathScreen();

int main() {

	std::string playerName;
	std::cout << "Hello, enter your name: ";
	std::cin >> playerName;
	bool reset = true;

	while (reset) {
		gameInit();
		while (!isGameOver) {
			gameRender(playerName);
			userInput();
			gameUpdate();
			Sleep(150);
		}
		reset = deathScreen();
	}

	return 0;
}

void gameInit() {
	isGameOver = false;
	player.snakeDir = stop;
	player.x = playArea.width / 2;
	player.y = playArea.height / 2;
	playArea.foodCordX = rand() % playArea.width;
	playArea.foodCordY = rand() % playArea.height;
	player.snakeTailLen = 0;
	player.score = 0;
}

void gameRender(std::string playerName) {
	system("cls");

	// Creation of the top wall
	for (int i = 0; i < playArea.width; i++)
		std::cout << '-';
	std::cout << std::endl;

	// Loop for rendering everything else
	for (int y = 0; y < playArea.height; y++) {
		for (int x = 0; x <= playArea.width + 2; x++) {
			// Side wall creation
			if (x == 0 || x == playArea.width)
				std::cout << "|";

			// Snake head creation
			if (x == player.x && y == player.y)
				std::cout << "O";

			// Fruit positioning
			else if (x == playArea.foodCordX && y == playArea.foodCordY)
				std::cout << "F";

			// Tail rendering
			else {
				bool printTail = false;
				for (int i = 0; i < player.snakeTailLen; i++) {
					if (x == player.snakeTailX[i] && y == player.snakeTailY[i]) {
						std::cout << "o";
						printTail = true;
					}
				}
				if (!printTail)
					std::cout << " ";
			}
		}
		std::cout << std::endl;
	}

	// Creation of the bottom walls
	for (int i = 0; i < playArea.width + 2; i++)
		std::cout << "-";
	std::cout << std::endl;

	// Player score display
	std::cout << playerName << "'s score: " << player.score << std::endl;
}

void gameUpdate() {
	int prevX = player.snakeTailX[0];
	int prevY = player.snakeTailY[0];
	int prev2X, prev2Y;

	player.snakeTailX[0] = player.x;
	player.snakeTailY[0] = player.y;

	for (int i = 1; i < player.snakeTailLen; i++) {
		prev2X = player.snakeTailX[i];
		prev2Y = player.snakeTailY[i];
		player.snakeTailX[i] = prevX;
		player.snakeTailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	switch (player.snakeDir) {
	case left:
		player.x--;
		break;

	case right:
		player.x++;
		break;

	case up:
		player.y--;
		break;

	case down:
		player.y++;
		break;
	}

	// Collison checks
	// Walls
	if (player.x >= playArea.width || player.x < 0 || player.y >= playArea.height || player.y < 0)
		isGameOver = true;

	// Tail
	for (int i = 0; i < player.snakeTailLen; i++)
		if (player.x == player.snakeTailX[i] && player.y == player.snakeTailY[i])
			isGameOver = true;

	// Food
	if (player.x == playArea.foodCordX && player.y == playArea.foodCordY) {
		player.score += 10;
		playArea.foodCordX = rand() % playArea.width;
		playArea.foodCordY = rand() % playArea.height;
		player.snakeTailLen++;
	}
}

// Process user input
void userInput() {
	if (_kbhit()) {
		switch (_getch()) {
		case 'a':
			player.snakeDir = left;
			break;

		case 'd':
			player.snakeDir = right;
			break;

		case 'w':
			player.snakeDir = up;
			break;

		case 's':
			player.snakeDir = down;
			break;

		case '\033':
			isGameOver = true;
			break;
		}
	}
}

bool deathScreen() {
	system("cls");
	std::cout << "Your score: " << player.score << "\nDo you want to play again? Y/n: ";
	std::string option;
	std::cin >> option;
	while (option != "Y" && option != "n") {
		std::cout << "Answer must be of the form Y/n: ";
		std::cin >> option;
	}

	if (option == "n")
		return false;
	else if(option == "Y")
		return true;

}