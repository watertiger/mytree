/**
 * @file redblacktree.h
 * @brief describe red black tree's defination and basic oprations.
 * @author watertiger <darkwkt@gmail.com>
 * @date 2014-06-05
 */
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief define the red black tree node and it's basic oprations
 *
 * 红黑树是一种自平衡二叉查找树，又称之为对称二叉B树。
 * 红黑树是每个节点都带有颜色属性的二叉查找树，颜色为红色或黑色。
 * 在二叉查找树强制一般要求以外，对于任何有效的红黑树我们增加了
 * 如下的额外要求:
 * 	性质1. 节点是红色或黑色。
 * 	性质2. 根是黑色。
 * 	性质3. 所有叶子都是黑色（叶子是NIL节点）。
 * 	性质4. 每个红色节点的两个子节点都是黑色。(从每个叶子到根的
 *		所有路径上不能有两个连续的红色节点。)
 *	性质5. 从任一节点到其每个叶子的所有简单路径都包含相同数目的黑色节点。
 *
 */
#define RED 0
#define BLACK 1

typedef struct _red_black_tree {
	int data;
	int rb;
	struct _red_black_tree *lc, *rc, *p;/* left and right child pointer, and parent pointer */
}rbt_node, *prbt;

prbt rbt_search1( prbt proot, int key );
int rbt_search2( prbt proot, int key, prbt *p );
int rbt_insert( prbt *proot, int e );
int rbt_delete( prbt *proot, int key );
int rbt_destroy( prbt *proot );
void rbt_show( prbt proot );

