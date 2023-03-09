#pragma once
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
#ifdef _WINDOWS
typedef unsigned long long uint64_t;
#elif defined(__linux)
typedef unsigned long  uint64_t;
#endif

namespace Endian {
	uint16_t 
	 htobe16_(uint16_t host)
	{
		return host >> 8 | host << 8;
	}
	uint32_t
	 htobe32_(uint32_t host)
	{
		return (host >> 24) |\
			   (host >> 8 &  0xff00) |\
		       (host << 8 &  0xff0000) | \
			   (host << 24);
	}
	uint64_t
	 htobe64_(uint64_t host)
	{
		return (host >> 56 & 0xff) | \
			   (host >> 40 & 0xff00) | \
			   (host >> 24 & 0xff0000) | \
			   (host >> 8 &  0xff000000) | \
			   (host << 8 &  0xff00000000) | \
		       (host << 24 & 0xff0000000000) | \
			   (host << 40 & 0xff000000000000) | \
			   (host << 56);
	}
	
}