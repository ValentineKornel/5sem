----------------------------fill Booking table with 100 000 rows;
CREATE OR REPLACE PROCEDURE fill_booking_table_for_one_client(
    p_client_id IN NUMBER,
    p_master_id IN NUMBER
) AS
    v_start_date DATE := TRUNC(SYSDATE - 1);
    v_end_date DATE := ADD_MONTHS(v_start_date, -12);
    v_current_date DATE;
    v_current_time TIMESTAMP;
BEGIN
    v_current_date := v_start_date;

    WHILE v_current_date > v_end_date LOOP
        FOR hour IN 0..23 LOOP
            v_current_time := TO_TIMESTAMP(v_current_date, 'YYYY-MM-DD') 
                 + (hour / 24) 
                 + (TRUNC(DBMS_RANDOM.VALUE(0, 60)) / 1440);

            INSERT INTO Booking (
                client_id, master_id, service, booking_date, time, booked
            ) VALUES (
                p_client_id, p_master_id, 'filling service', v_current_date, v_current_time, 1
            );
        END LOOP;

        v_current_date := v_current_date - 1;
    END LOOP;

    COMMIT;
EXCEPTION
    WHEN OTHERS THEN
        dbms_output.put_line('error filling booking table for client ' || sqlerrm);
END;


CREATE OR REPLACE PROCEDURE fill_booking_table IS
BEGIN
    fill_booking_table_for_one_client(7, 1);
    fill_booking_table_for_one_client(8, 2);
    fill_booking_table_for_one_client(9, 3);
    fill_booking_table_for_one_client(10, 4);
    fill_booking_table_for_one_client(21, 5);
    fill_booking_table_for_one_client(22, 1);
    fill_booking_table_for_one_client(23, 2);
    fill_booking_table_for_one_client(24, 3);
    fill_booking_table_for_one_client(25, 4);
    fill_booking_table_for_one_client(26, 5);
    fill_booking_table_for_one_client(27, 1);
    fill_booking_table_for_one_client(28, 2);
END;

begin
    fill_booking_table();
end;

select count(*) from booking;
select * from booking;
truncate table Booking;
select * from Users;




