The following commands,

Terminal 1: ./printer 5
Terminal 2: ./client 1 5 && ./client 2 10 && ./client 3 15 && ./client 4 5 && ./client 5 10 && ./client 6 15 && ./client 7 5 && ./client 8 10 && ./client 9 15 && ./client 10 5 produce the following output:


---
No request in buffer, Printer sleeps
---
Client 1 has 5 page(s) to print, puts request in Buffer[0]
Printer starts printing 5 pages from Buffer[0]
Client 2 has 10 page(s) to print, puts request in Buffer[0]
Client 3 has 15 page(s) to print, puts request in Buffer[1]
Client 4 has 5 page(s) to print, puts request in Buffer[2]
Client 5 has 10 page(s) to print, puts request in Buffer[3]
Client 6 has 15 page(s) to print, puts request in Buffer[4]
Client 7 has 5 page(s) to print, buffer full, sleeps
Printer done printing 5 pages from Buffer[0]
Printer starts printing 15 pages from Buffer[4]
Client 7 wakes up, puts request in Buffer[4]
Client 8 has 10 page(s) to print, buffer full, sleeps
Printer done printing 15 pages from Buffer[4]
Printer starts printing 5 pages from Buffer[4]
Client 8 wakes up, puts request in Buffer[4]
Client 9 has 15 page(s) to print, buffer full, sleeps
Printer done printing 5 pages from Buffer[4]
Printer starts printing 10 pages from Buffer[4]
Client 9 wakes up, puts request in Buffer[4]
Client 10 has 5 page(s) to print, buffer full, sleeps
Printer done printing 10 pages from Buffer[4]
Printer starts printing 15 pages from Buffer[4]
Client 10 wakes up, puts request in Buffer[4]
Printer done printing 15 pages from Buffer[4]
Printer starts printing 5 pages from Buffer[4]
Printer done printing 5 pages from Buffer[4]
Printer starts printing 10 pages from Buffer[3]
Printer done printing 10 pages from Buffer[3]
Printer starts printing 5 pages from Buffer[2]
Printer done printing 5 pages from Buffer[2]
Printer starts printing 15 pages from Buffer[1]
Printer done printing 15 pages from Buffer[1]
Printer starts printing 10 pages from Buffer[0]
Printer done printing 10 pages from Buffer[0]
---
No request in buffer, Printer sleeps
---
