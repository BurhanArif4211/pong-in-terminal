#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fcntl.h>
#include <io.h> 
#include <vector>
#include "menu.h"

GAME_DIFFICULTY game_difficulty = NORMAL;
GAME_MODE game_mode = SINGLE_PLAYER;
unsigned RIGHT_COLOR = 4;
unsigned LEFT_COLOR  = 2;
unsigned BALL_COLOR = 14;
/*
   The different color codes are
   0   BLACK
   1   BLUE
   2   GREEN
   3   CYAN
   4   RED
   5   MAGENTA
   6   BROWN
   7   LIGHTGRAY
   8   DARKGRAY
   9   LIGHTBLUE
   10  LIGHTGREEN
   11  LIGHTCYAN
   12  LIGHTRED
   13  LIGHTMAGENTA
   14  YELLOW
   15  WHITE
   */

using namespace std;
static void clsscr() {
	wcout << "\033[2J\033[1;1H";
}
static void printDialogueBox(vector<wstring> &options) {
	int nth = 1;
	wcout << L"╔════════════════════════════════════╗\n";
	for(wstring &option: options){
		size_t totalSpaces = 32;// @TODO 
		size_t nOfSpaces=totalSpaces-option.size();  
		if (nth > 9)
			nOfSpaces--;
		wstring whiteSpace(nOfSpaces, L' ');
		wcout<< L"║ "<<nth<<L": "<< option<< whiteSpace<<L"║\n";
		/*wprintf( L"║ %d: %s%s║\n", nth, option, whiteSpace); //does not work 😭*/
		nth++;
	}
	wcout<<L"╚════════════════════════════════════╝\n";
}
static void handleColorChange(unsigned &colorToChange) {
	clsscr();
	vector<wstring> Colors = {
		L" BLUE          ",
		L" GREEN         ",
		L" CYAN          ",
		L" RED           ",
		L" MAGENTA       ",
		L" BROWN         ",
		L" LIGHTGRAY     ",
		L" DARKGRAY      ",
		L"LIGHTBLUE     ",
		L"LIGHTGREEN    ",
		L"LIGHTCYAN     ",
		L"LIGHTRED      ",
		L"LIGHTMAGENTA  ",
		L"YELLOW        ",
		L"WHITE         ",
	};
	printDialogueBox(Colors);
	unsigned input;
	cin >> input;
	if (input >= 0 && input <= 15)
		colorToChange = input;
}

void InitiateMainMenu() {

if(!_setmode(_fileno(stdout), _O_U16TEXT))cerr<<"Error: can't change mode";

MainMenuStart: 
clsscr();
wcout<< L"██████╗  ██████╗ ███╗   ██╗ ██████╗\n"
		L"██╔══██╗██╔═══██╗████╗  ██║██╔════╝\n"
		L"██████╔╝██║   ██║██╔██╗ ██║██║  ███\n"
		L"██╔═══╝ ██║   ██║██║╚██╗██║██║   ██\n"
		L"██║     ╚██████╔╝██║ ╚████║╚██████╔\n"
		L"╚═╝      ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝\n";

vector<wstring> mainMenu = {
	L"🤖    One Player",
	L"🆚    Two Player",
	L"🤖🤖  Zero Player (Both AI)",
	L"⚙️    Options",
	L"👤    Credits",	
};
printDialogueBox(mainMenu);
//wcout << LEFT_COLOR << RIGHT_COLOR << BALL_COLOR;
RepeatMainMenuChoice:
	char enteredOption = _getch();
	switch (enteredOption) {
	case '1':
		game_mode = SINGLE_PLAYER;
		break;
	case '2':
		game_mode = TWO_PLAYER;
		break;
	case '3':
		game_mode = ZERO_PLAYER;
		break;
	case '4': {
		clsscr();
		vector<wstring> subMenu = {
			L"Set Difficulty ",
			L"Set Right Paddle Color",
			L"Set Left Paddle Color ",
			L"Set Ball Color ",
			L"Back to Main Menu"};
		printDialogueBox(subMenu);
		RepeatOptionMenuSelection:
			switch (_getch()) {
			case '1': {
					clsscr();
					vector<wstring> DifficultyMenu = {
						L"👶 Easy",
						L"👦 Normal",
						L"🧏 Tough",
						L"☠️ Impossible",
						L"Back To Main Menu"};
					printDialogueBox(DifficultyMenu);

					RepeatDifficultyChoice:
					switch (_getch()) {
						case '1':
							game_difficulty = EASY;
							break;
						case '2':
							game_difficulty = NORMAL;
							break;
						case '3':
							game_difficulty = HARD;
							break;
						case '4':
							game_difficulty = IMP;
							break;
						case '5':
							break;
						default:
							goto RepeatDifficultyChoice;
					};
					goto MainMenuStart;
					break;
				}
			case '2': {
				handleColorChange(RIGHT_COLOR);
				goto MainMenuStart;
				break;
			}
			case '3':
				{
				handleColorChange(LEFT_COLOR);
				goto MainMenuStart;
				break;
				}
			case '4':
				{
				handleColorChange(BALL_COLOR);
				goto MainMenuStart;
				break;
				}
			case '5':
				goto MainMenuStart;
				break;
			default: goto RepeatOptionMenuSelection;
			}
		break;
		}	
	case '5': {
			clsscr();
			vector<wstring> credits = {
				L"             Credits:          ",
				L"     Classic Pong in Terminal  ",
				L"◦ Burhan Arif        24F-CY-001",
				L"◦ M.Saad Rizwaan     24F-CY-048",
				L"◦ Youzdan Ahmed      24F-CY-037",
				L"◦ Hassan Mohiuddin   24F-CY-020",
				L"  Back To Main Menu"
			};
			printDialogueBox(credits);
			if (_getch()) goto MainMenuStart;
		}
	default:goto RepeatMainMenuChoice;
	}
	if (!_setmode(_fileno(stdout), _O_TEXT)) cerr << "Error: can't change mode";
}