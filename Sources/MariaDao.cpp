#include "MariaDao.h"
#include <string>
#include "global.h"
#include "MariaPool.h"
#include "mysql.h"

MariaDao::MariaDao()
{
    auto & cfg = ConfigMgr::Inst();
    const auto& host = cfg["Mysql"]["Host"];
    const auto& port = cfg["Mysql"]["Port"];
    const auto& pwd = cfg["Mysql"]["Passwd"];
    const auto& schema = cfg["Mysql"]["Schema"];
    const auto& user = cfg["Mysql"]["User"];
    pool_.reset(new MariaPool(host+":"+port, user, pwd,schema, 5));
}

MariaDao::~MariaDao(){
    pool_->Close();
}

int MariaDao::RegUser(const std::string& name, const std::string& email, const std::string& pwd) {
    auto conn = pool_->getConnection();
    if (!conn) {
        std::cerr << "Failed to get database connection" << std::endl;
        return -1;
    }

    try {
        // 准备调用存储过程的SQL语句
        std::string query = "CALL reg_user(?, ?, ?, @result)";
        MYSQL_STMT* stmt = mysql_stmt_init(conn.get());
        if (!stmt) {
            throw std::runtime_error("mysql_stmt_init failed");
        }

        // 预处理SQL语句
        if (mysql_stmt_prepare(stmt, query.c_str(), query.length()) != 0) {
            std::string error = mysql_stmt_error(stmt);
            mysql_stmt_close(stmt);
            throw std::runtime_error("Failed to prepare statement: " + error);
        }

        // 绑定输入参数
        MYSQL_BIND bind[3];
        memset(bind, 0, sizeof(bind));

        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = (void*)name.c_str();
        bind[0].buffer_length = name.length();

        bind[1].buffer_type = MYSQL_TYPE_STRING;
        bind[1].buffer = (void*)email.c_str();
        bind[1].buffer_length = email.length();

        bind[2].buffer_type = MYSQL_TYPE_STRING;
        bind[2].buffer = (void*)pwd.c_str();
        bind[2].buffer_length = pwd.length();

        if (mysql_stmt_bind_param(stmt, bind) != 0) {
            std::string error = mysql_stmt_error(stmt);
            mysql_stmt_close(stmt);
            throw std::runtime_error("Failed to bind parameters: " + error);
        }

        // 执行存储过程
        if (mysql_stmt_execute(stmt) != 0) {
            std::string error = mysql_stmt_error(stmt);
            mysql_stmt_close(stmt);
            throw std::runtime_error("Failed to execute statement: " + error);
        }

        mysql_stmt_close(stmt);

        // 处理所有结果集
        while (mysql_next_result(conn.get()) == 0) {
            MYSQL_RES* temp = mysql_store_result(conn.get());
            if (temp) mysql_free_result(temp);
        }

        // 查询@result变量的值
        if (mysql_query(conn.get(), "SELECT @result AS result") != 0) {
            throw std::runtime_error("Failed to get result value");
        }

        MYSQL_RES* result = mysql_store_result(conn.get());
        if (!result) {
            throw std::runtime_error("Failed to store result");
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        int return_value = -1;
        if (row && row[0]) {
            return_value = std::atoi(row[0]);
        }

        mysql_free_result(result);
        pool_->returnConnection(std::move(conn));
        return return_value;

    } catch (const std::exception& e) {
        std::cerr << "Error in RegUser: " << e.what() << std::endl;
        pool_->returnConnection(std::move(conn));
        return -1;
    }
}