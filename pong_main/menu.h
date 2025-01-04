#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <vector>
enum GAME_DIFFICULTY {
	EASY,
	NORMAL,
	HARD,
	IMP
};
enum GAME_MODE {
	SINGLE_PLAYER,
	TWO_PLAYER,
	ZERO_PLAYER
};
extern GAME_DIFFICULTY game_difficulty;
extern GAME_MODE game_mode;
extern unsigned RIGHT_COLOR;
extern unsigned LEFT_COLOR;
extern unsigned BALL_COLOR;

void InitiateMainMenu();
