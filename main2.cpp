#include "extendible_hash_table.h"

int main() {

	ExtendibleHashTable h(5, 1, 2);
	string fileName = "", key = "", name = "", name2 = "", course = "", courses[MAX_LEN];
	int i = 0, n = 0, size = -1, k = -1, p = -1, d = -1, opt = -1, count = 0;
	bool cycle = true, isActive = false, j = false, removed = false, added = false;;

	while (cycle) {
		cout << endl << "- - - - - - - - - - Meni - - - - - - - - - -" << endl << "1. Formiranje hes tabele" << endl << "2. Pretraga kljuca" << endl <<
			"3. Umetanje kljuca" << endl << "4. Brisanje kljuca" << endl << "5. Praznjenje tabele" << endl <<
			"6. Broj kljuceva" << endl << "7. Velicina tabele"
			<< endl << "8. Popunjenost tabele" << endl << "9. Ispis tabele" << endl << "0. Kraj programa" << endl << endl << "Unesite redni broj zeljene opcije: ";
		cin >> n;
		if (n < 0 || n > 9) {
			cout << "Pogresno uneta opcija. Molimo pokusajte ponovo!" << endl;
			continue;
		}

		switch (n) {
		case 1:
			if (isActive) {
				cout << "Tabela je vec prethodno kreirana! Izaberite drugu opciju." << endl;
				continue;
			}
			else {
				while (true) {
					cout << "Unesite parametar p primarne hes funkcije, velicinu baketa i pocetnu dubinu tabele d: ";
					cin >> size >> k >> d;
					if (size < 1 || k < 1 || d < 1) cout << "Neispravne vrednosti parametara!" << endl;
					else {
						h.set(size, k, d);
						isActive = true;
						break;
					}
				}
			}
			int m;
			cout << "Da li zelite da unesete podatke iz datoteke?" << endl << "Unesite opciju 1 ako zelite, 0 ako ne: ";
			cin >> m;
			if (m < 0 || m > 1) {
				cout << "Pogresno uneta opcija. Molimo pokusajte ponovo!" << endl;
				continue;
			}
			switch (m) {
			case 0:
				cout << "Molimo iskoristite opciju za umetanje kljuca u meniju pod brojem 3!" << endl;
				break;
			case 1:
				cout << "Unesite ime datoteke: ";
				cin >> fileName;
				j = h.loadFromFile(fileName);
				if (j) cout << "Unos uspesan." << endl;
				break;
			}
			break;

		case 2:
			if (!isActive) {
				cout << "Stablo nije incijalizovano!" << endl;
				continue;
			}
			else {
				cout << "Unesite broj indeksa za pretragu: ";
				cin >> key;
				if (key.length() < 1) {
					cout << "Neispravno uneta rec!" << endl;
					continue;
				}
				else {
					cout << "Unesite 1 za ispis studenta ili 2 za izmenu podataka: ";
					cin >> opt;
					if (opt < 1 || opt > 2) {
						cout << "Neispravno uneta opcija!" << endl;
						continue;
					}
					switch (opt) {
					case 1:
						h.printStudent(h.find(stoi(key)));
						break;
					case 2:
						int opt2;
						cout << "Unesite 1 za dodavanje ispita ili opciju 2 za brisanje ispita: ";
						cin >> opt2;
						switch (opt2) {
						case 1:
							cout << "Unesite sifru ispita koji zelite da dodate: ";
							cin >> course;
							j = h.addCourse(h.find(stoi(key)), course);
							if (j) {
								cout << "Ispit uspesno dodat!" << endl;
							}
							else {
								cout << "Ispit je vec prijavljen!" << endl;
							}
							break;
						case 2:
							cout << "Unesite sifru ispita koji zelite da uklonite: ";
							cin >> course;
							j = h.removeCourse(h.find(stoi(key)), course);
							if (j) {
								cout << "Ispit uspesno uklonjen!" << endl;
							}
							else {
								cout << "Ispit nije prijavljen!" << endl;
							}
							break;
						}
						break;
					}
				}
			}
			break;

		case 3:
			if (!isActive) {
				cout << "Stablo nije incijalizovano!" << endl;
				continue;
			}

			cout << "Unesite indeks studenta: ";
			cin >> key;
			cout << "Unesite ime i prezime studenta: ";
			cin >> name >> name2;
			name = name + " " + name2;
			cout << "Unosite redom prijavljene ispite u zasebnim redovima, a -1 za prekid unosa." << endl;
			count = 0;
			while (true) {
				cin >> course;
				if (course == "-1") break;
				courses[count++] = course;
			}

			if (key.length() < 1) {
				cout << "Neispravan unos reci." << endl;
				continue;
			}
			added = h.insert(stoi(key), h.createStudent(stoi(key), name, courses, count));
			if (added) cout << "Student sa indeksom " << key << " uspesno umetnut u tabelu!" << endl;
			else cout << "Student sa indeksom " << key << " vec postoji u tabeli!" << endl;
			break;

		case 4:
			if (!isActive) {
				cout << "Stablo nije incijalizovano!" << endl;
				continue;
			}

			cout << "Unesite indeks koji zelite da obrisete iz stabla: ";
			cin >> key;
			if (key.length() < 1) {
				cout << "Neispravan unos kljuca." << endl;
				continue;
			}
			removed = h.remove(stoi(key));
			if (removed) cout << "Student sa indeksom " << key << " uspesno obrisan iz tabele!" << endl;
			else cout << "Student sa indeksom " << key << " ne postoji u tabeli!" << endl;
			break;

		case 5:
			if (!isActive) {
				cout << "Stablo nije incijalizovano!" << endl;
				continue;
			}
			h.clear();
			cout << "Tabela ispraznjena!" << endl;
			break;

		case 6:
			if (!isActive) {
				cout << "Stablo nije incijalizovano!" << endl;
				continue;
			}
			cout << "Broj kljuceva u tabeli je " << h.countKeys() << "." << endl;
			break;

		case 7:
			if (!isActive) {
				cout << "Stablo nije incijalizovano!" << endl;
				continue;
			}
			cout << "Tabela ima " << h.tableSize() << " ulaza." << endl;
			break;
		case 8:
			if (!isActive) {
				cout << "Stablo nije incijalizovano!" << endl;
				continue;
			}
			cout << "Popunjenost tabele je " << round(h.fillRatio() * 10000) / 100. << "%." << endl;
			break;
		case 9:
			if (!isActive) {
				cout << "Stablo nije incijalizovano!" << endl;
				continue;
			}
			cout << "Tabela: " << endl;
			cout << "--------------------------------------------" << endl << h << "--------------------------------------------" << endl;
			break;
		case 0:
			cout << "Zdravo!" << endl;
			cycle = false;
			break;
		}
	}


	return 0;
}