//*******************element.cpp**********************
//

#include "element.h"
using namespace std;
#pragma comment(lib,"Winmm.lib")

Painter* Elebase::pPainter=NULL;
Manager *Elebase::pManager=NULL;
int FullScrAni::flscrw=0;
int FullScrAni::flscrh=0;
HWND Button::hwnd=NULL;

void Elebase::SetAni(int id1,int id2,int id3)
{
	pAni=pManager->PickAni(id1,id2,id3);
}

void Rect::Draw()
{
	if(!pAni)return;
	if(!pAni->Stoped())pPainter->Draw(*pAni,x,y,w,h);
	pAni->Refresh();
}


int Grid::px=0,Grid::py=0,Grid::gw=0,Grid::gh=0;

void Grid::Draw()
{
	if(!pAni) throw Init_Error("�����ද��δ��ʼ����");
	if(!pAni->Stoped())pPainter->Draw(*pAni,px+gx*gw,py+gy*gh,gw,gh);
	pAni->Refresh();
}

Text::Text(int rx,int ry,int rw,int rh,const char* face):Rect(rx,ry,rw,rh),font(NULL)
{
	if(font)DeleteObject(font);
	//�������廭��
	font=CreateFont(h,w,0,0,FW_NORMAL,false,false,false,
					CHINESEBIG5_CHARSET,OUT_CHARACTER_PRECIS,
					CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
					FF_MODERN,"face");
}

void Text::Draw()
{
	if(pAni)Rect::Draw();
	pPainter->Draw(*this,x,y);
}


//--------------------------------------Button------------------------
Button::Button(int bx,int by,int bw,int bh)
	:Rect(bx,by,bw,bh),focus(false),keydown(false),
	pressed(false),orgx(bx),orgy(by),orgw(bw),orgh(bh)
{
}

void Button::Deal()//����ť����Ӧ�¼�
{
	if(!hwnd)throw Init_Error("��ť��δ��ô��ھ��!");
	POINT point;
	//��ȡ���ָ������
	GetCursorPos(&point);
	ScreenToClient(hwnd,&point);
	EnNormal();
	if(point.x>x&&point.x<x+w&&point.y>y&&point.y<y+h)
	{    //�ж����ָ���Ƿ��ڰ�ť������
		if(!focus)PlaySound("data\\mouseon.wav",NULL,SND_FILENAME|SND_ASYNC);
		focus=true;
		EnLarge();
		if(GetAsyncKeyState(VK_LBUTTON))
		{   //�������Ƿ���
			keydown=true;
			Shift();
		}
		else
			if(keydown)
			{   
				pressed=true;
				keydown=false;
				PlaySound("data\\click.wav",NULL,SND_FILENAME|SND_ASYNC);
			}
	}
	else focus=false;
	GetAsyncKeyState(VK_LBUTTON);
}

//-------------------SelBox
SelBox::SelBox(int sx,int sy,int sw,int sh,float scale)
	:Rect(sx,sy,sw,sh),leftbutton(sx,sy,int(sw/(scale+2)),sh),rightbutton(int(sx+sw-sw/(scale+2)),sy,int(sw/(scale+2)),sh),
	tex(int(sx+sw/(scale+2)),sy,int(sw-2*sw/(scale+2)),sh),curstr(0)
{
	
}
void SelBox::SetAni(int psb1,int plb1,int prb1,int psb2,int plb2,int prb2,int psb3,int plb3,int prb3)
{
	Elebase::SetAni(psb1,psb2,psb3);
	leftbutton.SetAni(plb1,plb2,plb3);
	rightbutton.SetAni(prb1,prb2,prb3);
}
void SelBox::Deal()
{
	
	leftbutton.Deal();
	rightbutton.Deal();
	if(leftbutton.MousePressed()&&curstr>0)
	{   //������Ұ�ť�İ���������Ӧ
		curstr--;
		leftbutton.Recover();
	}
	if(rightbutton.MousePressed()&&curstr<int(strlist.size()-1))
	{
		curstr++;
		rightbutton.Recover();
	}
	tex.SetText(strlist[curstr]);
}

void SelBox::Draw()
{
	if(pAni)Rect::Draw();
	tex.Draw();
	leftbutton.Draw();
	rightbutton.Draw();
}

Snake::Snake(int lenth,int v,int startx,int starty,int direc)
	:rate(v),direction(direc),curratepos(0),added(false),live(true),
	newdirect(RIGHT)
{
	//�ߵĹ���
	Node* head=new Node(startx--,starty);
	head->SetAni(ANI_SNAKE_HEAD);
	nodes.push_back(head);
	for(int i=1;i<lenth-1;i++)
	{
		Node* body=new Node(startx--,starty);
		body->SetAni(ANI_SNAKE_BODY);
		nodes.push_back(body);
	}
	Node* tail=new Node(startx--,starty);
	tail->SetAni(ANI_SNAKE_TAIL);
	nodes.push_back(tail);
}

Snake::~Snake()
{
	//�ߵ����
	for(int i=0;i<(int)nodes.size();i++)
		delete nodes[i];
	nodes.clear();
}

int Snake::HeadX()
{
	return Head()->X();
}

int Snake::HeadY()
{
	return Head()->Y();
}

bool Snake::Move()//�ߵ��ƶ�
{
	if(!live) return false;
	curratepos+=1000/FPS;
	if(curratepos>=rate)
	{
		curratepos=0;
		int hx=Head()->gx;
		int hy=Head()->gy;
		direction=newdirect;
		//�����ߵķ���ı���ͷ����
		switch(direction)
		{
			case RIGHT:
				hx++;
				break;
			case DOWN:
				hy++;
				break;
			case LEFT:
				hx--;
				break;
			case UP:
				hy--;
				break;
		}
		//�ƶ�
		for(vector<Node*>::iterator it=nodes.end()-1;it>nodes.begin();it--)	
		{
			if(it==nodes.end()-1 && added)
			{
				added=false;
				continue;
			}
			(*it)->gx=(*(it-1))->gx;
			(*it)->gy=(*(it-1))->gy;
			(*it)->direction=(*(it-1))->direction;
		}
		nodes[0]->gx=hx;
		nodes[0]->gy=hy;
		nodes[0]->direction=direction;
		return true;
	}
	return false;
}

bool Snake::IsBumpFood(int fx,int fy)//�Լ�����
{
	if(HeadX()==fx && HeadY()==fy)//ײ��ʳ��
	{
		AddNode();
		return true;
	}
	return false;
}

bool Snake::IsBumpSelf()
{
	if(OnBody(HeadX(),HeadY()))//ײ���Լ�
	{
		live=false;
	}
	return live;
}

void Snake::AddNode()//�����ӽڵ�
{
	Node* tail=nodes.back();
	Node* body=new Node(tail->X(),tail->Y());
	body->SetAni(ANI_SNAKE_BODY);
	nodes.insert(nodes.end()-1,body);
	added=true;
}

bool Snake::OnHead(int x,int y)
{
	return x==nodes[0]->gx && y==nodes[0]->gy;
}

bool Snake::OnBody(int x,int y)//����ͷ��β
{
	return OnTail(x,y)||OnBelly(x,y);
}

bool Snake::OnTail(int x,int y)
{
	return x==nodes.back()->gx && y==nodes.back()->gy;
}

bool Snake::OnBelly(int x,int y)//�ߵ��м䲿��
{
	for(vector<Node*>::iterator it=nodes.begin()+1;it<nodes.end()-1;it++)
		if(x==(*it)->gx && y==(*it)->gy) return true;
	return false;
}

void Snake::Draw() 
{
	//ѭ������������
	for(vector<Node*>::iterator it=nodes.begin();it<nodes.end();it++)
		(*it)->Draw();
}

Panel::Panel(int Level,int rectx,int recty,int rectw,int recth,int gridxnum,int gridynum)
		:level(Level),Rect(rectx,recty,rectw,recth),xnum(gridxnum),ynum(gridynum),
		snake(NULL),food(xnum,ynum),overed(false),won(false),score(0),pause(false)
{
	food.SetAni(ANI_FOOD);
	food.Generate();
	Grid::PreInit(x,y,w/xnum,h/ynum);
	snake=new Snake(int(level*2),int(400/level));//������

	SetAni(ANI_BACKGROUND,int(level/2+0.5)%9);//���ö���
}

void Panel::DoFrame()
{

	Draw();
	food.Draw();
	if(!pause)
	{
		//��ⰴ��
		if(GetAsyncKeyState(VK_RIGHT)) snake->Right();
		if(GetAsyncKeyState(VK_DOWN)) snake->Down();
		if(GetAsyncKeyState(VK_LEFT)) snake->Left();
		if(GetAsyncKeyState(VK_UP)) snake->Up();

		if(snake->Move())
		{
			if(snake->HeadX()<0)snake->Head()->gx=xnum-1;
			if(snake->HeadY()<0)snake->Head()->gy=ynum-1;
			if(snake->HeadX()>=xnum)snake->Head()->gx=0;
			if(snake->HeadY()>=ynum)snake->Head()->gy=0;
			ChkBump();
		}
	}
	snake->Draw();

	if(RestFood()<=0)//�ж�ʳ���Ƿ�ȫ������
	{
		overed=true;
		won=true;
		PlaySound("data\\win.wav",NULL,SND_FILENAME|SND_ASYNC);
	}
}

int Panel::RestFood()
{
	return level*3+3-snake->lenth();
}

void Panel::ChkBump()//��ײ���
{
	snake->IsBumpSelf();
	if(snake->IsBumpFood(food.gx,food.gy))//���Ƿ�ײ��ʳ��
	{
		score+=level*10;
		PlaySound("data\\eat.wav",NULL,SND_FILENAME|SND_ASYNC);
		do
		{
			bool onsnake=false;
			food.Generate();
		}while(snake->OnSnake(food.gx,food.gy));
	}
	if(!snake->IsLive())//���Ƿ��ǻ��
	{
		overed=true;
		won=false;
		PlaySound("data\\fail.wav",NULL,SND_FILENAME|SND_ASYNC);
	}
}

//��������������أ����ڴ浵�Ķ�ȡ�봢��
ofstream& operator<<(ofstream& ofs,const Node& node)
{
	ofs<<node.gx<<endl<<node.gy<<endl<<node.direction<<endl;
	return ofs;
}

ifstream& operator>>(ifstream& ifs,Node& node)
{
	ifs>>node.gx>>node.gy>>node.direction;
	return ifs;
}

ofstream& operator<<(ofstream& ofs,const Snake& snake)
{
	ofs<<snake.rate<<endl<<snake.direction<<endl<<snake.newdirect<<endl;
	for(vector<Node*>::const_iterator it=snake.nodes.begin();it<snake.nodes.end();it++)
		ofs<<(**it);
	return ofs;
}

ifstream& operator>>(ifstream& ifs,Snake& snake)
{
	ifs>>snake.rate>>snake.direction>>snake.newdirect;

	for(vector<Node*>::const_iterator it=snake.nodes.begin();it<snake.nodes.end();it++)
	{
		ifs>>**it;
	}
	return ifs;
}


ofstream& operator<<(ofstream& ofs,const Panel& panel)
{
	ofs<<panel.xnum<<endl<<panel.ynum<<endl
		<<panel.food.gx<<endl<<panel.food.gy<<endl
		<<panel.level<<endl<<panel.score<<endl;
	ofs<<*panel.snake;
	return ofs;
}

ifstream& operator>>(ifstream& ifs,Panel& panel)
{
	ifs>>panel.xnum>>panel.ynum
	>>panel.food.gx>>panel.food.gy
	>>panel.level>>panel.score;
	ifs>>*panel.snake;
	return ifs;
}
