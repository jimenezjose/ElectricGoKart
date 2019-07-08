import java.io.PrintStream;
import java.lang.Thread;
import com.fazecast.jSerialComm.*;
import java.util.ArrayList;
import java.util.Vector;
import java.util.EventListener;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Singleton design
 * TODO
 */
public class SerialRoute implements SerialPortMessageListener {

  private static final SerialRoute instance = new SerialRoute();
  private ArrayList<ActionListener> listenerList;
  private SerialPort port = null; 

  private SerialRoute() {
    listenerList = new ArrayList<ActionListener>();
  }

  public static SerialRoute getInstance() {
    return instance;
  }

  public boolean connectTo( String selectedPortName ) {
    int index = 0;
    for( String portName : getPortList() ) {
      /* all bindable ports */
      if( selectedPortName.equals(portName) ) {
	/* connect to port and listen to serial data asynchronously */
        disconnect();
        port = SerialPort.getCommPorts()[ index ];
	port.openPort();
	port.addDataListener( this );
        return true;
      }
      index++;
    }
    return false;
  }

  public void disconnect() {
    if( port != null && port.isOpen() ) {
      /* clean-up previous port connection */
      port.removeDataListener();
      port.closePort();
    }
  }

  public Vector<String> getPortList() {
    SerialPort commPortList[] = SerialPort.getCommPorts();
    Vector<String> portList = new Vector<String>();

    for( SerialPort commPort : commPortList ) {
      /* name port as described by the host system */
      portList.add( commPort.getSystemPortName() );
    }
    return portList;
  }
  
  private void fireActionPerformed( ActionEvent evt ) {
    for( ActionListener listener : listenerList ) {
      /* notify all action listeners of new event */
      listener.actionPerformed( evt );
    }
  }

  public void addActionListener( ActionListener listener ) {
    listenerList.add( listener ); 
  }

  public void removeActionListener( ActionListener listener ) {
    listenerList.remove( listener );
  }

  @Override
  public boolean delimiterIndicatesEndOfMessage() { 
    return true; 
  }

  @Override
  public void serialEvent( SerialPortEvent event ) {
    String message = "";
    for( byte character : event.getReceivedData() ) {
      /* convert byte data to character data */
      if( !isInDelimiter(character) ) {
        /* character is not in message delimiter */
        message += (char) character;
      }
    }
    /* fire new event for new message detected */
    fireActionPerformed( new SerialRouteEvent(this, message) );
  }
  
  @Override
  public int getListeningEvents() { 
    return SerialPort.LISTENING_EVENT_DATA_RECEIVED; 
  }

  @Override
  public byte[] getMessageDelimiter() { 
    return new byte[] { '\r', '\n' }; 
  }

  public boolean isInDelimiter( byte character ) {
    for( byte delimiter : getMessageDelimiter() ) {
      if( character == delimiter ) {
        return true;
      }
    }
    return false;
  }
  
  public int getAvailablePortCount() {
    return getPortList().size();
  }
}
