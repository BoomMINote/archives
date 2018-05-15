//*********************paint.cpp**************************

#include "paint.h"
#pragma comment(lib,"Msimg32.lib")
//---------------Picture-----------------------
HINSTANCE Picture::hInst=NULL;
HDC Picture::horgdc=NULL;

//Ԥ�ȳ�ʼ�����������ڸ�����Ҫ�ڹ���ʱ֪��ĳЩ��Ҫ���������Ҷ������ж�����Щ����
//����ͬ�����Բ��þ�̬��ԱԤ�ȳ�ʼ���ķ���
void Picture::PreInit(HINSTANCE hInstance,HDC horiginaldc)
{
	hInst=hInstance;
	horgdc=horiginaldc;
}

Picture::Picture(const char* picfile):hdc(NULL),hbmp(NULL)
{
	//ͼƬ�ļ���
	if(!hInst || !horgdc) throw Init_Error("ͼƬ��δԤ��ʼ����");
	hbmp=(HBITMAP)LoadImage(hInst,picfile,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION |LR_LOADFROMFILE);
	if(!hbmp)throw Outer_Error("����ͼƬʧ�ܣ�");
	hdc=CreateCompatibleDC(horgdc);
	if(!hdc)throw Outer_Error("���������豸ʧ�ܣ�");
	SelectObject(hdc,hbmp);
	BITMAP bmp;
	if(!GetObject(hbmp,sizeof(BITMAP),&bmp))throw Outer_Error("��ȡͼƬ��Ϣʧ�ܣ�");
	w=bmp.bmWidth;
	h=bmp.bmHeight;
}

Picture::~Picture()
{
	if(hdc)DeleteObject(hdc);
	if(hbmp)DeleteObject(hbmp);
}


//---------------Animation-------------------
void Animation::Refresh()
{
	//�����Ŀ���
	if(intervalpos<interval)
	{
		intervalpos++;
		return;
	}
	else
	{
		intervalpos=0;
	}
	if(pos<(int)tilepv.size()-1 && !stoped)
	{
		pos++;
	}
	else
	{
		pos=0;
		if(cycl!=0)
		{
			cyctimes++;
			if(cyctimes>=cycl)
				stoped=true;
		}
	}
}

//--------------------Painter----------------------------
Painter::Painter(HDC hDC,int cx,int cy,int cw,int ch):hdc(hDC),hcompdc(NULL),hcompbmp(NULL),x(cx),y(cy),w(cw),h(ch)
{
	//����˫���壬����������
	hcompdc=CreateCompatibleDC(hdc);
	hcompbmp=CreateCompatibleBitmap(hdc,w,h);
	SelectObject(hcompdc,hcompbmp);
}

Painter::~Painter()
{
	if(hcompdc)
	{
		DeleteDC(hcompdc);
		hcompdc=NULL;
	}
	if(hcompbmp)
	{
		DeleteObject(hcompbmp);
		hcompbmp=NULL;
	}
}

void Painter::Draw(const Tile& tile,int x,int y)
{
	//λ�鴫�䣬������ͼ
	BitBlt(hcompdc,x,y,tile.tilew,tile.tileh,tile.pPic->horgdc,tile.picx,tile.picy,SRCCOPY);
}

void Painter::Draw(const Animation& ani,int x,int y)
{
	if(ani.Stoped())return;
	const Tile& tile=ani.GetTile();
	BitBlt(hcompdc,x,y,tile.tilew,tile.tileh,tile.pPic->hdc,tile.picx,tile.picy,SRCCOPY);
}

void Painter::Draw(const Animation& ani,int x,int y,int w,int h)
{
	if(ani.Stoped())return;
	const Tile& tile=ani.GetTile();
	//����͸��ɫ��λ�鴫��
	TransparentBlt(hcompdc,x,y,w,h,tile.pPic->hdc,tile.picx,tile.picy,tile.tilew,tile.tileh,RGB(0,0,0));
}

void Painter::Draw(const Text& tex,int x,int y)
{
	//��������
	SetBkMode(hcompdc,TRANSPARENT);
	SetTextColor(hcompdc,tex.color);
	SelectObject(hcompdc,tex.font);
	TextOut(hcompdc,x,y,tex.str.c_str(),tex.str.length());
}

void Painter::Erase()
{
	//��������ͼ������в���
	HBRUSH brush=CreateSolidBrush(bkcolor);
	SelectObject(hcompdc,brush);
	RECT rect={x,y,x+w,y+h};
	FillRect(hcompdc,&rect,brush);
	DeleteObject(brush);
}

void Painter::Apply()
{
	//�ѻ���ҳ�������������Ļ�豸��
	BitBlt(hdc,x,y,w,h,hcompdc,0,0,SRCCOPY);
}