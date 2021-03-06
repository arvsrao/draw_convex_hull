#ifndef DRAW_CONVEX_HULL_DIRECTACYCLICNODE_H
#define DRAW_CONVEX_HULL_DIRECTACYCLICNODE_H

#include <Vertex.h>

#include <array>

template <typename T = int>
class DirectedAcyclicNode {
 public:
  static const uint8_t MAX_CHILDREN;

  using DirectedAcyclicNodeRef = DirectedAcyclicNode<T>*;
  using ChildContainerType     = std::array<DirectedAcyclicNodeRef, MAX_CHILDREN>;

  T* data;
  ChildContainerType children;

  DirectedAcyclicNode(T* _data);

  void addChild(T* _data);

  T* getFace();
  bool hasFace();

  DirectedAcyclicNodeRef getChild(unsigned index);
  bool hasChildren();
  ChildContainerType getChildren();

  virtual ~DirectedAcyclicNode();
};

#include <impl/DirectedAcyclicNode.cpp>

#endif  // DRAW_CONVEX_HULL_DIRECTACYCLICNODE_H
