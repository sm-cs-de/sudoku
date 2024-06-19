#include "Solver.h"
#include <sys/time.h>
#include <iostream>

using namespace std;

bool Sudoku::solve() {

	bool changed;

	timeval start, end;
	gettimeofday(&start, 0);

	do {
		changed = false;

		for (int i=0; i<length; i++) {
			for (int j=0; j<length; j++) {
				changed |= dates[i][j].erase(box_counts);
			}
		}
		if (changed) { continue; }

		for (int i=0; i<length; i++) {
			for (int j=0; j<length; j++) {
				changed |= dates[i][j].compare(box_counts);
			}
		}
		if (changed) { continue; }

		for (int i=0; i<length; i++) {
			for (int j=0; j<length; j++) {
				changed |= dates[i][j].single();
			}
		}
		if (changed) { continue; }

		for (int i=0; i<length; i++) {
			for (int j=0; j<length; j++) {
				changed |= dates[i][j].line(box_counts);
			}
		}
		//if (changed) { continue; }

	} while (changed);

	gettimeofday(&end, 0);
    cout << "\n\tReine Rechenzeit: " << ((end.tv_sec-start.tv_sec)*1000.0 + (end.tv_usec-start.tv_usec)/1000.0) << " ms" << endl;

	return true;
}

bool Field::erase(const bool box_counts) const {

	if (possible > 1) { return false; }

	unsigned int new_row, new_col, new_box;
	bool set = false;

	for (int n=0; n<(length-1); n++) {
		new_row = row[n]->get_date() & ~date; // alt: (date ^ row[n]->get_date()) & ~date;
		if (new_row != row[n]->get_date()) {
			row[n]->set_date(new_row);
			set = true;
		}

		new_col = col[n]->get_date() & ~date;
		if (new_col != col[n]->get_date()) {
			col[n]->set_date(new_col);
			set = true;
		}

		if (box_counts) {
			new_box = box[n]->get_date() & ~date;
			if (new_box != box[n]->get_date()) {
				box[n]->set_date(new_box);
				set = true;
			}
		}
	}

	return set;
}

bool Field::compare(const bool box_counts) const {

	if (possible == 1) { return false; }

	unsigned int newdate;
	bool set = false;

	if (this->check_row()) {
		for (int j=0; j<(length-1); j++) {
			if (date != row[j]->get_date()) {
				newdate = row[j]->get_date() & ~date;
				if (newdate != row[j]->get_date()) { set = true; }
				row[j]->set_date(newdate);
			}
		}
	}

	if (this->check_col()) {
		for (int i=0; i<(length-1); i++) {
			if (date != col[i]->get_date()) {
				newdate = col[i]->get_date() & ~date;
				if (newdate != col[i]->get_date()) { set = true; }
				col[i]->set_date(newdate);
			}
		}
	}

	if (box_counts && this->check_box()) {
		for (int k=0; k<(length-1); k++) {
			if (date != box[k]->get_date()) {
				newdate = box[k]->get_date() & ~date;
				if (newdate != box[k]->get_date()) { set = true; }
				box[k]->set_date(newdate);
			}
		}
	}

	return set;
}

bool Field::single() {

	if (possible == 1) { return false; }

	unsigned int all_row = 0, all_col = 0, all_box = 0;
	for (int n=0; n<(length-1); n++) {
		all_row |= row[n]->get_date();
		all_col |= col[n]->get_date();
		all_box |= box[n]->get_date();
	}

	unsigned int new_row , new_col, new_box;
	new_row = date & ~all_row;
	new_col = date & ~all_col;
	new_box = date & ~all_box;


	unsigned int newdate = new_row | new_col | new_box;
	if (newdate) {
		this->set_date(newdate);
		return true;
	}

	return false;
}

bool Field::line(const bool box_counts) const {

	if ((possible == 1) || !box_counts) { return false; }

	unsigned int appear_off_row = 0, appear_in_row = date;
	unsigned int appear_off_col = 0, appear_in_col = date;
	for (int k=0; k<(length-1); k++) {
		if (box[k]->get_possible() == 1) { continue; }		// das wird nämlich schon von 'erase' übernommen

		if (box[k]->I == I) {
			appear_in_row |= box[k]->get_date();
		} else {
			appear_off_row |= box[k]->get_date();
		}

		if (box[k]->J == J) {
			appear_in_col |= box[k]->get_date();
		} else {
			appear_off_col |= box[k]->get_date();
		}
	}

	unsigned int only_in_row = appear_in_row & ~appear_off_row;
	unsigned int only_in_col = appear_in_col & ~appear_off_col;

	bool set = false;
	unsigned int newdate;

	if (only_in_row) {
		for (int j=0; j<(length-1); j++) {
			if (row[j]->K != K) {
				newdate = row[j]->get_date() & ~only_in_row;
				if (newdate != row[j]->get_date()) {
					row[j]->set_date(newdate);
					set = true;
				}
			}
		}
	}

	if (only_in_col) {
		for (int i=0; i<(length-1); i++) {
			if (col[i]->K != K) {
				newdate = col[i]->get_date() & ~only_in_col;
				if (newdate != col[i]->get_date()) {
					col[i]->set_date(newdate);
					set = true;
				}
			}
		}
	}

	return set;
}
