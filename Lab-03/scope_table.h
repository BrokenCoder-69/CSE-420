#include "symbol_info.h"



extern ofstream outlog; //for wwting to log file from scope_table class


class scope_table
{
private:
    int bucket_count;
    int unique_id;
    scope_table *parent_scope = NULL;
    vector<list<symbol_info *>> table;

    int hash_function(string name)
    {
        // write your hash function here
        int hash = 0;
        for (char c : name) {
            hash += c;
        }
        return hash % bucket_count;
    }

public:
    scope_table();
    scope_table(int bucket_count, int unique_id, scope_table *parent_scope);
    scope_table *get_parent_scope();
    int get_unique_id();
    symbol_info *lookup_in_scope(symbol_info* symbol);
    bool insert_in_scope(symbol_info* symbol);
    bool delete_from_scope(symbol_info* symbol);
    void print_scope_table(ofstream& outlog);
    ~scope_table();

    // you can add more methods if you need
};

// complete the methods of scope_table class


scope_table::scope_table()
{
    this->bucket_count = 10;
    this->unique_id = 1;
    this->table.resize(bucket_count);

    outlog << "New ScopeTable with id " << unique_id << " created" << endl << endl;
}

scope_table::scope_table(int bucket_count, int unique_id, scope_table *parent_scope)
{
    this->bucket_count = bucket_count;
    this->unique_id = unique_id;
    this->parent_scope = parent_scope;
    this->table.resize(bucket_count);

    outlog << "New ScopeTable with id " << unique_id << " created" << endl << endl;
}

scope_table* scope_table::get_parent_scope()
{
    return parent_scope;
}

int scope_table::get_unique_id()
{
    return unique_id;
}

symbol_info* scope_table::lookup_in_scope(symbol_info* symbol)
{
    int index = hash_function(symbol->getname());

    for(auto &sym : table[index]) {
        if(sym->getname() == symbol->getname()) {
            return sym;
        }
    }

    return NULL;
}

bool scope_table::insert_in_scope(symbol_info* symbol)
{
    if(lookup_in_scope(symbol) != NULL) {
        return false;
    }

    int index = hash_function(symbol->getname());
    table[index].push_back(symbol);

    return true;
}


bool scope_table::delete_from_scope(symbol_info* symbol)
{
    int index = hash_function(symbol->getname());

    for(auto it = table[index].begin(); it != table[index].end(); ++it) {
        if((*it)->getname() == symbol->getname()) {
            table[index].erase(it);
            return true;
        }
    }

    return false;
}





void scope_table::print_scope_table(ofstream& outlog)
{
    outlog << "ScopeTable # "+ to_string(unique_id) << endl;

    //iterate through the current scope table and print the symbols and all relevant information
    for(int i = 0; i < bucket_count; i++) {
        if(table[i].size() > 0) {
            outlog << i << " --> " << endl;

            for(auto &symbol : table[i]) {

                outlog << "< " << symbol->getname() 
                       << " : " << symbol->get_type() << " > " << endl;

                if(symbol->get_type_id() == 0) {
                    outlog << "Variable" << endl;
                    outlog << "Type: " << symbol->get_data_type() << endl;
                }
                else if(symbol->get_type_id() == 1) {
                    outlog << "Array" << endl;
                    outlog << "Type: " << symbol->get_data_type() << endl;
                    outlog << "Size: " << symbol->get_array_size() << endl;
                }
                else {
                    outlog << "Function Definition" << endl;
                    outlog << "Return Type: " << symbol->get_return_type() << endl;

                    vector<pair<string,string>> parameters = symbol->get_parameters();

                    outlog << "Parameters: " << parameters.size() << endl;
                    outlog << "Parameter Details: ";

                    for(int j = 0; j < parameters.size(); j++) {
                        if(j == parameters.size() - 1)
                            outlog << parameters[j].first << " " << parameters[j].second;
                        else
                            outlog << parameters[j].first << " " << parameters[j].second << ", ";
                    }

                    outlog << endl;
                }
            }

            outlog << endl;
        }
    }

    outlog << endl;



}

scope_table::~scope_table()
{
    if(unique_id != 1) {
        outlog << "ScopeTable with ID " << unique_id << " removed" << endl << endl;
    }

    for(auto &bucket : table) {
        for(auto &symbol : bucket) {
            delete symbol;
        }
        bucket.clear();
    }

    table.clear();
}
