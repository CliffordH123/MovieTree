#include <iostream>
#include <fstream>
#include "MovieTree.hpp"

using namespace std;

LLMovieNode* getLLMovieNode(int r, std::string t, int y, float q)
{
	LLMovieNode* lmn =new LLMovieNode();
	lmn->ranking = r;
	lmn->title=t;
	lmn->year =y;
	lmn->rating =q;
	lmn->next =NULL;
	return lmn;
}
/* ------------------------------------------------------ */
MovieTree::MovieTree()
{
	root = NULL;
}
/* ------------------------------------------------------ */

MovieTree::~MovieTree()
{
	destroyTree(root);
}

void destroyTree(TreeNode* t)
{
	if(t)
	{
		destroyTree(t->leftChild);
		destroyTree(t->rightChild);
		delete t;
	}
}

void showMovieCollectionHelper(TreeNode* t)
{
	if(t!=NULL)
	{
		showMovieCollectionHelper(t->leftChild);

		cout << "Movies starting with letter: " << t->titleChar << endl;
		LLMovieNode* m = t->head;
		while (m!=NULL)
		{
			cout << " >> " << m->title << " " << m->rating << endl;
			m = m->next;
		}

		showMovieCollectionHelper(t->rightChild);
	}
}

void MovieTree::showMovieCollection()
{
	showMovieCollectionHelper(root);
}

TreeNode* insertMovieHelper(TreeNode* &t, TreeNode* newNode)
{
   if (t == NULL)
   {
       return newNode;
   }

   if (newNode->titleChar < t->titleChar)
   {
       TreeNode* leftNode = insertMovieHelper(t->leftChild, newNode);
       t->leftChild = leftNode;
       leftNode->parent = t;
   }
   else if (newNode->titleChar > t->titleChar)
   {
       TreeNode* rightNode = insertMovieHelper(t->rightChild, newNode);
       t->rightChild = rightNode;
       rightNode->parent = t;
   }
   else if (newNode->titleChar == t->titleChar)
   {
       LLMovieNode* nodeToInsert = newNode->head;
       LLMovieNode* curr = t->head;
       LLMovieNode* prev = NULL;
       while (curr != NULL && curr->title.compare(nodeToInsert->title) < 0)
       {
           prev = curr;
           curr = curr->next;
       }

       if (curr != NULL && curr->title.compare(nodeToInsert->title) == 0)
       {
           return t; // duplicate title
       }
       else
       {
           if (prev == NULL)
           {
               nodeToInsert->next = t->head;
               t->head = nodeToInsert;
           }
           else if (curr == NULL)
           {
               prev->next = nodeToInsert;
           }
           else
           {
               prev->next = nodeToInsert;
               nodeToInsert->next = curr;
           }
       }
   }

   return t;
}


void MovieTree::insertMovie(int ranking, std::string title, int year, float rating)
{
   LLMovieNode* m = getLLMovieNode(ranking, title, year, rating);

   TreeNode* newNode = new TreeNode;
   newNode->titleChar = title.at(0);
   newNode->head = m;
   newNode->leftChild = NULL;
   newNode->rightChild = NULL;
   newNode->parent = NULL;

   root = insertMovieHelper(root, newNode);
}

TreeNode* removeMovieRecordHelper(TreeNode* t, string title)
{
	if(t==NULL)
	{
		cout << "Movie not found." << endl;
		return t;
	}

	if(title.at(0) < t->titleChar)
	{
		t->leftChild = removeMovieRecordHelper(t->leftChild, title);
	}
	else if(title.at(0) > t->titleChar)
	{
		t->rightChild = removeMovieRecordHelper(t->rightChild, title);
	}
	else if(title.at(0) == t->titleChar)
	{
		LLMovieNode* currNode = t->head;
		LLMovieNode* prevNode = NULL;

		while (currNode != NULL && currNode->title.compare(title) != 0)
       	{
           prevNode = currNode;
           currNode = currNode->next;
       	}

		if (currNode == NULL)
       	{
           cout << "Movie not found." << endl;
           return t;
       	}

		if (prevNode == NULL && t->head->next != NULL)
       	{
           t->head = t->head->next;          
           return t;
       	}

		if (prevNode != NULL && currNode != NULL)
       	{
           prevNode->next = currNode->next;          
           return t;
       	}
		
		if (t->leftChild == NULL && t->rightChild == NULL)
       	{
           delete t;
           t = NULL;
       	}
		else if (t->leftChild == NULL)
       	{
           TreeNode *tempNode = t;
           t->rightChild->parent = t->parent;
           t = t->rightChild;          
           delete tempNode;
       	}
		else if (t->rightChild == NULL)
       	{
           TreeNode *tempNode = t;
           t->leftChild->parent = t->parent;
           t = t->leftChild;          
           delete tempNode;
       	}
		else
       	{
           TreeNode* minValueNode = t->rightChild;
           while (minValueNode->leftChild != NULL)
           {
               minValueNode = minValueNode->leftChild;
           }
           t->head = minValueNode->head;
           t->titleChar = minValueNode->titleChar;
           t->rightChild = removeMovieRecordHelper(t->rightChild, minValueNode->head->title);
       	}
	}

	return t;
}

void MovieTree::removeMovieRecord(string title)
{
	root = removeMovieRecordHelper(root, title);
}

void MovieTree::leftRotation(TreeNode* curr)
{
   TreeNode* right = curr->rightChild;

   if (curr->titleChar == root->titleChar)
   {
       curr->rightChild = right->leftChild;
       right->leftChild = curr;
       right->parent = NULL;
       curr->parent = right;
       if (curr->rightChild != NULL)
       {
           curr->rightChild->parent = curr;
       }
       root = right;
       return;
   }

   if (curr->parent->rightChild->titleChar == curr->titleChar)
   {
       curr->parent->rightChild = right;
   }
   else
   {
       curr->parent->leftChild = right;
   }
   right->parent = curr->parent;
   curr->parent = right;

   curr->rightChild = right->leftChild;
   if (right->leftChild != NULL)
   {
       right->leftChild->parent = curr;
   }
   right->leftChild = curr;
}

void _grader_inorderTraversal(TreeNode * root) {
		if (root == NULL) {
			return;
		}

		_grader_inorderTraversal(root->leftChild);
		cout << root->titleChar << " ";
		_grader_inorderTraversal(root->rightChild);
}


void MovieTree::inorderTraversal() {
	_grader_inorderTraversal(root);
	cout << endl;
}



TreeNode* searchCharHelper(TreeNode* curr, char key)
{
    if (curr == NULL)
        return curr;
    else if(curr->titleChar == key)
        return curr;
    else if (curr->titleChar > key)
        return searchCharHelper(curr->leftChild, key);
    else
        return searchCharHelper(curr->rightChild, key);
}

TreeNode* MovieTree::searchCharNode(char key)
{
    return searchCharHelper(root, key);
}
