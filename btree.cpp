#include <string.h>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#define MAX_KEY 4	//B树节点的最大度数

using std::cout;
using std::endl;

struct ST_BTreeNode
{
	int key_num;								///< 当前存储的数据个数
	bool is_leaf;								///< 是否是(伪)叶节点[真正的叶子节点是空指针]
	int keys[MAX_KEY-1];							///< 键值数组
	ST_BTreeNode *key_pointers[MAX_KEY];		///< 子节点指针数组
	ST_BTreeNode() {
		memset( this, 0, sizeof( ST_BTreeNode ) );
	}
};

std::pair<ST_BTreeNode *, int> search( ST_BTreeNode *root_ptr, int key )
{
	int i = 0;
	std::pair<ST_BTreeNode *, int> result;

	while(( root_ptr->keys[i] < key ) && ( i < root_ptr->key_num ) )//找到节点中数据所在位置
		i++;

	if( root_ptr->keys[i] == key )	//数据找到
		return std::make_pair( root_ptr, i );

	if( root_ptr->is_leaf )	//已经搜索到叶节点
		return std::make_pair(( ST_BTreeNode * )NULL, -1 );
	else	//非叶节点，继续搜索子树节点
		return search( root_ptr->key_pointers[i], key );
}

void split( ST_BTreeNode *parent_ptr, ST_BTreeNode *child_ptr, int child_pos )
{
	ST_BTreeNode *new_node_ptr = new ST_BTreeNode;
	new_node_ptr->is_leaf = child_ptr->is_leaf;
	new_node_ptr->key_num = MAX_KEY / 2 - 1;

	//拷贝节点数据
	for( int i = 0; i < MAX_KEY / 2 - 1; i++ )
		new_node_ptr->keys[i] = child_ptr->keys[i + MAX_KEY / 2];

	//非叶节点，则拷贝指针
	if( !child_ptr->is_leaf )
		for( int i = 0; i < MAX_KEY / 2; i++ )
			new_node_ptr->key_pointers[i] = child_ptr->key_pointers[i + MAX_KEY / 2];
	child_ptr->key_num = MAX_KEY / 2 - 1;

	//将中位数作为索引插入到双亲节点中
	int i;
	for( i = parent_ptr->key_num; i > child_pos; i-- )
	{
		parent_ptr->keys[i + 1] = parent_ptr->keys[i];
		parent_ptr->key_pointers[i + 1] = parent_ptr->key_pointers[i];
	}
	parent_ptr->keys[i] = child_ptr->keys[MAX_KEY / 2 - 1];
	parent_ptr->key_num++;
	parent_ptr->key_pointers[i + 1] = new_node_ptr;
}

void insert_simple( ST_BTreeNode *node_ptr, int key )
{
	int i = node_ptr->key_num;

	if( node_ptr->is_leaf )
	{
		//向后移动
		while(( i > 0 ) && ( node_ptr->keys[i - 1] > key ) )
		{
			node_ptr->keys[i] = node_ptr->keys[i - 1];
			i--;
		}
		//插入
		node_ptr->keys[i] = key;
		node_ptr->key_num++;
	}
	else
	{
		while(( i > 0 ) && ( node_ptr->keys[i - 1] > key ) )
			i--;

		ST_BTreeNode *child_ptr = node_ptr->key_pointers[i];

		if( child_ptr->key_num == MAX_KEY - 1 )
		{
			split( node_ptr, child_ptr, i );
			if( key > node_ptr->keys[i] )
				i++;
		}

		child_ptr = node_ptr->key_pointers[i];
		insert_simple( child_ptr, key );
	}
}

ST_BTreeNode *insert( ST_BTreeNode *root_ptr, int key )
{
	if( root_ptr->key_num == MAX_KEY - 1 )
	{
		ST_BTreeNode *new_node_ptr = new ST_BTreeNode;
		new_node_ptr->key_pointers[0] = root_ptr;
		split( new_node_ptr, root_ptr, 0 );
		insert_simple( new_node_ptr, key );

		return new_node_ptr;
	}
	else
	{
		insert_simple( root_ptr, key );
		return root_ptr;
	}
}

void breadth_first_traversal( ST_BTreeNode *root_ptr )
{
	int queue_num = 0;
	ST_BTreeNode *queue[20], *current;

	queue[queue_num] = root_ptr;
	queue_num++;

	while( queue_num > 0 )
	{
		current = queue[0];
		queue_num--;
		for( int i = 0; i < queue_num; i++ )
			queue[i] = queue[i+1];
		cout << "{ ";
		for( int i = 0; i < current->key_num; i++ )
			cout << current->keys[i] << " ";
		cout << "}  ";
		if( current != NULL && current->is_leaf != 1 ) {
			for( int i = 0; i <= current->key_num; i++ ) {
				queue[queue_num] = current->key_pointers[i];
				queue_num++;
			}
		}
	}
	cout<<endl;
}

int main()
{
	ST_BTreeNode *root = new ST_BTreeNode;
	root->is_leaf = true;

#define MAX_VALUE 100
#define ARRAY_LENGTH 15
	int array[ARRAY_LENGTH];
	srand(( unsigned )time( NULL ) );
	for( int i = 0; i < ARRAY_LENGTH; i++ )
	{
		array[i] = rand() % MAX_VALUE;
		root = insert( root, array[i] );
		breadth_first_traversal( root );
	}

	return 0;
}

