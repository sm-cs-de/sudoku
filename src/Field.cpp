#include <cstdlib>
#include <cstdio>
#include "Solver.hpp"

using namespace std;

unsigned int bit_pow (const int exp) {
	if (exp < 0) {	abort(); }
	return (1 << exp);
}

int int_div (const int numerator, const int denominator) {
	return (numerator / denominator);
}

Field::Field(const int size, const int i, const int j, const int k):
	date(bit_pow(size)-1),   // Alle relevanten Bits auf 1 (erstes Bit == 1 und nicht 0)
	length(size), possible(size), startdate(false), row(NULL), col(NULL), box(NULL), I(i), J(j), K(k)
	{
}

bool Field::set_data(Field **dates, const int blocklength) {

	int count = 0;
	row = (Field **) malloc((length-1) * sizeof(Field *));
	if (row==NULL) { return false; }
	for (int j=0; j<length; j++) {
		if (j!=J) {
			row[count] = &dates[I][j];
			count++;
		}
	}

	count = 0;
	col = (Field **) malloc((length-1) * sizeof(Field *));
	if (col==NULL) { return false; }
	for (int i=0; i<length; i++) {
		if (i!=I) {
			col[count] = &dates[i][J];
			count++;
		}
	}

	count = 0;
	box = (Field **) malloc((blocklength*blocklength-1) * sizeof(Field *));
	if (box==NULL) { return false; }
	int rowblock = int_div(I, blocklength);   	// Achtung: liefert nur ganzzahligen Teil. alt: (I - (I % blocklength)) / blocklength
	int colblock = int_div(J, blocklength);
	for (int i=(rowblock*blocklength); i<((rowblock+1)*blocklength); i++) {
		for (int j=(colblock*blocklength); j<((colblock+1)*blocklength); j++) {
			if ((i!=I) || (j!=J)) {
				box[count] = &dates[i][j];
				count++;
			}
		}
	}

	return true;
}

void Field::set_date(const unsigned int number) {

	unsigned int num = 1;
	possible = length;
	for (int i=0; i<length; i++) {
		if ((num & number) == 0) {
			possible--;
		}
		num <<= 1;
	}

	date = number;
}

bool Field::check_row() const {

	int count = 1;
	for (int j=0; j<(length-1); j++) {
		if (date == row[j]->get_date()) { count++; }
	}
	if (count == possible) { return true; }

	return false;
}

bool Field::check_col() const {

	int count = 1;
	for (int i=0; i<(length-1); i++) {
		if (date == col[i]->get_date()) { count++; }
	}
	if (count == possible) { return true; }

	return false;
}

bool Field::check_box() const {

	int count = 1;
	for (int k=0; k<(length-1); k++) {
		if (date == box[k]->get_date()) { count++; }
	}
	if (count == possible) { return true; }

	return false;
}

#define START_COLOR "\033[1;31m"
#define CASES_COLOR "\033[0;33m"
#define COLOR_CLEAR "\033[m"
string Field::print_date(const bool cases, const bool color) const {

	string sdate;
	unsigned int num = 1;
	char c[4];

	if (!cases && (possible > 1)) {
		if (length < 10) {
			sdate.append("~");
		} else {
			sdate.append("~~");
		}

	} else {
		if (startdate && color) {
			sdate.append(START_COLOR);
		} else if ((possible > 1) && color) {
			sdate.append(CASES_COLOR);
		}
		for (int i=0; i<length; i++) {
			if ((num & date) != 0) {
				if ((length > 10) && (i+1 < 10)) {
					sdate.append(" ");
				}
				sprintf(c, "%d", i+1);
				sdate.append(c);
				if (possible > 1) {
					sdate.append(",");
				}
			}
			num <<= 1;
		}
		if (color) { sdate.append(COLOR_CLEAR); }
	}

	return sdate;
}
