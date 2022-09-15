#ident  "@(#).login     ver 1.0     Aug 20, 1996"
# Default user .login file.
#
# This file is executed after ~/.cshrc at login.
# This file should contain commands that set the options 
# for a terminal and/or window and commands that are
# only needed during an interactive session.


#####
# Source the site-wide syslogin and sysapps files. 
# The syslogin file displays motd and notifies users of
# news and mail during login.  The sysapps file sets up
# the user's environment for commonly used applications.
# These lines should not be deleted.
#####
source /site/env/syslogin
source /site/env/sysapps


#####
# Set up the terminal.
# The stty command sets certain terminal I/O options
# for the device that is the current standard input.
# The lines below define the actions of the keys on
# different keyboardsi (ie. delete and backspacce).
#####
# Uncomment this if you are using an NCD Xterminal keyboard.
stty erase "^?" kill "^U" intr "^C" eof "^D" susp "^Z" hupcl ixon ixoff tostop tabs


#####
# Define the string that will prompt you for interactive input.
# You may pick one or define your own.  Only one should be uncommented.
#####
set prompt = "`hostname`> "          # Name of the machine you are on.
#set prompt = "`hostname`[\!] "       # Name of the machine and history number
#set prompt = "`whoami`> "            # Your user name.
#set prompt = "`whoami`[\!] "         # Your user name and history number


#####
# Defines the LINES fand COLUMNS environment variables for users
# using the tcsh.  This is a workaround for a bug in the tcsh. 
#####
if ($?tcsh) then
   setenv LINES ""
   setenv COLUMNS ""
endif


#####
# User specific additions should be added below.
#####

