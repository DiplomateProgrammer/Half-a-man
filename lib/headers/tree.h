#ifndef MY_TREE
#define MY_TREE
namespace my
{
	struct tree 
	{
		tree *l, *r;
		char ch;
		tree(char ch) : ch(ch), l(nullptr), r(nullptr){};
		//bool went_right = 0;
		bool is_leaf() const;
		bool correct() const;
	};
	void delete_tree(tree *node);
}
#endif
