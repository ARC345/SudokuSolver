#include <iostream>
#include <chrono>
#include <cctype>
#include <ratio>
#include <xtr1common>
#include "Cell.h"
#include "timer/Timer.h"
#include <string>

#define NewPage() for(uchar sksksk=0; sksksk<=100;sksksk++){ std::cout << std::endl;}

using uchar = unsigned char;

enum class EPuzzleDifficulty : uint8_t
{
	VeryEasy,
	Easy,
	Medium,
	Hard,
	VeryHard
};
enum class EDebugLevel : uint8_t
{
	None=0,
	Basic=1,
	Extreme=2

};
struct SPuzzles {
	uchar custom[81];
	uchar p1[81]{
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
	uchar p2[81]{
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
	uchar p3[81]{
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
	uchar p4[81]{
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
	uchar p5[81]{
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
	uchar p6[81]{
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
private:
	static SPuzzles* Puzzles_;
	SPuzzles() = default;;
public:
	SPuzzles(SPuzzles& other) = delete;
	void operator=(const SPuzzles&) = delete;

	static SPuzzles* Get()
	{
		if (Puzzles_ == nullptr) {
			Puzzles_ = new SPuzzles;
		}
		return Puzzles_;
	}
};

class CPuzzleState;
class CGridPrinter {
	std::string grid = "# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #\n# # # # # # # # #";
public:
	void PrintCM(const CPuzzleState& PZI) const;

	void PrintTCM(const CPuzzleState& PZI) const;

	void PrintHeader(const CPuzzleState& PZI) const;

	void PrepareGridForPrinting(CCell* gv) {
		uint8_t i = 0;
		for (auto& grid_val : grid)
			if (grid_val != ' ' && grid_val != '\n')
				grid_val = gv[i++].GetNum() + '0';
	}

  void Print() const {
		std::cout << grid;
	}

private:
	static CGridPrinter* GridPrinter_;
	CGridPrinter() = default;;
public:
	CGridPrinter(CGridPrinter& other) = delete;
	void operator=(const CGridPrinter&) = delete;

	static CGridPrinter* Get()
	{
		if (!GridPrinter_) GridPrinter_ = new CGridPrinter;
		return GridPrinter_;
	}
};

CGridPrinter* CGridPrinter::GridPrinter_ = nullptr;;
SPuzzles* SPuzzles::Puzzles_ = nullptr;
CTimer* CTimer::Timer_ = nullptr;

// row column block
struct RCB {
	virtual void OnClear(CCell& _Cell) = 0;
	virtual bool IsClear(CCell& _Cell) = 0;

	uchar begin;
	uchar end;

	virtual uchar GetOffset(const uchar OffsetAmt) const = 0;
	virtual CCell& GetOffset(const uchar OffsetAmt, CCell* gv) const = 0;
};

struct Row : RCB {
	
	virtual void OnClear(CCell& _Cell) override { _Cell.bClearedRow = 1u; }
	virtual bool IsClear(CCell& _Cell) override { return _Cell.bClearedRow; };
	virtual uchar GetOffset(const uchar OffsetAmt) const override {
		return begin + OffsetAmt;
	};
	virtual CCell& GetOffset(const uchar OffsetAmt, CCell* gv) const override {
		return gv[GetOffset(OffsetAmt)];
	};
};
struct Col : RCB
{
	virtual void OnClear(CCell& _Cell) override { _Cell.bClearedCol = 1u; }
	virtual bool IsClear(CCell& _Cell) override { return _Cell.bClearedCol; };

	virtual uchar GetOffset(const uchar OffsetAmt) const override {
		return begin + (OffsetAmt * 9);
	};
	virtual CCell& GetOffset(const uchar OffsetAmt, CCell* gv) const override {
		return gv[GetOffset(OffsetAmt)];
	};
};
struct Box : RCB
{
	virtual void OnClear(CCell& _Cell) override { _Cell.bClearedBox = 1u; }
	virtual bool IsClear(CCell& _Cell) override { return _Cell.bClearedBox; };
	
	virtual uchar GetOffset(const uchar OffsetAmt)const override {
		uchar z=0;
		if (OffsetAmt <= 2)
			z = 0;
		else if (OffsetAmt < 6)
			z = 1;
		else
			z = 2;
		return begin + OffsetAmt % 3 + 9 * z; // 00|01|02|09|
	};
	virtual CCell& GetOffset(const uchar OffsetAmt, CCell* gv) const override {
		return gv[GetOffset(OffsetAmt)];
	};
};

class CPuzzleState {
public:
	Row rows[9];
	Col cols[9];
	Box boxs[9];
	CCell gridvals[81];

	uint8_t Pencil;
	uint8_t Pen;

	uint8_t TotalChangesMade_Pencil;
	uint8_t TotalChangesMade_Pen;

	CPuzzleState() : Pencil(0), Pen(0), TotalChangesMade_Pencil(0), TotalChangesMade_Pen(0){};
	CPuzzleState(const CPuzzleState& PZI) {
		Pencil = PZI.Pencil;
		Pen = PZI.Pen;

		TotalChangesMade_Pen = PZI.TotalChangesMade_Pen;
		TotalChangesMade_Pencil = PZI.TotalChangesMade_Pencil;

		for (uchar i = 0; i < 9; i++) {
			rows[i] = (PZI.rows[i]);
			cols[i] = (PZI.cols[i]);
			boxs[i] = (PZI.boxs[i]);
		}
		for (uchar i = 0; i < 81; i++) {
			gridvals[i] = CCell(PZI.gridvals[i]);
		}
	}

	void UpdateTCM() //TotalChangesMadde
	{
		TotalChangesMade_Pencil	+=Pencil;
		TotalChangesMade_Pen	+=Pen;
	}

	bool IsInvalid() {

		Solve_clearPN_itrl();
		bool binvalid = false;

		for (CCell& cl : gridvals)
		{
			if(cl.bSet) continue;
			binvalid=true;
			
			for (uint8_t i = 0; i < 9; i++)
			{
				if (cl.Data & (0x01 << i))
				{
					binvalid = false;
					break;
				}
			}
			if (binvalid) { 
				break;
			}
		}
		return binvalid;
	}
	bool IsSolved() {
		for (auto& gv : gridvals) {
			if (gv.GetNum()==0) {
				return false;
			}
		}
		return true;
	}
	
	void Solve_clearPN_itrl(RCB* _RCB);
	void Solve_clearPN_itrl();
	void Solve();

	void AdvancedSolve_itrl(RCB* _RCB);
	void AdvancedSolve();

	inline void SetupGridvals(uchar* pz)
	{
		for (uchar x = 0; x < 81; x++) { gridvals[x].SetNum(pz[x]); }
	}
	inline void SetupRCB()
	{
		uchar z = 0;

		for (uchar x = 0; x < 9; x++) {
			rows[x].begin = x * 9;		// 00|09|18|27...
			rows[x].end	  = (x * 9) + 8;// 08|17|26...

			cols[x].begin = x;			// 00|01|02...
			cols[x].end	  = x + 72;		// 72|73|74...

			boxs[x].begin = x % 3 * 3 + 27 * z;   // 00|03|06|27|30|33|54

			if (x % 3 == 2) {
				z++;
			}
		}
	}
};
uchar GetRowNo(uchar CellNo) {
	return (CellNo / 9) + 1;
}
uchar GetColNo(uchar CellNo) {
	return (CellNo % 9) + 1;
}

void CPuzzleState::Solve_clearPN_itrl(RCB* _RCB) {
	// for every row reduce possibilities
	for (uchar x = 0; x < 9; x++)
	{
		for (uchar y = 0; y < 9; y++)
		{	
			RCB& CR = _RCB[x];				
			CCell& CC = CR.GetOffset(y, gridvals);			// Current CCell alias
			if (CC.bSet && !_RCB->IsClear(CC))
			{	// if CurrCell is filled
				for (uchar z = 0; z < 9; z++)
				{			// for every CCell in row (ECR)
					CCell& CurrCellR = CR.GetOffset(z, gridvals);	// Current CCell R is the first CCell row + iteration to get the offset or column
					if (!CurrCellR.bSet)
					{
						CurrCellR.Data &= ~(1u << CC.GetNum() - 1);
						Pencil++;
					}
				}
				_RCB->OnClear(CC);
			}
		}
	}
}
void CPuzzleState::Solve_clearPN_itrl() {
	Solve_clearPN_itrl(rows);
	Solve_clearPN_itrl(cols);
	Solve_clearPN_itrl(boxs);
}
void CPuzzleState::Solve() {
	Solve_clearPN_itrl();
	// for every CCell make changes
	for (CCell & CC : gridvals) {
		if (CC.bSet) continue;

		uint8_t LastPN;

		if (CC.GetPossibleNumCount(LastPN) == 1) {
			CC.SetNum(LastPN + 1);
			++Pen;
		}
	}
}

void CPuzzleState::AdvancedSolve_itrl(RCB* _RCB) {
	Solve_clearPN_itrl();

	// iteration through RCBs
	for (uint8_t i = 0; i < 9; i++)
	{
		RCB& rcb = _RCB[i];

		// iteration through Cells in RCB
		for (uint8_t ii = 0; ii < 9; ii++)
		{
			CCell& cl = rcb.GetOffset(ii, gridvals);
			if (cl.bSet) continue;
			
			// iteration through PV in Cells in RCB
			for (uint8_t iii = 0; iii < 9; iii++)
			{
				if (cl.Data & (0x01 << iii))
				{
					bool bFill = false;

					// iteration through other Cells in RCB
					for (uint8_t iiii = 0; iiii < 9; iiii++) {
						if (ii != iiii)
						{
							CCell& cl2 = rcb.GetOffset(iiii, gridvals);
							if (cl2.bSet) continue;

							if (cl2.Data & (0x01 << iii)) {
								bFill = false;
								break;
							}
							else {
								bFill = true;
								continue;
							}
						}
					}

					if (bFill)
					{
						cl.SetNum(iii+1);
						++Pen;
					}
				}
			}
		}
	}
}
void CPuzzleState::AdvancedSolve() {

	AdvancedSolve_itrl(rows);
	AdvancedSolve_itrl(cols);
	AdvancedSolve_itrl(boxs);
}

uint8_t InceptionNo = 0;

bool BruteSolve(CPuzzleState& PZI, bool bDebug)
{
	if (PZI.IsSolved())	return true; 
	PZI.Solve_clearPN_itrl();

	InceptionNo++;

	uint8_t SelectedCellNo;
	uint8_t MinPnNo = 10; // impossible

	// Find CCell with least possible values
	for (uint8_t i = 0; i < 81; i++)
	{
		CCell& CC = PZI.gridvals[i];
		if(CC.bSet) continue;

		uint8_t PnNo = 0;

		for (uint8_t ii = 0; ii < 9; ii++)
		{
			if (CC.Data & (0x01 << ii))
			{
				PnNo++;
			}
		}

		if(PnNo == 2)
		{
			MinPnNo = 2;
			SelectedCellNo = i;
			break;
		}
		if (PnNo < MinPnNo) {
			MinPnNo = PnNo;
			SelectedCellNo = i;
		}
	}
	/*Copy Current State into PZI 2 for temp Modifications PZI2 being a temporary state*/
	CPuzzleState PZI2 = CPuzzleState(PZI);
	CCell* SelectedCell = &PZI2.gridvals[SelectedCellNo];

	if(bDebug)
	{
		std::cout << "------------------------------------------------------------------\n";
		std::cout << "InceptionNo: "<< InceptionNo << "\n"
				  << "Selected Cell: " << +SelectedCellNo	
				  << "\n[" << +MinPnNo << "]: ";
		SelectedCell->Print();
		std::cout << "------------------------------------------------------------------\n";
		std::cout << std::endl;

		CGridPrinter::Get()->PrepareGridForPrinting(PZI2.gridvals);
		CGridPrinter::Get()->Print();

		std::cout << std::endl;
		std::cout << std::endl;
	}

	// For Every Possible No
	for (uchar x = 0; x < MinPnNo; x++)
	{
		PZI2 = CPuzzleState(PZI);
		SelectedCell = &PZI2.gridvals[SelectedCellNo];

		uchar it = x;

		for (uint8_t i = 0; i < 9; i++)
		{
			if (SelectedCell->Data & (0x01 << i))
			{
				if (it != 0)
				{
					it--;
					continue;
				}
				SelectedCell->SetNum(i + 1);

				break;
			}
		}

		if (bDebug)
		{
			std::cout << std::endl;
			CGridPrinter::Get()->PrepareGridForPrinting(PZI2.gridvals);
			CGridPrinter::Get()->Print();
		}
		while (true)
		{
			if (PZI2.IsSolved())
			{
				PZI = CPuzzleState(PZI2);
				return true;
			}
			if (PZI2.IsInvalid())
			{
				PZI2 = CPuzzleState(PZI);
				SelectedCell = &PZI2.gridvals[SelectedCellNo];
				if (bDebug)
				{
					CGridPrinter::Get()->PrepareGridForPrinting(PZI2.gridvals);
					CGridPrinter::Get()->Print();

					std::cout << "INVALID: Reset" << std::endl;

					std::cout << "-------------------------xxxxxxxxxx------------------------------\n";
					std::cout << "InceptionNo: " << InceptionNo << "\n"
						<< "Selected CCell: " << +SelectedCellNo
						<< "\n[" << +MinPnNo << "]: ";
					SelectedCell->Print();
					std::cout << "------------------------------------------------------------------\n";
					std::cout << std::endl;

				}

				break;
			}

			PZI2.Pen = 0;
			PZI2.Pencil = 0;
			PZI2.Solve();
			PZI2.AdvancedSolve();
			if(bDebug)
				std::cout << "\n" << PZI2.Pen << ":" << PZI2.Pencil << "\n";

			if (PZI2.IsSolved())
			{
				PZI = CPuzzleState(PZI2);
				return true;
			}
			if (PZI2.IsInvalid())
			{
				PZI2 = CPuzzleState(PZI);
				SelectedCell = &PZI2.gridvals[SelectedCellNo];
				if (bDebug)
				{
					CGridPrinter::Get()->PrepareGridForPrinting(PZI2.gridvals);
					CGridPrinter::Get()->Print();

					std::cout << "INVALID: Reset" << std::endl;

					std::cout << "-------------------------xxxxxxxxxx------------------------------\n";
					std::cout << "InceptionNo: " << InceptionNo << "\n"
						<< "Selected CCell: " << +SelectedCellNo
						<< "\n[" << +MinPnNo << "]: ";
					SelectedCell->Print();
					std::cout << "------------------------------------------------------------------\n";
					std::cout << std::endl;

				}

				break;
			}

			if (PZI2.Pen == 0 && PZI2.Pencil == 0)
			{
				if((SelectedCell->bSet) && BruteSolve(PZI2, bDebug))
				{
					PZI = CPuzzleState(PZI2);
					return true;
				}
				else
				{
					break; // (While(true)).
				}
			}
		}
	}

	if (PZI.IsSolved()) return true;

	--InceptionNo;
	return false;
}

void MakePuzzle()
{
	std::cout << "press (#r + enter) to reset" << std::endl;
	uchar i = 0;
	while (true)
	{
		char in;
		std::cin >> in;
		if (in == '#')
		{
			in = '0';
		}
		uchar Num = in-'0';
		if (isdigit(in) && Num >= 0 && Num<=9)
		{
			SPuzzles::Get()->custom[i] = in - '0';
			i++;
		}
		if(i==81)
			break;

		if (in =='r')
		{
			std::cout <<"Reset" << "\n";
			i=0;
		}
	}
}

void ChoosePuzzle(char& a, uchar*& pz) {
	bool bbreak = false;

	std::cout << "Choose Puzzle\n\n"
		<< "Custom	: x\n"
		<< "Easy	: 1\n"
		<< "Easy	: 2\n"
		<< "Medium	: 3\n"
		<< "Hard	: 4\n"
		<< "Very Hard	: 5\n"
		<< "Very Hard	: 6\n"
		<< std::endl << std::endl;
	while (!bbreak) {
		std::cin >> a;
		switch (a)
		{
		case 'x':
			std::cout << "Enter puzzle in text format" << std::endl;
			pz = SPuzzles::Get()->custom;
			MakePuzzle();
			bbreak = true;
			break;
		case '1':
			pz = SPuzzles::Get()->p1;
			bbreak = true;
			break;
		case '2':
			pz = SPuzzles::Get()->p2;
			bbreak = true;
			break;
		case '3':
			pz = SPuzzles::Get()->p3;
			bbreak = true;
			break;
		case '4':
			pz = SPuzzles::Get()->p4;
			bbreak = true;
			break;
		case '5':
			pz = SPuzzles::Get()->p5;
			bbreak = true;
			break;
		case '6':
			pz = SPuzzles::Get()->p6;
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
	while (1) {
		CPuzzleState PZI;
		uchar* pz = SPuzzles::Get()->p1;

		// input stuff
		char a;
		ChoosePuzzle(a, pz);
		PZI.SetupGridvals(pz);
		PZI.SetupRCB();
		NewPage();
		uint16_t iteration = 1;

		CGridPrinter::Get()->PrepareGridForPrinting(PZI.gridvals);
		CGridPrinter::Get()->Print();
		std::cout << std::endl;
		std::cout << "Press #f + enter# to open hint sheet" << std::endl;
		std::cout << "Press #a + enter# to solve in normal mode" << std::endl << std::endl;

		EDebugLevel DebugLevel;
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
				DebugLevel = EDebugLevel::None;
				bbreak = true;
				break;
			}
			case 'b': {
				DebugLevel = EDebugLevel::Basic;
				bbreak = true;
				break;
			}
			case 'd': {
				DebugLevel = EDebugLevel::Extreme;
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
		
    CTimer::Get()->Begin();
		CTimer::Get()->Pause();

    while (1) {
			PZI.Pencil = 0;
			PZI.Pen = 0;
			bool bAdvanced = false;
			bool bBruteSolve = false;

			CTimer::Get()->Resume();
			PZI.Solve();
			CTimer::Get()->Pause();

			if (PZI.Pen == 0 && PZI.Pencil == 0) {
				if (PZI.IsSolved()) {
					break;
				}

				CTimer::Get()->Resume();
				PZI.AdvancedSolve();
				CTimer::Get()->Pause();

				bAdvanced = true;
				if (PZI.Pen == 0 && PZI.Pencil == 0) {
					if (PZI.IsSolved()) {
						break;
					}
					bBruteSolve = true;

					CTimer::Get()->Resume();
					BruteSolve(PZI, (int)DebugLevel > 0);
					CTimer::Get()->Pause();
					break;
				}
			}
			if ((int)DebugLevel > 0) {
				if (bBruteSolve)
					std::cout << "Iterations: " << iteration << " Type: BRUTE" << std::endl;
				else if (bAdvanced)
					std::cout << "Iterations: " << iteration << " Type: ADVANCED" << std::endl;
				else
					std::cout << "Iterations: " << iteration << " Type: BASIC" << std::endl;

				CGridPrinter::Get()->PrintHeader(PZI);
				CGridPrinter::Get()->PrepareGridForPrinting(PZI.gridvals);
				CGridPrinter::Get()->Print();
				std::cout << std::endl << std::endl;
				iteration++;

				if (DebugLevel == EDebugLevel::Extreme) {
					std::cin.get();
				}
			}
			PZI.UpdateTCM();
		}

		std::cout << std::endl << std::endl << std::endl;
		CGridPrinter::Get()->PrepareGridForPrinting(PZI.gridvals);
		CGridPrinter::Get()->Print();
		std::cout << std::endl << std::endl << std::endl;

		std::cout << "Press #m + enter# to return to menu" << std::endl;
		std::cout << "Press #s + enter# to show stats" << std::endl;

		bbreak = false;
		while (!bbreak) {
			std::cin >> a;
			switch (a)
			{
			case 'm': {
				CTimer::Get()->Clear();
				NewPage();
				bbreak = true;
				break;
			}
			case 's': { 
				CGridPrinter::Get()->PrintCM(PZI);
				const auto ElapsedTime_microseconds = (long long)CTimer::Get()->GetTimeElapsed();
				std::cout << "TimeTaken (In Total): " << ElapsedTime_microseconds	<< " microseconds"	<< std::endl << std::endl;
				std::cout << "TimeTaken (In Total): " << ElapsedTime_microseconds/1000000.f << " seconds" << std::endl << std::endl;
				//std::cout << "TimeTaken (In Total): " << Timer::Get()->GetTimeElapsed<std::chrono::microseconds>() << " microseconds" << std::endl << std::endl;
				//std::cout << "TimeTaken (In Total): " << Timer::Get()->GetTimeElapsed<std::chrono::microseconds>() << " microseconds" << std::endl << std::endl;
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

void CGridPrinter::PrintCM(const CPuzzleState& PZI) const
{
	std::cout << "Changes Made Pencil (Current Iteration): " << PZI.Pencil << std::endl;
	std::cout << "Changes Made Pen (Current Iteration):    " << PZI.Pen << std::endl;
}
void CGridPrinter::PrintTCM(const CPuzzleState& PZI) const
{
	std::cout << "Changes Made Pencil (In Total): " << PZI.TotalChangesMade_Pencil << std::endl;
	std::cout << "Changes Made Pen (In Total):    " << PZI.TotalChangesMade_Pen << std::endl << std::endl;
}
void CGridPrinter::PrintHeader(const CPuzzleState& PZI) const
{
	PrintCM(PZI);
	PrintTCM(PZI);
}