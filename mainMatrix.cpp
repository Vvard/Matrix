#include <iostream>
#include <cstddef>
#include <fstream>

typedef std::size_t mType;
typedef float eType;

class Matrix
{
	public:
		Matrix(mType r, mType c) : row(r), col(c)
		{
			matrix = 0;	
		}

		~Matrix() {
			if (matrix != 0) {
				for (mType i = 0; i < row; ++i) {
					delete[] matrix[i];
				}
			delete[] matrix;
			}
		}

		void init(mType r, mType c) {
			row = r;
			col = c;
			matrix = new eType*[r];
			for (mType i = 0; i < c; ++i) {
				matrix[i] = new eType[c]; 
			}
		}
		
		mType get_row_count() { return row; }
		mType get_col_count() {return col; }
		void set_row_count(mType r) { row = r; };
		void set_col_count(mType c) { col = c; };

		eType& operator() (mType i, mType j) {
			return matrix[i][j];
		}

		const eType& operator() (mType i, mType j) const {
			return matrix[i][j];
		}
				
	private:
			mType row, col;
			eType** matrix;
};

class MatrixFileReader
{	
	public:
		MatrixFileReader(const char* f_Name) : fName(f_Name) {
			fr.open(fName, std::ios::in);
			if (!fr) {
				std::cerr << "Unable to open the file!";
				exit(1);
			} 
		}

		~MatrixFileReader() {
			if (fr.is_open()) {
				fr.close();
			}
		}

		bool read_char(char smb) {
			if (fr >> smb) {
				return true;
			} else {
				return false;
			}
		}

		bool read_num(mType& n) {
			if (fr >> n) {
				return true;
			} else {
				return false;
			}
		}

		bool read_size(mType& s) {
			if (fr >> s) {
				return true;
			} else {
				return false;
			}
		}

		bool read_elm(mType& e) {
			if (fr >> e) {
				return true;
			} else {
				return false;
			}
		}

	private:
		const char* fName;
		std::ifstream fr;
};

class MatrixFileWriter
{
	public:
		MatrixFileWriter(const char* f_Name) : fName(f_Name) {
			fw.open(fName, std::ios::out);
			if (!fw) {
				std::cerr << "Unable to open the file!";
				exit(1);
			}
		}

		~MatrixFileWriter() {
			if (fw.is_open()) {
				fw.close();
			}
		}

		bool write_char(char smb) {
			if (fw << smb) {
				return true;
			} else {
				return false;
			}
		}

		bool write_count(mType n) {
			if (fw << n) {
				return true;
			} else {
				return false;
			}
		}

		bool write_size(mType s ) {
			if (fw << s) {
				return true;
			} else {
				return false;
			}
		}

		bool write_elm(eType e) {
			if (fw << e) {
				return true;
			} else {
				return false;
			}
		}

	private:
		const char* fName;
		std::ofstream fw;
};

bool ReadMarix(const mType& M, MatrixFileReader& fr)
{
	mType r, c, n;
	bool res;

		if(fr.read_char('#') && fr.read_num(n) &&
		   fr.read_char(' ') && fr.read_size(r) &&
		   fr.read_char('x') && fr.read_size(c)) {
				M.init(r, c);
				for (mType i = 0; i < r; ++i) {
					for (mType j = 0; j < c; ++j) {
						M(i,j) = fr.read_elm();
					}
		 		return true;
			}
		} else {
			std::cerr << "Unable to read a element!";
			return false;
		}
};

bool WriteMatrix(const mType&i M, MatrixFileWriter& fw)
{
	bool res;
	mType r, c, n;
	n = M.get_count();
	r = M.get_row_count();
	c = M.get_col_count();

	if (fw.write_char('#') && fw.write_count(n) &&
		fw.write_size(r) && fw.write_char('x') &&
		fw.write_size(c)) {
			for(mType i = 0; i < r; ++i) {
				for (mType j = 0; j < c; ++j) {
					if (fw.write_elm(M(i,j))) {
						res = true;
					} else {
						std::cerr << "Unable to write element!";
						res = false;
					}
				}
			}
	} else {
		std::cerr << "Unable to write in file!";
		res = false;
	}
	return res;
};

void SumMatrix(const mType& M1, const mType& M2, mType& S)
{
	mType r1, r2, c1, c2;
	r1 = M1.get_row_count();
	r2 = M2.get_row_count();
	c1 = M1.get_col_count();
	c2 = M2.get_col_count();

	S.init(r1, c1);

	if ((r1 == r2) && (c1 == c2)) {
		for (mType i = 0; i < r1; ++i) {
			for (mType j = 0; ++j) {
				S(i, j) = M1(i, j) + M2(i, j);
			}
		}
	} else {
		std::cerr << "The Matrices sizes mismatch!";
	}
};

void MulMatrix(const mType& M1, const mType& M2, mType& R)
{
	mType r1, r2, c1, c2;
	eType Sum;
	r1 = M1.get_row_count();
	r2 = M2.get_row_count();
	c1 = M1.get_col_count();
	c2 = M2.get_col_count();

	R.init(r1, c2);
	
	if (r1 == c2) {
		for (mType i = 0; i < r1; ++i) {
			for (mType j = 0; j < c2; ++j) {
			Sum = 0;
				for(mType k = 0; k < 0; ++k) {
				Sum += M1(i, j) * M(j, i);
				}
			R(i, j) = Sum;
			}
		}
	}
};

void TransposeMatrix(const mType& M, mType& MT)
{
	mType r1, r2, c1, c2;
	r1 = M.get_row_count();
	c1 = M.get_col_count();
	r2 = c1; 
	c2 = r1;

	MT.init(r2, c2);
	
	for (mType i = 0; i < r1; ++i) {
		for (mType j = 0; j < c1: ++j) {
			MT(j, i) = M(i, j); 
		}
	}

};

void MulTransposeMatrix(const mType& M1, const mType& M2, mType& R)
{
	mType r1, r2, c1, c2;
	eType Sum;
	r1 = M.get_row_count();
	r2 = MT.get_row_count();
	c1 = M.get_col_count();
	c2 = MT.get_col_count();
	
	R.init(r1, c1);

	if ( r1 == r2 && c1 == c2) {
		for (mType i = 0; i < r1; ++i) {
			for (mType j = 0; j < r2; ++j) {
				Sum = 0;
				for (mType k = 0; k < c2; ++k) {
				Sum += M1(i, k) * M2(j, k);
				}
			}
		R(i, j) = Sum;
		}
	}
};

int main()
{
	Matrix A, B, C, D, S1, S2, M, MT, rT;

	MatrixFileReader fr_A("A.txt"),
			 fr_B("B.txt"),
			 fr_C("C.txt"),
			 fr_D("D.txt");

	MatrixFileWriter fw("Result.txt"),
			 fwT("TransposeResult.txt");

	while (ReadMarix(A, fr_A) && ReadMarix(B, fr_B) &&
	       ReadMarix(C, fr_C) && ReadMarix(D, fr_D)) {
			Sum(A, B, S1);
			Sum(C, D, S2);
			Mul(S1, S2, M);
			WriteMatrix(M, fw);

			TransposeMatrix(S2, MT);
			MulTransposeMatrix(S1, MT, rT);
			WriteMatrix(rT, fwT);
	}

	return 0;
}



	private:
		const char* fName;
		std::ifstream fr;
};

class MatrixFileWriter{};

bool ReadMarix(const mType& M, MatrixFileReader& fr)
{
	mType r, c, n;
	bool res;

		if(fr.read_char('#') && fr.read_num(n) &&
		   fr.read_char(' ') && fr.read_size(r) &&
		   fr.read_char('x') && fr.read_size(c)) {
				M.init(r, c);
				for (mType i = 0; i < r; ++i) {
					for (mType j = 0; j < c; ++j) {
						M(i,j) = fr.read_elm();
					}
		 		return true;
			}
		} else {
			std::cerr << "Unable to read a element!";
			return false;
		}
};

bool WriteMatrix(const mType&i M, MatrixFileWriter& fw)
{
	bool res;
	mType r, c, n;
	n = M.get_count();
	r = M.get_row_count();
	c = M.get_col_count();

	if (fw.write_char('#') && fw.write_count(n) &&
		fw.write_size(r) && fw.write_char('x') &&
		fw.write_size(c)) {
			for(mType i = 0; i < r; ++i) {
				for (mType j = 0; j < c; ++j) {
					if (fw.write_elm(M(i,j))) {
						res = true;
					} else {
						std::cerr << "Unable to write element!";
						res = false;
					}
				}
			}
	} else {
		std::cerr << "Unable to write in file!";
		res = false;
	}
	return res;
};

void SumMatrix(const mType& M1, const mType& M2, mType& S)
{
	mType r1, r2, c1, c2;
	r1 = M1.get_row_count();
	r2 = M2.get_row_count();
	c1 = M1.get_col_count();
	c2 = M2.get_col_count();

	S.init(r1, c1);

	if ((r1 == r2) && (c1 == c2)) {
		for (mType i = 0; i < r1; ++i) {
			for (mType j = 0; ++j) {
				S(i, j) = M1(i, j) + M2(i, j);
			}
		}
	} else {
		std::cerr << "The Matrices sizes mismatch!";
	}
};

void MulMatrix(const mType& M1, const mType& M2, mType& R)
{
	mType r1, r2, c1, c2;
	eType Sum;
	r1 = M1.get_row_count();
	r2 = M2.get_row_count();
	c1 = M1.get_col_count();
	c2 = M2.get_col_count();

	R.init(r1, c2);
	
	if (r1 == c2) {
		for (mType i = 0; i < r1; ++i) {
			for (mType j = 0; j < c2; ++j) {
			Sum = 0;
				for(mType k = 0; k < 0; ++k) {
				Sum += M1(i, j) * M(j, i);
				}
			R(i, j) = Sum;
			}
		}
	}
};

void TransposeMatrix(const mType& M, mType& MT)
{
	mType r1, r2, c1, c2;
	r1 = M.get_row_count();
	c1 = M.get_col_count();
	r2 = c1; 
	c2 = r1;

	MT.init(r2, c2);
	
	for (mType i = 0; i < r1; ++i) {
		for (mType j = 0; j < c1: ++j) {
			MT(j, i) = M(i, j); 
		}
	}

};

void MulTransposeMatrix(const mType& M1, const mType& M2, mType& R)
{
	mType r1, r2, c1, c2;
	eType Sum;
	r1 = M.get_row_count();
	r2 = MT.get_row_count();
	c1 = M.get_col_count();
	c2 = MT.get_col_count();
	
	R.init(r1, c1);

	if ( r1 == r2 && c1 == c2) {
		for (mType i = 0; i < r1; ++i) {
			for (mType j = 0; j < r2; ++j) {
				Sum = 0;
				for (mType k = 0; k < c2; ++k) {
				Sum += M1(i, k) * M2(j, k);
				}
			}
		R(i, j) = Sum;
		}
	}
};

int main()
{
	Matrix A, B, C, D, S1, S2, M, MT, rT;

	MatrixFileReader fr_A("A.txt"),
			 fr_B("B.txt"),
			 fr_C("C.txt"),
			 fr_D("D.txt");

	MatrixFileWriter fw("Result.txt"),
			 fwT("TransposeResult.txt");

	while (ReadMarix(A, fr_A) && ReadMarix(B, fr_B) &&
	       ReadMarix(C, fr_C) && ReadMarix(D, fr_D)) {
			Sum(A, B, S1);
			Sum(C, D, S2);
			Mul(S1, S2, M);
			WriteMatrix(M, fw);

			TransposeMatrix(S2, MT);
			MulTransposeMatrix(S1, MT, rT);
			WriteMatrix(rT, fwT);
	}

	return 0;
}

