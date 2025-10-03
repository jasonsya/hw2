#include <string>
#include <set>
#include <vector>
#include "util.h"
#include "mydatastore.h"

using namespace std;

MyDataStore::MyDataStore()
{}

MyDataStore::~MyDataStore()
{
  for(Product* p : products_) {
    delete p;
  }
  for (User* u : users_) {
    delete u;
  }
}

void MyDataStore::addProduct(Product* p) {
  products_.push_back(p);
  set<string> keywords = p->keywords();
  for(const string& keyword : keywords) {
    keywordMap_[keyword].insert(p);
  }
}

void MyDataStore::addUser(User* u) {
  users_.push_back(u);

  userCarts_[u->getName()] = vector<Product*>();
}

vector<Product*> MyDataStore::search(vector<string>& terms, int type) {
  if (terms.empty()) {
    return vector<Product*>();
  }

  vector<string> termsLower;
  for (size_t i = 0; i < terms.size(); ++i) {
    convToLower(terms[i]);
    termsLower.push_back(terms[i]);
  }

  set<Product*> result;

  if (type == 0) {
    if (keywordMap_.find(termsLower[0]) != keywordMap_.end()) {
      result = keywordMap_[termsLower[0]];
    }

    for (size_t i = 1; i < termsLower.size(); i++) {
      set<Product*> termProducts;
      map<string, set<Product*>>::iterator it = keywordMap_.find(termsLower[i]);
      if (it != keywordMap_.end()) {
        termProducts = it->second;
      }
      result = setIntersection(result, termProducts);
    }
  }
  else {
    for (size_t i = 0; i < termsLower.size(); i++) {
      map<string, set<Product*>>::iterator it = keywordMap_.find(termsLower[i]);
      set<Product*> termProducts;
      if (it != keywordMap_.end()) {
        termProducts = it->second;
      }
      result = setUnion(result, termProducts);
    }
  }

  vector<Product*> resultVector;
  for (set<Product*>::iterator it = result.begin(); it != result.end(); ++it) {
    resultVector.push_back(*it);
  }
  return resultVector;
}

void MyDataStore::dump(std::ostream& ofile) {
  ofile << "<products>" << std::endl;
  for(size_t i = 0; i < products_.size(); ++i) {
    products_[i]->dump(ofile);
  }
  ofile << "</products>" << std::endl;

  ofile << "<users>" << std::endl;
  for(size_t i = 0; i < users_.size(); ++i) {
    users_[i]->dump(ofile);
  }
  ofile << "</users>" << std::endl;

}

User* MyDataStore::findUser(const string& username) {
  for(User* user : users_) {
    if (user->getName() == username) {
      return user;
    }
  }
  return NULL;
}

void MyDataStore::addToCart(const std::string& username, Product* product) {
  if (userCarts_.find(username) == userCarts_.end()) {
    std::cout << "Invalid request" << std::endl;
    return;
  }

  if (product == NULL) {
    std::cout << "Invalid request" << std::endl;
    return;
  }

  userCarts_[username].push_back(product);
}

void MyDataStore::viewCart(const std::string& username) {
  if (userCarts_.find(username) == userCarts_.end()) {
    std::cout << "Invalid username" << std::endl;
    return;
  }

  vector<Product*>& cart = userCarts_[username];

  for (size_t i = 0; i < cart.size(); ++i) {
    std::cout << "Item " << (i + 1) << std::endl;
    std::cout << cart[i]->displayString() << std::endl;
    std::cout << std::endl;
  }
}

void MyDataStore::buyCart(const std::string& username) {
  User* user = findUser(username); 
  if(user == NULL) {
    cout << "Invalid username" << endl;
    return;
  }

  vector<Product*>& cart = userCarts_[username];
  vector<Product*> newCart;

  for (Product* product : cart) {
    if (product->getQty() > 0 && user->getBalance() >= product->getPrice()) {
      user->deductAmount(product->getPrice());
      product->subtractQty(1);
    } 
    else {
      newCart.push_back(product);
    }
  }

  cart = newCart;
}

