#pragma once
#include <iostream>
#include <fstream>
#include <queue>	//for level order traversal
#include <string>
using namespace std;

class node {
	char value;	//mostly it is an ascii character-254 which determines that it's a parent
	node* left;
	node* right;
	int f;	//frequency
	friend class binaryTree;
	friend class MinHeap;
public:
	node() : value(0), left(0), right(0), f(0) {}
	node(char line) : value(line), left(0), right(0), f(0) {}
	node(char line, int f) : value(line), left(0), right(0), f(f) {}
};

struct HeapItem {
	int key;																		//frequency
	node* root;																		//value
};
class MinHeap {
private:
	HeapItem* arr;
	int capacity, totalitems;

	//helper functions
	void bubbleup(int n) {	//for insertion
		if (n == 0) return;															//base case

		if (arr[n / 2].key > arr[n].key) {												//if parent is larger than inserted child
			int tempkey = arr[n].key;
			node* tempvalue = arr[n].root;
			arr[n].key = arr[n / 2].key;
			arr[n].root = arr[n / 2].root;
			arr[n / 2].key = tempkey;
			arr[n / 2].root = tempvalue;

			bubbleup(n / 2);															//move to parent
		}
		else return;
	}
	void bubbledown(int n) {	//for deletion
		if (n > totalitems || n * 2 > totalitems) return;								//base case

		if (n * 2 + 1 > totalitems && arr[n].key > arr[n * 2].key) {					//if node has left child only which is less than parent
			int tempkey = arr[n].key;
			node* tempvalue = arr[n].root;

			arr[n].key = arr[n * 2].key;
			arr[n].root = arr[n * 2].root;
			arr[n * 2].key = tempkey;
			arr[n * 2].root = tempvalue;

			return;
		}
		else if (n * 2 + 1 <= totalitems && (arr[n].key > arr[n * 2].key || arr[n].key > arr[n * 2 + 1].key)) {		//if node has both children and one of them is less than current node
			int tempkey = arr[n].key;
			node* tempvalue = arr[n].root;

			if (arr[n * 2].key < arr[n * 2 + 1].key) {										//if left child is less than right child, swap with node
				arr[n].key = arr[n * 2].key;
				arr[n].root = arr[n * 2].root;
				arr[n * 2].key = tempkey;
				arr[n * 2].root = tempvalue;

				bubbledown(n * 2);
			}
			else {																	//if right child is less than left child, swap with node
				arr[n].key = arr[n * 2 + 1].key;
				arr[n].root = arr[n * 2 + 1].root;
				arr[n * 2 + 1].key = tempkey;
				arr[n * 2 + 1].root = tempvalue;

				bubbledown(n * 2 + 1);
			}
		}
		else return;
	}
	int deleteMin(node*& ptr) {	//to create binary Tree
		if (totalitems > 0) {
			int frequency = arr[1].key;

			swap(arr[1].key, arr[totalitems].key);
			swap(arr[1].root, arr[totalitems].root);
			ptr = arr[totalitems].root;
			arr[totalitems].root = NULL;

			totalitems--;
			bubbledown(1);			//call bubbledown

			return frequency;
		}
		return -1;
	}
	void deletetree(node* root) {	//for destructor (LRV deletion)
		if (root == NULL) return;
		deletetree(root->left);
		deletetree(root->right);
		delete root;
	}
public:
	MinHeap() : arr(0), capacity(0), totalitems(0) {}
	int getTotalItems() const { return totalitems; }
	void print() const {	//prints frequency heap
		if (arr) {
			int n = 1;
			for (int i = 1; i <= totalitems; i++) {
				if (arr[i].root->value == '\n')
					cout << "\\n" << ' ' << arr[i].key << '\t';
				else if (arr[i].root->value == ' ')
					cout << "_" << ' ' << arr[i].key << '\t';
				else if (arr[i].root->value == '\t')
					cout << "\\t" << ' ' << arr[i].key << '\t';
				else
					cout << arr[i].root->value << ' ' << arr[i].key << '\t';

				if (i == n) {				//next level
					cout << endl;
					n = n * 2 + 1;
				}
			}cout << endl;
		}
	}

	void insert(int key, char value) {
		if (arr) {
			if ((totalitems + 1) == capacity) {										//capacity is full, double size (via shallowcopy)
				HeapItem* temp = new HeapItem[capacity * 2];
				for (int i = 0; i < capacity; i++) {
					temp[i].key = arr[i].key;
					temp[i].root = arr[i].root;
					arr[i].root = NULL;
				}
				for (int i = capacity; i < capacity * 2; i++) {
					temp[i].key = 0;
					temp[i].root = NULL;
				}
				delete[] arr;
				capacity *= 2;
				arr = temp;
				temp = NULL;
			}
			totalitems++;															//insertion occurs here
			arr[totalitems].key = key;
			arr[totalitems].root = new node(value, key);

			bubbleup(totalitems);	//call bubbleup
		}
		else {
			capacity = 2;
			arr = new HeapItem[capacity];
			for (int i = 0; i < capacity; i++) {
				arr[i].key = 0;
				arr[i].root = NULL;
			}
			insert(key, value);
		}
	}
	void insert(int key, node*& ptr) {
		if (arr) {
			totalitems++;															//insertion occurs here
			arr[totalitems].key = key;
			arr[totalitems].root = ptr;

			bubbleup(totalitems);	//call bubbleup
		}
	}

	node* create_binaryTree() {
		node* ptr = NULL;

		while (totalitems > 1) {		//stops when heap has 1 item only (root of binary tree)
			node* ptr1 = NULL,
				* ptr2 = NULL;
			int f1 = deleteMin(ptr1),
				f2 = deleteMin(ptr2);
			ptr = new node;
			if (f1 <= f2) {				//for sorting and placing min value on left child and max value on right child
				ptr->left = ptr1;
				ptr->right = ptr2;
			}
			else {
				ptr->left = ptr2;
				ptr->right = ptr1;
			}
			ptr->value = 254;
			ptr->f = f1 + f2;
			insert(f1 + f2, ptr);
		}
		if (totalitems == 1) {
			arr[1].root = NULL;			//break link. (taking root to another class (binary tree))
			totalitems--;
		}

		return ptr;
	}

	~MinHeap() {
		if (arr) {
			for (int i = 0; i < capacity; i++)
				if (arr[i].root) {
					deletetree(arr[i].root);		//assumes that every entry in heap is binary tree and deletes using LRV phenomenon
					arr[i].root = NULL;
				}
			delete[] arr;
			arr = NULL;
			capacity = 0;
			totalitems = 0;
		}
	}
};

class binaryTree {
	node* root;

	char* value;	//distinct characters/numerics
	string* codes;	//codes
	int size;		//size of distinct elements

	//helper functions
	void deletetree(node* root) {	//for destructor (LRV deletion)
		if (root == NULL) return;
		deletetree(root->left);
		deletetree(root->right);
		delete root;
	}
	void countElements(node* root, int& n) {	//returns counter of leaf nodes (distinct characters)
		if (root == NULL) return;

		countElements(root->left, n);
		countElements(root->right, n);

		if (!root->left && !root->right) n++;	//increment only when root is leaf node
	}
	void generateCodes(node* root, string& current) {
		if (root == NULL) {
			current.resize(current.size() - 1);		//delete last entered entry for the movement is backwards
			return;
		}

		current = current + "0";	//left movement
		generateCodes(root->left, current);
		current = current + "1";	//right movement
		generateCodes(root->right, current);

		if (!root->left && !root->right) {	//leaf node, place values now
			value[size - 1] = root->value;
			codes[size - 1] = current;
			size--;	//instead of creating new variable, i used size to save memory. it is restored back after this function's call.
		}
		if (current.size() > 0)	//backwards movement
			current.resize(current.size() - 1);
	}
	void insertNode(char c, string s) {	//creating tree while reading from tree.bin (1 character at a time)
		if (!root) root = new node((char)254);

		int n = s.size();
		node* curr = root;
		for (int i = 0; i < n; i++) {
			if (s[i] == '0') {
				if (!curr->left) {
					if (i < n - 1)
						curr->left = new node((char)254);
					else
						curr->left = new node(c);	//target position
				}
				curr = curr->left;
			}
			else {
				if (!curr->right) {
					if (i < n - 1)
						curr->right = new node((char)254);
					else
						curr->right = new node(c);	//target position
				}
				curr = curr->right;
			}
		}
	}
	string spotCode(char c) {	//for encoding, returns char code as string
		if (size == 0) return "";
		for (int i = 0; i < size; i++)
			if (value[i] == c) return codes[i];
		return "";
	}
public:
	binaryTree() : root(0), value(0), codes(0), size(0) {}
	binaryTree(node*& ptr) : value(0), codes(0), size(0) { //constructor called when last item of heap is finished binary tree
		root = ptr;

		//rest of constructor generates codes for all distinct characters
		countElements(root, size);
		if (size > 0) {	//if true, initialize memory for value* and codes*
			value = new char[size];
			codes = new string[size];
			for (int i = 0; i < size; i++) {
				value[i] = '\0';
				codes[i] = "";
			}
			string current = "";
			int backup = size;
			generateCodes(root, current);
			size = backup;
		}
	}
	binaryTree(char const* file) : root(0), value(0), codes(0), size(0) {	//constructor called when tree is read from tree.bin
		ifstream fin;
		fin.open(file);
		if (fin.is_open()) {
			char v = '\0',
				cd = '\0';
			string s = "";

			fin >> noskipws;
			while (!fin.eof()) {
				fin >> v;

				if (fin.eof()) break;

				fin >> cd;
				while (cd != (char)254) {	//determines that current char is set up to be inserted in tree
					s = s + cd;
					fin >> cd;
				}cd = '\0';

				insertNode(v, s);
				s = "";
			}
			fin.close();

			//rest of constructor generates codes for all distinct characters
			countElements(root, size);
			if (size > 0) { //if true, initialize memory for value* and codes*
				value = new char[size];
				codes = new string[size];
				for (int i = 0; i < size; i++) {
					value[i] = '\0';
					codes[i] = "";
				}
				string current = "";
				int backup = size;
				generateCodes(root, current);
				size = backup;
			}
		}
	}

	void printTree() const {			//print tree using level order traversal
		if (root) {
			queue<node*>q1;
			q1.push(root);
			int count = 0,
				bcount = 1;
			node* ptr = NULL;

			while (!q1.empty()) {
				count = bcount;
				bcount = 0;

				while (count > 0) {
					ptr = q1.front();

					if (ptr->value == '\n')
						cout << "\\n" << ' ' << ptr->f << '\t';
					else if (ptr->value == ' ')
						cout << "_" << ' ' << ptr->f << '\t';
					else if (ptr->value == '\t')
						cout << "\\t" << ' ' << ptr->f << '\t';
					else
						cout << ptr->value << ' ' << ptr->f << '\t';

					if (ptr->left) {
						q1.push(ptr->left);
						bcount++;
					}
					if (ptr->right) {
						q1.push(ptr->right);
						bcount++;
					}

					q1.pop();
					count--;
				}cout << endl;
			}
		}
	}
	void printCodes() const {	//prints distinct char and their codes
		for (int i = 0; i < size; i++) {
			if (value[i] == '\n')
				cout << "\\n";
			else if (value[i] == ' ')
				cout << "_";
			else if (value[i] == '\t')
				cout << "\\t";
			else
				cout << value[i];

			cout << '\t' << codes[i] << endl;
		}
	}

	bool storeTree(char const* file) {	//stores tree in tree.bin using value* and codes*
		ofstream fout;
		fout.open(file);
		if (!fout.is_open() || size == 0) return false;

		for (int i = 0; i < size; i++)
			fout << value[i] << codes[i] << (char)254;
		fout.close();
		return true;
	}

	void encodeFile(string s, char const* file) {	//takes input string and encoded file
		ofstream fout;
		fout.open(file);

		string boolean = "";
		int n = s.size();

		//converts input string in boolean form inside another string
		for (int i = 0; i < n; i++)
			boolean = boolean + spotCode(s[i]);
		n = boolean.size();

		//rest code converts boolean string in encoded format
		int digits = 0;
		char c = 0;
		string encoded = "";
		for (int i = 0; i < n; i++) {

			if (boolean[i] == '1') {
				c = c << 1;
				c = c | 0x01;
				digits++;
			}
			else {
				c = c << 1;
				digits++;
			}

			if (digits == 8) {	//8 bits enough to be added to char and stored in encoded.bin
				encoded = encoded + c;
				c = 0;
				digits = 0;
			}
		}
		if (digits > 0)		//if last few bits were left because they were less than 1 byte or 8 bits
			encoded = encoded + c + (char)(digits + 48);
		fout << encoded;	//store encoded version here
		fout.close();

		cout << "Input Data:-\n" << s << endl;
		cout << "\nBinary Encoded Data:-\n";
		for (int i = 0; i < n; i++) {
			if (i % 8 == 0) cout << ' ';
			cout << boolean[i];
		}
		cout << endl;
		cout << "\nCompressed Data:-\n" << encoded << endl;
	}
	void decodeFile(string s, char const* file) {
		ofstream fout;
		fout.open(file);

		if (s == "") {	//nothing to decode
			fout.close();
			return;
		}

		string boolean = "";
		int n = s.size(), n1 = 0, n2 = 0;
		char c, checkup;

		//--------CONVERSION OF ENCODED DATA TO BINARY FORM-------
		for (int i = 0; i < n; i++) {
			if (i == n - 3) break;		//last few char are carefully decoded as they are partially encoded

			c = s[i];
			boolean = boolean + "00000000";
			n1 += 8;

			for (int j = 1; j <= 8; j++) {
				checkup = 0x01 & c;

				if (checkup == 0x00) boolean[n1 - j] = '0';
				else boolean[n1 - j] = '1';

				c = c >> 1;
			}
		}
		c = s[n - 3];
		n2 = s[n - 2] - 48;

		if (n2 > 0 && n2 < 8) {				//last character has only few bits in use, rest are free
			n1 = n1 + n2;
			for (int i = 0; i < n2; i++) boolean = boolean + '0';

			for (int j = 1; j <= n2; j++) {
				checkup = 0x01 & c;

				if (checkup == 0x00) boolean[n1 - j] = '0';
				else boolean[n1 - j] = '1';

				c = c >> 1;
			}
		}
		else {								//last 2 characters are fully consumed, hence they are decoded considering all 16 bits
			for (int i = n - 3; i < n - 1; i++) {
				c = s[i];
				boolean = boolean + "00000000";
				n1 += 8;

				for (int j = 1; j <= 8; j++) {
					checkup = 0x01 & c;

					if (checkup == 0x00) boolean[n1 - j] = '0';
					else boolean[n1 - j] = '1';

					c = c >> 1;
				}
			}
		}
		//last char was repetition hence it wasn't decoded. it was made to prevent skipping eof character in midway (during reading of encoded.bin)

		n = boolean.size();
		cout << "\nBinary Encoded Data:-\n";
		for (int i = 0; i < n; i++) {
			if (i % 8 == 0) cout << ' ';
			cout << boolean[i];
		}
		cout << endl;

		//--------CONVERSION OF BINARY DATA TO HUMAN READABLE WORDS------
		string decoded = "";
		node* curr = root;
		for (int i = 0; i < n; i++) {
			if (boolean[i] == '0') {
				if (curr->left) curr = curr->left;
				else {
					decoded = decoded + curr->value;	//leaf node
					curr = root;						//point curr back to root
					i--;								//start current for loop again
				}
			}
			else {
				if (curr->right) curr = curr->right;
				else {
					decoded = decoded + curr->value;
					curr = root;
					i--;
				}
			}
		}decoded = decoded + curr->value;
		fout << decoded;
		fout.close();
		cout << "\nDecoded Data:-\n";
		cout << decoded << endl;
	}

	~binaryTree() {
		deletetree(root);	//LRV deletion
		root = NULL;
	}
};