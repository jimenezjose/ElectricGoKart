import java.awt.event.ActionEvent;

public class SerialRouteEvent extends ActionEvent {
  SerialRoute port;
  String data;

  public SerialRouteEvent( SerialRoute port, String data ) {
    super( port, ActionEvent.ACTION_PERFORMED, "Serial Communication" );
    this.port = port;
    this.data = data;
  }

  public String getReceivedMessage() {
    return data;
  }
}
