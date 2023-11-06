#include <bits/stdc++.h>

using namespace std;
using tint = long long;
void fastIO() { ios_base::sync_with_stdio(false), cin.tie(0); }

struct Node;
using Treap = Node *;

struct Node {
  tint priority, size = 1, val, sum;
  bool rev = false;
  Treap left = nullptr, right = nullptr;

  Node(tint v) : priority{rand()}, val{v}, sum{v} {}
};

tint size(Treap root) { return root ? root->size : 0; }
tint sum(Treap root) { return root ? root->sum : 0; }
void toggleRev(Treap root) {
  if (root) root->rev ^= true;
}

void evaluate(Treap root) {
  if (root == nullptr) return;
  root->size = size(root->left) + size(root->right) + 1;
  root->sum = sum(root->left) + sum(root->right) + root->val;
}

void propagate(Treap root) {
  if (root == nullptr) return;
  if (root->rev) {
    root->rev = false;
    swap(root->left, root->right);
    toggleRev(root->left), toggleRev(root->right);
  }
}

pair<Treap, Treap> split(Treap root, tint k) {
  if (root == nullptr) return {nullptr, nullptr};

  propagate(root);
  tint key = size(root->left) + 1;
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

Treap build(const vector<tint> &v, tint from, tint to) {
  if (from == to) return nullptr;

  tint mid = (from + to) / 2;
  Treap root = new Node(v[mid]);
  root->left = build(v, from, mid);
  root->right = build(v, mid + 1, to);

  heapify(root);
  evaluate(root);
  return root;
}

void update(Treap &root, tint l, tint r) {
  Treap left, middle, right;
  tie(left, right) = split(root, r);
  tie(left, middle) = split(left, l - 1);
  toggleRev(middle);
  root = merge(merge(left, middle), right);
}

tint query(Treap &root, tint l, tint r) {
  Treap left, middle, right;
  tie(left, right) = split(root, r);
  tie(left, middle) = split(left, l - 1);

  tint ans = sum(middle);
  root = merge(merge(left, middle), right);
  return ans;
}

int main() {
  fastIO();
  tint n, q;
  cin >> n >> q;

  vector<tint> v(n);
  for (tint i = 0; i < n; i++) cin >> v[i];

  Treap root = build(v, 0, n);
  for (tint qi = 0; qi < q; qi++) {
    tint type, l, r;
    cin >> type >> l >> r;

    if (type == 1) {
      update(root, l, r);
    } else {
      cout << query(root, l, r) << '\n';
    }
  }
}