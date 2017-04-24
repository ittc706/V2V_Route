#include"route.h"
#include"route_tcp.h"
#include"route_udp.h"
using namespace std;

route* route::route_bind_by_mode(route_mode t_mode) {
	if (t_mode == TCP) {
		return new route_tcp();
	}
	else if (t_mode == UDP) {
		return new route_udp();
	}
	else {
		throw logic_error("error");
	}
}