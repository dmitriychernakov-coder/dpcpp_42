#include <iostream>
#include <vector>
#include <string>
#include <cassert>

class SqlSelectQueryBuilder {
private:
    std::vector<std::string> colon;
    std::string table;
    std::vector<std::pair<std::string, std::string>> zapros;

public:
    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        colon.push_back(column);

        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& tab) {
        table = tab;

        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value) {
        zapros.push_back({column, value});

        return *this;
    }

    std::string BuildQuery() {
        std::string query = "SELECT ";

        if (colon.empty()) {
			 query += "*";

			} else {

            for (size_t i = 0; i < colon.size(); ++i) {
                query += colon[i];

                if (i < colon.size() - 1) query += ", ";
            }
        }

        query += " FROM " + table;

        if (!zapros.empty()) {
            query += " WHERE ";

            for (size_t i = 0; i < zapros.size(); ++i) {
                query += zapros[i].first + "=" + zapros[i].second;

                if (i < zapros.size() - 1) query += " AND ";
            }
        }
        return query + ";";
    }
};

int main() {
    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumn("name").AddColumn("phone");
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");
    
    std::string result1 = query_builder.BuildQuery();
    std::cout << result1 << std::endl;
    assert(result1 == "SELECT name, phone FROM students WHERE id=42 AND name=John;");

    SqlSelectQueryBuilder query_builder_all;
    query_builder_all.AddWhere("name", "John")
                     .AddFrom("students");
    std::string result2 = query_builder_all.BuildQuery();
    std::cout << result2 << std::endl;
    assert(result2 == "SELECT * FROM students WHERE name=John;");

    return 0;
}


