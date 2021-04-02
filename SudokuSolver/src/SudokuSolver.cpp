#include <iostream>
#include <chrono>

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
};
struct Cell {
	Cell() :
		Num(0),
		Loc(0),
		IsFilled(0),
		IsFilledTemp(0),
		INO(0)
	{
		Num = 0;
		Loc = 0;
		for (bool& x : PossibleNums) {
			x = true;
		}
	}
	uint Num;
	uint Loc;
	uint INO;
	uint IsFilled : 1;
	uint IsFilledTemp : 1;
	bool PossibleNums[9];

	bool SetNum(uint x, bool btemp = false) {
		if (!IsFilled && x <= 9 && x > 0) {
			Num = x;
			btemp ? IsFilled : IsFilledTemp = true;
			return true;
		}
		return false;
	}
	bool SetNum(uint x, uint& CPencil, uint& CPen, bool btemp = false) {
		if (SetNum(x, btemp)) {
			CPen++;
			return true;
		}
		return false;
	}
};
class SudGrid {
	char grid[163] = "# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n";
public:
	void PrepareGridForPrinting(Cell* gv) {
		uint x = 0;
		for (char& c : grid)
			if (c != ' ' && c != '\n')
			{
				Cell& CurrCell = gv[x];
				if (CurrCell.Num != 0) {
					c = CurrCell.Num + '0';
					CurrCell.IsFilled = true;
				}
				else {
					CurrCell.IsFilled = false;
				}
				x++;
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
		SetPN(b1, b1, b1, b1, b1, b1, b1, b1, b1);
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
struct PuzzleInfo {
	Row rows[9];
	Col cols[9];
	Box boxs[9];
	Cell gridvals[81];

	uint Pencil=0;
	uint Pen=0;
	PuzzleInfo() : Pencil(0), Pen(0) {};
};

// are all cells filled
bool IsSolved(Cell* gridvals) {
	for (uint x = 0; x < 81; x++) {
		if (!gridvals[x].IsFilled) {
			return false;
		}
	}
	return true;
}

bool IsCellFilled(Cell& cv) {
	return (cv.IsFilled);
}

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
		if (IsCellFilled(currCell)) {
			std::cout << "Filled: " << gridvals[x].Num;
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
			if (IsCellFilled(CC))				// if CurrCell is filled
				if (CC.INO < 3) {
					CC.INO++;
					for (uint z = 0; z < 9; z++) {			// for every Cell in row (ECR)
						Cell& CurrCellR = CR.GetOffset(z, gv);	// Current Cell R is the first cell row + iteration to get the offset orcolumn
						CurrCellR.PossibleNums[CC.Num - 1] = false;
						CPencil++;
					}
					CR.PossibleNums[CC.Num - 1] = false;
				}
		}

}
void Solve_makeChanges(Cell* gridvals, uint& CPencil, uint& CPen) {
	// for every cell make changes
	for (uint CCNo = 0; CCNo < 81; CCNo++) {
		uint NoOfPN = 0;
		uint LastPN = 0;
		Cell& CC = gridvals[CCNo];

		for (uint CR = 0; CR < 9; CR++) {
			bool& y = CC.PossibleNums[CR];
			if (y == true) {
				NoOfPN++;
				LastPN = CR;
			}
		}
		if (NoOfPN == 1) {
			if (!IsCellFilled(CC)) {
				CC.Num = LastPN + 1;
				CC.IsFilled = true;
				++CPen;
			}
		}
	}
}
void Solve(PuzzleInfo& PZI) {
	Solve_clearPN_itrl(PZI.rows, PZI.gridvals, PZI.Pencil, PZI.Pen);
	Solve_clearPN_itrl(PZI.cols, PZI.gridvals, PZI.Pencil, PZI.Pen);
	Solve_clearPN_itrl(PZI.boxs, PZI.gridvals, PZI.Pencil, PZI.Pen);

	Solve_makeChanges(PZI.gridvals, PZI.Pencil, PZI.Pen);
}

void AdvancedSolve_itrl(RCB* _RCB, Cell* gv, uint& CPencil, uint& CPen) {
	for (uint x = 0; x < 9; x++)
		for (uint y = 0; y < 9; y++)
		{
			RCB& C1 = _RCB[x];
			Cell& CCC1 = C1.GetOffset(y, gv);

			if (!CCC1.IsFilled)
				for (uint PV = 0; PV < 9; PV++)
				{
					bool Fill = false;

					if (CCC1.PossibleNums[PV] == true)
					{
						for (uint CC2 = 0; CC2 < 9; CC2++)
						{
							if (y != CC2) {
								Cell& C2 = C1.GetOffset(CC2, gv);

								if (!C2.IsFilled)
									if (C2.PossibleNums[PV] == true) {
										Fill = false;
										break;		// jump out of loop CC2
									}
									else {
										Fill = true;
										continue;
									}
							}
						}
						if (Fill) {
							CCC1.SetNum(PV + 1, CPencil, CPen);
						}
					}
				}
		}
}

void AdvancedSolve(PuzzleInfo& PZI) {
	AdvancedSolve_itrl(PZI.rows, PZI.gridvals, PZI.Pencil, PZI.Pen);
	AdvancedSolve_itrl(PZI.cols, PZI.gridvals, PZI.Pencil, PZI.Pen);
	AdvancedSolve_itrl(PZI.boxs, PZI.gridvals, PZI.Pencil, PZI.Pen);
}

void ExtremeAdvancedSolve_itrl(PuzzleInfo& PZI) {
}
void ExtremeAdvancedSolve(PuzzleInfo& PZI) {
	PuzzleInfo PZI2 = PZI;
	ExtremeAdvancedSolve_itrl(PZI2);
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
		PuzzleInfo PZI;
		SudGrid SGrid;
		uint* pz = Puzzle.p1;

		// input stuff
		char a;
		bool bbreak = false;

		std::cout << "Choose Puzzle\n\n"
			<< "Easy	: 1\n"
			<< "Easy	: 2\n"
			<< "Medium	: 3\n"
			<< "Currently not solvable:\n"
			<< "Hard	: 4\n"
			<< std::endl << std::endl;
		while (!bbreak) {
			std::cin >> a;
			switch (a)
			{
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
			default:
				std::cout << "Invalid Puzzle no" << std::endl << std::endl;
				bbreak = false;
				break;
			}
		}
		NewPage();
		for (uint x = 0; x < 81; x++) {
			PZI.gridvals[x].Num = pz[x];
			PZI.gridvals[x].Loc = x;
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

		bbreak = false;
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
			bool bExtremeAdvancedSolve = false;

			Solve(PZI);
			if (PZI.Pen == 0 && PZI.Pencil == 0) {
				if (IsSolved(PZI.gridvals)) {
					EndTimepoint = std::chrono::high_resolution_clock::now();
					break;
				}

				//else
				bAdvanced = true;
				AdvancedSolve(PZI);

				if (PZI.Pen == 0 && PZI.Pencil == 0) {
					if (IsSolved(PZI.gridvals)) {
						EndTimepoint = std::chrono::high_resolution_clock::now();
						break;
					}
					bExtremeAdvancedSolve = true;
					ExtremeAdvancedSolve(PZI);
				}
			}
			SGrid.PrepareGridForPrinting(PZI.gridvals);

				if (bDebugMode || bExtremeDebugMode) {
					if (bExtremeDebugMode)
						ExtremeDebug(PZI.gridvals, PZI.Pencil, PZI.Pen);
					if (bExtremeAdvancedSolve)
						std::cout << "Iterations: " << iteration << " Type: BRUTE" << std::endl;
					else if (bAdvanced)
						std::cout << "Iterations: " << iteration << " Type: ADVANCED" << std::endl;
					else
						std::cout << "Iterations: " << iteration << " Type: BASIC" << std::endl;

					WriteHeader(PZI.Pencil, PZI.Pen, TotalChangesMade_Pencil, TotalChangesMade_Pen);
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