#include "db_test.h"

#include <stdlib.h>
#include <iostream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
using namespace std;

//DATABASE TEST
const string server = "127.0.0.1:3306";
const string username = "root";
const string password = "";

void db_test::test_ascii() {
    sql::Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    sql::PreparedStatement* pstmt;

    try
    {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
    }
    catch (sql::SQLException e)
    {
        std::cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
    stmt = con->createStatement();
    stmt->execute("USE project_11");

    sql::ResultSet* res;
    stmt = con->createStatement();

    res = stmt->executeQuery("SELECT * FROM asci");
    std::cout << "|DEC";
    std::cout << "|HEX";
    std::cout << "|CPP";
    std::cout << "|COD";
    std::cout << "|DESC" << endl;
    while (res->next()) {
        std::cout << "|" << res->getString("DEC");
        std::cout << "|" << res->getString("HEX");
        std::cout << "|" << res->getString("CPP");
        std::cout << "|" << res->getString("COD");
        std::cout << "|" << res->getString("DESC") << "|" << std::endl;
    }
    delete res;
    delete stmt;
    delete con;
    system("pause");
}

std::map<std::pair<std::string, std::string>, std::string> db_test::test_navigation() {

    std::map<std::pair<std::string, std::string>, std::string> nav_map;
    sql::Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    //sql::PreparedStatement* pstmt;

    try
    {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
    }
    catch (sql::SQLException e)
    {
        std::cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
    stmt = con->createStatement();
    stmt->execute("USE project_11");

    sql::ResultSet* res;
    stmt = con->createStatement();

    res = stmt->executeQuery("SELECT * FROM basic_navigation_test");
    while (res->next()) {
        nav_map.insert({std::make_pair(res->getString("context"), res->getString("description")), res->getString("path")});
        std::cout << res->getString("context") << " " << res->getString("description") << " " << res->getString("path") << std::endl;
    }
    delete res;
    delete stmt;
    delete con;
    return nav_map;
}