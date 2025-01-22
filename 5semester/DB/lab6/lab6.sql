--1
--C:\app\valen\product\21c\homes\OraDB21Home1\network\admin\sqlnet.ora
--C:\app\valen\product\21c\homes\OraDB21Home1\network\admin\tnsname.ora

--2
--sqlplus system
select NAME, DESCRIPTION, VALUE from v$parameter;

--3
--sqlplus system/password@//localhost:1521/xepdb1
select TABLESPACE_NAME from dba_tablespaces;
select TABLESPACE_NAME, FILE_NAME from dba_data_files;
select TABLESPACE_NAME, FILE_NAME from dba_temp_files;
select ROLE from dba_roles;
select USERNAME from dba_users;

--4
--regedit

--6
--sqlplus KVVCORE/password@KVVCORE_xepdb1

--7
--CREATE TABLE myTable (a NUMBER, b Number);
--insert into myTable values(1, 2);
--insert into myTable values(3, 4);
--commit;
--
--alter session set container=xepdb1;
--ALTER USER KVVCORE QUOTA UNLIMITED ON TS_KVV;

select * from myTable;

--8
--help timing
--set timing on
--select * from myTable;

--9
--help describe
--describe myTable

--10
select SEGMENT_NAME from user_segments;

--11
create view view_segments as
    select count(SEGMENT_NAME) segments_count, sum(EXTENTS) extents_count,
           sum(BLOCKS) bloks_count, (sum(BYTES)/1024) memory_size
    from user_segments;
    
--create view view_segments as select count(SEGMENT_NAME) segments_count, sum(EXTENTS) extents_count, sum(BLOCKS) bloks_count, (sum(BYTES)/1024) memory_size from user_segments;

select * from view_segments;



















