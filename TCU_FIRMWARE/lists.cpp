/*#include "lists.h"
#include <iostream>
lists::lists()
{
    head = new Node;
    head->next = NULL;
    size = 0;

}
lists::~lists()
{
    Node *tmp = this->head;
    Node *temp;
    while(tmp)
    {
        temp = tmp->next;
        delete(tmp);
        tmp = temp;
    }
    tmp = temp = NULL;
}

void lists::out()
{
    Node *pointer = head;
    if(pointer->next == NULL)
    {
        std::cout << "Empty List\n";
        return;
    }

    do{
        std::cout << pointer->next->name << " ";
        pointer = pointer->next;
    }while(pointer->next != NULL);
    std::cout << size <<"\n";
    return;
}
void lists::storeName(std::string Name)
{
    Node *pointer = head;
    while(pointer->next!=NULL)
        pointer = pointer -> next;
    pointer->next = new Node;
    pointer = pointer->next;
    pointer->name = Name;
    pointer->next = NULL;
    size++;

    return;
}
void lists::findName(std::string Name)
{
    Node *pointer = head;
    do
    {
        if(Name == pointer->next->name)
        {
            std::cout << Name << " found.\n";
            return;
        }
        pointer = pointer->next;
    }while(pointer->next != NULL);
    std::cout << Name << " not found.\n";
    return;
}

void lists::deleteName(std::string Name)
{
    Node *temp = head;
    Node *pointer = head;
    do
    {
        if(temp->next->name == Name)
        {
            pointer = temp->next->next;
            std::cout << "Deleting " << Name << "\n";
            delete(temp->next);
            temp->next = pointer;
            size--;
            return;
        }
        temp = temp->next;
    }while(pointer->next->next != NULL);
    std::cout << Name << " not found.\n";
    return;
}
*/
