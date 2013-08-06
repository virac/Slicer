/*
 * LithographHandler.h
 *
 *  Created on: Jun 5, 2013
 *      Author: Jon
 *
 */

#ifndef LITHOGRAPHHANDLER_H_
#define LITHOGRAPHHANDLER_H_

#include "Kernal\MessageHandler.h"
#include "Kernal\Letters.h"
#include "Math\Triangle.h"

/*The LithographHandler takes in files, scans the point
 *       data and setings, and then sends out the messages to
 *       create each lithographic slice
 */
class LithographHandler: public Kernal::MessageHandlerClient {
public:
	LithographHandler();
	~LithographHandler();
protected:
private:
	void cb(Kernal::MessageHandlerArg* arg);
};

namespace Kernal {
namespace Letters {
class LithographicFrameCreationLetter: public Letter {
public:
	static std::string ID;
	LithographicFrameCreationLetter() :
			frameId(0), layer(0.0), distOffset(0.0),
			planeNormal(NULL), triangles(NULL) {
	}
	~LithographicFrameCreationLetter() {
	}

	int frameId;
	double layer;
	double distOffset;
	Math::Vector *planeNormal;
	Math::TrianglePtrArray *triangles;
};
}
}
#endif /* LITHOGRAPHHANDLER_H_ */
