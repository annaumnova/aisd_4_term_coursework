#pragma once
#include<iostream>

template <class T>
class List
{
private:
	class Node {
	public:
		T inf; //the information part of the beginning element
		Node* previous; //the pointer to the previous
		Node* next; //the pointer to the next

		//constructor
		Node(const T& inf = 0, Node* next = NULL, Node* previous = NULL) {
			this->inf = inf;
			this->next = next;
			this->previous = previous;
		}

		//destructor
		~Node() {}
	};

	

	//get head
	Node* getHead() {
		return head;
	}


public:

//int size;//size of list
	Node* head; //the pointer of the beginning
	Node* end; //the pointer of the end
	//constructor
	List() {
		head = NULL;
		end = NULL;
	}

	//destructor
	~List() {
		Node* current = head;
		while (current != NULL) {
			current = current->next;
			delete head;
			head = current;
		}
	}

	//get size
	int getSize() {
		Node* current = head;
		int size = 0;
		while (current != NULL) {
			size++;
			current = current->next; //to next
		}
		return size; //size
	}

	//add in the end
	void pushBack(const T& elem) {
		Node* modernNode = new Node(elem, NULL, end);

		if (head == NULL) //if empty list?
			head = modernNode;
		else
			end->next = modernNode;
		end = modernNode;
	}

	//delete at the end
	void popBack() {
		if (head != NULL) {
			Node* current = end->previous;
			delete end;
			end = current;
			current->next = NULL;
		}
	}

	//delete at the beginning
	void popFront() {
		if (head != NULL) {
			Node* current = head->next;
			delete head;
			head = current;
			current->previous = NULL;
		}
	}

	//delete the node
	void remove(int index_position) {
		if (index_position >= 0 && index_position < getSize()) {

			//delete head element
			if (index_position == 0)
				popFront();

			//delete last element
			else if (index_position == getSize() - 1)
				popBack();

			//delete middle element
			else {

				Node* current = head;
				int counter = 0; //for counting position


				while (current != NULL) {
					if (counter == index_position - 1) {
						Node* prev_before_del = current;
						current = current->next;
						Node* the_del_tmp = current;
						current = current->next;
						prev_before_del->next = current;
						current->previous = prev_before_del;
						delete the_del_tmp;
					}

					counter++; //increased
					current = current->next; //next node
				}
			}
		}

		else throw std::invalid_argument("out_of_range");
	}


	//overload the operator
	T& operator[](const int index);
};

//overload the operator
template<typename T>
T& List<T>::operator[](const int index) {

	Node* node = this->getHead();
	int counter = 0;
	int amount = 0;

	while (node != NULL) {
		if (index == amount)
			return node->inf;

		amount++;
		node = node->next;
	}
}