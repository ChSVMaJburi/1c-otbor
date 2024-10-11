#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Global {
std::unordered_map<int64_t, int64_t> count;
std::unordered_map<int64_t, int64_t> hashs;
std::unordered_map<std::string, int64_t> index;
std::vector<std::string> strings;
}  // namespace Global

int64_t GetHash(const std::string& str) {
  return static_cast<int64_t>(std::hash<std::string>{}(str));
}
int64_t GetHashByIndex(int64_t index) { return Global::hashs[index]; }
int64_t ToInt(char c) {
  if (c >= 'A' && c <= 'Z') {
    return c - 'A';
  }
  return c - 'a';
}

struct Node {
 public:
  Node() : max_index_(-1) { next_.resize(26, nullptr); }

  std::shared_ptr<Node> GetNext(int64_t index) {
    if (next_[index] == nullptr) {
      next_[index] = std::make_shared<Node>();
    }
    return next_[index];
  }

  std::shared_ptr<Node> GoNext(int64_t index) const { return next_[index]; }
  int64_t GetIndexOfMax() const { return max_index_; }
  void SetIndexOfMax(int64_t index) { max_index_ = index; }

 private:
  std::vector<std::shared_ptr<Node>> next_;
  int64_t max_index_;
};

class Bor {
 public:
  Bor() { root_ = std::make_shared<Node>(); }

  static void RecursiveAdd(const std::shared_ptr<Node>& cur,
                           const std::string& str, int64_t cur_index,
                           int64_t str_index, int64_t str_hash) {
    if (cur_index != str.size()) {
      RecursiveAdd(cur->GetNext(ToInt(str[cur_index])), str, cur_index + 1,
                   str_index, str_hash);
    }
    if (cur->GetIndexOfMax() == -1 ||
        Global::count[str_hash] >
            Global::count[GetHashByIndex(cur->GetIndexOfMax())]) {
      cur->SetIndexOfMax(str_index);
    }
  }

  void Add(const std::string& str, int64_t str_index) const {
    int64_t hash = GetHash(str);
    ++Global::count[hash];
    RecursiveAdd(root_, str, 0, str_index, hash);
  }

  std::shared_ptr<Node> Get(const std::string& str) const {
    std::shared_ptr<Node> cur = root_;
    for (char i : str) {
      cur = cur->GoNext(ToInt(i));
      if (cur == nullptr) {
        return nullptr;
      }
    }
    return cur;
  }

  static std::shared_ptr<Node> Continue(std::shared_ptr<Node> cur, char c) {
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
  std::shared_ptr<Node> root_;
};

void AddingText(const Bor& bor, const std::string& text) {
  int64_t last = 0;
  std::string str;
  for (int64_t i = 0; i <= text.size(); ++i) {
    if (i == text.size() || text[i] == ' ') {
      if (last != i) {
        str = text.substr(last, i - last);
        if (Global::index.find(str) == Global::index.end()) {
          Global::index[str] = static_cast<int64_t>(Global::strings.size());
          Global::hashs[static_cast<int64_t>(Global::strings.size())] =
              GetHash(str);
          Global::strings.push_back(str);
        }
        bor.Add(str, Global::index[str]);
      }
      last = i + 1;
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  Bor Aminoff;

  int64_t type;
  std::string text;
  std::string last_str = "";
  char c;
  std::shared_ptr<Node> last = nullptr;
  while (true) {
    std::cin >> type;

    if (type == 0) {  // конец запросов
      return 0;
    }
    if (type == 1) {  // добавить текст
      std::getline(std::cin, text);
      std::getline(std::cin, text);
      AddingText(Aminoff, text);
      continue;
    }
    if (type == 2) {  // новый запрос
      std::cin >> last_str;
      last = Aminoff.Get(last_str);
    } else if (type == 3) {  // дописывание
      std::cin >> c;
      last_str += c;
      last = Bor::Continue(last, c);
    } else {
      std::cout << "Please enter a valid option." << std::endl;
      continue;
    }
    if (last == nullptr) {
      std::cout << last_str << std::endl;
    } else {
      std::cout << Global::strings[last->GetIndexOfMax()] << std::endl;
    }
  }
}
