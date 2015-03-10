/*#ifndef LISTS_H_INCLUDED
#define LISTS_H_INCLUDED

#include <string.h>
#include <iostream>

struct  Node {
        std::string name;
        Node *next;
};

class lists
{
    public:
        lists();
        virtual ~lists();
        Node *head;     // current node that is being pointed to
        int size;
        void out();     // Prints the list, true starts from beginning, false starts from end
        void storeName(std::string);
        void findName(std::string);    // Searches through the list to find the char
        void deleteName(std::string);

};

#endif
*/
