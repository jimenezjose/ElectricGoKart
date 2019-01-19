import java.awt.*;
import java.awt.geom.*;
import javax.swing.*;

public class Speedometer extends JFrame {

  private static final Color LIGHT_BLACK     = new Color( 32, 32, 32 );
  private static final Color NEON_GREEN      = new Color( 0, 128, 0 );
  private static final Color DARK_NEON_GREEN = new Color( 0, 100, 0 );
  private static final Color METALLIC_BLACK  = new Color( 50, 50, 50 );
  private static final Color NEON_RED        = new Color( 255, 0, 0 );
  private static final Color DARK_NEON_RED   = new Color( 200, 0, 0 );

  private static final String TRANSMISSION_LABEL = "P  R  D";
  private static final String MPH_LABEL          = "MPH";

  public static final int MAX_SPEED = 120;
  public static final int MIN_SPEED = 0;

  public static final int MIN_ANGLE  = -150;
  public static final int MAX_ANGLE  = 150;

  private int prevTheta = MIN_ANGLE;

  public RoundRectangle2D needle;
  public Point center;

  /* speedometer dimensions */
  private int outer_diameter;
  private int outer_radius;
  private int diameter;
  private int radius;   
  private int rim_diameter; 
  private int rim_radius;
  private int inner_diameter; 
  private int inner_radius; 
  private int needle_offset;
  private int needle_width;

  /* speedometer label dimensions */
  private int tensWidth; 
  private int tensOffset;
  private double arcAngle;

  /* transmission specs */
  private Font transmissionFont;
  private int transmission_width;
  private int MPH_offset;

  /**
   *  Sets up the window behavior and graphics of the Speedometer GUI.
   */
  public Speedometer() {
    super( "Speedometer Graphics" );
    setSize( 400, 400 );
    setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
    setBackground( Color.BLACK );
    getContentPane().setBackground( Color.BLACK );
    setResizable( true );
    setVisible( true );
    paint( getGraphics() );
  }

  /**
   *  Sets up a new graphical inerface of the Speedometer after the canvas is
   *  resized.
   *  @param g An abstract canvas for animations.
   *  @return nothing.
   */
  @Override
  public void paint( Graphics g ) {
    super.paint( g );
    drawSpeedometer( g );
  }

  /**
   *  Draws the speedometer design without needle.
   *  @param g An abstract canvas for animations.
   *  @return nothing.
   */
  private void drawSpeedometer( Graphics g ) {
    center = new Point( getWidth() / 2, getHeight() / 2 );

    outer_diameter = (int)(double)(0.75 * Math.min( getWidth(), getHeight() ));
    outer_radius   = (int)(double)(0.5 * outer_diameter);
    diameter       = (int)(double)(0.7 * Math.min( getWidth(), getHeight() ));
    radius         = (int)(double)(0.5 * diameter);
    rim_diameter   = (int)(double)(0.69 * Math.min( getWidth(), getHeight() ));
    rim_radius     = (int)(double)(0.5 * rim_diameter);
    inner_diameter = (int)(double)(0.05 * Math.min( getWidth(), getHeight() ));
    inner_radius   = (int)(double)(0.5 * inner_diameter);
    needle_offset  = (int)(double)(0.1 * rim_radius);
    needle_width   = (int)(double)(0.15 * inner_radius);
    tensWidth      = (int)(double)(0.5 * inner_radius);
    tensOffset     = (int)(double)(0.5 * tensWidth);
    int markHeight       = (int)(double)(0.4 * needle_offset);
    int markWidthOffset  = (int)(double)(0.2 * markHeight);
    int MPH_fontsize     = (int)(double)(0.06 * rim_radius);
    MPH_offset           = (int)(double)(0.25 * rim_radius);
    Font speedometerFont = new Font( Font.SANS_SERIF, Font.PLAIN, MPH_fontsize );
    transmissionFont     = new Font( Font.SANS_SERIF, Font.BOLD, (int)(1.5 * MPH_fontsize) );

    if( needle_width == 0 ) needle_width = 1;

    /* larger speedometer background */
    g.setColor( LIGHT_BLACK );
    g.fillOval( center.x - outer_radius, center.y - outer_radius, outer_diameter, outer_diameter );

    /* green rim arc */
    g.setColor( NEON_GREEN );
    g.fillArc( center.x - radius, center.y - radius, diameter, diameter, 300, MAX_ANGLE - MIN_ANGLE );
    g.setColor( LIGHT_BLACK );
    g.fillOval( center.x - rim_radius, center.y - rim_radius, rim_diameter, rim_diameter );

    /* speedometer needles */
    needle = new RoundRectangle2D.Double( center.x, center.y - rim_radius + needle_offset, needle_width, rim_radius - needle_offset - inner_radius, 6, 6 );
    rotateNeedle( prevTheta - MIN_ANGLE );

    /* speedometer needle origin */
    g.setColor( METALLIC_BLACK );
    g.fillOval( center.x - inner_radius, center.y - inner_radius, inner_diameter, inner_diameter );
    g.setColor( Color.BLACK );
    g.drawOval( center.x - inner_radius, center.y - inner_radius, inner_diameter, inner_diameter );

    /* speedometer labels */
    Rectangle2D tensMark = new Rectangle2D.Double( center.x, center.y - rim_radius, needle_width + markWidthOffset, 2 * markHeight );
    Rectangle2D fiveMark = new Rectangle2D.Double( center.x, center.y - rim_radius, needle_width + markWidthOffset, markHeight );
    Rectangle2D onesMark = new Rectangle2D.Double( center.x, center.y - rim_radius, needle_width / 2, markHeight );

    arcAngle = MAX_ANGLE - MIN_ANGLE - Math.toDegrees( (double)(needle_width + 2) / rim_radius );
    double increment = arcAngle / MAX_SPEED;
    int speed = 0;
    double theta = MIN_ANGLE;

    /* draw dash marks on Speedometer */
    drawSpeedMark( tensMark, theta, NEON_GREEN );
    drawSpeedDigit( speed, theta, NEON_GREEN );
    theta += increment;
    speed++;

    for( ; theta < MAX_ANGLE; theta += increment ) {
      /* assign different sized dashes for speed marks */
      Rectangle2D speedMark;
      Color markColor = NEON_GREEN;

      if( speed % 10 == 0 ) speedMark = tensMark;
      else if( speed % 5 == 0 ) speedMark = fiveMark;
      else speedMark = onesMark;
      
      drawSpeedMark( speedMark, theta, NEON_GREEN );
      if( speed % 10 == 0 ) drawSpeedDigit( speed, theta, NEON_GREEN );
      speed++;
    }

    /* draw MPH and Transmission labels */
    g.setFont( speedometerFont );
    g.setColor( Color.WHITE );
    int MPH_width = g.getFontMetrics().stringWidth( MPH_LABEL );
    g.drawString( MPH_LABEL, (int) (center.x - MPH_width / 2.0), center.y + MPH_offset );
    g.setFont( transmissionFont );
    transmission_width = g.getFontMetrics().stringWidth( TRANSMISSION_LABEL );
    /* Assume car is in park */
    setInPark(); // TODO make sure this is not left. breaks when window resize and transmission set to park compliment
  }

  private void drawSpeedDigit( int speed, double theta, Color color ) {
    double needle_radius = rim_radius - needle_offset; 
    double x = center.x + needle_radius * Math.cos( Math.toRadians(-theta + 90) );
    double y = center.y - needle_radius * Math.sin( Math.toRadians(-theta + 90) );
    //double dx = 
    //double dy
    Graphics g = getGraphics();
    g.setFont( transmissionFont );
    g.setColor( color );
    g.drawString( Integer.toString(speed), (int)(x), (int)(y) );
  }

  /**
   *  Draws an individual rectangular speedMark on the speedometer.
   *  @param rectangle rectangle that will be drawn.
   *  @param theta angle that the rectangle will be placed. [-150, 150]. 
   *  @param color color of the rectangle.
   *  @return nothing.
   */
  private void drawSpeedMark( Rectangle2D rectangle, double theta, Color color ) {
    Graphics2D g2d = (Graphics2D) getGraphics();
    AffineTransform old = g2d.getTransform();
    g2d.setColor( color );
    /* rotate needle around origin */
    g2d.rotate( Math.toRadians(theta), center.x, center.y );
    g2d.fill( rectangle );
    g2d.setTransform( old );
  }


  /**
   *  Removes previous needle display from speedometer.
   *  @return nothing.
   */
  public void clearSpeedometer() {
    rotateNeedle( prevTheta, LIGHT_BLACK );
    /* repaint needle origin */
    Graphics g2d = getGraphics();
    g2d.setColor( METALLIC_BLACK );
    g2d.fillOval( center.x - inner_radius, center.y - inner_radius, inner_diameter, inner_diameter );
    g2d.setColor( Color.BLACK );
    g2d.drawOval( center.x - inner_radius, center.y - inner_radius, inner_diameter, inner_diameter );
  }

  /**
   *  Rotates speedometer needle.
   *  @param theta Degree needle makes with 0mph. Angle range [0, 300]. 
   *               Angles out of bounds are corrected internally. 
   *  @return nothing.
   */
  private void rotateNeedle( int theta ) {
    theta = (theta - MAX_ANGLE) % (MAX_ANGLE + 1);
    rotateNeedle( theta, NEON_RED );

    if( prevTheta != theta ) {
      /* remove previous needle */
      clearSpeedometer();
      prevTheta = theta;
      rotateNeedle( theta, NEON_RED );
      delay( 100 ); 
    }

  }

  /**
   *  Rotates needle with a specific needle color.
   *  @param theta Degree needle makes with 0mph mark. Angle range [150, -150].
   *               Angles out of bounds are corrected internally. 
   *  @param color Color of needle.
   *  @return nothing.
   */
  private void rotateNeedle( int theta, Color color ) {
    Graphics2D g2d = (Graphics2D) getGraphics();
    AffineTransform old = g2d.getTransform();
    g2d.setColor( color );
    /* rotate needle around origin */
    g2d.rotate( Math.toRadians(theta), center.x, center.y );
    g2d.fill( needle );
    g2d.setTransform( old );
  }

  /**
   * TODO 
   */
  private void setTransmission( char transmission ) {
    String label = TRANSMISSION_LABEL;

    if( transmission != 'P' && transmission != 'R' && transmission != 'D' ) {
      /* invlid input */
      return;
    }
    
    Graphics g = getGraphics();
    g.setFont( transmissionFont );
    int x_startPoint = (int)(center.x - transmission_width / 2.0); 
    int x_offset = g.getFontMetrics().stringWidth( label.substring(0, label.indexOf(transmission)) );
    int y_offset = center.y + 3 * MPH_offset;

    g.setColor( Color.WHITE );
    g.drawString( label, x_startPoint, y_offset );
    g.setColor( DARK_NEON_RED );
    g.drawString( Character.toString(transmission), x_startPoint + x_offset, y_offset );

  }

  /**
   * TODO 
   */
  public void setInDrive() {
    setTransmission( 'D' );
  }

  /**
   * TODO 
   */
  public void setInPark() {
    setTransmission( 'P' );
  }

  /**
   * TODO 
   */
  public void setInReverse() {
    setTransmission( 'R' );
  }

  /**
   *  Stops program execution for smother motion pictures.
   *  @param ms time in milliseconds
   *  @return nothing.
   */
  public void delay( long ms ) {

    try {
      Thread.sleep( ms );
    }
    catch( InterruptedException e ) {
      e.printStackTrace();
    }

  }

  public static void main( String[] args ) {
    Speedometer speedometer = new Speedometer();

    boolean increasing = true;
    int theta = 0;
    while( true ) {
      speedometer.rotateNeedle( theta );

      if( theta == 0 ) speedometer.setInPark();
      else if( theta == 150 ) speedometer.setInReverse();
      else if( theta == 300 ) speedometer.setInDrive();

      if( theta ==  300 )   increasing = false;   
      else if( theta == 0 ) increasing = true;

      if( increasing ) theta += 1;
      else theta -= 1;

      theta = theta % 301;
    }

  }

}
