
#include <DirectedAcyclicNode.h>

template <typename T>
const uint8_t DirectedAcyclicNode<T>::MAX_CHILDREN = 3;

template <typename T>
DirectedAcyclicNode<T>::DirectedAcyclicNode(T* _data) : data(_data) {
  children = {nullptr, nullptr, nullptr};
}

template <typename T>
void DirectedAcyclicNode<T>::addChild(T* _data) {
  children.push_back(new DirectedAcyclicNode<T>(_data));
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
  for (auto& child : children) {
    if (child != nullptr) return true;
  }
  return false;
}

template <typename T>
std::array<DirectedAcyclicNode<T>*, DirectedAcyclicNode<T>::MAX_CHILDREN>
DirectedAcyclicNode<T>::getChildren() {
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
