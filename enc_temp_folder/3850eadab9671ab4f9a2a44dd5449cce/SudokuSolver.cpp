#include <iostream>
#include <chrono>
#include <ctype.h>


#define NewPage() for(uint sksksk=0; sksksk<=100;sksksk++){std::cout << std::endl;}
typedef unsigned int uint;
enum PuzzleDifficulty {
	VeryEasy,
	Easy,
	Medium,
	Hard,
	VeryHard
};

struct Puzzles {
	uint custom[81];
	uint p1[81]{
		0,0,0,0,0,1,9,0,4,
		0,0,0,2,0,0,0,0,0,
		3,8,1,0,0,0,0,5,0,
		0,0,0,6,9,2,0,0,0,
		0,7,3,0,0,0,4,2,0,
		0,0,8,0,0,0,0,0,0,
		5,0,0,9,0,0,0,3,2,
		0,3,0,1,2,0,7,0,0,
		8,2,4,0,7,0,1,0,6
	};
	uint p2[81]{
		0,0,4,3,0,0,2,0,9,
		0,0,5,0,0,9,0,0,1,
		0,7,0,0,6,0,0,4,3,
		0,0,6,0,0,2,0,8,7,
		1,9,0,0,0,7,4,0,0,
		0,5,0,0,8,3,0,0,0,
		6,0,0,0,0,0,1,0,5,
		0,0,3,5,0,8,6,9,0,
		0,4,2,9,1,0,3,0,0
	};
	uint p3[81]{
		0,0,3,0,0,6,0,0,4,
		0,0,0,1,0,8,2,0,7,
		0,0,2,9,0,0,0,1,0,
		0,4,9,7,0,0,0,3,2,
		0,7,0,0,8,0,1,0,0,
		0,0,0,6,9,0,0,0,5,
		0,0,7,0,0,0,0,0,0,
		5,0,8,3,6,0,0,0,0,
		0,0,0,5,1,4,3,0,0
	};
	uint p4[81]{
	0,0,0,0,9,0,8,0,0,
	0,0,4,1,5,0,0,0,0,
	0,6,0,0,0,0,0,7,0,
	0,0,5,0,0,0,0,0,0,
	1,8,0,7,0,0,5,0,0,
	0,0,0,0,2,0,0,6,0,
	0,0,0,0,1,5,4,0,0,
	3,0,6,0,0,8,0,9,0,
	0,7,0,0,0,0,0,0,0
	};
	uint p5[81]{
		8,0,0,0,0,0,0,0,0,
		0,0,3,6,0,0,0,0,0,
		0,7,0,0,9,0,2,0,0,
		0,5,0,0,0,7,0,0,0,
		0,0,0,0,4,5,7,0,0,
		0,0,0,1,0,0,0,3,0,
		0,0,1,0,0,0,0,6,8,
		0,0,8,5,0,0,0,1,0,
		0,9,0,0,0,0,4,0,0
	};
	uint p6[81]{
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,1,0,0,0,2,0,0,
		0,3,0,4,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,5,0,6,0,0,0,
		0,0,7,0,0,0,8,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0
	};
};
struct Cell {
	Cell(const Cell& C)
	{
		Num = C.Num;
		INO = C.INO;
		for (uint x =0;x<9;x++) {
			PossibleNums[x] = C.PossibleNums[x];
		}
	}
	Cell() :
		Num(0),
		INO(0)
	{
		for (bool& x : PossibleNums) {
			x = true;
		}
	}
	uint INO;
	bool PossibleNums[9];

	void PrintPossibleNums() const
	{
		for (uint x = 0; x < 9; x++)
		{
			if (PossibleNums[x])
			{
				std::cout << x+1 << "|";
			}
		}
	}

	bool SetNum(uint x, bool bAdd = false) {
		if (!IsFilled() && x <= uint(9-bAdd) && x > 0) {
			Num = x + bAdd;
			INO = 0;
			return true;
		}
		//__debugbreak();
		return false;
	}
	bool SetNum(uint x, uint& CPencil, uint& CPen, bool bAdd = false) {
		if (SetNum(x, bAdd)) {
			CPen++;
			return true;
		}
		return false;
	}
	uint GetNum() const
	{
		return Num;
	}
	bool IsFilled() {
		return (Num!=0);
	}
private:
	uint Num;
};
class SudGrid {
	char grid[164] = "# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n ";
public:
	void PrepareGridForPrinting(Cell* gv) {
		uint x = 0;
		for (uint z =0;z < 163; z++)
		{
			char& c = grid[z];
			if (c != ' ' && c != '\n')
			{
				Cell& CurrCell = gv[x];
				c = CurrCell.GetNum() + '0';
				x++;
			}
		}
	}
	inline void Print(){
		for (uint x=0; x < 162; x++)
			std::cout << grid[x];
	}
};
// row column block
struct RCB {
	RCB()
	{
		SetPN(true);
	}
	bool PossibleNums[9];

	uint begin;
	uint end;

	void SetPN(bool b1) {
		for (bool& bx : PossibleNums)
			bx = b1;
	}
	void SetPN(bool b1,bool b2,bool b3,bool b4,bool b5,bool b6,bool b7,bool b8,bool b9) {
		PossibleNums[0] = b1;
		PossibleNums[1] = b2;
		PossibleNums[2] = b3;
		PossibleNums[3] = b4;
		PossibleNums[4] = b5;
		PossibleNums[5] = b6;
		PossibleNums[6] = b7;
		PossibleNums[7] = b8;
		PossibleNums[8] = b9;
	}
	inline void SetPN(bool b, uint pos) { if (pos < 9 && pos >= 0)PossibleNums[pos] = b; }
	inline void SetPN(bool b, uint pos, uint& CPencil) { if (pos < 9 && pos >= 0) { PossibleNums[pos] = b; CPencil++; } }
	
	virtual uint GetOffset(uint OffsetAmt) = 0;
	virtual Cell& GetOffset(uint OffsetAmt, Cell* gv) = 0;

	RCB(const RCB& rcb)
	{
		for (uint x = 0; x < 9; x++)
		{
			PossibleNums[x] = rcb.PossibleNums[x];
		}
	}
};
struct Row : RCB {
	virtual uint GetOffset(uint OffsetAmt) override {
		return begin + OffsetAmt;
	};
	virtual Cell& GetOffset(uint OffsetAmt, Cell* gv) override {
		return gv[GetOffset(OffsetAmt)];
	};
};
struct Col : RCB
{
	virtual uint GetOffset(uint OffsetAmt) override {
		return begin + (OffsetAmt * 9);
	};
	virtual Cell& GetOffset(uint OffsetAmt, Cell* gv) override {
		return gv[GetOffset(OffsetAmt)];
	};
};
struct Box : RCB
{
	virtual uint GetOffset(const uint OffsetAmt) override {
		uint z=0;
		if (OffsetAmt <= 2)
			z = 0;
		else if (OffsetAmt < 6)
			z = 1;
		else
			z = 2;
		return begin + OffsetAmt % 3 + 9 * z; // 00|01|02|09|
	};
	virtual Cell& GetOffset(uint OffsetAmt, Cell* gv) override {
		return gv[GetOffset(OffsetAmt)];
	};
};

class PuzzleState;
void Solve_clearPN_itrl(PuzzleState& PZI);

class PuzzleState {
public:
	Row rows[9];
	Col cols[9];
	Box boxs[9];
	Cell gridvals[81];

	uint Pencil=0;
	uint Pen=0;
	PuzzleState() : Pencil(0), Pen(0) {};
	PuzzleState(const PuzzleState& PZI) {
		Pencil = PZI.Pencil;
		Pen = PZI.Pen;

		for (uint x = 0; x < 9; x++) {
			rows[x] = (PZI.rows[x]);
			cols[x] = (PZI.cols[x]);
			boxs[x] = (PZI.boxs[x]);
		}
		for (uint x = 0; x < 81; x++) {
			gridvals[x] = Cell(PZI.gridvals[x]);
		}
	}
	bool IsInvalid() {

		Solve_clearPN_itrl(*this);
		bool rval = true;

		for (Cell& gz : gridvals)
		{
			if(gz.IsFilled()) continue;
			rval=true;
			for (bool& b : gz.PossibleNums)
			{
				if (b)
				{
					rval = false;
					break;
				}
			}
			if (!rval)
			{
				break;
			}
		}
		return rval;
	}
	// are all cells filled
	bool IsSolved() {
		for (auto& gv: gridvals) {
			if (gv.GetNum()==0) {
				return false;
			}
		}
		return true;
	}
};
uint GetRowNo(uint CellNo) {
	return (CellNo / 9) + 1;
}
uint GetColNo(uint CellNo) {
	return (CellNo % 9) + 1;
}
void WriteHeader(uint CPencil, uint CPen, uint TCPencil, uint TCPen) {
	std::cout << "Changes Made Pencil (Current Iteration): " << CPencil << std::endl;
	std::cout << "Changes Made Pen (Current Iteration):    " << CPen << std::endl;
	std::cout << "Changes Made Pencil (In Total): " << TCPencil << std::endl;
	std::cout << "Changes Made Pen (In Total):    " << TCPen << std::endl << std::endl;
}
void ExtremeDebug(Cell* gridvals, uint& CPencil, uint& CPen) {
	uint currRN = 1;
	for (uint x = 0; x < 81; x++) {
		std::cout << "Cell No: " << x << "	:	";
		Cell& currCell = gridvals[x];
		if (currCell.IsFilled()) {
			std::cout << "Filled: " << gridvals[x].GetNum();
			std::cout << std::endl;
		}
		else {
			for (uint y = 0; y < 9; y++) {
				std::cout << y + 1 << ": " << bool(currCell.PossibleNums[y] == true) << "	";
			}
			for (uint y = 0; y < 9; y++) {
				if (currCell.PossibleNums[y] == true)
					std::cout << y + 1 << " ";
			}

			std::cout << std::endl;
		}

		if (currRN != GetRowNo(x + 1)) {
			currRN = GetRowNo(x + 1);
			std::cout << std::endl;
		}
	}
}

void Solve_clearPN_itrl(RCB* _RCB, Cell* gv, uint& CPencil, uint& CPen) {
	// for every row reduce possibilities
	for (uint x = 0; x < 9; x++)
		for (uint y = 0; y < 9; y++) {	
			RCB& CR = _RCB[x];				
			Cell& CC = CR.GetOffset(y, gv);			// Current Cell alias
			if (CC.IsFilled())				// if CurrCell is filled
				if (CC.INO < 3) {
					CC.INO++;
					for (uint z = 0; z < 9; z++) {			// for every Cell in row (ECR)
						Cell& CurrCellR = CR.GetOffset(z, gv);	// Current Cell R is the first cell row + iteration to get the offset or column
						uint PV = CC.GetNum();
						CurrCellR.PossibleNums[PV-1] = false;
						CPencil++;
					}
					CR.PossibleNums[CC.GetNum() - 1] = false;
				}
		}
}
void Solve_clearPN_itrl(PuzzleState& PZI) {
	Solve_clearPN_itrl(PZI.rows, PZI.gridvals, PZI.Pencil, PZI.Pen);
	Solve_clearPN_itrl(PZI.cols, PZI.gridvals, PZI.Pencil, PZI.Pen);
	Solve_clearPN_itrl(PZI.boxs, PZI.gridvals, PZI.Pencil, PZI.Pen);
}
void Solve(PuzzleState& PZI) {
	// for every cell make changes
	for (uint CCNo = 0; CCNo < 81; CCNo++) {
		Solve_clearPN_itrl(PZI);

		uint NoOfPN = 0;
		uint LastPN = 0;
		Cell& CC = PZI.gridvals[CCNo];

		for (uint CR = 0; CR < 9; CR++) {
			if (CC.PossibleNums[CR]) {
				NoOfPN++;
				LastPN = CR;
			}
		}
		if (NoOfPN == 1) {
			if (!CC.IsFilled()) {
				if(LastPN+1==0 || LastPN+1==10)__debugbreak();
				CC.SetNum(LastPN + 1);
				++PZI.Pen;

				Solve_clearPN_itrl(PZI);
			}
		}
	}
}

void AdvancedSolve_itrl(PuzzleState& PZI, RCB* _RCB, Cell* gv, uint& CPencil, uint& CPen) {
	for (uint x = 0; x < 9; x++)
		for (uint y = 0; y < 9; y++)
		{
			RCB& RCB1 = _RCB[x];
			Cell& C1 = RCB1.GetOffset(y, gv);

			if (C1.IsFilled()) continue;
			for (uint PV = 0; PV < 9; PV++)
				if (C1.PossibleNums[PV])
				{
					bool bFill = false;
					for (uint z = 0; z < 9; z++)
						if (y != z) {
							Cell& C2 = RCB1.GetOffset(z, gv);
							if (C2.IsFilled()) continue;
							if (C2.PossibleNums[PV]) {
								bFill = false;
								break;		// jump out of loop CC2
							}
							else {
								bFill = true;
								continue;
							}
						}
					if (bFill)
					{
						Solve_clearPN_itrl(PZI.boxs, gv, CPencil, CPen);
						Solve_clearPN_itrl(PZI.rows, gv, CPencil, CPen);
						Solve_clearPN_itrl(PZI.cols, gv, CPencil, CPen);
						C1.SetNum(PV + 1, CPencil, CPen);
						Solve_clearPN_itrl(PZI.boxs, gv, CPencil, CPen);
						Solve_clearPN_itrl(PZI.rows, gv, CPencil, CPen);
						Solve_clearPN_itrl(PZI.cols, gv, CPencil, CPen);
					}
				}
		}
}

void AdvancedSolve(PuzzleState& PZI) {

	AdvancedSolve_itrl(PZI, PZI.rows, PZI.gridvals, PZI.Pencil, PZI.Pen);

	AdvancedSolve_itrl(PZI, PZI.cols, PZI.gridvals, PZI.Pencil, PZI.Pen);

	AdvancedSolve_itrl(PZI, PZI.boxs, PZI.gridvals, PZI.Pencil, PZI.Pen);
}

// WIP
// bool BruteSolve_itrl(PuzzleState& PZI, uint Cloc, uint it, uint rNum, bool bDebug) {
// 	SudGrid SGrid;
// 	while (true) {
// 		if (PZI.IsSolved()) return true;
// 		if (PZI.IsInvalid()) return false;
// 
// 		PZI.Pen = 0;
// 		PZI.Pencil = 0;
// 
// 		Solve(PZI);
// 		std::cout << "Solve set num \n";
// 		std::cout << "\n" << PZI.Pen << " " << PZI.Pencil << "\n";
// 		if(bDebug){
// 			SGrid.PrepareGridForPrinting(PZI.gridvals);
// 			SGrid.Print();
// 		}
// 		if (PZI.IsSolved()) return true;
// 		if (PZI.IsInvalid()) return false;
// 		std::cout << "\n";
// 		
// 		AdvancedSolve(PZI);
// 		std::cout << "AdvancedSolve set num \n";
// 		std::cout << "\n" << PZI.Pen << " " << PZI.Pencil << "\n";
// 		if (bDebug) {
// 			SGrid.PrepareGridForPrinting(PZI.gridvals);
// 			SGrid.Print();
// 		}
// 
// 		std::cout << "\n";
// 		//std::cin.get();
// 
// 		if (PZI.Pen == 0 && PZI.Pencil == 0) break;
// 
// 		std::cout << "\n" << PZI.Pen << " " << PZI.Pencil << "\n";
// 	}
// 	if (PZI.IsSolved()) return true;
// 	if (PZI.IsInvalid()) return false;
// 	
// 	if(Cloc>=81){__debugbreak();}
// 
// 	Cell& gv = PZI.gridvals[Cloc];
// 	if (!gv.IsFilled())
// 	{
// 		Solve_clearPN_itrl(PZI);
// 
// 		for (uint x = 0; x < 9; x++)
// 		{
// 			if (gv.PossibleNums[x]) // Set the first possible no found
// 			{
// 				if (it == 0) {
// 					gv.SetNum(x + 1);
// 					Solve_clearPN_itrl(PZI);
// 					std::cout << "Brute set num \n"<< rNum <<"\n";
// 					if (bDebug) {
// 						SGrid.PrepareGridForPrinting(PZI.gridvals);
// 						SGrid.Print();
// 					}
// 					std::cout << "\n";
// 					//std::cin.get();
// 					break;
// 				}
// 				else {
// 					it--;
// 				}
// 			}
// 		}
// 		if (PZI.IsSolved()) return true;
// 		if (PZI.IsInvalid()) return false;
// 		if(gv.IsFilled()) BruteSolve(PZI, Cloc+1, 0, bDebug);
// 	}
// 	if (PZI.IsSolved()) return true;
// 	if (PZI.IsInvalid()) return false;
// 	return false;
// }
//bool BruteSolve_itrl(PuzzleState& PZI, uint Cloc, uint it) {
// 	SudGrid SGrid;
// 	while (true) {
// 		if (PZI.IsSolved()) return true;
// 		if (PZI.IsInvalid()) return false;
// 
// 		PZI.Pen = 0;
// 		PZI.Pencil = 0;
// 
// 		Solve(PZI);
// 		std::cout << "Solve set num \n";
// 		std::cout << "\n" << PZI.Pen << " " << PZI.Pencil << "\n";
// 		SGrid.PrepareGridForPrinting(PZI.gridvals);
// 		SGrid.Print();
// 
// 		if (PZI.IsSolved()) return true;
// 		if (PZI.IsInvalid()) return false;
// 		std::cout << "\n";
// 		
// 		AdvancedSolve(PZI);
// 		std::cout << "AdvancedSolve set num \n";
// 		std::cout << "\n" << PZI.Pen << " " << PZI.Pencil << "\n";
// 		SGrid.PrepareGridForPrinting(PZI.gridvals);
// 		SGrid.Print();
// 
// 		std::cout << "\n";
// 		//std::cin.get();
// 
// 		if (PZI.Pen == 0 && PZI.Pencil == 0) break;
// 
// 		std::cout << "\n" << PZI.Pen << " " << PZI.Pencil << "\n";
// 	}
// 	if (PZI.IsSolved()) return true;
// 	if (PZI.IsInvalid()) return false;
// 	
// 	for (auto& gv : PZI.gridvals)
// 	{
// 		if (!gv.IsFilled())
// 		{
// 			Solve_clearPN_itrl(PZI);
// 
// 			for (uint x = 0; x < 9; x++)
// 			{
// 				if (gv.PossibleNums[x]) // Set the first poossible no found
// 				{
// 					if (it == 0) {
// 						gv.SetNum(x + 1);
// 						gv.IsFilled() = true;
// 						Solve_clearPN_itrl(PZI);
// 						std::cout << "Brute set num \n";
// 						SGrid.PrepareGridForPrinting(PZI.gridvals);
// 						SGrid.Print(); 
// 						std::cout << "\n";
// 						//std::cin.get();
// 						break;
// 					}
// 					else {
// 						it--;
// 					}
// 				}
// 			}
// 			if (PZI.IsSolved()) return true;
// 			if (PZI.IsInvalid()) return false;
// 			if(gv.IsFilled()) BruteSolve(PZI, Cloc+1, 0);
// 		}
// 	}
// 	if (PZI.IsSolved()) return true;
// 	if (PZI.IsInvalid()) return false;
// 	return false;
// }
// void BruteSolve(PuzzleState& PZI, uint Cloc, uint it, bool bDebug) {
// 	rNum++;
// 	PuzzleState PZI2 = PuzzleState(PZI);
// 
// 	uint NoOfPN = 0;
// 	uint LastPN = 0;
// 	Cell* CC = &PZI.gridvals[Cloc];
// 	while (CC->IsFilled()) {
// 		Cloc++;
// 		CC = &PZI.gridvals[Cloc];
// 	}
// 	for (uint CR = 0; CR < 9; CR++) {
// 		if (CC->PossibleNums[CR]) {
// 			NoOfPN++;
// 			LastPN = CR;
// 		}
// 	}
// 	SudGrid SGrid;
// 	//std::cin.get();
// 
// 	//todo 
// 	while(!BruteSolve_itrl(PZI2, Cloc, it, rNum, bDebug)) {
// 		std::cout << "INVALID ~-~" << Cloc << "\n";
// 		
// 		PZI2 = PuzzleState(PZI);
// 		
// 		SGrid.PrepareGridForPrinting(PZI2.gridvals);
// 		SGrid.Print();
// 		std::cout << "\n";
// 		//std::cin.get();
// 
// 		if(it <= NoOfPN)
// 		{
// 			it++;
// 		}
// 		else
// 		{
// 			if (rNum == 1)
// 			{
// 				std::cout << Cloc << " | " << LastPN; 
// 				PZI.gridvals[Cloc].SetNum(LastPN+1);
// 				Cloc++;
// 				rNum=0;
// 				NoOfPN=0;
// 				PZI2 = PuzzleState(PZI);
// 				BruteSolve(PZI, 0, 0, bDebug);
// 				return;
// 			}
// 			else
// 			{
// 				rNum--;
// 				return;
// 			}
// 		}
// 	}
// 	PZI = PuzzleState(PZI2);
// }
uint InceptionNo = 0;

bool BruteSolve(PuzzleState& PZI, bool bDebug)
{
	if (PZI.IsSolved())
	{
		return true; 
	}
	Solve_clearPN_itrl(PZI);

	InceptionNo++;
	SudGrid GridPrinter;

	uint SelectedCellNo;
	uint MinPnNo = 10;
	Cell* SelectedCell = &PZI.gridvals[0];

	// Find Cell with least possible values
	for (uint x = 0; x<81; x++)
	{
		Cell* CC = &PZI.gridvals[x];

		if(CC->IsFilled()) continue;

		uint PnNo = 0;
		for (bool& b : CC->PossibleNums)
		{
			if (b)
			{
				PnNo++;
			}
		}
		if(PnNo == 2)
		{
			MinPnNo = 2;
			SelectedCellNo = x;
			break;
		}
		if (PnNo < MinPnNo) {
			MinPnNo = PnNo;
			SelectedCellNo = x;
		}
	}
	/*Copy Current State into PZI 2 for temp Modifications PZI2 being a temporary state*/
	PuzzleState PZI2 = PuzzleState(PZI);

	SelectedCell = &PZI2.gridvals[SelectedCellNo];

	if(bDebug)
	{
		std::cout << "------------------------------------------------------------------\n";
		std::cout << "InceptionNo: "<< InceptionNo << "\n"
				  <<"Selected Cell: " <<SelectedCellNo	
				  << "\n[" << MinPnNo << "]: ";
		SelectedCell->PrintPossibleNums();
		std::cout << "------------------------------------------------------------------\n";
		std::cout << std::endl;

		GridPrinter.PrepareGridForPrinting(PZI2.gridvals);
		GridPrinter.Print();

		std::cout << std::endl;
		std::cout << std::endl;
	}

	// For Every Possible No
	for (uint x = 0; x<MinPnNo; x++)
	{
		PZI2 = PuzzleState(PZI);
		SelectedCell = &PZI2.gridvals[SelectedCellNo];

		uint it = x;

		uint SelectedPN; 
		for (uint z = 0; z<9; z++)
		{
			bool& zz = SelectedCell->PossibleNums[z];
			if (zz)
			{
				if (it != 0)
				{
					it--;
					continue;
				}
				SelectedPN = z;
				SelectedCell->SetNum(SelectedPN + 1);

				break;
			}
		}

		std::cout << std::endl;
		GridPrinter.PrepareGridForPrinting(PZI2.gridvals);
		GridPrinter.Print();

		while (true)
		{
			if (PZI2.IsSolved())
			{
				PZI = PuzzleState(PZI2);
				return true;
			}
			if (PZI2.IsInvalid())
			{
				PZI2 = PuzzleState(PZI);
				SelectedCell = &PZI2.gridvals[SelectedCellNo];
				
				GridPrinter.PrepareGridForPrinting(PZI2.gridvals);
				GridPrinter.Print();

				std::cout << "INVALID: Reset" << std::endl;
				if (bDebug)
				{
					std::cout << "-------------------------xxxxxxxxxx------------------------------\n";
					std::cout << "InceptionNo: " << InceptionNo << "\n"
						<< "Selected Cell: " << SelectedCellNo
						<< "\n[" << MinPnNo << "]: ";
					SelectedCell->PrintPossibleNums();
					std::cout << "------------------------------------------------------------------\n";
					std::cout << std::endl;

				}

				break;
			}

			PZI2.Pen = 0;
			PZI2.Pencil = 0;
			Solve(PZI2);
			AdvancedSolve(PZI2);
			std::cout << "\n" << PZI2.Pen << ":" << PZI2.Pencil << "\n";

			if (PZI2.IsSolved())
			{
				PZI = PuzzleState(PZI2);
				return true;
			}
			if (PZI2.IsInvalid())
			{
				PZI2 = PuzzleState(PZI);
				SelectedCell = &PZI2.gridvals[SelectedCellNo];
				
				GridPrinter.PrepareGridForPrinting(PZI2.gridvals);
				GridPrinter.Print();

				std::cout << "INVALID: Reset" << std::endl;
				if (bDebug)
				{
					std::cout << "-------------------------xxxxxxxxxx------------------------------\n";
					std::cout << "InceptionNo: " << InceptionNo << "\n"
						<< "Selected Cell: " << SelectedCellNo
						<< "\n[" << MinPnNo << "]: ";
					SelectedCell->PrintPossibleNums();
					std::cout << "------------------------------------------------------------------\n";
					std::cout << std::endl;

				}

				break;
			}

			if (PZI2.Pen == 0 && PZI2.Pencil == 0)
			{
				if((SelectedCell->IsFilled()) && BruteSolve(PZI2, bDebug))
				{
					PZI = PuzzleState(PZI2);
					return true;
				}
				else
				{
				break; // (While(true)).
				}
			}
		}
	}

	if (PZI.IsSolved())
	{
		return true;
	}

	--InceptionNo;
	return false;
}

void MakePuzzle(Puzzles& Puzzle)
{
	std::cout << "press (#r + enter) to reset" << std::endl;
	uint i = 0;
	while (true)
	{
		char in;
		std::cin >> in;
		if (isdigit(in))
		{
			Puzzle.custom[i] = in - '0';
			i++;
		}
		if(i==81)
			break;

		if (in =='r')
		{
			i=0;
		}
	}
}

void ChoosePuzzle(char& a, Puzzles& Puzzle ,uint*& pz) {
	bool bbreak = false;

	std::cout << "Choose Puzzle\n\n"
		<< "Custom	: x\n"
		<< "Easy	: 1\n"
		<< "Easy	: 2\n"
		<< "Medium	: 3\n"
		<< "Hard	: 4\n"
		<< "Currently not solvable:\n"
		<< "Very Hard	: 5\n"
		<< "Very Hard	: 6\n"
		<< std::endl << std::endl;
	while (!bbreak) {
		std::cin >> a;
		switch (a)
		{
		case 'x':
			std::cout << "Enter puzzle in csv format (Comma Separated)" << std::endl;
			pz = Puzzle.custom;
			MakePuzzle(Puzzle);
			bbreak = true;
			break;
		case '1':
			pz = Puzzle.p1;
			bbreak = true;
			break;
		case '2':
			pz = Puzzle.p2;
			bbreak = true;
			break;
		case '3':
			pz = Puzzle.p3;
			bbreak = true;
			break;
		case '4':
			pz = Puzzle.p4;
			bbreak = true;
			break;
		case '5':
			pz = Puzzle.p5;
			bbreak = true;
			break;
		case '6':
			pz = Puzzle.p6;
			bbreak = true;
			break;
		default:
			std::cout << "Invalid Puzzle no" << std::endl << std::endl;
			bbreak = false;
			break;
		}
	}
}
int main()
{
	while (true) {
		/*
			1st [0]
			2nd [2]
			3rd [4]
			4th [6]

			17 => 9th col

			   00 01 02 03 04 05 06 07 08
			00  #  #  #  #  #  #  #  #  # 08
			09  #  #  #  #  #  #  #  #  # 17
			18  #  #  #  #  #  #  #  #  # 26
			27  #  #  #  #  #  #  #  1  # 35
			36  #  #  #  #  #  #  #  2  # 44
			45  #  #  #  #  #  #  #  #  # 53
			54  #  #  #  #  #  #  #  #  # 62
			63  #  #  #  #  #  #  #  #  # 71
			72  #  #  #  #  #  #  #  #  # 80
			   72 73 74 75 76 77 78 79 80
		*/

		Puzzles Puzzle;
		PuzzleState PZI;
		SudGrid SGrid;
		uint* pz = Puzzle.p1;

		// input stuff
		char a;

		ChoosePuzzle(a, Puzzle, pz);

		NewPage();
		for (uint x = 0; x < 81; x++) {
			PZI.gridvals[x].SetNum(pz[x]);
		}
		uint z = 0;

		for (uint x = 0; x < 9; x++) {
			PZI.rows[x].begin = x * 9;		// 00|09|18|27...
			PZI.rows[x].end = (x * 9) + 8;// 08|17|26...

			PZI.cols[x].begin = x;			// 00|01|02...
			PZI.cols[x].end = x + 72;		// 72|73|74...

			PZI.boxs[x].begin = x % 3 * 3 + 27 * z;   // 00|03|06|27|30|33|54

			if (x % 3 == 2) {
				z++;
			}
		}

		uint iteration = 1;
		SGrid.PrepareGridForPrinting(PZI.gridvals);
		SGrid.Print();
		std::cout << std::endl;
		std::cout << "Press #f + enter# to open hint sheet" << std::endl;
		std::cout << "Press #a + enter# to solve in normal mode" << std::endl << std::endl;

		bool bBasicMode = false;
		bool bDebugMode = false;
		bool bExtremeDebugMode = false;
		bool bReturnToMenu = false;

		bool bbreak = false;
		while (!bbreak) {
			std::cin >> a;
			switch (a)
			{
			case 'f': {
				std::cout << "Press #b + enter# to solve in debug mode" << std::endl;
				std::cout << "Press #d + enter# to solve in extreme debug mode" << std::endl;
				std::cout << "Press #a + enter# to solve in normal mode" << std::endl << std::endl;
				std::cout << "Press #m + enter# to return to menu" << std::endl << std::endl;
				bbreak = false;
				break;
			}
			case 'a': {
				bBasicMode = true;
				bDebugMode = false;
				bExtremeDebugMode = false;
				bbreak = true;
				break;
			}
			case 'b': {
				bBasicMode = false;
				bDebugMode = true;
				bExtremeDebugMode = false;
				bbreak = true;
				break;
			}
			case 'd': {
				bBasicMode = false;
				bDebugMode = false;
				bExtremeDebugMode = true;
				bbreak = true;
				break;
			}
			case 'm': {
				bReturnToMenu = true;
				bbreak = true;
				break;
			}
			default: {
				std::cout << "Invalid command" << std::endl;
				bbreak = false;
				break;
			}
			}
		}

		if (bReturnToMenu) {
			NewPage();
			continue;
		}

		uint TotalChangesMade_Pencil = 0;
		uint TotalChangesMade_Pen = 0;

		typedef std::chrono::steady_clock::time_point TP;

		TP EndTimepoint;
		TP StartTimepoint = std::chrono::high_resolution_clock::now();
		while (true) {
			PZI.Pencil = 0;
			PZI.Pen = 0;
			bool bAdvanced = false;
			bool bBruteSolve = false;

			Solve(PZI);

			if (PZI.Pen == 0 && PZI.Pencil == 0) {
				if (PZI.IsSolved()) {
					EndTimepoint = std::chrono::high_resolution_clock::now();
					break;
				}

				AdvancedSolve(PZI);
				bAdvanced = true;
				if (PZI.Pen == 0 && PZI.Pencil == 0) {
					if (PZI.IsSolved()) {
						EndTimepoint = std::chrono::high_resolution_clock::now();
						break;
					}
					bBruteSolve = true;
					BruteSolve(PZI, bDebugMode ||  bExtremeDebugMode);
					EndTimepoint = std::chrono::high_resolution_clock::now();

					break;
				}
			}
				if (bDebugMode || bExtremeDebugMode) {
					if (bExtremeDebugMode)
						ExtremeDebug(PZI.gridvals, PZI.Pencil, PZI.Pen);
					if (bBruteSolve)
						std::cout << "Iterations: " << iteration << " Type: BRUTE" << std::endl;
					else if (bAdvanced)
						std::cout << "Iterations: " << iteration << " Type: ADVANCED" << std::endl;
					else
						std::cout << "Iterations: " << iteration << " Type: BASIC" << std::endl;

					WriteHeader(PZI.Pencil, PZI.Pen, TotalChangesMade_Pencil, TotalChangesMade_Pen);
					SGrid.PrepareGridForPrinting(PZI.gridvals);
					SGrid.Print();
					std::cout << std::endl << std::endl;
					iteration++;

					if (bExtremeDebugMode) {
						std::cin.get();
					}
				}
				TotalChangesMade_Pencil += PZI.Pencil;
				TotalChangesMade_Pen += PZI.Pen;
			}
			std::cout << std::endl << std::endl << std::endl;
			SGrid.PrepareGridForPrinting(PZI.gridvals);
			SGrid.Print();
			std::cout << std::endl << std::endl << std::endl;

			std::cout << "Press #m + enter# to return to menu" << std::endl;
			std::cout << "Press #s + enter# to show stats" << std::endl;

			bbreak = false;
			while (!bbreak) {
				std::cin >> a;
				switch (a)
				{
				case 'm': {
					NewPage();
					bbreak = true;
					break;
				}
				case 's': {
					std::cout << "Changes Made Pencil (In Total): " << TotalChangesMade_Pencil << std::endl;
					std::cout << "Changes Made Pen (In Total):    " << TotalChangesMade_Pen << std::endl << std::endl;
					long long start = std::chrono::time_point_cast<std::chrono::microseconds>(StartTimepoint).time_since_epoch().count();
					long long end = std::chrono::time_point_cast<std::chrono::microseconds>(EndTimepoint).time_since_epoch().count();
					std::cout << "TimeTaken (In Total):    " << end - start << " microseconds" << std::endl << std::endl;
					std::cout << "Press #m + enter# to return to menu" << std::endl;
					bbreak = false;
					break;
				}
				default: {
					std::cout << "InvalidCommand" << std::endl;
					bbreak = false;
					break;
				}
			}
		}
	}
}