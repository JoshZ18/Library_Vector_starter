#include "../includes_usr/fileIO.h"
#include <fstream>
#include <sstream>
#include <vector>

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
			vector<std::string> ele;
			int length = line.length();

			for (int i = 0; i < length; i++) {
				std::string letter = line.substr(i, 1);
				if (letter == ",") {
					ele.push_back(word);
					word = "";
				}
				else {
					word += letter;
				}
			}

			struct book book;

			stringstream id(ele[0]);
			int bookid = 0;
			id >> bookid;

			stringstream state(ele[1]);
			int bookstate = 0;
			state >> bookstate;

			stringstream patronid(ele[2]);
			int loanedid = 0;
			patronid >> loanedid;

			book.book_id = bookid;
			book.title = ele[1];
			book.author = ele[2];
			book.state = OUT;
			book.loaned_to_patron_id = loanedid;

			books.push_back(book);
		}

		if (books.size() != 0) {
			return SUCCESS;
		}

		return NO_BOOKS_IN_LIBRARY;
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

		for (bookItr = books.begin(); bookItr == books.end(); bookItr++) {
			bookWriter << bookItr->book_id << "," << '\"' << bookItr->title << '\", \"' << bookItr->author
					<< "\"," << bookItr->state << "," << bookItr->loaned_to_patron_id;
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
			vector<std::string> words;
			int length = line.length();

			for (int i = 0; i < length; i++) {
				std::string letter = line.substr(i, 1);
				if (letter == ",") {
					words.push_back(word);
					word = "";
				}
				else {
					word += letter;
				}
			}

			stringstream id(words[0]);
			int patronid = 0;
			id >> patronid;

			stringstream numOfBooks(words[2]);
			int numBooks = 0;
			numOfBooks >> numBooks;

			struct patron patron;
			patron.patron_id = patronid;
			patron.name = words[1];
			patron.number_books_checked_out = numBooks;
			patrons.push_back(patron);
		}

		if (patrons.size() != 0) {
			return SUCCESS;
		}

		return NO_PATRONS_IN_LIBRARY;
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

		for (patronItr = patrons.begin(); patronItr == patrons.end(); patronItr++) {
			patronWriter << patronItr->patron_id << "," << '\"' << patronItr->name << "," << patronItr->number_books_checked_out;
		}
		patronWriter.close();
		return SUCCESS;
	}

	patronWriter.close();
	return COULD_NOT_OPEN_FILE;
}
