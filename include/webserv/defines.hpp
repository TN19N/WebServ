// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 3/16/23 at 12:28 AM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

#pragma once

# define BUFFER_SIZE 		5120 // should not bypass (MAX_SIZE_OF_PIPE / 5) = (65536 / 5 = 13107)

# define WEB_SERV_RUNNING 0
# define WEB_SERV_STOPPING 1

# define CLOSE_CONNECTION   false
# define KEEP_ALIVE         true

# define READ_END 	0
# define WRITE_END 	1

# define TIMEOUT 10000 // on millisecond

// client state
enum { 
	FROM_CLIENT,
	TO_CLIENT,
	FROM_CGI,
	TO_CGI
};

// request and response state
enum {
	CREATING,
	READY
};

# define END_CHUNKED 	"0\r\n\r\n"
# define END_HEADERS 	"\r\n\r\n"
# define CRLF 			"\r\n"

