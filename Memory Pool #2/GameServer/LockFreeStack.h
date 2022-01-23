#pragma once

//template<typename T>
//struct Node
//{
//	T data;
//	Node* node;
//}; 

//-----------------
//	   1Â÷ ½Ãµµ
//-----------------

struct SListEntry
{
	SListEntry* next;
};



struct SListHeader
{
	SListEntry* next = nullptr;
};

// [ ][ ][ ]
// [Header][next]

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);


