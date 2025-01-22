--1
declare 
    procedure GET_TEACHERS(PCODE TEACHER.PULPIT%TYPE)
    is
    cursor GetTeachers is select TEACHER, TEACHER_NAME, PULPIT
                          from TEACHER
                          where PULPIT = PCODE;
        m_teacher      TEACHER.TEACHER%TYPE;
        m_teacher_name TEACHER.TEACHER_NAME%TYPE;
        m_pulpit       TEACHER.PULPIT%TYPE;
    begin
        open GetTeachers;
        fetch GetTeachers into m_teacher, m_teacher_name, m_pulpit;
        DBMS_OUTPUT.PUT_LINE(m_teacher || ' ' || m_teacher_name || ' ' || m_pulpit);

        while (GetTeachers%found)
            loop
                DBMS_OUTPUT.PUT_LINE(m_teacher || ' ' || m_teacher_name || ' ' || m_pulpit);
                fetch GetTeachers into m_teacher, m_teacher_name, m_pulpit;
            end loop;
        close GetTeachers;
    end GET_TEACHERS;
begin
    GET_TEACHERS('»—Ë“');
end;

--2
declare
    res number;
    function GET_AVG_SALARY(PCODE TEACHER.PULPIT%TYPE)
    return number is 
        result_salary number;
    begin
        select AVG(SALARY) into result_salary from TEACHER where PULPIT = PCODE;
        return result_salary;
    end GET_AVG_SALARY;
    
begin
    res := GET_AVG_SALARY('»—Ë“');
    DBMS_OUTPUT.PUT_LINE(res);
end;

--3
declare 
    res number;
    function GET_NUM_TEACHERS (PCODE TEACHER.PULPIT%TYPE)
    return number
    is
        result_num number;
    begin
        select count(TEACHER) into result_num from TEACHER where PULPIT=PCODE;
        return result_num;
    end GET_NUM_TEACHERS;
begin
    res := GET_NUM_TEACHERS('»—Ë“');
    DBMS_OUTPUT.PUT_LINE(res);
end;

--4
create or replace procedure GET_TEACHERS(FCODE FACULTY.FACULTY%TYPE)
    is
    cursor GetTeachers is
        select TEACHER, TEACHER_NAME, P.PULPIT
        from TEACHER inner join PULPIT P on P.PULPIT = TEACHER.PULPIT
        where FACULTY = FCODE;

    m_teacher      TEACHER.TEACHER%TYPE;
    m_teacher_name TEACHER.TEACHER_NAME%TYPE;
    m_pulpit       TEACHER.PULPIT%TYPE;
begin
    open GetTeachers;
    fetch GetTeachers into m_teacher, m_teacher_name, m_pulpit;

    while (GetTeachers%found)
    loop
        DBMS_OUTPUT.PUT_LINE(m_teacher || ' ' || m_teacher_name || ' ' || m_pulpit);
        fetch GetTeachers into m_teacher, m_teacher_name, m_pulpit;
    end loop;

    close GetTeachers;
end GET_TEACHERS;

begin
    GET_TEACHERS('“Œ¬');
end;

create or replace procedure GET_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE)
is
    cursor GetSubjects is
    select * from SUBJECT where PULPIT=PCODE;

    m_subject SUBJECT.SUBJECT%type;
    m_subject_name SUBJECT.SUBJECT_NAME%type;
    m_pulpit SUBJECT.PULPIT%type;
begin
    open GetSubjects;
    fetch GetSubjects into m_subject, m_subject_name, m_pulpit;

    while (GetSubjects%found)
    loop
        DBMS_OUTPUT.PUT_LINE(m_subject || ' ' || m_subject_name || ' ' || m_pulpit);
        fetch GetSubjects into m_subject, m_subject_name, m_pulpit;
    end loop;
    close GetSubjects;

end GET_SUBJECTS;

begin
    GET_SUBJECTS('»—Ë“');
end;

--5
declare 
    res number;
    function FGET_NUM_TEACHERS (FCODE FACULTY.FACULTY%TYPE)
    return number
    is
        result_num number;
    begin
        select count(TEACHER) into result_num 
                                from TEACHER T 
                                inner join PULPIT P 
                                on T.PULPIT = P.PULPIT 
                                where FACULTY = FCODE;
        return result_num;
    end FGET_NUM_TEACHERS;

begin
    res := FGET_NUM_TEACHERS('»ƒËœ');
    DBMS_OUTPUT.PUT_LINE(res);
end;

declare 
    res number;
    function GET_NUM_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE) return number
    is
        result_num number;
    begin
        select count(SUBJECT) into result_num from SUBJECT where PULPIT=PCODE;
        return result_num;
    end GET_NUM_SUBJECTS;
begin
    res := GET_NUM_SUBJECTS('»—Ë“');
    DBMS_OUTPUT.PUT_LINE(res);
end;

--6
create or replace package TEACHERS as
  procedure GET_TEACHERS(FCODE FACULTY.FACULTY%type);
  procedure GET_SUBJECTS (PCODE SUBJECT.PULPIT%type);
  function GET_NUM_TEACHERS(FCODE FACULTY.FACULTY%type) return number;
  function GET_NUM_SUBJECTS(PCODE SUBJECT.PULPIT%type) return number;
end TEACHERS;

create or replace package body TEACHERS
is
    procedure GET_TEACHERS(FCODE FACULTY.FACULTY%TYPE)
        is
        cursor GetTeachers is
            select TEACHER, TEACHER_NAME, P.PULPIT
            from TEACHER inner join PULPIT P on P.PULPIT = TEACHER.PULPIT
            where FACULTY = FCODE;
    
        m_teacher      TEACHER.TEACHER%TYPE;
        m_teacher_name TEACHER.TEACHER_NAME%TYPE;
        m_pulpit       TEACHER.PULPIT%TYPE;
    begin
        open GetTeachers;
        fetch GetTeachers into m_teacher, m_teacher_name, m_pulpit;
    
        while (GetTeachers%found)
        loop
            DBMS_OUTPUT.PUT_LINE(m_teacher || ' ' || m_teacher_name || ' ' || m_pulpit);
            fetch GetTeachers into m_teacher, m_teacher_name, m_pulpit;
        end loop;
    
        close GetTeachers;
    end GET_TEACHERS;
    
    procedure GET_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE)
    is
        cursor GetSubjects is
        select * from SUBJECT where PULPIT=PCODE;
    
        m_subject SUBJECT.SUBJECT%type;
        m_subject_name SUBJECT.SUBJECT_NAME%type;
        m_pulpit SUBJECT.PULPIT%type;
    begin
        open GetSubjects;
        fetch GetSubjects into m_subject, m_subject_name, m_pulpit;
    
        while (GetSubjects%found)
        loop
            DBMS_OUTPUT.PUT_LINE(m_subject || ' ' || m_subject_name || ' ' || m_pulpit);
            fetch GetSubjects into m_subject, m_subject_name, m_pulpit;
        end loop;
        close GetSubjects;
    
    end GET_SUBJECTS;
    
    function GET_NUM_TEACHERS (FCODE FACULTY.FACULTY%TYPE)
    return number
    is
        result_num number;
    begin
        select count(TEACHER) into result_num 
                                from TEACHER T 
                                inner join PULPIT P 
                                on T.PULPIT = P.PULPIT 
                                where FACULTY = FCODE;
        return result_num;
    end GET_NUM_TEACHERS;
    
    function GET_NUM_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE) return number
    is
        result_num number;
    begin
        select count(SUBJECT) into result_num from SUBJECT where PULPIT=PCODE;
        return result_num;
    end GET_NUM_SUBJECTS;
    
end TEACHERS;

--7
begin
  DBMS_OUTPUT.PUT_LINE('Teachers amount at faculty: ' || TEACHERS.GET_NUM_TEACHERS('»ƒËœ'));
  DBMS_OUTPUT.PUT_LINE('Teachers amount at pulpit: ' || TEACHERS.GET_NUM_SUBJECTS('»—Ë“'));
  TEACHERS.GET_TEACHERS('“Œ¬');
  DBMS_OUTPUT.PUT_LINE('');
  TEACHERS.GET_SUBJECTS('»—Ë“');
end;























