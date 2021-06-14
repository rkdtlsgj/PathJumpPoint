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
	/// <param name="iSX">�������� X ��ǥ</param>
	/// <param name="iSY">�������� Y ��ǥ</param>
	/// <param name="iDX">����~�������� X����</param>
	/// <param name="iDY">����~�������� Y����</param>
	void SerachBresenhamLine(std::list<stPOINT*>* point, int iSX, int iSY, int iDX, int iDY);
};