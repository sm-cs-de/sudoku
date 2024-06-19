#include "Solver.h"
#include <iostream>
#include <cstdlib>
#include <sys/time.h>

using namespace std;

int main(int argc, char *argv[]) {

    cout << "\n~" << endl;
    cout << "~\tWillkommen zu Chrisi's Sudoku-Löser." << endl;
    cout << "~\n" << endl;

    if (argc != 2) {
        cout << "#Fehler:\tBitte genau eine Datei angeben!\n" << endl;
        return EXIT_FAILURE;
    }

	timeval start, end;
	gettimeofday(&start, 0);

    Sudoku *s = new Sudoku();

    if (!s->read(argv[1])) {
        cout << "#Fehler:\tDateiformat oder Eingabe von \"" << argv[1] << "\" ungültig!\n" << endl;
        return EXIT_FAILURE;
    }

	cout << endl;
	cout << "~ Lösungsdaten und Lösung:" << endl;

    if (!s->solve()) { //TODO: sinnlos, da das nicht vorkommen kann
        cout << "#Fehler:\tSudoku nicht lösbar!\n\t\tBitte Eingabe überprüfen!\n" << endl;
        return EXIT_FAILURE;
    }

    if (!s->print_s(true, string(argv[1]).append("-Lösung"))) {
		cout << "#Fehler:\tLösungsdatei konnte nicht geschrieben werden!\n" << endl;
        return EXIT_FAILURE;
    }

    gettimeofday(&end, 0);
    cout << "\tZeit mit I/O und Initialisierung: " << ((end.tv_sec-start.tv_sec)*1000 + (end.tv_usec-start.tv_usec)/1000.0) << " ms" << endl;

	int remain = s->remainingdates();
	if (remain) {
		cout << "\tFür " << remain << " Felder wurde keine Lösung gefunden!" << endl;
	}
	cout << endl;

    s->print_s(false);
    delete s;

	return EXIT_SUCCESS;
}

/*
TODO: Herausfinden, warum es bei `test_9` nicht richtig funktioniert
TODO: 'line' hängt sich ab und zu auf
*/