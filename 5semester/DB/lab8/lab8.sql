-- DROP TABLE FACULTY
CREATE TABLE FACULTY
(
    FACULTY      CHAR(10) NOT NULL,
    FACULTY_NAME VARCHAR2(100),
    CONSTRAINT PK_FACULTY PRIMARY KEY (FACULTY)
);

truncate table FACULTY;
insert into FACULTY (FACULTY, FACULTY_NAME)
values ('ИДиП', 'Издателькое дело и полиграфия');
insert into FACULTY (FACULTY, FACULTY_NAME)
values ('ХТиТ', 'Химическая технология и техника');
insert into FACULTY (FACULTY, FACULTY_NAME)
values ('ЛХФ', 'Лесохозяйственный факультет');
insert into FACULTY (FACULTY, FACULTY_NAME)
values ('ИЭФ', 'Инженерно-экономический факультет');
insert into FACULTY (FACULTY, FACULTY_NAME)
values ('ТТЛП', 'Технология и техника лесной промышленности');
insert into FACULTY (FACULTY, FACULTY_NAME)
values ('ТОВ', 'Технология органических веществ');

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
values ('ИСиТ', 'Информационных систем и технологий ', 'ИДиП');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ПОиСОИ', 'Полиграфического оборудования и систем обработки информации ', 'ИДиП');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ЛВ', 'Лесоводства', 'ЛХФ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ОВ', 'Охотоведения', 'ЛХФ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ЛУ', 'Лесоустройства', 'ЛХФ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ЛЗиДВ', 'Лесозащиты и древесиноведения', 'ЛХФ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ЛПиСПС', 'Ландшафтного проектирования и садово-паркового строительства', 'ЛХФ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ТЛ', 'Транспорта леса', 'ТТЛП');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ЛМиЛЗ', 'Лесных машин и технологии лесозаготовок', 'ТТЛП');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ОХ', 'Органической химии', 'ТОВ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ТНХСиППМ', 'Технологии нефтехимического синтеза и переработки полимерных материалов', 'ТОВ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ТНВиОХТ', 'Технологии неорганических веществ и общей химической технологии ', 'ХТиТ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ХТЭПиМЭЕ', 'Химии, технологии электрохимических производств и материалов электронной техники', 'ХТиТ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('ЭТиМ', 'экономической теории и маркетинга', 'ИЭФ');
insert into PULPIT (PULPIT, PULPIT_NAME, FACULTY)
values ('МиЭП', 'Менеджмента и экономики природопользования', 'ИЭФ');
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
values ('СМЛВ', 'Смелов Владимир Владиславович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('АКНВЧ', 'Акунович Станислав Иванович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('КЛСНВ', 'Колесников Леонид Валерьевич', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('ГРМН', 'Герман Олег Витольдович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('ЛЩНК', 'Лащенко Анатолий Пвалович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('БРКВЧ', 'Бракович Андрей Игорьевич', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('ДДК', 'Дедко Александр Аркадьевич', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('КБЛ', 'Кабайло Александр Серафимович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('УРБ', 'Урбанович Павел Павлович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('РМНК', 'Романенко Дмитрий Михайлович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('ПСТВЛВ', 'Пустовалова Наталия Николаевна', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('?', 'Неизвестный', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('ГРН', 'Гурин Николай Иванович', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('ЖЛК', 'Жиляк Надежда Александровна', 'ИСиТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('БРТШВЧ', 'Барташевич Святослав Александрович', 'ПОиСОИ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('ЮДНКВ', 'Юденков Виктор Степанович', 'ПОиСОИ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('БРНВСК', 'Барановский Станислав Иванович', 'ЭТиМ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('НВРВ', 'Неверов Александр Васильевич', 'МиЭП');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('РВКЧ', 'Ровкач Андрей Иванович', 'ОВ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('ДМДК', 'Демидко Марина Николаевна', 'ЛПиСПС');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('МШКВСК', 'Машковский Владимир Петрович', 'ЛУ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('ЛБХ', 'Лабоха Константин Валентинович', 'ЛВ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('ЗВГЦВ', 'Звягинцев Вячеслав Борисович', 'ЛЗиДВ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('БЗБРДВ', 'Безбородов Владимир Степанович', 'ОХ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('ПРКПЧК', 'Прокопчук Николай Романович', 'ТНХСиППМ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('НСКВЦ', 'Насковец Михаил Трофимович', 'ТЛ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('МХВ', 'Мохов Сергей Петрович', 'ЛМиЛЗ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('ЕЩНК', 'Ещенко Людмила Семеновна', 'ТНВиОХТ');
insert into TEACHER (TEACHER, TEACHER_NAME, PULPIT)
values ('ЖРСК', 'Жарский Иван Михайлович', 'ХТЭПиМЭЕ');
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
values ('СУБД', 'Системы управления базами данных', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('БД', 'Базы данных', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ИНФ', 'Информацтонные технологии', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ОАиП', 'Основы алгоритмизации и программирования', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ПЗ', 'Представление знаний в компьютерных системах', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ПСП', 'Пограммирование сетевых приложений', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('МСОИ', 'Моделирование систем обработки информации', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ПИС', 'Проектирование информационных систем', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('КГ', 'Компьютерная геометрия ', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ПМАПЛ', 'Полиграфические машины, автоматы и поточные линии', 'ПОиСОИ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('КМС', 'Компьютерные мультимедийные системы', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ОПП', 'Организация полиграфического производства', 'ПОиСОИ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ДМ', 'Дискретная матеатика', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('МП', 'Математисеское программирование', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ЛЭВМ', 'Логические основы ЭВМ', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ООП', 'Объектно-ориентированное программирование', 'ИСиТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ЭП', 'Экономика природопользования', 'МиЭП');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ЭТ', 'Экономическая теория', 'ЭТиМ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('БЛЗиПсOO', 'Биология лесных зверей и птиц с осн. охотов.', 'ОВ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ОСПиЛПХ', 'Основы садовопаркового и лесопаркового хозяйства', 'ЛПиСПС');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ИГ', 'Инженерная геодезия ', 'ЛУ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ЛВ', 'Лесоводство', 'ЛЗиДВ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ОХ', 'Органическая химия', 'ОХ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ТРИ', 'Технология резиновых изделий', 'ТНХСиППМ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ВТЛ', 'Водный транспорт леса', 'ТЛ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ТиОЛ', 'Технология и оборудование лесозаготовок', 'ЛМиЛЗ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ТОПИ', 'Технология обогащения полезных ископаемых ', 'ТНВиОХТ');
insert into SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
values ('ПЭХ', 'Прикладная электрохимия', 'ХТЭПиМЭЕ');
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
values ('ЛК', 'Лекционная');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME)
values ('ЛБ-К', 'Компьютерный класс');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME)
values ('ЛК-К', 'Лекционная с уст. компьютерами');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME)
values ('ЛБ-X', 'Химическая лаборатория');
insert into AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDITORIUM_TYPENAME)
values ('ЛБ-СК', 'Спец. компьютерный класс');
---------------------------------------------------------------------------------------------------------------------
-- DROP TABLE AUDITORIUM
create table AUDITORIUM
(
    AUDITORIUM          char(10) primary key, -- код аудитории
    AUDITORIUM_NAME     varchar2(200),        -- аудитория
    AUDITORIUM_CAPACITY number(4),            -- вместимость
    AUDITORIUM_TYPE     char(10) not null     -- тип аудитории
        references AUDITORIUM_TYPE (AUDITORIUM_TYPE)
);

truncate table AUDITORIUM;
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('206-1', '206-1', 'ЛБ-К', 15);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('301-1', '301-1', 'ЛБ-К', 15);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('236-1', '236-1', 'ЛК', 60);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('313-1', '313-1', 'ЛК', 60);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('324-1', '324-1', 'ЛК', 50);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('413-1', '413-1', 'ЛБ-К', 15);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('423-1', '423-1', 'ЛБ-К', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('408-2', '408-2', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('103-4', '103-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('105-4', '105-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('107-4', '107-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('110-4', '110-4', 'ЛК', 30);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('111-4', '111-4', 'ЛК', 30);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('114-4', '114-4', 'ЛК-К', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('132-4', '132-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('02Б-4', '02Б-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('229-4', '229-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('304-4', '304-4', 'ЛБ-К', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('314-4', '314-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('320-4', '320-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('429-4', '429-4', 'ЛК', 90);
insert into AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_TYPE, AUDITORIUM_CAPACITY)
values ('311-1', '311-1', 'ЛК', 90);
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
    pulp := 'ПИ';
    dbms_output.put_line(pulp);
end;

--20
declare
    faculty_res faculty%rowtype;
begin
    faculty_res.faculty := 'ИТ';
    faculty_res.faculty_name := 'Факультет информационных технологий';
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















