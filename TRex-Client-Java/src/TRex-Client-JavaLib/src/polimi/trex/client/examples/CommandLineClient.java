package polimi.trex.client.examples;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import polimi.trex.client.communication.PacketListener;
import polimi.trex.client.communication.TransportManager;
import polimi.trex.client.packets.Packet;
import polimi.trex.client.packets.PubPacket;
import polimi.trex.client.packets.SubPacket;
import polimi.trex.common.Attribute;

/**
 * @author Gianpaolo Cugola
 * 
 *         A very basic, command line oriented, client for TRex.
 */
public class CommandLineClient implements PacketListener {
	private final TransportManager tManager = new TransportManager(true);

	public static void main(String[] args) {
		String serverHost = null;
		int serverPort = -1;
		List<Integer> subTypes = null;
		int pubType = -1;
		List<String> keys = null, values = null;
		CommandLineClient client;
		int i = 0;

		try {
			subTypes = new ArrayList<Integer>();
			pubType = -1;
			keys = new ArrayList<String>();
			values = new ArrayList<String>();
			if (args.length < 2)
				printUsageAndExit();
			serverHost = args[i++];
			serverPort = Integer.parseInt(args[i++]);
			while (i < args.length) {
				if (i < args.length && args[i].equals("-pub")) {
					i++;
					pubType = Integer.parseInt(args[i++]);
					while (i < args.length && !args[i].equals("-sub")) {
						keys.add(args[i++]);
						values.add(args[i++]);
					}
				}
				if (i < args.length && args[i].equals("-sub")) {
					i++;
					while (i < args.length && !args[i].equals("-sub")) {
						subTypes.add(Integer.parseInt(args[i++]));
					}
				}
			}
		} catch (NumberFormatException e) {
			System.out.println("Error at parameter " + i);
			printUsageAndExit();
		}
		try {
			client = new CommandLineClient(serverHost, serverPort);
			if (subTypes.size() > 0) {
				client.tManager.addPacketListener(client);
				client.tManager.start();
				client.subscribe(subTypes);
			}
			if (pubType != -1)
				client.publish(pubType, keys, values);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private static void printUsageAndExit() {
		System.out.println("Usage: java -jar TRexClient-JavaEx.jar " + "<server_host> <server_port> " + "[-sub <evt_type_1> ... <evt_type_n>]"
				+ "[-pub <evt_type> [<key_1> <val_1> ... <key_n> <val_n>]]");
		System.exit(-1);
	}

	public CommandLineClient(String serverHost, int serverPort) throws IOException {
		tManager.connect(serverHost, serverPort);
	}

	public void subscribe(List<Integer> subTypes) {
		for (int subType : subTypes) {
			SubPacket sub = new SubPacket(subType);
			try {
				tManager.send(sub);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public void publish(int pubType, List<String> keys, List<String> values) {
		PubPacket pub;
		boolean boolVal;
		int intVal;
		float floatVal;

		pub = new PubPacket(pubType);
		for (int i = 0; i < keys.size(); i++) {
			if (values.get(i).equals("true")) {
				boolVal = true;
				// add a bool attr
				pub.addAttribute(new Attribute(keys.get(i), boolVal));
			} else if (values.get(i).equals("false")) {
				boolVal = false;
				// add a bool attr
				pub.addAttribute(new Attribute(keys.get(i), boolVal));
			} else {
				try {
					intVal = Integer.parseInt(values.get(i));
					// add an int attr
					pub.addAttribute(new Attribute(keys.get(i), intVal));
				} catch (NumberFormatException e1) {
					try {
						floatVal = Float.parseFloat(values.get(i));
						// add a float attr
						pub.addAttribute(new Attribute(keys.get(i), floatVal));
					} catch (NumberFormatException e2) {
						// add a String attr
						pub.addAttribute(new Attribute(keys.get(i), values.get(i)));
					}
				}
			}
		}
		try {
			tManager.send(pub);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void notifyPktReceived(Packet pkt) {
		if (!(pkt instanceof PubPacket)) {
			System.out.println("Ingnoring wrong packet: " + pkt);
			return;
		}
		PubPacket pub = (PubPacket) pkt;
		System.out.print("PubPacket received: {");
		System.out.print(pub.getEventType());
		for (Attribute att : pub.getAttributes()) {
			System.out.print(" <" + att.getName());
			switch (att.getValType()) {
			case BOOL:
				System.out.print(" : bool = " + att.getBoolVal() + ">");
				break;
			case INT:
				System.out.print(" : int = " + att.getIntVal() + ">");
				break;
			case FLOAT:
				System.out.print(" : float = " + att.getFloatVal() + ">");
				break;
			case STRING:
				System.out.print(" : string = " + att.getStringVal() + ">");
				break;
			}
		}
		System.out.println("}");
	}

	@Override
	public void notifyConnectionError() {
		System.out.println("Connection error. Exiting.");
		System.exit(-1);
	}
}
