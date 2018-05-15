//**********************manager.h*******************
//����ģ��

#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "global.h"
#include "paint.h"
#include "element.h"

using std::string;
using std::fstream;
using std::vector;

class Picture;
class Tile;
class Animation;
class Painter;
class Panel;
class Mid;

class Manager
{
	typedef vector<Picture*> PICPV;
	typedef vector<Tile*> TILEPV;
	typedef vector<Animation*> ANIPV;
	typedef vector<Mid*> MIDPV;
	//��˳�������������е�ͼƬ�뱳��������Դ
	PICPV pics;
	TILEPV	tiles;
	ANIPV anis;
	string curmusic;
public:
	Manager(){}
	~Manager();
	void Add(const string& picfile);//���ͼƬ
	void Add(Picture *ppic, int x, int y, int w, int h);//�����Ƭ
	void Add(int picid, int x, int y, int w, int h);//�����Ƭ
	void Add(const vector<int>& tilevct, int cycl, int interval);//��Ӷ���

	void Add(int aniIndex);//��ӵ���ѭ������
	Animation& Ani(int id) { return *anis.at(id); }
	void LoadRes();
	void ReleaseRes();

	Animation* PickAni(int p1, int p2 = -1, int p3 = -1);

};

class Game
{
	//��Ϸ״̬
	enum GAMESTATE { GAME_SHOWLOGO, GAME_SHOWMENU, GAME_SHOWHELP, GAME_CREATE, GAME_RESTART, GAME_RUN, GAME_DESTROY, GAME_WIN, GAME_FAIL, GAME_OVER, GAME_EXIT };
	GAMESTATE state;

	SDL_Renderer* renderer;
	SDL_Window* window;
	Painter *pPainter;
	int clientw, clienth;
	Manager manager;

	int level;
	int roundtime;//ÿһ�����ŵ�ʱ��
	void ShowLogo();
	void ShowMenu();
	void ShowWin();
	void ShowFail();
	void ShowOver();
	void ShowHelp();
	void Create();
	void ReStart();
	void Run();
	void Destroy();

	void ReadArchive();
	void SaveArchive();
	///////////////////Game Element///////////////////
	Panel *pPanel;
public:
	Game(int clw, int clh);
	~Game();
	void Loop();

};
