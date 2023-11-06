#include <bits/stdc++.h>

using namespace std;
void fastIO() { ios_base::sync_with_stdio(false), cin.tie(0); }
using tint = long long;

struct Node;
using Treap = Node *;

struct Node {
  tint priority, key, value, sum, size = 1;
  tint lazySet = 0, lazyAdd = 0;
  bool mustSet = false;
  Treap left = nullptr, right = nullptr;

  Node(tint k, tint v) : priority{rand()}, key{k}, value{v}, sum{v} {}
};

tint size(Treap root) { return root ? root->size : 0; }
tint sum(Treap root) { return root ? root->sum : 0; }

void evaluate(Treap root) {
  if (root == nullptr) return;
  root->size = size(root->left) + size(root->right) + 1;

  if (root->mustSet) {
    root->sum = size(root) * (root->lazySet + root->lazyAdd);
  } else {
    root->sum = sum(root->left) + sum(root->right) + root->value +
                size(root) * root->lazyAdd;
  }
}

void propagate(Treap root) {
  if (root == nullptr) return;
  if (not root->mustSet && root->lazyAdd == 0) return;

  if (root->mustSet) {
    for (Treap child : {root->left, root->right}) {
      if (child != nullptr) {
        child->lazySet = root->lazySet;
        child->mustSet = true;
        child->lazyAdd = 0;
      }
    }
    root->value = root->lazySet;
    root->lazySet = 0;
    root->mustSet = false;
  }

  for (Treap child : {root->left, root->right}) {
    if (child != nullptr) {
      child->lazyAdd += root->lazyAdd;
    }
  }
  root->value += root->lazyAdd;
  root->lazyAdd = 0;

  evaluate(root->left), evaluate(root->right);
}

pair<Treap, Treap> split(Treap root, tint k) {
  if (root == nullptr) return {nullptr, nullptr};

  propagate(root);
  if (root->key <= k) {
    auto [left, right] = split(root->right, k);
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

// Precondición: v está ordenado
Treap build(const vector<tint> &v, tint from, tint to) {
  if (from == to) return nullptr;

  tint mid = (from + to) / 2;
  Treap root = new Node(mid + 1, v[mid]);
  root->left = build(v, from, mid);
  root->right = build(v, mid + 1, to);

  heapify(root);
  evaluate(root);
  return root;
}

tint query(Treap &root, tint l, tint r) {
  Treap left, middle, right;
  tie(left, right) = split(root, r);
  tie(left, middle) = split(left, l - 1);

  tint ans = sum(middle);
  root = merge(merge(left, middle), right);
  return ans;
}

void rangeAdd(Treap &root, tint l, tint r, tint x) {
  Treap left, middle, right;
  tie(left, right) = split(root, r);
  tie(left, middle) = split(left, l - 1);

  middle->lazyAdd += x;
  evaluate(middle);

  root = merge(merge(left, middle), right);
}

void rangeSet(Treap &root, tint l, tint r, tint x) {
  Treap left, middle, right;
  tie(left, right) = split(root, r);
  tie(left, middle) = split(left, l - 1);

  middle->mustSet = true;
  middle->lazySet = x;
  middle->lazyAdd = 0;
  evaluate(middle);

  root = merge(merge(left, middle), right);
}

int main() {
  fastIO();
  tint n, q;
  cin >> n >> q;

  vector<tint> nums(n);
  for (tint i = 0; i < n; i++) cin >> nums[i];

  Treap root = build(nums, 0, n);
  for (tint qi = 0; qi < q; qi++) {
    tint type, l, r, x;
    cin >> type >> l >> r;
    if (type == 1) {
      cin >> x;
      rangeAdd(root, l, r, x);
    } else if (type == 2) {
      cin >> x;
      rangeSet(root, l, r, x);
    } else {
      cout << query(root, l, r) << '\n';
    }
  }
}