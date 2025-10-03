#include <sstream>
#include <iomanip>
#include "book.h"
#include "util.h"

using namespace std;

Book::Book (const std::string category, const std::string name, double price,
              int qty, const std::string isbn, const std::string author) :
    Product (category, name, price, qty), 
    isbn_(isbn),
    author_(author)
{}

Book::~Book() 
{}

std::set<std::string> Book::keywords() const {
  std::set<std::string> keywords;

  std::set<std::string> nameKeywords = parseStringToWords(name_);

  std::set<std::string> authorKeywords = parseStringToWords(author_);

  keywords = setUnion(nameKeywords, authorKeywords);

  keywords.insert(isbn_);

  return keywords;
}

std::string Book::displayString() const {
  std::stringstream ss;
  ss << name_ << "\n";
  ss << "Author: " << author_ << " ISBN: " << isbn_ << "\n";
  ss << price_ << " " << qty_ << " left.";

  return ss.str();
}

void Book::dump(std::ostream& os) const {
  os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << 
  isbn_ << "\n" << author_ << endl;
}

std::string Book::getISBN() const {
  return isbn_;
}

std::string Book::getAuthor() const {
  return author_;
}