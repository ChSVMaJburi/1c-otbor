#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Global {
std::unordered_map<int32_t, int32_t> count;
std::unordered_map<int32_t, int32_t> hashs;
std::unordered_map<std::string, int32_t> index;
std::vector<std::string> strings;
}  // namespace Global

int32_t GetHash(const std::string& str) {
  return std::hash<std::string>{}(str);
}
int32_t GetHashByIndex(int32_t index) { return Global::hashs[index]; }
int32_t ToInt(char c) {
  if (c >= 'A' && c <= 'Z') {
    return c - 'A';
  }
  return c - 'a';
}

struct Node {
 public:
  Node() : ind_max_(-1) { next_.resize(26, nullptr); }

  Node* GetNext(int32_t index) {
    if (next_[index] == nullptr) {
      next_[index] = new Node();
    }
    return next_[index];
  }

  Node* GoNext(int32_t index) const { return next_[index]; }
  int32_t GetIndexOfMax() const { return ind_max_; }
  void SetIndexOfMax(int32_t index) { ind_max_ = index; }

 private:
  std::vector<Node*> next_;
  int32_t ind_max_;
};

class Bor {
 public:
  Bor() { root_ = new Node(); }
  ~Bor() { Delete(root_); }

  static void RecursiveAdd(Node* cur, const std::string& str, int32_t cur_index,
                           int32_t str_index, int32_t str_hash) {
    if (cur_index == str.size()) {
      return;
    }
    RecursiveAdd(cur->GetNext(ToInt(str[cur_index])), str, cur_index + 1,
                 str_index, str_hash);

    if (cur->GetIndexOfMax() == -1 ||
        Global::count[str_hash] >
            Global::count[GetHashByIndex(cur->GetIndexOfMax())]) {
      cur->SetIndexOfMax(str_index);
    }
  }

  void Add(const std::string& str, int32_t str_index) const {
    int32_t hash = GetHash(str);
    ++Global::count[hash];
    RecursiveAdd(root_, str, 0, str_index, hash);
  }

  Node* Get(const std::string& str) const {
    Node* cur = root_;
    for (int32_t i = 0; i < str.size(); ++i) {
      cur = cur->GoNext(ToInt(str[i]));
      if (cur == nullptr) {
        return nullptr;
      }
    }
    return cur;
  }

  static Node* Continue(Node* cur, char c) {
    if (cur == nullptr) {
      return nullptr;
    }
    cur = cur->GoNext(ToInt(c));
    if (cur == nullptr) {
      return nullptr;
    }
    return cur;
  }

 private:
  void Delete(Node* cur) {
    if (cur == nullptr) {
      return;
    }
    for (int32_t i = 0; i < 26; i++) {
      Delete(cur->GoNext(i));
    }
    delete cur;
  }
  Node* root_;
};

void AddingText(Bor& bor, const std::string& text) {
  int32_t last = 0;
  std::string str;
  for (int32_t i = 0; i <= text.size(); ++i) {
    if (i == text.size() || text[i] == ' ') {
      if (last != i) {
        str = text.substr(last, i - last);
        if (Global::index.find(str) == Global::index.end()) {
          Global::index[str] = Global::strings.size();
          Global::hashs[Global::strings.size()] = GetHash(str);
          Global::strings.push_back(str);
        }
        bor.Add(str, Global::index[str]);
      }
      last = i + 1;
    }
  }
}

int32_t main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  Bor Aminoff;

  int32_t type;
  std::string text;
  std::string last_str;
  char c;
  Node* last = nullptr;
  while (true) {
    std::cin >> type;

    if (type == 0) {  // конец запросов
      return 0;
    }
    if (type == 1) {  // добавить текст
      std::getline(std::cin, text);
      std::getline(std::cin, text);
      std::cout << "Adding " << text << std::endl;
      AddingText(Aminoff, text);
      continue;
    }
    if (type == 2) {  // новый запрос
      std::cin >> last_str;
      last = Aminoff.Get(last_str);
    } else {  // дописывание
      std::cin >> c;
      last_str += c;
      last = Aminoff.Continue(last, c);
    }
    if (last == nullptr) {
      std::cout << last_str << std::endl;
    } else {
      std::cout << Global::strings[last->GetIndexOfMax()] << std::endl;
    }
  }
}
