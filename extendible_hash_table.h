#ifndef _extendible_hash_table_h_
#define _extendible_hash_table_h_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#define MAX_LEN 100

using namespace std;

class ExtendibleHashTable {
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
		Student* s;
		bool empty;
		bool deleted;
		tile() {
			key = -1;
			s = nullptr;
			empty = true;
			deleted = false;
		}
	} Tile;

	// Struktura baketa
	typedef struct bucket {
		Tile* tiles;
		int depth = 0;
		int tileNo = 0;
		bucket() {
			tiles = nullptr;
			depth = 0;
			tileNo = 0;
		}
	} Bucket;

	// Atributi tabele
	Bucket** table = nullptr;
	int bucketsize = 0;
	int max_size = 0;
	int size = 0;
	int depth = 0;

public:

	// Konstruktori i destruktor
	ExtendibleHashTable(int p = 8, int k = 5, int d = 2);
	ExtendibleHashTable(const ExtendibleHashTable& h);
	ExtendibleHashTable(ExtendibleHashTable&& h) noexcept;
	~ExtendibleHashTable();

	// Operatori dodele
	ExtendibleHashTable& operator=(const ExtendibleHashTable& h);
	ExtendibleHashTable& operator=(ExtendibleHashTable&& h) noexcept;

	// Metode za rad sa strukturom studenta
	Student* createStudent(int k, string nam, string cour[MAX_LEN], int len);
	bool addCourse(Student* s, string course);
	bool removeCourse(Student* s, string course);
	void printStudent(Student* s);

	// Metoda za unos iz datoteke
	bool loadFromFile(string fileName);

	// Metode za rad sa hes tabelom
	void set(int p = 8, int k = 5, int d = 3);
	Student* find(int key);
	bool insert(int key, student* s);
	bool remove(int key);
	void hasEmptied();
	void clear();
	int countKeys() const;
	int tableSize() const;
	double fillRatio() const;
	friend ostream& operator<<(ostream& os, const ExtendibleHashTable& h);
};

#endif
