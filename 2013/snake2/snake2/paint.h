//*******************paint.h**************************
//��Ҫ������ͼ��ģ��
#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "global.h"
#include "manager.h"
#include "element.h"

using std::vector;

class Text;

class Picture//ͼƬ��
{
	friend class Painter;
	static SDL_Renderer* renderer_;
	SDL_Texture* texture;
	int w, h;
public:
	static void PreInit(SDL_Renderer* renderer);
	Picture(const char* picfile);
	~Picture();
};

class Tile//��Ƭ�࣬��ͼƬ�ϵ�һ���������
{
	friend class Painter;
	Picture *pPic;
	int picx, picy, tilew, tileh;
public:
	Tile(Picture *ppic, int x, int y, int w, int h)
		:pPic(ppic), picx(x), picy(y), tilew(w), tileh(h)
	{
		if (ppic<0 || x<0 || y<0 || w <= 0 || h <= 0)throw Init_Error("��Ƭ���ʼ��ʧ�ܣ�");
	}
	virtual const Tile& GetTile()const { return *this; }
	Picture* Pic() { return pPic; }
};

class Animation//�����࣬�������Ƭ���ɶ���
{
	friend class Painter;
	vector<Tile*> tilepv;
	//
	int cycl;
	int interval;
	int  pos;
	int intervalpos;
	int cyctimes;
	bool stoped;
public:
	Animation(int cyc, int itvl) :cycl(cyc), interval(itvl), pos(0), cyctimes(0), intervalpos(0), stoped(false)
	{
		if (cyc<0 || itvl<0)throw Init_Error("�������ʼ��ʧ�ܣ�");
	}
	void AddTile(Tile* tile) { tilepv.push_back(tile); }
	void Refresh();
	void Restart() { pos = 0; intervalpos = 0; cyctimes = 0; stoped = false; }
	const Tile& GetTile()const { return *tilepv[pos]; }
	bool Stoped()const { return stoped; }
};

class Painter//����ͼ����
{
	SDL_Renderer* renderer_;
	int x, y, w, h;
	SDL_Color bkcolor;
public:
	Painter(SDL_Renderer* renderer, int cx, int cy, int cw, int ch);
	~Painter();
	void Draw(const Tile& tile, int x, int y);
	void Draw(const Animation& ani, int x, int y);
	void Draw(const Animation& ani, int x, int y, int w, int h);
	void Draw(const Text& tex, int x, int y);
	void Erase();
	void Apply();
};
