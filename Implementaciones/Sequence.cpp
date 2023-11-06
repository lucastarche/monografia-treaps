#include <bits/stdc++.h>

using namespace std;
using tint = long long;
void fastIO() { ios_base::sync_with_stdio(false), cin.tie(0); }

struct Node;
using Treap = shared_ptr<Node>;

struct Node {
  tint size = 1, val, sum;
  Treap left = nullptr, right = nullptr;
  Node(tint v) : val{v}, sum{v} {}
};

tint size(Treap root) { return root ? root->size : 0; }
tint sum(Treap root) { return root ? root->sum : 0; }

void evaluate(Treap root) {
  if (root == nullptr) return;
  root->size = size(root->left) + size(root->right) + 1;
  root->sum = sum(root->left) + sum(root->right) + root->val;
}

pair<Treap, Treap> split(Treap root, int k) {
  if (root == nullptr) return {nullptr, nullptr};

  tint key = size(root->left) + 1;
  Treap left, right;
  if (key <= k) {
    left = make_shared<Node>(*root);
    tie(left->right, right) = split(root->right, k - key);
    evaluate(left);
  } else {
    right = make_shared<Node>(*root);
    tie(left, right->left) = split(root->left, k);
    evaluate(right);
  }
  return {left, right};
}

Treap merge(Treap left, Treap right) {
  if (left == nullptr) return right;
  if (right == nullptr) return left;

  Treap root;
  tint dice = rand() % (size(left) + size(right));
  if (dice < size(left)) {
    root = make_shared<Node>(*left);
    root->right = merge(left->right, right);
  } else {
    root = make_shared<Node>(*right);
    root->left = merge(left, right->left);
  }
  evaluate(root);
  return root;
}

Treap build(const vector<tint> &v, int from, int to) {
  if (from == to) return nullptr;

  int mid = (from + to) / 2;
  Treap root = make_shared<Node>(v[mid]);
  root->left = build(v, from, mid);
  root->right = build(v, mid + 1, to);

  evaluate(root);
  return root;
}

int main() {
  fastIO();

  tint n, q;
  cin >> n >> q;

  vector<tint> nums(n);
  for (int i = 0; i < n; i++) cin >> nums[i];

  Treap orig = build(nums, 0, n);
  Treap root = orig;

  for (int qi = 0; qi < q; qi++) {
    tint type, l, r;
    cin >> type >> l >> r;

    if (type == 1) {
      Treap left, middle, right;
      tie(left, right) = split(root, r);
      tie(left, middle) = split(left, l - 1);
      cout << sum(middle) << '\n';
    } else if (type == 2) {
      tint k;
      cin >> k;
      tint len = r - l + 1;

      Treap toCopy = split(split(root, l - 1).first, l - k - 1).second;
      while (size(toCopy) < len) toCopy = merge(toCopy, toCopy);
      toCopy = split(toCopy, len).first;

      Treap left, right;
      tie(left, right) = split(root, r);
      left = split(root, l - 1).first;
      root = merge(merge(left, toCopy), right);
    } else {
      Treap toCopy = split(split(orig, r).first, l - 1).second;
      auto [left, right] = split(root, r);
      left = split(left, l - 1).first;
      root = merge(merge(left, toCopy), right);
    }
  }
}