#ifndef MESSAGE_H
#define MESSAGE_H
class message {
public:
	message();
	virtual ~message();
	int length();
	const char *payload();
};
#endif