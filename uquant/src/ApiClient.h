#ifndef UBEE_APICLIENT_H
#define UBEE_APICLIENT_H
/* 暂时不用grpc
#include "../grpc/cpp/uquantapi/uapi_client.h"
*/
#include "uBEE.h"

namespace uBEE
{

#define API_GET_FUTURE_TICKS 		1001
#define API_GET_FUTURE_BARS 		1002
#define API_GET_STOCK_BARS 			2001

//int getFutureBars(uBEE::Base *b,char *future_id, char *start, char *end, int frequency) ;
int getFutureBars(uBEE::Base *b,const std::string& id, const std::string& start,
                    const std::string& end,int frequency);

} // namespace

#endif // UBEE_APICLIENT_H
