ALTER SESSION SET CONTAINER = "XEPDB1";
alter session set nls_date_format='dd-mm-yyyy hh24:mi:ss';
ALTER TABLESPACE TS_KVV ADD DATAFILE 'C:/LABS/LAB2/new_file.dbf' SIZE 100M;
ALTER TABLESPACE TS_KVV ADD DATAFILE 'C:/LABS/LAB2/new_file2.dbf' SIZE 100M;

--1
--drop table MyTable_RANGE;
create table MyTable_RANGE
(
    id number,
    TIME_ID date
)
partition by range (id)
(
    partition part0 values less than (10),
    partition part1 values less than (20),
    partition part2 values less than (30),
    partition PartMAX values less than (maxvalue)
);

begin
    for i in 1..40
    loop
        insert into MyTable_RANGE(id, time_id) values (i, sysdate);
    end loop;
end;
commit;

select * from MyTable_RANGE partition(part0);
select * from MyTable_RANGE partition(part1);
select * from MyTable_RANGE partition(part2);
select * from MyTable_RANGE partition(PartMAX);

select TABLE_NAME, PARTITION_NAME, HIGH_VALUE, TABLESPACE_NAME
from USER_TAB_PARTITIONS
where table_name = 'MYTABLE_RANGE';

--2
--drop table MyTable_INTERVAL;
create table MyTable_INTERVAL
(
    id number,
    time_id date
)
    partition by range (time_id)
    interval (numtoyminterval(1, 'month'))
(
    partition part0 values less than (to_date('1-10-2024', 'dd-mm-yyyy')),
    partition part1 values less than (to_date('1-11-2024', 'dd-mm-yyyy')),
    partition part2 values less than (to_date('1-12-2024', 'dd-mm-yyyy'))
);

insert into MyTable_INTERVAL(id, time_id) values (1, '01-09-2024');
insert into MyTable_INTERVAL(id, time_id) values (2, '16-09-2024');
insert into MyTable_INTERVAL(id, time_id) values (3, '02-10-2024');
insert into MyTable_INTERVAL(id, time_id) values (4, '17-10-2024');
insert into MyTable_INTERVAL(id, time_id) values (5, '03-11-2024');
insert into MyTable_INTERVAL(id, time_id) values (6, '25-11-2024');
insert into MyTable_INTERVAL(id, time_id) values (7, '29-12-2024');
insert into MyTable_INTERVAL(id, time_id) values (7, '29-01-2024');
insert into MyTable_INTERVAL(id, time_id) values (7, '21-03-2025');
insert into MyTable_INTERVAL(id, time_id) values (7, '29-06-2025');
commit;

select * from MyTable_INTERVAL partition (part0);
select * from MyTable_INTERVAL partition (part1);
select * from MyTable_INTERVAL partition (part2);
select * from MyTable_INTERVAL partition (SYS_P2297);

select TABLE_NAME, PARTITION_NAME, HIGH_VALUE, TABLESPACE_NAME
from USER_TAB_PARTITIONS
where table_name = 'MYTABLE_INTERVAL';

--3
--drop table MyTable_HASH;
create table MyTable_HASH
(
    str varchar2(50),
    id  number
)
partition by hash (str)
(
    partition part0,
    partition part1,
    partition part2,
    partition part3
);

insert into MyTable_HASH (str, id) values ('qwerty', 1);
insert into MyTable_HASH (str, id) values ('bbbbbbbb', 2);
insert into MyTable_HASH (str, id) values ('cccccccc', 3);
insert into MyTable_HASH (str, id) values ('dddddddd', 4);
insert into MyTable_HASH (str, id) values ('eeeeeeee', 5);
insert into MyTable_HASH (str, id) values ('ffffffff', 6);
commit;

select * from MyTable_HASH partition (part0);
select * from MyTable_HASH partition (part1);
select * from MyTable_HASH partition (part2);
select * from MyTable_HASH partition (part3);

--4
--drop table MyTable_LIST;
create table MyTable_LIST
(
    obj char(3)
)
partition by list(obj)
(
    partition part0 values ('a', 'd'),
    partition part1 values ('b', 'e'),
    partition part2 values ('c', 'f')
);

insert into MyTable_LIST(obj) values('a');
insert into MyTable_LIST(obj) values('b');
insert into MyTable_LIST(obj) values('c');
insert into MyTable_LIST(obj) values('d');
insert into MyTable_LIST(obj) values('e');
insert into MyTable_LIST(obj) values('f');
commit;

select * from MyTable_LIST partition (part0);
select * from MyTable_LIST partition (part1);
select * from MyTable_LIST partition (part2);

--6
alter table MyTable_RANGE enable row movement;
select * from MyTable_RANGE partition(PartMAX);
select * from MyTable_RANGE partition(part0);
update MyTable_RANGE set id=45 where id=2;

alter table MyTable_INTERVAL enable row movement;
select * from MyTable_INTERVAL partition(part0);
update MyTable_INTERVAL set time_id=to_date('21-11-2024') where id=1;
select * from MyTable_INTERVAL partition(part2);

alter table MyTable_HASH enable row movement;
update MyTable_HASH set str='ggggggggg' where id=2;
select * from MyTable_HASH partition(part2);
select * from MyTable_HASH partition(part1);


alter table MyTable_LIST enable row movement;
update MyTable_LIST set obj='b' where obj='a';
select * from MyTable_LIST partition (part0);
select * from MyTable_LIST partition (part1);

--7
alter table MyTable_RANGE merge partitions part1, part2 into partition part5;
select * from MyTable_RANGE partition(part5);
select * from MyTable_RANGE partition(part1);
select * from MyTable_RANGE partition(part2);

--8
alter table MyTable_RANGE split partition part5 at (20) into (partition part1, partition part2);
select * from MyTable_RANGE partition(part5);
select * from MyTable_RANGE partition(part1);
select * from MyTable_RANGE partition(part2);

--9
--drop table MyTable_RANGE_TEMP;
create table MyTable_RANGE_TEMP
(
    id number,
    time_id date
);
alter table MyTable_RANGE exchange partition part0 with table MyTable_RANGE_TEMP without validation;
select * from MyTable_RANGE partition (part0);
select * from MyTable_RANGE_TEMP;


-----
drop table MyTable_COMPOSE;
CREATE TABLE MyTable_COMPOSE
(
    id NUMBER,
    time_id DATE,
    obj char(3)
)
partition by range(time_id)
    subpartition by list(obj)
    (
    partition part_range_1 values less than (to_date('1-10-2024', 'dd-mm-yyyy'))
        (subpartition part_list_1 values('a', 'd'),
        subpartition part_list_2 values('b', 'e'),
        subpartition part_list_3 values('c', 'f')
        ),
    partition part_range_2 values less than (to_date('1-11-2024', 'dd-mm-yyyy'))
        (subpartition part_list_4 values('a', 'd'),
        subpartition part_list_5 values('b', 'e'),
        subpartition part_list_6 values('c', 'f')
        ),
    partition part_range_3 values less than (to_date('1-12-2024', 'dd-mm-yyyy'))
        (subpartition part_list_7 values('a', 'd'),
        subpartition part_list_8 values('b', 'e'),
        subpartition part_list_9 values('c', 'f')
        )
    );

insert into MyTable_COMPOSE values (1, '01-09-2024', 'a');
insert into MyTable_COMPOSE values (2, '02-09-2024', 'b');
insert into MyTable_COMPOSE values (3, '03-09-2024', 'c');
insert into MyTable_COMPOSE values (4, '01-10-2024', 'a');
insert into MyTable_COMPOSE values (5, '02-10-2024', 'b');
insert into MyTable_COMPOSE values (6, '03-10-2024', 'c');
insert into MyTable_COMPOSE values (7, '01-11-2024', 'a');
insert into MyTable_COMPOSE values (8, '02-11-2024', 'b');
insert into MyTable_COMPOSE values (9, '03-11-2024', 'c');
commit;

select * from MyTable_COMPOSE subpartition (part_list_7);
select * from MyTable_COMPOSE partition (part_range_3);

select TABLE_NAME, PARTITION_NAME, HIGH_VALUE, TABLESPACE_NAME
from USER_TAB_PARTITIONS
where table_name = 'MYTABLE_COMPOSE';

SELECT TABLE_NAME, PARTITION_NAME, HIGH_VALUE, TABLESPACE_NAME
FROM USER_TAB_SUBPARTITIONS 
WHERE TABLE_NAME = 'MYTABLE_COMPOSE';




