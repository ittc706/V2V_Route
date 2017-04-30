#include<regex>
#include<fstream>
#include<iostream>
#include<queue>
#include<set>
#include<map>
#include<utility>
#include<assert.h>
#include"bean_loader.h"

using namespace std;

vector<bean_definition*> bean_loader::load() {
	vector<bean_definition*> definitions;

	const regex BEAN_REGEX("<bean([^>]+)>([\\s\\S]*?)</bean>");


	//读取待解析字符串
	ifstream in(configuration_path);
	istreambuf_iterator<char> if_it(in), if_eof;
	string content(if_it, if_eof);
	in.close();

	//解析，并存储
	for (sregex_iterator it(content.begin(), content.end(), BEAN_REGEX), eof; it != eof; ++it) {
		string inside_info = it->operator[](1);
		string content_info = it->operator[](2);

		bean_definition* definition = create_bean_definition(inside_info);

		add_property(definition, content_info);
		add_dependency(definition, content_info);
		add_pre_processor(definition, content_info);
		add_post_processor(definition, content_info);

		definitions.push_back(definition);
	}

	//根据依赖关系确定有向图访问顺序
	order_by_dependency(definitions);

	for (bean_definition* definition : definitions) {
		cout << definition->to_string() << endl;
	}

	return definitions;
}

bean_definition* bean_loader::create_bean_definition(const std::string s) {
	bean_definition* definition = new bean_definition();
	const regex REGEX("id *= *\"([^\"]+)\" *, *class *= *\"([^\"]+)\"");
	sregex_iterator it(s.begin(), s.end(), REGEX);
	definition->id = it->operator[](1);
	definition->class_type = it->operator[](2);
	return definition;
}

void bean_loader::add_property(bean_definition* definition, const std::string s) {
	const regex REGEX("<property +name *= *\"([^\"]+)\" *, *value *= *\"([^\"]+)\" */>");
	for (sregex_iterator it(s.begin(), s.end(), REGEX), eof; it != eof; ++it) {
		definition->properties.push_back(bean_property(
			it->operator[](1),
			it->operator[](2)
			));
	}
}

void bean_loader::add_dependency(bean_definition* definition, const std::string s) {
	const regex REGEX("<dependency +ref-id *= *\"([^\"]+)\" */>");
	for (sregex_iterator it(s.begin(), s.end(), REGEX), eof; it != eof; ++it) {
		definition->dependencies.push_back(it->operator[](1));
	}
}

void bean_loader::add_pre_processor(bean_definition* definition, const std::string s) {
	const regex REGEX("<pre-processor +method_name *= *\"([^\"]+)\" */>");
	for (sregex_iterator it(s.begin(), s.end(), REGEX), eof; it != eof; ++it) {
		definition->pre_processors.push_back(it->operator[](1));
	}
}

void bean_loader::add_post_processor(bean_definition* definition, const std::string s) {
	const regex REGEX("<post-processor +method_name *= *\"([^\"]+)\" */>");
	for (sregex_iterator it(s.begin(), s.end(), REGEX), eof; it != eof; ++it) {
		definition->post_processors.push_back(it->operator[](1));
	}
}


void bean_loader::order_by_dependency(std::vector<bean_definition*>& definitions) {
	map<string, bean_definition*> bean_map;
	map<string, vector<string>> adj_map;
	map<string, int> degree_map;
	for (bean_definition* definition : definitions) {
		string bean_id = definition->id;
		assert(bean_map.insert({ bean_id,definition }).second);
		for (string dependency : definition->dependencies) {
			if (adj_map.find(dependency) == adj_map.end()) {
				adj_map.insert({ dependency,{ bean_id } });
			}
			else {
				assert(adj_map[dependency].size() > 0);
				adj_map[dependency].push_back(bean_id);
			}
		}
		assert(degree_map.insert({ bean_id,definition->dependencies.size() }).second);
	}
	queue<string> queue;
	for (pair<string, int> key_value: degree_map){
		if (key_value.second == 0) {
			queue.push(key_value.first);
		}
	}
	vector<bean_definition*> temp;
	while (!queue.empty()) {
		string bean_id = queue.front();
		queue.pop();

		bean_definition* definition = bean_map[bean_id];
		temp.push_back(definition);
		const vector<string>& adj_list = adj_map[bean_id];
		for (string other_bean_id : adj_list) {
			if (--degree_map[other_bean_id] == 0) {
				queue.push(other_bean_id);
			}
		}
	}
	definitions.swap(temp);
}


//int main() {
//	bean_loader bl("E:\\CppProgram\\Reflect\\Reflect\\pattern2\\beans.xml");
//	bl.load();
//	system("pause");
//}