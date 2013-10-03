//
// This file is part of T-Rex, a Complex Event Processing Middleware.
// See http://home.dei.polimi.it/margara
//
// Authors: Alessandro Margara
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "EchoClient.h"

#define PORT 9999
#define NUM_PACKETS 40

using namespace std;

void trex_testing::launchEchoClient() {
	int sock, n;
	struct sockaddr_in sa;
	memset(&sa, 0, sizeof(struct sockaddr_in));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORT);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	Marshaller marshaller;
	Unmarshaller unmarshaller;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock<0) {
		cout << "Error creating the socket!" << endl;
		exit(-1);
	}
	if (connect(sock, (struct sockaddr *) &sa, sizeof(sa))<0) {
		cout << "Error while connecting!" << endl;
		exit(-1);
	} else {
		cout << "Connection established with the server!" << endl;
	}
	for (int receivedPkts=0; receivedPkts<NUM_PACKETS; receivedPkts++) {
		cout << endl << "Message " << receivedPkts << endl << endl;
		// Reads the packet type
		char typeByteArray[1];
		int alreadyRead = 0;
		while (alreadyRead < 1) {
			n = read(sock, typeByteArray, 1);
			if (n<=0) {
				cout << "Error reading the type of message!" << endl;
				exit(-1);
			}
			alreadyRead += n;
		}
		cout << "Type of message read successfully!" << endl;
		PktType pktType = unmarshaller.decodePktType(typeByteArray);
		// Reads the packet length
		char lengthByteArray[4];
		alreadyRead = 0;
		while (alreadyRead < 4) {
			n = read(sock, lengthByteArray+alreadyRead, 4-alreadyRead);
			if (n<=0) {
				cout << "Error reading the length of message!" << endl;
				exit(-1);
			}
			alreadyRead += n;
		}
		cout << "Length of message read successfully!" << endl;
		int length = unmarshaller.decodeInt(lengthByteArray);
		cout << "Message length = " << length << " byte." << endl;
		// Reads the packet
		char pktByteArray[length];
		alreadyRead = 0;
		while (alreadyRead < length) {
			n = read(sock, pktByteArray+alreadyRead, length-alreadyRead);
			if (n<=0) {
				cout << "Error reading the type of message!" << endl;
				exit(-1);
			}
			alreadyRead += n;
		}
		cout << "Message read successfully!" << endl;
		// Decodes the packet and sends it back
		char *sendArray;
		int size;
		if (pktType == PUB_PKT) {
			cout << "The packet is a PubPkt: decoding it..." << endl;
			PubPkt *pkt = unmarshaller.decodePubPkt(pktByteArray);
			size = marshaller.getSize(pkt);
			sendArray = marshaller.encode(pkt);
			delete pkt;
		} else if (pktType == RULE_PKT) {
			cout << "The packet is a RulePkt: decoding it..." << endl;
			RulePkt *pkt = unmarshaller.decodeRulePkt(pktByteArray);
			size = marshaller.getSize(pkt);
			sendArray = marshaller.encode(pkt);
			delete pkt;
		} else if (pktType == SUB_PKT) {
			cout << "The packet is a SubPkt: decoding it..." << endl;
			SubPkt *pkt = unmarshaller.decodeSubPkt(pktByteArray);
			size = marshaller.getSize(pkt);
			sendArray = marshaller.encode(pkt);
			delete pkt;
		} else if (pktType == ADV_PKT) {
			cout << "The packet is an AdvPkt: decoding it..." << endl;
			AdvPkt *pkt = unmarshaller.decodeAdvPkt(pktByteArray);
			size = marshaller.getSize(pkt);
			sendArray = marshaller.encode(pkt);
			delete pkt;
		} else {
			cout << "Unknown packet type!" << endl;
			exit(-1);
		}
		cout << "Sending back the packet to the server" << endl;
		n = write(sock, sendArray, size);
		if (n<0) {
			cout << "Error sending packet!" << endl;
			exit(-1);
		}
	}
}
