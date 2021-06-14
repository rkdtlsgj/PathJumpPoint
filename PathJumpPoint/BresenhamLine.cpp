#include "BresenhamLine.h"


void BresenhamLine::SerachBresenhamLine(std::list<stPOINT*>* pointList,int iSX, int iSY,int iEX, int iEY)
{
	int iX = iSX;
	int iY = iSY;
	

	int iDX = abs(iSX - iEX);
	int iDY = abs(iSY - iEY);

	int iYStep = 1;
	if (iSY > iEY)
	{
		iYStep = -1;
	}

	int iXStep = 1;
	if (iSX > iEX)
	{
		iXStep = -1;
	}

	float fError = 0;
	//float fErrorX = (float)iDX / 2;
	//float fErrorY = (float)iDY / 2;

	stPOINT* point = new stPOINT;
	point->iX = iX;
	point->iY = iY;

	pointList->push_back(point);

	if (iDX > iDY)
	{
		fError = (float)iDX / 2;
		for (int i = 0; i < iDX; i++)
		{
			iX += iXStep;
			fError += iDY;

			if (fError >= iDX)
			{
				iY += iYStep;
				fError -= iDX;
			}

			stPOINT* point2 = new stPOINT;
			point2->iX = iX;
			point2->iY = iY;

			pointList->push_back(point2);
		}
	}
	else
	{
		fError = (float)iDY / 2;
		for (int i = 0; i < iDY; i++)
		{
			iY += iYStep;
			fError += iDX;

			if (fError >= iDY)
			{
				iX += iXStep;
				fError -= iDY;

			}


			stPOINT* point2 = new stPOINT;
			point2->iX = iX;
			point2->iY = iY;

			pointList->push_back(point2);
		}

	}
}