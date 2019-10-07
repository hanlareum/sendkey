/* 

 Copyright 2018 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <map>
#include <vector>
#include <memory>
#include <string>

#ifndef SENDKEY_SRC_DATABASE_H
#define SENDKEY_SRC_DATABASE_H

namespace jethro {

class Database {
public:
	static std::shared_ptr<Database> create(std::string path = "");

	long findByName(std::string name);
	std::string findByCode(long code);
	
	std::map<std::string, long>::iterator begin();
	std::map<std::string, long>::iterator end();

private:
	Database(std::string path);

	bool initialize();

	void insert(std::string k, std::string v);

	std::string m_path;

	std::map<std::string, long> m_db;
}; /*Database*/

} /*jethro*/

#endif /*SENDKEY_SRC_DATABASE_H*/
