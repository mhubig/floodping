package org.floodping;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.Formatter;

public class Main {

        public static void main(String[] args) {
                try {
                        DatagramSocket serverSocket;
                        serverSocket = new DatagramSocket(12345);
                        byte[] receiveData = new byte[1024];
                        while (true) {
                                DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
                                serverSocket.receive(receivePacket);


                                DataInputStream dis = new DataInputStream(new ByteArrayInputStream(receiveData));


                                String airid = "XXXX";
                                {
                                        Formatter f = new Formatter();
                                        airid = f.format("%c%c%c%c", dis.readByte(), dis.readByte(), dis.readByte(), dis.readByte()).toString();
                                }

                                byte type = dis.readByte();
                                switch(type)
                                {
                                case 'g':
                                        {
                                                short x1 = (short)(( dis.readByte() & 0xff ) | (dis.readByte() & 0xff  << 8));
                                                short y1 = (short)(( dis.readByte() & 0xff ) | (dis.readByte() & 0xff  << 8));
                                                short z1 = (short)(( dis.readByte() & 0xff ) | (dis.readByte() & 0xff << 8));
                                                dis.readByte();
                                                short x2 = (short)(( dis.readByte() & 0xff ) | (dis.readByte() & 0xff << 8 ));
                                                short y2 = (short)(( dis.readByte() & 0xff ) | (dis.readByte() & 0xff << 8 ));
                                                short z2 = (short)(( dis.readByte() & 0xff ) | (dis.readByte() & 0xff << 8 ));
                                                if(x1!=x2 || y1!=y2 || z1!=z2)
                                                {
                                                        System.out.println("error: " + x1 + "!=" + x2 + " || " + y1 + "!=" + y2 + " || " + z1 + "!=" + z2 + "");
                                                        break;
                                                }
                                                double x = (x1 * 1.0) / 4.6;
                                                double y = (y1 * 1.0) / 4.6;
                                                double z = (z1 * 1.0) / 4.6;
                                                int res = z < -9 ? (z < -13 ? 2 : 1) : 0;
                                                System.out.println("" + airid + " g:" + x + "," + y + "," +z);
                                                Formatter cmdf = new Formatter();
                                                String command = cmdf.format("/usr/bin/submit_check_result ned %s %d %+3.1fcm", airid, res, z).toString();
                                                System.out.println("" + command);
                                                Runtime.getRuntime().exec(command);
                                        }
                                        break;
                                case 'T':
                                        {
                                                byte x1 = dis.readByte();
                                                byte x2 = dis.readByte();
                                                short temp = (short)((x2 & 0xff) << 8 | (x1 & 0xff));
                                                System.out.println("T:" + temp);
                                                double t = (temp * 1.0) / 16;
                                                Formatter f = new Formatter();
                                                String id = f.format("%02x-%02x %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
                                                                x1,x2,dis.readByte(), dis.readByte(), dis.readByte(),dis.readByte(),
                                                                dis.readByte(), dis.readByte(), dis.readByte(), dis.readByte()
                                                                ).toString();
                                                System.out.println("" + airid + " T:" + t + "," + id);
                                                Formatter cmdf = new Formatter();
                                                String command = cmdf.format("/usr/bin/submit_check_result ned %s 0 %+3.1fdegC", airid, t).toString();
                                                System.out.println("" + command);
                                                Runtime.getRuntime().exec(command);
                                        }
                                        break;
                                default:
                                        String sentence = new String(receivePacket.getData());
                                        System.out.println("RECEIVED: " + sentence);
                                }

                        }
                } catch (Exception e) {
                        e.printStackTrace();
                }

        }

}

