-- 创建数据库
CREATE DATABASE IF NOT EXISTS chat_db
DEFAULT CHARACTER SET utf8mb4
COLLATE utf8mb4_unicode_ci;

USE chat_db;

-- 工作节点注册表
CREATE TABLE IF NOT EXISTS worker_nodes (
                                            id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
                                            datacenter_id TINYINT UNSIGNED NOT NULL,
                                            worker_id TINYINT UNSIGNED NOT NULL,
                                            host_name VARCHAR(100) NOT NULL,
    ip_address VARCHAR(45) NOT NULL,
    port INT UNSIGNED NOT NULL,
    status TINYINT NOT NULL DEFAULT 1 COMMENT ni'1:正常 2:下线 3:故障',
    last_heartbeat TIMESTAMP NULL,
    create_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    UNIQUE KEY uk_dc_worker (datacenter_id, worker_id),
    KEY idx_status_heartbeat (status, last_heartbeat)
    ) ENGINE=InnoDB;

-- 分片配置表
CREATE TABLE IF NOT EXISTS shard_config (
                                            shard_id INT UNSIGNED PRIMARY KEY,
                                            table_name VARCHAR(50) NOT NULL,
    node_host VARCHAR(100),
    node_port INT UNSIGNED,
    min_uid BIGINT UNSIGNED,
    max_uid BIGINT UNSIGNED,
    status TINYINT NOT NULL DEFAULT 1 COMMENT '1:正常 2:只读 3:迁移中',
    create_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    UNIQUE KEY uk_table_name (table_name)
    ) ENGINE=InnoDB;

-- 创建存储过程
DELIMITER //

-- 注册工作节点
CREATE PROCEDURE reg_worker_node(
    IN p_datacenter_id TINYINT,
    IN p_worker_id TINYINT,
    IN p_host_name VARCHAR(100),
    IN p_ip_address VARCHAR(45),
    IN p_port INT,
    OUT p_result INT
)
BEGIN
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
BEGIN
        SET p_result = -1;
ROLLBACK;
END;

START TRANSACTION;

-- 检查是否已存在
IF EXISTS (SELECT 1 FROM worker_nodes
               WHERE datacenter_id = p_datacenter_id
               AND worker_id = p_worker_id) THEN
UPDATE worker_nodes
SET host_name = p_host_name,
    ip_address = p_ip_address,
    port = p_port,
    status = 1,
    last_heartbeat = CURRENT_TIMESTAMP
WHERE datacenter_id = p_datacenter_id
  AND worker_id = p_worker_id;
ELSE
        INSERT INTO worker_nodes
            (datacenter_id, worker_id, host_name, ip_address, port)
        VALUES
            (p_datacenter_id, p_worker_id, p_host_name, p_ip_address, p_port);
END IF;

    SET p_result = 1;
COMMIT;
END //

-- 创建分片表
CREATE PROCEDURE create_shard(
    IN p_shard_id INT,
    IN p_node_host VARCHAR(100),
    IN p_node_port INT
)
BEGIN
    DECLARE v_table_name VARCHAR(50);

    -- 设置表名
    SET v_table_name = CONCAT('users_', p_shard_id);

    -- 创建用户表
    SET @create_users = CONCAT('
        CREATE TABLE IF NOT EXISTS ', v_table_name, ' (
            uid BIGINT UNSIGNED PRIMARY KEY,
            username VARCHAR(50) NOT NULL,
            email VARCHAR(100) NOT NULL,
            password VARCHAR(100) NOT NULL,
            status TINYINT NOT NULL DEFAULT 1,
            create_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
            update_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
            UNIQUE KEY uk_username (username),
            UNIQUE KEY uk_email (email)
        ) ENGINE=InnoDB
    ');

PREPARE stmt FROM @create_users;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- 创建用户档案表
SET @create_profiles = CONCAT('
        CREATE TABLE IF NOT EXISTS user_profiles_', p_shard_id, ' (
            uid BIGINT UNSIGNED PRIMARY KEY,
            nickname VARCHAR(50),
            avatar VARCHAR(255),
            create_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
            update_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
            FOREIGN KEY (uid) REFERENCES ', v_table_name, '(uid)
        ) ENGINE=InnoDB
    ');

PREPARE stmt FROM @create_profiles;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- 注册分片配置
INSERT INTO shard_config
(shard_id, table_name, node_host, node_port)
VALUES
    (p_shard_id, v_table_name, p_node_host, p_node_port)
    ON DUPLICATE KEY UPDATE
                         node_host = p_node_host,
                         node_port = p_node_port;
END //

DELIMITER ;