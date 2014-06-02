/**
 * @file bsearchtree.h
 * @brief describe binary search tree's defination and basic oprations.
 * @author watertiger <darkwkt@gmail.com>
 * @date 2014-05-31
 */
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief define the binary search tree node and it's basic oprations
 *
 * 二叉排序树又称“二叉查找树”、“二叉搜索树”。
 * 二叉排序树：或者是一棵空树，或者是具有下列性质的二叉树：
 * 	1. 若它的左子树不空，则左子树上所有结点的值均小于它的根结点的值；
 *	2. 若它的右子树不空，则右子树上所有结点的值均大于它的根结点的值；
 *	3. 它的左、右子树也分别为二叉排序树。
 *  
 */
typedef struct _binary_search_tree {
	int data;
	struct _binary_search_tree *lc, *rc;/* left and right child pointer */
}bst_node, *pbst;

pbst bst_search1( pbst proot, int key );
int bst_search2( pbst proot, int key, pbst *p);
int bst_insert( pbst *proot, int e );
int bst_delete( pbst *proot, int key);
