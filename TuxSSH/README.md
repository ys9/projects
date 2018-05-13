## TuxSSH

TuxSSH is an all-in-one terminal based ssh utility written in C and Bash. 
You can access the source code in the src folder, but the resulting
program will not run right because of missing dependency files which are
only in the .tar.gz archive. If you want to modify the code and run it, 
follow the install instructions, edit the code, and compile from the 
included Makefile, which would overwrite the executable from the .tar.gz archive.

TO INSTALL:
1. Download the TuxSSH.tar.gz archive and install.sh script
2. Open a terminal and install the program with `./install.sh`
3. If you get a permission error on `./install.sh`, then run `chmod 755 install.sh && ./install.sh`
4. You're done! Run `./TuxSSH` to run the program

NOTE: For the program to work correctly, at the prompts to set up passwords for keys, jus hit enter both times - don't enter 
a password. Otherwise, it's just as useful to login in with a regular ssh password.

For any questions or comments, contact me at brendan.wilson@student.nmt.edu
