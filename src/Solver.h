#ifndef SOLVER
#define SOLVER

#include <string>

unsigned int bit_pow(const int exp);
int int_div(const int numerator, const int denominator);
class Field;

class Sudoku {

	private:
		Field **dates;
		int length;
		int blocklength;
		int startdates;
		bool box_counts;

		bool set_data(const int size);
		bool fill(const int date, const int i, const int j);
		bool check_in();

	public:
		Sudoku();
		~Sudoku();

		bool read(const std::string argv);
		bool print_s(const bool cases, const std::string argv = "") const;
		int remainingdates() const;

		bool solve();

};

class Field {

	private:
		unsigned int date;
		int length;
		int possible;
		bool startdate;
		Field **row;
		Field **col;
		Field **box;

		bool check_row() const;
		bool check_col() const;
		bool check_box() const;

	protected:
		int I;
		int J;
		int K;

	public:
		Field(const int size, const int i, const int j, const int k);
		bool set_data(Field **dates, const int blocklength);
		void set_date(const unsigned int num);
		inline void set_is_startdate() { startdate = true; }

		inline unsigned int get_date() const { return date; }
		std::string print_date(const bool cases, const bool color) const;
		inline int get_possible() const {return possible; }

		bool erase(const bool box_counts) const;
		bool compare(const bool box_counts) const;
		bool single();
		bool line(const bool box_counts) const;
};

#endif
