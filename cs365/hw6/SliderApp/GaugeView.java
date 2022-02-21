package SliderApp;
import javax.swing.JPanel;
import java.awt.Component;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import javax.swing.BorderFactory;
import java.awt.Graphics;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Point;

/* Nicholas Creech
 * CS365 HW6
 * 03/24/21
 */ 

class GaugeView extends JPanel implements AppView
{
	private AppModel model;
	private boolean canDrag;
	private Point lineEnd;

	public GaugeView(AppModel parent)
	{
		model = parent;
		model.addObserver(this);
		setBorder(BorderFactory.createLineBorder(Color.BLACK));
		lineEnd = new Point();
		canDrag = false;

		//Detect mouse press and release
		addMouseListener(new MouseAdapter() 
		{
			@Override
			public void mousePressed(MouseEvent e)
			{
				double dist;
		
				//Calculate distance from mouse to end of needle
				dist = Math.sqrt(Math.pow(e.getX() - lineEnd.getX(), 2) + Math.pow(e.getY() - lineEnd.getY(), 2));
		
				//Only drag if mouse is 10 pixels from needle
				if(dist <= 10)
					canDrag = true;
			}

			@Override
			public void mouseReleased(MouseEvent e) { canDrag = false; }
		});

		//Detect mouse drag
		addMouseMotionListener(new MouseMotionAdapter()
		{
			@Override
			public void mouseDragged(MouseEvent e)
			{
				if(canDrag)
				{
					double angle;
					int newVal, normalMax;
					
					//Find angle of current mouse position and use to calculate new slider value
					angle = Math.atan2(getHeight() - e.getY(), e.getX() - getWidth() / 2);
					normalMax = model.getMaxVal() - model.getMinVal();
					newVal = (int)(Math.toDegrees(angle) / 180 * normalMax + model.getMinVal());

					//Restrict needle between max and min
					if(Math.toDegrees(angle) < 0 || Math.toDegrees(angle) > 180)
						return;

					model.setSliderVal(newVal);
				}
			}
		});
	}

	@Override
	public Dimension getPreferredSize()
	{
		return new Dimension(200, 100);
	}

	@Override
	public void paintComponent(Graphics g)
	{
		int radius, normalMax;
		double angle;

		super.paintComponent(g);
	
		g.setColor(Color.ORANGE);

		//Fill arc with radius equal to panel height
		if(getHeight() * 2 <= getWidth())
		{
			radius = getHeight();
			g.fillArc(getWidth() / 2 - radius, 0, radius * 2, radius * 2, 0, 180);	
		}

		//Fill arc with radius equal to panel width / 2
		else
		{
			radius = getWidth() / 2;
			g.fillArc(getWidth() / 2 - radius, 0, radius * 2, radius * 2, 0, 180);
		}

		g.setColor(Color.BLUE);
			
		//Find angle of needle based on slider value
		normalMax = model.getMaxVal() - model.getMinVal();
		angle = (double)(model.getSliderVal() - model.getMinVal()) / (double)normalMax * 180;
		lineEnd.setLocation(getWidth() / 2 + (int)(radius * Math.cos(Math.toRadians(angle))), 
		radius - (int)(radius * Math.sin(Math.toRadians(angle))));
		
		//Draw the needle
		g.drawLine(getWidth() / 2, radius, (int)lineEnd.getX(), (int)lineEnd.getY());
	}

	//Repaint gauge and needle on model update
	public void update(int oldVal)
	{
		repaint();
	}
}
