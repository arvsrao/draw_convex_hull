#ifndef DRAW_CONVEX_HULL_DIRECTACYCLICNODE_H
#define DRAW_CONVEX_HULL_DIRECTACYCLICNODE_H

#include <vector>

template <typename T = int>
class DirectedAcyclicNode {
 public:
  using NodeChildrenType = std::vector<DirectedAcyclicNode<T>*>;

  T data;
  NodeChildrenType children;

  DirectedAcyclicNode(T _data);

  void addChild(T _data);
  T getData();
  DirectedAcyclicNode* getChild(unsigned index);
  NodeChildrenType getChildren();

  virtual ~DirectedAcyclicNode();
};

#include <impl/DirectedAcyclicNode.cpp>

#endif  // DRAW_CONVEX_HULL_DIRECTACYCLICNODE_H
