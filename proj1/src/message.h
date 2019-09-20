#ifndef MESSAGE_H
#define MESSAGE_H

#include <chrono>
#include <cstdint>
#include <cctype>

#define MSG_SIZE 4096

typedef struct header {
	int64_t id;
	int32_t command;
	int64_t param;
	int32_t key_size;
	int32_t value_size;
} header_t;


class message {
public:
	message();
	~message();
	
	int64_t get_param() const;
	void set_param(int64_t param);
	
	int  get_key_size() const;
	void set_key(const char* key);
	
	int  get_value_size() const;
	void set_value(const char* value, int len);

	//returns the pointer to the key string
	const char *key();
	
	//returns the pointers to the value
	const char *value();

private:
	void set_key_size(int len);
	void set_value_size(int len);

	header_t header_;
	char payload[MSG_SIZE-sizeof(header_t)];	
};
#endif