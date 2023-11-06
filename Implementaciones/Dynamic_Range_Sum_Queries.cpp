#include <bits/stdc++.h>

using namespace std;
void fastIO() { ios_base::sync_with_stdio(false), cin.tie(0); }
using tint = long long;

struct Node;
using Treap = Node *;

struct Node {
  tint priority, key, value, sum;
  Treap left = nullptr, right = nullptr;

  Node(tint k, tint v) : priority{rand()}, key{k}, value{v}, sum{v} {}
};

tint sum(Treap root) { return root ? root->sum : 0; }
void evaluate(Treap root) {
  if (root == nullptr) return;
  root->sum = sum(root->left) + root->value + sum(root->right);
}

pair<Treap, Treap> split(Treap root, tint k) {
  if (root == nullptr) return {nullptr, nullptr};

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

void update(Treap &root, tint k, tint v) {
  Treap left, middle, right;
  tie(left, right) = split(root, k);
  tie(left, middle) = split(left, k - 1);

  middle->value = middle->sum = v;
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
    tint type;
    cin >> type;
    if (type == 1) {
      tint k, v;
      cin >> k >> v;
      update(root, k, v);
    } else {
      tint l, r;
      cin >> l >> r;
      cout << query(root, l, r) << '\n';
    }
  }
}