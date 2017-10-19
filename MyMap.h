#ifndef MYMAP_H
#define MYMAP_H

#include <queue>

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap<KeyType, ValueType>() : m_size(0), m_root(nullptr) {};
	~MyMap<KeyType, ValueType>() { clear(); };
	void clear();
	int size() const { return m_size; };

	//	The	associate	method	associates	one	item	(key)	with	another	(value).
	//	If	no	association	currently	exists	with	that	key,	this	method	inserts
	//	a	new	association	into	the	tree	with	that	key/value	pair.		If	there	is
	//	already	an	association	with	that	key	in	the	tree,	then	the	item
	//	associated	with	that	key	is	replaced	by	the	second	parameter	(value).
	//	Thus,	the	tree	contains	no	duplicate	keys.
	void associate(const KeyType& key, const ValueType& value);

	// for a map that can't be modified, return a pointer to const ValueType
	//	If	no	association	exists	with	the	given	key,	return	nullptr;	otherwise,
	//	return	a	pointer	to	the	value	associated	with	that	key.		This	pointer	can	be
	//	used	to	examine	that	value,	and	if	the	map	is	allowed	to	be	modified,	to
	//	modify	that	value	directly	within	the	map (the	second	overload	enables
	//	this).		Using	a	little	C++	magic,	we	have	implemented	it	in	terms	of	the
	//	first	overload,	which	you	must	implement.
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct Node
	{
		Node(const KeyType& key, const ValueType& value)
		{
			k = key;
			v = value;
			left = right = nullptr;
		}

		KeyType k;
		ValueType v;
		Node* left;
		Node* right;
	};

	int m_size;
	Node* m_root;

};

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear()
{
	Node* curr = m_root;

	// The tree is already empty
	if (curr == nullptr) {
		return;
	}

	// Queue for a level-order traversal
	std::queue<Node*> q;
	q.push(curr);

	while (!q.empty()) {
		Node* node = q.front();
		q.pop();

		if (node->left != nullptr) {
			q.push(node->left);
		}

		if (node->right != nullptr) {
			q.push(node->right);
		}

		delete node;
		m_size--;
	}

	m_root = nullptr;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	// Empty tree, just insert a node
	if (m_root == nullptr) {
		m_root = new Node(key, value);
		m_size++;
		return;
	}

	Node* curr = m_root;

	// Infinitely loop until you find the key(value is already associated) or a leaf node(value is not associated)
	for (;;) {
		if (key == curr->k) {
			curr->v = value;
			return;
		}

		if (key < curr->k) {
			if (curr->left != nullptr) {
				curr = curr->left;
			} else {
				curr->left = new Node(key, value);
				m_size++;
				return;
			}
		} else if (key > curr->k) {
			if (curr->right != nullptr) {
				curr = curr->right;
			} else {
				curr->right = new Node(key, value);
				m_size++;
				return;
			}
		}
	}
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const
{
	// Empty tree
	if (m_root == nullptr) {
		return nullptr;
	}

	Node* curr = m_root;

	// Infinitely loop until you find the key(association exists) or a leaf(association doesn't exist)
	for (;;) {
		if (key == curr->k) {
			return &(curr->v);
		}

		if (key < curr->k) {
			if (curr->left != nullptr) {
				curr = curr->left;
			} else {
				return nullptr;
			}
		} else if (key > curr->k) {
			if (curr->right != nullptr) {
				curr = curr->right;
			} else {
				return nullptr;
			}
		}
	}
}

#endif