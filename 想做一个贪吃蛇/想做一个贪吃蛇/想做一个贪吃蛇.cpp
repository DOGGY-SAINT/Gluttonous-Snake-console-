#include<iostream>						//差一手输
#include<string>
#include<windows.h>
#include <stdlib.h>
#include <time.h>
#include<conio.h>
#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3
#define SIZE 12
#define GAPTIME 900
const int DIR[4][2] = { {-1,0},{0,-1} , {1,0} , {0,1} };
const char to[12] = { 0,1,2,3,4,5,6,7,8,9,10,11 };
const double RATE = 0.95;
using namespace std;

class position
{
private:
	int x;
	int y;
public:
	position()
	{
		x = 0, y = 0;
	}
	position(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	int operator==(position b)
	{
		if (this->x == b.x&&this->y == b.y)
			return 1;
		else
			return 0;
	}
	int rx() { return x; }
	int ry() { return y; }
};


class snake
{
private:
	string x;
	string y;
public:
	snake()
	{
		x = "22";
		y = "22";
		x[0] = 1, y[0] = 3;
		x[1] = 1, y[1] = 2;
	}
	int isinside(int t)
	{
		if (t >= SIZE - 1)
			return 1;
		if (t <= 0)
			return SIZE - 2;
		return t;
	}
	void move(int dir)					//不删尾
	{
		int lx = x.length();
		x.insert(0, " ");
		x[0] = isinside(x[1] + DIR[dir][0]);
		int ly = y.length();
		y.insert(0, " ");
		y[0] = isinside(y[1] + DIR[dir][1]);
	}
	void del()
	{
		int lx = x.length();
		x.erase(lx - 1, 1);
		int ly = y.length();
		y.erase(ly - 1, 1);
	}
	int inmap(char p[12][13])
	{
		int l = x.length();
		
		for (int i = 1; i < l; i++)
		{
			p[x[i]][y[i]] = '=';
		}
		if (p[x[0]][y[0]] == '=')
			return 0;
		else
			p[x[0]][y[0]] = '@';
		return 1;
	}
	position p_head()
	{
		position p(x[0], y[0]);
		return p;
	}
	int rx()
	{
		return x[0];
	}
	int ry()
	{
		return y[0];
	}
};

class food
{
private:
	position star;
public:
	food()
	{
		star = position(0, 0);
	}
	position rp()
	{
		return star;
	}
	void refresh(char map[SIZE][SIZE+1])
	{
		srand((unsigned)time(NULL));
		while (1)
		{
			int x = rand() % 9 + 1;
			int y = rand() % 9 + 1;
			if (map[x][y] == ' ')
			{
				star = position(x, y);
				map[x][y] = '*';
				break;
			}
		}
	}
	void inmap(char map[12][13])
	{
		map[star.rx()][star.ry()] = '*';
	}
};

class total
{
private:
	char map[SIZE][SIZE+1];
	char introduction[SIZE][41];
	snake s;
	food f;
	int GapTime;
	int dir;
	int L_dir;
	int score;
	int clock_s;
	int alive;
public:
	total()
	{
		memset(map, 0, sizeof(map));
		memset(introduction, 0, sizeof(introduction));
		GapTime = GAPTIME;
		dir = L_dir=-1;
		score = 0;
		clock_s = 0;
		alive = 1;
	}
	void map_in(char t[SIZE][SIZE + 1])
	{
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
				map[i][j] = t[i][j];
	}
	void introduction_in(char t[SIZE][41])
	{
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < 41; j++)
				introduction[i][j] = t[i][j];
	}
	void show_all()
	{
		for (int i = 0; i < SIZE; i++)
		{
			cout << map[i]<< introduction[i];
			if(i==SIZE-1)
				cout << score;
			cout << endl;
		}
		
	}
	void init()
	{
		char mt[12][13] =
		{
		"++++++++++++",
		"+          +",
		"+          +",
		"+          +",
		"+          +",
		"+          +",
		"+          +",
		"+          +",
		"+          +",
		"+          +",
		"+          +",
		"++++++++++++"
		};
		map_in(mt);
		char it[SIZE][41] =
		{
		"    制作人:DOGGY_LEE",
		"    操作说明:",
		"    按下d开始游戏",
		"    按下wasd来控制蛇的方向",
		"    @为蛇头,=为蛇身,*为食物",
		"    吃食物可以加得分",
		"    随着得分越来越高,蛇会越来越快",
		"    蛇可以穿过边界来回到另一边",
		"    但不可以触碰自己的身体",
		"    转身操作视为无效",
		"    ",
		"    当前得分:"
		};
		introduction_in(it);
		s.inmap(map);
		f.refresh(map);
	}
	void turn(int dir)
	{
		char mt[12][13] =
		{
		"++++++++++++",
		"+          +",
		"+          +",
		"+          +",
		"+          +",
		"+          +",
		"+          +",
		"+          +",
		"+          +",
		"+          +",
		"+          +",
		"++++++++++++"
		};
		map_in(mt);
		s.move(dir);
		if (s.p_head() == f.rp())
		{
			s.inmap(map);
			f.refresh(map);
			score += 100;
			GapTime *= RATE;
		}
		else
		{
			s.del();
			alive = s.inmap(map);
		}
		f.inmap(map);
		system("cls");
		show_all();
		L_dir = dir;
	}
	int prevent(int d1, int d2)
	{
		if (d1 != d2 && d1 % 2 == d2 % 2)
			return 0;
		return 1;
	}
	void loop()
	{
		while (alive)
		{
			if (clock_s == 0)
			{
				clock_s = clock();
				dir = RIGHT;
				turn(dir);
			}
			if (clock() - clock_s >= GapTime)
			{
				turn(dir);
				clock_s = clock();
			}
			if (_kbhit())
			{
				char tmp = _getch();
				int t;
				if (tmp >= 'A'&&tmp <= 'Z')
					tmp = 'a' + tmp - 'A';
				switch(tmp)
				{
				case 'w':
					t = UP;
					break;
				case'a':
					t = LEFT;
					break;
				case's':
					t = DOWN;
					break;
				case'd':
					t = RIGHT;
					break;
				}
				if (prevent(t, L_dir))
					dir = t;
			}
		}
		system("cls");
		char it[SIZE][41] =
		{
		"    制作人:DOGGY_LEE",
		"    ",
		"    ",
		"    ",
		"    ",
		"    ",
		"    ",
		"    ",
		"    ",
		"    ",
		"    你输了,按下任意按键退出",
		"    最终得分:"
		};
		introduction_in(it);
		show_all();
	}
};







int main()
{
	total glb;
	glb.init();
	glb.show_all();
	while (1)
	{
		if (_getch() == 'd')
			break;
	}
	glb.loop();
	_getch();
	return 0;
}