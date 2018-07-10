#include "nattable.h"

NatTableRow::NatTableRow(string public_ip, int public_port,
                         string nat_ip, int nat_port,
                         string private_ip, int private_port,
                         int entry_id, char type_entry)
{
    _public_ip = public_ip;
    _public_port = public_port;

    _nat_ip = nat_ip;
    _nat_port = nat_port;

    _private_ip = private_ip;
    _private_port = private_port;

    _entry_id = entry_id;

    _type_entry = type_entry;
}

NatTable::NatTable()
{
    _rows_limit = 10;
}

int NatTable::InsertRow(string public_ip, int public_port,
                        string nat_ip, int nat_port,
                        string private_ip, int private_port,
                        int entry_id, char type_entry)
{
    _rows.emplace_back(public_ip, public_port,
                       nat_ip, nat_port,
                       private_ip, private_port,
                       entry_id, type_entry);

    return NO_ERRORS;
}

int NatTable::DeleteRow(int entry_id)
{
    for (unsigned int i = 0; i < _rows.size(); i++)
        if (_rows[i]._entry_id == entry_id)
        {
            _rows.erase(_rows.begin()+i);
            return NO_ERRORS;
        }

    return ERROR_ENTRY_NOT_EXIST;
}
