#pragma once

//	1xx INFORMATIONAL RESPONSE	//

# define CODE_100 100 // "Continue"

# define CODE_101 101 // "Switching protocols"

# define CODE_102 102 // "Processing" WebDAV request which may contain many sub-requests with file operations.

# define CODE_103 103 // "Early hints"

//	2xx SUCCESS	 //

# define CODE_200 200 // OK

# define CODE_201 201 // REQUEST CREATED

# define CODE_202 202 // REQUEST ACCEPTED

# define CODE_203 203 // Non-Authoritative Information

//	3XX REDIRECTION	 //

# define CODE_307 307 // TEMPORARY REDIRECT

# define CODE_308 308 // PERMANENT REDIRECT

//	4XX CLIENT ERRORS	//

# define CODE_400 400 // BAD REQUEST

# define CODE_403 403 // FORBIDDEN

# define CODE_404 404 // NOT FOUND

# define CODE_405 405 // METHOD NOT ALLOWED

# define CODE_408 408 // REQUEST TIMEOUT

//	5XX SERVER ERRORS	//

# define CODE_500 500 // INTERNAL SERVER ERROR

# define CODE_502 502 // BAD GATEWAY