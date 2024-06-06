#pragma once
#include <stdint.h>

class CCell
{	
	/*--------------FUNCS--------------*/
	public:
		CCell();
		CCell(const CCell& C);
		void Print() const;
		void SetNum(uint8_t _Num);

		const uint8_t GetPossibleNumCount(uint8_t& _LastPN) const;
		const uint8_t GetPossibleNumCount() const;
		uint8_t GetNum();
	protected:
	private:
	/*---------------------------------*/
	
	/*---------------VARS--------------*/
	public:
		uint16_t Data : 9;
		uint8_t bSet : 1;

		uint8_t bClearedRow : 1;
		uint8_t bClearedCol : 1;
		uint8_t bClearedBox : 1;
	/*---------------------------------*/
};