//**********************manager.h*******************
//����ģ��

#pragma once
#include "global.h"
#include "paint.h"
#include "element.h"

#include <fstream>
#include <string>
#include <vector>

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
	MIDPV mids;
	int curmid;
public:
	Manager():curmid(-1){}
	~Manager();
	void Add(const string& picfile);//���ͼƬ
	void Add(Picture *ppic,int x,int y,int w,int h);//�����Ƭ
	void Add(int picid,int x,int y,int w,int h);//�����Ƭ
	void Add(const vector<int>& tilevct,int cycl,int interval);//��Ӷ���
	void AddMid(const string& midfile);
	void Add(int aniIndex);//��ӵ���ѭ������
	Animation& Ani(int id){return *anis.at(id);}
	void LoadRes();
	void LoadMid();
	Animation* PickAni(int p1,int p2=-1,int p3=-1);
	Mid* PicMid(int id);
	void Play(int id);
	void Stop();
};

class Game
{
	//��Ϸ״̬
	enum GAMESTATE{GAME_SHOWLOGO,GAME_SHOWMENU,GAME_SHOWHELP,GAME_CREATE,GAME_RESTART,GAME_RUN,GAME_DESTROY,GAME_WIN,GAME_FAIL,GAME_OVER};
	GAMESTATE state;

	HDC hclientdc;
	HWND hWindow;
	Painter *pPainter;
	int clientw,clienth;
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
	Game(HINSTANCE hinst,HWND hwnd,int clw,int clh);
	~Game();
	void Loop();
	
};

class Mid//mid�������ִ������
{
	string filename;
	UINT wDeviceID;
public:
	Mid(const string& fname);
	~Mid();
	void Play();
	void Stop();
};
