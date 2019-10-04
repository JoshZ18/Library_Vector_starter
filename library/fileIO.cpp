#include "../includes_usr/fileIO.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <string>

using namespace std;
/* clears, then loads books from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries in books
 * 			SUCCESS if all data is loaded
 * */
int loadBooks(std::vector<book> &books, const char* filename)
{
	books.clear();

	ifstream myfile;

	myfile.open(filename, ios::in);

	if (myfile.is_open()) {

		while (!myfile.eof()) {
			std::string line = "";
			getline(myfile, line);

			std::string word = "";
			std::string strArr[5];
			int slot = 0;
			int length = line.length();

			for (int i = 0; i < length; i++) {
				std::string letter = line.substr(i, 1);
				if (letter == ",") {
					strArr[slot] = word;
					slot++;
					word = "";
				}
				else {
					word += letter;
				}
			}

			if (word != "\0") {
				strArr[slot] = word;
			}

			struct book book;
			int bookid = 0;
			int loaned = 0;

			std::istringstream iss (strArr[0]);
			iss >> bookid;
			std::istringstream iss1 (strArr[4]);
			iss1 >> loaned;

			book.book_id = bookid;
			book.title = strArr[1];
			book.author = strArr[2];

			if (strArr[3] == "1") {
				book.state = IN;
			}
			else {
				book.state = OUT;
			}

			if (loaned == NO_ONE) {
				book.loaned_to_patron_id = NO_ONE;
			}
			else {
				book.loaned_to_patron_id = loaned;
			}

			books.push_back(book);
		}

		books.pop_back();

		if (books.size() == 0) {
			myfile.close();
			return NO_BOOKS_IN_LIBRARY;
		}

		myfile.close();
		return SUCCESS;

	}

	return COULD_NOT_OPEN_FILE;
}

/* serializes books to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries books (do not create file)
 * 			SUCCESS if all data is saved
 * */
int saveBooks(std::vector<book> &books, const char* filename)
{
	if (books.empty()) {
		return NO_BOOKS_IN_LIBRARY;
	}

	ofstream bookWriter;
	bookWriter.open(filename);

	if (bookWriter.is_open()) {
		vector<book>::iterator bookItr;

		for (bookItr = books.begin(); bookItr != books.end(); bookItr++) {

			bookWriter << bookItr->book_id << "," << bookItr->title << "," << bookItr->author
					<< "," << bookItr->state << "," << bookItr->loaned_to_patron_id << endl;
		}

		bookWriter.close();
		return SUCCESS;
	}

	bookWriter.close();
	return COULD_NOT_OPEN_FILE;
}

/* clears, then loads patrons from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons
 * 			SUCCESS if all data is loaded
 * */
int loadPatrons(std::vector<patron> &patrons, const char* filename)
{
	patrons.clear();

	fstream input;
	input.open(filename, ios::in);

	if (input.is_open()) {
		while (!input.eof()) {
			std::string line = "";
			getline(input, line);

			std::string word = "";
			std::string words[4];
			int slot = 0;
			int length = line.length();

			for (int i = 0; i < length; i++) {
				std::string letter = line.substr(i, 1);
				if (letter == ",") {
					words[slot] = word;
					slot++;
					word = "";
				}
				else {
					word += letter;
				}
			}

			if (word != "\0") {
				words[slot] = word;
			}

			int patronid = 0;
			int numBooks = 0;

			std::istringstream iss (words[0]);
			iss >> patronid;

			std::istringstream iss1 (words[2]);
			iss1 >> numBooks;

			struct patron patron;
			patron.patron_id = patronid;
			patron.name = words[1];
			patron.number_books_checked_out = numBooks;
			patrons.push_back(patron);
		}

		patrons.pop_back();

		if (patrons.size() == 0) {
			input.close();
			return NO_PATRONS_IN_LIBRARY;
		}

		input.close();
		return SUCCESS;

	}
	return COULD_NOT_OPEN_FILE;
}

/* serializes patrons to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons  (do not create file)
 * 			SUCCESS if all data is saved
 * */
int savePatrons(std::vector<patron> &patrons, const char* filename)
{
	if (patrons.empty()) {
		return NO_BOOKS_IN_LIBRARY;
	}

	ofstream patronWriter;
	patronWriter.open(filename);

	if (patronWriter.is_open()) {
		vector<patron>::iterator patronItr;

		for (patronItr = patrons.begin(); patronItr != patrons.end(); patronItr++) {
			patronWriter << patronItr->patron_id << "," << patronItr->name << "," << patronItr->number_books_checked_out << endl;
		}

		patronWriter.close();
		return SUCCESS;
	}

	patronWriter.close();
	return COULD_NOT_OPEN_FILE;
}
