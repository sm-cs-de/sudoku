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

		void solve();

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
		inline uint32_t get_possible() const {return m_possible; }

		bool erase(const bool box_counts) const;
		bool compare(const bool box_counts) const;
		bool single();
		bool line(const bool box_counts) const;
};


#endif
