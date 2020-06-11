
template <typename T>
DirectedAcyclicNode<T>::DirectedAcyclicNode(T _data) : data(_data) {
  children = {};
}

template <typename T>
void DirectedAcyclicNode<T>::addChild(T _data) {
  children.push_back(new DirectedAcyclicNode<T>(_data));
}

template <typename T>
DirectedAcyclicNode<T>* DirectedAcyclicNode<T>::getChild(unsigned index) {
  return children.at(index);
}

template <typename T>
T DirectedAcyclicNode<T>::getData() {
  return data;
}

template <typename T>
std::vector<DirectedAcyclicNode<T>*> DirectedAcyclicNode<T>::getChildren() {
  return children;
}

template <typename T>
DirectedAcyclicNode<T>::~DirectedAcyclicNode() {
  for (auto& child : children) delete child;
}
