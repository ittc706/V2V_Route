# version_2017_3_28(V2V_Route项目启动)

# version_2017_3_28(框架搭建中)

# version_2017_3_28(删了一些不必要的东西)

# version_2017_4_2(路由框架搭建中)

# version_2017_4_2(路由框架搭建中,删除一些不必要的组件)

# version_2017_4_3(路由框架搭建中)

# version_2017_4_3(路由框架初版完成!测试通过!)

# version_2017_4_4(将事件拆分为路由事件以及链路事件)

# version_2017_4_4(测试完成)

# version_2017_4_4(修复warning)

# version_2017_4_4(修复容器初始化时的bug)
* 系统配置文件必须在容器初始化一开始就进行读取，因为后面的初始化依赖于该配置文件中的模式选择

# version_2017_4_11(修改route_tcp,每个节点支持同时收发，且支持同时多收)

# version_2017_4_11(debug，原因未知)

# version_2017_4_11(debug)
* 错误原因是，在syn选择频段时，如果选择了某一频段，恰好该频段收到了来自其他车辆的syn，那么此时收发就会选择同一个频段了

# version_2017_4_11(插入日志)

# version_2017_4_11(添加载干比计算)

# version_2017_4_24(基于UDP的GPSR协议)

# version_2017_4_24(DEBUG)

# version_2017_4_24(DEBUG)

# version_2017_4_26(增加了输出到日志的信息)

# version_2017_4_26(进一步增加了输出到日志的信息并且对所有申请的内存空间进行了释放)

# version_2017_4_26(DEBUG)

# version_2017_4_30(添加反射机制,尚未完成)

# version_2017_4_27(日志中增加输出了传输失败的详细原因)

# version_2017_4_30(添加反射模块)
* 合并配置文件
* 存疑:当object不含有虚函数，但是object的子类多重继承了具有虚函数的基类，会导致get_class_id错误
* gtt和route必须继承object，否则访问成员函数会出现错误，大概原因可能是由于虚函数表的存在导致内存发生了偏移

# version_2017_5_1(初步完成反射模块)
* 尚有问题，tcp和udp都没有日志

# version_2017_5_1(DEBUG)

# version_2017_5_1(优化)
* 配置文件支持注释
* 修复了TCP模式下的问题，由于没有从物理层拉取邻接表

# version_2017_5_1(DEBUG)

# version_2017_5_3(信令与数据频分)

# version_2017_5_4(增加了计算PDR-距离的.m程序)

# version_2017_5_9(修改容器初始化逻辑)

# version_2017_5_9(merge)

# version_2017_5_15(修改GPSR)

# version_2017_5_15(修改回去，修改统计)

# version_2017_5_18(增加了程序最后输出车辆位置)