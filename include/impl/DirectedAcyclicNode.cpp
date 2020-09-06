
#include <DirectedAcyclicNode.h>

template <typename T>
const uint8_t DirectedAcyclicNode<T>::MAX_CHILDREN = 3;

template <typename T>
DirectedAcyclicNode<T>::DirectedAcyclicNode(DataRef data) : data(data), numChildren(0) {
  children = {nullptr, nullptr, nullptr};
}

template <typename T>
void DirectedAcyclicNode<T>::addChild(DataRef data) {
  if (numChildren < MAX_CHILDREN) {
    children[numChildren] = new DirectedAcyclicNode<T>(data);
    numChildren++;
  }
}

template <typename T>
DirectedAcyclicNode<T>* DirectedAcyclicNode<T>::getChild(unsigned index) {
  return children.at(index % MAX_CHILDREN);
}

template <typename T>
T* DirectedAcyclicNode<T>::getFace() {
  return data;
}

template <typename T>
bool DirectedAcyclicNode<T>::hasChildren() {
  return numChildren > 0;
}

template <typename T>
typename DirectedAcyclicNode<T>::ChildContainerType DirectedAcyclicNode<T>::getChildren() {
  return children;
}

template <typename T>
DirectedAcyclicNode<T>::~DirectedAcyclicNode() {
  for (auto& child : children) delete child;
}

template <typename T>
bool DirectedAcyclicNode<T>::hasFace() {
  return data != nullptr;
}
