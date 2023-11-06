#include <bits/stdc++.h>

using namespace std;
void fastIO() { ios_base::sync_with_stdio(false), cin.tie(0); }

const string BASES = "AGTC";
int toBaseIndex(char c) {
  for (int i = 0; i < 4; i++) {
    if (c == BASES[i]) return i;
  }

  assert(false);
}

struct Node;
using Treap = Node *;
using Freq = array<int, 4>;

Freq operator+(const Freq &lhs, const Freq &rhs) {
  Freq freq;
  for (int i = 0; i < 4; i++) freq[i] = lhs[i] + rhs[i];
  return freq;
}

ostream &operator<<(ostream &out, const Freq &freq) {
  for (int i = 0; i < 4; i++) {
    out << freq[i];
    if (i + 1 < 4) out << ' ';
  }
  return out;
}

struct Node {
  int priority, size = 1, base;
  Freq f;
  Treap left = nullptr, right = nullptr;

  Node(char c) : priority{rand()} {
    base = toBaseIndex(c);
    for (int i = 0; i < 4; i++) f[i] = 0;
    f[base]++;
  }

  Node(Treap root) {
    assert(root != nullptr);
    priority = root->priority;
    size = root->size;
    f = root->f;
    left = root->left;
    right = root->right;
    base = root->base;
  }
};

int size(Treap root) { return root ? root->size : 0; }
Freq freq(Treap root) { return root ? root->f : Freq({0, 0, 0, 0}); }

void evaluate(Treap root) {
  if (root == nullptr) return;
  root->size = size(root->left) + size(root->right) + 1;
  root->f = freq(root->left) + freq(root->right);
  root->f[root->base]++;
}

pair<Treap, Treap> split(Treap root, int k) {
  if (root == nullptr) return {nullptr, nullptr};

  int key = size(root->left) + 1;
  Treap left, right;
  if (key <= k) {
    left = new Node(root);
    tie(left->right, right) = split(root->right, k - key);
    evaluate(left);
  } else {
    right = new Node(root);
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
  if (left->priority > right->priority) {
    root = new Node(left);
    root->right = merge(left->right, right);
  } else {
    root = new Node(right);
    root->left = merge(left, right->left);
  }
  evaluate(root);
  return root;
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

int main() {
  fastIO();

  int n;
  cin >> n;

  vector<Treap> DNAs(n);
  for (int i = 0; i < n; i++) {
    string s;
    cin >> s;
    DNAs[i] = build(s, 0, (int)(s.size()));
  }

  int q;
  cin >> q;
  for (int qi = 0; qi < q; qi++) {
    string type;
    cin >> type;

    if (type == "CROSS") {
      int id1, id2, k1, k2;
      cin >> id1 >> id2 >> k1 >> k2;
      id1--, id2--;

      Treap startDNA1, startDNA2, endDNA1, endDNA2;
      tie(startDNA1, endDNA1) = split(DNAs[id1], k1);
      tie(startDNA2, endDNA2) = split(DNAs[id2], k2);
      DNAs.push_back(merge(startDNA1, endDNA2));
      DNAs.push_back(merge(startDNA2, endDNA1));
    } else if (type == "MUTATE") {
      int id, k;
      char base;
      cin >> id >> k >> base;
      id--;

      Treap left, middle, right;
      tie(left, right) = split(DNAs[id], k);
      tie(left, middle) = split(left, k - 1);

      middle = new Node(base);
      DNAs[id] = merge(merge(left, middle), right);
    } else {
      int id, k1, k2;
      cin >> id >> k1 >> k2;
      id--;

      Treap left, middle, right;
      tie(left, right) = split(DNAs[id], k2);
      tie(left, middle) = split(left, k1 - 1);

      cout << middle->f << '\n';
    }
  }
}