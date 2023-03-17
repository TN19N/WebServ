// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 3/16/23 at 12:28 AM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

#pragma once

# define BUFFER_SIZE 		5120

# define WEB_SERV_RUNNING 0
# define WEB_SERV_STOPPING 1

# define CLOSE_CONNECTION   false
# define KEEP_ALIVE         true

# define READ_END 	0
# define WRITE_END 	1

// client state
enum { 
	READING_REQUEST,
	SENDING_RESPONSE,
	READING_RESPONSE,
	SENDING_REQUEST//,
	// WRITE_BODY_TO_CGI,
	// UPLOADING_FILE,
	// DOWNLOADING_FILE, 
	// READING_HEADERS,
	// READING_BODY, 
	// BODY_READY 
};

// request and response state
enum {
	CREATING,
	READY
};

# define END_CHUNKED 	"0\r\n\r\n"
# define END_HEADERS 	"\r\n\r\n"
# define CRLF 			"\r\n"

