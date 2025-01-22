------------------------------data export/import
CREATE OR REPLACE PROCEDURE import_bookings(file_name IN VARCHAR2) AS
    file_handle UTL_FILE.FILE_TYPE;
    v_id NUMBER;
    v_client_id NUMBER;
    v_master_id NUMBER;
    v_service VARCHAR2(255);
    v_booking_date DATE;
    v_time TIMESTAMP;
    v_booked NUMBER(1);
BEGIN
    file_handle := UTL_FILE.FOPEN('JSON_DIR', file_name, 'R', 500);
    
    WHILE TRUE LOOP
        DECLARE
            line VARCHAR2(500);
        BEGIN
            UTL_FILE.GET_LINE(file_handle, line);

            SELECT id, client_id, master_id, service, booking_date, time, booked
            INTO v_id, v_client_id, v_master_id, v_service, v_booking_date, v_time, v_booked
            FROM JSON_TABLE(
                line,
                '$' COLUMNS (
                    id NUMBER PATH '$.id',
                    client_id NUMBER PATH '$.client_id',
                    master_id NUMBER PATH '$.master_id',
                    service VARCHAR2(255) PATH '$.service',
                    booking_date DATE PATH '$.booking_date',
                    time TIMESTAMP PATH '$.time',
                    booked NUMBER(1) PATH '$.booked'
                )
            );

            IF v_id IS NULL THEN
                INSERT INTO Booking (client_id, master_id, service, booking_date, time, booked)
                VALUES (v_client_id, v_master_id, v_service, v_booking_date, v_time, v_booked);
            ELSE
                INSERT INTO Booking (id, client_id, master_id, service, booking_date, time, booked)
                VALUES (v_id, v_client_id, v_master_id, v_service, v_booking_date, v_time, v_booked);
            END IF;
        EXCEPTION
            WHEN NO_DATA_FOUND THEN
                EXIT; -- Выходим из цикла, если достигнут конец файла
            WHEN OTHERS THEN
                RAISE_APPLICATION_ERROR(-20001, 'Error importing data: ' || SQLERRM);
        END;
    END LOOP;

    UTL_FILE.FCLOSE(file_handle);
    DBMS_OUTPUT.PUT_LINE('import done');
    COMMIT;
EXCEPTION
    WHEN OTHERS THEN
        UTL_FILE.FCLOSE_ALL;
        RAISE;
END;

CREATE OR REPLACE PROCEDURE export_bookings(file_name IN VARCHAR2) AS
    file_handle UTL_FILE.FILE_TYPE;
BEGIN
    file_handle := UTL_FILE.FOPEN('JSON_DIR', file_name, 'W', 500);

    FOR record IN (
        SELECT id, client_id, master_id, service, booking_date, time, booked
        FROM Booking
    ) LOOP
        DECLARE
            json_record CLOB;
        BEGIN
            SELECT JSON_OBJECT(
                       'id' VALUE record.id,
                       'client_id' VALUE record.client_id,
                       'master_id' VALUE record.master_id,
                       'service' VALUE record.service,
                       'booking_date' VALUE record.booking_date,
                       'time' VALUE record.time,
                       'booked' VALUE record.booked
                   )
            INTO json_record
            FROM DUAL;

            UTL_FILE.PUT_LINE(file_handle, json_record);
        END;
    END LOOP;

    UTL_FILE.FCLOSE(file_handle);
EXCEPTION
    WHEN OTHERS THEN
        UTL_FILE.FCLOSE_ALL;
        RAISE_APPLICATION_ERROR(-20001, 'Error exporting data: ' || SQLERRM);
END;


select * from Booking;
select count(*) from Booking;
truncate table Booking;



