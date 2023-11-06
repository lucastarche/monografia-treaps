#include <bits/stdc++.h>

using namespace std;
void fastIO() { ios_base::sync_with_stdio(false), cin.tie(0); }

struct Node;
using Treap = Node *;

struct Node {
  int priority, value, size = 1;
  Treap left = nullptr, right = nullptr;
  Node(int v) : priority{rand()}, value{v} {}
};

int size(Treap root) { return root ? root->size : 0; }
void evaluate(Treap root) {
  if (root == nullptr) return;
  root->size = size(root->left) + size(root->right) + 1;
}

pair<Treap, Treap> split(Treap root, int k) {
  if (root == nullptr) return {nullptr, nullptr};

  if (root->value <= k) {
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

void insert(Treap &root, int x) {
  auto [left, right] = split(root, x);
  root = merge(merge(left, new Node(x)), right);
}

void remove(Treap &root, int x) {
  Treap left, middle, right;
  tie(left, right) = split(root, x);
  tie(left, middle) = split(left, x - 1);

  // middle es cero o un elementos, no perdemos memoria
  if (middle) delete middle;
  root = merge(left, right);
}

// Retorna la cantidad de elementos <= x
int count(Treap &root, int x) {
  auto [left, right] = split(root, x);
  int ans = size(left);
  root = merge(left, right);
  return ans;
}

int main() {
  fastIO();
  int n, q;
  cin >> n >> q;

  vector<int> nums(n);
  for (int i = 0; i < n; i++) cin >> nums[i];

  map<int, Treap> valToTreap;
  for (int i = 0; i < n; i++) {
    insert(valToTreap[nums[i]], i);
  }

  for (int qi = 0; qi < q; qi++) {
    int k, x;
    cin >> k >> x;

    int ans = count(valToTreap[x], k - 1);
    remove(valToTreap[nums[k]], k);
    insert(valToTreap[x], k);
    nums[k] = x;

    cout << ans << '\n';
  }
}