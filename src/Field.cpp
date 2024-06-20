#include "Solver.hpp"

using namespace std;

uint32_t bit_pow(const uint32_t exp) {
	if (exp > 31) {
		abort();
	}
	return (1 << exp);
}

uint32_t int_div(const uint32_t numerator, const uint32_t denominator) {
	return (numerator / denominator);
}

Field::Field(const uint32_t size, const uint32_t i, const uint32_t j, const uint32_t k) :
		m_date(bit_pow(size) - 1), // Alle relevanten Bits auf 1 (erstes Bit == 1 und nicht 0)
		m_length(size), m_possible(size), m_startdate(false), m_row(nullptr), m_col(
				nullptr), m_box(nullptr), m_i(i), m_j(j), m_k(k) {
}

bool Field::set_data(Field ***dates, const uint32_t blocklength) {
	uint32_t count = 0;
	m_row = static_cast<Field**>(malloc((m_length-1) * sizeof(Field*)));
	if (!m_row) {
		return false;
	}
	for (uint32_t j=0; j<m_length; j++) {
		if (j != m_j) {
			m_row[count] = dates[m_i][j];
			count++;
		}
	}

	count = 0;
	m_col = static_cast<Field**>(malloc((m_length-1) * sizeof(Field*)));
	if (!m_col) {
		return false;
	}
	for (uint32_t i=0; i<m_length; i++) {
		if (i != m_i) {
			m_col[count] = dates[i][m_j];
			count++;
		}
	}

	count = 0;
	m_box = static_cast<Field**>(malloc((blocklength*blocklength-1) * sizeof(Field*)));
	if (!m_box) {
		return false;
	}
	uint32_t rowblock = int_div(m_i, blocklength); // Achtung: liefert nur ganzzahligen Teil. alt: (I - (I % blocklength)) / blocklength
	uint32_t colblock = int_div(m_j, blocklength);
	for (uint32_t i=rowblock*blocklength; i<(rowblock+1)*blocklength; i++) {
		for (uint32_t j=colblock*blocklength; j<(colblock+1)*blocklength; j++) {
			if ((i != m_i) || (j != m_j)) {
				m_box[count] = dates[i][j];
				count++;
			}
		}
	}

	return true;
}

void Field::set_date(const uint32_t number) {
	uint32_t num = 1;
	m_possible = m_length;
	for (uint32_t i=0; i<m_length; i++) {
		if ((num & number) == 0) {
			m_possible--;
		}
		num <<= 1;
	}

	m_date = number;
}

bool Field::check_row() const {
	uint32_t count = 1;
	for (uint32_t j=0; j<(m_length-1); j++) {
		if (m_date == m_row[j]->get_date()) {
			count++;
		}
	}
	if (count == m_possible) {
		return true;
	}

	return false;
}

bool Field::check_col() const {
	uint32_t count = 1;
	for (uint32_t i=0; i<(m_length-1); i++) {
		if (m_date == m_col[i]->get_date()) {
			count++;
		}
	}
	if (count == m_possible) {
		return true;
	}

	return false;
}

bool Field::check_box() const {
	uint32_t count = 1;
	for (uint32_t k=0; k<(m_length-1); k++) {
		if (m_date == m_box[k]->get_date()) {
			count++;
		}
	}
	if (count == m_possible) {
		return true;
	}

	return false;
}

#define START_COLOR "\033[1;31m"
#define CASES_COLOR "\033[0;33m"
#define COLOR_CLEAR "\033[m"
string Field::print_date(const bool cases, const bool color) const {
	string sdate;
	uint32_t num = 1;
	char c[4] = { 0 };

	if (!cases && (m_possible > 1)) {
		if (m_length < 10) {
			sdate.append("~");
		} else {
			sdate.append("~~");
		}

	} else {
		if (m_startdate && color) {
			sdate.append(START_COLOR);
		} else if ((m_possible > 1) && color) {
			sdate.append(CASES_COLOR);
		}
		if (m_date == 0) sdate.append("#");
		for (uint32_t i=0; i<m_length; i++) {
			if ((num & m_date) != 0) {
				if ((m_length > 10) && (i+1 < 10)) {
					sdate.append(" ");
				}
				sprintf(c, "%d", i+1);
				sdate.append(c);
				if (m_possible > 1) {
					sdate.append(",");
				}
			}
			num <<= 1;
		}
		if (color) {
			sdate.append(COLOR_CLEAR);
		}
	}

	return sdate;
}
