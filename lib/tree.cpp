#include "headers/tree.h";
using namespace my;
using my::tree;
bool tree::is_leaf() const
{
	return (!l && !r);
}
void my::delete_tree(tree *node)
{
	if (!node) return;
	delete_tree(node->l);
	delete_tree(node->r);
	delete node;
}