#include "NetworkPacket.h"

#include <algorithm>

namespace Kernal {
NetworkPacket::NetworkPacket() :
		m_pos(0), m_valid(true) {
	//ctor
}

NetworkPacket::~NetworkPacket() {
	//dtor
}

void NetworkPacket::Append(std::vector<uint8> &dataIn) {
	if (!dataIn.empty()) {
		/*std::size_t curSize = this->m_data.size();
		 this->m_data.resize( curSize + sizeIn);*/
		std::copy(dataIn.begin(), dataIn.end(),
				std::back_inserter(this->m_data));
	}
}

void NetworkPacket::Clear() {
	this->m_data.clear();
	this->m_pos = 0;
	this->m_valid = true;
}

const char* NetworkPacket::GetData() {
	return !this->m_data.empty() ?
			reinterpret_cast<const char*>(&this->m_data[0]) : NULL;
}

std::size_t NetworkPacket::GetDataSize() {
	return this->m_data.size();
}

bool NetworkPacket::GetValid() {
	return m_valid;
}

uint16 ToNeworkOrder16(uint16 byte) {
	uint8 shift = 8;

	return (byte << shift) | (byte >> shift);
}
uint16 FromNeworkOrder16(uint16 byte) {
	return ToNeworkOrder16(byte);
}

uint32 ToNeworkOrder32(uint32 word) {
	uint16 shift = 16;

	return ((Kernal::ToNeworkOrder16((word << shift) >> shift)) << shift)
			| (Kernal::ToNeworkOrder16(word >> shift));
}
uint32 FromNeworkOrder32(uint32 word) {
	return ToNeworkOrder32(word);
}

uint64 ToNeworkOrder64(uint64 doubleWord) {
	uint32 shift = 32;

	return ((Kernal::ToNeworkOrder32((doubleWord << shift) >> shift)) << shift)
			| (Kernal::ToNeworkOrder32(doubleWord >> shift));
}
uint64 FromNeworkOrder64(uint64 doubleWord) {
	return ToNeworkOrder64(doubleWord);
}

}
