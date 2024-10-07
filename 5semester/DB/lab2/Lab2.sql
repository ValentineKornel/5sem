create tablespace TS_KVV
datafile 'C:\labs\lab2\TS_KVVV.dbf' size 7M
autoextend on next 5M maxsize 20M
extent management local;

drop tablespace TS_KVV including contents and datafiles;

create temporary tablespace TS_KVV_TEMP
tempfile 'C:\labs\lab2\TS_KVVV_TEMP.dbf' size 5M
autoextend on next 3M maxsize 30M
extent management local;

drop tablespace TS_KVV_TEMP including contents and datafiles;

select * from dba_tablespaces;
select * from dba_data_files;
select * from dba_temp_files;

select * from dba_roles;

create role RL_KVVCORE;

grant create session to RL_KVVCORE;
grant create table to RL_KVVCORE;
--grant drop any table to RL_KVVCORE;
grant create view, drop any view to RL_KVVCORE;
grant create procedure, drop any procedure to RL_KVVCORE;

select * from dba_roles where role = 'RL_KVVCORE';
select * from dba_sys_privs where grantee = 'RL_KVVCORE';

--cистемные привилегии

create profile PF_KVVCORE LIMIT
    PASSWORD_LIFE_TIME 180
    SESSIONS_PER_USER 3
    FAILED_LOGIN_ATTEMPTS 7
    PASSWORD_LOCK_TIME 1
    PASSWORD_REUSE_TIME 10
    PASSWORD_GRACE_TIME DEFAULT
    CONNECT_TIME 180
    IDLE_TIME 30;
    
select * from dba_profiles;

select * from dba_profiles where profile = 'PF_KVVCORE';
select * from dba_profiles where profile = 'DEFAULT';


create user KVVCORE
identified by 12345
default tablespace TS_KVV
temporary tablespace TS_KVV_TEMP
profile "PF_KVVCORE"
account UNLOCK;

grant create session to KVVCORE;
ALTER USER KVVCORE PASSWORD EXPIRE;
GRANT RL_KVVCORE TO  KVVCORE;


select SYS_CONTEXT('USERENV', 'CON_NAME') as current_pdb from dual;
select SYS_CONTEXT('USERENV', 'SERVICE_NAME') as current_pdb from dual;

ALTER SESSION SET CONTAINER =  XEPDB1;
ALTER SESSION SET CONTAINER = CDB$ROOT;

SELECT * FROM dba_users;

select * from dba_sys_privs where grantee = 'KVVCORE';
select * from dba_role_privs where grantee = 'KVVCORE';
select * from all_tab_privs_recd where grantee = 'KVVCORE';

CREATE TABLE employees(
    id NUMBER PRIMARY KEY,
    first_name VARCHAR2(50),
    last_name VARCHAR(50),
    email VARCHAR2(50)
);


CREATE VIEW employee_names AS
    SELECT id, first_name, last_name
    FROM employees;

CREATE TABLESPACE KVV_QDATA
DATAFILE 'C:\labs\lab2\KVV_QDATA.dbf' SIZE 10M
OFFLINE;

ALTER TABLESPACE KVV_QDATA ONLINE;
ALTER USER KVVCORE QUOTA 2M ON KVV_QDATA;

CREATE TABLE KVV_T1(
    id NUMBER PRIMARY KEY,
    name VARCHAR2(50)
) TABLESPACE KVV_QDATA;

INSERT INTO KVV_T1 VALUES (1, 'person1');
INSERT INTO KVV_T1 VALUES (2, 'person2');
INSERT INTO KVV_T1 VALUES (3, 'person3');

select * from KVV_T1;




