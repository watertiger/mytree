/**
 * @file redblacktree.c
 * @brief realize red black tree's basic oprations.
 * @author watertiger <darkwkt@gmail.com>
 * @date 2014-06-06
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "redblacktree.h"

#define INIT_SIZE 10

/**
 * @brief rbt_search1 - searching key in red black tree.
 * @param proot pointer to the root of red black tree.
 * @param key the enum to search.
 * @return prbt pointer to the node of reb black tree if found,
 *	   NULL pointer if key not found.
 * 
 * 在红黑树b中查找x的过程为：
 *	 1.若b是空树，则搜索失败，否则：
 *	 2.若x等于b的根节点的数据域之值，则查找成功；否则：
 *	 3.若x小于b的根节点的数据域之值，则搜索左子树；否则：
 *	 4.查找右子树。 
 *
 */
prbt
rbt_search1( prbt proot, int key )
{
	if ( !proot || ( key == proot->data) )
		return proot;
	if ( key < proot->data )
		return rbt_search1( proot->lc, key );
	else 
		return rbt_search1( proot->rc, key );
}

/**
 * @brief rbt_search2 - searching key in red black tree.
 * @param proot pointer to the root of red black tree.
 * @param key the enum to search.
 * @param p pointer to the pointer to the red black tree node if found,
 *	    otherwise pointer to the last comparison node.
 * @return 1 for succeed,
 *	   0 for failure.
 *
 * the pointer p must be NULL when first called, it's used for returning the
 * pointer needed or last compared.
 *
 * 在红黑树b中查找x的过程为：
 *	 1.若b是空树，则搜索失败，否则：
 *	 2.若x等于b的根节点的数据域之值，则查找成功；否则：
 *	 3.若x小于b的根节点的数据域之值，则搜索左子树；否则：
 *	 4.查找右子树。 
 */
int
rbt_search2( prbt proot, int key, prbt *p )
{
	if ( !proot ) 
		return 0;

	*p = proot;
	if ( key == proot->data )
		return 1;
	if ( key < proot-> data )
		return rbt_search2( proot->lc, key, p );
	else 
		return rbt_search2( proot->rc, key, p );
}

/**
 * @brief right_rotate - rotate the red black tree to the right.
 * @param proot pointer to the pointer to the root of red black tree.
 * @param pn pointer to the root of red black subtree.
 * @return none.
 * 
 */
void
right_rotate( prbt *proot, prbt pn )
{
	prbt plc = NULL;

	plc = pn->lc;
	pn->lc = plc->rc;
	if ( plc->rc )	/* 原始为 plc->rc != 哨兵节点指针，现在为不空即可*/
		plc->rc->p = pn;
	plc->p = pn->p;
	if ( !pn->p )
		(*proot) = plc;
	else if ( pn == pn->p->rc )
		pn->p->rc = plc;
	else
		pn->p->lc = plc;
	plc->rc = pn;
	pn->p = plc;

	return;
}

/**
 * @brief left_rotate - rotate the red black tree to the left.
 * @param proot pointer to the pointer to the root of red black tree.
 * @param p pointer to the pointer to the root of red black subtree.
 * @return none.
 * 
 */
void
left_rotate( prbt *proot, prbt pn )
{
	prbt prc = NULL;

	prc = pn->rc;
	pn->rc = prc->lc;
	if ( prc->lc )
		prc->lc->p = pn;
	prc->p = pn->p;
	if ( !pn->p )
		(*proot) = prc;
	else if ( pn == pn->p->lc )
		pn->p->lc = prc;
	else
		pn->p->rc = prc;
	prc->lc = pn;
	pn->p = prc;

	return;
}

/**
 * @brief rbt_insert_fixup - adjust red black tree to keep 5 natures
 * @param proot pointer to the pointer to the root of red black tree.
 * @param pe pointer to the node inserted
 * @return none.
 *
 * 插入修复通过  重新着色 和 左右旋转 完成。
 * 插入修复有以下几种情况：
 * 	1、插入的是根节点，仅仅违反性质2，将节点颜色置为黑色(预处理)
 * 	2、插入节点不是根节点：
 * 	a）插入节点父节点为黑色，什么也不做；
 * 	b）插入节点（当前节点）的父节点为红色，叔叔节点也为红色
 * 	   将当前节点的父节点和叔叔节点涂黑，祖父节点涂红，把当前节点
 * 	   指向祖父节点，重新开始算法（连锁产生c和d）
 * 	c）插入节点的父节点为红色，叔叔节点为黑色，当前节点为父节点右子
 * 	   当前节点的父节点作为新的当前节点，以新当前节点左旋（产生d）
 * 	d）插入节点父节点是红色，叔叔节点是黑色，当前节点是父节点左子
 * 	   父节点为黑色，祖父节变红色，以祖父节点为支点右旋。（完成）
 * 
 * 注意：去掉了哨兵节点，需要处理很多NULL值，需要在操作过程中，添加更多
 * 的判断 
 */
void
rbt_insert_fixup( prbt *proot, prbt pe )
{
	prbt pu = NULL; /* pe节点的叔叔节点 */

	if ( !pe->p )	/* 插入的是根节点 */
	       return;	
	for ( ; pe->p && RED == pe->p->rb; ) {
		if ( !pe->p->p )
			break;
		if ( pe->p == pe->p->p->lc ) {
			pu = pe->p->p->rc;
			if ( pu && RED == pu->rb ) { /* 检查pu是否存在，case1: b) */
				printf("Left -- case1.\n");
				pe->p->rb = BLACK;
				pu->rb = BLACK;
				pe->p->p->rb = RED;
				pe = pe->p->p;
				continue; /* 此处continue的作用是由于 case1(left)可能转化为 case2(right)、case3(right) */
			} else if ( pe == pe->p->rc ) { /* case2: c) */
				printf("Left -- case2.\n");
				pe = pe->p;
				left_rotate( proot, pe );
			}  
			if ( !pe->p || !pe->p->p )
				break;
			if ( pe == pe->p->lc ) { 	/* case3: d) */
				printf("Left -- case3.\n");
				pe->p->rb = BLACK;
				pe->p->p->rb = RED;
				right_rotate( proot, pe->p->p );
			}	
		} else {
			pu = pe->p->p->lc;
			if ( pu && RED == pu->rb ) { /* 检查pu是否存在，case1: b) */
				printf("Right -- case1.\n");
				pe->p->rb = BLACK;
				pu->rb = BLACK;
				pe->p->p->rb = RED;
				pe = pe->p->p;
				continue; /* 此处continue的作用是由于 case1(right)可能转化为case2(left)、case3(left) */
			} else if ( pe == pe->p->lc ) { /* case2: c) */
				printf("Right -- case2.\n");
				pe = pe->p;
				right_rotate( proot, pe );
			} 
			if ( !pe->p || !pe->p->p )
				break;
			if ( pe == pe->p->rc ) {	/* case3: d) */
				printf("Right -- case3.\n");
				pe->p->rb = BLACK;
				pe->p->p->rb = RED;
				left_rotate( proot, pe->p->p );
			}
		}
	}
	(*proot)->rb = BLACK;	

	return;
}
/**
 * @brief rbt_insert - insert an enum into red black tree.
 * @param proot pointer to the pointer to the root of red black tree.
 * @param e the enum to insert.
 * @return 1 for succeed,
 *	   0 for failure.
 * 
 */
int 
rbt_insert( prbt *proot, int e )
{
	prbt parent = NULL;
	prbt pe = NULL;

	if ( rbt_search2( *proot, e, &parent ) ) {
		return 0;
	} else {
		pe = ( prbt )malloc( sizeof( rbt_node ) );
		if ( !pe ) {
			printf("No Memory!!\n");
			return 0;
		}
		pe->data = e;
		pe->lc = NULL;
		pe->rc = NULL;
		pe->rb = RED;
		pe->p = parent;
		if ( !parent ) {
			(*proot) = pe;
			(*proot)->rb = BLACK; //pe->rb = BLACK;
		} else if ( e < parent->data ) {
			parent->lc = pe;
		} else {
			parent->rc = pe;
		}
	}
	rbt_insert_fixup( proot, pe ); 

	return 1;
}


/**
 * @brief tree_minmum - find subtree's minmum node.
 * @param pn pointer to current red black subtree's root node
 * @return pointer to the minmum node if found,
 * 	   otherwise return NULL
 *
 */
prbt
tree_minmum( prbt pn )
{
	prbt ps = pn;
	if ( !pn->p ){
		printf("Only have root node!!\n");
		return NULL;
	}
	for ( ; ps->lc; ) 
		ps = ps->lc;
	
	return ps;
}

/**
 * @brief tree_successor - find node's successor node.
 * @param pn pointer to current red black tree node
 * @return pointer to the successor node if found,
 * 	   otherwise return NULL
 *
 */
prbt
tree_successor( prbt pn )
{
	prbt ps = NULL;

	if ( pn->rc ) {
		/* return tree_minimum( pn->rc );*/
		ps = pn->rc;
		for ( ; ps->lc; ) {
			ps = ps->lc;
		}
	} else {
		ps = pn->p;
		for ( ; ps && ( pn == ps->rc ); ) {
			pn = ps;
			ps = ps->p;
		}
	}

	return ps;
}

/**
 * @brief rbt_transplant - link node u's child to u's parent node's child. 
 * @param proot pointer to the pointer to the root of red black tree.
 * @param pn pointer to current node u.
 * @param pc pointer to the u's child (left or right child).
 * @return none.
 *
 */
void
rbt_transplant( prbt *proot, prbt pn, prbt pc )
{
	if ( !pn->p )
		*proot = pc;
	else if ( pn == pn->p->lc )
		pn->p->lc = pc;
	else
		pn->p->rc = pc;
	if ( pc )
		pc->p = pn->p;
	
	return;
}

/**
 * @brief rbt_delete_fixup - adjust red black tree to keep 5 natures
 * @param proot pointer to the pointer to the root of red black tree.
 * @param pe pointer to the pointer to the node replacing deleted-node.
 * @param pep pointer to node pe's parent.
 * 	      if pe == NULL, pep exists and != NULL,
 * 	      otherwise pep == NULL.
 * @return none.
 *
 * 删除修复通过  重新着色 和 左右旋转 完成。
 * 删除修复有以下几种情况：
 * 	1、如果被删除的节点是红色，什么也不做。(被删节点被当前的pe替代)
 * 	2、如果被删除的节点是黑色，则需要调整：
 * 	a) 节点pe的兄弟节点w是红色
 * 	   交换pe双亲节点和w节点的颜色，以pe的双亲执行一次左旋。可能转化为
 * 	   b、c、d，转化为b则循环终止
 * 	b) 节点pe的兄弟节点w是黑色，且w的两个孩子都是黑色
 * 	   将节点w置为红色，将pe指向pe的双亲节点，重复循环（即将额外的黑色
 * 	   沿树根上升）
 * 	c) 节点pe的兄弟节点w是黑色，且w的左孩子是红色，w的右孩子是黑色
 * 	   交换w和w的左孩子的节点颜色，并以w执行一次右旋，转化为d。
 * 	d) 节点pe的兄弟节点w是黑色，且w的右孩子为红色，左孩子或黑或红
 * 	   通过改变某些节点的颜色，以pe的双亲节点执行一次左旋，可以将x表示
 * 	   的额外黑色去掉，循环终止。
 * 
 * 注意：pep指针仅在pe==NULL时起作用，目的是为了保证循环的统一性，方便操作。
 */
void
rbt_delete_fixup( prbt *proot, prbt pe, prbt pep )
{
	prbt px = NULL;
	prbt pw = NULL; /* 指向节点pe的兄弟节点指针 */

	printf("root:0x%p, node:0x%p, node-p:0x%p.\n", (*proot), pe, pep);
	if ( !(*proot) ) /* 删除的是根节点，且为最后一个节点 */
		return;
	if ( !pe ) {
		px = ( prbt )malloc( sizeof(rbt_node) );
		if ( !px ) {
			printf("No Memory!!\n");
			return;
		}
		px->rb = BLACK;
		px->p = pep;
		px->lc = NULL;
		px->rc = NULL;
		pe = px;
		if ( !pep->lc )
			pep->lc = px;
		else
			pep->rc = px;
	}

	for ( ; (pe != (*proot)) && (BLACK == pe->rb); ) {
		if ( pe == pe->p->lc ) {
			pw = pe->p->rc;
			if ( !pw ) {	
				printf("Left -- Error happened.\n");
				return;
			}
			if ( RED == pw->rb ) {	/* case1: a) */
				printf("Left -- case1.\n");
				pw->rb = BLACK;
				pe->p->rb = RED;
				left_rotate( proot, pe->p );
				pw = pe->p->rc;
			}
			if ( (!pw->lc || BLACK==pw->lc->rb) \
				&& (!pw->rc || BLACK==pw->rc->rb) ) { /* case2: b) */
				printf("Left -- case2.\n");
				pw->rb = RED;
				pe = pe->p;
			} else if ( !pw->rc || BLACK == pw->rc->rb ) { /* case3: c) */
				printf("Left -- case3.\n");
				pw->lc->rb = BLACK;
				pw->rb = RED;
				right_rotate( proot, pw ); 
				pw = pe->p->rc;

			}
			if ( BLACK == pw->rb &&  pw->rc && RED == pw->rc->rb) { /* case4: d) */
				printf("Left -- case4.\n");
				pw->rb = pe->p->rb;
				pe->p->rb = BLACK;
				pw->rc->rb = BLACK;
				left_rotate( proot, pe->p );

				pe = *proot; /* 终止循环 */
			}
		} else {
			pw = pe->p->lc;
			if ( !pw ) {	
				printf("Right -- Error happened.\n");
				return;
			}
			if ( RED == pw->rb ) { /* case1: a) */
				printf("Right -- case1.\n");
				pw->rb = BLACK;
				pe->p->rb = RED;
				right_rotate( proot, pe->p );
				pw = pe->p->lc;
			}
			if ( (!pw->lc || BLACK==pw->lc->rb) \
				&& (!pw->rc || BLACK==pw->rc->rb) ) { /* case2: b) */
				printf("Right -- case2.\n");
				pw->rb = RED;
				pe = pe->p;
			} else if ( !pw->lc || BLACK == pw->lc->rb ) { /* case3: c) */
				printf("Right -- case3.\n");
				pw->rc->rb = BLACK;
				pw->rb = RED;
				left_rotate( proot, pw ); 
				pw = pe->p->lc;
			}
			if ( BLACK == pw->rb && pw->lc && RED == pw->lc->rb ) { /* case4: d) */
				printf("Right -- case4.\n");
				pw->rb = pe->p->rb;
				pe->p->rb = BLACK;
				pw->lc->rb = BLACK;
				right_rotate( proot, pe->p );

				pe = *proot; /* 终止循环 */
			}
		}
	}

	pe->rb = BLACK;
	if ( px ) {
		if ( px == px->p->lc )
			px->p->lc = NULL;
		else
			px->p->rc = NULL;
		free(px);
	}
	return;
}
/**
 * @brief rbt_delete - delete an enum from red black tree.
 * @param proot pointer to the pointer to the root of red black tree.
 * @param e the enum to delete.
 * @return 1 for succeed,
 *	   0 for failure.
 * 
 */
int 
rbt_delete( prbt *proot, int e )
{
	prbt pn = NULL;
	prbt ps = NULL; /* 待删节点的后继节点 */
	prbt pc = NULL; /* 待删节点的孩子节点 */
	prbt pcp = NULL; /* 若pc为叶子节点NULL，将pc的双亲节点指针赋给pcp */
	int ps_rb = 0;
	
	if ( ! rbt_search2( *proot, e, &pn ) )
		return 0;
	ps = pn;
	ps_rb = ps->rb;
	if ( !pn->lc ) {
		pc = pn->rc;	/* 此处pc可能为NULL */
		rbt_transplant( proot, pn, pn->rc);
		if ( !pc ) 
			pcp = pn->p;
	} else if ( !pn->rc ) {
		pc = pn->lc;
		rbt_transplant( proot, pn, pn->lc);
	} else {
		ps = tree_minmum( pn->rc );
		ps_rb = ps->rb;
		pc = ps->rc;
		if ( ps->p == pn ) {
			if ( pc )
				pc->p = ps;
			else
				pcp = ps;
		} else {
			rbt_transplant( proot, ps, ps->rc );
			if ( !pc )
				pcp = ps->p;
			ps->rc = pn->rc;
			ps->rc->p = ps;
		}
		rbt_transplant( proot, pn, ps );
		ps->lc = pn->lc;
		ps->lc->p = ps;
		ps->rb = pn->rb;
	}
	if ( BLACK == ps_rb ) {
		rbt_delete_fixup( proot, pc, pcp );
	}

	free( pn );
	
	return 1;
}

/**
 * @brief rbt_show - show all node's information of red black tree.
 * @param proot pointer to the node of red black tree.
 * @return none.
 *
 */
void
rbt_show( prbt proot )
{
	if ( proot ) {
		printf("Current:%-4d color:%s. ", proot->data, proot->rb ? "BLACK" : "RED");
		if ( proot->lc )
			printf("LC:%-4d", proot->lc->data);
		else
			printf("LC:$   ");
		if ( proot->rc )
			printf("RC:%-4d", proot->rc->data);
		else
			printf("RC:$   ");
		if ( proot->p )
			printf("P:%-4d\n", proot->p->data);
		else
			printf("P:$   \n");
		rbt_show( proot->lc);
		rbt_show( proot->rc);
	} 

	return;
}


int
main()
{
	int array[INIT_SIZE];
	//int array1[INIT_SIZE] = {4,17,54,61,11,0,51,75,43,88};
	int array1[INIT_SIZE] = {87,10,68,69,6,27,24,28,70,62};
	int i = 0;
	prbt proot = NULL; /* 此处一定要显式置NULL，否则调用会出错，因为是根 */
	prbt proot1 = NULL; /* 此处一定要显式置NULL，否则调用会出错，因为是根 */
	prbt pfind = NULL;

	srand( (unsigned int)time(NULL) );
	for ( i = 0; i < INIT_SIZE; i++ ) {
		array[i] = rand() % 100;
		printf("%-4d", array[i]);
	}
	printf("\n");
	/* 创建红黑树 */
	for ( i = 0; i < INIT_SIZE; i++ ) {
		rbt_insert( &proot, array[i] );
//		if ( proot ) {
//			rbt_show( proot );
//			printf("\n");
//		}
	}
	rbt_show( proot );
	printf("\n");
	for ( i = 0; i < INIT_SIZE; i++ ) {
		printf("%d -- \n", array[i]);
		rbt_delete( &proot, array[i]);
		rbt_show( proot );
		printf("\n");
	}
	
	printf("======================分割线======================\n");
	for ( i = 0; i < INIT_SIZE; i++ ) {
		printf("%d -- \n", array1[i]);
		rbt_insert( &proot1, array1[i] );
		rbt_show( proot1 );
		printf("\n");
	}
	printf("======================分割线1--insert finish======================\n");
	rbt_show( proot1 );
	printf("\n");
	for ( i = 0; i < INIT_SIZE; i++ ) {
		printf("%d -- \n", array1[i]);
		rbt_delete( &proot1, array1[i]);
		rbt_show( proot1 );
		printf("\n");
	}

	return 0;
}


