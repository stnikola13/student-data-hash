#ifndef _hash_table_h_
#define _hash_table_h_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include "addr_func.h"
#define MAX_LEN 100

using namespace std;

class HashTable {
private:

	// Struktura studenta
	typedef struct student {
		int key;
		int course_num;
		string name;
		string courses[MAX_LEN];
		student() {
			key = -1;
			course_num = 0;
			name = "";
			for (int i = 0; i < MAX_LEN; i++) courses[i] = "";
		}
	} Student;

	// Struktura polja tabele
	typedef struct tile {
		int key;
		Student *s;
		bool empty;
		bool deleted;
		tile() {
			key = -1;
			s = nullptr;
			empty = true;
			deleted = false;
		}
	} Tile;

	// Atributi tabele
	Tile** table = nullptr;
	int bucket = 0;
	int size = 0;
	DoubleHashing dh;

public:

	// Konstruktori i destruktor
	HashTable(int p = 8, int k = 5, int pdh = 5, int qdh = 1);
	HashTable(const HashTable& h);
	HashTable(HashTable&& h) noexcept;
	~HashTable();

	// Operatori dodele
	HashTable& operator=(const HashTable& h);
	HashTable& operator=(HashTable&& h) noexcept;

	// Metode za rad sa strukturom studenta
	Student* createStudent(int k, string nam, string cour[MAX_LEN], int len);
	bool addCourse(Student* s, string course);
	bool removeCourse(Student* s, string course);
	void printStudent(Student* s);

	// Metoda za unos iz datoteke
	bool loadFromFile(string fileName);

	// Metode za rad sa hes tabelom
	void set(int p = 8, int k = 5, int pdh = 5, int qdh = 1);
	Student* find(int key);
	bool insert(int key, student *s);
	bool remove(int key);
	void clear();
	int countKeys() const;
	int tableSize() const;
	double fillRatio() const;
	friend ostream& operator<<(ostream& os, const HashTable& h);
};

#endif
