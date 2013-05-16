#include "XMLTree.h"
#include "Log.h"
#include "StringManip.h"
namespace Kernal {

XMLTree::XMLTree() :
		m_s_name(""), m_vp_data(NULL), m_b_valid(false) {
}

XMLTree::XMLTree(std::string name_In, XMLTreeVector* children_In,
		std::vector<std::string>* data_In) :
		m_s_name(name_In), m_vp_data(data_In), m_b_valid(true) {
	if (children_In != NULL) {
		for (XMLTreeVector::iterator j = children_In->begin();
				j != children_In->end(); j++) {
			if (this->m_msx_children[(*j)->GetName()] == NULL) {
				this->m_msx_children[(*j)->GetName()] = new XMLTreeVector;
			}
			this->m_msx_children[(*j)->GetName()]->push_back(*j);
		}
	}
}

XMLTree::XMLTree(std::string filename_In) :
		m_s_name(""), m_vp_data(NULL), m_b_valid(false) {
	LOG("XMLTree::XMLTree");
	FileInput *in;
	in = new FileInput(filename_In);
	LOG_WRITE("Got " + filename_In);
	if (!in->IsValid()) {
		LOG_ERROR("Bad filename, " + filename_In + " is not a valid file.");
		return;
	}

	LOG_WRITE("And it is good");
	std::string s_token;

	in->SetCommentBlock("<!--", "-->");
	in->SetDeliminators(" \n");
	in->SetSeparators("<\"", "\">");
	s_token = in->GetToken();
	if ("<?xml" == s_token) {
		do {
			s_token = in->GetToken();
			if (s_token == "\"")
				continue;
			if (EndsWith(s_token, "?>")) {
				break;
			} else if (s_token == "version=") {
				if (in->GetToken() != "\"" || in->GetToken() != "1.0"
						|| in->GetToken() != "\"") {
					LOG_ERROR("Bad version");
				} else {
					this->m_b_valid = true;
				}
			} else if (s_token == "encoding=") {
				if (in->GetToken() != "\"" || in->GetToken() != "UTF-8"
						|| in->GetToken() != "\"") {
					LOG_ERROR("Bad encoding");
				} else {
					this->m_b_valid = true;
				}
			} else {
				LOG_WARN("Found Unknown token " + s_token + " ignoring.");
			}
		} while (s_token != "?>" && in->IsValid());
		if (this->m_b_valid)
			this->m_b_valid = Fill(in);
		delete in;
	} else {
		LOG_ERROR("No <?xml tag found");
		this->m_b_valid = false;
		return;
	}
}

XMLTree::XMLTree(FileInput *file_In) :
		m_s_name(""), m_vp_data(NULL), m_b_valid(true) {
	this->m_b_valid = Fill(file_In);
}

XMLTree::~XMLTree() {
	if (this->m_vp_data != NULL) {
		delete this->m_vp_data;
		this->m_vp_data = NULL;
	}
	if (this->m_msx_children.size() != 0) {
		XMLTreeVector *temp_tree_vec = NULL;
		XMLTree *temp_tree = NULL;
		for (std::map<std::string, XMLTreeVector*>::iterator i =
				this->m_msx_children.begin(); i != this->m_msx_children.end();
				i++) {
			temp_tree_vec = i->second;
			for (XMLTreeVector::iterator j = temp_tree_vec->begin();
					j != temp_tree_vec->end(); j++) {
				temp_tree = *j;
				delete temp_tree;
			}
			delete temp_tree_vec;
		}
		this->m_msx_children.clear();
	}
}
//! NOTE: This function assumes that the XMLTree is correctly formatted.
bool XMLTree::Fill(FileInput *file_In) {
	LOG("XMLTree::XMLTree");
	std::string s_token;
	std::string s_temp;

	XMLTree *node;
	std::vector<std::string> toks;

	s_token = file_In->GetToken();
	this->m_s_name = Strip(s_token, "</>");
	LOG_DEBUG("Trying to parse tag " + this->m_s_name);

	if (EndsWith(s_token, "/>")) {
		// we got a "<node/>"
		return this->m_b_valid = true;
	} else if (!EndsWith(s_token, ">")) {
		// we got "<node" so there *may* be attributes
		s_token = file_In->GetToken();
		if (!EndsWith(s_token, ">")) {
			// we got attributes
			do {
				s_temp = Strip(s_token, "=");
				if (!EndsWith(s_token, "=")) {
					LOG_ERROR(
							"There was somthing wrong with this node, \""
									+ this->m_s_name + "\" was expecting and \""
									+ s_temp + "=\" but got " + s_token);
					return this->m_b_valid = false;
				} else {
					s_token = file_In->GetToken();
					if (s_token == "\"") {
						std::string buff = "";
						this->m_mss_attributes[s_temp] = "";
						do {
							s_token = file_In->GetToken();
							this->m_mss_attributes[s_temp] += buff
									+ Strip(s_token, "\"");
							buff = " ";
							s_token = file_In->PeekToken();
						} while (s_token != "\"");
						s_token = file_In->GetToken();
					} else {
						LOG_ERROR(
								"There was somthing wrong with this node, \""
										+ this->m_s_name + "\" on attribute \""
										+ s_temp
										+ "\" was expecting and \" but got "
										+ s_token);
						return this->m_b_valid = false;
					}
				}
				s_token = file_In->GetToken();
			} while (s_token != "/>" && s_token != ">");
		}
		// we got a > or />
		if (s_token == "/>") {
			return this->m_b_valid = true;
		}
	}
	// this is a "<node ... >" so there may be data or child-nodes
	s_token = file_In->PeekToken();
	for (; file_In->IsValid() && !StartsWith(s_token, "</");
			s_token = file_In->PeekToken()) {
		// until we find "</..." we keep reading
		if (StartsWith(s_token, "<")) {
			// we found a child-node
			LOG_DEBUG("Trying to get child " + Strip(s_token, "</>"));
			node = new XMLTree(file_In);
			if (node->IsValid() == false) {
				// the child tree was invalid so we bail
				delete node;
				return this->m_b_valid = false;
			}
			if (this->m_msx_children[node->GetName()] == NULL) {
				this->m_msx_children[node->GetName()] = new XMLTreeVector;
			}
			this->m_msx_children[node->GetName()]->push_back(node);
			node = NULL;
		} else {
			// we found data
			LOG_DEBUG("Got data \"" + file_In->PeekToken() + "\"");
			if (this->m_vp_data == NULL) {
				this->m_vp_data = new std::vector<std::string>;
			}
			this->m_vp_data->push_back(file_In->GetToken());
		}
	}
	if (file_In->IsValid() == false) {
		LOG_ERROR(
				"Ran out of stream data when trying to process node "
						+ this->m_s_name + " no end tag found");
		return this->m_b_valid = false;
	}
	//we have found the "</..."
	s_token = file_In->GetToken();
	if (this->m_s_name != Strip(s_token, "</>")) {
		// argh we failed
		LOG_ERROR("Argh the token didnt match what i wanted.");
		return false;
	} else {
		if (EndsWith(s_token, ">"))
			return this->m_b_valid = true;
		else {
			do {
				s_token = file_In->GetToken();
			} while (!EndsWith(s_token, ">") && file_In->IsValid());
			return this->m_b_valid = file_In->IsValid();
		}
	}
}

bool XMLTree::IsValid() {
	return this->m_b_valid;
}

std::string XMLTree::GetName() {
	return this->m_s_name;
}

bool XMLTree::HasChildren() {
	return !this->m_msx_children.empty();
}

bool XMLTree::HasChild(std::string name_In) {
	if (this->m_msx_children.find(name_In) != this->m_msx_children.end())
		return this->m_msx_children[name_In]->size() == 1;
	else
		return false;
}

void XMLTree::AddChild(std::string name_In) {
	if (this->m_msx_children[name_In] == NULL) {
		this->m_msx_children[name_In] = new XMLTreeVector;
	}
	this->m_msx_children[name_In]->push_back(new XMLTree(name_In, NULL, NULL));
}

void XMLTree::AddChild(XMLTree *tree_In) {
	if (this->m_msx_children[tree_In->GetName()] == NULL) {
		this->m_msx_children[tree_In->GetName()] = new XMLTreeVector;
	}
	this->m_msx_children[tree_In->GetName()]->push_back(tree_In);
}

void XMLTree::AddChildren(XMLTreeVector *tree_In) {
	for (XMLTreeVector::iterator i = tree_In->begin(); i != tree_In->end();
			i++) {
		this->AddChild(*i);
	}
}

XMLTreeVector* XMLTree::GetChildren(std::string name_In) {
	if (this->m_msx_children.find(name_In) != this->m_msx_children.end())
		return this->m_msx_children[name_In];
	else
		return NULL;
}

bool XMLTree::HasChildren(std::string name_In) {
	return (this->m_msx_children.find(name_In) != this->m_msx_children.end());
}

void XMLTree::RemoveChildren(std::string name_In) {
	std::map<std::string, XMLTreeVector*>::iterator iter =
			this->m_msx_children.find(name_In);
	if (iter != this->m_msx_children.end()) {
		this->m_msx_children.erase(iter);
	}
}

void XMLTree::ClearChildren() {
	if (this->m_msx_children.size() != 0) {
		XMLTreeVector *temp_tree_vec;
		for (std::map<std::string, XMLTreeVector*>::iterator i =
				this->m_msx_children.begin(); i != this->m_msx_children.end();
				i++) {
			temp_tree_vec = i->second;
			delete temp_tree_vec;
		}
		this->m_msx_children.clear();
	}
}

//XMLTreeVector XMLTree::GetChildren()
//{
//    XMLTreeVector toReturn;
//    if( this->m_msx_children.size() != 0 )
//    {
//        for( std::map<std::string,XMLTreeVector*>::iterator i = this->m_msx_children.begin(); i != this->m_msx_children.end(); i++ )
//        {
//            toReturn->push_back(i->second);
//        }
//    }
//    return toReturn;
//}

std::string XMLTree::GetAttribute(std::string attrib_In) {
	if (this->m_mss_attributes.find(attrib_In) != this->m_mss_attributes.end())
		return this->m_mss_attributes[attrib_In];
	else
		return NULL;
}

bool XMLTree::HasAttribute(std::string attrib_In) {
	return (this->m_mss_attributes.find(attrib_In)
			!= this->m_mss_attributes.end());
}

void XMLTree::AddAttribute(std::string attrib_In, std::string value_In) {
	this->m_mss_attributes[attrib_In] = value_In;
}

void XMLTree::RemoveAttribute(std::string attrib_In) {
	this->m_mss_attributes.erase(attrib_In);
}

void XMLTree::ClearAttribute() {
	this->m_mss_attributes.clear();
}

std::vector<std::string>* XMLTree::GetData() {
	return this->m_vp_data;
}

bool XMLTree::HasData() {
	if (this->m_vp_data != NULL)
		return (this->m_vp_data->size() != 0);
	else
		return false;
}

void XMLTree::AddData(std::vector<std::string> *data_In) {
	if (this->m_vp_data == NULL) {
		this->m_vp_data = new std::vector<std::string>;
	}
	for (std::vector<std::string>::iterator i = data_In->begin();
			i != data_In->end(); i++) {
		this->m_vp_data->push_back(*i);
	}
}

void XMLTree::ClearData() {
}

void XMLTree::WriteTree(std::string filename) {
	LOG("XMLTree::WriteTree:file");
	std::fstream os; // = new std::fstream;
	os.open(filename.c_str(), std::fstream::out | std::fstream::trunc);
	if (os.bad()) {
		return;
	}

	os << "<?xml version=\"1.0\"?>" << std::endl;
	this->WriteTree(&os);

	os.close();
	//delete os;
}

std::string XMLTree::WriteTree() {
	LOG("XMLTree::WriteTree:string");
	std::stringstream os;
	if (os.bad()) {
		return "";
	}

	os << "<?xml version=\"1.0\"?>" << std::endl;
	this->WriteTree(&os);

	return os.str();
	//delete os;
}

void XMLTree::WriteTree(std::iostream *os, int i_indent) {
	LOG("XMLTree::WriteTree:stream");
	for (int i = 0; i < i_indent; i++) {
		(*os) << '\t';
	}
	(*os) << "<" << this->m_s_name;

	for (std::map<std::string, std::string>::iterator j =
			this->m_mss_attributes.begin(); j != this->m_mss_attributes.end();
			j++) {
		(*os) << " " << j->first << "=\"" << j->second << "\"";
	}

	if (!this->HasData() && !this->HasChildren()) {
		(*os) << "/>" << std::endl;
	} else {
		(*os) << ">";
		if (this->HasData()) {
			bool first = true;
			for (std::vector<std::string>::iterator j =
					this->m_vp_data->begin(); j != this->m_vp_data->end();
					j++) {
				if (first == false) {
					(*os) << std::endl;
				}
				(*os) << (*j);
				first = false;
			}
		}
		if (this->HasChildren()) {
			(*os) << std::endl;
			XMLTreeVector *temp_tree_vec;
			for (std::map<std::string, XMLTreeVector*>::iterator i =
					this->m_msx_children.begin();
					i != this->m_msx_children.end(); i++) {
				temp_tree_vec = i->second;
				for (XMLTreeVector::iterator j = temp_tree_vec->begin();
						j != temp_tree_vec->end(); j++) {
					(*j)->WriteTree(os, i_indent + 1);
				}
			}
		}

		for (int i = 0; i < i_indent; i++) {
			(*os) << '\t';
		}
		(*os) << "</" << this->m_s_name << ">" << std::endl;

	}
}

void XMLTree::Print(std::ostringstream *oss, int i_indent) {
	LOG("XMLTree::Print");
	bool first = false;
	if (oss == NULL) {
		oss = new std::ostringstream();
		first = true;
	}
	for (int i = 0; i < i_indent; i++) {
		(*oss) << '\t';
	}
	(*oss) << this->m_s_name << "\t";
	for (std::map<std::string, std::string>::iterator j =
			this->m_mss_attributes.begin(); j != this->m_mss_attributes.end();
			j++) {
		(*oss) << "  -" << j->first << " = " << j->second;
	}
	(*oss) << std::endl;
	if (this->HasData()) {
		for (std::vector<std::string>::iterator j = this->m_vp_data->begin();
				j != this->m_vp_data->end(); j++) {
			for (int i = 0; i < i_indent + 2; i++) {
				(*oss) << '\t';
			}
			(*oss) << (*j) << std::endl;
		}
	}
	if (this->HasChildren()) {
		XMLTreeVector *temp_tree_vec;
		for (std::map<std::string, XMLTreeVector*>::iterator i =
				this->m_msx_children.begin(); i != this->m_msx_children.end();
				i++) {
			temp_tree_vec = i->second;
			for (XMLTreeVector::iterator j = temp_tree_vec->begin();
					j != temp_tree_vec->end(); j++) {
				(*j)->Print(oss, i_indent + 1);
			}
		}
	}

	if (first == true) {
		LOG_DEBUG("\n" + oss->str());
//TODO There is something wrong here. delete never removes its handle from the memory manager.
		delete oss;
	}
	return;
}
}

