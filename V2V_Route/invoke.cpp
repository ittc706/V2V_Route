#include<stdexcept>
#include"micro.h"
#include"object.h"



/*
* ��Ԫ��������ע��
*/
object* new_instance(const std::string& class_name) {
	FACTORY_INVOKE_HEAD

		return nullptr;
}

/*
* ��Ԫ��������ע��
*/
void invoke(const object* obj, const std::string& method_name) {
	long class_id = obj->get_class_id();

}

/*
* ��������ע��
* �������ͣ�int
*/
void invoke(const object* obj, const std::string& method_name, int param1) {
	long class_id = obj->get_class_id();

}

/*
* ��������ע��
* �������ͣ�long
*/
void invoke(const object* obj, const std::string& method_name, long param1) {
	long class_id = obj->get_class_id();


}

/*
* ��������ע��
* �������ͣ�float
*/
void invoke(const object* obj, const std::string& method_name, float param1) {
	long class_id = obj->get_class_id();


}

/*
* ��������ע��
* �������ͣ�double
*/
void invoke(const object* obj, const std::string& method_name, double param1) {
	long class_id = obj->get_class_id();

}

/*
* ��������ע��
* �������ͣ�void*
*/
void invoke(const object* obj, const std::string& method_name, void* param1) {
	long class_id = obj->get_class_id();

}

/*
* ��������ע��
* �������ͣ�void*
*/
void invoke(const object* obj, const std::string& method_name, const std::string& param1) {
	long class_id = obj->get_class_id();
	METHOD_INVOKE_CLASS_START
}
