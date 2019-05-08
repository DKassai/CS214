all: client.c commit.c compression.c connection.c manifest.c update.c server.c WTFClient.c WTFServer.c WTFTest.c
	mkdir client
	mkdir server
	gcc -g -O -o client/WTF WTFClient.c client.c commit.c compression.c connection.c manifest.c update.c -lcrypto -lm
	gcc -g -O -o server/WTFServer WTFServer.c server.c compression.c manifest.c -pthread -lm -lcrypto
	gcc -g -O -o WTFTest WTFTest.c



clean:
	rm -r client
	rm -r server
	rm WTFTest.c
	clear
	

