#include"context.h"
#include"invoke.h"
#include<iostream>

using namespace std;

context* context::__context = nullptr;

context::context(const std::string& path) :configuration_path(path) {
	loader = new bean_loader(path);
}

context::~context() {
	delete loader;
}

void context::create_context(const std::string& path) {
	static context _context = context(path);
	__context = &_context;
	__context->init();
}

context* context::get_context() {
	assert(__context != nullptr);
	return __context;
}

void context::init() {
	parseConfiguration();

	create_and_init_bean();

	dependency_injection();

	pre_process();

	post_process();
}

void context::parseConfiguration() {
	bean_definitions.swap(loader->load());
}

void context::create_and_init_bean() {
	for (bean_definition* definition : bean_definitions) {
		string bean_id = definition->id;
		string class_type = definition->class_type;
		/*cout << bean_id << endl;
		cout << class_type << endl;*/
		object* _obj = new_instance(class_type);
		assert(_obj != nullptr);
		assert(bean_map.insert({ bean_id ,_obj }).second);
		cout << bean_id<<","<< class_type << ", " << _obj->get_class_id() << endl;
		for (bean_property p : definition->properties) {
			invoke(_obj, "set_" + p.name, p.value);
		}
	}
}

void context::dependency_injection() {
	for (bean_definition* definition : bean_definitions) {
		string bean_id = definition->id;
		assert(bean_map.find(bean_id) != bean_map.end());
		for (bean_dependency dependency : definition->dependencies) {
			string dependency_bean_id = dependency.ref_id;
			assert(bean_map.find(dependency_bean_id) != bean_map.end());
			invoke(bean_map[bean_id], "set_" + dependency.name, bean_map[dependency_bean_id]);
		}
	}
}

void context::pre_process() {
	for (bean_definition* definition : bean_definitions) {
		string bean_id = definition->id;
		object* __obj = bean_map[bean_id];
		for (string pre_processor : definition->pre_processors) {
			invoke(__obj, pre_processor);
		}
	}
}

void context::post_process() {
	for (bean_definition* definition : bean_definitions) {
		string bean_id = definition->id;
		object* __obj = bean_map[bean_id];
		for (string post_processor : definition->post_processors) {
			cout << bean_id << endl;
			cout << post_processor << endl;
			invoke(__obj, post_processor);
		}
	}
}

object* context::get_bean(std::string bean_id) {
	assert(bean_map.find(bean_id) != bean_map.end());
	return bean_map[bean_id];
}

void context::add_non_bean(std::string bean_id,void* non_bean) {
	assert(non_bean_map.find(bean_id) == non_bean_map.end());
	non_bean_map.insert({ bean_id ,non_bean });
}

void* context::get_non_bean(std::string bean_id) {
	assert(non_bean_map.find(bean_id) != non_bean_map.end());
	return non_bean_map[bean_id];
}

//int main() {
//	context::create_context("E:\\CppProgram\\Reflect\\Reflect\\pattern2\\beans.xml");
//	
//	system("pause");
//}