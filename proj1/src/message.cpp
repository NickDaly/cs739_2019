#include "message.h"

message::message() {
	header_.id = 0;
}

message::~message() {
	
}

int64_t message::get_param() const  {
	return header_.param;
}

void message::set_param(int64_t param) {
	header_.param = param;
}

void message::set_key_size(int len) {
	header_.key_size = len;
}

int message::get_key_size() const {
	return header_.key_size;
}

const char *message::key() {
	return payload;
}

const char *message::value() {
	return payload + header_.key_size + 1;
}