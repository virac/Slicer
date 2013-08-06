/*
 * LithographHandler.cpp
 *
 *  Created on: Jun 5, 2013
 *      Author: Jon
 */

#include "LithographHandler.h"

#include <set>

#include "Kernal\Log.h"

#include "StlLoader.h"

//<sends>
std::string Kernal::Letters::LithographicFrameCreationLetter::ID =
		"stl_data_loaded";
//</sends>

LithographHandler::LithographHandler() {
	LOG("LithographHandler::LithographHandler");LOG_DEBUG("Registering messages");
	std::vector<std::string> messages;
	messages.push_back(Kernal::Letters::StlDataLetter::ID);

	this->Init(messages);
}

LithographHandler::~LithographHandler() {
}

void LithographHandler::cb(Kernal::MessageHandlerArg* arg) {
	LOG("LithographHandler::cb");
	if (arg == NULL) {
		LOG_ERROR("NULL message received.");
		return;
	}

	Kernal::Letters::LithographicFrameCreationLetter *lithoFrameLetter = NULL;
	double objHeight = 0.0;
	double layerHeight = 0.0;
	double xCenter = 0.0;
	double yCenter = 0.0;
	double zLayerOffset = 0.0;
	Math::TrianglePtrArray* triangles;
	triangles = new Math::TrianglePtrArray();

	unsigned int send_id = 0;
	if (arg->message_type
			== Kernal::GetMessage(Kernal::Letters::StlDataLetter::ID)) {
		send_id = Kernal::GetMessage(
				Kernal::Letters::LithographicFrameCreationLetter::ID);
		Kernal::Letters::StlDataLetter *stlDataLetter =
				reinterpret_cast<Kernal::Letters::StlDataLetter*>(arg->letter);

		xCenter = stlDataLetter->header->xmax - stlDataLetter->header->xmin;
		yCenter = stlDataLetter->header->ymax - stlDataLetter->header->ymin;
		zLayerOffset = stlDataLetter->header->zmin;
		objHeight = stlDataLetter->header->zmax - stlDataLetter->header->zmin;

		for (unsigned int i = 0; i < stlDataLetter->header->nfacets; i++) {
			std::set<Math::Vector*, Math::VectorPointerLess> vectorSet;
			STL::STLFacet *facet = stlDataLetter->facets[i];
			Math::Vector *v1 = new Math::Vector(facet->v1.x, facet->v1.y,
					facet->v1.z);
			Math::Vector *v2 = new Math::Vector(facet->v2.x, facet->v2.y,
					facet->v2.z);
			Math::Vector *v3 = new Math::Vector(facet->v3.x, facet->v3.y,
					facet->v3.z);

			std::pair<std::set<Math::Vector*>::iterator, bool> v1pair;
			std::pair<std::set<Math::Vector*>::iterator, bool> v2pair;
			std::pair<std::set<Math::Vector*>::iterator, bool> v3pair;

			v1pair = vectorSet.insert(v1);
			v2pair = vectorSet.insert(v2);
			v3pair = vectorSet.insert(v3);

			if (!v1pair.second) {
				delete v1;
			}
			if (!v2pair.second) {
				delete v2;
			}
			if (!v3pair.second) {
				delete v3;
			}

			Math::Triangle *tri = new Math::Triangle(*v1pair.first, *v2pair.first,
					*v3pair.first);

			triangles->push_back(tri);
		}
	} else {
		LOG_ERROR(
				"Received unknown message, this client can only receive \""
						+ Kernal::Letters::StlDataLetter::ID + "\" letters.");
		return;
	}

	lithoFrameLetter = new Kernal::Letters::LithographicFrameCreationLetter();
	lithoFrameLetter->triangles = triangles;
	lithoFrameLetter->distOffset = zLayerOffset;
	lithoFrameLetter->planeNormal = new Math::Vector(0, 0, 1);
	int j = 0;
	for (double i = 0.0; i < objHeight; i += layerHeight) {
		lithoFrameLetter->frameId = j++;
		lithoFrameLetter->layer = i;

		Kernal::MessageHandlerArg *send_arg = new Kernal::MessageHandlerArg();
		send_arg->message_type = send_id;
		send_arg->priority = 1;
		send_arg->blocking = 1;

		send_arg->letter = lithoFrameLetter;

		this->Send(send_arg);
	}
}
