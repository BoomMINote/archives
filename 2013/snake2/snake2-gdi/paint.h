//*******************paint.h**************************
//��Ҫ������ͼ��ģ��
#pragma once
#include "global.h"
#include "manager.h"
#include "element.h"
#include <windows.h>
#include <vector>


using std::vector;

class Text;

class Picture//ͼƬ��
{
	friend class Painter;
	HDC hdc;
	HBITMAP hbmp;
	int w,h;
	static HINSTANCE hInst;
	static HDC horgdc;
public:
	static void PreInit(HINSTANCE hInstance,HDC horiginaldc);
	Picture(const char* picfile);
	~Picture();
};

class Tile//��Ƭ�࣬��ͼƬ�ϵ�һ���������
{
	friend class Painter;
	Picture *pPic;
	int picx,picy,tilew,tileh;
public:
	Tile(Picture *ppic,int x,int y,int w,int h)
		:pPic(ppic),picx(x),picy(y),tilew(w),tileh(h)
	{if(ppic<0||x<0||y<0||w<=0||h<=0)throw Init_Error("��Ƭ���ʼ��ʧ�ܣ�");}
	virtual const Tile& GetTile()const{return *this;}
	Picture* Pic(){return pPic;}
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
	Animation(int cyc,int itvl):cycl(cyc),interval(itvl),pos(0),cyctimes(0),intervalpos(0),stoped(false)
	{if(cyc<0||itvl<0)throw Init_Error("�������ʼ��ʧ�ܣ�");}
	void AddTile(Tile* tile){tilepv.push_back(tile);}
	void Refresh();
	void Restart(){pos=0;intervalpos=0;cyctimes=0;stoped=false;}
	const Tile& GetTile()const {return *tilepv[pos];}
	bool Stoped()const{return stoped;}
};

class Painter//����ͼ����
{
	HDC hcompdc;
	HDC hdc;
	HBITMAP hcompbmp;
	int x,y,w,h;
	DWORD bkcolor;
public:
	Painter(HDC hDC,int cx,int cy,int cw,int ch);
	~Painter();
	void Draw(const Tile& tile,int x,int y);
	void Draw(const Animation& ani,int x,int y);
	void Draw(const Animation& ani,int x,int y,int w,int h);
	void Draw(const Text& tex,int x,int y);
	void Erase();
	void Apply();
};
