#include<stdexcept>
#include"invoke.h"
#include"../config.h"
#include"../system_control.h"
#include"../gtt_highspeed.h"
#include"../gtt_urban.h"

/*
* 零元函数反射注册
*/
object* new_instance(const std::string& class_name) {
	FACTORY_INVOKE_HEAD
		FACTORY_INVOKE(system_control)
		FACTORY_INVOKE(global_control_config)
		FACTORY_INVOKE(gtt_highspeed_config)

		FACTORY_INVOKE(gtt_urban_config)
		FACTORY_INVOKE(rrm_config)
		FACTORY_INVOKE(tmc_config)
		FACTORY_INVOKE(route_config)

		return nullptr;
}

/*
* 零元函数反射注册
*/
void invoke(const object* obj, const std::string& method_name) {
	long class_id = obj->get_class_id();
	METHOD_INVOKE_CLASS_START(rrm_config)
		METHOD_INVOKE_ZERO_PARAM(rrm_config, set_pattern_num)
		METHOD_INVOKE_CLASS_END(rrm_config)
}

/*
* 函数反射注册
* 参数类型：int
*/
void invoke(const object* obj, const std::string& method_name, int param1) {
	long class_id = obj->get_class_id();

}

/*
* 函数反射注册
* 参数类型：long
*/
void invoke(const object* obj, const std::string& method_name, long param1) {
	long class_id = obj->get_class_id();


}

/*
* 函数反射注册
* 参数类型：float
*/
void invoke(const object* obj, const std::string& method_name, float param1) {
	long class_id = obj->get_class_id();


}

/*
* 函数反射注册
* 参数类型：double
*/
void invoke(const object* obj, const std::string& method_name, double param1) {
	long class_id = obj->get_class_id();

}

/*
* 函数反射注册
* 参数类型：void*
*/
void invoke(const object* obj, const std::string& method_name, void* param1) {
	long class_id = obj->get_class_id();

}

/*
* 函数反射注册
* 参数类型：void*
*/
void invoke(const object* obj, const std::string& method_name, const std::string& param1) {
	long class_id = obj->get_class_id();
	METHOD_INVOKE_CLASS_START(global_control_config)
		METHOD_INVOKE_ONE_PARAM(global_control_config, set_platform)
		METHOD_INVOKE_ONE_PARAM(global_control_config, set_ntti)
		METHOD_INVOKE_ONE_PARAM(global_control_config, set_gtt_mode)
		METHOD_INVOKE_ONE_PARAM(global_control_config, set_route_mode)
		METHOD_INVOKE_CLASS_END(global_control_config)


		METHOD_INVOKE_CLASS_START(gtt_highspeed_config)
		METHOD_INVOKE_ONE_PARAM(gtt_highspeed_config, set_road_length)
		METHOD_INVOKE_ONE_PARAM(gtt_highspeed_config, set_road_width)
		METHOD_INVOKE_ONE_PARAM(gtt_highspeed_config, set_speed)
		METHOD_INVOKE_ONE_PARAM(gtt_highspeed_config, set_freshtime)
		METHOD_INVOKE_CLASS_END(gtt_highspeed_config)

		METHOD_INVOKE_CLASS_START(gtt_urban_config)
		METHOD_INVOKE_ONE_PARAM(gtt_urban_config, set_road_length_ew)
		METHOD_INVOKE_ONE_PARAM(gtt_urban_config, set_road_length_sn)
		METHOD_INVOKE_ONE_PARAM(gtt_urban_config, set_road_width)
		METHOD_INVOKE_ONE_PARAM(gtt_urban_config, set_speed)
		METHOD_INVOKE_ONE_PARAM(gtt_urban_config, set_freshtime)
		METHOD_INVOKE_CLASS_END(gtt_urban_config)

		METHOD_INVOKE_CLASS_START(rrm_config)
		METHOD_INVOKE_ONE_PARAM(rrm_config, set_total_bandwidth)
		METHOD_INVOKE_ONE_PARAM(rrm_config, set_rb_num_per_pattern)
		METHOD_INVOKE_ONE_PARAM(rrm_config, set_drop_sinr_boundary)
		METHOD_INVOKE_ONE_PARAM(rrm_config, set_select_altorithm)
		METHOD_INVOKE_ONE_PARAM(rrm_config, set_time_division_granularity)
		METHOD_INVOKE_CLASS_END(rrm_config)

		METHOD_INVOKE_CLASS_START(tmc_config)
		METHOD_INVOKE_ONE_PARAM(tmc_config, set_package_num)
		METHOD_INVOKE_ONE_PARAM(tmc_config, set_hello_tti)
		METHOD_INVOKE_ONE_PARAM(tmc_config, set_trigger_rate)
		METHOD_INVOKE_CLASS_END(tmc_config)

		METHOD_INVOKE_CLASS_START(route_config)
		METHOD_INVOKE_ONE_PARAM(route_config, set_interval)
		METHOD_INVOKE_CLASS_END(route_config)
}
