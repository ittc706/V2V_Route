#include"route.h"
#include"route_tcp.h"
using namespace std;

route* route::route_bind_by_mode(route_mode t_mode) {
	return new route_tcp();
}