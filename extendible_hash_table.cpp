#include "extendible_hash_table.h"

// Pomocne funkcije

bool checkBit(int key, int pos) {
	int shifted = key >> ((pos - 1));

	if ((shifted & 1) == 1) {
		return true;
	}
	return false;
}

// Konstruktori i destruktori

ExtendibleHashTable::ExtendibleHashTable(int p, int k, int d) {
	max_size = int(pow(2, p));
	bucketsize = k;
	depth = d;
	size = int(pow(2, d));
	table = new Bucket* [max_size];
	for (int i = 0; i < max_size; i++) {
		table[i] = new Bucket;
		table[i]->depth = d;
		table[i]->tileNo = k;
		table[i]->tiles = new Tile[k];
	}
}

ExtendibleHashTable::ExtendibleHashTable(const ExtendibleHashTable& h) {
	size = h.size;
	max_size = h.max_size;
	bucketsize = h.bucketsize;
	depth = h.depth;

	table = new Bucket * [size];
	for (int i = 0; i < size; i++) {
		table[i] = new Bucket;
		table[i]->tiles = new Tile[bucketsize];
		for (int j = 0; j < bucketsize; j++) {
			table[i]->tiles[j] = h.table[i]->tiles[j];
			table[i]->depth = h.table[i]->depth;
			table[i]->tileNo = h.table[i]->tileNo;
		}
	}
}

ExtendibleHashTable::ExtendibleHashTable(ExtendibleHashTable&& h) noexcept {
	size = h.size;
	max_size = h.max_size;
	table = h.table;
	bucketsize = h.bucketsize;
	depth = h.depth;
	h.table = nullptr;
}

ExtendibleHashTable::~ExtendibleHashTable() {
	for (int i = 0; i < max_size; i++) {
		Bucket* b = table[i];
		if (b != nullptr) {
			//for (int j = 0; j < bucketsize; j++) {
				//Tile t = b->tiles[j];
				//if (t.s != nullptr) delete t.s;
			//}
			//delete b;
		}
	}
	delete[] table;
}

// Operatori dodele

ExtendibleHashTable& ExtendibleHashTable::operator=(const ExtendibleHashTable& h) {
	if (this != &h) {
		delete table;

		size = h.size;
		max_size = h.max_size;
		bucketsize = h.bucketsize;
		depth = h.depth;

		table = new Bucket * [size];
		for (int i = 0; i < size; i++) {
			table[i] = new Bucket;
			table[i]->tiles = new Tile[bucketsize];
			for (int j = 0; j < bucketsize; j++) {
				table[i]->tiles[j] = h.table[i]->tiles[j];
				table[i]->depth = h.table[i]->depth;
				table[i]->tileNo = h.table[i]->tileNo;
			}
		}
	}
	return *this;
}

ExtendibleHashTable& ExtendibleHashTable::operator=(ExtendibleHashTable&& h) noexcept {
	if (this != &h) {
		delete table;

		size = h.size;
		max_size = h.max_size;
		table = h.table;
		bucketsize = h.bucketsize;
		depth = h.depth;
		h.table = nullptr;
	}
	return *this;
}

// Metode za rad sa strukturom studenta

ExtendibleHashTable::Student* ExtendibleHashTable::createStudent(int k, string nam, string cour[MAX_LEN], int len) {
	Student* st = new Student;
	st->key = k;
	st->name = nam;
	st->course_num = len;
	for (int i = 0; i < len; i++) st->courses[i] = cour[i];
	return st;
}

bool ExtendibleHashTable::addCourse(Student* s, string course) {
	if (s == nullptr) return false;
	int i;
	for (i = 0; i < s->course_num; i++) {
		if (s->courses[i] == course) return false;
	}
	s->courses[s->course_num++] = course;
	return true;
}

bool ExtendibleHashTable::removeCourse(Student* s, string course) {
	if (s == nullptr) return false;
	int i;
	for (i = 0; i < s->course_num; i++) {
		if (s->courses[i] == course) {
			for (int j = i; j < s->course_num - 1; j++) s->courses[j] = s->courses[j + 1];
			s->course_num--;
			return true;
		}
	}
	return false;
}

void ExtendibleHashTable::printStudent(Student* s) {
	if (s == nullptr) cout << "Pretraga neuspesna!";
	else {
		//cout << endl << "Pretraga uspesna: student sa indeksom " << s->key << " je pronadjen." << endl;
		cout << endl << "Podaci: " << endl << "-------------" << endl << "Indeks: \t";
		cout << setw(4) << setfill('0') << s->key / 10000 << "/" << setw(4) << setfill('0') << s->key % 10000 << ";" << endl;
		cout << "Ime i prezime: \t" << s->name << ";" << endl;
		cout << "Ispiti: \t";
		for (int i = 0; i < s->course_num; i++) {
			if (i != 0) cout << ", ";
			cout << s->courses[i];
		}
		if (s->course_num == 0) cout << "X";
		cout << ";" << endl;
	}
}

// Metoda za unos iz datoteke

bool ExtendibleHashTable::loadFromFile(string fileName) {

	ifstream file(fileName);
	if (!file.is_open()) {
		cout << "Greska u radu sa datotekom!" << endl;
		return false;
	}

	bool first = false;
	string id, name, course, courses[MAX_LEN], line;
	int count = 0;


	while (getline(file, line)) {
		count = 0;

		if (!first) {
			first = true;
			continue;
		}

		int p = 0;
		for (p = 0; p < line.length(); p++) {
			if (line[p] == ',') break;
		}
		id = line.substr(0, p);

		int q = 0;
		for (q = p + 1; q < line.length(); q++) {
			if (line[q] == ',') break;
		}
		name = line.substr(p + 1, q - (p + 1));

		int r = q + 1;
		q = r;
		for (r; r < line.length(); r++) {
			while (line[r] != ' ' && r != line.length()) r++;
			course = line.substr(q, r - q);
			q = r + 1;
			courses[count++] = course;
		}

		this->insert(stoi(id), this->createStudent(stoi(id), name, courses, count));
	}

	file.close();
	return true;
}

// Metode za rad sa hes tabelom

void ExtendibleHashTable::set(int p, int k, int d) {
	max_size = int(pow(2, p));
	bucketsize = k;
	depth = d;
	size = int(pow(2, d));
	table = new Bucket * [max_size];
	for (int i = 0; i < max_size; i++) {
		table[i] = new Bucket;
		table[i]->depth = d;
		table[i]->tileNo = k;
		table[i]->tiles = new Tile[k];
	}
}

bool ExtendibleHashTable::insert(int key, student* s) {
	int addr = key % size;
	for (int i = 0; i < bucketsize; i++) {
		if (table[addr]->tiles[i].key == key) return false;
		if (table[addr]->tiles[i].empty || table[addr]->tiles[i].deleted) {
			table[addr]->tiles[i].key = key;
			table[addr]->tiles[i].s = s;
			table[addr]->tiles[i].empty = false;
			table[addr]->tiles[i].deleted = false;
			return true;
		}
	}

	if (size == max_size) return false;

	if (table[addr]->depth + 1 > depth) { // Ukoliko nova dubina baketa prelazi dubinu tabele vrsi se prelom i dupliranje velicine tabele
		for (int i = 2 * size - 1; i > 0; i -= 2) {

			int old = floor(i / 2.0);

			table[i] = table[old];
			table[i - 1] = table[old];

			if (old == addr) {
				Bucket* oldbucket = table[addr];
				Bucket* new1 = new Bucket, *new2 = new Bucket;
				new1->tileNo = new2->tileNo = bucketsize;
				new1->depth = new2->depth = table[addr]->depth + 1; // Mora da se poveca jer se kod njega vrsi pravi prelom
				new1->tiles = new Tile[bucketsize];
				new2->tiles = new Tile[bucketsize];

				// Umetanje starih kljuceva iz oldbucket u dva nova baketa
				for (int j = 0; j < bucketsize; j++) {
					if (checkBit(oldbucket->tiles[j].key, table[addr]->depth + 1)) { // Ako je d+1 bit 1 ide u 1-baket
						for (int k = 0; k < bucketsize; k++) { // Umetanje u 1-baket
							if (new2->tiles[k].empty || new2->tiles[k].deleted) {
								new2->tiles[k].key = oldbucket->tiles[j].key;
								new2->tiles[k].s = oldbucket->tiles[j].s;
								oldbucket->tiles[j].s = nullptr;
								new2->tiles[k].empty = false;
								new2->tiles[k].deleted = false;
								break;
							}
						}
					}
					else { // Ako je d+1 bit 0 ide u 0-baket
						for (int k = 0; k < bucketsize; k++) { // Umetanje u 0-baket
							if (new1->tiles[k].empty || new1->tiles[k].deleted) {
								new1->tiles[k].key = oldbucket->tiles[j].key;
								new1->tiles[k].s = oldbucket->tiles[j].s;
								oldbucket->tiles[j].s = nullptr;
								new1->tiles[k].empty = false;
								new1->tiles[k].deleted = false;
								break;
							}
						}
					}
				}

				// Umetanje novog kljuca u neki od novih baketa
				if (checkBit(key, table[addr]->depth + 1)) { // Ako je d+1 bit 1 ide u 1-baket
					for (int k = 0; k < bucketsize; k++) { // Umetanje u 1-baket
						if (new2->tiles[k].empty || new2->tiles[k].deleted) {
							new2->tiles[k].key = key;
							new2->tiles[k].s = s;
							new2->tiles[k].empty = false;
							new2->tiles[k].deleted = false;
							break;
						}
					}
				}
				else { // Ako je d+1 bit 0 ide u 0-baket
					for (int k = 0; k < bucketsize; k++) { // Umetanje u 0-baket
						if (new1->tiles[k].empty || new1->tiles[k].deleted) {
							new1->tiles[k].key = key;
							new1->tiles[k].s = s;
							new1->tiles[k].empty = false;
							new1->tiles[k].deleted = false;
							break;
						}
					}
				}

				table[i] = new2; // 1-baket
				table[i - 1] = new1; // 0-baket

				if (oldbucket != nullptr) {
					for (int j = 0; j < oldbucket->tileNo; j++) {
						if (oldbucket->tiles[j].s != nullptr) oldbucket->tiles[j].s = nullptr;
					}
					delete[] oldbucket->tiles;
					delete oldbucket;
				}	
			}
		}
		depth = depth + 1;
		size = size * 2;
		return true;
	}
	else { // Ako ne prevazilazi dubinu samo se uzimaju dva sukcesivna baketa i u njima se vrsi raspodela

		Bucket* oldbucket = table[addr];
		Bucket* new1 = new Bucket, * new2 = new Bucket;
		new1->tileNo = new2->tileNo = bucketsize;
		new1->depth = new2->depth = table[addr]->depth + 1;
		new1->tiles = new Tile[bucketsize];
		new2->tiles = new Tile[bucketsize];


		// Umetanje starih kljuceva iz oldbucket u dva nova baketa
		for (int j = 0; j < bucketsize; j++) {
			if (checkBit(oldbucket->tiles[j].key, table[addr]->depth + 1)) { // Ako je d+1 bit 1 ide u 1-baket
				for (int k = 0; k < bucketsize; k++) { // Umetanje u 1-baket
					if (new2->tiles[k].empty || new2->tiles[k].deleted) {
						new2->tiles[k].key = oldbucket->tiles[j].key;
						new2->tiles[k].s = oldbucket->tiles[j].s;
						oldbucket->tiles[j].s = nullptr;
						new2->tiles[k].empty = false;
						new2->tiles[k].deleted = false;
						break;
					}
				}
			}
			else { // Ako je d+1 bit 0 ide u 0-baket
				for (int k = 0; k < bucketsize; k++) { // Umetanje u 0-baket
					if (new1->tiles[k].empty || new1->tiles[k].deleted) {
						new1->tiles[k].key = oldbucket->tiles[j].key;
						new1->tiles[k].s = oldbucket->tiles[j].s;
						oldbucket->tiles[j].s = nullptr;
						new1->tiles[k].empty = false;
						new1->tiles[k].deleted = false;
						break;
					}
				}
			}
		}

		// Umetanje novog kljuca u neki od novih baketa
		if (checkBit(key, table[addr]->depth + 1)) { // Ako je d+1 bit 1 ide u 1-baket
			for (int k = 0; k < bucketsize; k++) { // Umetanje u 1-baket
				if (new2->tiles[k].empty || new2->tiles[k].deleted) {
					new2->tiles[k].key = key;
					new2->tiles[k].s = s;
					new2->tiles[k].empty = false;
					new2->tiles[k].deleted = false;
					break;
				}
			}
		}
		else { // Ako je d+1 bit 0 ide u 0-baket
			for (int k = 0; k < bucketsize; k++) { // Umetanje u 0-baket
				if (new1->tiles[k].empty || new1->tiles[k].deleted) {
					new1->tiles[k].key = key;
					new1->tiles[k].s = s;
					new1->tiles[k].empty = false;
					new1->tiles[k].deleted = false;
					break;
				}
			}
		}

		/*int start;
		if (addr % 2 == 1) start = addr - 1;
		else start = addr;
		table[start + 1] = new2; // 1-baket
		table[start] = new1; // 0-baket*/

		/*int start = depth * floor(addr / (depth + 0.0));
		int end = start + depth;
		int mid = depth / 2;
		for (int k = start; k < start + mid; k++) table[k] = new1;
		for (int k = start + mid; k < end; k++) table[k] = new2;*/

		int newd = table[addr]->depth;
		int pointNo = pow(2, depth - newd);
		int start = pointNo * floor(addr / (pointNo + 0.0));
		int end = start + pow(2, depth - newd);
		int mid = pow(2, depth - newd) / 2;
		for (int k = start; k < start + mid; k++) table[k] = new1;
		for (int k = start + mid; k < end; k++) table[k] = new2;


		if (oldbucket != nullptr) {
			for (int j = 0; j < oldbucket->tileNo; j++) {
				if (oldbucket->tiles[j].s != nullptr) oldbucket->tiles[j].s = nullptr;
			}
			delete[] oldbucket->tiles;
			delete oldbucket;
		}
		return true;
	}

	return false;
}

ExtendibleHashTable::Student* ExtendibleHashTable::find(int key) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < bucketsize; j++) {
			if (table[i]->tiles[j].key == key) return table[i]->tiles[j].s;
		}
	}
	return nullptr;
}

bool ExtendibleHashTable::remove(int key) {
	int del_addr = -1, del_ind = -1;

	// Pretraga
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < bucketsize; j++) {
			if (table[i]->tiles[j].key == key) {
				del_addr = i;
				del_ind = j;
				break;
			}
		}
	}
	
	if (del_addr == -1 || del_ind == -1) return false;

	// Brisanje samog kljuca iz baketa
	delete table[del_addr]->tiles[del_ind].s;
	table[del_addr]->tiles[del_ind].s = nullptr;
	table[del_addr]->tiles[del_ind].key = -1;
	table[del_addr]->tiles[del_ind].empty = false;
	table[del_addr]->tiles[del_ind].deleted = true;

	// Provera da li baket ostaje popunjen
	for (int i = 0; i < bucketsize; i++) {
		if (table[del_addr]->tiles[i].deleted == false && table[del_addr]->tiles[i].empty == false) {
			hasEmptied();
			return true;
		}
	}

	// Svi pokazivaci koji su pokazivali na prethodno 2 baketa, sada pokazuju na 1
	Bucket* b = table[del_addr], *bnew = nullptr;
	int newd = table[del_addr]->depth;
	int pointNo = pow(2, depth - newd);
	if (pointNo == 1) pointNo = 2;
	int start = pointNo * floor(del_addr / (pointNo + 0.0));
	int end = start + pointNo;
	for (int k = start; k < end; k++) { // Trazenje drugog baketa
		if (table[k] != table[del_addr]) {
			bnew = table[k];
			bnew->depth--; // Ako zapravo dolazi do spajanja 2 u 1 smanju je se dubina baketa
			break;
		}
	}
	if (bnew == nullptr) bnew = b; // Ako nije pronadjen nijedan drugi baket (prvi slucaj kada nema spajanja) ostavlja se stari baket
	for (int k = start; k < end; k++) table[k] = bnew;

	// Provera da li je potrebno sazimati tabelu
	if (depth == 1) {
		hasEmptied();
		return true;
	} // Ne treba sazimati ispod 2^1 polja
	for (int i = 0; i < size; i += 2) { // Ako su svaka dva susedna (2k, 2k+1) isti, moze da se vrsi sazimanje
		if (table[i] != table[i + 1]) {
			hasEmptied();
			return true;
		}
	}

	// Sazimanje
	for (int i = 0; i < size; i += 2) {
		table[i / 2] = table[i];
	}
	depth = depth - 1; // Smanjujemo dubinu tabele
	size = size / 2; // Polovimo velicinu tabele
	hasEmptied();
	return true;
}

void ExtendibleHashTable::hasEmptied() {
	if (depth == 1) { // Ako je tabela dubine 1 i svi kljucevi su orbisani, restartuje se
		for (int i = 0; i < bucketsize; i++) {
			if (!(table[0]->tiles[i].deleted && table[1]->tiles[i].deleted))
				return;
		}
		// Resetovanje
		clear();
		table[0]->depth = 1;
		table[1]->depth = 1;
	}
}

void ExtendibleHashTable::clear() {
	for (int i = 0; i < size; i++) {
		if (table[i] == nullptr) continue;
		for (int j = 0; j < bucketsize; j++) {
			if (table[i]->tiles[j].s != nullptr) delete table[i]->tiles[j].s;
			table[i]->tiles[j].s = nullptr;
			table[i]->tiles[j].empty = true;
			table[i]->tiles[j].deleted = false;
			table[i]->tiles[j].key = -1;
		}
	}
}

int ExtendibleHashTable::countKeys() const {
	if (table == nullptr) return 0;

	int count = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < bucketsize; j++) {
			if (table[i]->tiles[j].key != -1) count++;
		}
	}
	return count;
}

int ExtendibleHashTable::tableSize() const {
	if (table == nullptr) return 0;
	else return size;
}

double ExtendibleHashTable::fillRatio() const {
	if (table == nullptr) return 0;

	int keys = this->countKeys();
	int tiles = size * bucketsize;

	if (keys == 0 || tiles == 0) return 0;
	else return (keys / (tiles + 0.0));
}

ostream& operator<<(ostream& os, const ExtendibleHashTable& h) {
	for (int i = 0; i < h.size; i++) {
		os << i << "|\t";
		int cur = h.bucketsize;
		for (int j = 0; j < cur; j++) {
			if (h.table[i]->tiles[j].empty) os << setw(8) << "   EMPTY";
			else if (h.table[i]->tiles[j].deleted) os << setw(8) << " DELETED";
			else os << setw(8) << setfill(' ') << h.table[i]->tiles[j].s->key;
			os << '\t';
		}
		os << endl;
	}
	return os;
}