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
#include "Email.h"
//#include "Lib\libcurl\include\curl\curl.h"

#pragma comment(lib, "PocoNet.lib")

using namespace std;
#define NS_HTTP_REQUEST 100 /* struct http_message * */

static const char *http_port = "8000";
static struct mg_serve_http_opts http_server_options;
Database *prod = new Database("prod.db");

static void sendEmailAlert(std::string emailTo, std::string body, std::string subject = "PerfCheck - Email Alert") {
	std::string emailFrom = "emanon4ever@gmail.com";
	
	Email *email = new Email(emailFrom, "Peter Newsome", "", "", emailTo, "", "", subject, body);
	email->SendEmail();
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

// memory=1235132&cpu=90perc&disk=50perc
static void handle_perfcheck_call(struct mg_connection *connection, struct http_message *hm) 
{
	std::string performanceTable("PerformanceCheck");
	char memory[120], cpu[120], disk[120];
	printf("message with dot p: %s\n" , hm->query_string.p);
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

