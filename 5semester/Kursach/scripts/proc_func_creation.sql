-----------------------Notification
create or replace type notification_type as object(
    id NUMBER,
    receiver_id NUMBER,
    creation_date DATE,
    message VARCHAR2(255),
    title VARCHAR2(255),
    watched NUMBER(1)
)
create or replace type notification_table as table of notification_type;

CREATE OR REPLACE FUNCTION find_notifications_by_receiver(
    p_receiver_id IN NUMBER
) RETURN notification_table IS
    notifications notification_table := notification_table();
BEGIN
    FOR r IN (SELECT id, receiver_id, creation_date, message, title, watched 
              FROM Notification 
              WHERE receiver_id = p_receiver_id) LOOP
        notifications.extend;
        notifications(notifications.count) := notification_type(r.id, r.receiver_id, r.creation_date, r.message, r.title, r.watched);
    END LOOP;
    RETURN notifications;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error trying find notification: ' || SQLERRM);
END;

select * from Users;

CREATE OR REPLACE PROCEDURE add_notification_to_user (
    p_receiver_id NUMBER,
    p_creation_date DATE,
    p_message VARCHAR2,
    p_title VARCHAR2,
    p_watched NUMBER
) AS
BEGIN
    INSERT INTO Notification (receiver_id, creation_date, message, title, watched)
    VALUES (p_receiver_id, p_creation_date, p_message, p_title, p_watched);
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error adding notification to user: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE mark_notifications_as_read (
    p_receiver_id NUMBER
) AS
BEGIN
    UPDATE Notification SET watched = 1 WHERE watched = 0 AND receiver_id = p_receiver_id;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error marking notifications as read: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION count_unread_notifications(
    p_receiver_id IN NUMBER
) RETURN NUMBER IS
    v_count NUMBER;
BEGIN
    select count(*) into v_count from Notification where receiver_id = p_receiver_id AND watched = 0;
    return v_count;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error counting unread notifications: ' || SQLERRM);
END;

drop type notification_type;
drop type notification_table;
drop FUNCTION find_notifications_by_receiver;
drop PROCEDURE add_notification_to_user;
drop PROCEDURE mark_notifications_as_read;
drop FUNCTION count_unread_notifications;


----------------------------Post
CREATE OR REPLACE TYPE post_type AS OBJECT (
    id NUMBER,
    creation_date DATE,
    post_image BLOB,
    description VARCHAR2(255),
    master_info_id NUMBER
);

CREATE OR REPLACE TYPE post_table AS TABLE OF post_type;

CREATE OR REPLACE FUNCTION find_post_by_id(
    p_id IN NUMBER
) RETURN post_table PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT id, creation_date, post_image, description, master_info_id from Post where id = p_id
    )LOOP
        PIPE ROW(post_type(rec.id, rec.creation_date, rec.post_image, rec.description, rec.master_info_id));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error finding post: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE delete_post (
    p_id NUMBER
) AS
BEGIN
    delete Post where id = p_id;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error deleting post: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE add_post (
    p_creation_date DATE,
    p_post_image BLOB,
    p_description VARCHAR2,
    p_master_info_id NUMBER
) AS
BEGIN
    INSERT INTO Post(creation_date, post_image, description, master_info_id)
    VALUES (p_creation_date, p_post_image, p_description, p_master_info_id);
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error adding post: ' || SQLERRM);
END;

drop type post_type;
drop type post_table;
drop FUNCTION find_post_by_id;
drop PROCEDURE delete_post;
drop PROCEDURE add_post;


------------------------Request
CREATE OR REPLACE TYPE request_type AS OBJECT (
    id NUMBER,
    creation_date DATE,
    user_id NUMBER,
    request_image BLOB,
    cover_letter VARCHAR2(500)
);
CREATE OR REPLACE TYPE reqeust_tabl AS TABLE OF request_type;

CREATE OR REPLACE FUNCTION find_request_by_id(
    p_id IN NUMBER
) RETURN reqeust_tabl PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT id, creation_date, user_id, request_image, cover_letter from Request where id = p_id
    )LOOP
        PIPE ROW(request_type(rec.id, rec.creation_date, rec.user_id, rec.request_image, rec.cover_letter));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error finding request: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE add_request (
    p_creation_date DATE,
    p_user_id NUMBER,
    p_request_image BLOB,
    p_cover_letter VARCHAR2
) AS
BEGIN
    INSERT INTO Request(creation_date, user_id, request_image, cover_letter)
    VALUES (p_creation_date, p_user_id, p_request_image, p_cover_letter);
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error adding request: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE delete_request (
    p_id NUMBER
) AS
BEGIN
    delete Request where id = p_id;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error deleting request: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION get_all_requests
RETURN reqeust_tabl PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT id, creation_date, user_id, request_image, cover_letter from Request
    )LOOP
        PIPE ROW(request_type(rec.id, rec.creation_date, rec.user_id, rec.request_image, rec.cover_letter));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error getting all requests: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION exists_request_by_user(p_user_id IN NUMBER) 
RETURN NUMBER IS
    v_count NUMBER;
BEGIN
    SELECT COUNT(*) INTO v_count
    FROM Request
    WHERE user_id = p_user_id;

    IF v_count > 0 THEN
        RETURN 1;
    ELSE
        RETURN 0;
    END IF;
END;

drop type request_type;
drop type reqeust_tabl;
drop FUNCTION find_request_by_id;
drop PROCEDURE add_request;
drop PROCEDURE delete_request;
drop FUNCTION get_all_requests;
drop FUNCTION exists_request_by_user;


-------------------Setting
CREATE OR REPLACE TYPE setting_type AS OBJECT (
    id NUMBER,
    account_id NUMBER,
    enable_two_step_authentication NUMBER(1)
);
CREATE OR REPLACE TYPE setting_table AS TABLE OF setting_type;

CREATE OR REPLACE FUNCTION find_setting_by_id(
    p_id IN NUMBER
) RETURN setting_table PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT id, account_id, enable_two_step_authentication from Setting where id = p_id
    )LOOP
        PIPE ROW(setting_type(rec.id, rec.account_id, rec.enable_two_step_authentication));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error finding setting: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE change_setting(
    p_id IN NUMBER,
    p_enable_two_step_authentication IN NUMBER
) AS
BEGIN
    UPDATE Setting set enable_two_step_authentication = p_enable_two_step_authentication where id = p_id;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error changing setting: ' || SQLERRM);
END;

drop type setting_type;
drop type setting_table;
drop FUNCTION find_setting_by_id;
drop PROCEDURE change_setting;


----------------------------------User
CREATE OR REPLACE TYPE user_type AS OBJECT (
    id NUMBER,
    username VARCHAR2(255),
    first_name VARCHAR2(255),
    second_name VARCHAR2(255),
    city VARCHAR2(255),
    email VARCHAR2(255),
    tel VARCHAR2(20),
    role VARCHAR2(50),
    profile_image BLOB,
    credential_id NUMBER,
    master_info_id NUMBER
);
CREATE OR REPLACE TYPE user_table AS TABLE OF user_type;

CREATE OR REPLACE FUNCTION find_user_by_id(
    p_id IN NUMBER
) RETURN user_table PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT id, username, first_name, second_name, city, email, tel, role, profile_image, credential_id, master_info_id from Users where id = p_id
    )LOOP
        PIPE ROW(user_type(rec.id, rec.username, rec.first_name, rec.second_name, rec.city, rec.email, rec.tel, rec.role, rec.profile_image, rec.credential_id, rec.master_info_id));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error finding user: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION find_user_by_username(
    p_username IN VARCHAR2
) RETURN user_table PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT id, username, first_name, second_name, city, email, tel, role, profile_image, credential_id, master_info_id from Users where username = p_username
    )LOOP
        PIPE ROW(user_type(rec.id, rec.username, rec.first_name, rec.second_name, rec.city, rec.email, rec.tel, rec.role, rec.profile_image, rec.credential_id, rec.master_info_id));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error finding user: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION exists_user_by_username(p_username IN VARCHAR2) 
RETURN NUMBER IS
    v_count NUMBER;
BEGIN
    SELECT COUNT(*) INTO v_count
    FROM Users
    WHERE username = p_username;

    IF v_count > 0 THEN
        RETURN 1;
    ELSE
        RETURN 0;
    END IF;
END;

CREATE OR REPLACE FUNCTION search_master(
    p_user_id IN NUMBER,
    p_search IN VARCHAR2
) RETURN user_table PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT u.id, u.username, u.first_name, u.second_name, u.city, u.email, u.tel, u.role, u.profile_image, u.credential_id, u.master_info_id 
    from Users u join Master_info mi on u.master_info_id = mi.id
    where master_info_id IS NOT NULL
          AND u.id != p_user_id
          AND (LOWER(u.first_name) LIKE '%' || LOWER(p_search) || '%'
               OR LOWER(u.second_name) LIKE '%' || LOWER(p_search) || '%'
               OR LOWER(u.username) LIKE '%' || LOWER(p_search) || '%'
               OR LOWER(u.first_name) || ' ' || LOWER(u.second_name) LIKE '%' || LOWER(p_search) || '%'
               OR LOWER(u.second_name) || ' ' || LOWER(u.first_name) LIKE '%' || LOWER(p_search) || '%'
               OR LOWER(mi.description) LIKE '%' || LOWER(p_search) || '%')
    )LOOP
        PIPE ROW(user_type(rec.id, rec.username, rec.first_name, rec.second_name, rec.city, rec.email, rec.tel, rec.role, rec.profile_image, rec.credential_id, rec.master_info_id));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error searching master: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION search_user(
    p_search IN VARCHAR2
) RETURN user_table PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT u.id, u.username, u.first_name, u.second_name, u.city, u.email, u.tel, u.role, u.profile_image, u.credential_id, u.master_info_id 
    from Users u
    where (u.role = 'ROLE_CLIENT' OR u.role = 'ROLE_MASTER')
          AND (LOWER(u.first_name) LIKE '%' || LOWER(p_search) || '%'
               OR LOWER(u.second_name) LIKE '%' || LOWER(p_search) || '%'
               OR LOWER(u.username) LIKE '%' || LOWER(p_search) || '%'
               OR LOWER(u.first_name) || ' ' || LOWER(u.second_name) LIKE '%' || LOWER(p_search) || '%'
               OR LOWER(u.second_name) || ' ' || LOWER(u.first_name) LIKE '%' || LOWER(p_search) || '%')
    )LOOP
        PIPE ROW(user_type(rec.id, rec.username, rec.first_name, rec.second_name, rec.city, rec.email, rec.tel, rec.role, rec.profile_image, rec.credential_id, rec.master_info_id));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error searching user: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE add_user (
    p_password_hash VARCHAR2,
    p_username VARCHAR2,
    p_first_name VARCHAR2,
    p_second_name VARCHAR2,
    p_city VARCHAR2,
    p_email VARCHAR2,
    p_tel VARCHAR2,
    p_role VARCHAR2,
    p_profile_image BLOB,
    p_master_info_id NUMBER
) AS
    v_user_id NUMBER;
    v_credential_id NUMBER;
BEGIN
    INSERT INTO Credential(username, password_hash) VALUES (p_username, p_password_hash);

    SELECT id into v_credential_id FROM Credential where username = p_username;
    INSERT INTO Users (username, first_name, second_name, city, email, tel, role, profile_image, credential_id, master_info_id) 
         VALUES (p_username, p_first_name, p_second_name, p_city, p_email, p_tel, p_role, p_profile_image, v_credential_id, p_master_info_id);
    
    SELECT id into v_user_id FROM Users where username = p_username;
    INSERT INTO Setting(account_id, enable_two_step_authentication) VALUES (v_user_id, 0);
    
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error adding new user: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE update_user_client (
    p_id NUMBER,
    p_username VARCHAR2,
    p_first_name VARCHAR2,
    p_second_name VARCHAR2,
    p_city VARCHAR2,
    p_email VARCHAR2,
    p_tel VARCHAR2,
    p_profile_image BLOB
) AS
BEGIN
    UPDATE USERS set username = p_username, first_name = p_first_name, second_name = p_second_name, 
        city = p_city, email = p_email, tel = p_tel, profile_image = p_profile_image
        WHERE id = p_id;
        update Credential set username = p_username where id = (select credential_id from Users where id = p_id);
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error updating client info: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE update_user_master (
    p_id NUMBER,
    p_username VARCHAR2,
    p_first_name VARCHAR2,
    p_second_name VARCHAR2,
    p_city VARCHAR2,
    p_email VARCHAR2,
    p_tel VARCHAR2,
    p_profile_image BLOB,
    p_description VARCHAR2,
    p_business_address VARCHAR2
) AS
    v_master_info_id NUMBER;
BEGIN    
    SELECT master_info_id into v_master_info_id from Users where id = p_id;
    IF v_master_info_id IS NULL THEN
        insert into Master_info(description, business_address) VALUES(p_description, p_business_address)
        returning id into v_master_info_id;
        UPDATE USERS set username = p_username, first_name = p_first_name, second_name = p_second_name, 
        city = p_city, email = p_email, tel = p_tel, profile_image = p_profile_image, master_info_id = v_master_info_id
        WHERE id = p_id;
    ELSE
        update Master_info set description = p_description, business_address = p_business_address 
        where id = v_master_info_id;
        
        UPDATE USERS set username = p_username, first_name = p_first_name, second_name = p_second_name, 
        city = p_city, email = p_email, tel = p_tel, profile_image = p_profile_image
        WHERE id = p_id;
    END IF;
    update Credential set username = p_username where id = (select credential_id from Users where id = p_id);
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error during updating master info: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE make_master(p_user_id IN NUMBER)
AS
BEGIN
    update Users set role = 'ROLE_MASTER' where id = p_user_id;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error making master: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE follow_master (
    p_client_id NUMBER,
    p_master_id NUMBER
) AS
BEGIN
    insert into masters_followers(client_id, master_id) values(p_client_id, p_master_id);
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error following master: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE unfollow_master (
    p_client_id NUMBER,
    p_master_id NUMBER
) AS
BEGIN
    delete masters_followers where client_id = p_client_id AND master_id = p_master_id;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error unfollowing master: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION is_client_following_master(
    p_client_id NUMBER,
    p_master_id NUMBER
) 
RETURN NUMBER IS
    v_res NUMBER;
BEGIN
    SELECT COUNT(*) INTO v_res
    FROM masters_followers
    WHERE client_id = p_client_id AND master_id = p_master_id;

    IF v_res > 0 THEN
        RETURN 1;
    ELSE
        RETURN 0;
    END IF;
END;

drop type user_type;
drop type user_table;
drop FUNCTION find_user_by_id;
drop FUNCTION find_user_by_username;
drop FUNCTION exists_user_by_username;
drop FUNCTION search_master;
drop FUNCTION search_user;
drop PROCEDURE add_user;
drop PROCEDURE update_user_client;
drop PROCEDURE update_user_master;
drop PROCEDURE follow_master;
drop PROCEDURE unfollow_master;
drop FUNCTION is_client_following_master;


------------------------------------Booking
CREATE OR REPLACE TYPE booking_type AS OBJECT (
    id NUMBER,
    client_id NUMBER,
    master_id NUMBER,
    service VARCHAR2(255),
    booking_date DATE,
    time TIMESTAMP,
    booked NUMBER(1)
);
CREATE OR REPLACE TYPE booking_table AS TABLE OF booking_type;

CREATE OR REPLACE FUNCTION find_booking_by_id(
    p_id IN NUMBER
) RETURN booking_table PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT id, client_id, master_id, service, booking_date, time, booked from Booking where id = p_id
    )LOOP
        PIPE ROW(booking_type(rec.id, rec.client_id, rec.master_id, rec.service, rec.booking_date, rec.time, rec.booked));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error finding booking: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION find_bookings_in_range(
    p_master_id IN NUMBER,
    p_start_date DATE,
    p_end_date DATE,
    p_now_date DATE,
    p_now_time TIMESTAMP
) RETURN booking_table PIPELINED IS
BEGIN
    IF (p_start_date = p_now_date) THEN
        FOR rec IN(
        SELECT id, client_id, master_id, service, booking_date, time, booked from Booking 
        where master_id = p_master_id AND booking_date = p_start_date AND time > p_now_time
        )LOOP
            PIPE ROW(booking_type(rec.id, rec.client_id, rec.master_id, rec.service, rec.booking_date, rec.time, rec.booked));
        END LOOP;
    ELSE
        FOR rec IN(
        SELECT id, client_id, master_id, service, booking_date, time, booked from Booking 
        where master_id = p_master_id AND booking_date = p_start_date
        )LOOP
            PIPE ROW(booking_type(rec.id, rec.client_id, rec.master_id, rec.service, rec.booking_date, rec.time, rec.booked));
        END LOOP;
    END IF;
    
    FOR rec IN(
    SELECT id, client_id, master_id, service, booking_date, time, booked from Booking 
    where master_id = p_master_id AND booking_date BETWEEN p_start_date AND p_end_date 
    AND booking_date > p_start_date
    )LOOP
        PIPE ROW(booking_type(rec.id, rec.client_id, rec.master_id, rec.service, rec.booking_date, rec.time, rec.booked));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error finding booking in range: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE add_booking (
    p_master_id NUMBER,
    p_service VARCHAR2,
    p_booking_date DATE,
    p_time TIMESTAMP
) AS
BEGIN
    INSERT INTO Booking(master_id, service, booking_date, time) 
    VALUES (p_master_id, p_service, p_booking_date, p_time);
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error adding new booking: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE update_booking (
    p_id NUMBER,
    p_client_id NUMBER,
    p_service VARCHAR2,
    p_booking_date DATE,
    p_time TIMESTAMP,
    p_booked NUMBER
) AS
BEGIN
    UPDATE Booking set client_id = p_client_id, service = p_service, booking_date = p_booking_date, 
        time = p_time, booked = p_booked
        WHERE id = p_id;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error updating booking: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION find_free_bookings_in_range(
    p_master_id IN NUMBER,
    p_start_date DATE,
    p_end_date DATE,
    p_now_date DATE,
    p_now_time TIMESTAMP
) RETURN booking_table PIPELINED IS
BEGIN
    IF (p_start_date = p_now_date) THEN
        FOR rec IN(
        SELECT id, client_id, master_id, service, booking_date, time, booked from Booking 
        where master_id = p_master_id AND booked = 0 AND booking_date = p_start_date AND time > p_now_time
        )LOOP
            PIPE ROW(booking_type(rec.id, rec.client_id, rec.master_id, rec.service, rec.booking_date, rec.time, rec.booked));
        END LOOP;
    ELSE
        FOR rec IN(
        SELECT id, client_id, master_id, service, booking_date, time, booked from Booking 
        where master_id = p_master_id AND booked = 0 AND booking_date = p_start_date
        )LOOP
            PIPE ROW(booking_type(rec.id, rec.client_id, rec.master_id, rec.service, rec.booking_date, rec.time, rec.booked));
        END LOOP;
    END IF;

    FOR rec IN(
    SELECT id, client_id, master_id, service, booking_date, time, booked from Booking 
    where master_id = p_master_id AND booked = 0 AND booking_date BETWEEN p_start_date AND p_end_date 
    AND booking_date > p_start_date
    )LOOP
        PIPE ROW(booking_type(rec.id, rec.client_id, rec.master_id, rec.service, rec.booking_date, rec.time, rec.booked));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error finding free bookings in range: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION find_bookings_by_date(
    p_master_id IN NUMBER,
    p_date DATE,
    p_now_date DATE,
    p_now_time TIMESTAMP
) RETURN booking_table PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT id, client_id, master_id, service, booking_date, time, booked from Booking 
    where master_id = p_master_id AND booking_date = p_date AND (p_date != p_now_date OR time > p_now_time)
    )LOOP
        PIPE ROW(booking_type(rec.id, rec.client_id, rec.master_id, rec.service, rec.booking_date, rec.time, rec.booked));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error finding bookings by date: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION find_free_bookings_by_date(
    p_master_id IN NUMBER,
    p_date DATE,
    p_now_date DATE,
    p_now_time TIMESTAMP
) RETURN booking_table PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT id, client_id, master_id, service, booking_date, time, booked from Booking 
    where master_id = p_master_id AND booked = 0 AND booking_date = p_date AND (p_date != p_now_date OR time > p_now_time)
    )LOOP
        PIPE ROW(booking_type(rec.id, rec.client_id, rec.master_id, rec.service, rec.booking_date, rec.time, rec.booked));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error finding free bookings by date: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION find_bookings_by_client(
    p_client_id IN NUMBER,
    p_now_date DATE,
    p_now_time TIMESTAMP
) RETURN booking_table PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT id, client_id, master_id, service, booking_date, time, booked from Booking 
    where client_id = p_client_id AND booking_date > p_now_date OR (booking_date = p_now_date AND time > p_now_time)
    )LOOP
        PIPE ROW(booking_type(rec.id, rec.client_id, rec.master_id, rec.service, rec.booking_date, rec.time, rec.booked));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error finding bookings by client: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION find_booked_bookings_by_master(
    p_master_id IN NUMBER,
    p_now_date DATE,
    p_now_time TIMESTAMP
) RETURN booking_table PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT id, client_id, master_id, service, booking_date, time, booked from Booking 
    where master_id = p_master_id AND booked = 1 AND booking_date > p_now_date OR (booking_date = p_now_date AND time > p_now_time)
    )LOOP
        PIPE ROW(booking_type(rec.id, rec.client_id, rec.master_id, rec.service, rec.booking_date, rec.time, rec.booked));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error finding bookings by master: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION find_history_bookings_by_client(
    p_client_id IN NUMBER,
    p_now_date DATE,
    p_now_time TIMESTAMP
) RETURN booking_table PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT id, client_id, master_id, service, booking_date, time, booked from Booking 
    where client_id = p_client_id AND (booking_date < p_now_date OR (booking_date = p_now_date AND time < p_now_time))
    )LOOP
        PIPE ROW(booking_type(rec.id, rec.client_id, rec.master_id, rec.service, rec.booking_date, rec.time, rec.booked));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error finding history bookings by client: ' || SQLERRM);
END;

CREATE OR REPLACE FUNCTION find_history_bookings_by_master(
    p_master_id IN NUMBER,
    p_now_date DATE,
    p_now_time TIMESTAMP
) RETURN booking_table PIPELINED IS
BEGIN
    FOR rec IN(
    SELECT id, client_id, master_id, service, booking_date, time, booked from Booking 
    where master_id = p_master_id AND booked = 1 AND (booking_date < p_now_date OR (booking_date = p_now_date AND time < p_now_time))
    )LOOP
        PIPE ROW(booking_type(rec.id, rec.client_id, rec.master_id, rec.service, rec.booking_date, rec.time, rec.booked));
    END LOOP;
    RETURN;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error finding history bookings by master: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE delete_booking (
    p_id NUMBER
) AS
BEGIN
    DELETE Booking WHERE id = p_id;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error deleting booking: ' || SQLERRM);
END;

CREATE OR REPLACE PROCEDURE delete_expired_bookings
AS
BEGIN
    DELETE Booking WHERE booked = 0 and booking_date < sysdate;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20001, 'Error deleting expired bookings: ' || SQLERRM);
END;

drop type booking_type;
drop type booking_table;
drop FUNCTION find_booking_by_id;
drop FUNCTION find_bookings_in_range;
drop PROCEDURE add_booking;
drop PROCEDURE update_booking;
drop FUNCTION find_bookings_by_date;
drop FUNCTION find_free_bookings_by_date;
drop FUNCTION find_bookings_by_client;
drop FUNCTION find_booked_bookings_by_master;
drop FUNCTION find_history_bookings_by_client;
drop FUNCTION find_history_bookings_by_master;
drop PROCEDURE delete_booking;
drop PROCEDURE delete_expired_bookings;






