# --------------------------------------------------------------------------- #
#  This project uses a BBC micro:bit microcontroller, information at:
#  https://microbit.org/
#
#  This code is a sample for using a micro:bit (https://microbit.org/) 
#  microcontroller with Microsoft Data Streamer for Excel
#  (aka.ms/data-streamer).  It will reads Button A and reads comma delimited
#  strings from serial, and write button state and echo the first string read
#  from serial in a comma delimited format.
#
#  Comments, contributions, suggestions, bug reports, and feature
#  requests are welcome! For source code and bug reports see:
#  https://github.com/microsoft/HackingSTEMSamples
#
#  Copyright 2019, Adi Azulay Microsoft EDU Workshop - HackingSTEM
#  MIT License terms detailed in LICENSE.txt
# --------------------------------------------------------------------------- #

from microbit import *

# Interval of code looping, this helps reduce the impact of buffer overflow in
# Data Streamer
SERIAL_INTERVAL = 10

# Number of columns from Data Streamer
# IMPORTANT: This must be equal to the number of channels set in Data Streamer
NUM_DATA_COL = 5

# Change this to true to verify the number of data columns recieved.  Note
# that you have to also change the NUM_DATA_COL constant above to match the
# settings in Data Streamer.
CHECK_DATA = False

# Sets serial baud rate
uart.init(baudrate=9600)

# Array to hold the serial data
incoming_data = [0] * NUM_DATA_COL


def get_data_columns():
    '''
    Returns parsed string from serial.Editing any part of this method may
    cause bugs when sending or receiving data from Data Streamer.
    '''
    while uart.any() is True:
        stringIn = str(uart.readline(), "utf-8")
        stringIn = stringIn.split('\n')
        # Comma delimiter is used here
        parsed_data = stringIn[0].split(',')
        if CHECK_DATA:
            return verify_data(parsed_data)
        return parsed_data


def verify_data(data_to_verify):
    '''
    Returns argument if check is succesful or a None array if it's not.  Use
    this method only if you want to verify that string is well formed.
    '''
    if len(data_to_verify) == NUM_DATA_COL:
        return data_to_verify
    return [None]

# =========================================================================== #
# -----------------------------Main Program Loop----------------------------- #
# =========================================================================== #
while (True):

    button_state = button_a.is_pressed()

    if uart.any():
        # The get_data_columns() is defined above
        incoming_data = get_data_columns()

    # Create a string of the data to be sent, note the comma delimiter is part
    # of the string here.
    data_to_send = "{},{}".format(button_state, incoming_data[0])

    # uart is the micro:bit command for serial
    if data_to_send:
        uart.write(data_to_send+'\n')

    sleep(SERIAL_INTERVAL)
