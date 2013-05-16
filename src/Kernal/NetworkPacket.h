#ifndef __NETWORK_PACKET_H__
#define __NETWORK_PACKET_H__

#include <vector>
#include "Config.h"

namespace Kernal {
class NetworkPacket {
public:
	/** Default constructor */
	NetworkPacket();
	/** Default destructor */
	virtual ~NetworkPacket();

	void Append(std::vector<uint8> &dataIn);
	void Clear();
	/** Access m_data
	 * \return The current value of m_data
	 */
	const char* GetData();
	/** Gets the current datas size
	 * \return The size of data
	 */
	std::size_t GetDataSize();
	/** Access m_valid
	 * \return The current value of m_valid
	 */
	bool GetValid();
protected:
private:
	std::vector<uint8> m_data; //!< Member variable "m_data"
	std::size_t m_pos; //!< Member variable "m_pos"
	bool m_valid; //!< Member variable "m_valid"
};

uint16 ToNeworkOrder16(uint16 byte);
uint16 FromNeworkOrder16(uint16 byte);

uint32 ToNeworkOrder32(uint32 word);
uint32 FromNeworkOrder32(uint32 word);

uint64 ToNeworkOrder64(uint64 doubleWord);
uint64 FromNeworkOrder64(uint64 doubleWord);

} //namespace Kernal

#endif // __NETWORK_PACKET_H__
