#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include <map>
#include <set>
#include <vector>
#include <string>
#include "datastore.h"
#include "product.h"
#include "user.h"

class MyDataStore : public DataStore {
public:
  MyDataStore();
  ~MyDataStore();

void addProduct(Product* p);
void addUser(User* u);
std::vector<Product*> search(std::vector<std::string>& terms, int type;
void dump(std::ostream& ofile);

private:
    std::vector<Product*> products_;
  std::vector<User*> users_;
  std::map<std::string, std::set<Product*> keywordMap_;
};
#endif
