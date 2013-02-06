// ABT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/**
A B-tree node
*/
class cABTNode
{
public:
	/*
	Each internal node of a B-tree will contain a number of keys.
	The keys act as separation values which divide its subtrees.
	For example, if an internal node has 3 child nodes (or subtrees)
	then it must have 2 keys: a1 and a2.
	All values in the leftmost subtree will be less than a1,
	all values in the middle subtree will be between a1 and a2,
	and all values in the rightmost subtree will be greater than a2.
	*/
	vector< int > myKey;

	vector< cABTNode * > myChild;
	int myIndex;
	string myData;

	cABTNode * Search( int index );
	/** A leaf node has no children */
	bool IsLeaf()	{ return myChild.size() == 0; }
	void Insert( int index, const char * data );
	void Dump();
};

/**
A B-tree implementation
*/
class cABT
{
public:
	
	cABT();
	bool Insert(
		int index,
		const char* data );
	bool Delete(
		int index );
	bool Search(
		int index,
		char * data );
	void Dump();
private:
	cABTNode * myRoot;
	vector < cABTNode* > myTree;
};

typedef vector < int >::iterator iter_key;
typedef vector < cABTNode* >::iterator iter_node;

/**

  Search node and children for an index

  @param[in] index being searched for

  @return pointer to node holding index, 
  or if index is missing ... the internal node
  that should point to leaf containing index

*/
cABTNode * cABTNode::Search( int index )
{
	// check for leaf node
	if( IsLeaf() )
		return this;
	if( ! myKey.size() ) {
		// just one child
		if( myChild[0]->myIndex == index )
			return myChild[0];
		// index must be new
		return this;
	}
	iter_node child = myChild.begin();
	// loop over keys
	for( iter_key k = myKey.begin();
		k != myKey.end(); k++ )
	{
		if( index < *k ) {
			// the index is in this range
			return (*child)->Search( index );
		}
		child++;
	}
	return this;
}

void cABTNode::Insert( int index, const char * data )
{
	cABTNode *new_node = new cABTNode();
	new_node->myIndex = index;
	new_node->myData  = data;
	myChild.push_back( new_node );
}


cABT::cABT()
{
	// create empty root node
	myRoot = new cABTNode();
	myTree.push_back( myRoot );
}
/**

  Search tree for an indez

  @param[in] index
  @param[out] data

  @return true if index found

*/
bool cABT::Search( int index, char* data )
{
	data[0] = '\0';
	cABTNode * n = myRoot->Search( index );
	if( ! n )
		return false;

	strcpy( data, n->myData.c_str() );

	return true;
}
bool cABT::Insert(
				  int index,
				  const char* data )
{
	// check for index already in tree
	cABTNode * n = myRoot->Search( index );
	if( n ) {
		if( n->myIndex == index ) {
			// overwrite old data
			n->myData = data;
			return false;
		}
		n->Insert( index, data );
	}
}

void cABT::Dump()
{
	myRoot->Dump();
}
void cABTNode::Dump()
{
	if( IsLeaf() )
		printf("Leaf %d %s\n",
			myIndex, myData.c_str());
	else {
		printf("Internal\n");
		for( iter_node k = myChild.begin();
			k != myChild.end(); k++ )
		{
			(*k)->Dump();
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	printf("ABT: A B-Tree implementation\n");
	cABT theABT;
	char cmd;
	int index;
	char data[1000];
	// loop for ever while user enters commands
	while( 1 ) {
		printf("Enter command\n");
		scanf_s("%c",&cmd,1);

		// execute command entered
		switch( cmd ) {
		case 'A': case 'a':
			scanf_s("%d %s",&index,&data,999);
			printf("Add at index %d the data '%s'\n",
				index,data);
			theABT.Insert( index, data );
			theABT.Dump();
			break;

		case 'D': case 'd':
			scanf_s("%d",&index);
			printf("Delete index %d\n",index);
			break;

		case 'S': case 's':
			scanf_s("%d",&index);
			printf("Search for index %d\n",index);
			if( theABT.Search( index, data ) ) {
				printf("Found '%s'\n",data);
			} else {
				printf("No such index\n");
			}
			break;

		case 'Z': case 'z':
			printf("Delete all data\n");
			break;
		}
	}

	return 0;
}

