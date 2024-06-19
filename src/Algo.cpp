#include "Solver.hpp"

using namespace std;

void Sudoku::solve() {
	bool changed = false;
	do {
		changed = false;

		for (uint32_t i=0; i<m_length; i++) {
			for (uint32_t j=0; j<m_length; j++) {
				changed |= m_dates[i][j]->erase(m_box_counts);
			}
		}
		if (changed) { continue; }

		for (uint32_t i=0; i<m_length; i++) {
			for (uint32_t j=0; j<m_length; j++) {
				changed |= m_dates[i][j]->compare(m_box_counts);
			}
		}
		if (changed) { continue; }

		for (uint32_t i=0; i<m_length; i++) {
			for (uint32_t j=0; j<m_length; j++) {
				changed |= m_dates[i][j]->single();
			}
		}
		if (changed) { continue; }

		for (uint32_t i=0; i<m_length; i++) {
			for (uint32_t j=0; j<m_length; j++) {
				changed |= m_dates[i][j]->line(m_box_counts);
			}
		}
		//if (changed) { continue; }

	} while (changed);
}

bool Field::erase(const bool box_counts) const {
	if (m_possible > 1) { return false; }

	uint32_t new_row = 0, new_col = 0, new_box = 0;
	bool set = false;

	for (uint32_t n=0; n<(m_length-1); n++) {
		new_row = m_row[n]->get_date() & ~m_date; // alt: (date ^ row[n]->get_date()) & ~date;
		if (new_row != m_row[n]->get_date()) {
			m_row[n]->set_date(new_row);
			set = true;
		}

		new_col = m_col[n]->get_date() & ~m_date;
		if (new_col != m_col[n]->get_date()) {
			m_col[n]->set_date(new_col);
			set = true;
		}

		if (box_counts) {
			new_box = m_box[n]->get_date() & ~m_date;
			if (new_box != m_box[n]->get_date()) {
				m_box[n]->set_date(new_box);
				set = true;
			}
		}
	}

	return set;
}

bool Field::compare(const bool box_counts) const {
	if (m_possible == 1) { return false; }

	uint32_t newdate = 0;
	bool set = false;

	if (check_row()) {
		for (uint32_t j=0; j<(m_length-1); j++) {
			if (m_date != m_row[j]->get_date()) {
				newdate = m_row[j]->get_date() & ~m_date;
				if (newdate != m_row[j]->get_date()) {
					m_row[j]->set_date(newdate);
					set = true;
				}
			}
		}
	}

	if (check_col()) {
		for (uint32_t i=0; i<(m_length-1); i++) {
			if (m_date != m_col[i]->get_date()) {
				newdate = m_col[i]->get_date() & ~m_date;
				if (newdate != m_col[i]->get_date()) {
					m_col[i]->set_date(newdate);
					set = true;
				}
			}
		}
	}

	if (box_counts && check_box()) {
		for (uint32_t k=0; k<(m_length-1); k++) {
			if (m_date != m_box[k]->get_date()) {
				newdate = m_box[k]->get_date() & ~m_date;
				if (newdate != m_box[k]->get_date()) {
					m_box[k]->set_date(newdate);
					set = true;
				}
			}
		}
	}

	return set;
}

bool Field::single() {
	if (m_possible == 1) { return false; }

	uint32_t all_row = 0, all_col = 0, all_box = 0;
	for (uint32_t n=0; n<(m_length-1); n++) {
		all_row |= m_row[n]->get_date();
		all_col |= m_col[n]->get_date();
		all_box |= m_box[n]->get_date();
	}

	uint32_t new_row = m_date & ~all_row;
	uint32_t new_col = m_date & ~all_col;
	uint32_t new_box = m_date & ~all_box;

	uint32_t newdate = new_row | new_col | new_box;
	if (newdate > 0) {
		set_date(newdate);
		return true;
	}

	return false;
}

bool Field::line(const bool box_counts) const {
	if ((m_possible == 1) || !box_counts) { return false; }

	uint32_t appear_off_row = 0, appear_in_row = m_date;
	uint32_t appear_off_col = 0, appear_in_col = m_date;
	for (uint32_t k=0; k<(m_length-1); k++) {
		if (m_box[k]->get_possible() == 1) { continue; }		// das wird nämlich schon von 'erase' übernommen

		if (m_box[k]->m_i == m_i) {
			appear_in_row |= m_box[k]->get_date();
		} else {
			appear_off_row |= m_box[k]->get_date();
		}

		if (m_box[k]->m_j == m_j) {
			appear_in_col |= m_box[k]->get_date();
		} else {
			appear_off_col |= m_box[k]->get_date();
		}
	}

	uint32_t only_in_row = appear_in_row & ~appear_off_row;
	uint32_t only_in_col = appear_in_col & ~appear_off_col;

	bool set = false;
	uint32_t newdate = 0;

	if (only_in_row) {
		for (uint32_t j=0; j<(m_length-1); j++) {
			if (m_row[j]->m_k != m_k) {
				newdate = m_row[j]->get_date() & ~only_in_row;
				if (newdate != m_row[j]->get_date()) {
					m_row[j]->set_date(newdate);
					set = true;
				}
			}
		}
	}

	if (only_in_col) {
		for (uint32_t i=0; i<(m_length-1); i++) {
			if (m_col[i]->m_k != m_k) {
				newdate = m_col[i]->get_date() & ~only_in_col;
				if (newdate != m_col[i]->get_date()) {
					m_col[i]->set_date(newdate);
					set = true;
				}
			}
		}
	}

	return set;
}
