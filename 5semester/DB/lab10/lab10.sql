
--1
alter table TEACHER add BIRTHDAY date;
alter table TEACHER add SALARY number;

update TEACHER set BIRTHDAY = '12-02-1959' where TEACHER = 'ÑÌËÂ';
update TEACHER set BIRTHDAY = '30-01-1987' where TEACHER = 'ÀÊÍÂ×';
update TEACHER set BIRTHDAY = '19-04-1991' where TEACHER = 'ÊËÑÍÂ';
update TEACHER set BIRTHDAY = '16-04-1964' where TEACHER = 'ÃÐÌÍ';
update TEACHER set BIRTHDAY = '19-11-1988' where TEACHER = 'ËÙÍÊ';
update TEACHER set BIRTHDAY = '05-10-1966' where TEACHER = 'ÁÐÊÂ×';
update TEACHER set BIRTHDAY = '10-08-1976' where TEACHER = 'ÄÄÊ';
update TEACHER set BIRTHDAY = '11-09-1989' where TEACHER = 'ÊÁË';
update TEACHER set BIRTHDAY = '24-12-1983' where TEACHER = 'ÓÐÁ';
update TEACHER set BIRTHDAY = '03-06-1990' where TEACHER = 'ÐÌÍÊ';
update TEACHER set BIRTHDAY = '10-05-1970' where TEACHER = 'ÏÑÒÂËÂ';
update TEACHER set BIRTHDAY = '26-10-1999' where TEACHER = '?';
update TEACHER set BIRTHDAY = '30-07-1984' where TEACHER = 'ÃÐÍ';
update TEACHER set BIRTHDAY = '11-03-1975' where TEACHER = 'ÆËÊ';
update TEACHER set BIRTHDAY = '12-07-1969' where TEACHER = 'ÁÐÒØÂ×';
update TEACHER set BIRTHDAY = '26-02-1983' where TEACHER = 'ÞÄÍÊÂ';
update TEACHER set BIRTHDAY = '13-12-1991' where TEACHER = 'ÁÐÍÂÑÊ';
update TEACHER set BIRTHDAY = '20-01-1968' where TEACHER = 'ÍÂÐÂ';
update TEACHER set BIRTHDAY = '21-12-1969' where TEACHER = 'ÐÂÊ×';
update TEACHER set BIRTHDAY = '28-01-1975' where TEACHER = 'ÄÌÄÊ';
update TEACHER set BIRTHDAY = '10-07-1983' where TEACHER = 'ÌØÊÂÑÊ';
update TEACHER set BIRTHDAY = '08-10-1988' where TEACHER = 'ËÁÕ';
update TEACHER set BIRTHDAY = '30-07-1984' where TEACHER = 'ÇÂÃÖÂ';
update TEACHER set BIRTHDAY = '16-04-1964' where TEACHER = 'ÁÇÁÐÄÂ';
update TEACHER set BIRTHDAY = '12-05-1985' where TEACHER = 'ÏÐÊÏ×Ê';
update TEACHER set BIRTHDAY = '20-10-1980' where TEACHER = 'ÍÑÊÂÖ';
update TEACHER set BIRTHDAY = '21-08-1990' where TEACHER = 'ÌÕÂ';
update TEACHER set BIRTHDAY = '13-08-1966' where TEACHER = 'ÅÙÍÊ';
update TEACHER set BIRTHDAY = '11-11-1978' where TEACHER = 'ÆÐÑÊ';

update TEACHER set SALARY = 1030 where TEACHER = 'ÑÌËÂ';
update TEACHER set SALARY = 1030 where TEACHER = 'ÀÊÍÂ×';
update TEACHER set SALARY = 980 where TEACHER = 'ÊËÑÍÂ';
update TEACHER set SALARY = 1050 where TEACHER = 'ÃÐÌÍ';
update TEACHER set SALARY = 590 where TEACHER = 'ËÙÍÊ';
update TEACHER set SALARY = 870 where TEACHER = 'ÁÐÊÂ×';
update TEACHER set SALARY = 815 where TEACHER = 'ÄÄÊ';
update TEACHER set SALARY = 995 where TEACHER = 'ÊÁË';
update TEACHER set SALARY = 1460 where TEACHER = 'ÓÐÁ';
update TEACHER set SALARY = 1120 where TEACHER = 'ÐÌÍÊ';
update TEACHER set SALARY = 1250 where TEACHER = 'ÏÑÒÂËÂ';
update TEACHER set SALARY = 333 where TEACHER = '?';
update TEACHER set SALARY = 1520 where TEACHER = 'ÃÐÍ';
update TEACHER set SALARY = 1430 where TEACHER = 'ÆËÊ';
update TEACHER set SALARY = 900 where TEACHER = 'ÁÐÒØÂ×';
update TEACHER set SALARY = 875 where TEACHER = 'ÞÄÍÊÂ';
update TEACHER set SALARY = 970 where TEACHER = 'ÁÐÍÂÑÊ';
update TEACHER set SALARY = 780 where TEACHER = 'ÍÂÐÂ';
update TEACHER set SALARY = 1150 where TEACHER = 'ÐÂÊ×';
update TEACHER set SALARY = 805 where TEACHER = 'ÄÌÄÊ';
update TEACHER set SALARY = 905 where TEACHER = 'ÌØÊÂÑÊ';
update TEACHER set SALARY = 1200 where TEACHER = 'ËÁÕ';
update TEACHER set SALARY = 1500 where TEACHER = 'ÇÂÃÖÂ';
update TEACHER set SALARY = 905 where TEACHER = 'ÁÇÁÐÄÂ';
update TEACHER set SALARY = 715 where TEACHER = 'ÏÐÊÏ×Ê';
update TEACHER set SALARY = 880 where TEACHER = 'ÍÑÊÂÖ';
update TEACHER set SALARY = 735 where TEACHER = 'ÌÕÂ';
update TEACHER set SALARY = 595 where TEACHER = 'ÅÙÍÊ';
update TEACHER set SALARY = 850 where TEACHER = 'ÆÐÑÊ';

--2
select regexp_substr(teacher_name, '(\S+)', 1, 1) || ' ' ||
       substr(regexp_substr(teacher_name, '(\S+)', 1, 2), 1, 1) || '. ' ||
       substr(regexp_substr(teacher_name, '(\S+)', 1, 3), 1, 1) || '. ' as FIO
from teacher;

--3
select * from teacher where TO_CHAR(birthday, 'd') = 2;

--4
--drop view next_month_birthday

create or replace view next_month_birthday as
select * from teacher where TO_CHAR(birthday, 'mm') = (select TO_CHAR(last_day(sysdate) + 1, 'mm') from dual);

select * from next_month_birthday;

--5
--drop view teacher_number_a_month

create or replace view teacher_number_a_month as
select TO_CHAR(birthday, 'Month') mon, count(*) num
    from teacher group by TO_CHAR(birthday, 'Month')
    order by num desc;
    
select * from teacher_number_a_month;

--6
declare
    cursor teacher_annyversary
        return teacher%rowtype is
        select *
        from teacher
        where MOD((TO_CHAR(sysdate, 'yyyy') - TO_CHAR(birthday, 'yyyy') + 1), 10) = 0;
    v_teacher  TEACHER%rowtype;
begin
    open teacher_annyversary;

    fetch teacher_annyversary into v_teacher;

    while (teacher_annyversary%found)
        loop
            dbms_output.put_line(v_teacher.teacher || ' ' || v_teacher.teacher_name || ' ' || v_teacher.pulpit || ' ' ||
                                 v_teacher.birthday || ' ' || v_teacher.salary);
            fetch teacher_annyversary into v_teacher;
        end loop;

    close teacher_annyversary;
end;


--7
declare
    cursor teachers_avg_salary is
        select pulpit, floor(avg(salary)) as AVG_SALARY
        from TEACHER
        group by pulpit;
    cursor faculty_avg_salary is
        select FACULTY, AVG(SALARY)
        from TEACHER
                 join PULPIT P on TEACHER.PULPIT = P.PULPIT
        group by FACULTY;
    cursor faculties_avg_salary is
        select AVG(SALARY)
        from TEACHER;
        
    m_pulpit  TEACHER.PULPIT%type;
    m_salary  TEACHER.SALARY%type;
    m_faculty PULPIT.FACULTY%type;
begin

    dbms_output.put_line('Pulpuits:');
    open teachers_avg_salary;
    fetch teachers_avg_salary into m_pulpit, m_salary;

    while (teachers_avg_salary%found)
        loop
            dbms_output.put_line(m_pulpit || ' ' || m_salary);
            fetch teachers_avg_salary into m_pulpit, m_salary;
        end loop;
    close teachers_avg_salary;

    dbms_output.put_line('');
    dbms_output.put_line('Faculties:');
    open faculty_avg_salary;
    fetch faculty_avg_salary into m_faculty, m_salary;

    while (faculty_avg_salary%found)
        loop
            dbms_output.put_line(m_faculty || ' ' || m_salary);
            fetch faculty_avg_salary into m_faculty, m_salary;
        end loop;
    close faculty_avg_salary;

    dbms_output.put_line('');
    dbms_output.put_line('All facultes:');
    open faculties_avg_salary;
    fetch faculties_avg_salary into m_salary;
    dbms_output.put_line(round(m_salary, 2));
    close faculties_avg_salary;
end;

--8
DECLARE
    TYPE Address_record IS RECORD (
        street   VARCHAR2(100),
        city     VARCHAR2(50)
    );

    TYPE Emp_with_address_record IS RECORD (
        emp_id   NUMBER(6),
        first_name VARCHAR2(50),
        last_name  VARCHAR2(50),
        address    address_record
    );

    emp_with_address Emp_with_address_record;
    emp_address Address_record;
BEGIN
    emp_with_address.emp_id := 203;
    emp_with_address.first_name := 'Valentine';
    emp_with_address.last_name := 'Korneliuk';
    
    emp_address := address_record('Mayakovskovo', 'Minsk');  
    emp_with_address.address := emp_address;

    DBMS_OUTPUT.PUT_LINE('Employee ID: ' || emp_with_address.emp_id);
    DBMS_OUTPUT.PUT_LINE('Name: ' || emp_with_address.first_name || ' ' || emp_with_address.last_name);
    DBMS_OUTPUT.PUT_LINE('Address: ' || emp_with_address.address.street || ', ' || 
                         emp_with_address.address.city);
END;




















