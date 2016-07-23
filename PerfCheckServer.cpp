// PerfCheckServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string.h>
#include <sstream>
#pragma warning(disable: 4244)
#include "Lib\mongoose\include\mongoose.h"
#include "Database.h"
#include "PerformanceData.h"
#include "PocoEmail.h"
#include "tinyxml2.h"


#ifdef _DEBUG
#pragma comment(lib, "Lib/libcurl/lib/libcurl_a_debug.lib")
#else
#pragma comment(lib, "Lib/libcurl/lib/libcurl_a.lib")
#endif

using namespace std;
#define NS_HTTP_REQUEST 100 /* struct http_message * */

static const char *httpPort = "8000";
static struct mg_serve_http_opts httpServerOptions;
Database *prod = new Database("prod.db");

static void sendEmailAlert(std::string emailTo, std::string body, std::string subject = "PerfCheck - Email Alert") {
	std::string emailFrom = "emanon4ever@gmail.com";
	
	Email *email = new PocoEmail(emailFrom, "Fanion Newsome", "", "", emailTo, "", "", subject, body);
	email->SendEmail();
	delete email;
}

static int InsertPerformanceData(PerformanceData *data, std::string tableName) {
	int result = 0;
	std::string insertQuery("");
	std::stringstream ss;

	printf("Checking if table: '%s' exists'", tableName.c_str());
	ss << "SELECT name FROM sqlite_master WHERE type='table' and name='" << tableName << "';";
	std::string tableExistsQuery = ss.str();
	ss.str(std::string());

	result = prod->executeQuery(tableExistsQuery);
	if (result != 0) {
		std::string createTable("");
		ss << "CREATE TABLE " << tableName << " (id INTEGER PRIMARY KEY, memory INTEGER, cpu INTEGER, disk INTEGER);";
		result = prod->executeQuery(ss.str());

		if (result) {
			printf("Creating table '%s' was successful", tableName.c_str());
		}
		else {
			printf("Creating table '%s' failed!", tableName.c_str());
		}
	}
	// let's insert
	
    ss << "INSERT INTO " << tableName << "(memory, cpu, disk) VALUES('" << data->getMemory() << "','" << data->getCPU() << "','" << data->getDiskSpace() << "');";
	insertQuery = ss.str();
	result = prod->executeQuery(insertQuery);
	
	return result;
}

// QueryString Format: memory=1235132&cpu=90&disk=50
static void handle_perfcheck_call(struct mg_connection *connection, struct http_message *hm) 
{
	std::string performanceTable("PerformanceCheck");
	char memory[120], cpu[120], disk[120];
	
	// GET form variables 
	mg_get_http_var(&hm->query_string, "memory", memory, sizeof(memory));
	mg_get_http_var(&hm->query_string, "cpu", cpu, sizeof(cpu));
	mg_get_http_var(&hm->query_string, "disk", disk, sizeof(disk));
		
	// insert into the database
	PerformanceData * pd = new PerformanceData(atol(memory),atol(cpu),atol(disk));
	std::string emailBody = "We have an alert we need to take care of!";
	sendEmailAlert("fanionnewsome@gmail.com", emailBody);
	
	//int status = InsertPerformanceData(pd, performanceTable);

	prod->displayTable(performanceTable);
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
				mg_serve_http(connection, message, httpServerOptions);
			}
			break;
		default:
			break;

	}
}

int main(int argc, char *argv[])
{
	struct mg_mgr eventManager;			// holds all the connections
	struct mg_connection *connection;		// describes the connection

	mg_mgr_init(&eventManager, NULL);		// initialize the event manager object

	for (int i = 0; i < argc; ++i) 
	{
		if (strcmp(argv[i], "-p") == 0 && i + 1 < argc)
		{
			httpPort = argv[++i];
		}
	}

	// Set HTTP server options
	connection = mg_bind(&eventManager, httpPort, ev_handler);

	if (NULL == connection) 
	{
		fprintf(stderr, "Error has occurred starting server on port %s\n", httpPort);
		exit(1);
	}

	mg_set_protocol_http_websocket(connection);
	printf("Starting RESTful server on port %s\n", httpPort);

	for (;;)
	{
		mg_mgr_poll(&eventManager, 1000);
	}
	mg_mgr_free(&eventManager);
	delete prod;

    return 0;
}

