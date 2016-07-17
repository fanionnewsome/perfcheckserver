// PerfCheckServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string.h>
#include "Lib\mongoose\include\mongoose.h"
using namespace std;
#define NS_HTTP_REQUEST 100 /* struct http_message * */

static const char *http_port = "8000";
static struct mg_serve_http_opts http_server_options;

//void sendEmailAlert(const string& emailAddr, const string& body, const string& subject = "PerfCheck - Email Alert") {

//}

// memory=1235132&cpu=90perc&disk=50perc
static void handle_perfcheck_call(struct mg_connection *connection, struct http_message *hm) 
{
	char memory[120], cpu[120], disk[120];
	printf("message with dot p: %s\n" , hm->query_string.p);
	// GET form variables 
	mg_get_http_var(&hm->query_string, "memory", memory, sizeof(memory));
	mg_get_http_var(&hm->query_string, "cpu", cpu, sizeof(cpu));
	mg_get_http_var(&hm->query_string, "disk", disk, sizeof(disk));

	
	printf("memory: %s\r\n", memory);
	printf("cpu: %s\r\n", cpu);
	printf("disk: %s\r\n", disk);

	// Send Headers
	mg_printf(connection, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

	// send empty chunk, indicate end of response
	mg_send_http_chunk(connection, "", 0); 
}

static void ev_handler(struct mg_connection *connection, int ev, void *ev_data) 
{
	struct http_message *message = (struct http_message *) ev_data;

	switch (ev) 
	{
		case NS_HTTP_REQUEST:
			if (mg_vcmp(&message->uri, "/api/v1/perfcheck") == 0) 
			{
				// handle RESTful call for performance check
				handle_perfcheck_call(connection, message);
			}
			else 
			{
				// serve static content
				mg_serve_http(connection, message, http_server_options);
			}
			break;
		default:
			break;

	}
}

int main(int argc, char *argv[])
{
	struct mg_mgr event_manager;			// holds all the connections
	struct mg_connection *connection;		// describes the connection

	mg_mgr_init(&event_manager, NULL);		// initialize the event manager object

	for (int i = 0; i < argc; ++i) 
	{
		if (strcmp(argv[i], "-p") == 0 && i + 1 < argc)
		{
			http_port = argv[++i];
		}
	}

	// Set HTTP server options
	connection = mg_bind(&event_manager, http_port, ev_handler);

	if (NULL == connection) 
	{
		fprintf(stderr, "Error has occurred starting server on port %s\n", http_port);
		exit(1);
	}

	mg_set_protocol_http_websocket(connection);
	printf("Starting RESTful server on port %s\n", http_port);

	for (;;)
	{
		mg_mgr_poll(&event_manager, 1000);
	}
	mg_mgr_free(&event_manager);


    return 0;
}

