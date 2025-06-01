/*
Every user on a Unix system has a unique user ID (UID) number. 
The directories and files have permissions that regulate who can access them.

- id     -> display the user and group IDs
- su     -> switch user 
- whoami -> display the current user
- groups -> display the groups the user belongs to
- env    -> display the environment variables

chmod has one more option:
    chmod 4755 file or chmod u+s file -> setuid bit (set user ID = is used to allow a user to run a program with the permissions of the file owner)
   
chown
    chown user:group file -> change the owner and group of a file
    chown user file -> change the owner of a file
    chown :group file -> change the group of a file
*/

