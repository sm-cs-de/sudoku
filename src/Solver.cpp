#include "Solver.hpp"

using namespace std;


int main(int argc, char *argv[]) {
    cout << "~" << endl;
    cout << "~ Willkommen zu Chrisi's Sudoku-Löser!" << endl;
    cout << "~" << endl << endl;

    if (argc != 2) {
        cout << "#Fehler:\tBitte genau eine Datei angeben!\n" << endl;
        return EXIT_FAILURE;
    }

    Sudoku *s = new Sudoku();
    if (!s->read(argv[1])) {
        cout << "#Fehler:\tDateiformat oder Eingabe von \"" << argv[1] << "\" ungültig!\n" << endl;
        return EXIT_FAILURE;
    }

	cout << "~ Lösungsdaten und Lösung:" << endl;
	timeval start, end;
	gettimeofday(&start, 0);

	bool solvable = s->solve();

    gettimeofday(&end, 0);
    cout << "\tZeitbedarf: " << ((end.tv_sec-start.tv_sec)*1000 + (end.tv_usec-start.tv_usec)/1000.0) << " ms" << endl;
    
	uint32_t remain = s->remain();
	if (!solvable) {
		cout << "\tDas Sudoku ist nicht lösbar!" << endl;
	} else if (remain > 0) {
		cout << "\tFür " << remain << " Felder wurde keine Lösung gefunden!" << endl;
	}
	cout << endl;
    
    if (!s->print_s(true, string(argv[1]).append("-loesung"))) {
		cout << "#Fehler:\tLösungsdatei konnte nicht geschrieben werden!\n" << endl;
        return EXIT_FAILURE;
    }
    s->print_s(false);
    delete s;

	return EXIT_SUCCESS;
}
