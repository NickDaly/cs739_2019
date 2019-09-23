#include "message.h"
#include <assert.h>
#include <cstring>

#define MAX_KEY_SIZE   128
#define MAX_VALUE_SIZE 2048

message::message() {
	header_.id = 0;
	header_.cmd = command::NONE;
}

message::message(command cmd) {
	header_.id = 0;
	header_.cmd = cmd;
}

message::~message() {

}

command message::get_command() const {
	return header_.cmd;
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


void message::set_value_size(int len) {
	header_.value_size = len;
}


int message::get_value_size() const {
	return header_.value_size;
}


void message::set_key(const char* key) {
	auto len = strlen(key);	
	assert(len>0 && len<=MAX_KEY_SIZE);
	set_key_size(len);
	memcpy((void*) payload, (void*) key, len);
	payload[len] = 0;	
}


void message::set_value(const char* val, int len) {
	assert(len>=0 && len<MAX_VALUE_SIZE );
	set_value_size(len);
	memcpy((void*) value(), (void*) val, len);
}


const char *message::key() {
	return payload;
}


const char *message::value() {
	return payload + header_.key_size + 1;
}