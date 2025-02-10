#!/usr/bin/python3
import os
import sys

# Read query string (for GET requests)
query_string = os.environ.get("QUERY_STRING", "")

# Read POST data from stdin
content_length = int(os.environ.get("CONTENT_LENGTH", 0))
post_data = sys.stdin.read(content_length) if content_length > 0 else ""

# Output HTTP headers and response
print("Content-Type: text/html\n")
print("<html><body>")
print("<h1>CGI Script Response</h1>")
print(f"<p>GET Data: {query_string}</p>")
print(f"<p>POST Data: {post_data}</p>")
print("</body></html>")
