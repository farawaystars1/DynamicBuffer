#include "Buffer.h"
#include"Endian.h"
Buffer::Buffer()
{
	data_ = new char[default_capacity];
	readPos_ = 0;
	writePos_ = 0;
	capacity_ = default_capacity;

}

Buffer::Buffer(std::string str)
{
	data_ = new char[default_capacity];
	readPos_ = 0;
	writePos_ = 0;
	capacity_ = default_capacity;
	append(str);

}
Buffer::~Buffer()
{
	if (data_)
	{
		delete []data_;
		data_ = nullptr;
	}
}

int8_t Buffer::readInt8()
{

	assert(readableBytes() >= 1);
	readPos_++;
	return *(data_ + readPos_);

}



 int16_t Buffer::readInt16()
{

	assert(readableBytes() >= 2);
	readPos_ += 2;
	return Endian::htobe16_(*(reinterpret_cast<short*>(data_ + readPos_)));
}
 int32_t Buffer::readInt32()
{

	assert(readableBytes() >= 4);
	readPos_ += 4;
	return Endian::htobe32_(*(reinterpret_cast<int*>(data_ + readPos_)));
}
 int64_t Buffer::readInt64()
 {

	 assert(readableBytes() >= 8);
	 readPos_ += 8;
	 return Endian::htobe64_(*(reinterpret_cast<int64_t*>(data_ + readPos_)));
 }

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

  void Buffer::read(char* outData, int n) {

	 assert(readableBytes() >= n);
	 readPos_ += n;
	 memcpy(outData, data_ + readPos_, n);

 }

  std::string Buffer::read(int n)
 {

	 assert(readableBytes() >= n);
	 std::string ret(readPos_ + data_, n);
	 readPos_ += n;

	 return ret;


 }

  void Buffer::append(std::string str)
 {

	 if (str.size() > writableBytes())
	 {
		 assert(str.size() + capacity_ <= max_capacity);
		 recapacity(str.size() + capacity_);
	 }
	 memcpy(data_ + writePos_, str.data(), str.size());
	 writePos_ += str.size();

 }

  void Buffer::append(char* data, int size)
 {

	 if (size > writableBytes())
	 {
		 assert(size + capacity_ <= max_capacity);
		 recapacity(size + capacity_);
	 }
	 memcpy(data_ + writePos_, data, size);
	 writePos_ += size;

 }

  std::string Buffer::readAllAsString()
 {

	 std::string ret(data_ + readPos_, readableBytes());

	 readPos_ = 0;
	 writePos_ = 0;

	 return ret;


 }

  int Buffer::readableBytes() const
 {
	 return writePos_ - readPos_;
 }

  int Buffer::capacity() const
 {
	 return capacity_;
 }

  void Buffer::recapacity(int capacity_new)
 {

	 assert(capacity_new >= readableBytes());
	 char* p = new char[capacity_new];
	 int readable_size = readableBytes();
	 memcpy(p, data_ + readPos_, readable_size);
	 mute.lock();
	 readPos_ = 0;
	 writePos_ = readable_size;
	 capacity_ = capacity_new;
	 delete[]data_;
	 data_ = p;
	 mute.unlock();

 }