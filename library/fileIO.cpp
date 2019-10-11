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

	//Opens the file
	myfile.open(filename, ios::in);

	//Checks to see if file open before reading data
	if (myfile.is_open()) {
		while (!myfile.eof()) {
			std::string line = "";
			getline(myfile, line);

			std::string word = "";
			std::string strArr[5];
			int slot = 0;
			int length = line.length();

			//Loops through every line in the file and parses out data
			for (int i = 0; i < length; i++) {
				std::string letter = line.substr(i, 1);
				//Separates pieces by data based on ,
				if (letter == ",") {
					strArr[slot] = word;
					slot++;
					word = "";
				}
				else {
					word += letter;
				}
			}

			//Adds the last part of a book
			if (word != "\0") {
				strArr[slot] = word;
			}

			struct book book;
			int bookid = 0;
			int loaned = 0;

			//Converts strings into ints
			std::istringstream iss (strArr[0]);
			iss >> bookid;
			std::istringstream iss1 (strArr[4]);
			iss1 >> loaned;

			book.book_id = bookid;
			book.title = strArr[1];
			book.author = strArr[2];

			//Converts the enum values to strings to compare values
			//and assign whether it is in or out
			int state = IN;
			std::string in = to_string(state);

			if (strArr[3] == in) {
				book.state = IN;
			}
			else {
				state = OUT;
				std::string out = to_string(state);
				if (strArr[3] == out) {
					book.state = OUT;
				}
				else {
					book.state = UNKNOWN;
				}
			}

			if (loaned == NO_ONE) {
				book.loaned_to_patron_id = NO_ONE;
			}
			else {
				book.loaned_to_patron_id = loaned;
			}

			books.push_back(book);
		}

		//Creates an extra book that needs to be removed
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

	//Attempts to open a file to write to it
	ofstream bookWriter;
	bookWriter.open(filename);

	//Checks to see if the file is open
	if (bookWriter.is_open()) {
		vector<book>::iterator bookItr;
		//Iterates through and writes the books elements
		for (bookItr = books.begin(); bookItr != books.end(); bookItr++) {
			bookWriter << bookItr->book_id << "," << bookItr->title << "," << bookItr->author
					<< "," << bookItr->state << "," << bookItr->loaned_to_patron_id << endl;
		}

		bookWriter.close();
		return SUCCESS;
	}

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

	//Opens a file to read in data
	fstream input;
	input.open(filename, ios::in);

	//Checks to see if the file is open
	if (input.is_open()) {
		while (!input.eof()) {
			std::string line = "";
			getline(input, line);

			std::string word = "";
			std::string words[4];
			int slot = 0;
			int length = line.length();

			//Reads through each line and parses out data based on a ,
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

			//Checks to see if word has data to input
			if (word != "\0") {
				words[slot] = word;
			}

			int patronid = 0;
			int numBooks = 0;

			//Converts strings to ints
			std::istringstream iss (words[0]);
			iss >> patronid;
			std::istringstream iss1 (words[2]);
			iss1 >> numBooks;

			//Creates a patron with the data
			struct patron patron;
			patron.patron_id = patronid;
			patron.name = words[1];
			patron.number_books_checked_out = numBooks;
			patrons.push_back(patron);
		}

		//Removes the last patron due to an extra patron being created
		patrons.pop_back();

		//Checks to see if there are no patrons
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
	//Checks to make sure there are patrons
	if (patrons.empty()) {
		return NO_BOOKS_IN_LIBRARY;
	}

	//Attempts to open a file to write to
	ofstream patronWriter;
	patronWriter.open(filename);

	//Checks to see if the file is open
	if (patronWriter.is_open()) {
		vector<patron>::iterator patronItr;

		//Iterates through to write patron data to a file
		for (patronItr = patrons.begin(); patronItr != patrons.end(); patronItr++) {
			patronWriter << patronItr->patron_id << "," << patronItr->name << "," << patronItr->number_books_checked_out << endl;
		}

		patronWriter.close();
		return SUCCESS;
	}

	patronWriter.close();
	return COULD_NOT_OPEN_FILE;
}
