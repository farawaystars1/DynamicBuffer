#pragma once
#include<iostream>
#include<vector>
#include<memory.h>
#include<memory>
#include<assert.h>
#include<string>
#include<mutex>
#include<atomic>
//读取写入数据线程安全 append/read
#define ____BIG_ENDIAN
//|data_-----readPos_--------(有效数据)---writePos_--------capacity_ |
class Buffer :public std::enable_shared_from_this<Buffer>
{
	friend class tcp_connection;

public:
	static constexpr int max_capacity = 1024 * 1024 * 64;
	static constexpr int default_capacity = 1024;
	explicit
		Buffer();
	explicit
		Buffer(std::string str);
	~Buffer();

	int8_t readInt8();
#ifdef ____BIG_ENDIAN
	int16_t readInt16();
	int32_t readInt32();
	int64_t readInt64();
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
	void read(char* outData, int n);
	std::string read(int n);
	void append(std::string str);
	void append(char* data, int size);
	std::string readAllAsString();
	void readAll()
	{
		
		readPos_ = 0;
		writePos_ = 0;
		
	}
	int readableBytes()const;
	int writableBytes()const
	{
		return capacity_ - writePos_;
	}
	int capacity()const;
	//char* dataRead()const {//limit use
	//	return data_+readPos_;
	//}
	//char* dataWrite()const//limit use
	//{
	//	return data_+writePos_;
	//}
	//void writePos_add(int n)//limit use
	//{
	//	writePos_+= n;
	//}
	void recapacity(int capacity_new);
private:
	char* data_;//缓存起始位置
	//有效数据 readPos_------->writePos_
	std::atomic_int readPos_;//数据读取位置data_+readPos_
	std::atomic_int writePos_;//数据写入位置data_+writePos_
	std::atomic_int capacity_;//缓存大小
	std::mutex mute;



};

//|data_-----readPos_--------(有效数据)-----capacity_ |
class SimpleBuffer {
public:
	friend class tcp_connection;
	static constexpr int defaultCapacity = 1024;
	SimpleBuffer(){
		data_ = new char[defaultCapacity];
		capacity_ = defaultCapacity;
		readPos_ = 0;
		writePos_ = 0;
	}
	~SimpleBuffer(){
		if (data_) {
			delete[] data_;
			data_ = nullptr;
	}
	}
	void resize(size_t n)
	{
		if (n > capacity_) {
			delete[]data_;
			data_ = new char[n];
			readPos_ = 0;
			writePos_ = 0;
			capacity_ = n;
			
		}
		
	}
	//void read(char* out, size_t n)
	//{
	//	assert(n <= capacity_);
	//	memcpy(out, data_, n);
	//	readPos_ += n;

	//}
	//std::string readAllAsString()
	//{
	//	std::string ret(data_ + readPos_, size());
	//	readPos_ = 0;
	//	return ret;
	//}
	size_t readableBytes()const
	{
		return writePos_ - readPos_;
	}
	size_t capacity()const {
		return capacity_;
	}
private:
	char* data_;
	size_t capacity_;
	size_t readPos_;
	size_t writePos_;

};