#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iostream>
#include <fstream>

#include "../includes_usr/library.h"
#include "../includes_usr/datastructures.h"
#include "../includes_usr/fileIO.h"
#include "../includes_usr/constants.h"
using namespace std;

vector<book> books;
vector<patron> patrons;

//NOTE: please ensure patron and book data are loaded from disk before calling the following
//NOTE: also make sure you save patron and book data to disk any time you make a change to them
//NOTE: for files where data is stored see constants.h BOOKFILE and PATRONFILE

/*
 * clear books and patrons containers
 * then reload them from disk 
 */
void reloadAllData(){

	books.clear();
	patrons.clear();

	loadBooks(books, BOOKFILE.c_str());
	loadPatrons(patrons, PATRONFILE.c_str());

}

/* checkout a book to a patron
 * first load books and patrons from disk
 * make sure patron enrolled (patronid is assigned to a patron in patrons container)
 * make sure book in collection (bookid is assigned to a book in books container)
 * 
 * see if patron can check out any more books 
 * 	if not return TOO_MANY_OUT patron has the MAX_BOOKS_ALLOWED_OUT
 * 	
 * if so then check the book out to the patron, set the following fields for the book in the
 * books container
 * book.loaned_to_patron_id = patronid;
 * book.state = OUT;
 * 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 *         PATRON_NOT_ENROLLED
 * 		   BOOK_NOT_IN_COLLECTION
 *         TOO_MANY_OUT patron has the max number of books allowed checked out
 */
int checkout(int bookid, int patronid){
	reloadAllData();
	vector<patron>::iterator patronItr;
	vector<book>::iterator bookItr;
	bool enrolled = false;
	bool in_col = false;

	for (patronItr = patrons.begin(); patronItr != patrons.end(); patronItr++) {
		if (patronItr->patron_id == patronid) {
			enrolled = true;
			break;
		}
	}

	if (!enrolled) {
		return PATRON_NOT_ENROLLED;
	}

	if (patronItr->number_books_checked_out == MAX_BOOKS_ALLOWED_OUT) {
		return TOO_MANY_OUT;
	}

	for (bookItr = books.begin(); bookItr != books.end(); bookItr++) {
		if (bookItr->book_id == bookid) {
			in_col = true;
			break;
		}
	}

	if (!in_col) {
		return BOOK_NOT_IN_COLLECTION;
	}

	bookItr->loaned_to_patron_id = patronItr->patron_id;
	bookItr->state = OUT;
	patronItr->number_books_checked_out++;

	saveBooks(books, BOOKFILE.c_str());
	savePatrons(patrons, PATRONFILE.c_str());

	return SUCCESS;
}

/* check a book back in 
 * first load books and patrons from disk
 * make sure book in collection (bookid is assigned to a book in books container)
 *  	
 * if so find the the patron the book is checked out to and decrement his/hers number_books_checked_out
 * then check the book back in by marking the book.loaned_to_patron_id = NO_ONE and the book.state = IN;
 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 * 		   BOOK_NOT_IN_COLLECTION
 */
int checkin(int bookid){
	reloadAllData();

	vector<book>::iterator bookItr;
	vector<patron>::iterator patronItr;
	int patronid = 0;
	bool haveBook = false;

	for(bookItr = books.begin(); bookItr != books.end(); bookItr++) {
		if (bookItr->book_id == bookid) {
			haveBook = true;
			patronid = bookItr->loaned_to_patron_id;
			break;
		}
	}

	if (!haveBook) {
		return BOOK_NOT_IN_COLLECTION;
	}

	for (patronItr = patrons.begin(); patronItr != patrons.end(); patronItr++) {
		if (patronItr->patron_id == patronid) {
			patronItr->number_books_checked_out--;
			break;
		}
	}

	bookItr->loaned_to_patron_id = NO_ONE;
	bookItr->state = IN;

	saveBooks(books, BOOKFILE.c_str());
	savePatrons(patrons, PATRONFILE.c_str());

	return SUCCESS;
}

/*
 * enroll a patron, duplicate names are fine as patrons are uniquely identified by their patronid
 * first load books and patrons from disk
 * create a patron object, initialize its fields as appropriate, assign him/her the nextPatronID
 * then push the patron onto the patrons container
 * save all changes to the patrons container to disk
 * return 
 *    the patron_id of the person added
 */
int enroll(std::string &name){
//	reloadAllData();

	int id = 0;

	if (patrons.size() != 0) {;
		id = patrons[patrons.size() - 1].patron_id + 1;
	}

	patron patr;
	patr.name = name;
	patr.number_books_checked_out = 0;
	patr.patron_id = id;

	patrons.push_back(patr);

	savePatrons(patrons, PATRONFILE.c_str());

	return id;
}

/*
 * the number of books in the books container
 * (ie. if 3 books returns 3)
 * 
 */
int numbBooks(){
	reloadAllData();
	return books.size();
}

/*
 * the number of patrons in the patrons container
 * (ie. if 3 patrons returns 3)
 */
int numbPatrons(){
	reloadAllData();
	return patrons.size();
}

/*the number of books patron has checked out
 *
 *returns a positive number indicating how many books are checked out 
 *        or PATRON_NOT_ENROLLED         
 */
int howmanybooksdoesPatronHaveCheckedOut(int patronid){
	vector<patron>::iterator patronItr;
	for (patronItr = patrons.begin(); patronItr != patrons.end(); patronItr++) {
		if (patronItr->patron_id == patronid) {
			return patronItr->number_books_checked_out;
		}
	}
	return PATRON_NOT_ENROLLED;
}

/* search through patrons container to see if patronid is there
 * if so returns the name associated with patronid in the variable name
 * 
 * returns SUCCESS found it and name in name
 *         PATRON_NOT_ENROLLED no patron with this patronid
 */
int whatIsPatronName(std::string &name,int patronid){
	vector<patron>::iterator patronItr;

	for (patronItr = patrons.begin(); patronItr != patrons.end(); patronItr++) {
		if (patronItr->patron_id == patronid) {
			if (patronItr->name == name) {
				return SUCCESS;
			}
		}
	}
	return PATRON_NOT_ENROLLED;
}

