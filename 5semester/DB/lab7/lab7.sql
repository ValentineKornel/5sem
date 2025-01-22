ALTER SESSION SET CONTAINER = "XEPDB1";
show con_name;

--1
--sys
grant create sequence, create cluster, create synonym, 
    create public synonym, create materialized view to KVVCORE;

grant select on SYS.DBA_CLUSTERS to KVVCORE;
grant select on SYS.DBA_TABLES to KVVCORE;
grant execute on DBMS_MVIEW to KVVCORE;

--2
--drop sequence S1
create sequence S1
    increment by 10
    start with 1000
    nomaxvalue
    nominvalue
    nocycle
    nocache
    noorder;
    
select S1.nextval from dual;
select S1.currval from dual;

--3
--drop sequence S2;
create sequence S2
    start with 10
    increment by 10
    maxvalue 100
    nocycle;
    
select S2.nextval from dual;
select S2.currval from dual;
    
--4
--drop sequence S3;
create sequence S3
    start with 10 
    increment by -10
    minvalue -100
    maxvalue 10
    nocycle
    order;

select S3.nextval from dual;
select S3.currval from dual;

--5
--drop sequence S4;
create sequence S4
    start with 1
    increment by 1
    maxvalue 10
    cycle
    cache 5
    noorder;
    
select S4.nextval from dual;
select S4.currval from dual;
    
    
--6
select * from user_sequences;


--7
--drop table T1;


create table T1
(
    N1 number(20),
    N2 number(20),
    N3 number(20),
    N4 number(20)
) storage (buffer_pool keep) cache;

begin
    for i in 1..7
        loop
            insert into T1 values(S1.nextval, S2.nextval, S3.nextval, S4.nextval);
        end loop;
end;

select * from T1;


--8
drop cluster ABC;

create cluster ABC
(
    X number(10),
    V varchar2(12)
) hashkeys 200 tablespace TS_KVV;

--9
--drop table A;

create table A
(
    XA number(10),
    VA varchar2(12),
    Y int
) cluster ABC(XA, VA);


--10
--drop table B;

create table B
(
    XB number(10),
    VB varchar2(12),
    Y  int
) cluster ABC (XB, VB);

--11
--drop table C;

create table C
(
    XC number(10),
    VC varchar2(12),
    Y  int
) cluster ABC (XC, VC);


--12
select owner, cluster_name from sys.dba_clusters;
select * from sys.dba_tables where cluster_name = 'ABC';


--13
--drop synonym S_SYNONYM;

create synonym S_SYNONYM for C;
insert into S_SYNONYM values (10, 'a', 4);
select * from S_SYNONYM;


--14
--drop public synonym B_SYNONYM;
create public synonym B_SYNONYM for B;
insert into B_SYNONYM values (9, 'b', 5);
select * from B_SYNONYM;

--15
--drop table AA;

create table AA
(
    X number(20) primary key
);

--drop table BB;

create table BB
(
    Y number(20),
    constraint fk_y foreign key (Y) references AA (X)
);

insert into AA(X) values (1);
insert into AA(X) values (2);
insert into BB(Y) values (1);
insert into BB(Y) values (2);
commit;

--drop view V1
create view V1 as select * from AA inner join BB on AA.X = BB.Y;

select * from V1;

--16
--drop materialized view MV;

create materialized view MV
    build immediate
    refresh complete on demand
    next sysdate + numtodsinterval(2, 'minute')
    as select * from AA inner join BB on AA.X = BB.Y;

select * from MV;
insert into AA values (5);
insert into BB values (5);
commit;

begin
DBMS_MVIEW.REFRESH('MV');
end;






    