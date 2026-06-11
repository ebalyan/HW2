#include <iostream>
#include "mydatastore.h"
#include "util.h"

using namespace std;

MyDataStore::MyDataStore()
{
}

MyDataStore::~MyDataStore()
{
    for(size_t i = 0; i < products_.size(); i++) {
        delete products_[i];
    }

    for(size_t i = 0; i < users_.size(); i++) {
        delete users_[i];
    }
}

void MyDataStore::addProduct(Product* p)
{
    products_.push_back(p);

    set<string> keys = p->keywords();

    for(set<string>::iterator it = keys.begin(); it != keys.end(); ++it) {
        keywordMap_[*it].insert(p);
    }
}

void MyDataStore::addUser(User* u)
{
    users_.push_back(u);
    userMap_[convToLower(u->getName())] = u;
}

vector<Product*> MyDataStore::search(vector<string>& terms, int type)
{
    set<Product*> result;

    if(terms.size() == 0) {
        return vector<Product*>();
    }

    if(keywordMap_.find(terms[0]) != keywordMap_.end()) {
        result = keywordMap_[terms[0]];
    }

    for(size_t i = 1; i < terms.size(); i++) {
        set<Product*> temp;

        if(keywordMap_.find(terms[i]) != keywordMap_.end()) {
            temp = keywordMap_[terms[i]];
        }

        if(type == 0) {
            result = setIntersection(result, temp);
        }
        else {
            result = setUnion(result, temp);
        }
    }

    vector<Product*> hits(result.begin(), result.end());
    return hits;
}

void MyDataStore::dump(ostream& ofile)
{
    ofile << "<products>" << endl;

    for(size_t i = 0; i < products_.size(); i++) {
        products_[i]->dump(ofile);
    }

    ofile << "</products>" << endl;

    ofile << "<users>" << endl;

    for(size_t i = 0; i < users_.size(); i++) {
        users_[i]->dump(ofile);
    }

    ofile << "</users>" << endl;
}

bool MyDataStore::addToCart(string username, Product* p)
{
    username = convToLower(username);

    if(userMap_.find(username) == userMap_.end()) {
        return false;
    }

    carts_[username].push_back(p);
    return true;
}

bool MyDataStore::viewCart(string username)
{
    username = convToLower(username);

    if(userMap_.find(username) == userMap_.end()) {
        return false;
    }

    vector<Product*>& cart = carts_[username];

    for(size_t i = 0; i < cart.size(); i++) {
        cout << "Item " << i + 1 << endl;
        cout << cart[i]->displayString() << endl;
        cout << endl;
    }

    return true;
}

bool MyDataStore::buyCart(string username)
{
    username = convToLower(username);

    if(userMap_.find(username) == userMap_.end()) {
        return false;
    }

    User* user = userMap_[username];
    vector<Product*>& cart = carts_[username];
    vector<Product*> remaining;

    for(size_t i = 0; i < cart.size(); i++) {
        Product* p = cart[i];

        if(p->getQty() > 0 && user->getBalance() >= p->getPrice()) {
            p->subtractQty(1);
            user->deductAmount(p->getPrice());
        }
        else {
            remaining.push_back(p);
        }
    }

    cart = remaining;
    return true;
}
