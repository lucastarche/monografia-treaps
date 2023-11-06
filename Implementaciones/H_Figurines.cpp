#include <bits/stdc++.h>

using namespace std;
using tint = long long;

void fastIO() { cin.tie(0), ios_base::sync_with_stdio(false); }

struct Node;
using Treap = shared_ptr<Node>;

struct Node {
  tint p, val, size = 1;
  Treap left, right;

  Node(tint v) : p{rand()}, val{v} {}
};

tint size(Treap root) { return root ? root->size : 0; }
void evaluate(Treap root) {
  if (root == nullptr) return;
  root->size = size(root->left) + size(root->right) + 1;
}

pair<Treap, Treap> split(Treap root, tint k) {
  if (root == nullptr) return {nullptr, nullptr};

  Treap left, right;
  if (root->val <= k) {
    left = make_shared<Node>(*root);
    tie(left->right, right) = split(root->right, k);
    evaluate(left);
  } else {
    right = make_shared<Node>(*root);
    tie(left, right->left) = split(root->left, k);
    evaluate(right);
  }
  return {left, right};
}

Treap merge(Treap left, Treap right) {
  if (left == nullptr && right == nullptr) return nullptr;
  if (left == nullptr) return right;
  if (right == nullptr) return left;

  Treap root;
  if (left->p > right->p) {
    root = make_shared<Node>(*left);
    root->right = merge(left->right, right);
  } else {
    root = make_shared<Node>(*right);
    root->left = merge(left, right->left);
  }
  evaluate(root);
  return root;
}

Treap insert(Treap root, tint x) {
  auto [left, right] = split(root, x);
  return merge(merge(left, make_shared<Node>(x)), right);
}

Treap remove(Treap root, tint x) {
  Treap left, middle, right;
  tie(left, right) = split(root, x);
  tie(left, middle) = split(left, x - 1);
  return merge(left, right);
}

tint count(Treap root, tint x) {
  auto [left, right] = split(root, x - 1);
  return size(right);
}

int main() {
  fastIO();

  tint n;
  cin >> n;
  cin.ignore();

  Treap root = nullptr;
  vector<Treap> treaps;
  treaps.push_back(root);

  for (tint i = 0; i < n; i++) {
    string s;
    getline(cin, s);
    if (s.empty()) {
      treaps.push_back(root);
      continue;
    }

    s += ' ';

    bool isAdd = true;
    tint curr = 0;
    for (auto c : s) {
      if (c == '+') {
        isAdd = true;
      } else if (c == '-') {
        isAdd = false;
      } else if (c == ' ') {
        if (isAdd) {
          root = insert(root, curr + 1);
        } else {
          root = remove(root, curr + 1);
        }
        curr = 0;
      } else {
        curr = curr * 10 + (c - '0');
      }
    }

    treaps.push_back(root);
  }

  tint xi = 0;
  for (tint i = 0; i < n; i++) {
    tint d;
    cin >> d;
    xi = (xi + count(treaps[d], xi + 1)) % n;
  }

  cout << xi << '\n';
}