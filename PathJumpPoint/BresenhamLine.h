#pragma once
#include <list>


struct  stPOINT
{
	int iX;
	int iY;

};
class BresenhamLine
{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="iSX">시작지점 X 좌표</param>
	/// <param name="iSY">시작지점 Y 좌표</param>
	/// <param name="iDX">시작~도착지점 X길이</param>
	/// <param name="iDY">시작~도착지점 Y길이</param>
	void SerachBresenhamLine(std::list<stPOINT*>* point, int iSX, int iSY, int iDX, int iDY);
};