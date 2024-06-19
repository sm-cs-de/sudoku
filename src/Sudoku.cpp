#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <cctype>
#include "Solver.hpp"

using namespace std;

Sudoku::Sudoku(): dates(NULL), length(0), blocklength(0), startdates(0), box_counts(true) {}

Sudoku::~Sudoku() {
	for(int i=0; i<length; i++) {
		for(int j=0; i<length; i++) {
			delete &dates[i][j];
		}
	}
	if(dates != NULL) {
		free(dates);
	}
}

bool Sudoku::read(const string argv) {

	ifstream ifs;

	ifs.open(argv.c_str(), ios::in);
	if(!ifs.is_open()) {
		cout << "#Fehler:\tDie Datei \"" << argv << "\" konnte nicht geöffnet werden!" << endl;
		return false;
	}

	int count = 0;
	char inc, prec = ' ';
	while(true) {
		inc = ifs.get();								// TODO: Das ist immer so umständlich mit get(), weil es nur zeichenweise einliest
		if(ifs.eof()) {
			break;
		} else if(ifs.fail()) {
			cout << "#Fehler:\tI/O-Fehler!"<< endl;
			return false;
		} else if(!isdigit(inc)) {
			if(!isspace(inc) && !ifs.eof()) {
				cout << "#Fehler:\tUngültiges Zeichen \"" << inc << "\" gefunden!"<< endl;
				return false;
			}
		} else if(!isdigit(prec)) {
			count++;
		}
		prec = inc;
	}

	double sqr = sqrt(count);
	if(floor(sqr) != sqr) {
		cout << "#Fehler:\tDie Werteanzahl muss quadratisch sein! (aktuelle Länge: " << sqr << "-" << count <<")" << endl;
		return false;
	}
	count = (int) sqr;

	if(count > 32) {	// 32-Bit System :>
		cout << "#Fehler:\tDie Seitenlänge darf nicht größer als 32 sein! (aktuelle Länge: " << count << ")" << endl;
		return false;

	} else if(count == 1) {
		cout << "Tolles Sudoku ..   Das kannste selber!\n" << endl;
		return false;
	}

	if(!this->set_data(count)) {
		cout << "#Fehler:\tDer Speicher konnte nicht reserviert werden!" << endl;
		return false;
	}

	ifs.clear();
	ifs.seekg(ios::beg);

	int num;
	for(int i=0; i<count; i++) {
		for(int j=0; j<count; j++) {
			ifs >> num;
			if(!this->fill(num, i, j)) { return false; }
		}
	}
	ifs.close();

	cout << "~ Eingegebenes Sudoku:" << endl;
	this->print_s(false);
	cout << "~ Sonstige Daten:" << endl;
	cout << "\tSeitenlänge:\t"  << length << endl;
	cout << "\tBlocklänge:\t"   << blocklength << endl;
	cout << "\tStartwerte:\t"   << startdates << endl;

	if(!this->check_in()) {
		this->print_s(false);
		return false;
	}

	return true;
}

bool Sudoku::set_data(const int size) {

	length = size;

	double n = sqrt(size);
	if(floor(n) != n) {     // n ist keine ganze Zahl
		blocklength = size;
		box_counts = false;
	} else {
		blocklength = n;
	}

	dates = (Field **) malloc(size * sizeof(Field *));
	if(dates==NULL) { return false; }

	int k = 0;
	for(int i=0; i<size; i++) {
		dates[i] = (Field *) malloc(size * sizeof(Field));
		if(dates[i]==NULL) { return false; }

		for(int j=0; j<size; j++) {
			if(box_counts) {
				k = blocklength*int_div(i, blocklength) + int_div(j, blocklength); // TODO: überprüfen!
			}
			dates[i][j] = *(new Field(size, i, j, k));
			if(&dates[i][j]==NULL) { return false; }
		}
	}

	for(int i=0; i<size; i++) {
		for(int j=0; j<size; j++) {
			if(!dates[i][j].set_data(dates,blocklength)) { return false; }
		}
	}

	return true;
}

bool Sudoku::fill(const int date, const int i, const int j) {

	if((i<0) || (i>=length) || (j<0) || (j>=length)) {
		cout << "#Fehler:\tIndex falsch! (" << i << "," << j << ")" << endl;
		return false;
	}

	if((date<0) || (date>length)) {
		cout << "#Fehler:\tDatum ungültig! (" << date << ")" << endl;
		return false;
	}

	if(date!=0) {
		int bdate = 1;
		bdate <<= (date-1);
		dates[i][j].set_date(bdate);
		dates[i][j].set_is_startdate();
		startdates++;
	}

	return true;
}

bool Sudoku::check_in() {

	bool changed = true;
	while(changed) {
		changed = false;
		for(int i=0; i<length; i++) {
			for(int j=0; j<length; j++) {
				changed |= dates[i][j].erase(box_counts);
			}
		}
	}

	for(int i=0; i<length; i++) {
		for(int j=0; j<length; j++) {
			if(dates[i][j].get_possible() < 1) {
				cout << "#Fehler:\tEingabedaten offensichtlich widersprüchlich! (" << i << "," << j << ")" << endl;
				return false;
			}
		}
	}

	return true;
}

bool Sudoku::print_s(const bool cases, const string argv) const {

	ostream *os = NULL;
	ofstream ofs;
	bool color = true;
	bool file  = (argv.size() > 0);

	if(file) {
		ofs.open(argv.c_str(), ios::out);

		if(!ofs.is_open()) {
			cout << "#Fehler:\tDie Datei \"" << argv << "\" konnte nicht geöffnet werden!" << endl;
			return false;
		}
		color = false;
		os = &ofs;
	} else {
		os = &cout;
	}

	for(int i=0; i<length; i++) {
		if (!file) {
			*os << "\t";
		}
		for(int j=0; j<length; j++) {
			if(!os->fail()) {
				*os << dates[i][j].print_date(cases, color) << " " << flush;
				if (j>0 && (j+1)%blocklength == 0 && j<length-1) {
					*os << "|";
				}
			} else {
				cout << "#Fehler:\tI/O-Fehler!" << endl;
				return false;
			}
		}
		*os << endl;

		if (i>0 && (i+1)%blocklength == 0 && i<length-1) {
			*os << "\t";
			for(int j=0; j<length; j++) {
				if (length >= 10) {
					*os << "-";
				}
				if (j>0 && (j+1)%blocklength == 0 && j<length-1) {
					*os << "--+";
				} else {
					*os << "--";
				}
			}
			*os << flush << endl;
		}
	}

	if(argv.size() != 0) {
		ofs.close();
	}

	return true;
}

int Sudoku::remain() const {
	int remain = 0;

	for(int i=0; i<length; i++) {
		for(int j=0; j<length; j++) {
			if(dates[i][j].get_possible() > 1) {
				remain++;
			}
		}
	}

	return remain;
}
