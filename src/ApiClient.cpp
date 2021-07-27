#include "ApiClient.h"
namespace uBEE
{

int getFutureBars(uBEE::Base *b,const std::string& id, const std::string& start, 
                    const std::string& end,int frequency)
{

  /* 暂时不用grpc
  kBarReply ret = b->api->getFutureBars(id,start,end,frequency);
  int size = ret.kk_size();
  std::cout << "bars num: " << size << std::endl;
  for(int i=0; i<size; i++) {
    kBar bar = ret.kk(i);
    std::cout << i+1 <<":" << bar.b() << " o: " << bar.o() << " h: " << bar.h() << std::endl;
  }
  */
  return 0;
}

} // namespace
