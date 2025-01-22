alter session set nls_date_format='dd-mm-yyyy hh24:mi:ss';
ALTER SESSION SET CONTAINER = "xepdb1";

--1

--drop table MyTable;
create table MyTable(
    id number,
    str varchar2(10)
);

--drop table MyTableDestination;
create table MyTableDestination(
    id number,
    str varchar2(10)
);

--drop table taskStatus;
create table taskStatus(
    status varchar2(20),
    err_message varchar2(200),
    datetime date
);

insert into MyTable values(1, 'aaa');
insert into MyTable values(2, 'bbb');
insert into MyTable values(3, 'ccc');
insert into MyTable values(4, 'ddd');
insert into MyTable values(5, 'eee');
insert into MyTable values(6, 'fff');
insert into MyTable values(7, 'ggg');
commit;

select * from MyTable;
select * from MyTableDestination;


create or replace procedure transfer_data
is
    cursor data_cursor is select * from MyTable where id < 6;
    err_message varchar2(200);
begin
    for row in data_cursor
    loop
        insert into MyTableDestination values(row.id, row.str);
    end loop;
        
    delete MyTable where id < 6;
    insert into taskStatus(status, datetime) values ('success', sysdate);
    commit;
    exception
        when others then
            err_message := sqlerrm;
            insert into taskStatus(status, err_message) values ('error', err_message);
            commit;
end transfer_data;

declare
    task_number user_jobs.job%type;
begin
    dbms_job.submit(task_number, 'begin transfer_data(); end;', TO_DATE('2024-12-17 10:02:00', 'yyyy-mm-dd hh24:mi:ss'), 'sysdate + 7');
    commit;
    dbms_output.put_line(task_number);
end;

select * from taskStatus;
select * from MyTable;
select * from MyTableDestination;
truncate table taskStatus;

--4
select job, what, last_date, last_sec, next_date, next_sec, broken from user_jobs;

begin
  dbms_job.run(24);
end;
begin

BEGIN
DBMS_JOB.CHANGE(job => 23,
            what => 'BEGIN transfer_data(); END;',
            next_date => SYSDATE+1),
            interval => 'SYSDATE + 7');
COMMIT;
END;

end;

begin
    dbms_job.broken(20, true);
end;

begin
  dbms_job.remove(24);
end;

--6
begin
dbms_scheduler.create_schedule(
  schedule_name => 'MyChedule',
  start_date => sysdate,
  repeat_interval => 'FREQ=WEEKLY',
  comments => 'MyChedule WEEKLY starts now'
);
end;
begin
dbms_scheduler.drop_schedule('MyChedule');
end;

select * from user_scheduler_schedules;

begin
dbms_scheduler.create_program(
  program_name => 'MyProgramm',
  program_type => 'STORED_PROCEDURE',
  program_action => 'transfer_data',
  number_of_arguments => 0,
  enabled => true,
  comments => 'MyProgramm comment'
);
end;

begin
dbms_scheduler.drop_program('MyProgramm');
end;

select * from user_scheduler_programs;

begin
    dbms_scheduler.create_job(
            job_name => 'MyJob',
            program_name => 'MyProgramm',
            schedule_name => 'MyChedule',
            enabled => true
        );
end;

select job_name, program_name, job_type, job_action, start_date, repeat_interval, 
end_date, enabled, next_run_date from user_scheduler_jobs where job_name = 'MYJOB';

begin
  DBMS_SCHEDULER.DISABLE('MyJob');
end;

begin
    DBMS_SCHEDULER.RUN_JOB('MyJob');
end;

BEGIN
    DBMS_SCHEDULER.CREATE_SCHEDULE(
        schedule_name => 'MyScheduleNew',
        start_date    => SYSTIMESTAMP + INTERVAL '5' MINUTE,
        repeat_interval => 'FREQ=MINUTELY; INTERVAL=5',
        comments      => 'Strart in 5 minutes'
    );
END;
/

BEGIN
    DBMS_SCHEDULER.SET_ATTRIBUTE(
        name      => 'MyJob',
        attribute => 'schedule_name',
         value     => 'MyScheduleNew'
    );
END;

begin
  DBMS_SCHEDULER.DROP_JOB('MyJob');
end;

select * from taskStatus;
select * from MyTable;
select * from MyTableDestination;
truncate table taskStatus;

ALTER SYSTEM SET JOB_QUEUE_PROCESSES = 5;


