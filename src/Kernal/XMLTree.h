#ifndef __XML_TREE_H__
#define __XML_TREE_H__

#include <vector>
#include <map>
#include <sstream>

#include "FileInput.h"

namespace Kernal {

class XMLTree;

typedef std::vector<XMLTree*> XMLTreeVector;

class XMLTree {
public:
	XMLTree();
	XMLTree(std::string name_In, XMLTreeVector* children_In,
			std::vector<std::string>* data_In);
	XMLTree(std::string filename_In);
	~XMLTree();

	bool IsValid(); //!< Returns whether the tree is valid or not.
	std::string GetName(); //!< The name of the Tree
//    XMLTreeVector GetChildren(); //!< A vector to the children of the Tree
	bool HasChildren(); //!< Whether children in this node exist.
	bool HasChild(std::string name_In); //!< Whether one and only one child exists.
	XMLTreeVector* GetChildren(std::string name_In); //!< Get only the children be a certain name.
	bool HasChildren(std::string name_In); //!< Whether children of a certain name exist.
	void AddChild(std::string name_In); //!< Adds a new child node to the current node.
	void AddChild(XMLTree* tree_In); //!< Adds the tree as a child of the current node.
	void AddChildren(XMLTreeVector* tree_In); //!< Adds the trees as children of the current node.
	void RemoveChildren(std::string name_In); //!< Removes all children of name from the current nodes children.
	void ClearChildren(); //!< Removes all children from he current node.

	std::string GetAttribute(std::string attrib_In); //!< Gets the attribute of the node by a certain name.
	bool HasAttribute(std::string attrib_In); //!< Whether a node has an attribute by a certain name.
	void AddAttribute(std::string attrib_In, std::string value_In); //!< Adds the value to the attribute in the node if one doesn't already exist.
	void RemoveAttribute(std::string attrib_In); //!< Removes the attribute in the node if one doesn't already exist.
	void ClearAttribute(); //!< Removes all attributes from the node.

	std::vector<std::string>* GetData(); //!< Gets the data stored in the node
	bool HasData(); //!< Whether this node has data.
	void AddData(std::vector<std::string>* data_In); //!< Appends the data to the data already stored in the node
	void ClearData(); //!< Clears the data stored in the node

	void WriteTree(std::string filename); //!< Write the XMLTree to a file.
	std::string WriteTree(); //!< Write the XMLTree to a string.
	void Print(std::ostringstream *oss = NULL, int i_indent = 0); //!< Print out the XMLTree
private:
	XMLTree(FileInput *file_In);
	bool Fill(FileInput *file_In);
	void WriteTree(std::iostream *os, int i_indent = 0);
	std::string m_s_name;
	std::map<std::string, XMLTreeVector*> m_msx_children;
	std::map<std::string, std::string> m_mss_attributes;
	std::vector<std::string>* m_vp_data;
	bool m_b_valid;
	bool m_use_white_space;
};
} //namespace Kernal
#endif

