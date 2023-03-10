#pragma once
#include<iostream>
#include<vector>
#include<memory.h>
#include<memory>
#include<assert.h>
#include<string>
#include<mutex>
#include<atomic>
#define ____BIG_ENDIAN

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
	char* data_;//������ʼλ��
	//��Ч���� readPos_------->writePos_
	std::atomic_int readPos_;//���ݶ�ȡλ��data_+readPos_
	std::atomic_int writePos_;//����д��λ��data_+writePos_
	std::atomic_int capacity_;//�����С
	std::mutex mute;



};