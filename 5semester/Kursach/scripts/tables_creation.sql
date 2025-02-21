CREATE TABLE Master_info (
    id NUMBER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    description VARCHAR2(2000) NOT NULL,
    business_address VARCHAR2(255) NOT NULL
) TABLESPACE mastergram_tablespace;
CREATE INDEX idx_description ON Master_info (description);

CREATE TABLE Credential (
    id NUMBER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    username VARCHAR2(255) UNIQUE NOT NULL,
    password_hash VARCHAR2(255) NOT NULL
) TABLESPACE mastergram_tablespace;

CREATE TABLE Users (
    id NUMBER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    username VARCHAR2(255) NOT NULL,
    first_name VARCHAR2(255) NOT NULL,
    second_name VARCHAR2(255) NOT NULL,
    city VARCHAR2(255) NOT NULL,
    email VARCHAR2(255) NOT NULL,
    tel VARCHAR2(20)NOT NULL,
    role VARCHAR2(50) NOT NULL,
    profile_image BLOB,
    credential_id NUMBER NOT NULL,
    master_info_id NUMBER,
    CONSTRAINT fk_credential FOREIGN KEY (credential_id) REFERENCES Credential(id),
    CONSTRAINT fk_masterInfo FOREIGN KEY (master_info_id) REFERENCES Master_info(id)
) TABLESPACE mastergram_tablespace;

CREATE INDEX idx_users_name ON Users (username, first_name, second_name);

CREATE TABLE Post (
    id NUMBER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    creation_date DATE NOT NULL,
    post_image BLOB NOT NULL,
    description VARCHAR2(2000),
    master_info_id NUMBER NOT NULL,
    CONSTRAINT fk_post_masterinfo FOREIGN KEY (master_info_id) REFERENCES Master_info(id)
) TABLESPACE mastergram_tablespace;


/

drop table Booking;
CREATE TABLE Booking (
    id NUMBER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    client_id NUMBER,
    master_id NUMBER NOT NULL,
    service VARCHAR2(255) NOT NULL,
    booking_date DATE NOT NULL,
    time TIMESTAMP NOT NULL,
    booked NUMBER(1) DEFAULT 0 CHECK (booked IN (0, 1)),
    CONSTRAINT fk_booking_client FOREIGN KEY (client_id) REFERENCES Users(id),
    CONSTRAINT fk_booking_master FOREIGN KEY (master_id) REFERENCES Master_info(id)
) TABLESPACE mastergram_tablespace
partition by range (booking_date)
    interval (numtoyminterval(1, 'month'))
(
    partition part0 values less than (to_date('2024-01-01', 'yyyy-mm-dd')),
    partition part1 values less than (to_date('2024-02-01', 'yyyy-mm-dd'))
);

CREATE INDEX idx_booking_date ON Booking (booking_date) LOCAL;

CREATE TABLE masters_followers (
    client_id NUMBER NOT NULL,
    master_id NUMBER NOT NULL,
    CONSTRAINT pk_masters_followers PRIMARY KEY (client_id, master_id),
    CONSTRAINT fk_masters_followers_client FOREIGN KEY (client_id) REFERENCES Users(id),
    CONSTRAINT fk_masters_followers_master FOREIGN KEY (master_id) REFERENCES Users(id)
) TABLESPACE mastergram_tablespace;

CREATE TABLE Notification(
    id NUMBER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    receiver_id NUMBER NOT NULL,
    creation_date DATE NOT NULL,
    message VARCHAR2(255) NOT NULL,
    title VARCHAR2(255) NOT NULL,
    watched NUMBER(1) DEFAULT 0 CHECK (booked IN (0, 1)),
    CONSTRAINT fk_notification_user FOREIGN KEY (receiver_id) REFERENCES Users(id)
) TABLESPACE mastergram_tablespace;

CREATE TABLE Request(
    id NUMBER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    creation_date DATE NOT NULL,
    user_id NUMBER NOT NULL,
    request_image BLOB NOT NULL,
    cover_letter VARCHAR2(500) NOT NULL,
    CONSTRAINT fk_request_user FOREIGN KEY (user_id) REFERENCES Users(id)
) TABLESPACE mastergram_tablespace;

CREATE TABLE Setting(
    id NUMBER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    account_id NUMBER NOT NULL,
    enable_two_step_authentication NUMBER(1) DEFAULT 0 CHECK (enable_two_step_authentication IN (0, 1)),
    CONSTRAINT fk_setting_user FOREIGN KEY (account_id) REFERENCES Users(id)

) TABLESPACE mastergram_tablespace;

DROP TABLE Setting;
DROP TABLE Notification;
DROP TABLE Request;
DROP TABLE masters_followers;
DROP TABLE Booking;
DROP TABLE Post;
DROP TABLE Users;
DROP TABLE Credential;
DROP TABLE Master_info;











