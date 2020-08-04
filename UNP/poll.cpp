/*
  #include <poll.h>
  int poll(struct pollfd fd[], nfds_t nfds, int timeout);

*/
#include <poll.h>
#include <vector>

int main() {
  typedef std::vector<struct pollfd> PollFdList;
  
  

  return 0;
}