CREATE PLUGGABLE DATABASE mastergram_pdb
    ADMIN USER mastergram_pdb_admin IDENTIFIED BY 12345
    ROLES = (DBA)
    FILE_NAME_CONVERT = ('C:\app\valen\product\21c\oradata\XE\pdbseed', 'C:\app\valen\product\21c\oradata\XE\mastergram_pdb');
    
ALTER PLUGGABLE DATABASE mastergram_pdb OPEN;

--mastergram_pdb_admin
CREATE TABLESPACE mastergram_tablespace 
DATAFILE 'mastergram_tablespace.dbf' SIZE 100M;

CREATE USER mastergram_app IDENTIFIED BY 12345
DEFAULT TABLESPACE mastergram_tablespace QUOTA UNLIMITED ON mastergram_tablespace
ACCOUNT UNLOCK;

GRANT DBA to mastergram_app;