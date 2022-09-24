#include "hash_table.h"

// Konstruktori i destruktor

HashTable::HashTable(int p, int k, int pdh, int qdh) {
	dh = DoubleHashing(pdh, qdh);
	size = int(pow(2, p));
	bucket = k;
	table = new Tile*[size];
	for (int i = 0; i < size; i++) table[i] = new Tile[k];
}

HashTable::HashTable(const HashTable& h) {
	dh = h.dh;
	size = h.size;
	bucket = h.bucket;
	table = new Tile * [size];
	for (int i = 0; i < size; i++) {
		table[i] = new Tile[bucket];
		table[i] = h.table[i];
	}
}

HashTable::HashTable(HashTable&& h) noexcept {
	dh = h.dh;
	size = h.size;
	bucket = h.bucket;
	table = h.table;
	h.table = nullptr;
}

HashTable::~HashTable() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < bucket; j++) {
			if (table[i][j].s != nullptr) delete table[i][j].s;
		}
		delete[] table[i];
	}
	delete[] table;
}

// Operatori dodele

HashTable& HashTable::operator=(const HashTable& h) {
	if (this != &h) {
		// Brisanje stare tabele
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < bucket; j++) {
				if (table[i][j].s != nullptr) delete table[i][j].s;
			}
			delete[] table[i];
		}
		delete[] table;

		// Kopiranje stare tabele
		dh = h.dh;
		size = h.size;
		bucket = h.bucket;
		table = new Tile * [size];
		for (int i = 0; i < size; i++) {
			table[i] = new Tile[bucket];
			table[i] = h.table[i];
		}
	}
	return *this;
}

HashTable& HashTable::operator=(HashTable&& h) noexcept {
	if (this != &h) {
		// Brisanje stare tabele
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < bucket; j++) {
				if (table[i][j].s != nullptr) delete table[i][j].s;
			}
			delete[] table[i];
		}
		delete[] table;

		// Premestanje stare tabele
		dh = h.dh;
		size = h.size;
		bucket = h.bucket;
		table = h.table;
		h.table = nullptr;
	}
	return *this;
}

// Metode za rad sa strukturom studenta

HashTable::Student* HashTable::createStudent(int k, string nam, string cour[MAX_LEN], int len) {
	Student* st = new Student;
	st->key = k;
	st->name = nam;
	st->course_num = len;
	for (int i = 0; i < len; i++) st->courses[i] = cour[i];
	return st;
}

bool HashTable::addCourse(Student* s, string course) {
	if (s == nullptr) return false;
	int i;
	for (i = 0; i < s->course_num; i++) {
		if (s->courses[i] == course) return false;
	}
	s->courses[s->course_num++] = course;
	return true;
}

bool HashTable::removeCourse(Student* s, string course) {
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

void HashTable::printStudent(Student* s) {
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

bool HashTable::loadFromFile(string fileName) {

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
		
		//cout << stoi(id) << " || " << name << " || ";
		//for (int i = 0; i < count; i++) cout << courses[i] << "->";
		//cout << endl;

		this->insert(stoi(id), this->createStudent(stoi(id), name, courses, count));
	}
	
	file.close();
	return true;
}

// Metode za rad sa hes tabelom

void HashTable::set(int p, int k, int pdh, int qdh) {
	//this->clear();
	dh = DoubleHashing(pdh, qdh);
	size = int(pow(2, p));
	bucket = k;
	table = new Tile * [size];
	for (int i = 0; i < size; i++) table[i] = new Tile[k];
}

bool HashTable::insert(int key, student *s) {
	int addr = key % size;
	for (int i = 0; i < bucket; i++) {
		if (table[addr][i].key == key) return false;
		if (table[addr][i].empty || table[addr][i].deleted) {
			table[addr][i].key = key;
			table[addr][i].s = s;
			table[addr][i].empty = false;
			table[addr][i].deleted = false;
			return true;
		}
	}

	int tries = 1;
	int new_addr = dh.getAddress(key, addr, tries, size);
	while (new_addr != addr) {
		for (int i = 0; i < bucket; i++) {
			if (table[new_addr][i].key == key) return false;
			if (table[new_addr][i].empty || table[new_addr][i].deleted) {
				table[new_addr][i].key = key;
				table[new_addr][i].s = s;
				table[new_addr][i].empty = false;
				table[new_addr][i].deleted = false;
				return true;
			}
		}
		tries++;
		new_addr = dh.getAddress(key, addr, tries, size);
	}
	return false;
}

HashTable::Student* HashTable::find(int key) {
	int addr = key % size;
	for (int i = 0; i < bucket; i++) {
		if (table[addr][i].key == key && !(table[addr][i].deleted)) {
			return table[addr][i].s;
		}
	}
	int tries = 1;
	int new_addr = dh.getAddress(key, addr, tries, size);
	while (new_addr != addr) {
		for (int i = 0; i < bucket; i++) {
			if (table[new_addr][i].key == key && !(table[new_addr][i].deleted)) {
				return table[new_addr][i].s;
			}
		}
		tries++;
		new_addr = dh.getAddress(key, addr, tries, size);
	}
	return nullptr;
}

bool HashTable::remove(int key) {
	int del_addr = -1, del_ind = -1;
	int addr = key % size;
	for (int i = 0; i < bucket; i++) {
		if (table[addr][i].key == key && !(table[addr][i].deleted)) {
			del_addr = addr;
			del_ind = i;
		}
	}
	if (del_addr == -1 || del_ind == -1) {
		int tries = 1;
		int new_addr = dh.getAddress(key, addr, tries, size);
		while (new_addr != addr) {
			for (int i = 0; i < bucket; i++) {
				if (table[new_addr][i].key == key && !(table[new_addr][i].deleted)) {
					del_addr = new_addr;
					del_ind = i;
				}
			}
			tries++;
			new_addr = dh.getAddress(key, addr, tries, size);
		}
	}

	if (del_addr == -1 || del_ind == -1) return false;
	else {
		delete table[del_addr][del_ind].s;
		table[del_addr][del_ind].s = nullptr;
		table[del_addr][del_ind].key = -1;
		table[del_addr][del_ind].empty = false;
		table[del_addr][del_ind].deleted = true;
		return true;
	}
}

void HashTable::clear() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < bucket; j++) {
			if (table[i][j].s != nullptr) delete table[i][j].s;
			table[i][j].s = nullptr;
			table[i][j].empty = true;
			table[i][j].deleted = false;
			table[i][j].key = -1;
		}
	}
}

int HashTable::countKeys() const {
	if (table == nullptr) return 0;

	int count = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < bucket; j++) {
			if (table[i][j].key != -1) count++;
		}
	}
	return count;
}

int HashTable::tableSize() const {
	if (table == nullptr) return 0;
	else return size;
}

double HashTable::fillRatio() const {
	if (table == nullptr) return 0;

	int keys = this->countKeys();
	int tiles = size * bucket;

	if (keys == 0 || tiles == 0) return 0;
	else return (keys / (tiles + 0.0));
}

ostream& operator<<(ostream& os, const HashTable& h) {
	for (int i = 0; i < h.size; i++) {
		os << i << "|\t";
		for (int j = 0; j < h.bucket; j++) {
			if (h.table[i][j].empty) os << setw(8) << "   EMPTY";
			else if (h.table[i][j].deleted) os << setw(8) << " DELETED";
			else os << setw(8) << setfill(' ') << h.table[i][j].s->key;
			os << '\t';
		}
		os << endl;
	}
	return os;
}