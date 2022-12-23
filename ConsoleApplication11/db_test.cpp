#include "db_test.h"

#include <stdlib.h>
#include <iostream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
using namespace std;

//for demonstration only. never save your password in the code!
const string server = "127.0.0.1:3306";
const string username = "root";
const string password = "";

void db_test::test_db() {
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
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }
    //please create database "quickstartdb" ahead of time
    //con->setSchema("quickstartdb");

    stmt = con->createStatement();
    stmt->execute("USE test_db");
    stmt->execute("DROP TABLE IF EXISTS test");
    stmt->execute("CREATE TABLE test(id INT, label CHAR(1))");
    stmt->execute("INSERT INTO test(id, label) VALUES (1, 'a')");
    sql::ResultSet* res;
    // ...
    stmt = con->createStatement();
    // ...

    res = stmt->executeQuery("SELECT * FROM test ORDER BY id ASC");
    while (res->next()) {
        // You can use either numeric offsets...
        cout << "id = " << res->getInt(1); // getInt(1) returns the first column
        // ... or column names for accessing results.
        // The latter is recommended.
        cout << ", label = '" << res->getString("label") << "'" << endl;
    }

    delete res;
    delete stmt;
    delete con;
    /*
    pstmt = con->prepareStatement("INSERT INTO inventory(name, quantity) VALUES(?,?)");
    pstmt->setString(1, "banana");
    pstmt->setInt(2, 150);
    pstmt->execute();
    cout << "One row inserted." << endl;

    pstmt->setString(1, "orange");
    pstmt->setInt(2, 154);
    pstmt->execute();
    cout << "One row inserted." << endl;

    pstmt->setString(1, "apple");
    pstmt->setInt(2, 100);
    pstmt->execute();
    cout << "One row inserted." << endl;

    delete pstmt;
    
    */
    system("pause");
}