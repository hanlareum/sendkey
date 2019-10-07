/* 

 Copyright 2018-2019 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <list>
#include <string>
#include <memory>

#ifndef SENDKEY_SRC_QUEUE_H
#define SENDKEY_SRC_QUEUE_H

namespace jethro {

class Queue {
public:
	Queue();

	void push(std::string name, std::string code);
	void push(std::pair<std::string, std::string> &key);
	std::pair<std::string, std::string> pop();

	std::list<std::pair<std::string, std::string> >::iterator first();
	std::list<std::pair<std::string, std::string> >::iterator next();
	std::list<std::pair<std::string, std::string> >::iterator last();
	bool hasMore();

	int count();

private:
	std::list<std::pair<std::string, std::string> > m_queue;
	std::list<std::pair<std::string, std::string> >::iterator m_iter;
}; /*KeyEventQueue*/

} /*jethro*/

#endif /*SENDKEY_SRC_QUEUE_H*/
