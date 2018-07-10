#ifndef NATTABLE_H
#define NATTABLE_H

#include <string>
#include <vector>

using namespace std;

const int NO_ERRORS = -1;
const int ERROR_ADD_ENTRY = 0; // Could not add an entry to the NAT table
const int ERROR_TABLE_FULL = 1; // No more entries can be added to the NAT table
const int ERROR_ENTRY_NOT_EXIST = 2; // The Entry ID does not exist
const int ERROR_ENTRY_ANOTHER_CLIENT = 3; // The ID of the entry to be deleted was added by another Client
const int ERROR_DELETE_ENTRY = 4; // Could not delete an entry of the NAT table

struct NatTableRow
{
    string _public_ip;
    int _public_port;

    string _nat_ip;
    int _nat_port;

    string _private_ip;
    int _private_port;

    int _entry_id;

    char _type_entry;

    NatTableRow(string public_ip, int public_port,
                string nat_ip, int nat_port,
                string private_ip, int private_port,
                int entry_id, char type_entry);
};

class NatTable
{
    public:
        int _rows_limit;
        vector<NatTableRow> _rows;

        NatTable();
        int InsertRow(string public_ip, int public_port,
                      string nat_ip, int nat_port,
                      string private_ip, int private_port,
                      int entry_id, char type_entry);
        int DeleteRow(int entry_id);
};

#endif
