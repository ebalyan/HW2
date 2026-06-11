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
