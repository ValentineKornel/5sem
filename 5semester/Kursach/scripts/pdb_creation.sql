CREATE PLUGGABLE DATABASE mastergram_pdb
    ADMIN USER mastergram_pdb_admin IDENTIFIED BY 12345
    ROLES = (DBA)
    FILE_NAME_CONVERT = ('C:\app\valen\product\21c\oradata\XE\pdbseed', 'C:\app\valen\product\21c\oradata\XE\mastergram_pdb');
    
ALTER SESSION SET CONTAINER = "CDB$ROOT";
    
ALTER PLUGGABLE DATABASE mastergram_pdb OPEN;
ALTER SESSION SET CONTAINER = "mastergram_pdb";
select * from dba_users;

--mastergram_pdb_admin
CREATE TABLESPACE mastergram_tablespace 
DATAFILE 'mastergram_tablespace.dbf' SIZE 200M;
ALTER TABLESPACE mastergram_tablespace
ADD DATAFILE 'mastergram_tablespace2.dbf' SIZE 200M AUTOEXTEND ON NEXT 50M;

----ADMIN
CREATE PROFILE ADMIN_PROFILE LIMIT
    PASSWORD_LIFE_TIME 180
    SESSIONS_PER_USER 10
    FAILED_LOGIN_ATTEMPTS 3
    PASSWORD_LOCK_TIME 1
    PASSWORD_REUSE_TIME 10
    PASSWORD_GRACE_TIME DEFAULT
    CONNECT_TIME 180
    IDLE_TIME 30;

CREATE USER ADMIN IDENTIFIED BY 12345 PROFILE ADMIN_PROFILE
DEFAULT TABLESPACE mastergram_tablespace QUOTA UNLIMITED ON mastergram_tablespace
ACCOUNT UNLOCK;

GRANT DBA to ADMIN;


CREATE PROFILE USERS_PROFILE LIMIT
    PASSWORD_LIFE_TIME 180
    SESSIONS_PER_USER 20
    FAILED_LOGIN_ATTEMPTS 3
    PASSWORD_LOCK_TIME 3
    PASSWORD_REUSE_TIME 10
    CONNECT_TIME 180
    IDLE_TIME 60;

CREATE USER MASTER IDENTIFIED BY 12345 PROFILE USERS_PROFILE
DEFAULT TABLESPACE mastergram_tablespace
ACCOUNT UNLOCK;

GRANT CREATE SESSION TO MASTER;
GRANT EXECUTE ON find_notifications_by_receiver TO MASTER;
GRANT EXECUTE ON add_notification_to_user TO MASTER;
GRANT EXECUTE ON mark_notifications_as_read TO MASTER;
GRANT EXECUTE ON count_unread_notifications TO MASTER;
GRANT EXECUTE ON find_post_by_id TO MASTER;
GRANT EXECUTE ON delete_post TO MASTER;
GRANT EXECUTE ON add_post TO MASTER;
GRANT EXECUTE ON add_post TO MASTER;
GRANT EXECUTE ON find_setting_by_id TO MASTER;
GRANT EXECUTE ON change_setting TO MASTER;
GRANT EXECUTE ON find_user_by_id TO MASTER;
GRANT EXECUTE ON find_user_by_username TO MASTER;
GRANT EXECUTE ON exists_user_by_username TO MASTER;
GRANT EXECUTE ON search_master TO MASTER;
GRANT EXECUTE ON update_user_master TO MASTER;
GRANT EXECUTE ON follow_master TO MASTER;
GRANT EXECUTE ON unfollow_master TO MASTER;
GRANT EXECUTE ON is_client_following_master TO MASTER;
GRANT EXECUTE ON find_master_info_by_id TO MASTER;
GRANT EXECUTE ON find_booking_by_id TO MASTER;
GRANT EXECUTE ON find_bookings_in_range TO MASTER;
GRANT EXECUTE ON add_booking TO MASTER;
GRANT EXECUTE ON update_booking TO MASTER;
GRANT EXECUTE ON find_free_bookings_in_range TO MASTER;
GRANT EXECUTE ON find_bookings_by_date TO MASTER;
GRANT EXECUTE ON find_free_bookings_by_date TO MASTER;
GRANT EXECUTE ON find_bookings_by_client TO MASTER;
GRANT EXECUTE ON find_booked_bookings_by_master TO MASTER;
GRANT EXECUTE ON find_history_bookings_by_client TO MASTER;
GRANT EXECUTE ON find_history_bookings_by_master TO MASTER;
GRANT EXECUTE ON delete_booking TO MASTER;


CREATE USER CLIENT IDENTIFIED BY 12345 PROFILE USERS_PROFILE
DEFAULT TABLESPACE mastergram_tablespace
ACCOUNT UNLOCK;

GRANT CREATE SESSION TO CLIENT;
GRANT EXECUTE ON find_notifications_by_receiver TO CLIENT;
GRANT EXECUTE ON add_notification_to_user TO CLIENT;
GRANT EXECUTE ON mark_notifications_as_read TO CLIENT;
GRANT EXECUTE ON count_unread_notifications TO CLIENT;
GRANT EXECUTE ON find_post_by_id TO CLIENT;
GRANT EXECUTE ON add_request TO CLIENT;
GRANT EXECUTE ON exists_request_by_user TO CLIENT;
GRANT EXECUTE ON find_setting_by_id TO CLIENT;
GRANT EXECUTE ON change_setting TO CLIENT;
GRANT EXECUTE ON find_user_by_id TO CLIENT;
GRANT EXECUTE ON find_user_by_username TO CLIENT;
GRANT EXECUTE ON exists_user_by_username TO CLIENT;
GRANT EXECUTE ON search_master TO CLIENT;
GRANT EXECUTE ON add_user TO CLIENT;
GRANT EXECUTE ON update_user_client TO CLIENT;
GRANT EXECUTE ON follow_master TO CLIENT;
GRANT EXECUTE ON unfollow_master TO CLIENT;
GRANT EXECUTE ON is_client_following_master TO CLIENT;
GRANT EXECUTE ON find_booking_by_id TO CLIENT;
GRANT EXECUTE ON find_free_bookings_in_range TO CLIENT;
GRANT EXECUTE ON find_free_bookings_by_date TO CLIENT;
GRANT EXECUTE ON find_bookings_by_client TO MASTER;
GRANT EXECUTE ON find_history_bookings_by_client TO MASTER;





