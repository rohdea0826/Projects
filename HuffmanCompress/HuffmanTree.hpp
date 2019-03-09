#pragma once
#include <vector>
#include <queue>

template<class W>
struct HuffmanTreeNode
{
	HuffmanTreeNode(const W& weight)
		: _pLeft(nullptr)
		, _pRight(nullptr)
		, _pParent(nullptr)
		, _weight(weight)
	{}
	HuffmanTreeNode<W>* _pLeft;
	HuffmanTreeNode<W>* _pRight;
	HuffmanTreeNode<W>* _pParent;
	W _weight;
};

//仿函数将优先级队列变为小堆方式
template<class W>
class Compare
{
	typedef HuffmanTreeNode<W>* PNode;
public:
	bool operator()(PNode pLeft, PNode pRight)
	{
		return pLeft->_weight > pRight->_weight;
	}
};

template<class W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W> Node;
	typedef Node* PNode;
public:
	HuffmanTree()
		:_pRoot(nullptr)
	{}
	~HuffmanTree()
	{
		_Destroy(_pRoot);
	}

	void CreathuffmanTree(const std::vector<W>& v, W invalid)
	{
		if (v.empty()) return;

		//用所给的权值创建二叉树森林并用优先级队列管理
		std::priority_queue<PNode,std::vector<PNode>,Compare<W>> q;
		for (size_t i = 0; i < v.size(); ++i)
		{
			if(v[i] != invalid)
			q.push(new Node(v[i]));
		}

		while (q.size() > 1)
		{
			PNode pLeft = q.top();
			q.pop();

			PNode pRight = q.top();
			q.pop();

			PNode pParent = new Node(pLeft->_weight + pRight->_weight);
			pLeft->_pParent = pParent;
			pRight->_pParent = pParent;
			_pRoot = pParent;
			_pRoot->_pLeft = pLeft;
			_pRoot->_pRight = pRight;
			q.push(pParent);
		}
	}

	PNode getRoot()
	{
		return _pRoot;
	}

private:
	void _Destroy(PNode& pRoot)
	{
		if (pRoot)
		{
			_Destroy(pRoot->_pLeft);
			_Destroy(pRoot->_pRight);
			delete pRoot;
			pRoot = nullptr;
		}
	}
private:
	PNode _pRoot;
};

