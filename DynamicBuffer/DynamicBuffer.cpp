// DynamicBuffer.cpp: 定义应用程序的入口点。
//

#include "DynamicBuffer.h"
#include"Buffer/Buffer.h"
#include<iostream>
#include<iostream>
#include<memory>
#include<vector>
int main()
{
	std::shared_ptr<Buffer> ptr(new Buffer("good"));
	ptr->append("hello");
	ptr->append("hello9999");
	ptr->append("hello");
	ptr->append("hello");
	ptr->append("hello");
	auto x=ptr->read(11);

}
