#include <iostream>
#include <ctime>
#include <curses.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <utility>
#include <string.h>

using namespace std;

class Game
{

private:


	vector<int>  get_endpoints(int x, int y, int color) {
		vector<int> ep;
		if (map[x][y]) {
			return ep;
		}

		for (auto it : dirs) {
			if (map[x + it.first][y + it.second] == -color) {
				for (int i = 0; -1 > (x + (i + 2) * (it.first)) < 8 && -1 > (y + (i + 2) * (it.second)) < 8; i++)
				{
					if (!map[x + (i + 2) * (it.first)][y + (i + 2) * (it.second)]) break;
					if (map[x + (i + 2) * (it.first)][y + (i + 2) * (it.second)] == color) {
						ep.push_back((x + (i + 2) * (it.first)) * 8 + (y + (i + 2) * (it.second)));
						break;
					}
				}
			}
		}
		return ep;
	}


	vector< pair<int, vector<int> > > get_moves(int color) {
		vector< pair<int, vector< int> > > moves;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				auto end_points = get_endpoints(i, j, color);
				if (end_points.size()) {
					moves.push_back({ i * 8 + j, end_points });
				}
			}
		}
		return moves;
	}


	void make_move(int x, int y, int color, vector<int> ep) {
		for (auto it = ep.begin(); it != ep.end(); it++) {
			int offsetX = (x < (*it / 8)) ? 1 : -1;
			int offsetY = (y < (*it % 8)) ? 1 : -1;

			while ((x != (*it / 8)) || (y != (*it % 8))) {
				map[x][y] = color;
				x = x != *it / 8 ? x += offsetX : x;
				y = y != *it % 8 ? y += offsetY : y;
			}
		}
	}

	int score(int color) {
		int sum = 0;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				sum += map[i][j];
			}
		}
		return sum * color;
	}
	int map[8][8];
	int gamer = 1;
	vector<pair<int, int>>   dirs{ {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1} };
	pair<int, int> cur = make_pair<int, int>(3, 3);

public:
	Game() {
		memset(map, 0, sizeof(map));
		map[3][3] = map[4][4] = -1;
		map[3][4] = map[4][3] = 1;

	}
	void Gameloop() {
		while (true)
		{
			Input(getch());
			Invalidate();
		}
	}

	void Input(chtype ch) {

		switch (ch) {
		case KEY_UP:
			if (cur.second > 0)
				cur.second--;
			break;
		case KEY_DOWN:
			if (cur.second < 7)
				cur.second++;
			break;
		case KEY_LEFT:
			if (cur.first > 0)
				cur.first--;
			break;
		case KEY_RIGHT:
			if (cur.first < 7)
				cur.first++;
			break;
		case 32:
			Move();
			break;
		}
	}

	void Move() {
		for (size_t i = 0; i < 8; i++)
		{
			for (size_t j = 0; j < 8; j++)
			{
				if (map[i][j] == 2) map[i][j] = 0;
			}

		}
		auto gm = get_moves(gamer);

		for (auto it = gm.begin(); it != gm.end(); it++) {
			if (cur.second * 8 + cur.first == it->first) {
				make_move(cur.second, cur.first, gamer, it->second);
				gamer = -gamer;
				break;
			}
		}
	}

	void Invalidate() {
		clear();

		bool flag = true;
		for (size_t i = 0; i < 8; i++)
		{
			for (size_t j = 0; j < 8; j++)
			{
				if (map[i][j] == 0 || map[i][j] == 2) flag = false;
			}

		}
		if (flag) {
			clear();
			printw("%d", score(-gamer));
			exit(0);
		}


		auto moves = get_moves(gamer);


		for (auto it = moves.begin(); it != moves.end(); it++) {
			map[it->first / 8][it->first % 8] = 2;
		}


		int x, y;
		int px = (getmaxx(stdscr) - (8 * 2 + 1)) / 2;
		for (y = 0; y < (8 * 2); y++)
		{
			if (y % 2 == 0)
			{
				for (x = 0; x < 8 * 2; x += 2)
				{
					chtype c;
					int ch = map[y / 2][x / 2];
					int ind_c = 1;
					switch (ch)
					{
					case 1:
						c = 'x';
						ind_c = 2;
						break;
					case -1:
						c = 'o';
						ind_c = 3;
						break;
					case 2:
						c = '*';
						break;
					default:
						c = '.';
						break;
					}
					move(y + 3, px + x + 1);
					addch(c | COLOR_PAIR(ind_c));
				}
			}


		}
		mvprintw(cur.second * 2 + 3, px + cur.first * 2 + 1, "%c", '@');
	}
};

void Init_scr() {
	const char items[2][5] = {
		"Play",
		"Exit"
	};
	int choice = 0;
	while (true)
	{
		clear();
		box(stdscr, 0, 0);

		for (unsigned i = 0; i < 2; i++)
		{
			move((getmaxy(stdscr) + i + 1) / 2, (getmaxx(stdscr)) / 2);
			if (i == choice)
				addch('>');
			else
				addch(' ');

			printw("%s\n", items[i]);
		}

		switch (getch())
		{
		case KEY_UP:
			if (choice)
				choice--;
			break;
		case KEY_DOWN:
			if (choice != 1)
				choice++;
			break;
		case 10:
			if (choice == 0) return;
			if (choice == 1) {
				clear();
				exit(0);
			}
			break;
		}
	}
}


int main()
{
	initscr();
	cbreak();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	init_pair(3, COLOR_RED, COLOR_GREEN);

	attron(COLOR_PAIR(1));
	bkgd(COLOR_PAIR(1));
	Init_scr();
	clear();

	Game g;
	g.Invalidate();
	g.Gameloop();
	return 0;
}
