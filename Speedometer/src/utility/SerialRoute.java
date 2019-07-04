import java.io.PrintStream;
import java.lang.Thread;
import com.fazecast.jSerialComm.*;

public class SerialRoute {

  public SerialRoute() {
    SerialPort ports[] = SerialPort.getCommPorts();

    for( SerialPort port : ports ) {
      System.out.println( port.getDescriptivePortName() );
    }
  }

  public static void main( String[] args ) {
    new SerialRoute();
  }
}
