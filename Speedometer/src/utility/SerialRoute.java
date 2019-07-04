import java.io.PrintStream;
import java.lang.Thread;
import com.fazecast.jSerialComm.*;
import java.util.LinkedList;
import java.util.Vector;

/**
 * Singleton design
 * TODO
 */
public class SerialRoute {
  private static final SerialRoute instance = new SerialRoute();
  private SerialPort port = null;

  private SerialRoute() {
  }

  public static SerialRoute getInstance() {
    return instance;
  }

  public Vector<String> getPortList() {
    SerialPort commPortList[] = SerialPort.getCommPorts();
    Vector<String> portLista = new Vector<String>();

    for( SerialPort commPort : commPortList ) {
      portLista.add( commPort.getSystemPortName() );
    }
    return portLista;
  }

  public boolean connectTo( String selectedPortName ) {
    int index = 0;
    for( String portName : getPortList() ) {
      if( selectedPortName.equals(portName) ) {
        if( port != null && port.isOpen() ) port.closePort();
        port = SerialPort.getCommPorts()[ index ];
	port.openPort();
	System.err.println( port.getBaudRate() );
        return true;
      }
      index++;
    }

    return false;
  }

  public void dumpData() {
    if( port == null || !port.isOpen() ) return;
    try {
   while (true)
   {
      while (port.bytesAvailable() == 0)
         Thread.sleep(20);

      byte[] readBuffer = new byte[port.bytesAvailable()];
      int numRead = port.readBytes(readBuffer, readBuffer.length);
      System.out.println("Read " + numRead + " bytes.");
   }
} catch (Exception e) { e.printStackTrace(); }
port.closePort();
  }
}
