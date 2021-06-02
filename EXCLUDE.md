The exclude file format is as follows:

At the start of the file is a 4-byte magic 0x68 0xBA 0x17 0xED, and a 4-byte count of the number of RAM ranges in the file.
After that will be the ranges of RAM, stored as a 4-byte address and a 4-byte range width.
Every exclude file should have one extra range filled with 0xFF.
