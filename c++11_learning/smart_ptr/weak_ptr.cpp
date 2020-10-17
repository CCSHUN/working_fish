/*
  参考https://www.cnblogs.com/goya/p/11967200.html
*/
#include <memory>
#include <iostream>
#include <vector>

class Notify;

class Observer {
 public:
  Observer() {
   }
  ~Observer() {
    std::cout<<"Observer deconstructor"<<std::endl;
  }
  void Notice() {
      std::cout<<"I get notify"<<std::endl;
  }
};

class Notify {
 public:  
  Notify(){
  }
  ~Notify() {
    std::cout<<"Notify deconstructor"<<std::endl;
  }
  void Register(std::shared_ptr<Observer> observer) {
    observers_.push_back(observer);
  }
  void NoticeObservers() {
    auto it = observers_.begin();
    for (; it != observers_.end();) {
      auto ptr= it->lock();  //升级为share
      if (ptr) {
        ptr->Notice();
        it++;
      } else {
        it = observers_.erase(it);
        continue;
      }
    }
  }
 private:
  std::vector<std::weak_ptr<Observer>> observers_;
};

int main() {
  std::shared_ptr<Observer> observer = std::make_shared<Observer>();
  Notify notify;
  notify.Register(observer);
  notify.NoticeObservers();
  return 0;
}