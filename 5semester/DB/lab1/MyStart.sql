create table XXX_t(x number(3) primary key, s varchar2(50));

insert into XXX_t values(1, 'one');
insert into XXX_t values(2, 'two');
insert into XXX_t values(3, 'thre');
COMMIT;

update XXX_t
    set x = 4, 
    s = 'four'
where x = 2;

update XXX_t
    set x = 5, 
    s = 'five'
where x = 3;
COMMIT;

select * from XXX_t;
select * from  XXX_t where x > 1;
select sum(x) from XXX_t;
select avg(x) from XXX_t;
select min(x) from XXX_t;

delete from XXX_t where x = 5;
COMMIT;

create table XXX_t1 (a number, b varchar2(50), constraint x foreign key(a) references XXX_t(x));
insert into XXX_t1 values(1, 'one');
insert into XXX_t1 values(4, 'seven');


select * from XXX_t t
    INNER JOIN XXX_t1 t1
    on t.s = t1.b;

select * from XXX_t t
    left JOIN XXX_t1 t1
    on t.s = t1.b;
    
select * from XXX_t t
    right JOIN XXX_t1 t1
    on t.s = t1.b;
    
drop table XXX_t1;
drop table XXX_t;
COMMIT;