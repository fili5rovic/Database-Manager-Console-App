This project is a C++ based command-line Database Management System (DBMS) that provides an interactive user interface for database operations. It allows users to perform a wide range of tasks on databases, tables, and records. The system is capable of interpreting and executing commands similar to SQL, supporting a comprehensive set of operations such as creating, reading, updating, and deleting (CRUD) data.

> The primary motivation for this project was to explore the complexities of database systems, specifically focusing on operations such as table creation, record insertion and deletion, and data querying.

> This project is a part of the first assignment for the "Practicum of Object-Oriented Programming" course at the University of Belgrade School of Electrical Engineering, majoring in Software Engineering. For detailed assignment instructions, please refer to the respective file.

This project is a comprehensive simulation of a Database Management System (DBMS) with a variety of functionalities. Here's a brief overview:

# Features

> For a comprehensive understanding of the syntax, please refer to the detailed guide provided in the corresponding PDF document. This guide offers an in-depth explanation of the syntax rules, helping users to avoid errors and use the system more effectively.

- **Command-Line Interaction**: The project offers an intuitive command-line interface, enabling users to interact with the database and execute SQL-like commands directly.

- **Database Management**: The system allows users to perform basic database operations such as creation, reading, updating, and deletion. This includes the ability to generate new databases, enumerate all existing databases, select a database for operations, and remove a database.

- **Table Handling**: The project supports numerous table operations, including the creation of new tables, listing all tables within a database, and table deletion.

- **Record Manipulation**: The system enables users to execute CRUD operations on table records. This encompasses the insertion of new records, querying for records, updating existing records, and record deletion.

- **SQL-like Command Interpretation**: The system is capable of interpreting and executing commands that resemble SQL syntax. This includes commands such as CREATE TABLE, INSERT INTO, DELETE FROM, UPDATE, SELECT, and DROP TABLE.

- **Join Operations**: The project supports the execution of INNER JOIN operations between two tables, which allows users to merge rows from two or more tables based on a related column.

- **Error Management**: The system is equipped with robust error handling capabilities, capable of detecting and reporting syntax errors, missing arguments, and invalid operations.

- **C++ Based**: The entire system is implemented using C++, showcasing the application of object-oriented programming principles and various C++ features in the construction of a complex system.

The project in question is a command-line Database Management System (DBMS) developed in C++. It interprets and executes SQL-like commands to manipulate databases, tables, and records. Here's a brief overview of how it operates:

1. **Creating a Table**: The syntax for creating a table is `CREATE TABLE table_name (column1, column2, ...)`. 

2. **Inserting Records**: To insert records into a table, the syntax is `INSERT INTO table_name VALUES (value1, value2, ...)`. 

3. **Updating Records**: To update existing records in a table, the syntax is `UPDATE table_name SET column1 = value1, column2 = value2, ... WHERE condition`. The `Update` class handles this operation.

4. **Deleting Records**: To delete records from a table, the syntax is `DELETE FROM table_name WHERE condition`.

5. **Selecting Records**: To retrieve records from a table, the syntax is `SELECT column1, column2, ... FROM table_name WHERE condition`. The `Select` class handles this operation.

6. **Joining Tables**: The project supports joining tables with the syntax `SELECT column1, column2, ... FROM table1 JOIN table2 ON condition`.

7. **Error Handling**: The project includes robust error handling, with specific exceptions for runtime errors, syntax errors, and missing arguments or keywords.

# Prerequisites
Before you begin, ensure you meet the following prerequisites:
- C++ Compiler: Verify that a C++ compiler is installed on your system.
- Terminal Support: Ensure your terminal supports ASCII escape codes to properly display colorized and formatted output.

