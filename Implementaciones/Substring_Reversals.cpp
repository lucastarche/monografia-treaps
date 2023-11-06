#include <bits/stdc++.h>

using namespace std;
void fastIO() { ios_base::sync_with_stdio(false), cin.tie(0); }

struct Node;
using Treap = Node *;

struct Node {
  int priority, size = 1;
  char c;
  bool rev = false;
  Treap left = nullptr, right = nullptr;

  Node(char c) : priority{rand()}, c{c} {}
};

int size(Treap root) { return root ? root->size : 0; }
void toggleRev(Treap root) {
  if (root) root->rev ^= true;
}

void evaluate(Treap root) {
  if (root == nullptr) return;
  root->size = size(root->left) + size(root->right) + 1;
}

void propagate(Treap root) {
  if (root == nullptr) return;
  if (root->rev) {
    root->rev = false;
    swap(root->left, root->right);
    toggleRev(root->left), toggleRev(root->right);
  }
}

pair<Treap, Treap> split(Treap root, int k) {
  if (root == nullptr) return {nullptr, nullptr};

  propagate(root);
  int key = size(root->left) + 1;
  if (key <= k) {
    auto [left, right] = split(root->right, k - key);
    root->right = left;
    evaluate(root);
    return {root, right};
  } else {
    auto [left, right] = split(root->left, k);
    root->left = right;
    evaluate(root);
    return {left, root};
  }
}

Treap merge(Treap left, Treap right) {
  if (left == nullptr) return right;
  if (right == nullptr) return left;

  propagate(left), propagate(right);
  if (left->priority > right->priority) {
    left->right = merge(left->right, right);
    evaluate(left);
    return left;
  } else {
    right->left = merge(left, right->left);
    evaluate(right);
    return right;
  }
}

void heapify(Treap root) {
  if (root == nullptr) return;

  Treap maxPriority = root;
  if (root->left != nullptr && root->left->priority > maxPriority->priority)
    maxPriority = root->left;
  if (root->right != nullptr && root->right->priority > maxPriority->priority)
    maxPriority = root->right;

  if (maxPriority != root) {
    swap(maxPriority->priority, root->priority);
    heapify(maxPriority);
  }
}

Treap build(const string &s, int from, int to) {
  if (from == to) return nullptr;

  int mid = (from + to) / 2;
  Treap root = new Node(s[mid]);
  root->left = build(s, from, mid);
  root->right = build(s, mid + 1, to);

  heapify(root);
  evaluate(root);
  return root;
}

void update(Treap &root, int l, int r) {
  Treap left, middle, right;
  tie(left, right) = split(root, r);
  tie(left, middle) = split(left, l - 1);
  toggleRev(middle);
  root = merge(merge(left, middle), right);
}

void buildString(Treap root, string &s) {
  if (root == nullptr) return;
  propagate(root);
  buildString(root->left, s);
  s.push_back(root->c);
  buildString(root->right, s);
}

int main() {
  fastIO();
  int n, q;
  cin >> n >> q;

  string s;
  cin >> s;

  Treap root = build(s, 0, n);
  for (int qi = 0; qi < q; qi++) {
    int l, r;
    cin >> l >> r;
    update(root, l, r);
  }

  s.clear();
  buildString(root, s);
  cout << s << '\n';
}