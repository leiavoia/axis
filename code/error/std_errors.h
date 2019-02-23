/*******************************************

STANDARD ERROR CLASSES
==========================
version 0.0.1

*******************************************/




// GENERIC ERROR
// reports a simple string message
class err_generic {
public:
	char msg[75]; // the error report string message
	//constructor - initializes with report string
	err_generic(char message[]) {
		strcpy(msg, message);
		}
}


