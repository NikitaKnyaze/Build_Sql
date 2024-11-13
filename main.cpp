#include <iostream>
#include <pqxx/pqxx>
#include <map>
#include <Windows.h>

struct Table
{
   std::string name_table{ "FirstName" };
   std::vector<std::string> columns{ "id" };
   std::map<std::string, std::string> add_data;
};

class SqlSelectQueryBuilder
{
private:
   Table table;
   pqxx::connection conn;
   bool flag{ false };

public:
   SqlSelectQueryBuilder()
      : conn("host=127.0.0.1 port=5432 dbname=dbbuilder user=postgres password=ads67") {}

   SqlSelectQueryBuilder& AddColumn(const std::string& new_column)
   {
      table.columns.push_back(new_column);
      flag = true;
      return *this;
   }

   SqlSelectQueryBuilder& AddFrom(const std::string& new_name_table)
   {
      table.name_table = new_name_table;
      return *this;
   }
   SqlSelectQueryBuilder& AddWhere(const std::string& key, const std::string& value)
   {
      table.add_data[key] = value;
      return *this;
   }

   Table BuildTable()
   {
      try {
         pqxx::work t(conn);

         t.exec("CREATE TABLE IF NOT EXISTS \"" + t.esc(table.name_table) + "\" ("
            "\"" + table.columns[0] + "\" SERIAL PRIMARY KEY)");

         for (const auto& column : table.columns) {
            if (column == "id") continue;

            t.exec("ALTER TABLE \"" + t.esc(table.name_table) + "\" "
               "ADD COLUMN \"" + t.esc(column) + "\" VARCHAR(40);");
         }

         std::string columns;
         std::string values;
         bool first = true;

         for (const auto& pair : table.add_data) {
            if (!first) {
               columns += ", ";
               values += ", ";
            }
            columns += "\"" + t.esc(pair.first) + "\"";
            values += "'" + t.esc(pair.second) + "'";
            first = false;
         }

         if (!columns.empty() && !values.empty()) {
            t.exec("INSERT INTO \"" + t.esc(table.name_table) + "\" (" + columns + ") "
               "VALUES (" + values + ");");
         }

         t.commit();
      }
      catch (const std::exception& e)
      {
         std::cerr << "Îøèáêà: " << e.what() << std::endl;
      }
      return table;
   }

   void SqlSelectQueryBuilder::BuildSelect()
   {
      try {
         pqxx::work t(conn);
         if (flag)
         {
            t.exec("SELECT name, phone FROM students WHERE id=42 AND name='John';");
         }
         else
         {
            t.exec("SELECT * FROM students WHERE id=42 AND name='John';");
         }

         t.commit();
      }
      catch (const std::exception& e)
      {
         std::cerr << "Îøèáêà: " << e.what() << std::endl;
      }
   }
};


int main()
{
   SetConsoleCP(CP_UTF8);
   SetConsoleOutputCP(CP_UTF8);
   SqlSelectQueryBuilder query_builder;
   query_builder.AddColumn("name").AddColumn("phone");
   query_builder.AddFrom("students");
   query_builder.AddWhere("id", "42").AddWhere("name", "John");
   query_builder.BuildTable();
   query_builder.BuildSelect();
  

   return EXIT_SUCCESS;
}