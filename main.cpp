#include <iostream>
#include <fstream>
#include <string>
#include "huffman.h"
using namespace std;

bool readFile(const char* f, string& s);
int removeDuplicates(string& s);		//counts first index char and removes it from entire string, returns count at end

int main() {
	cout << "INPUT-FILE---------------------------------------------\n\n";

	char const* file = "input.txt";

	string s;
	if (!readFile(file, s)) {
		cout << "file not found!\n";
		return -1;
	}
	s.resize(s.size() - 1);
	cout << "Input Data:-\n" << s << endl << endl;

	cout << "CREATING MIN HEAP--------------------------------------\n\n";

	int frequency = 0;
	char c = '\0';
	string sbackup = s;

	MinHeap m1;
	while (!s.empty()) {
		c = s[0];
		frequency = removeDuplicates(s);
		m1.insert(frequency, c);			//distinct character inserted with its frequency as position identifer in heap
	}s = sbackup;
	cout << "Frequency Min-Heap:-\n";
	m1.print();
	cout << endl;

	cout << "CREATING BINARY TREE AND CODES-------------------------\n\n";

	node* root = m1.create_binaryTree();
	binaryTree b1(root);

	cout << "Binary Tree:-\n";
	b1.printTree();
	cout << endl;

	cout << "Binary Codes:-\n";
	b1.printCodes();
	cout << endl;

	cout << "STORING AND/OR READING BINARY TREE---------------------\n\n";

	char const* treefile = "tree.bin";

	if (!b1.storeTree(treefile)) {
		cout << "Tree doesn't exist so it can't be stored!\n\n";
		return -1;
	}
	else cout << "Tree stored successfully!\n\n";

	/*binaryTree b2(treefile);
	cout << "Binary Tree Read from file:-\n";
	b2.printTree();
	cout << endl;*/

	cout << "ENCODING INPUT FILE-----------------------------------\n\n";

	char const* encodedfile = "encoded.bin";
	b1.encodeFile(s, encodedfile);
	cout << endl;

	cout << "DECODING ENCODED FILE---------------------------------\n\n";

	binaryTree b3(treefile);
	cout << "Binary Tree Read from file:-\n";
	b3.printTree();
	cout << endl;

	char const* decodedfile = "decoded.txt";

	//reading encoded data
	s = "";
	if (!readFile(encodedfile, s)) {
		cout << "Encoded File not found!\n";
		return -1;
	}
	cout << "Encoded Data:-\n";
	int n = s.size();
	for (int i = 0; i < n - 1; i++) cout << s[i];
	cout << endl << endl;

	//and now using binary tree b2 (which was created by reading tree.bin) to decode data
	b3.decodeFile(s, decodedfile);

	cout << "\nVALIDITY CHECK----------------------------------------\n\n";
	string t = "";
	if (readFile(file, s) && readFile(decodedfile, t)) {
		if (s == t) cout << "Input file and Decoded file are same hence proving that encoding went well!\n\n";
		else cout << "Input file and Decoded file aren't same hence proving that the algorithm is somewhere defected!\n\n";
	}
	else cout << "Either one or both files couldn't be opened!\n\n";

	cout << "THE_END-------------------------------------------------\n\n";

	system("pause");
	return 0;
}

bool readFile(const char* f, string& s) {
	ifstream fin;
	fin >> noskipws;
	fin.open(f);
	if (!fin.is_open()) return false;

	s = "";
	char c;
	while (!fin.eof()) {
		fin >> c;
		if (fin.eof()) {
			s = s + c;
			fin >> c;
			if (fin.eof()) break;
		}
		s = s + c;
	}
	fin.close();
	return true;
}
int removeDuplicates(string& s) {
	char c = s[0];
	int count = 0,
		size = s.size();

	for (int i = 0; i < size; i++) {
		if (s[i] == c) {
			for (int j = i; j < size - 1; j++) {
				s[j] = s[j + 1];
			}
			s.resize(size - 1);

			count++;
			size--;
			i--;
		}
	}
	return count;
}