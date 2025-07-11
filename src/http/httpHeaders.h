/* Http helpers */
#define END_HEADERS "\r\n\r\n"
#define LENGTH_HEADER "Content-Length: "
#define HEADER_END "\r\n"

/* Http content builders */

// Http response
#define HTTP_OK "HTTP/1.1 200 OK\r\n"
#define HTTP_NOT_FOUND "HTTP/1.1 404 Not Found\r\n"
#define HTTP_INTERNAL_ERROR "HTTP/1.1 500 Internal Server Error\r\n"

// Http Content type
#define CONTENT_TYPE_TXT "Content-Type: text/plain\r\n"
#define CONTENT_TYPE_HTML "Content-Type: text/html\r\n"
#define CONTENT_TYPE_JSON "Content-Type: application/json\r\n"
#define CONTENT_TYPE_JPEG "Content-Type: image/jpeg\r\n"
#define CONTENT_TYPE_PNG "Content-Type: image/png\r\n"
#define CONTENT_TYPE_CSS "Content-Type: text/css\r\n"
#define CONTENT_TYPE_JS "Content-Type: application/javascript\r\n"
#define CONTENT_TYPE_ICO "Content-Type: image/x-icon\r\n"

// Complete messages
#define HTTP_404_NOT_FOUND_TEXT "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\n"
#define HTTP_500_INTERNAL_ERROR_TEXT "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\n"

// Access control
#define ACCESS_CONTROL_ALLOW "Access-Control-Allow-Origin: *\r\n"

// Paths
#define DEFAULT_PATH "index.html"

// Api requests
#define API_STATUS_OK R"({"status":"ok"})"