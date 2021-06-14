#include "JumpPoint.h"


JumpPoint::JumpPoint()
{
	hNullPen = (HPEN)GetStockObject(NULL_PEN);
	hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));

	Init();
}

void JumpPoint::Init()
{
	ClearMap();

	m_iStartX = -1;
	m_iStartY = -1;

	m_iEndX = -1;
	m_iEndY = -1;

	m_bFirst = true;
}

void JumpPoint::ClearMap()
{
	for (int i = 0; i < MAPSIZE_Y; i++)
	{
		for (int j = 0; j < MAPSIZE_X; j++)
		{
			m_mapColor[i][j] = 255;

			if (m_map[i][j] != BLOCK)
			{
				m_map[i][j] = TILE_TYPE::NONE;				
			}
		}
	}
	list<NODE*>::iterator iter;
	for (iter = m_OpenList.begin(); iter != m_OpenList.end(); ++iter)
	{
		delete (*iter);
	}

	for (iter = m_CloseList.begin(); iter != m_CloseList.end(); ++iter)
	{
		delete (*iter);
	}

	m_bFirst = true;
	m_OpenList.clear();
	m_CloseList.clear();
	m_EndNode = NULL;
}

JumpPoint::~JumpPoint()
{
	DeleteObject(hNullPen);
	DeleteObject(hPen);

	list<NODE*>::iterator iter;
	for (iter = m_OpenList.begin(); iter != m_OpenList.end(); ++iter)
	{
		delete (*iter);
	}

	for (iter = m_CloseList.begin(); iter != m_CloseList.end(); ++iter)
	{
		delete (*iter);
	}

	m_OpenList.clear();
	m_CloseList.clear();
}


bool JumpPoint::Search(HWND hWnd)
{
	if (m_iStartX != -1 && m_iStartY != -1 && m_iEndX != -1 && m_iEndY != -1)
	{
		if (m_bFirst == true)
		{
			m_OpenList.push_front(CreateNode(m_iStartX, m_iStartY, 0, NULL));
			m_bFirst = false;
		}

		NODE* pSearchNode;

		if (m_OpenList.size() == 0)
			return true;

		m_OpenList.sort(Compare());
		pSearchNode = m_OpenList.front();
		m_OpenList.pop_front();

		m_CloseList.push_back(pSearchNode);

		if (pSearchNode->iX == m_iEndX && pSearchNode->iY == m_iEndY)
		{
			m_EndNode = pSearchNode;
			SendMessage(hWnd, WM_PAINT, 0, 0);
			InvalidateRect(hWnd, NULL, FALSE);
			return true;
		}

		color = rand() % 200 + 10;

		if (pSearchNode->pParent == NULL)
		{
			CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY, pSearchNode, DIR_LL);
			CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY - 1, pSearchNode, DIR_LU);
			CheckNodeDir(pSearchNode->iX, pSearchNode->iY - 1, pSearchNode, DIR_UU);

			CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY - 1, pSearchNode, DIR_RU);
			CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY, pSearchNode, DIR_RR);
			CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY + 1, pSearchNode, DIR_RD);

			CheckNodeDir(pSearchNode->iX, pSearchNode->iY + 1,pSearchNode, DIR_DD);	
			CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY + 1,  pSearchNode, DIR_LD);
			
		}
		else
		{
			int iDirX = pSearchNode->iX - pSearchNode->pParent->iX;
			int iDirY = pSearchNode->iY - pSearchNode->pParent->iY;


			iDirX = iDirX / max(abs(iDirX), 1);
			iDirY = iDirY / max(abs(iDirY), 1);


			//DD
			if (iDirX == 0 && iDirY == 1)
			{
				CheckNodeDir(pSearchNode->iX, pSearchNode->iY + 1, pSearchNode, DIR_DD);

				if (CheckBlock(pSearchNode->iX - 1, pSearchNode->iY) == true)
					CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY + 1, pSearchNode, DIR_LD);

				if (CheckBlock(pSearchNode->iX + 1, pSearchNode->iY) == true)
					CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY + 1, pSearchNode, DIR_RD);
			}
			//UU
			else if (iDirX == 0 && iDirY == -1)
			{
				CheckNodeDir(pSearchNode->iX, pSearchNode->iY - 1, pSearchNode, DIR_UU);

				if (CheckBlock(pSearchNode->iX - 1, pSearchNode->iY) == true)
					CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY - 1, pSearchNode, DIR_LU);

				if (CheckBlock(pSearchNode->iX + 1, pSearchNode->iY) == true)
					CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY - 1, pSearchNode, DIR_RU);
			}
			//RR
			else if (iDirX == 1 && iDirY == 0)
			{
				CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY, pSearchNode, DIR_RR);

				if (CheckBlock(pSearchNode->iX, pSearchNode->iY - 1) == true)
					CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY - 1, pSearchNode, DIR_RU);

				if (CheckBlock(pSearchNode->iX, pSearchNode->iY + 1) == true)
					CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY + 1, pSearchNode, DIR_RD);
			}
			//LL
			else if (iDirX == -1 && iDirY == 0)
			{
				CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY, pSearchNode, DIR_LL);

				if (CheckBlock(pSearchNode->iX , pSearchNode->iY - 1) == true)
					CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY - 1, pSearchNode, DIR_LU);

				if (CheckBlock(pSearchNode->iX, pSearchNode->iY + 1) == true)
					CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY + 1, pSearchNode, DIR_LD);
			}
			//RD
			else if (iDirX == 1 && iDirY == 1)
			{
				CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY, pSearchNode, DIR_RR);
				CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY + 1, pSearchNode, DIR_RD);
				CheckNodeDir(pSearchNode->iX, pSearchNode->iY + 1, pSearchNode, DIR_DD);

				if (CheckBlock(pSearchNode->iX, pSearchNode->iY - 1) == true)
					CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY - 1, pSearchNode, DIR_RU);

				if (CheckBlock(pSearchNode->iX - 1, pSearchNode->iY) == true)
					CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY + 1, pSearchNode, DIR_LD);
			}
			//RU
			else if (iDirX == 1 && iDirY == -1)
			{
				CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY, pSearchNode, DIR_RR);
				CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY - 1, pSearchNode, DIR_RU);
				CheckNodeDir(pSearchNode->iX, pSearchNode->iY - 1, pSearchNode, DIR_UU);

				if (CheckBlock(pSearchNode->iX - 1, pSearchNode->iY) == true)
					CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY - 1, pSearchNode, DIR_LU);

				if (CheckBlock(pSearchNode->iX, pSearchNode->iY + 1) == true)
					CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY + 1, pSearchNode, DIR_RD);
			}
			//LD
			else if (iDirX == -1 && iDirY == 1)
			{
				CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY, pSearchNode, DIR_LL);
				CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY + 1, pSearchNode, DIR_LD);
				CheckNodeDir(pSearchNode->iX, pSearchNode->iY + 1, pSearchNode, DIR_DD);

				if (CheckBlock(pSearchNode->iX, pSearchNode->iY-1) == true)
					CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY - 1, pSearchNode, DIR_LU);

				if (CheckBlock(pSearchNode->iX+1, pSearchNode->iY) == true)
					CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY + 1, pSearchNode, DIR_RD);
			}
			//LU
			else if (iDirX == -1 && iDirY == -1)
			{
				CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY, pSearchNode, DIR_LL);
				CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY - 1, pSearchNode, DIR_LU);
				CheckNodeDir(pSearchNode->iX, pSearchNode->iY - 1, pSearchNode, DIR_UU);

				if (CheckBlock(pSearchNode->iX+1, pSearchNode->iY) == true)
					CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY - 1, pSearchNode, DIR_RU);

				if (CheckBlock(pSearchNode->iX, pSearchNode->iY+1) == true)
					CheckNodeDir(pSearchNode->iX-1, pSearchNode->iY + 1, pSearchNode, DIR_LD);
			}
		/*	CheckNodeDir(pSearchNode->iX, pSearchNode->iY + 1, 1, pSearchNode);
			CheckNodeDir(pSearchNode->iX, pSearchNode->iY - 1, 1, pSearchNode);
			CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY, 1, pSearchNode);
			CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY, 1, pSearchNode);
			CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY + 1, 1.5, pSearchNode);
			CheckNodeDir(pSearchNode->iX + 1, pSearchNode->iY - 1, 1.5, pSearchNode);
			CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY + 1, 1.5, pSearchNode);
			CheckNodeDir(pSearchNode->iX - 1, pSearchNode->iY - 1, 1.5, pSearchNode);*/
		}

		InvalidateRect(hWnd, NULL, FALSE);
		SendMessage(hWnd, WM_PAINT, 0, 0);

		return false;
	}
	
	return true;
	

}

void JumpPoint::Draw(HDC hdc)
{
	list<NODE*>::iterator iter;
	HBRUSH hBrush, hBrushOld;
	HPEN hPenOld;

	hPenOld = (HPEN)SelectObject(hdc, hPen);

	for (int i = 0; i <= MAPSIZE_X; i++)
	{
		MoveToEx(hdc, (i * TILE_SIZE) + 5, 0, NULL);
		LineTo(hdc, (i * TILE_SIZE) + 5, MAPSIZE_Y * TILE_SIZE);
	}

	for (int i = 0; i <= MAPSIZE_Y; i++)
	{
		MoveToEx(hdc, 5, i * TILE_SIZE, NULL);
		LineTo(hdc, (MAPSIZE_X * TILE_SIZE) + 5, i * TILE_SIZE);
	}

	SelectObject(hdc, hPenOld);
	hPenOld = (HPEN)SelectObject(hdc, hNullPen);




	hBrush = CreateSolidBrush(RGB(128, 128, 128));
	hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

	for (int iY = 0; iY < MAPSIZE_Y; iY++)
	{
		for (int iX = 0; iX < MAPSIZE_X; iX++)
		{
			if (m_map[iY][iX] == TILE_TYPE::BLOCK)
			{
				Rectangle(hdc, iX * TILE_SIZE + 7, iY * TILE_SIZE + 2,
					(iX * TILE_SIZE + 5) + TILE_SIZE, (iY * TILE_SIZE) + TILE_SIZE);
			}
		}
	}



	for (int iY = 0; iY < MAPSIZE_Y; iY++)
	{
		for (int iX = 0; iX < MAPSIZE_X; iX++)
		{
			if (m_mapColor[iY][iX] < 255)
			{
				hBrush = CreateSolidBrush(m_mapColor[iY][iX]);
				hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, iX * TILE_SIZE + 7, iY * TILE_SIZE + 2,
					(iX * TILE_SIZE + 5) + TILE_SIZE, (iY * TILE_SIZE) + TILE_SIZE);

				SelectObject(hdc, hBrushOld);
				DeleteObject(hBrush);
			}


		}
	}


	hBrush = CreateSolidBrush(RGB(50, 50, 255));
	hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

	for (iter = m_OpenList.begin(); iter != m_OpenList.end(); ++iter)
	{
		Rectangle(hdc, (*iter)->iX * TILE_SIZE + 7, (*iter)->iY * TILE_SIZE + 2,
			((*iter)->iX * TILE_SIZE + 5) + TILE_SIZE, ((*iter)->iY * TILE_SIZE) + TILE_SIZE);

	}

	SelectObject(hdc, hBrushOld);
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(RGB(255, 255, 0));
	hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

	for (iter = m_CloseList.begin(); iter != m_CloseList.end(); ++iter)
	{
		Rectangle(hdc, (*iter)->iX * TILE_SIZE + 7, (*iter)->iY * TILE_SIZE + 2,
			((*iter)->iX * TILE_SIZE + 5) + TILE_SIZE, ((*iter)->iY * TILE_SIZE) + TILE_SIZE);
	}

	SelectObject(hdc, hBrushOld);
	DeleteObject(hBrush);

	if (m_iStartX > -1 && m_iStartY > -1)
	{
		hBrush = CreateSolidBrush(RGB(0, 255, 0));
		hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

		Rectangle(hdc, m_iStartX * TILE_SIZE + 7, m_iStartY * TILE_SIZE + 2,
			(m_iStartX * TILE_SIZE + 5) + TILE_SIZE, (m_iStartY * TILE_SIZE) + TILE_SIZE);

		SelectObject(hdc, hBrushOld);
		DeleteObject(hBrush);
	}

	if (m_iEndX > -1 && m_iEndY > -1)
	{
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

		Rectangle(hdc, m_iEndX * TILE_SIZE + 7, m_iEndY * TILE_SIZE + 2,
			(m_iEndX * TILE_SIZE + 5) + TILE_SIZE, (m_iEndY * TILE_SIZE) + TILE_SIZE);

		SelectObject(hdc, hBrushOld);
		DeleteObject(hBrush);
	}

	SelectObject(hdc, hPenOld);
	SelectObject(hdc, hBrushOld);

	DeleteObject(hBrush);
	DeleteObject(hPenOld);


}

NODE* JumpPoint::FindNode(int iX, int iY)
{
	if (iX >= 0 && iX < MAPSIZE_X && iY >= 0 && iY < MAPSIZE_Y)
	{
		list<NODE*>::iterator iter;
		for (iter = m_OpenList.begin(); iter != m_OpenList.end(); ++iter)
		{
			if ((*iter)->iX == iX && (*iter)->iY == iY)
			{
				return (*iter);
			}
		}
		for (iter = m_CloseList.begin(); iter != m_CloseList.end(); ++iter)
		{
			if ((*iter)->iX == iX && (*iter)->iY == iY)
			{
				return (*iter);
			}
		}

	}

	return NULL;
}

void JumpPoint::DrawLine(HDC hdc)
{
	if (m_EndNode != NULL)
	{
		HPEN hPenOld;
		hPenOld = (HPEN)SelectObject(hdc, hPen);

		NODE* tempNode = m_EndNode;
		while (tempNode->pParent != NULL)
		{
			MoveToEx(hdc, (tempNode->iX * TILE_SIZE + 7) + (TILE_SIZE / 2), (tempNode->iY * TILE_SIZE) + (TILE_SIZE / 2), NULL);
			LineTo(hdc, (tempNode->pParent->iX * TILE_SIZE + 7) + (TILE_SIZE / 2), (tempNode->pParent->iY * TILE_SIZE) + (TILE_SIZE / 2));

			tempNode = tempNode->pParent;
		}

		SelectObject(hdc, hPenOld);

		SerachBresenham(m_EndNode);
	}
}

void JumpPoint::CheckNodeDir(int iX, int iY,  NODE* pSearchNode,int iDir)
{
	if (iX >= 0 && iX < MAPSIZE_X && iY >= 0 && iY < MAPSIZE_Y)
	{
		if (m_map[iY][iX] != TILE_TYPE::BLOCK && m_map[iY][iX] != TILE_TYPE::OBJECT)
		{

			int iJumpX;
			int iJumpY;

			if (Jump(iX, iY, iDir, &iJumpX, &iJumpY) == true)
			{
				int newG = pSearchNode->fG + abs(iJumpX - pSearchNode->iX) + abs(iJumpY - pSearchNode->iY);
				//int newG = pSearchNode->fG + 1;

				list<NODE*>::iterator iter;
			

			/*	for (iter = m_CloseList.begin(); iter != m_CloseList.end(); ++iter)
				{
					if ((*iter)->iX == iX && (*iter)->iY == iY)
					{
						if ((*iter)->fG > newG)
						{
							(*iter)->fG = newG;
							(*iter)->fF = newG + (*iter)->fH;
							(*iter)->pParent = pSearchNode;							
						}


						return;
					}
				}*/
				for (iter = m_OpenList.begin(); iter != m_OpenList.end(); ++iter)
				{
					if ((*iter)->iX == iX && (*iter)->iY == iY)
					{
						if ((*iter)->fG > newG)
						{
							(*iter)->fG = newG;
							(*iter)->fF = newG + (*iter)->fH;
							(*iter)->pParent = pSearchNode;
						}

					/*	m_OpenList.erase(iter);
						m_OpenList.push_front(pSearchNode);*/
						return;
					}
				}


				NODE* newNode = CreateNode(iJumpX, iJumpY, newG, pSearchNode);
				m_map[iJumpY][iJumpX] = TILE_TYPE::OBJECT;
				m_OpenList.push_back(newNode);
			}			
		}
	}
}

bool JumpPoint::Jump(int iX, int iY, int iDir, int* iJumpX, int* iJumpY)
{
	if (iX >= 0 && iX < MAPSIZE_X && iY >= 0 && iY < MAPSIZE_Y)
	{
		if (m_map[iY][iX] != TILE_TYPE::BLOCK)
		{
			m_mapColor[iY][iX] = color;

			if (iX == m_iEndX && iY == m_iEndY)
			{
				*iJumpX = iX;
				*iJumpY = iY;

				return true;
			}


			switch (iDir)
			{
				//后 寒
				//   唱
				//后 寒
			case DIR_LL:
				if ((CheckBlock(iX, iY - 1) && CheckBlank(iX - 1, iY - 1)) ||
					(CheckBlock(iX, iY + 1) && CheckBlank(iX - 1, iY + 1)))
				{
					*iJumpX = iX;
					*iJumpY = iY;
					return true;
				}

				return Jump(iX - 1, iY, iDir, iJumpX, iJumpY);

				//      后
				//   唱 寒
				//后 寒 

			case DIR_LU:
				if ((CheckBlock(iX+1, iY) && CheckBlank(iX + 1, iY - 1)) ||
					(CheckBlock(iX, iY + 1) && CheckBlank(iX - 1, iY + 1)))
				{
					*iJumpX = iX;
					*iJumpY = iY;
					return true;
				}

				if (Jump(iX - 1, iY, DIR_LL, iJumpX, iJumpY) || Jump(iX, iY - 1, DIR_UU, iJumpX, iJumpY))
				{
					*iJumpX = iX;
					*iJumpY = iY;
					return true;
				}

				return Jump(iX - 1, iY-1, iDir, iJumpX, iJumpY);

				//后    后
				//寒 唱 寒
			case DIR_UU:
				if ((CheckBlock(iX - 1, iY) && CheckBlank(iX - 1, iY - 1)) ||
					(CheckBlock(iX+1, iY) && CheckBlank(iX + 1, iY - 1)))
				{
					*iJumpX = iX;
					*iJumpY = iY;
					return true;
				}

				return Jump(iX, iY - 1, iDir, iJumpX, iJumpY);

				//寒 后
				//唱  
				//寒 后
			case DIR_RR:
				if ((CheckBlock(iX , iY-1) && CheckBlank(iX + 1, iY - 1)) ||
					(CheckBlock(iX, iY+1) && CheckBlank(iX + 1, iY + 1)))
				{
					*iJumpX = iX;
					*iJumpY = iY;
					return true;
				}

				return Jump(iX+1, iY, iDir, iJumpX, iJumpY);

				//后     
				//寒 唱 
				//   寒 后
			case DIR_RU:
				if ((CheckBlock(iX - 1, iY) && CheckBlank(iX - 1, iY - 1)) ||
					(CheckBlock(iX, iY + 1) && CheckBlank(iX + 1, iY + 1)))
				{
					*iJumpX = iX;
					*iJumpY = iY;
					return true;
				}

				if (Jump(iX + 1, iY, DIR_RR, iJumpX, iJumpY) || Jump(iX, iY - 1, DIR_UU, iJumpX, iJumpY))
				{
					*iJumpX = iX;
					*iJumpY = iY;
					return true;
				}

				return Jump(iX + 1, iY - 1, iDir, iJumpX, iJumpY);

				//   寒 后  
				//寒 唱 
				//后
			case DIR_RD:
				if ((CheckBlock(iX - 1, iY) && CheckBlank(iX - 1, iY + 1)) ||
					(CheckBlock(iX, iY - 1) && CheckBlank(iX + 1, iY - 1)))
				{
					*iJumpX = iX;
					*iJumpY = iY;
					return true;
				}

				if (Jump(iX + 1, iY, DIR_RR, iJumpX, iJumpY) || Jump(iX, iY + 1, DIR_DD, iJumpX, iJumpY))
				{
					*iJumpX = iX;
					*iJumpY = iY;
					return true;
				}

				return Jump(iX + 1, iY + 1, iDir, iJumpX, iJumpY);

				//后 寒    
				//   唱 寒 
				//      后
			case DIR_LD:
				if ((CheckBlock(iX , iY-1) && CheckBlank(iX - 1, iY - 1)) ||
					(CheckBlock(iX+1, iY) && CheckBlank(iX + 1, iY + 1)))
				{
					*iJumpX = iX;
					*iJumpY = iY;
					return true;
				}

				if (Jump(iX - 1, iY, DIR_LL, iJumpX, iJumpY) || Jump(iX, iY + 1, DIR_DD, iJumpX, iJumpY))
				{
					*iJumpX = iX;
					*iJumpY = iY;
					return true;
				}

				return Jump(iX - 1, iY + 1, iDir, iJumpX, iJumpY);

				//    
			//寒 唱 寒
			//后    后
			case DIR_DD:
				if ((CheckBlock(iX - 1, iY) && CheckBlank(iX - 1, iY + 1)) ||
					(CheckBlock(iX + 1, iY) && CheckBlank(iX + 1, iY + 1)))
				{
					*iJumpX = iX;
					*iJumpY = iY;
					return true;
				}

				return Jump(iX, iY + 1, iDir, iJumpX, iJumpY);
			}
		}
	}

	return false;
}

bool JumpPoint::CheckBlock(int iX, int iY)
{
	if (iX >= 0 && iX < MAPSIZE_X && iY >= 0 && iY < MAPSIZE_Y)
	{
		if (m_map[iY][iX] == TILE_TYPE::BLOCK)
		{
			return true;
		}
	}

	return false;
}

bool JumpPoint::CheckBlank(int iX, int iY)
{
	if (iX >= 0 && iX < MAPSIZE_X && iY >= 0 && iY < MAPSIZE_Y)
	{
		if (m_map[iY][iX] == TILE_TYPE::NONE || m_map[iY][iX] == TILE_TYPE::OBJECT)
		{
			return true;
		}
	}

	return false;
}

NODE* JumpPoint::CreateNode(int iX, int iY, float fG, NODE* pParent)
{
	NODE* newNode = new NODE;
	newNode->iX = iX;
	newNode->iY = iY;
	newNode->pParent = pParent;

	newNode->fG = fG;
	newNode->fH = abs(iX - m_iEndX) + abs(iY - m_iEndY);
	newNode->fF = fG + newNode->fH;

	return newNode;
}

void JumpPoint::CreateStart(int iX, int iY)
{
	if (iX < MAPSIZE_X && iY < MAPSIZE_Y)
	{
		m_iStartX = iX;
		m_iStartY = iY;
	}
}

void JumpPoint::CreateEnd(int iX, int iY)
{
	if (iX < MAPSIZE_X && iY < MAPSIZE_Y)
	{
		m_iEndX = iX;
		m_iEndY = iY;

	}
}

bool JumpPoint::FindNodeList(list<NODE*> nodeList, int iX, int iY, NODE* node)
{
	list<NODE*>::iterator iter;

	for (iter = m_OpenList.begin(); iter != m_OpenList.end(); ++iter)
	{
		if ((*iter)->iX == iX && (*iter)->iY == iY)
		{
			node = (*iter);
			return true;
		}
	}

	return false;
}

void JumpPoint::CreateBlock(int iX, int iY)
{
	if (iX < MAPSIZE_X && iY < MAPSIZE_Y)
	{
		m_SelectBlock = m_map[iY][iX];

		if (m_map[iY][iX] != TILE_TYPE::BLOCK)
		{
			m_map[iY][iX] = TILE_TYPE::BLOCK;
		}
		else if (m_map[iY][iX] == TILE_TYPE::BLOCK)
		{
			m_map[iY][iX] = TILE_TYPE::NONE;
		}
	}
}

void JumpPoint::CreateDrag(int iX, int iY)
{
	if (iX < MAPSIZE_X && iY < MAPSIZE_Y)
	{
		if (m_SelectBlock != TILE_TYPE::BLOCK)
		{
			m_map[iY][iX] = TILE_TYPE::BLOCK;
		}
		else if (m_SelectBlock == TILE_TYPE::BLOCK)
		{
			m_map[iY][iX] = TILE_TYPE::NONE;
		}
	}
}

void JumpPoint::SerachBresenham(NODE* pEndNode)
{
	list<stPOINT*> pointList;
	NODE* pTempNode = pEndNode;
	NODE* pParentNode = pTempNode->pParent->pParent;
	bool isBlock = false;
	while (pParentNode != NULL)
	{
		isBlock = false;
		cBresenhamLine.SerachBresenhamLine(&pointList, pTempNode->iX, pTempNode->iY, pParentNode->iX, pParentNode->iY);

		std:list<stPOINT*>::iterator iter;
		for (iter = pointList.begin(); iter != pointList.end();++iter)
		{
			if (m_map[(*iter)->iY][(*iter)->iX] == TILE_TYPE::BLOCK)
			{
				isBlock = true;
				break;
			}
		}

		if (isBlock == false)
		{
			pTempNode->pParent = pTempNode->pParent->pParent;		
		}
		else
		{
			pTempNode = pTempNode->pParent;
		}

		pParentNode = pTempNode->pParent->pParent;
		pointList.clear();
	}
}

