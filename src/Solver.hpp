#ifndef SOLVER
#define SOLVER

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cctype>
#include <cstdint>
#include <sys/time.h>

uint32_t bit_pow(const uint32_t exp);
uint32_t int_div(const uint32_t numerator, const uint32_t denominator);
std::string bin_str(const uint32_t date, const uint32_t length);

class Field;


class Sudoku {
	private:
		Field ***m_dates;
		uint32_t m_length;
		uint32_t m_blocklength;
		uint32_t m_startdates;
		bool m_box_counts;

		bool set_data(const uint32_t size);
		bool fill(const uint32_t date, const uint32_t i, const uint32_t j);
		bool check_in();

	public:
		Sudoku();
		~Sudoku();

		bool read(const std::string argv);
		bool print_s(const bool cases, const std::string argv = "") const;
		uint32_t remain() const;

		bool solve();

};


class Field {
	private:
		uint32_t m_date;
		uint32_t m_length;
		uint32_t m_possible;
		bool m_startdate;
		Field **m_row;
		Field **m_col;
		Field **m_box;

		bool check_row() const;
		bool check_col() const;
		bool check_box() const;

	protected:
		uint32_t m_i;
		uint32_t m_j;
		uint32_t m_k;

	public:
		Field(const uint32_t size, const uint32_t i, const uint32_t j, const uint32_t k);
		bool set_data(Field ***dates, const uint32_t blocklength);
		void set_date(const uint32_t num);
		inline void set_is_startdate() { m_startdate = true; }

		inline uint32_t get_date() const { return m_date; }
		std::string 	print_date(const bool cases, const bool color) const;
		std::string 	print_date_bin() const;
		inline uint32_t get_possible() const {return m_possible; }

		bool erase(const bool box_counts) const; 	/* Wenn nur noch ein Wert möglich ist, wird dieser in anderen Feldern geöscht */
		bool compare(const bool box_counts) const;	/* Identifikation von Paaren, Trippeln, usw. und entspr. löschen anderen Feldern */
		bool single();								/* Prüft ob Feld unikaten Wert enthält und entfernt die anderen Möglichkeiten */
		bool line(const bool box_counts) const;		/* Prüft ob Feld unikaten Wert enthält und löscht Werte aus anderen Feldern (in entspr. Spalte/Zeile) */
};

// TODO: algoritmen erase() und line() zusammenfassen:
// - wenn ein wert in zeile/spalte/box unikat ist, kann er überall entspr. als option gelöscht werden.
// - löschen muss auch erweitert erfolgen, d.h. z.B. ein zeilen-unikater wert kann auch in der zugehörigen box gelöscht werden)
// - ggf. ist auch ein zusammenfassen mit single() möglich
// - so dass am ende ein algorithmus existert, der unikate werte behandelt

#endif
