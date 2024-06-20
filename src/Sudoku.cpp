#include "Solver.hpp"

using namespace std;

Sudoku::Sudoku(): m_dates(nullptr), m_length(0), m_blocklength(0), m_startdates(0), m_box_counts(true) {}

Sudoku::~Sudoku() {
	for(uint32_t i=0; i<m_length; i++) {
		for(uint32_t j=0; i<m_length; i++) {
			delete m_dates[i][j];
		}
	}
	if(m_dates) {
		free(m_dates);
	}
}

bool Sudoku::read(const string argv) {
	ifstream ifs;
	ifs.open(argv.c_str(), ios::in);
	if(!ifs.is_open()) {
		cout << "#Fehler:\tDie Datei \"" << argv << "\" konnte nicht geöffnet werden!" << endl;
		return false;
	}

	uint32_t count = 0;
	char inc = 0, prec = ' ';
	while(true) {
		inc = ifs.get();								// TODO: Das ist immer so umständlich mit get(), weil es nur zeichenweise einliest
		if(ifs.eof()) {
			break;
		} else if(ifs.fail()) {
			cout << "#Fehler:\tI/O-Fehler!" << endl;
			return false;
		} else if(!isdigit(inc)) {
			if(!isspace(inc) && !ifs.eof()) {
				cout << "#Fehler:\tUngültiges Zeichen \"" << inc << "\" gefunden!" << endl;
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
	count = static_cast<uint32_t>(sqr);

	if(count > 32) {	// 32-Bit System :>
		cout << "#Fehler:\tDie Seitenlänge darf nicht größer als 32 sein! (aktuelle Länge: " << count << ")" << endl;
		return false;

	} else if(count == 1) {
		cout << "Tolles Sudoku.. Das kannste selber!\n" << endl;
		return false;
	}

	if(!set_data(count)) {
		cout << "#Fehler:\tDer Speicher konnte nicht reserviert werden!" << endl;
		return false;
	}

	ifs.clear();
	ifs.seekg(ios::beg);

	uint32_t num = 0;
	for(uint32_t i=0; i<count; i++) {
		for(uint32_t j=0; j<count; j++) {
			ifs >> num;
			if(!fill(num, i, j)) { return false; }
		}
	}
	ifs.close();

	cout << "~ Eingegebenes Sudoku:" << endl;
	print_s(false);
	cout << "~ Sonstige Daten:" << endl;
	cout << "\tSeitenlänge:\t"  << m_length << endl;
	cout << "\tBlocklänge:\t"   << m_blocklength << endl;
	cout << "\tStartwerte:\t"   << m_startdates << endl;

	if(!check_in()) {
		print_s(false);
		return false;
	}

	return true;
}

bool Sudoku::set_data(const uint32_t size) {
	m_length = size;

	double n = sqrt(size);
	if(floor(n) != n) {     // n ist keine ganze Zahl
		m_blocklength = size;
		m_box_counts = false;
	} else {
		m_blocklength = n;
	}

	m_dates = static_cast<Field ***>(malloc(size * sizeof(Field **)));
	if(!m_dates) { return false; }

	uint32_t k = 0;
	for(uint32_t i=0; i<size; i++) {
		m_dates[i] = static_cast<Field **>(malloc(size * sizeof(Field *)));
		if(!m_dates[i]) { return false; }

		for(uint32_t j=0; j<size; j++) {
			if(m_box_counts) {
				k = m_blocklength*int_div(i, m_blocklength) + int_div(j, m_blocklength); // TODO: überprüfen!
			}
			m_dates[i][j] = new Field(size, i, j, k);
			if(!m_dates[i][j]) { return false; }
		}
	}

	for(uint32_t i=0; i<size; i++) {
		for(uint32_t j=0; j<size; j++) {
			if(!m_dates[i][j]->set_data(m_dates,m_blocklength)) { return false; }
		}
	}

	return true;
}

bool Sudoku::fill(const uint32_t date, const uint32_t i, const uint32_t j) {
	if((i>=m_length) || (j>=m_length)) {
		cout << "#Fehler:\tIndex falsch! (" << i << "," << j << ")" << endl;
		return false;
	}

	if(date>m_length) {
		cout << "#Fehler:\tDatum ungültig! (" << date << ")" << endl;
		return false;
	}

	if(date!=0) {
		uint32_t bdate = 1;
		bdate <<= (date-1);
		m_dates[i][j]->set_date(bdate);
		m_dates[i][j]->set_is_startdate();
		m_startdates++;
	}

	return true;
}

bool Sudoku::check_in() {
	bool changed = true;
	while(changed) {
		changed = false;
		for(uint32_t i=0; i<m_length; i++) {
			for(uint32_t j=0; j<m_length; j++) {
				changed |= m_dates[i][j]->erase(m_box_counts);
			}
		}
	}

	for(uint32_t i=0; i<m_length; i++) {
		for(uint32_t j=0; j<m_length; j++) {
			if(m_dates[i][j]->get_possible() < 1) {
				cout << "#Fehler:\tEingabedaten offensichtlich widersprüchlich! (" << i << "," << j << ")" << endl;
				return false;
			}
		}
	}

	return true;
}

bool Sudoku::print_s(const bool cases, const string argv) const {
	ostream *os = nullptr;
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

	for(uint32_t i=0; i<m_length; i++) {
		if (!file) {
			*os << "\t";
		}
		for(uint32_t j=0; j<m_length; j++) {
			if(!os->fail()) {
				*os << m_dates[i][j]->print_date(cases, color) << " " << flush;
				if (!file && j>0 && (j+1)%m_blocklength == 0 && j<m_length-1) {
					*os << "|";
				}
			} else {
				cout << "#Fehler:\tI/O-Fehler!" << endl;
				return false;
			}
		}
		*os << endl;

		if (!file && i>0 && (i+1)%m_blocklength == 0 && i<m_length-1) {
			*os << "\t";
			for(uint32_t j=0; j<m_length; j++) {
				if (m_length >= 10) {
					*os << "-";
				}
				if (j>0 && (j+1)%m_blocklength == 0 && j<m_length-1) {
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

uint32_t Sudoku::remain() const {
	uint32_t remain = 0;

	for(uint32_t i=0; i<m_length; i++) {
		for(uint32_t j=0; j<m_length; j++) {
			if(m_dates[i][j]->get_possible() > 1) {
				remain++;
			}
		}
	}

	return remain;
}
