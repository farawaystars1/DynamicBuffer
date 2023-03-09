#pragma once
#include<iostream>
#include<vector>
#include<memory.h>
#include<memory>
#include<assert.h>
#include<string>
#include"Endian.h"
#define BIG_ENDIAN
using namespace std;
class Buffer :public std::enable_shared_from_this<Buffer>
{


public:
	static constexpr int max_capacity = 1024 * 1024 * 16;
	static constexpr int default_capacity = 1024;
	explicit
		Buffer() {
		data_ = new char[default_capacity];
		readPos_ = 0;
		writePos_ = 0;
		capacity_ = default_capacity;

	}
	explicit
		Buffer(std::string str) {
		data_ = new char[default_capacity];
		readPos_ = 0;
		writePos_ = 0;
		capacity_ = default_capacity;
		append(str);

	}
	~Buffer()
	{
		delete[]data_;
		data_ = nullptr;
	}

	int8_t readInt8()
	{
		assert(readableBytes() >= 1);
		readPos_++;
		return *(data_ + readPos_);
	}
#ifdef BIG_ENDIAN
	int16_t readInt16()
	{
		assert(readableBytes() >= 2);
		readPos_ += 2;
		return Endian::htobe16_(*(reinterpret_cast<short*>(data_ + readPos_)));
	}
	int32_t readInt32()
	{
		assert(readableBytes() >= 4);
		readPos_ += 4;
		return Endian::htobe32_(*(reinterpret_cast<int*>(data_ + readPos_)));
	}
	int64_t readInt64()
	{

		assert(readableBytes() >= 8);
		readPos_ += 8;
		return Endian::htobe64_(*(reinterpret_cast<int64_t*>(data_ + readPos_)));
	}
#else
	int16_t readInt16()
	{
		assert(readableBytes() >= 2);
		readPos_ += 2;
		return *(reinterpret_cast<short*>(data_ + readPos_));
	}
	int32_t readInt32()
	{
		assert(readableBytes() >= 4);
		readPos_ += 4;
		return *(reinterpret_cast<int*>(data_ + readPos_));
	}
	int64_t readInt64()
	{

		assert(readableBytes() >= 8);
		readPos_ += 8;
		return *(reinterpret_cast<int64_t*>(data_ + readPos_));
	}
#endif
	void read(char*outData,int n) {
		assert(readableBytes() >= n);
		readPos_ += n;
		memcpy(outData, data_ + readPos_, n);
	}
	std::string read(int n)
	{
		assert(readableBytes() >= n);
		std::string ret(readPos_ + data_, n);
		readPos_ += n;
		return ret;

	}
	void append(std::string str)
	{
		if (str.size() > writableBytes())
		{
			assert(str.size() + capacity_ <= max_capacity);
			recapacity(str.size() + capacity_);
		}
		memcpy(data_ + writePos_, str.data(), str.size());
		writePos_ += str.size();

	}
	void append(char* data, int size)
	{
		if (size > writableBytes())
		{
			assert(size + capacity_ > max_capacity);
			recapacity(size + capacity_);
		}
		memcpy(data_ + writePos_, data, size);
		writePos_ += size;

	}
	std::string readAllAsString()
	{
		std::string ret(data_+readPos_, readableBytes());
	
		readPos_ = 0;
		writePos_ = 0;
		return ret;

	}
	void readAll()
	{
		readPos_ = 0;
		writePos_ = 0;
	}
	int readableBytes()
	{
		return writePos_-readPos_;
	}
	int writableBytes()
	{
		return capacity_ - writePos_;
	}
private:
	void recapacity(int capacity_new)
	{
		assert(capacity_new >= readableBytes());
		char* p = new char[capacity_new];
		int readable_size = readableBytes();
		memcpy(p, data_ + readPos_, readable_size);
		readPos_ = 0;
		writePos_ = readable_size;
		capacity_ = capacity_new;
		delete[]data_;
		data_ = p;
	}
private:
	char* data_;//缓存起始位置
	//有效数据 readPos_------->writePos_
	int readPos_;//数据读取位置data_+readPos_
	int writePos_;//数据写入位置data_+writePos_
	int capacity_;//缓存大小



};