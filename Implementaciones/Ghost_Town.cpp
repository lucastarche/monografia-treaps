#include <bits/stdc++.h>

using namespace std;
void fastIO() { ios_base::sync_with_stdio(false), cin.tie(0); }

struct Node;
using Treap = Node *;

struct Node {
  int priority, value, size = 1, freq = 1;
  Treap left = nullptr, right = nullptr;

  Node(int v) : priority{rand()}, value{v} {}
};

int size(Treap root) { return root ? root->size : 0; }
void evaluate(Treap root) {
  if (root == nullptr) return;
  root->size = size(root->left) + size(root->right) + root->freq;
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
Treap build(const vector<int> &v, int from, int to) {
  if (from == to) return nullptr;

  int mid = (from + to) / 2;
  Treap root = new Node(v[mid]);
  root->left = build(v, from, mid);
  root->right = build(v, mid + 1, to);

  heapify(root);
  evaluate(root);
  return root;
}

void insert(Treap &root, int x) {
  Treap left, middle, right;
  tie(left, right) = split(root, x);
  tie(left, middle) = split(left, x - 1);

  if (middle == nullptr) {
    middle = new Node(x);
  } else {
    middle->freq++, evaluate(middle);
  }

  root = merge(merge(left, middle), right);
}

// Retorna la cantidad de elementos <= x
int count(Treap &root, int x) {
  auto [left, right] = split(root, x);
  int ans = size(left);
  root = merge(left, right);
  return ans;
}

// Retorna el k-ésimo elemento más chico del multiset
int select(Treap root, int k) {
  if (size(root->left) + 1 <= k && k <= size(root->left) + root->freq)
    return root->value;

  if (size(root->left) >= k) {
    return select(root->left, k);
  } else {
    return select(root->right, k - size(root->left) - root->freq);
  }
}

int main() {
  fastIO();
  int n, q;
  cin >> n >> q;

  vector<int> nums(n);
  for (int i = 0; i < n; i++) cin >> nums[i];
  sort(nums.begin(), nums.end());

  Treap root = build(nums, 0, n);
  for (int i = 0; i < q; i++) {
    int type, x;
    cin >> type >> x;

    if (type == 1) {  // Insertar x + count(root, x) al multiset
      insert(root, x + count(root, x));
    } else if (type == 2) {  // Imprimir count(root, x)
      cout << count(root, x) << '\n';
    } else {  // Imprimir el x-ésimo elemento más chico, (o "invalid")
      if (x > size(root)) {
        cout << "invalid" << '\n';
      } else {
        cout << select(root, x) << '\n';
      }
    }
  }
}