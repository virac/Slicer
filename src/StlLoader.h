/*
 * StlLoader.h
 *
 *  Created on: May 18, 2013
 *      Author: Jon
 */

#ifndef __STLLOADER_H__
#define __STLLOADER_H__

#include <string>

#include "Kernal\MessageHandler.h"
#include "Kernal\Letters.h"

class StlLoader : public Kernal::MessageHandlerClient {
public:
	StlLoader();
	~StlLoader();
protected:
private:
	void cb(Kernal::MessageHandlerArg* arg);
};

namespace STL {
struct STLHeader {
public:
	char id[22];
	char date[26];
	float xmin, xmax; // The x, y and z bounds on the data.
	float ymin, ymax;
	float zmin, zmax;
	float xpixelsize; // Dimensions of grid for this model
	float ypixelsize; // in user units.
	Kernal::uint32 nfacets;
//private: //I want this to be private but WTF it no work
//	friend class StlLoader;
	void fixEndian();
};
class STLVertex {
public:
	float x;
	float y;
	float z;
	void fixEndian();
};
class STLFacet {
public:
	STLVertex normal; // facet surface normal
	STLVertex v1; // vertex 1
	STLVertex v2; // vertex 2
	STLVertex v3; // vertex 3
//private:
//	friend class StlLoader;
	void fixEndian();
};
} //STL namespace

namespace Kernal {
namespace Letters {
class StlDataLetter: public Letter {
public:
	static std::string ID;
	StlDataLetter();
	~StlDataLetter();
	void clearFacets();
	STL::STLHeader* header;
	STL::STLFacet** facets;
};

class StlLoadLetter: public Letter {
public:
	static std::string ID;
	StlLoadLetter() {
	}
	~StlLoadLetter() {
	}
	std::string filename;
};
} // Letters namespace
} // Kernal namespace

#endif /* __STLLOADER_H__ */
