#include <iostream>
#include <chrono>

std::string grid;
std::string defgrid;

#define NewPage() for(int sksksk=0; sksksk<=100;sksksk++){std::cout << std::endl;}

enum PuzzleDifficulty{
	VeryEasy,
	Easy,
	Medium,
	Hard,
	VeryHard
};

struct Puzzles {
	int p1[81]{
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
	int p2[81]{
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
	int p3[81]{
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
	int p4[81]{
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
		IsIteratedR(0),
		IsIteratedC(0),
		IsIteratedB(0)
	{
		Num = 0;
		Loc = 0;
		for (bool& x : PossibleNums) {
			x = true;
		}
	}
	int Num;
	int Loc;
	typedef unsigned int uint;
	uint IsFilled : 1;
	uint IsFilledTemp : 1;
	uint IsIteratedR : 1; // if filled
	uint IsIteratedC : 1; // if filled
	uint IsIteratedB : 1; // if filled
	bool PossibleNums[9];

	bool SetNum(int x, bool btemp=false) {
		if (!IsFilled && x <= 9 && x > 0) {
			Num = x;
			btemp ? IsFilled:IsFilledTemp = true;
			return true;
		}
		return false;
	}
	bool SetNum(int x, int& CPencil, int& CPen, bool btemp = false) {
		if(SetNum(x, btemp)){
			CPen++;
			return true;
		}
	return false;
	}
};
// row column block
struct RCB {
	RCB()
	{
		SetPN(true);
	}
    bool PossibleNums[9];
	int begin;
	int end;

	void SetPN(bool b1){
		SetPN(b1, b1, b1, b1, b1, b1, b1, b1, b1);
	}
	void SetPN(
		bool b1,
		bool b2,
		bool b3,
		bool b4,
		bool b5,
		bool b6,
		bool b7,
		bool b8,
		bool b9) {
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
	void SetPN(bool b, int pos){
		if(pos<9 && pos >= 0)
			PossibleNums[pos] = b;
	}
};
struct PuzzleInfo {
	RCB rows[9];
	RCB cols[9];
	RCB boxs[9];
	Cell gridvals[81];
};

// are all cells filled
bool IsSolved(Cell* gridvals, bool bCheckForTemp){
	for (int x = 0; x < 81; x++) {
		if (!gridvals[x].IsFilled || bCheckForTemp ? !gridvals[x].IsFilledTemp : false) {
			return false;
		}
	}
	return true;
}

bool IsCellFilled(Cell& cv, bool bCheckForTemp) {
	bool pp = false;
	if (bCheckForTemp) {
		pp = cv.IsFilledTemp;
	}
	return (cv.IsFilled || pp);
}

void DrawSudoku(Cell* arr) {
	int x = 0;
	for (char& c : grid)
		if (!isspace(c))
		{
			Cell& CurrCell = arr[x];
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
int GetRowNo(int CellNo){
	return (CellNo/9)+1;
}
int GetColNo(int CellNo){
	return (CellNo % 9)+1;
}
void WriteHeader(int CPencil, int CPen, int TCPencil, int TCPen) {
	std::cout << "Changes Made Pencil (Current Iteration): " << CPencil << std::endl;
	std::cout << "Changes Made Pen (Current Iteration):    " << CPen << std::endl;
	std::cout << "Changes Made Pencil (In Total): " << TCPencil << std::endl;
	std::cout << "Changes Made Pen (In Total):    " << TCPen << std::endl << std::endl;
}
void ExtremeDebug(RCB* boxs, RCB* cols, RCB* rows, Cell* gridvals, int& CPencil, int& CPen) {
	int currRN = 1;
	for (int x = 0; x < 81; x++) {
		std::cout << "Cell No: " << x << "	:	";
		Cell& currCell = gridvals[x];
		if (IsCellFilled(currCell, true)) {
			std::cout << "Filled: "<< gridvals[x].Num;
			std::cout << std::endl;
		}
		else {
			for (int y = 0; y < 9; y++) {
				std::cout << y + 1 << ": " << bool(currCell.PossibleNums[y] == true) << "	";
			}
			for (int y = 0; y < 9; y++) {
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
void Solve_clearPN_itrl_r(RCB* rows, Cell* gridvals, int& CPencil, int& CPen) {
	// for every row reduce possibilities
	for (int currRow = 0; currRow < 9; currRow++)
		for (int currCell = 0; currCell < 9; currCell++) {	// for ever cell in row
			int CellLoc = rows[currRow].begin + currCell;	// Current Cell in iteration 

			RCB& CurrentRow = rows[currRow];				// Current Row alias
			Cell& CurrentCell = gridvals[CellLoc];			// Current Cell alias
			bool ISF = IsCellFilled(CurrentCell, true);
			if (ISF)				// if CurrCell is filled
				if (CurrentCell.IsIteratedR == false) {
					CurrentCell.IsIteratedR = true;
					for (int ECR = 0; ECR < 9; ECR++) {			// for every Cell in row (ECR)
						Cell& CurrCellR = gridvals[CurrentRow.begin + ECR];	// Current Cell R is the first cell row + iteration to get the offset orcolumn
						CurrCellR.PossibleNums[CurrentCell.Num - 1] = false;
						CPencil++;
					}
					CurrentRow.PossibleNums[CurrentCell.Num - 1] = false;
				}
		}

}
void Solve_clearPN_itrl_c(RCB* cols, Cell* gridvals, int& CPencil, int& CPen) {
	// for every column reduce possibilities
	for (int currCol = 0; currCol < 9; currCol++)
		for (int currCell = 0; currCell < 9; currCell++) {		// for ever cell in row
			int cellLoc = cols[currCol].begin + currCell * 9;	// Current Cell in iteration (first cell of col + offset) 

			RCB& CurrentCol = cols[currCol];					// Current Row alias
			Cell& CurrentCell = gridvals[cellLoc];				// Current Cell alias

			if (IsCellFilled(CurrentCell, true)) 					// if CurrCell is filled
				if (CurrentCell.IsIteratedC == false) {
					CurrentCell.IsIteratedC = true;
					for (int ECC = 0; ECC < 9; ECC++) {				// for every Cell in col (ECC)
						Cell& CurrCellR = gridvals[CurrentCol.begin + ECC * 9];	// Current Cell R is the first cell row + iteration to get the offset or column
						CurrCellR.PossibleNums[CurrentCell.Num - 1] = false;
						CPencil++;
					}
					CurrentCol.PossibleNums[CurrentCell.Num - 1] = false;
				}
		}
}
void Solve_clearPN_itrl_b(RCB* boxs, Cell* gridvals, int& CPencil, int& CPen) {

	// for every box reduce possibilities
	for (int currBox = 0; currBox < 9; currBox++) {
		RCB& CurrBox = boxs[currBox];
		int z = 0;

		for (int currCell = 0; currCell < 9; currCell++) {
			int CellLoc = CurrBox.begin + currCell % 3 + 9 * z; // 00|01|02|09|

			if (currCell % 3 == 2)
				z++;
			// iteration code above ^

			Cell& CurrentCell = gridvals[CellLoc];
			if (IsCellFilled(CurrentCell, true)) {					// if CurrCell is filled
				if (CurrentCell.IsIteratedB == false) {
					CurrentCell.IsIteratedB = true;
					int m = 0;

					for (int ECB = 0; ECB < 9; ECB++) {				// for every Cell in box (ECB)
						int CellLoc2 = CurrBox.begin + (ECB % 3) + (9 * m); // 00|01|02|09
						Cell& CurrCell2 = gridvals[CellLoc2];	// Current Cell R is the first cell row + iteration to get the offset or column

						CurrCell2.PossibleNums[CurrentCell.Num - 1] = false;
						if (ECB % 3 == 2)
							m++;
						// iteration code above ^

						CPencil++;
					}
					CurrBox.PossibleNums[CurrentCell.Num - 1] = false;
				}
			}
		}
	}
}
void Solve_makeChanges(Cell* gridvals, int& CPencil, int& CPen) {
	// for every cell make changes
	for (int CCNo = 0; CCNo < 81; CCNo++) {
		int NoOfPN = 0;
		int LastPN = 0;
		Cell& CC = gridvals[CCNo];

		for (int CR = 0; CR < 9; CR++) {
			bool& y = CC.PossibleNums[CR];
			if (y == true) {
				NoOfPN++;
				LastPN = CR;
			}
		}
		if (NoOfPN == 1) {
			if (!IsCellFilled(CC, true)) {
				CC.Num = LastPN + 1;
				CC.IsFilled = true;
				++CPen;
			}
		}
	}
}

void Solve(RCB* boxs, RCB* cols, RCB* rows, Cell* gridvals, int& CPencil, int& CPen){
	Solve_clearPN_itrl_r(rows, gridvals, CPencil, CPen);
	Solve_clearPN_itrl_c(cols, gridvals, CPencil, CPen);
	Solve_clearPN_itrl_b(boxs, gridvals, CPencil, CPen);

	Solve_makeChanges(gridvals, CPencil, CPen);
}
void AdvancedSolve(RCB* boxs, RCB* cols, RCB* rows, Cell* gridvals, int& CPencil, int& CPen) {
	ExtremeDebug(boxs, cols, rows, gridvals, CPencil, CPen);
	for (int CR = 0; CR < 9; CR++)
		for (int CRC = 0; CRC < 9; CRC++) 
		{
			RCB& R1 = rows[CR];
			int x = R1.begin + CRC;
			Cell& C1 = gridvals[x];

			if (!C1.IsFilled)
				for (int PV = 0; PV < 9; PV++) 
				{
					bool Fill = false;

					if (C1.PossibleNums[PV])
					{
						for (int CC2 = 0; CC2 < 9; CC2++)
						{
							if (CRC != CC2) {
								int y = R1.begin + CC2;
								Cell& C2 = gridvals[y];
								//Cell& C1 = gridvals[x];
								//RCB& R1 = rows[CR];

								if (!C2.IsFilled)
									if (C2.PossibleNums[PV]) {
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
							C1.SetNum(PV+1, CPencil, CPen);
						}
					}
				}
		}
	for (int CC = 0; CC < 9; CC++)
		for (int CCC = 0; CCC < 9; CCC++) 
		{
			RCB& C1 = cols[CC];
			int x = C1.begin + CCC * 9;
			Cell& CCC1 = gridvals[x];

			if (!CCC1.IsFilled)
				for (int PV = 0; PV < 9; PV++) 
				{
					bool Fill = false;

					if (CCC1.PossibleNums[PV] == true)
					{
						for (int CC2 = 0; CC2 < 9; CC2++)
						{
							if (CCC != CC2) {
								int y = C1.begin + CC2*9;
								Cell& C2 = gridvals[y];
	
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
							CCC1.SetNum(PV+1, CPencil, CPen);
						}
					}
				}
		}	
}
bool BruteSolve_itrl(PuzzleInfo& PZI, int& CPencil, int& CPen, int& IT) {
	// calculate starting cell no and possible value no
	int currIT = IT;
	int currFilledCell;
	int currFilledNum;
	for (auto& z : PZI.gridvals) {
		if (!IsCellFilled(z, true)) {
			for (int y = 0; y < 9; y++) {
				if (z.PossibleNums[y]) {
					if (currIT == 0) {
						z.Num = y + 1;
						z.IsFilledTemp = true;

						currFilledCell = z.Loc;
						currFilledNum = z.Num;
						break;
					}
					else {
						currIT--;
					}
				}
			}
			if (currIT == 0) {
				break;
			}
			else {
				currIT--;
			}
		}
	}


	bool rval = false;
	bool solved = true;

	while (true) {
		Solve(PZI.boxs, PZI.cols, PZI.rows, PZI.gridvals, CPencil, CPen);
		AdvancedSolve(PZI.boxs, PZI.cols, PZI.rows, PZI.gridvals, CPencil, CPen);

		for (auto& z : PZI.gridvals)
			if (IsCellFilled(z, false)) {
				for (int y = 0; y < 9; y++)
					if (z.PossibleNums[y])
						rval = true;
				solved = false;
			}

		if (!rval) {
			break;
		}
	}
	return rval;
}
	// set that no as temp value
void BruteSolve(RCB* boxs, RCB* cols, RCB* rows, Cell* gridvals, int& CPencil, int& CPen) {
	// Copy/create PZI
	PuzzleInfo PZI;

	for (int x = 0; x < 81; x++) {
		PZI.gridvals[x] = gridvals[x];
	}
	for (int x = 0; x < 9; x++) {
		PZI.rows[x] = rows[x];
		PZI.cols[x] = cols[x];
		PZI.boxs[x] = boxs[x];
	}

	int CPEN = 0;
	int CPENCIL = 0;
	int Iteration = 0;

	if (BruteSolve_itrl(PZI, CPENCIL, CPEN, Iteration)) {
		for (int x = 0; x < 81; x++) {
			gridvals[x] = PZI.gridvals[x];
		}
		for (int x = 0; x < 9; x++) {
			rows[x] = PZI.rows[x];
			cols[x] = PZI.cols[x];
			boxs[x] = PZI.boxs[x];
		}
		std::cout << "SOLVEDDDDDDD";
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
			09  #  1  #  #  #  #  #  #  # 17
			18  #  2  #  #  #  #  #  #  # 26
			27  #  #  #  #  #  #  #  1  # 35
			36  #  #  #  #  #  #  #  2  # 44
			45  #  #  #  #  #  #  #  #  # 53
			54  #  #  #  #  #  #  #  #  # 62
			63  #  #  #  #  #  #  #  #  # 71
			72  #  #  #  #  #  #  #  #  # 80
			   72 73 74 75 76 77 78 79 80
		*/

		Puzzles Puzzle;

		grid = "# # # # # # # # #\n";
		grid += "# # # # # # # # #\n";
		grid += "# # # # # # # # #\n";
		grid += "# # # # # # # # #\n";
		grid += "# # # # # # # # #\n";
		grid += "# # # # # # # # #\n";
		grid += "# # # # # # # # #\n";
		grid += "# # # # # # # # #\n";
		grid += "# # # # # # # # #\n";

		defgrid = grid;

		PuzzleInfo PZI;
		int* pz;
		char a;
		std::cout << "Choose Puzzle\n\n"
			<< "Easy	: 1\n"
			<< "Easy	: 2\n"
			<< "Medium	: 3\n"
			<< "Hard	: 4\n"
			<< std::endl << std::endl;

		while (true) {
			std::cin >> a;
			if (a == '1') {
				pz = Puzzle.p1;
				break;
			}
			else if (a == '2') {
				pz = Puzzle.p2;
				break;
			}
			else if (a == '3') {
				pz = Puzzle.p3;
				break;
			}
			else if (a == '4') {
				pz = Puzzle.p4;
				break;
			}
			else {
				std::cout << "Invalid Puzzle no" << std::endl << std::endl;
			}
		}
		NewPage();
		for (int x = 0; x < 81; x++) {
			PZI.gridvals[x].Num = pz[x];
			PZI.gridvals[x].Loc = x;
		}
		int z = 0;

		for (int x = 0; x < 9; x++) {
			PZI.rows[x].begin = x * 9;		// 00|09|18|27...
			PZI.rows[x].end = (x * 9) + 8;// 08|17|26...

			PZI.cols[x].begin = x;			// 00|01|02...
			PZI.cols[x].end = x + 72;		// 72|73|74...

			PZI.boxs[x].begin = x % 3 * 3 + 27 * z;   // 00|03|06|27|30|33|54

			if (x % 3 == 2) {
				z++;
			}
		}

		bool bSolved = false;

		int iteration = 1;
		DrawSudoku(PZI.gridvals);
		std::cout << grid << std::endl;
		std::cout << "Press #f + enter# to open hint sheet" << std::endl;
		std::cout << "Press #a + enter# to solve in normal mode" << std::endl << std::endl;

		bool bBasicMode = false;
		bool bDebugMode = false;
		bool bExtremeDebugMode = false;
		bool bReturnToMenu = false;

		while (true) {
			std::cin >> a;

			if (a == 'f') {
				std::cout << "Press #b + enter# to solve in debug mode" << std::endl;
				std::cout << "Press #d + enter# to solve in extreme debug mode" << std::endl;
				std::cout << "Press #a + enter# to solve in normal mode" << std::endl << std::endl;
				std::cout << "Press #m + enter# to return to menu" << std::endl << std::endl;
			}
			else if (a == 'a') {
				bBasicMode = true;
				bDebugMode = false;
				bExtremeDebugMode = false;
				break;
			}
			else if (a == 'b') {
				bBasicMode = false;
				bDebugMode = true;
				bExtremeDebugMode = false;
				break;
			}
			else if (a == 'd') {
				bBasicMode = false;
				bDebugMode = false;
				bExtremeDebugMode = true;
				break;
			}
			else if (a == 'm') {
				bReturnToMenu = true;
				break;
			}
		}

		if (bReturnToMenu) {
			NewPage();
			continue;
		}

		int TotalChangesMade_Pencil = 0;
		int TotalChangesMade_Pen = 0;

		auto StartTimepoint = std::chrono::high_resolution_clock::now();
		while (!bSolved) {
			int ChangesMade_Pencil = 0;
			int ChangesMade_Pen = 0;
			bool bAdvanced = false;

			Solve(PZI.boxs, PZI.cols, PZI.rows, PZI.gridvals, ChangesMade_Pencil, ChangesMade_Pen);
			if (ChangesMade_Pen == 0 && ChangesMade_Pencil == 0) {
				if (IsSolved(PZI.gridvals, false)) {
					break;
				}

				//else
				bAdvanced = true;
				AdvancedSolve(PZI.boxs, PZI.cols, PZI.rows, PZI.gridvals, ChangesMade_Pencil, ChangesMade_Pen);

				if (ChangesMade_Pencil == 0 && ChangesMade_Pen == 0 && false) {
					BruteSolve(PZI.boxs, PZI.cols, PZI.rows, PZI.gridvals, ChangesMade_Pencil, ChangesMade_Pen);
				}
			}
			if (bDebugMode || bExtremeDebugMode) {
				if (bExtremeDebugMode) {
					ExtremeDebug(PZI.boxs, PZI.cols, PZI.rows, PZI.gridvals, ChangesMade_Pencil, ChangesMade_Pen);
				}
				if (!bAdvanced)
					std::cout << "Iterations: " << iteration << " Type: BASIC" << std::endl;
				else
					std::cout << "Iterations: " << iteration << " Type: ADVANCED" << std::endl;
				DrawSudoku(PZI.gridvals);
				WriteHeader(ChangesMade_Pencil, ChangesMade_Pen, TotalChangesMade_Pencil, TotalChangesMade_Pen);
				std::cout << grid << std::endl << std::endl;
				iteration++;

				if (bExtremeDebugMode) {
					std::cin.get();
				}
			}
			TotalChangesMade_Pencil += ChangesMade_Pencil;
			TotalChangesMade_Pen += ChangesMade_Pen;
		}
		auto EndTimepoint = std::chrono::high_resolution_clock::now();
		std::cout << std::endl << std::endl << std::endl;
		DrawSudoku(PZI.gridvals);
		std::cout << grid;
		std::cout << std::endl << std::endl << std::endl;

		std::cout << "Press #m + enter# to return to menu" << std::endl;
		std::cout << "Press #s + enter# to show stats" << std::endl;

		while (true) {
			std::cin >> a;

			if (a == 'm') {
				NewPage();
				break;
			}
			if (a == 's') {
				std::cout << "Changes Made Pencil (In Total): " << TotalChangesMade_Pencil << std::endl;
				std::cout << "Changes Made Pen (In Total):    " << TotalChangesMade_Pen << std::endl << std::endl;
				long long start = std::chrono::time_point_cast<std::chrono::microseconds>(StartTimepoint).time_since_epoch().count();
				long long end = std::chrono::time_point_cast<std::chrono::microseconds>(EndTimepoint).time_since_epoch().count();
				std::cout << "TimeTaken (In Total):    " << end - start << " microseconds" << std::endl << std::endl;
				std::cout << "Press #m + enter# to return to menu" << std::endl;
			}
		}
	}

}