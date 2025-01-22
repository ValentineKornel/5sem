-- DROP TABLE FACULTY
CREATE TABLE FACULTY
(
    FACULTY      CHAR(10) NOT NULL,
    FACULTY_NAME VARCHAR2(100),
    CONSTRAINT PK_FACULTY PRIMARY KEY (FACULTY)
);

truncate table FACULTY;
insert into FACULTY (FACULTY, FACULTY_NAME)
values ('����', '����������� ���� � ����������');
insert into FACULTY (FACULTY, FACULTY_NAME)
values ('����', '���������� ���������� � �������');
insert into FACULTY (FACULTY, FACULTY_NAME)
values ('���', '����������������� ���������');
insert into FACULTY (FACULTY, FACULTY_NAME)
values ('���', '���������-������������� ���������');
insert into FACULTY (FACULTY, FACULTY_NAME)
values ('����', '���������� � ������� ������ ��������������');
insert into FACULTY (FACULTY, FACULTY_NAME)
values ('���', '���������� ������������ �������');

--------------------------------------------------------------------------------------------
-- DROP TABLE PULPIT
CREATE TABLE PULPIT
(
    PULPIT      CHAR(20) NOT NULL,
    PULPIT_NAME VARCHAR2(160),
    FACULTY     CHAR(10) NOT NULL,
    CONSTRAINT FK_PULPIT_FACULTY FOREIGN KEY (FACULTY) REFERENCES FACULTY (FACULTY),
    CONSTRAINT PK_PULPIT PRIMARY KEY (PULPIT)
);

truncate table PULPIT;
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('����', '�������������� ������ � ���������� ', '����');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('������', '���������������� ������������ � ������ ��������� ���������� ', '����');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('��', '�����������', '���');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('��', '������������', '���');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('��', '��������������', '���');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('�����', '���������� � ����������������', '���');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('������', '������������ �������������� � ������-��������� �������������', '���');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('��', '���������� ����', '����');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('�����', '������ ����� � ���������� �������������', '����');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('��', '������������ �����', '���');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('��������', '���������� ���������������� ������� � ����������� ���������� ����������', '���');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('�������', '���������� �������������� ������� � ����� ���������� ���������� ', '����');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('��������', '�����, ���������� ����������������� ����������� � ���������� ����������� �������', '����');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('����', '������������� ������ � ����������', '���');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('����', '����������� � ��������� ������������������', '���');
------------------------------------------------------------------------------------------------------------------------
-- DROP  TABLE TEACHER
CREATE TABLE TEACHER
(
    TEACHER      CHAR(20) NOT NULL,
    TEACHER_NAME VARCHAR2(100),
    PULPIT       CHAR(20) NOT NULL,
    CONSTRAINT PK_TEACHER PRIMARY KEY (TEACHER),
    CONSTRAINT FK_TEACHER_PULPIT FOREIGN KEY (PULPIT) REFERENCES PULPIT (PULPIT)
);

truncate table TEACHER;
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('����', '������ �������� �������������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('�����', '�������� ��������� ��������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('�����', '���������� ������ ����������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('����', '������ ���� �����������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('����', '������� �������� ��������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('�����', '�������� ������ ���������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('���', '����� ��������� ����������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('���', '������� ��������� �����������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('���', '��������� ����� ��������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('����', '��������� ������� ����������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('������', '����������� ������� ����������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('?', '�����������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('���', '����� ������� ��������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('���', '����� ������� �������������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('������', '���������� ��������� �������������', '������');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('�����', '������� ������ ����������', '������');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('������', '����������� ��������� ��������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('����', '������� ��������� ����������', '����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('����', '������ ������ ��������', '��');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('����', '������� ������ ����������', '������');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('������', '���������� �������� ��������', '��');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('���', '������ ���������� ������������', '��');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('�����', '��������� �������� ���������', '�����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('������', '���������� �������� ����������', '��');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('������', '��������� ������� ���������', '��������');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('�����', '�������� ������ ����������', '��');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('���', '����� ������ ��������', '�����');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('����', '������ ������� ���������', '�������');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('����', '������� ���� ����������', '��������');
---------------------------------------------------------------------------------------------------------------------
-- DROP TABLE SUBJECT
CREATE TABLE SUBJECT
(
    SUBJECT      CHAR(20)      NOT NULL,
    SUBJECT_NAME VARCHAR2(100) NOT NULL,
    PULPIT       CHAR(20)      NOT NULL,
    CONSTRAINT PK_SUBJECT PRIMARY KEY (SUBJECT),
    CONSTRAINT FK_SUBJECT_PULPIT FOREIGN KEY (PULPIT) REFERENCES PULPIT (PULPIT)
);

truncate table SUBJECT;
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('����', '������� ���������� ������ ������', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('��', '���� ������', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('���', '�������������� ����������', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('����', '������ �������������� � ����������������', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('��', '������������� ������ � ������������ ��������', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('���', '��������������� ������� ����������', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('����', '������������� ������ ��������� ����������', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('���', '�������������� �������������� ������', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('��', '������������ ��������� ', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('�����', '��������������� ������, �������� � �������� �����', '������');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('���', '������������ �������������� �������', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('���', '����������� ���������������� ������������', '������');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('��', '���������� ���������', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('��', '�������������� ����������������', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('����', '���������� ������ ���', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('���', '��������-��������������� ����������������', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('��', '��������� ������������������', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('��', '������������� ������', '����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('������OO', '�������� ������ ������ � ���� � ���. ������.', '��');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('�������', '������ ��������������� � ������������� ���������', '������');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('��', '���������� �������� ', '��');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('��', '�����������', '�����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('��', '������������ �����', '��');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('���', '���������� ��������� �������', '��������');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('���', '������ ��������� ����', '��');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('����', '���������� � ������������ �������������', '�����');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('����', '���������� ���������� �������� ���������� ', '�������');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('���', '���������� ������������', '��������');
---------------------------------------------------------------------------------------------------------------------
-- DROP TABLE AUDITORIUM_TYPE
create table AUDITORIUM_TYPE
(
    AUDITORIUM_TYPE     char(10)
        constraint AUDITORIUM_TYPE_PK primary key,
    AUDITORIUM_TYPENAME varchar2(100)
        constraint AUDITORIUM_TYPENAME_NOT_NULL not null
);

truncate table AUDITORIUM_TYPE;
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME)
values ('��', '����������');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME)
values ('��-�', '������������ �����');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME)
values ('��-�', '���������� � ���. ������������');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME)
values ('��-X', '���������� �����������');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME)
values ('��-��', '����. ������������ �����');
---------------------------------------------------------------------------------------------------------------------
-- DROP TABLE AUDITORIUM
create table AUDITORIUM
(
    AUDITORIUM          char(10) primary key, -- ��� ���������
    AUDITORIUM_NAME     varchar2(200),        -- ���������
    AUDITORIUM_CAPACITY number(4),            -- �����������
    AUDITORIUM_TYPE     char(10) not null     -- ��� ���������
        references AUDITORIUM_TYPE (AUDITORIUM_TYPE)
);

truncate table AUDITORIUM;
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('206-1', '206-1', '��-�', 15);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('301-1', '301-1', '��-�', 15);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('236-1', '236-1', '��', 60);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('313-1', '313-1', '��', 60);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('324-1', '324-1', '��', 50);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('413-1', '413-1', '��-�', 15);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('423-1', '423-1', '��-�', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('408-2', '408-2', '��', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('103-4', '103-4', '��', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('105-4', '105-4', '��', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('107-4', '107-4', '��', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('110-4', '110-4', '��', 30);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('111-4', '111-4', '��', 30);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('114-4', '114-4', '��-�', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('132-4', '132-4', '��', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('02�-4', '02�-4', '��', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('229-4', '229-4', '��', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('304-4', '304-4', '��-�', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('314-4', '314-4', '��', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('320-4', '320-4', '��', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('429-4', '429-4', '��', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('311-1', '311-1', '��', 90);
-----------------------------------------------------------------------------------------------------------------------
commit;



--1
begin
    null;
end;

--2
begin
    dbms_output.put_line('Hello world!');
end;

--3
declare
    a number(3) := 3;
    b number(3) := 0;
    c number(3);
begin
    c := a / b;
    dbms_output.put_line('c = ' || c);
exception
    when others
        then dbms_output.put_line(sqlcode || ': error = ' || sqlerrm);
end;

--4
declare
    a number(3) := 3;
    b number(3) := 0;
    c number(3);
begin
    dbms_output.put_line('a = ' || a || ', b = ' || b);
    begin
        c := a / b;
    exception
        when others
            then dbms_output.put_line('error = ' || sqlerrm);
    end;
    dbms_output.put_line('c = ' || c);
end;

--5
--sys
ALTER SESSION SET CONTAINER = "XEPDB1";
select name, value from v$parameter where name = 'plsql_warnings';

--6
select keyword from v$reserved_words where length = 1;

--7
select keyword from v$reserved_words where length > 1 order by keyword;

--8
select name, value from v$parameter where name like 'plsql%';
--show parameter plsql;

--9 - 17
declare
    var1     number(3)      := 50;
    var2     number(3)      := 15;
    sum_var number(10, 2);
    diff_var number(10, 2);
    mult_var number(10, 2);
    dwr     number(10, 2);
    var3     number(10, 2)  := 2.11;
    var4     number(10, -3) := 222999.45;
    var5     binary_float   := 123456789.123456789;
    var6     binary_double  := 123456789.123456789;
    var7     number(38, 10) := 12345E+10;
    var8     boolean        := true;
begin
    sum_var := var1 + var2;
    diff_var := var1 - var2;
    mult_var := var1 * var2;
    dwr := mod(var1, var2);

    dbms_output.put_line('t10 = ' || var1);
    dbms_output.put_line('t11 = ' || var2);
    dbms_output.put_line('rem = ' || dwr);
    dbms_output.put_line('sum = ' || sum_var);
    dbms_output.put_line('diff = ' || diff_var);
    dbms_output.put_line('mult = ' || mult_var);
    dbms_output.put_line('fixed = ' || var3);
    dbms_output.put_line('rounded = ' || var4);
    dbms_output.put_line('binary float = ' || var5);
    dbms_output.put_line('binary double = ' || var6);
    dbms_output.put_line('E+10 = ' || var7);
    if var8
    then
        dbms_output.put_line('bool = ' || 'true');
    end if;
end;

--18
DECLARE
    var_ch CONSTANT VARCHAR2(50) := 'Hello, PL/SQL!';
    ch CONSTANT CHAR(1) := 'A';
    numb CONSTANT NUMBER := 42;

    v_message VARCHAR2(100);
    v_number_result NUMBER;
BEGIN
    v_message := var_ch || ' Initial letter is: ' || ch;
    dbms_output.put_line(v_message);

    v_number_result := numb * 2;
    dbms_output.put_line('Double the number: ' || v_number_result);

    v_number_result := numb + 10;
    dbms_output.put_line('Number plus 10: ' || v_number_result);

    IF numb > 30 THEN
        dbms_output.put_line('The number is greater than 30.');
    ELSE
        dbms_output.put_line('The number is less than or equal to 30.');
    END IF;
END;

--19
declare
    pulp pulpit.pulpit%type;
begin
    pulp := '��';
    dbms_output.put_line(pulp);
end;

--20
declare
    faculty_res faculty%rowtype;
begin
    faculty_res.faculty := '��';
    faculty_res.faculty_name := '��������� �������������� ����������';
    dbms_output.put_line(faculty_res.faculty || ' - ' || faculty_res.faculty_name);
end;

--21
declare
    x pls_integer := 5;
begin
    if x < 10 then
        dbms_output.put_line('x < 10');
    elsif x > 10 then
        dbms_output.put_line('x > 10');
    else
        dbms_output.put_line('x = 10');
    end if;
end;

--23
declare
    x pls_integer := 21;
begin
    case
        when x between 10 and 20 then dbms_output.put_line('10 <= x <= 20');
        when x between 21 and 40 then dbms_output.put_line('21 <= x <= 40');
        else dbms_output.put_line('else block');
        end case;
end;

--24
declare
    x pls_integer := 0;
begin
    dbms_output.put_line('LOOP: ');
    loop
        x := x + 1;
        dbms_output.put_line(x);
        exit when x >= 10;
    end loop;
end;

--25
declare
    x pls_integer := 10;
begin
    dbms_output.put_line('WHILE: ');
    while (x > 0)
        loop
            dbms_output.put_line(x);
            x := x - 1;
        end loop;
end;

--26
begin
    dbms_output.put_line('FOR: ');
    for k in 1..10
        loop
            dbms_output.put_line(k);
        end loop;
end;















