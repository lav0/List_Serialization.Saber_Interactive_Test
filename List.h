//
//  List.h
//  serializedList
//
//  Created by MacBook on 07.06.15.
//  Copyright (c) 2015 Lav. All rights reserved.
//


#include <fstream>
#include <string>

struct ListNode
{
    ListNode* prev;
    ListNode* next;
    ListNode* rand;
    
    std::string data;
};

class List
{
    List(const List&)/* = delete*/;
    List& operator=(const List&)/* = delete*/;
    
    void clean();

public:
    
    List();
    ~List();
    
    void Insert(const std::string& data);
    
    void Serialize(std::ostream& stream);
    void Deserialize(std::istream& stream);
    
    void SetRandomPointers();
    
private:
    
    ListNode* head;
    ListNode* tail;
    int       count;
};
