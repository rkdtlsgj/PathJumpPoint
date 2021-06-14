#pragma once
#include <list>
#include <iostream>
#include <Windows.h>
#include "BresenhamLine.h"

using namespace std;

#define MAPSIZE_X 40
#define MAPSIZE_Y 25

#define TILE_SIZE 25


#define DIR_LL 0
#define DIR_LU 1
#define DIR_UU 2
#define DIR_RR 3
#define DIR_RU 4
#define DIR_LD 5
#define DIR_RD 6
#define DIR_DD 7

enum TILE_TYPE
{
	NONE,
	OBJECT,
	BLOCK
};

struct NODE
{
	int iX;
	int iY;

	float fG;
	float fH;
	float fF;

	NODE* pParent;

};



//이건좀더 자세히 알아보자.
struct Compare
{
	bool operator()(const NODE* node1, const NODE* node2)
	{
		return node1->fF < node2->fF;
	}
};

class JumpPoint
{
public:
	JumpPoint();
	~JumpPoint();

	void Init();
	void ClearMap();

	bool Search(HWND hWnd);

	void CreateStart(int iX, int iY);
	void CreateEnd(int iX, int iY);
	void CreateBlock(int iX, int iY);
	void CreateDrag(int iX, int iY);

	void Draw(HDC hdc);
	void DrawLine(HDC hdc);

	NODE* FindNode(int iX, int iY);

private:

	NODE* CreateNode(int iX, int iY, float fG, NODE* pParent);
	void CheckNodeDir(int iX, int iY, NODE* pSearchNode,int iDir);
	bool FindNodeList(list<NODE*> list, int iX, int iY, NODE* node);
	bool CheckBlock(int iX, int iY);
	bool CheckBlank(int iX, int iY);


	bool Jump(int iX, int iY, int iDir,int* iJumpX, int* iJumpY);

	void SerachBresenham(NODE* pEndNode);
	BresenhamLine cBresenhamLine;

	TILE_TYPE m_map[MAPSIZE_Y][MAPSIZE_X];
	BYTE m_mapColor[MAPSIZE_Y][MAPSIZE_X];

	TILE_TYPE m_SelectBlock;

	//sort에 대해서 좀 알아보자
	list<NODE*> m_OpenList;
	list<NODE*> m_CloseList;
	NODE* m_EndNode;

	int m_iStartX;
	int m_iStartY;

	int m_iEndX;
	int m_iEndY;

	bool m_bFirst;

	HPEN hPen;
	HPEN hNullPen;

	BYTE color;
};

