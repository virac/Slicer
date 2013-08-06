/*
 * StlLoader.cpp
 *
 *  Created on: May 18, 2013
 *      Author: Jon
 */

#include "Kernal/Log.h"
#include "StlLoader.h"
#include "Kernal/Lock.h"
#include "Kernal/StringManip.h"
#include "Kernal/Endian.h"

//<sends>
std::string Kernal::Letters::StlDataLetter::ID = "stl_data_loaded";
//</sends>

//<receives>
std::string Kernal::Letters::StlLoadLetter::ID = "load_stl_data";
//</receives>
Kernal::Letters::StlDataLetter::StlDataLetter() {
	Kernal::InitEndian();
	this->header = new STL::STLHeader();
	this->facets = NULL;
}
Kernal::Letters::StlDataLetter::~StlDataLetter() {
	clearFacets();
	if (header != NULL) {
		delete header;
	}
}

void Kernal::Letters::StlDataLetter::clearFacets() {
	int numFacets = 0;
	if( header != NULL ) {
		numFacets = header->nfacets;
	}
	if( numFacets != 0 ) {
		if( facets != NULL ) {
			for( int i = 0; i < numFacets; i++ ) {
				if( facets[i] != NULL ) {
					delete facets[i];
				}
			}
		}
	}
	if( facets != NULL ) {
		delete[] facets;
	}
}

StlLoader::StlLoader() {
	LOG("StlLoader::StlLoader");LOG_DEBUG("Registering messages");
	std::vector<std::string> messages;
	messages.push_back(Kernal::Letters::StlLoadLetter::ID);

	this->Init(messages);
}

StlLoader::~StlLoader() {

}

void StlLoader::cb(Kernal::MessageHandlerArg* arg) {
	LOG("StlLoader::cb");
	if (arg == NULL) {
		LOG_ERROR("NULL message received.");
		return;
	}

	unsigned int send_id = 0;
	if (arg->message_type
			== Kernal::GetMessage(Kernal::Letters::StlLoadLetter::ID)) {
		send_id = Kernal::GetMessage(Kernal::Letters::StlDataLetter::ID);
	} else {
		LOG_ERROR(
				"Received unknown message, this client can only receive \""
						+ Kernal::Letters::StlLoadLetter::ID + "\" letters.");
	}
	if (send_id != 0) {
		Kernal::Lock lock(arg->letter->letterMutex);
		Kernal::Letters::StlDataLetter *stlDataLetter =
				new Kernal::Letters::StlDataLetter();
		std::string filename = "";
		Kernal::Letters::StlLoadLetter *stlLoadLetter =
				reinterpret_cast<Kernal::Letters::StlLoadLetter*>(arg->letter);
		filename = stlLoadLetter->filename;
		std::ifstream is;
		is.open(filename.c_str(), std::ios::binary);

		is.seekg(0, std::ios::end);
		int length = is.tellg();
		is.seekg(0, std::ios::beg);
		int read = 0;
		is.read((char*) stlDataLetter->header, sizeof(STL::STLHeader));
		stlDataLetter->header->fixEndian();

		int facets = stlDataLetter->header->nfacets;
		stlDataLetter->facets = new STL::STLFacet*[facets];
		char pad[2];
		for( int i = 0; i < facets && !is.eof(); i++ ) {
			stlDataLetter->facets[i] = new STL::STLFacet();
			is.read((char*) stlDataLetter->facets[i], sizeof(STL::STLHeader));
			stlDataLetter->facets[i]->fixEndian();
			is.read(pad,2);
		}
		LOG_WRITE("Finished parsing file.");

		Kernal::MessageHandlerArg *send_arg =
				new Kernal::MessageHandlerArg();
		send_arg->message_type = send_id;
		send_arg->priority = 1;
		send_arg->blocking = 1;

		send_arg->letter = stlDataLetter;

		this->Send(send_arg);
		is.close();
	}
}

void STL::STLVertex::fixEndian() {
	this->x = Kernal::FromBigFloat(this->x);
	this->y = Kernal::FromBigFloat(this->y);
	this->z = Kernal::FromBigFloat(this->z);
}

void STL::STLFacet::fixEndian() {
	this->normal.fixEndian();
	this->v1.fixEndian();
	this->v2.fixEndian();
	this->v3.fixEndian();
}

void STL::STLHeader::fixEndian() {
	this->xmin = Kernal::FromBigFloat(this->xmin);
	this->xmax = Kernal::FromBigFloat(this->xmax);
	this->ymin = Kernal::FromBigFloat(this->ymin);
	this->ymax = Kernal::FromBigFloat(this->ymax);
	this->zmin = Kernal::FromBigFloat(this->zmin);
	this->zmax = Kernal::FromBigFloat(this->zmax);

	this->xpixelsize = Kernal::FromBigFloat(this->xpixelsize);
	this->ypixelsize = Kernal::FromBigFloat(this->ypixelsize);

	this->nfacets = Kernal::FromBigInt(this->nfacets);
}
/*
 *
 *
 Number scientific notation : sci_num_RE = ((-?[0-9].[0-9]{6})E([+-]?[0-9]{2}))
 Valid File Name : filename = ([0-9a-zA-Z\._-]+.(bin|raw)) solid_start = "(solid)" + filename facet = "(facet) (normal)" + sci_num_RE  +

 sci_num_RE  + sci_num_RE outer_loop = "(outer loop)"

 3x vertex = "(vertex)" + sci_num_RE  + sci_num_RE  + sci_num_RE

 end_loop = "(endloop)"
 endfacet = "(endfacet)"
 ...

 solid_end = "(endsolid)" + filename
 */
