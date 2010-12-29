package org.floodping;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;

import java.io.BufferedWriter;
import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.Formatter;
import java.util.HashMap;

import org.jivesoftware.smack.Chat;
import org.jivesoftware.smack.ChatManager;
import org.jivesoftware.smack.ChatManagerListener;
import org.jivesoftware.smack.ConnectionConfiguration;
import org.jivesoftware.smack.MessageListener;
import org.jivesoftware.smack.XMPPConnection;
import org.jivesoftware.smack.XMPPException;
import org.jivesoftware.smack.packet.Message;
import org.jivesoftware.smack.packet.Presence;

public class Main {
	public static HashMap<String, String> aValues = new HashMap<String, String>();

	public static class MessageParrot implements MessageListener, ChatManagerListener {

		private Message msg = new org.jivesoftware.smack.packet.Message("alex@nfsroot.de", org.jivesoftware.smack.packet.Message.Type.chat);

		public void processMessage(Chat chat, Message message) {
			if (message.getType().equals(org.jivesoftware.smack.packet.Message.Type.chat) && message.getBody() != null) {
				try {
					String sResult = "";
					switch (message.getBody().charAt(0)) {
//					case '>':
//						CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier("COM29");
//				        if ( portIdentifier.isCurrentlyOwned() )
//				        {
//				            System.out.println("Error: Port is currently in use");
//				        }
//				        else
//				        {
//				            CommPort commPort = portIdentifier.open(this.getClass().getName(),2000);
//				            
//				            if ( commPort instanceof SerialPort )
//				            {
//				                SerialPort serialPort = (SerialPort) commPort;
//				                serialPort.setSerialPortParams(115200,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
//				                
//				                InputStream in = serialPort.getInputStream();
//				                OutputStream out = serialPort.getOutputStream();
//				               
//				                out.write(message.getBody().getBytes());
//				                Thread.sleep(400);
//				                while(in.available()>0)
//				                {
//				                	sResult += in.read();
//				                }
//				            }
//				            else
//				            {
//				                System.out.println("Error: Only serial ports are handled by this example.");
//				            }
//				        }     
//						msg.setBody(sResult);
//						chat.sendMessage(msg);
					default:
						try {
							sResult += "Wohnen:" + aValues.get("0001") + "°C\r";
						} catch (Exception e) {
						}
						try {
							sResult += "Bad:" + aValues.get("0002") + "°C\r";
						} catch (Exception e) {
						}
						try {
							sResult += "Küche:" + aValues.get("0003") + "°C\r";
						} catch (Exception e) {
						}
						try {
							sResult += "Terasse:" + aValues.get("0004") + "°C\r";
						} catch (Exception e) {
						}
						try {
							sResult += "Schlafen:" + aValues.get("0005") + "°C\r";
						} catch (Exception e) {
						}
						try {
							sResult += "Wasserstand:" + aValues.get("0101") + "cm\r";
						} catch (Exception e) {
						}
						msg.setBody(sResult);
						chat.sendMessage(msg);
					}
				} catch (Exception ex) {
					System.out.println("Failed to send message");
					ex.printStackTrace();
				}
			} else {
				System.out.println("I got a message I didn''t understand");
			}
		}

		@Override
		public void chatCreated(Chat arg0, boolean arg1) {
			arg0.addMessageListener(new MessageParrot());
		}
	}

	public static void main(String[] args) {
		try {
			ConnectionConfiguration connConfig = new ConnectionConfiguration("talk.google.com", 5222, "gmail.com");
			XMPPConnection connection = new XMPPConnection(connConfig);

			try {
				connection.connect();
				System.out.println("Connected to " + connection.getHost());
			} catch (XMPPException ex) {
				// ex.printStackTrace();
				System.out.println("Failed to connect to " + connection.getHost());
				System.exit(1);
			}
			try {
				System.out.println("Logging in as " + args[0]);
				connection.login(args[0], args[1]);
				System.out.println("Logged in as " + connection.getUser());

				Presence presence = new Presence(Presence.Type.available);
				connection.sendPacket(presence);

			} catch (XMPPException ex) {
				// ex.printStackTrace();
				System.out.println("Failed to log in as " + connection.getUser());
				System.exit(1);
			}

			ChatManager chatmanager = connection.getChatManager();
			MessageParrot parrot = new MessageParrot();
			chatmanager.addChatListener(parrot);

			DatagramSocket serverSocket;
			serverSocket = new DatagramSocket(12345);
			byte[] receiveData = new byte[1024];
			boolean bLoop = true;
			while (bLoop) {
				DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
				serverSocket.receive(receivePacket);

				DataInputStream dis = new DataInputStream(new ByteArrayInputStream(receiveData));

				String airid = "XXXX";
				{
					Formatter f = new Formatter();
					airid = f.format("%c%c%c%c", dis.readByte(), dis.readByte(), dis.readByte(), dis.readByte()).toString();
				}

				byte type = dis.readByte();
				switch (type) {
				case 'g': {
					short x1 = (short) ((dis.readByte() & 0xff) | (dis.readByte() & 0xff << 8));
					short y1 = (short) ((dis.readByte() & 0xff) | (dis.readByte() & 0xff << 8));
					short z1 = (short) ((dis.readByte() & 0xff) | (dis.readByte() & 0xff << 8));
					dis.readByte();
					short x2 = (short) ((dis.readByte() & 0xff) | (dis.readByte() & 0xff << 8));
					short y2 = (short) ((dis.readByte() & 0xff) | (dis.readByte() & 0xff << 8));
					short z2 = (short) ((dis.readByte() & 0xff) | (dis.readByte() & 0xff << 8));
					if (x1 != x2 || y1 != y2 || z1 != z2) {
						System.out.println("error: " + x1 + "!=" + x2 + " || " + y1 + "!=" + y2 + " || " + z1 + "!=" + z2 + "");
						break;
					}
					double xx = (x1 * 1.0) / 4.6;
					double yx = (y1 * 1.0) / 4.6;
					double zx = (z1 * 1.0) / 4.6;

					double x = xx;
					int res = x < -9 ? (x < -13 ? 2 : 1) : 0;
					System.out.println("" + airid + " g:" + xx + "," + yx + "," + zx);

					aValues.put(airid, new Integer(new Double(x).intValue()).toString());

					try {
						Formatter cmdf = new Formatter();
						String command = cmdf.format("/usr/bin/submit_check_result ned %s %d %+3.1fcm", airid, res, x).toString();
						System.out.println("" + command);
						Runtime.getRuntime().exec(command);
					} catch (Exception e) {
						System.err.println("Errore: " + e.getMessage());
					}

					try {
						String sFile = "/tmp/airid" + airid;
						FileWriter fstream = new FileWriter(sFile);
						BufferedWriter out = new BufferedWriter(fstream);
						Formatter cmdf = new Formatter();
						out.write(cmdf.format("%+3.1f", x).toString());
						out.close();
					} catch (Exception e) {// Catch exception if any
						System.err.println("Error: " + e.getMessage());
					}

				}
					break;
				case 'T': {
					byte x1 = dis.readByte();
					byte x2 = dis.readByte();
					short temp = (short) ((x2 & 0xff) << 8 | (x1 & 0xff));
					System.out.println("T:" + temp);
					double t = (temp * 1.0) / 16;
					Formatter f = new Formatter();
					String id = f.format("%02x-%02x %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", x1, x2, dis.readByte(), dis.readByte(), dis.readByte(), dis.readByte(), dis.readByte(), dis.readByte(), dis.readByte(), dis.readByte()).toString();
					x1 = dis.readByte();
					x2 = dis.readByte();
					short temp2 = (short) ((x2 & 0xff) << 8 | (x1 & 0xff));
					if (temp != temp2) {
						break;
					}

					System.out.println("" + airid + " T:" + t + "," + id);
					aValues.put(airid, new Double(t).toString());
					try {
						Formatter cmdf = new Formatter();
						String command = cmdf.format("/usr/bin/submit_check_result ned %s 0 %+3.1fdegC", airid, t).toString();
						System.out.println("" + command);
						Runtime.getRuntime().exec(command);
					} catch (Exception e) {
						System.err.println("Errore: " + e.getMessage());
					}

					try {
						String sFile = "/tmp/airid" + airid;
						FileWriter fstream = new FileWriter(sFile);
						BufferedWriter out = new BufferedWriter(fstream);
						Formatter cmdf = new Formatter();
						out.write(cmdf.format("%+3.1f", t).toString());
						out.close();
					} catch (Exception e) {// Catch exception if any
						System.err.println("Error: " + e.getMessage());
					}
				}
					break;
				default:
					String sentence = new String(receivePacket.getData());
					System.out.println("RECEIVED: " + sentence);
				}

			}
			connection.disconnect();
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

}
