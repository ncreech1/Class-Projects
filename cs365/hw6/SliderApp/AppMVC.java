package SliderApp;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JLabel;
import java.awt.BorderLayout;
import javax.swing.SwingConstants;

/* Nicholas Creech
 * CS365 HW6
 * 03/24/21
 */

public class AppMVC extends JFrame
{
	private AppModel model;

	public AppMVC(String args[])
	{
		initComponents(args);
	}

	//Sets up the application components
	private void initComponents(String args[])
	{
		int minVal = 0;
		int maxVal = 0;
		int startVal = 0;
		int increment = 0;
		JLabel textView;
		JPanel sliderView;
		JPanel gaugeView;

		//Get the command line args
		try
		{
			if(args.length != 4)
				throw new NumberFormatException();

			minVal = Integer.parseInt(args[0]);
			maxVal = Integer.parseInt(args[1]);
			startVal = Integer.parseInt(args[2]);
			increment = Integer.parseInt(args[3]);

			if(minVal >= maxVal)
				throw new NumberFormatException();
			if(startVal > maxVal || startVal < minVal)
				startVal = minVal;
		}

		catch(NumberFormatException e)
		{
			System.out.println("Usage: java SliderApp.AppMVC min max start increment");
			System.out.println("Arguments must be four integers with max > min");
			System.exit(1);
		}

		model = new AppModel(minVal, maxVal, startVal, increment);
		textView = new TextView(model);
		textView.setHorizontalAlignment(SwingConstants.CENTER);
		sliderView = new SliderView(model);
		gaugeView = new GaugeView(model);

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		getContentPane().add(textView, BorderLayout.PAGE_START);
		getContentPane().add(sliderView, BorderLayout.CENTER);
		getContentPane().add(gaugeView, BorderLayout.PAGE_END);

		pack();	
	}

	public static void main(String args[])
	{
		//Start application thread
		java.awt.EventQueue.invokeLater(new Runnable()
		{
			public void run()
			{
				new AppMVC(args).setVisible(true);
			}
		});
	}
}
