Bài tập 1:
ở Terminal 1: chạy
nc -l 9000

ở Terminal 2:
./tcp_client 127.0.0.1 9000
==> Kết quả:
Connected to server!
Enter message:

Bài tập 2:
chạy server:
./tcp_server 9000 hello.txt output.txt

ở Terminal khác (client):
nc 127.0.0.1 9000
sau khi kêt nối thì ta gõ văn bản ở client , thì văn bản sẽ in ra ở file output.txt

Bài tập 3:
chạy server
./sv_server 9000

chạy client
./sv_client 127.0.0.1 9000
sau khi kết nối thành công thì ta nhập các trường thông tin của sinh viên

Bài tập 4:
chạy server
./sv_server 9000 sv_log.txt

chạy client 
./sv_client 127.0.0.1 9000

khi kết nối thành công thì nhập thông tin tương tự bài 3
