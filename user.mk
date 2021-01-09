#Userlib directories
CRCLIB = ./crc

# List of all the Userlib files
CRCSRC =  $(CRCLIB)/source/crc.c
			
			
          
# Required include directories
CRCINC =  $(CRCLIB)/include



# Shared variables
ALLCSRC += $(CRCSRC)
ALLINC  += $(CRCINC)