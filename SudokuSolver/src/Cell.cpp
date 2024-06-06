#include "Cell.h"
#include <iostream>

CCell::CCell() :
	bSet(0),	
	bClearedRow(0),
	bClearedCol(0),
	bClearedBox(0)
{
	Data ^= ~Data;
}

CCell::CCell(const CCell& C) :
	bSet(C.bSet),
	bClearedRow(C.bClearedRow),
	bClearedCol(C.bClearedCol),
	bClearedBox(C.bClearedBox),
	Data(C.Data)
{
}

void CCell::Print() const
{
	if (bSet) std::cout << +Data << ": ";

	for (uint8_t i = 0; i < 9; i++)
		std::cout << (bool(Data & (0x01 << i)) ? +(i+1) : 0 ) << "|";
	std::cout << "\n";
}

void CCell::SetNum(uint8_t _Num)
{
	if(bSet) __debugbreak();
	if(_Num == 0) return;
	bSet = 1;
	Data = _Num;
}

const uint8_t CCell::GetPossibleNumCount(uint8_t& _LastPN) const
{
	uint8_t NoOfPN = 0;
	for (uint8_t i = 0; i < 9; i++)
	{
		if (Data & (0x01 << i))
		{
			NoOfPN++;
			_LastPN = i;
		}
	}
	return NoOfPN;
}
const uint8_t CCell::GetPossibleNumCount() const
{
	uint8_t NoOfPN = 0;
	for (uint8_t i = 0; i < 9; i++)
	{
		if (Data & (0x01 << i))
		{
			NoOfPN++;
		}
	}
	return NoOfPN;
}

uint8_t CCell::GetNum()
{
	return bSet ? uint8_t(Data) : 0;
}