//*********************paint.cpp**************************
#include <SDL2/SDL_image.h>
#include "paint.h"
//---------------Picture-----------------------

//Ԥ�ȳ�ʼ�����������ڸ�����Ҫ�ڹ���ʱ֪��ĳЩ��Ҫ���������Ҷ������ж�����Щ����
//����ͬ�����Բ��þ�̬��ԱԤ�ȳ�ʼ���ķ���
void Picture::PreInit(SDL_Renderer* renderer)
{
	renderer_ = renderer;
}

Picture::Picture(const char* picfile) 
{
	//ͼƬ�ļ���
	if (!renderer_) throw Init_Error("ͼƬ��δԤ��ʼ����");
	texture = IMG_LoadTexture(renderer_, picfile);
	if (!texture)throw Outer_Error("����ͼƬʧ�ܣ�");
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

Picture::~Picture()
{
	if (texture)
		SDL_DestroyTexture(texture);
}


//---------------Animation-------------------
void Animation::Refresh()
{
	//�����Ŀ���
	if (intervalpos<interval)
	{
		intervalpos++;
		return;
	}
	else
	{
		intervalpos = 0;
	}
	if (pos<(int)tilepv.size() - 1 && !stoped)
	{
		pos++;
	}
	else
	{
		pos = 0;
		if (cycl != 0)
		{
			cyctimes++;
			if (cyctimes >= cycl)
				stoped = true;
		}
	}
}

//--------------------Painter----------------------------
Painter::Painter(SDL_Renderer* renderer, int cx, int cy, int cw, int ch) 
	:renderer_(renderer), x(cx), y(cy), w(cw), h(ch)
{
}

Painter::~Painter()
{
}

void Painter::Draw(const Tile& tile, int x, int y)
{
	//λ�鴫�䣬������ͼ
	SDL_Rect srcrect, dstrect;
	srcrect.x = tile.picx;
	srcrect.y = tile.picy;
	srcrect.w = tile.tilew;
	srcrect.h = tile.tileh;

	dstrect.x = x;
	dstrect.y = y;
	dstrect.w = tile.tilew;
	dstrect.h = tile.tileh;
	SDL_RenderCopy(renderer_, tile.pPic->texture, &srcrect, &dstrect);
}

void Painter::Draw(const Animation& ani, int x, int y)
{
	if (ani.Stoped())return;
	const Tile& tile = ani.GetTile();
	SDL_Rect srcrect, dstrect;
	srcrect.x = tile.picx;
	srcrect.y = tile.picy;
	srcrect.w = tile.tilew;
	srcrect.h = tile.tileh;

	dstrect.x = x;
	dstrect.y = y;
	dstrect.w = tile.tilew;
	dstrect.h = tile.tileh;

	SDL_RenderCopy(renderer_, tile.pPic->texture, &srcrect, &dstrect);
}

void Painter::Draw(const Animation& ani, int x, int y, int w, int h)
{
	if (ani.Stoped())return;
	const Tile& tile = ani.GetTile();

	SDL_Rect srcrect, dstrect;
	srcrect.x = tile.picx;
	srcrect.y = tile.picy;
	srcrect.w = tile.tilew;
	srcrect.h = tile.tileh;

	dstrect.x = x;
	dstrect.y = y;
	dstrect.w = w;
	dstrect.h = h;

	SDL_RenderCopy(renderer_, tile.pPic->texture, &srcrect, &dstrect);
}

void Painter::Draw(const Text& tex, int x, int y)
{
	//��������
	SDL_Rect dstrect;
	dstrect.x = tex.x;
	dstrect.y = tex.y;
	dstrect.w = tex.w;
	dstrect.h = tex.h;

	SDL_RenderCopy(renderer_, tex.texture, NULL, &dstrect);
}

void Painter::Erase()
{
	//��������ͼ������в���
	SDL_RenderClear(renderer_);
}

void Painter::Apply()
{
	//�ѻ���ҳ�������������Ļ�豸��
	SDL_RenderPresent(renderer_);
}