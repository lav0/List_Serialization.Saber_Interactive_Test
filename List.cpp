//
//  List.cpp
//  serializedList
//
//  Created by MacBook on 07.06.15.
//  Copyright (c) 2015 Lav. All rights reserved.
//

#include "List.h"
#include "stdlib.h"
#include <map>
#include <vector>


//==============================================================================
std::string findBetween(
                        const std::string& a_strSource,
                        const std::string& a_strLeft,
                        const std::string& a_strRight,
                        size_t& a_position)
{
  auto indexLeft  = a_strSource.find(a_strLeft, a_position) + a_strLeft.length();
  auto indexRight = a_strSource.find(a_strRight, a_position);
    
  a_position = indexRight + a_strRight.length();
    
  return a_strSource.substr(indexLeft, indexRight - indexLeft);
}


//==============================================================================
//
//==============================================================================
List::List()
: head(nullptr)
, tail(nullptr)
, count(0)
{
}

//==============================================================================
List::~List()
{
  clean();
}

//==============================================================================
void List::clean()
{
    auto itr = head;
    
    while (itr != nullptr)
    {
        auto next = itr->next;
        delete itr;
        itr = next;
    }
}

//==============================================================================
void List::Insert(const std::string& data)
{
    if (head == nullptr)
    {
        head = new ListNode;
        head->prev = nullptr;
                
        tail = head;
    }
    else
    {
        tail->next = new ListNode;
        tail->next->prev = tail;
        tail = tail->next;
    }
    
    tail->rand = nullptr;
    tail->next = nullptr;
    tail->data = data;
    
    ++count;
}

//==============================================================================
void List::Serialize(std::ostream &file)
{
    std::map<ListNode*, int> indexedMap;
    
    ////////////////////////////////////////////////////////////////////////////
    // Filling up map of indexes for all nodes
    ////////////////////////////////////////////////////////////////////////////

    int index = 0;
    auto walker = head;
    while (walker != nullptr)
    {
        indexedMap.insert(std::pair<ListNode*, int>(walker, index));
        ++index;
        walker = walker->next;
    }
        
    ////////////////////////////////////////////////////////////////////////////
    // Writing in xml-like document
    ////////////////////////////////////////////////////////////////////////////

    std::string start("<start size=" + std::to_string(count) + ">\n");
    file.write(start.c_str(), start.length());
    
    
    ////////////////////////////////////////////////////////////////////////////
    // Writing one by one node's data and index of pointed by rand node
    ////////////////////////////////////////////////////////////////////////////
    auto node = head;
    while (node != nullptr)
    {
        int randIndex = -1;
        if (node->rand != nullptr)
            randIndex = indexedMap[node->rand];
            
        std::string strNode("<node>\n");
        strNode.append(" <data=\"" + node->data + "\">\n\0");
        strNode.append(" <rand=" + std::to_string(randIndex) + ">\n</node>\n");
        file.write(strNode.c_str(), strNode.length());
        
        node = node->next;
    }
    
    file.write("<end>", 5 * sizeof(char));
}


//==============================================================================
void List::Deserialize(std::istream &stream)
{
  clean();

  auto intSize  = 0;
  std::string str;

  ////////////////////////////////////////////////////////////////////////////
  // Reading size of the list
  ////////////////////////////////////////////////////////////////////////////
  if (std::getline(stream, str))
  {
      size_t pos = 0;
      auto strSize = findBetween(str, "size=", ">", pos);
        
      intSize = atoi(strSize.c_str());
  }
    
  std::vector<ListNode*> nodesRand;
  std::vector<int> indexesRand;

  nodesRand.reserve(intSize);
  indexesRand.reserve(intSize);
    
  size_t position = 0;
    
  std::getline(stream, str, '\0');

  ////////////////////////////////////////////////////////////////////////////
  // 1. Reading and inserting data into the list
  // 2. Mapping of the read nodes and indexes of their random nodes
  ////////////////////////////////////////////////////////////////////////////
  for (int i=0; i<intSize; ++i)
  {
      std::string strNode;
        
      strNode = findBetween(str, "<node>", "</node>", position);
            
      size_t innerPos = 0;
      auto strData = findBetween(strNode, "data=\"", "\">\n", innerPos);
      auto strRand = findBetween(strNode, "rand=", ">\n", innerPos);
            
      auto intRand = atoi(strRand.c_str());
            
      Insert(strData);
      nodesRand.push_back(tail);
      indexesRand.push_back(intRand);
  }
    
  ////////////////////////////////////////////////////////////////////////////
  // Setting up the random pointers
  ////////////////////////////////////////////////////////////////////////////
  auto nodesCount = 0;
  auto nodesWalker  = head;
  while (nodesWalker != nullptr)
  {
      nodesWalker->rand = nodesRand[indexesRand[nodesCount]];
      nodesWalker = nodesWalker->next;
      ++nodesCount;
  }
}



//==============================================================================
void List::SetRandomPointers()
{
    auto itr = head;
    
    while (itr != nullptr)
    {
        int rnd = rand() % count;
        int cnt = 0;
        
        auto randNode = head;
        
        while (cnt++ != rnd)
            randNode = randNode->next;
        
        itr->rand = randNode;
        itr = itr->next;
    }
}
